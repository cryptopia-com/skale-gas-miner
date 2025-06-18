
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

## Building for Android

1. **Install Prerequisites:**
   - Android NDK (r21 or newer)
   - CMake 3.19+
   - Ninja (recommended)
   - Modern C++ compiler (Clang)

2. **Clone the Repository:**
   ```bash
   git clone https://github.com/cryptopia-com/skale-gas-miner.git
   cd skale-gas-miner
   ```

3. **Set the ANDROID_NDK Environment Variable:**
   - **Windows:**
     ```cmd
     set ANDROID_NDK=C:\Path\To\Android\ndk\XX.X.XXXXX
     ```
   - **macOS/Linux:**
     ```bash
     export ANDROID_NDK=/path/to/android/ndk/XX.X.XXXXX
     ```

4. **Build for All Supported ABIs:**

   - **arm64-v8a:**
     ```bash
     cmake --preset=android-arm64-v8a
     cmake --build --preset=android-arm64-v8a
     ```
   - **armeabi-v7a:**
     ```bash
     cmake --preset=android-armeabi-v7a
     cmake --build --preset=android-armeabi-v7a
     ```
   - **x86:**
     ```bash
     cmake --preset=android-x86
     cmake --build --preset=android-x86
     ```
   - **x86_64:**
     ```bash
     cmake --preset=android-x86_64
     cmake --build --preset=android-x86_64
     ```

   Each build will output `libCryptopia.SkaleMiner.so` to its respective directory, for example:
   ```
   build-android/arm64-v8a/libCryptopia.SkaleMiner.so
   build-android/armeabi-v7a/libCryptopia.SkaleMiner.so
   build-android/x86/libCryptopia.SkaleMiner.so
   build-android/x86_64/libCryptopia.SkaleMiner.so
   ```

5. **Add to Unity:**
   - Copy each `.so` file to your Unity project's `Assets/Plugins/Android/libs/<abi>/` directory (create folders as needed, e.g., `arm64-v8a`, `armeabi-v7a`, etc.).



## Using Precompiled Binaries (recommended)

