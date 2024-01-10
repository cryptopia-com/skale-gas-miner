#include "pch.h"
#include <memory>
#include <algorithm>
#include "helpers/convert.h"
#include "helpers/hashing.h"
#include "cryptopp/hex.h"
#include "cryptopp/osrng.h"
#include "miner.h"

namespace Cryptopia 
{
    // Singleton instance of the miner.
    std::mutex SkaleGasMiner::instanceMutex_;
    std::unique_ptr<SkaleGasMiner> SkaleGasMiner::instance_;

    /**
     * Retrieves the singleton instance of SkaleGasMiner.
     */
    SkaleGasMiner& SkaleGasMiner::GetInstance() 
    {
        std::lock_guard<std::mutex> lock(instanceMutex_);
        if (!instance_) 
        {
            instance_ = std::unique_ptr<SkaleGasMiner>(
                new SkaleGasMiner());
        }
        return *instance_;
    }

    /**
     * Constructor for the SkaleGasMiner class.
     * As part of the singleton pattern, this constructor is private to prevent direct instantiation.
     */
    SkaleGasMiner::SkaleGasMiner()
        : isMining_(false), hashRate_(0), resultFound_(false) {}

    /**
     * Destructor for cleaning up resources.
     */
    SkaleGasMiner::~SkaleGasMiner() 
    {
        Stop();
    }

    /**
     * Sets the result of the mining operation.
     *
     * @param value The result string to be set.
     */
    void SkaleGasMiner::SetResult(const std::string& value) 
    {
        std::lock_guard<std::mutex> lock(resultMutex_);
        result_ = value;
    }

    /**
     * Retrieves the result of the mining operation.
     *
     * @return The result string.
     */
    std::string SkaleGasMiner::GetResult() const 
    {
        std::lock_guard<std::mutex> lock(resultMutex_);
        return result_;
    }

    /**
     * Checks if the mining process is currently active.
     *
     * @return A boolean value indicating whether mining is currently in progress.
     */
    bool SkaleGasMiner::IsMining() const 
    {
        return isMining_;
    }

    /**
     * Starts the mining process.
     *
     * @param amount The amount of gas to mine.
     * @param fromAddress The address initiating the mining process.
     * @param nonce A nonce value for the mining process.
     * @param difficulty The mining difficulty level.
     * @param hashRateCallback A callback function to report the hash rate.
     * @param resultCallback A callback function to report the result of mining.
     * @param maxThreads (Optional) The maximum number of threads to use for mining. Default is 0.
     */
    void SkaleGasMiner::MineGas(const unsigned long long amount, const std::string& fromAddress, const unsigned long long nonce, const unsigned int difficulty, HashRateDelegate hashRateCallback, ResultDelegate resultCallback, const unsigned int maxThreads)
    {
        if (isMining_)
        {
            resultCallback(false, "", "Already mining");
            return;
        }

        isMining_ = true;
        hashRate_ = 0;
        resultFound_ = false;
        SetResult("");

        CryptoPP::Integer numerator((CryptoPP::Integer::Power2(256) - 1) / difficulty);
        CryptoPP::Integer nonceHash(GetSoliditySha3(nonce).c_str());
        CryptoPP::Integer fromAddressHash(GetSoliditySha3(HexToByteArray(fromAddress)).c_str());
        CryptoPP::Integer precomputed = nonceHash ^ fromAddressHash;

        stopSource_ = std::stop_source{};
        unsigned int threadCount = std::thread::hardware_concurrency();
        if (maxThreads > 0 && maxThreads < threadCount) 
        {
            threadCount = maxThreads;
        }

        miningThreads_.resize(threadCount);
        auto localHashRates = std::make_unique<std::atomic<unsigned long long>[]>(threadCount);

        for (unsigned int i = 0; i < threadCount; i++) 
        {
            miningThreads_[i] = std::thread(
                &SkaleGasMiner::DoMineGas, this, amount, numerator, precomputed, std::ref(localHashRates[i]), stopSource_.get_token());
        }

        std::thread measureHashRateThread = std::thread(
            &SkaleGasMiner::SetHashRate, this, localHashRates.get(), threadCount, hashRateCallback, stopSource_.get_token());

        for (auto& thread : miningThreads_) 
        {
            if (thread.joinable()) 
            {
                thread.join();
            }
        }

        if (measureHashRateThread.joinable()) 
        {
            measureHashRateThread.join();
        }

        isMining_ = false;
        hashRate_ = 0;

        std::string result = GetResult();
        if (result.empty()) 
        {
            resultCallback(false, "", "Aborted");
        }
        else 
        {
            resultCallback(true, result.c_str(), "");
        }
    }

