// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#define SKALEGASMINER_EXPORTS
#include "miner.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

namespace Cryptopia
{
	/**
	 * Checks if the mining process is currently active.
	 *
	 * @return A boolean value indicating whether mining is currently in progress.
	 */
	extern "C" SKALEGASMINER_API bool IsMining()
	{
		return Cryptopia::SkaleGasMiner::GetInstance().IsMining();
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