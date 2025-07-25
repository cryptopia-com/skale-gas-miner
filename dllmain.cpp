// dllmain.cpp : Defines the entry point for the DLL application.
#define SKALEGASMINER_EXPORTS
#include "miner.h"

namespace Cryptopia
{
	/**
	 * Checks if the mining process is currently active.
	 *
	 * @return An int value indicating whether mining is currently in progress where 1 means mining is active and 0 means it is not.
	 */
	extern "C" SKALEGASMINER_API int IsMining()
	{
		return Cryptopia::SkaleGasMiner::GetInstance().IsMining() ? 1 : 0;
	}

	/**
	 * Retrieves the current hash rate of the mining process.
	 *
	 * @return The current hash rate expressed as an unsigned long long integer.
	 */
	extern "C" SKALEGASMINER_API unsigned long long GetHashRate()
	{
		return Cryptopia::SkaleGasMiner::GetInstance().GetHashRate();
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
	extern "C" SKALEGASMINER_API void MineGas(const unsigned long long amount, const char* fromAddress, const unsigned long long nonce, const unsigned int difficulty, HashRateDelegate hashRateCallback, ResultDelegate resultCallback, const unsigned int maxThreads = 0)
	{
		Cryptopia::SkaleGasMiner::GetInstance().MineGas(amount, std::string(fromAddress), nonce, difficulty, hashRateCallback, resultCallback, maxThreads);
	}

	/**
	 * Stops the mining process.
	 */
	extern "C" SKALEGASMINER_API void Stop()
	{
		Cryptopia::SkaleGasMiner::GetInstance().Stop();
	}
}