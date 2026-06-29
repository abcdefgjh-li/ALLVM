# ALLVM Obfuscation Configuration Tool

Android NDK code obfuscation one-click configuration tool based on LLVM 21.x, which automatically injects compilation flags by modifying `Android.mk`.

---

## Quick Start

### Step 1: Select Project Directory

Click the `Select` button next to **jni folder** to select your NDK project directory (the jni folder containing Android.mk or the project root directory).

### Step 2: Set NDK Path

Click the `Auto Detect` button, the tool will automatically find the NDK in the same directory. If not found, please manually select the NDK root directory.

### Step 3: Select Obfuscation Features

Check the required protection options in the **Obfuscation Features** area. See below for feature descriptions.

### Step 4: Inject Android.mk

Click the **One-Click Inject** button to automatically inject obfuscation flags into the Android.mk file.

### Step 5: One-Click Build

After confirming the NDK path is correct, click **One-Click Build (ndk-build)** to start compiling.

### Step 6: Collect Artifacts

After successful compilation, click **Collect Artifacts to Output** to copy the compiled artifacts to the specified output directory.

---

## Obfuscation Features

### Code Obfuscation

| Feature | Parameter | Description |
|---------|-----------|-------------|
| **Control Flow Flattening** | `-irobf-fla` | Flatten function control flow to increase reverse engineering difficulty |
| **Indirect Branch** | `-irobf-indbr` | Convert direct jumps to indirect jumps (L1-L3) |
| **Indirect Call** | `-irobf-icall` | Convert direct calls to indirect calls (L1-L3) |
| **Indirect Global Variable** | `-irobf-indgv` | Convert global variable access to indirect access (L1-L3) |
| **String Encryption** | `-irobf-cse` | Encrypt string constants, decrypt at runtime |

### Constant Encryption

| Feature | Parameter | Description |
|---------|-----------|-------------|
| **Constant Integer Encryption** | `-irobf-cie` | Encrypt integer constants (L1-L3) |
| **Constant Float Encryption** | `-irobf-cfe` | Encrypt floating-point constants (L1-L3) |

### Anti-Debug/Detection

| Feature | Parameter | Description |
|---------|-----------|-------------|
| **RTTI Erasure** | `-irobf-rtti` | Remove C++ RTTI type information |
| **LD_PRELOAD Detection** | `-irobf-ldpreload` | Detect LD_PRELOAD injection |
| **VM Detection** | `-irobf-vmdetect` | Detect if running in a virtual machine |
| **Debugger Detection** | `-irobf-ida` | Unified detection of IDA port listening and TracerPid debug attachment |
| **VPN Detection** | `-irobf-vpn` | Detect VPN connection |
| **Proxy/iptables Detection** | `-irobf-proxy` | Detect proxy and iptables |
| **Time Difference Detection** | `-irobf-time` | Detect time difference debugging |
| **Hosts File Detection** | `-irobf-hosts` | Detect Hosts file tampering |
| **Root Detection** | `-irobf-root` | Detect Root environment (exit if Rooted) |
| **Non-Root Detection** | `-irobf-noroot` | Detect non-Root environment (exit if not Rooted) |

### Protection

| Feature | Parameter | Description |
|---------|-----------|-------------|
| **Syscall Protection** | `-irobf-syscall` | Replace libc functions with direct syscalls to prevent Hook |
| **USB Debug Protection** | `-irobf-usb` | Try to disable USB debugging; silently skip if device doesn't support |
| **Memory Dump Protection** | `-irobf-bandump` | Use `mlockall`, `setrlimit`, `prctl(PR_SET_DUMPABLE, 0)` to disable dumping |
| **Hide Maps Protection** | `-irobf-hidemaps` | Generate fake maps in Root environment and bind mount to `/proc/self/maps` |
| **Fake Maps Content** | `-irobf-fakemaps` | Generate fake maps content to confuse analysis |

### ELF Packing

| Feature | Parameter | Description |
|---------|-----------|-------------|
| **ELF Packer** | `-firobf-linker` | Pack ELF executable with ChaCha20 encryption and environment variable verification |
| **GZ Compression Pack** | `-firobf-gz` | Compress ELF with gzip+base64, generate shell script wrapper |

