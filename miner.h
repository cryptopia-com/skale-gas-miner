#pragma once
#include <string>
#include <mutex>
#include <atomic>
#include <vector>
#include <thread>
#include <stop_token>
#include "cryptopp/integer.h"

#ifdef SKALEGASMINER_EXPORTS
#define SKALEGASMINER_API __declspec(dllexport)
#else
#define SKALEGASMINER_API __declspec(dllimport)
#endif

namespace Cryptopia 
{
    // Delegate type for reporting hash rate
    typedef void(_stdcall* HashRateDelegate) (unsigned long long);

    // Delegate type for notifying the result of mining
    typedef void(_stdcall* ResultDelegate) (bool, const char*, const char*);

    /**
     * Solution developped by Cryptopia for efficient mining of SKALE gas 
     * within Unity3D.
     * 
     * Features:
     * - Multi-threaded mining
     * - Adjustable number of threads
     * - Hash rate reporting
     * - Mid-mining stop
     * 
     * @author Frank Bonnet
     * @see https://cryptopia.com/team/frank-bonnet/
     * @version 1.0
     */
    class SKALEGASMINER_API SkaleGasMiner {
    public:

        /**
         * Retrieves the singleton instance of SkaleGasMiner.
         */
        static SkaleGasMiner& GetInstance();

        /**
         * Destructor for cleaning up resources.
         */
        ~SkaleGasMiner();

        /**
         * Checks if the mining process is currently active.
         *
         * @return A boolean value indicating whether mining is currently in progress.
         */
        bool IsMining() const;

        /**
         * Retrieves the current hash rate of the mining process.
         *
         * @return The current hash rate expressed as an unsigned long long integer.
         */
        unsigned long long GetHashRate() const;

        /**
         * Starts the mining process with the given parameters.
         *
         * @param amount The amount of gas to mine.
         * @param fromAddress The address initiating the mining process.
         * @param nonce A nonce value for the mining process.
         * @param difficulty The mining difficulty level.
         * @param hashRateCallback A callback function to report the hash rate.
         * @param resultCallback A callback function to report the result of mining.
         * @param maxThreads (Optional) The maximum number of threads to use for mining. Default is 0.
         */
        void MineGas(const unsigned long long amount, const std::string& fromAddress, const unsigned long long nonce, const unsigned int difficulty, HashRateDelegate hashRateCallback, ResultDelegate resultCallback, const unsigned int maxThreads = 0);
        
        /**
         * Stops the mining process.
         */
        void Stop();
        
    private:

        // Singleton instance of the miner.
        static std::mutex instanceMutex_;
        static std::unique_ptr<SkaleGasMiner> instance_;

        std::atomic<bool> isMining_;
        std::atomic<unsigned long long> hashRate_;
        std::vector<std::thread> miningThreads_;
        mutable std::mutex resultMutex_;
        std::string result_;
        std::atomic<bool> resultFound_;
        std::stop_source stopSource_;

        /**
         * Constructor for the SkaleGasMiner class.
         * As part of the singleton pattern, this constructor is private to prevent direct instantiation.
         */
        SkaleGasMiner();

        /**
         * Copy constructor for the SkaleGasMiner class.
         * Deleted to prevent copying of the singleton instance.
         */
        SkaleGasMiner(const SkaleGasMiner&) = delete;

        /**
         * Copy assignment operator for the SkaleGasMiner class.
         * Deleted to prevent copying of the singleton instance.
         */
        SkaleGasMiner& operator=(const SkaleGasMiner&) = delete;

        /**
         * Sets the result of the mining operation.
         * This function updates the result string with the outcome of the mining process.
         *
         * @param value The result string to be set.
         */
        void SetResult(const std::string& value);

        /**
         * Retrieves the result of the mining operation.
         * This function provides thread-safe access to the result string.
         *
         * @return The result string.
         */
        std::string GetResult() const;
        
        /**
         * Executes the mining operation in individual threads.
         * This function is responsible for the core mining algorithm, generating and evaluating
         * potential solutions in a multi-threaded environment.
         *
         * @param amount The target amount of gas for the mining operation.
         * @param numerator The numerator for the mining calculation.
         * @param precomputed Precomputed value for the mining algorithm.
         * @param localHashRate Atomic variable to store the hash rate calculated by this thread.
         * @param stopToken Token to signal the thread to stop running.
         */
        void DoMineGas(const unsigned long long amount, CryptoPP::Integer numerator, CryptoPP::Integer precomputed, std::atomic<unsigned long long>& localHashRate, std::stop_token stopToken);
        
        /**
         * Calculates and updates the overall hash rate of the mining operation.
         * This function aggregates the hash rates reported by individual threads and updates
         * the global hash rate value.
         *
         * @param localHashRates Array of hash rates from each mining thread.
         * @param threadCount The number of threads involved in the mining process.
         * @param callback The callback function to report the hash rate.
         * @param stopToken Token to signal the thread to stop running.
         */
        void SetHashRate(std::atomic<unsigned long long>* localHashRates, unsigned int threadCount, HashRateDelegate callback, std::stop_token stopToken);
    };
}