    /**
     * Performs the mining operation in a single thread.
     * This function is called by each mining thread to execute the mining algorithm.
     *
     * @param amount The target amount of gas for the mining operation.
     * @param numerator The numerator for the mining calculation.
     * @param precomputed Precomputed value for the mining algorithm.
     * @param localHashRate Atomic variable to store the hash rate calculated by this thread.
     * @param stopToken Token to signal the thread to stop running.
     */
    void SkaleGasMiner::DoMineGas(const unsigned long long amount, CryptoPP::Integer numerator, CryptoPP::Integer precomputed, std::atomic<unsigned long long>& localHashRate, std::stop_token stopToken) 
    {
        CryptoPP::AutoSeededRandomPool rng;
        unsigned char candidateBytes[32];

        while (!stopToken.stop_requested())
        {
            // Generate randomness
            rng.GenerateBlock(candidateBytes, sizeof(candidateBytes));

            std::string hexString;
            CryptoPP::ArraySource(candidateBytes, sizeof(candidateBytes), true,
                new CryptoPP::HexEncoder(
                    new CryptoPP::StringSink(hexString)));

            // Convert to lowercase
            std::transform(hexString.begin(), hexString.end(), hexString.begin(), ::tolower);

            // Calculate externalGas
            CryptoPP::Integer candidate = HexToInteger(GetSoliditySha3(HexToInteger(hexString)));
            CryptoPP::Integer result = precomputed ^ candidate;
            CryptoPP::Integer externalGas = numerator / result;

            // Update hash rate
            localHashRate++;

            // Check if found
            if (externalGas >= amount)
            {
                if (!resultFound_)
                {
                    resultFound_ = true;
                    SetResult(IntegerToString(HexToInteger(hexString)));
                    stopSource_.request_stop();
                }

                break;
            }
        }
    }

    /**
     * Retrieves the current hash rate of the mining process.
     *
     * @return The current hash rate expressed as an unsigned long long integer.
     */
    unsigned long long SkaleGasMiner::GetHashRate() const
    {
        return hashRate_;
    }

    /**
     * Calculates and sets the hash rate of the mining operation.
     * This function should be run in a separate thread to continuously update the hash rate.
     *
     * @param localHashRates Array of hash rates from each mining thread.
     * @param threadCount The number of threads involved in the mining process.
     * @param callback The callback function to report the hash rate.
     * @param stopToken Token to signal the thread to stop running.
     */
    void SkaleGasMiner::SetHashRate(std::atomic<unsigned long long>* localHashRates, unsigned int threadCount, HashRateDelegate callback, std::stop_token stopToken) 
    {
        while (!stopToken.stop_requested()) 
        {
            unsigned long long hashRate = 0;
            for (unsigned int i = 0; i < threadCount; i++) 
            {
                hashRate += localHashRates[i].load();
                localHashRates[i] = 0;
            }

            hashRate_.store(hashRate);
            callback(hashRate);
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    /**
     * Stops all active mining threads.
     * This function signals all threads to stop and resets the mining state.
     */
    void SkaleGasMiner::Stop() 
    {
        if (!isMining_) 
        {
            return;
        }

        stopSource_.request_stop();
        isMining_ = false;
        hashRate_ = 0;
    }
}