> **Note**: When packer is enabled, environment variable detection (`-irobf-envcheck` or `-irobf-gzcheck`) is automatically injected. The packer automatically strips symbols before packing.

### Other Features

| Feature | Parameter | Description |
|---------|-----------|-------------|
| **A-Protect Output** | `-irobf-aprotect` | Output A-Protect protection information |
| **Debug Log** | `-irobf-debug` | Output obfuscation debug information |

---

## ELF Packer Details

### ELF Packer (-firobf-linker)

1. Automatically strips symbols using `llvm-strip`
2. Encrypts ELF with ChaCha20
3. Generates packed executable with environment variable verification
4. Sets `lc` environment variable, embedded code verifies it

### GZ Compression Pack (-firobf-gz)

1. Automatically strips symbols using `llvm-strip`
2. Compresses ELF with gzip + base64
3. Generates shell script wrapper
4. Sets `lc_gz` environment variable, embedded code verifies it

### Environment Variable Detection

When packer is enabled, the following detection is automatically injected:

| Feature | Parameter | Description |
|---------|-----------|-------------|
| **Linker Env Check** | `-irobf-envcheck` | Verify `lc` environment variable (auto-injected with ELF packer) |
| **GZ Env Check** | `-irobf-gzcheck` | Verify `lc_gz` environment variable (auto-injected with GZ packer) |

> **Important**: If the packed program is extracted/unpacked, the environment variable will be missing and the program will be killed.

---

## VMP Virtual Machine Protection

VMP is the highest intensity code protection, converting code to custom virtual machine instructions for execution.

> **Important**: VMP requires disabling C++ exception handling, UI will automatically inject `-fno-exceptions -frtti` compilation options.

### How to Enable

**Method 1: Use Annotation (Recommended)**

```cpp
__attribute__((annotate("vmp")))
int my_protected_function(int a, int b) {
    return a + b;
}
```

**Method 2: Use Command Line Parameters**

```bash
-mllvm -irobf-vmp
-mllvm -irobf-vm_functions=func1;func2;func3
```

**Method 3: Add in Android.mk**

```makefile
LOCAL_CFLAGS += -mllvm -irobf-vmp
LOCAL_CFLAGS += -fno-exceptions -frtti
```

---

## Syscall Protection Details

Syscall protection replaces the following libc functions with direct syscalls:

| Original Function | Syscall Number | Description |
|-------------------|----------------|-------------|
| `connect` | 203 | Socket connection |
| `send` / `sendto` | 206 | Send data |
| `recv` / `recvfrom` | 207 | Receive data |
| `read` | 63 | Read data |
| `write` | 64 | Write data |
| `clock_gettime` | 223 | Get time |

---

## Obfuscation Intensity Levels

Some features support L1/L2/L3 three levels:

- **L1**: Light obfuscation, small performance impact
- **L2**: Medium obfuscation, balance performance and protection
- **L3**: Heavy obfuscation, highest protection strength

---

## Notes

1. **Backup**: Android.mk.bak backup file is automatically created before injection
2. **Compatibility**: Some features only support ARM64 architecture
3. **Performance**: Obfuscation increases code size and runtime overhead, choose as needed
4. **VMP**: VMP protection significantly increases code size, use only for critical functions. Requires disabling exception handling (-fno-exceptions)
5. **Root Detection**: Root detection and non-Root detection are mutually exclusive, only one can be selected
6. **A-Protect**: A-Protect output is now enabled by default, prints protection information at program startup
7. **Packer**: ELF packer and GZ packer can be enabled simultaneously. Symbols are automatically stripped before packing.

---

## Quick Actions

- `Refresh`: Reload Android.mk file
- `Clear Console`: Clear compilation output log
- `Stop Build`: Forcefully terminate ongoing compilation

---

## Author Information

- **Author**: abcdefgjh
- **QQ**: 3986612313
- **TG**: @abcdefgjha
- **GitHub**: https://github.com/abcdefgjh-li/ALLVM
