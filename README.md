# Cryptopia.SkaleGasMiner

Welcome to the `Cryptopia.SkaleGasMiner` project, a solution for gas mining in Unity3D on the Skale network. This project encapsulates the logic required for efficient mining, offering a minimal API for managing mining processes.

## About SKALE

[SKALE](https://skale.space/) is a limitlessly scalable, fast, on-demand blockchain network with zero gas fees, allowing quick deployment of interoperable EVM-compatible chains without compromising security or decentralization.

SKALE is a fully decentralized, open-source, community-owned network.

## About Cryptopia

[Cryptopia](https://cryptopia.com/) is a fully decentralized multiplayer RPG and strategy game, created in Unity3D. Running on the SKALE Network as its default blockchain, it offers a gasless gaming experience with a built-in native wallet and gas mining solutions. This game uniquely blends RPG elements with strategic gameplay in a blockchain-based environment.

## Building for Windows

To build the `SkaleGasMiner` DLL, follow these steps:

1. **Clone the Repository:**
   ```bash
   git clone https://github.com/cryptopia-com/skale-gas-miner.git
    ```
 
 2. **Open the Solution:**
    Open the `SkaleGasMiner.sln` file in Visual Studio.
 
 3. **Build the Project:**
    - Set the configuration to `Release` and build the solution. 
	- This will generate the `Cryptopia.SkaleGasMiner.dll` file.
 
## Building for macOS

1. **Clone the Repository:**
    ```bash
    git clone https://github.com/cryptopia-com/skale-gas-miner.git
    ```

 2. **Navigate to the Project Directory:**
    ```bash
    cd skale-gas-miner
    ```
	
 3. **Build for macOS:**
    - Compile Crypto++ for both `arm64` and `x86_64` architectures.
    - Use the `lipo` tool to create a universal binary.
    - Build the `SkaleGasMiner` project in Xcode with `Release` configuration.
	- This will generate the `Cryptopia.SkaleGasMiner.dylib` file.

### Building Crypto++ for macOS

 To create a universal binary of the Crypto++ library that works on both Intel and Apple Silicon Macs, follow these steps:
 
1. **Clone the Crypto++ Repository:**
    ```bash
    git clone https://github.com/weidai11/cryptopp.git
    cd cryptopp
    ```
	
	2. **Build for x86_64 Architecture:**
    ```bash
    make CXXFLAGS="-arch x86_64"
    mv libcryptopp.a libcryptopp-x86_64.a
    make clean
    ```
 3. **Build for arm64 Architecture:**
    ```bash
    make CXXFLAGS="-arch arm64"
    mv libcryptopp.a libcryptopp-arm64.a
    ```
 4. **Create Universal Binary:**
    ```bash
    lipo -create libcryptopp-x86_64.a libcryptopp-arm64.a -output libcryptopp-universal.a
    ```
 5. **Link in Xcode:**
    Add the path to `libcryptopp-universal.a` in Xcode's "Library Search Paths" and the Crypto++ headers in "Header Search Paths."
    In your build settings, choose "Any Mac (Apple Silicon, Intel)" as the target.

 This process involves compiling the library separately for each architecture and then combining them using the `lipo` tool. This ensures compatibility with all types of Macs.

## Using Precompiled Binaries (recommended)

1. **Download Binaries:**
    - Download the latest Cryptopia.SkaleGasMiner binaries from the [Release Page](https://github.com/cryptopia-com/skale-gas-miner/releases/) 
	
 2. **Add to Unity Project:**
    - Copy the `.dll` (for Windows) and/or `.bundle` (for macOS) into your Unity project's `Assets/Plugins` folder.
	
 3. **Use in Unity Scripts:**
    - Follow the same method as in "Using the DLL in Unity" for calling functions from the binary.

 ## Using the Binaries in Unity
 
 To use the `Cryptopia.SkaleGasMiner` binary in a Unity project, follow these steps:
 
 1. **Import the DLL:**
    Copy the built `Cryptopia.SkaleGasMiner.dll` (or .bundle) files into your Unity project's `Assets/Plugins` folder.
 
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
	[DllImport("Cryptopia.SkaleGasMiner", EntryPoint = "IsMining", CallingConvention = CallingConvention.StdCall)]
	private static extern bool _IsMining();

	/// <summary>
	/// Get hash rate
	/// </summary>
	/// <returns></returns>
	[DllImport("Cryptopia.SkaleGasMiner", EntryPoint = "GetHashRate", CallingConvention = CallingConvention.StdCall)]
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
	[DllImport("Cryptopia.SkaleGasMiner", EntryPoint = "MineGas", CallingConvention = CallingConvention.StdCall)]
	private static extern void _MineGas(ulong amount, string fromAddress, ulong nonce, uint difficulty, HashRateDelegate hashRateCallback, ResultDelegate resultCallback, uint maxThreads = 0);

	/// <summary>
	/// Stop mining
	/// </summary>
	[DllImport("Cryptopia.SkaleGasMiner", EntryPoint = "Stop", CallingConvention = CallingConvention.StdCall)]
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
 
 ## Contributing
 
 We welcome contributions to the `Cryptopia.SkaleGasMiner` project! Please feel free to fork the repository, make your changes, and submit a pull request.
 
 ## License
 
 This project is licensed under the [MIT License](LICENSE).
 
 ## Contact
 
 For any queries or support, please contact [info@cryptopia.com](mailto:info@cryptopia.com).

