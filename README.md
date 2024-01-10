# Cryptopia.SkaleGasMiner

Welcome to the `Cryptopia.SkaleGasMiner` project, a solution for gas mining in Unity3D on the Skale network. This project encapsulates the logic required for efficient mining, offering a minimal API for managing mining processes.

## About Cryptopia

Cryptopia is a multi-blockchain multiplayer RPG and strategy game build in Unity3D. The game offers a gasless gaming experience on Skale Network using a custom build-in native wallet solution.

## Building the DLL

To build the `SkaleGasMiner` DLL, follow these steps:

1. **Clone the Repository:**
   ```bash
   git clone https://github.com/cryptopia-com/skale-gas-miner.git
    ```
 
 2. **Open the Solution:**
    Open the `SkaleGasMiner.sln` file in Visual Studio.
 
 3. **Build the Project:**
    Set the configuration to `Release` and build the solution. This will generate the `Cryptopia.SkaleGasMiner.dll` file.
 
 ## Using the DLL in Unity
 
 To use the `Cryptopia.SkaleGasMiner` DLL in a Unity project, follow these steps:
 
 1. **Import the DLL:**
    Copy the built `Cryptopia.SkaleGasMiner.dll` file into your Unity project's `Assets/Plugins` folder.
 
 2. **Create a C# Script:**
    In your Unity project, create a C# script to interface with the DLL. Use `[DllImport]` to import the functions from the DLL.
 
 3. **Implement Function Wrappers:**
    Write C# wrapper functions that call the imported DLL functions:
 
    ```csharp
	using System.Runtime.InteropServices;
	
    /// <summary>
	/// Hash rate delegate
	/// </summary>
	/// <param name="hashRate"></param>
	public delegate void HashRateDelegate(ulong hashRate);

	/// <summary>
	/// Response delegate
	/// </summary>
	/// <param name="success"></param>
	/// <param name="result"></param>
	/// <param name="error"></param>
	public delegate void ResultDelegate(bool success, string result, string error);

	/// <summary>
	/// True if mining
	/// </summary>
	/// <returns></returns>
	[DllImport("Cryptopia.SkaleGasMiner.dll", EntryPoint = "IsMining", CallingConvention = CallingConvention.StdCall)]
	private static extern bool _IsMining();

	/// <summary>
	/// Get hash rate
	/// </summary>
	/// <returns></returns>
	[DllImport("Cryptopia.SkaleGasMiner.dll", EntryPoint = "GetHashRate", CallingConvention = CallingConvention.StdCall)]
	private static extern ulong _GetHashRate();

	/// <summary>
	/// Mine gas
	/// </summary>
	/// <param name="amount"></param>
	/// <param name="fromAddress"></param>
	/// <param name="nonce"></param>
	/// <param name="difficulty"></param>
	/// <param name="hashRateCallback"></param>
	/// <param name="resultCallback"></param>
	/// <param name="maxThreads"></param>
	[DllImport("Cryptopia.SkaleGasMiner.dll", EntryPoint = "MineGas", CallingConvention = CallingConvention.StdCall)]
	private static extern void _MineGas(ulong amount, string fromAddress, ulong nonce, uint difficulty, HashRateDelegate hashRateCallback, ResultDelegate resultCallback, uint maxThreads = 0);

	/// <summary>
	/// Stop mining
	/// </summary>
	[DllImport("Cryptopia.SkaleGasMiner.dll", EntryPoint = "Stop", CallingConvention = CallingConvention.StdCall)]
	private static extern void _Stop();
    ```
 
	4. **Call Function Wrappers:**
	Use the C# wrapper functions. For example:
 
    ```csharp
	bool isMiningCompleted = false;
	
	// Start the mining operation
	Task.Run(() => 
	{
		_MineGas(
			GasAmount,
			FromAddress,
			Nonce,
			Difficulty,
			(nextHashRate) => 
			{
				// Display 'nextHashRate' to player
			},
			(success, result, error) =>
			{
				if (success)
				{
					// Set gas price to 'result'
				}

				isMiningCompleted = true;
			},
			(uint)_maxConcurrentJobs);
	});

	// Wait for the mining operation to complete
	yield return new WaitUntil(() => isMiningCompleted);
	```
 
 5. **Use the Wrapper in Your Game Scripts:**
    You can now use `SkaleGasMinerWrapper` in your Unity scripts to control and interact with the mining process.
 
 ## Contributing
 
 We welcome contributions to the `Cryptopia.SkaleGasMiner` project! Please feel free to fork the repository, make your changes, and submit a pull request.
 
 ## License
 
 This project is licensed under the [MIT License](LICENSE).
 
 ## Contact
 
 For any queries or support, please contact [info@cryptopia.com](mailto:info@cryptopia.com).