1. **Download Binaries:**
    - Download the latest Cryptopia.SkaleGasMiner binaries from the [Release Page](https://github.com/cryptopia-com/skale-gas-miner/releases/) 
	
 2. **Add to Unity Project:**
    - Copy the `.dll` (for Windows) and/or `.bundle` (for macOS) into your Unity project's `Assets/Plugins` folder.
    -   Copy each `.so` file (for Android) to your Unity project's `Assets/Plugins/Android/libs/<abi>/` directory (create folders as needed, e.g., `arm64-v8a`, `armeabi-v7a`, etc.).
	
 3. **Use in Unity Scripts:**
    - Follow the same method as in "Using the Binaries in Unity" for calling functions from the binary.

 
## Using the Binaries in Unity

To use the `Cryptopia.SkaleGasMiner` binary in a Unity project, follow these steps:

1. **Import the Plugin Files:**
   - Copy the built `Cryptopia.SkaleGasMiner.dll` (Windows), `.bundle` or `.dylib` (macOS), and each `libCryptopia.SkaleMiner.so` (Android) file into your Unity project's `Assets/Plugins` folder.
   - For Android, place each `.so` file into `Assets/Plugins/Android/libs/<abi>/` (e.g., `arm64-v8a`, `armeabi-v7a`, etc.).

2. **Create a C# Script:**
   - In your Unity project, create a C# script to interface with the plugin. Use `[DllImport]` to import the native functions.


   - For full coroutine and callback safety, use a pattern like the example below. This approach avoids threading issues and is safe for IL2CPP/AOT platforms.

```csharp
using System;
using System.Collections;
using System.Runtime.InteropServices;
using UnityEngine;

// Helper class to carry mining result
public class ValueResult<T>
{
    public bool success;
    public T value;
    public string error;
}

// MonoBehaviour that wraps native plugin calls
public class SkaleGasMiner : MonoBehaviour
{
    // Native delegates
    public delegate void HashRateDelegate(ulong hashRate);
    public delegate void ResultDelegate(bool success, string result, string error);

    // Internal static state for bridging native → managed
    private static bool _isMiningCompleted;
    private static bool _miningSuccess;
    private static string _miningResult;
    private static string _miningError;
    private static ulong _lastHashRate;

    // Lock to protect native calls (DLL is not thread-safe)
    private static readonly object _miningLock = new object();
    private static bool _isMiningActive = false;

    [DllImport("Cryptopia.SkaleGasMiner", EntryPoint = "IsMining")]
    private static extern bool _IsMining();

    [DllImport("Cryptopia.SkaleGasMiner", EntryPoint = "GetHashRate")]
    private static extern ulong _GetHashRate();

    [DllImport("Cryptopia.SkaleGasMiner", EntryPoint = "MineGas")]
    private static extern void _MineGas(
        ulong amount,
        string fromAddress,
        ulong nonce,
        uint difficulty,
        HashRateDelegate hashRateCallback,
        ResultDelegate resultCallback,
        uint maxThreads = 0);

    [DllImport("Cryptopia.SkaleGasMiner", EntryPoint = "Stop")]
    private static extern void _Stop();

    // IL2CPP/AOT-safe static callback handlers
    [AOT.MonoPInvokeCallback(typeof(HashRateDelegate))]
    private static void HandleHashRate(ulong rate)
    {
        _lastHashRate = rate;
        // Optionally: broadcast hash rate update, e.g. event/callback/UI.
    }

    [AOT.MonoPInvokeCallback(typeof(ResultDelegate))]
    private static void HandleResult(bool success, string result, string error)
    {
        _miningSuccess = success;
        _miningResult = result;
        _miningError = error;
        _isMiningCompleted = true;
    }

    // Start mining gas. Fills the provided result object when done.
    // Re-entrancy protection: throws if mining is already active.
    public IEnumerator MineGas(
        ulong amount,
        string fromAddress,
        ulong nonce,
        uint difficulty,
        ValueResult<string> result,
        uint maxThreads = 8)
    {
        lock (_miningLock)
        {
            if (_isMiningActive)
                throw new InvalidOperationException("Mining already in progress. The DLL is not reentrant.");

            _isMiningActive = true;
        }

        try
        {
            _isMiningCompleted = false;
            _miningSuccess = false;
            _miningResult = null;
            _miningError = null;
            _lastHashRate = 0;

            _MineGas(
                amount,
                fromAddress,
                nonce,
                difficulty,
                HandleHashRate,
                HandleResult,
                maxThreads);

            yield return new WaitUntil(() => _isMiningCompleted);

            result.success = _miningSuccess;
            result.value = _miningResult;
            result.error = _miningError;
        }
        finally
        {
            lock (_miningLock)
            {
                _isMiningActive = false;
            }
        }
    }

    // Returns the last reported hash rate, or zero if none reported yet.
    public ulong GetLastHashRate() => _lastHashRate;

    // True if mining is in progress.
    public bool IsMining() => _IsMining();

    // Stop the mining operation.
    public void Stop() => _Stop();
}
```

4. **Call the Miner:**

```csharp
// Example MonoBehaviour that shows how to start mining and get updates
public class GasMiningExample : MonoBehaviour
{
    public SkaleGasMiner miner;

    void Start()
    {
        ulong amount = 1000000;
        string fromAddress = "0xYourAddressHere";
        ulong nonce = 1234;
        uint difficulty = 1;
        uint maxThreads = 0;

        // Start the mining coroutine
        StartCoroutine(MineAndReport(amount, fromAddress, nonce, difficulty, maxThreads));
        // Optionally: start coroutine to monitor hash rate
        StartCoroutine(MonitorHashRate());
    }

    private IEnumerator MineAndReport(
        ulong amount,
        string fromAddress,
        ulong nonce,
        uint difficulty,
        uint maxThreads)
    {
        var result = new ValueResult<string>();
        yield return StartCoroutine(miner.MineGas(amount, fromAddress, nonce, difficulty, result, maxThreads));

        if (result.success)
            Debug.Log("Mining succeeded, result (use as gas price): " + result.value);
        else
            Debug.LogError("Mining failed: " + result.error);
    }

    private IEnumerator MonitorHashRate()
    {
        // Log hash rate every second while mining is in progress
        while (miner.IsMining())
        {
            ulong hashRate = miner.GetLastHashRate();
            Debug.Log("Current hash rate: " + hashRate);
            yield return new WaitForSeconds(1f);
        }
    }
}
```
 
 ## Contributing
 
 We welcome contributions to the `Cryptopia.SkaleGasMiner` project! Please feel free to fork the repository, make your changes, and submit a pull request.
 
 ## License
 
 This project is licensed under the [MIT License](LICENSE).
 
 ## Contact
 
 For any queries or support, please contact [info@cryptopia.com](mailto:info@cryptopia.com).

