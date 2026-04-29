# STSAFE-A Software Development Kit (Linux Userspace)  

The **STSAFE-A120 example package** offers developers a collection of applicative examples demonstrating the use of the STSAFE-A120 secure element product line from STMicroelectronics.  
These examples can serve as software reference implementations for integrating STSAFE-A120 devices on embedded Linux Host Microprocessor 

```
+------------------------+---------------------------------------------------------------------+
|                        |                                                                     |
|                        |   +-------------------------------------------------------------+   |
|       APPLICATION      |   |                 Example Projects / Tools                    |   |
|                        |   +-------------------------------------------------------------+   |
|                        |                                                                     |
|========================|=====================================================================|
|                        |   +-------------------------------------------------------------+   |
|                        |   |                             |                               |   |
|                        |   |                             |                               |   |
|       MIDDLEWARE       |   |            STSELib          |           OpenSSL             |   |
|                        |   |                             |                               |   |
|                        |   |                             |                               |   |
|                        |   +-------------------------------------------------------------+   |
|                        |                                                                     |
|========================|=====================================================================|
|                        |                                                                     |
|                        |   +-------------------------------------------------------------+   |
|        PLATFORM        |   |                         STM32MP1                            |   |
|                        |   +-------------------------------------------------------------+   |
|                        |                                                                     |
+------------------------+---------------------------------------------------------------------+

```

The following readme sections explain how to build and run the STSAFE-A120 example applications on the **STM32MP1** platform running **c** (Linux userspace on the Cortex-A7 core).

## Platform Overview

The STM32MP1 port runs the STSAFE-A120 examples as **Linux userspace applications** on the Cortex-A7 core. This is fundamentally different from the NUCLEO-L452RE bare-metal port and brings several advantages:

- Standard Linux development workflow (no IDE required)
- Native cross-compilation toolchain support
- OpenSSL-based cryptographic operations
- Standard I2C Linux kernel driver (no custom register-level code)

### Architecture

```
STM32MP157-DK2 (OpenSTLinux)
├── Cortex-A7 (Linux userspace)
│   ├── STSAFE-A120 Example application
│   ├── STSELib (ST Secure Element Library)
│   ├── Platform (Linux platform abstraction)
│   │   ├── I2C driver (/dev/i2c-X)
│   │   ├── Delay driver (POSIX nanosleep)
│   │   ├── RNG driver (getrandom syscall)
│   │   └── Crypto (OpenSSL)
│   └── Linux I2C kernel driver
└── STSAFE-A120 (connected via I2C)
```

---

## Hardware Prerequisites

| Component | Description | Link |
|-----------|-------------|------|
| **STM32MP157C-DK2** | STM32MP1 Discovery Kit (or compatible STM32MP1 board) | [Product page](https://www.st.com/en/evaluation-tools/stm32mp157c-dk2.html) |
| **X-NUCLEO-ESE01A1** | STSAFE-A120 expansion board | [Product page](https://www.st.com/en/evaluation-tools/x-nucleo-ese01a1.html) |

> **Note**: Any STM32MP1-based board with Arduino-compatible connectors and OpenSTLinux support can be used. The X-NUCLEO-ESE01A1 expansion board plugs into the Arduino connector.

### Hardware Connections

The X-NUCLEO-ESE01A1 connects to the STM32MP157-DK2 via the **Arduino connector (CN13)**. The STSAFE-A120 communicates via the **I2C** bus:

| X-NUCLEO-ESE01A1 Signal | STM32MP157-DK2 Arduino Pin | Description |
|------------------------|---------------------------|-------------|
| SDA                    | CN13 - D14 (I2C SDA)      | I2C data    |
| SCL                    | CN13 - D15 (I2C SCL)      | I2C clock   |
| GND                    | CN13 - GND                | Ground      |
| 3.3V / 5V              | CN13 - 3.3V or 5V         | Power       |

The Arduino I2C pins on the STM32MP157-DK2 are connected to **I2C5**. After booting OpenSTLinux, verify the I2C bus number:

```bash
ls /dev/i2c-*
i2cdetect -l
```

Identify the I2C bus corresponding to the Arduino connector (typically `/dev/i2c-1` on STM32MP157-DK2). The examples use bus ID `1` by default – update `stse_handler.io.busID` in `main.c` or the application to match your actual bus number.
In case the bus is not accessible or the board is not detected corectly (aswer on I2C address 0x20) , enablement is required at device tree level (please refer to following section for details on how to recompile device tree)  

---

## Software Prerequisites

### On the Development Host (Cross-compilation)

| Software | Version | Purpose | Link |
|----------|---------|---------|------|
| **OpenSTLinux SDK** | ≥ 3.1 | ARM cross-toolchain + sysroot with OpenSSL | [st.com](https://www.st.com/en/embedded-software/stm32-mpu-openstlinux-distribution.html) |
| **git** | Any | Cloning and submodule management | System package |
| **make** | Any | Build system | System package |

### On the STM32MP157-DK2 Board (Runtime)

OpenSTLinux provides all required runtime libraries. The board must have:
- Linux kernel with I2C support enabled (default in OpenSTLinux)
- OpenSSL runtime libraries (`libssl`, `libcrypto`)
- I2C utilities (optional, for diagnostics): `i2c-tools`

---

## Step-by-Step Setup

### 1. Clone the Repository

```bash
git clone https://github.com/STMicroelectronics/STSAFE-A120_examples_dev.git
cd STSAFE-A120_examples_dev
```

### 2. Initialize the STSELib Submodule

The ST Secure Element Library (STSELib) is required and distributed as a git submodule:

```bash
git submodule update --init Middleware/STSELib
```

> **Important**: The examples will not build without STSELib. Verify the submodule is populated:
> ```bash
> ls Middleware/STSELib/
> ```

### 3. Install OpenSSL Development Libraries

**For native build on the board:**
```bash
# On the STM32MP157-DK2 with OpenSTLinux:
sudo apt-get install libssl-dev   # or equivalent for your package manager
```

**For cross-compilation using OpenSTLinux SDK:**
```bash
# Source the SDK environment (adjust path to your SDK installation)
source /opt/st/stm32mp1/<version>/environment-setup-cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi
# The SDK sysroot already includes OpenSSL headers and libraries
```

---

## Building the Examples

All examples are built using the `Makefile` provided in the repository root directory.

### Build All Examples (Native)

If building directly on the board:

```bash
make
```

Binaries are placed in `Linux/build/`.

### Build a Specific Example (Native)

```bash
make EXAMPLE=01_Echo_loop
```

### Cross-compile for STM32MP1

There are two supported workflows depending on your toolchain.

#### Option A — OpenSTLinux SDK (recommended)

The SDK environment-setup script sets `CC` with `--sysroot`, `-march`, `-mfpu` and other target-specific flags. **Source the SDK first, then run `make` without `CROSS_COMPILE`**:

```bash
source /opt/st/stm32mp1/<version>/environment-setup-cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi
make
```

> **Important**: Do **not** pass `CROSS_COMPILE=` when using the OpenSTLinux SDK. The SDK environment already provides a correctly configured `CC`. Passing `CROSS_COMPILE` would reconstruct `CC` without `--sysroot`, causing compiler errors such as `stdint.h: No such file or directory`.

To build a specific example with the SDK:

```bash
source /opt/st/stm32mp1/<version>/environment-setup-cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi
make EXAMPLE=01_Echo_loop
```

#### Option B — Generic cross-toolchain (e.g. Linaro, Debian arm-gnueabihf)

If you are **not** using the OpenSTLinux SDK and have a generic ARM cross-compiler installed:

```bash
make CROSS_COMPILE=arm-linux-gnueabihf-
```

Or with a specific example:

```bash
make CROSS_COMPILE=arm-linux-gnueabihf- EXAMPLE=01_Echo_loop
```

### Build System Options

| Variable | Default | Description |
|----------|---------|-------------|
| `CROSS_COMPILE` | *(empty)* | Toolchain prefix for generic toolchains — **not needed** when using the OpenSTLinux SDK |
| `EXAMPLE` | *(all)* | Build only the specified example |

---

## Running the Examples

### 1. Transfer the Binary to the Board

After cross-compiling, copy the binary to the STM32MP157-DK2:

```bash
scp build/01_Echo_loop root@<board-ip>:/home/root/
```

### 2. Set I2C Bus Permissions

On the board, ensure the I2C device is accessible:

```bash
# Check I2C bus for STSAFE device (default address 0x20)
i2cdetect -y 1   # Replace 1 with your actual I2C bus number

# Allow user access (if needed)
sudo chmod 666 /dev/i2c-1
```

Note that 'sudo chmod 666 /dev/i2c-1' can also be executed at boot 


### 3. Configure the I2C Bus ID

By default, examples use `busID = 1` (mapping to `/dev/i2c-1`). On the STM32MP157-DK2, the Arduino connector uses I2C5, so you may need to adjust `main.c`:

```c
stse_handler.io.busID = 5;   /* /dev/i2c-5 on STM32MP157-DK2 */
```

Alternatively, create a symlink:
```bash
sudo ln -s /dev/i2c-5 /dev/i2c-1
```

### 4. Run the Example

```bash
./01_Echo_loop
```

Expected output for the Echo loop example:
```
----------------------------------------------------------------------------------------------------------------
-                                    STSAFE-A Echo loop example                                                -
----------------------------------------------------------------------------------------------------------------
 - Initialize target STSAFE-A120
 ## Message :
   00 01 02 03 04 05 06 07  08 09 0a 0b 0c 0d 0e 0f

 ## Echoed Message :
   00 01 02 03 04 05 06 07  08 09 0a 0b 0c 0d 0e 0f

*#*# STMICROELECTRONICS #*#*
```

---

## Available Examples

| Example | Description |
|---------|-------------|
| `01_Echo_loop` | Basic STSAFE-A120 communication test (echo loop) |
| `01_Random_number` | Generate random numbers from the secure element |
| `01_Hash` | Hash computation using the secure element |
| `01_Device_authentication` | ECDSA-based device/accessory authentication |
| `01_Device_authentication_multi_steps` | Multi-step authentication for remote server scenarios |
| `01_Key_pair_generation_NIST_P256` | ECC key pair generation (NIST P-256) |
| `01_Key_pair_generation_NIST_P521` | ECC key pair generation (NIST P-521) |
| `01_Key_pair_generation_BRAINPOOL_P512` | ECC key pair generation (Brainpool P-512) |
| `01_Key_pair_generation_EDWARDS_25519` | ECC key pair generation (Edwards-25519) |
| `01_Secure_data_storage_zone_access` | Secure data storage (zone-based access) |
| `01_Secure_data_storage_counter_access` | Secure data storage (counter-based access) |
| `02_Command_AC_provisioning` | Command Access Control provisioning |
| `02_Host_key_provisioning` | Host key installation |
| `02_Host_key_provisioning_wrapped` | Encrypted host key installation |
| `03_Key_wrapping` | Symmetric key wrapping and unwrapping |
| `04_Symmetric_key_provisioning_control_fields` | Key control field configuration |
| `05_Symmetric_key_establishment_compute_AES-128_CMAC` | AES-128 CMAC key establishment |
| `05_Symmetric_key_establishment_encrypt_AES-256_CCM` | AES-256 CCM key establishment |
| `05_Symmetric_key_provisioning_wrapped_compute_AES-128_CMAC` | Wrapped AES-128 CMAC provisioning |
| `05_Symmetric_key_provisioning_wrapped_encrypt_AES-256_CCM` | Wrapped AES-256 CCM provisioning |

---

## Platform Architecture Details

### Linux Platform Layer (`Platform/`)

The Linux platform layer provides Linux-compatible implementations of all hardware abstraction interfaces required by STSELib:

| Module | Implementation | Notes |
|--------|---------------|-------|
| **I2C** | `/dev/i2c-X` via `ioctl(I2C_RDWR)` | Standard Linux I2C userspace interface |
| **Delays** | `nanosleep()` (ms), `nanosleep()` (µs) | POSIX real-time clock |
| **RNG** | `getrandom()` syscall | Kernel CSPRNG, falls back to `/dev/urandom` |
| **CRC16** | Software lookup table | Same algorithm as STM32 hardware CRC |
| **AES** | OpenSSL (`EVP_CIPHER_CTX`, `CMAC_CTX`) | AES-128/192/256: ECB, CBC, CMAC |
| **ECC** | OpenSSL (`EC_KEY`, `ECDSA_SIG`, `EVP_PKEY`) | NIST P-256/384/521, Brainpool, Ed25519, X25519 |
| **Hash** | OpenSSL (`EVP_MD_CTX`) | SHA-1/224/256/384/512, HMAC-SHA256 |
| **Key Wrap** | OpenSSL AES Key Wrap (`AES_wrap_key`) | RFC 3394 compliant |
| **UART** | `printf`/`getchar` (stdio) | No serial port needed; output goes to stdout |
| **Power** | Stub (no-op) | Managed by hardware/device tree |

### Key Differences from NUCLEO-L452RE Port

| Aspect | NUCLEO-L452RE (bare-metal) | STM32MP1 (Linux) |
|--------|--------------------------|------------------|
| **OS** | None (bare-metal) | OpenSTLinux |
| **Crypto** | STM32 CMOX library | OpenSSL |
| **I2C** | Direct register access | `/dev/i2c-X` ioctl |
| **Delays** | TIM6 hardware timer | POSIX nanosleep |
| **RNG** | STM32 hardware RNG | `getrandom()` syscall |
| **Output** | UART serial (115200 baud) | stdout / terminal |
| **Build** | STM32CubeIDE / Keil / IAR | gcc Makefile |

---

## Troubleshooting


### I2C device not found

```
i2c_init: failed to open /dev/i2c-1: No such file or directory
```

**Solution**: Check the correct I2C bus number and update `busID` in `main.c`:
```bash
i2cdetect -l        # List all I2C buses
i2cdetect -y 1      # Scan bus 1 for connected devices
```
Note: If the Arduino connector I2C (I2C5 at 40015000) is missing, it is likely disabled in the default device tree to prevent pin conflicts with SPI or HDMI.
In case the Arduino I2C is disabled (default in many images) , follow these steps to activate it:

Extract the device tree from the STM32 target (from Host) 

``` bash
scp root@<ip>:/boot/stm32mp157c-dk2.dtb ./
```

Decompile the device tree 
``` bash
dtc -I dtb -O dts -o stm32mp1-custom.dts stm32mp157c-dk2.dtb
```

Open stm32mp1-custom.dts in a text editor. Search for the address of I2C5 (40015000).

You are looking for a block that looks like this:

```
i2c@40015000 {
    compatible = "st,stm32mp15-i2c";
    reg = <0x40015000 0x400>;
    interrupts = <0x0 0x6b 0x4 0x0 0x6c 0x4>;
    clocks = <0x3 0xa5>;
    resets = <0x3 0x2f>;
    status = "disabled";  <-- CHANGE THIS TO "okay"
};

```

**Crucial Check:** While in the file, ensure no other device is using the same pins (PA11/PA12). Search for PA11 or PA12. If they are assigned to a spi or usart node that is also marked as okay, you must change that node to disabled to avoid a pin conflict.

Convert your edited text and recompile it :

```bash
dtc -I dts -O dtb -o stm32mp157c-dk2.dtb stm32mp1-custom.dts
```

Push the new dts file back to the board and reboot.

```bash
# Backup the original first
ssh root@192.168.31.154 "mv /boot/stm32mp157c-dk2.dtb /boot/stm32mp157c-dk2.dtb.old"

# Upload the new one
scp stm32mp157c-dk2.dtb root@192.168.31.154:/boot/stm32mp157c-dk2.dtb

# Reboot
ssh root@192.168.31.154 "reboot"
```

After the reboot, check the status:

```bash
cat /proc/device-tree/soc/bus@5c007000/i2c@40015000/status
```
It should now return okay.

Finally, find the new bus number and scan for your STSAFE-A120:

```bash
i2cdetect -l
# Look for the new I2C bus associated with 40015000
i2cdetect -y -r <NEW_BUS_NUMBER>
```

### Permission denied accessing I2C

```
i2c_init: failed to open /dev/i2c-1: Permission denied
```

**Solution**:
```bash
sudo usermod -a -G i2c $USER   # Add user to i2c group
# or temporarily:
sudo chmod 666 /dev/i2c-1
```

### STSAFE device not responding (STSE_PLATFORM_BUS_ACK_ERROR)

- Verify the X-NUCLEO-ESE01A1 is correctly seated on the Arduino connector
- Check I2C bus with `i2cdetect -y <bus_num>` – STSAFE-A120 should appear at address `0x20`
- Ensure the expansion board is powered (check 3.3V supply)

### OpenSSL not found during build

```
fatal error: openssl/evp.h: No such file or directory
```

**Solution** (native build):
```bash
sudo apt-get install libssl-dev
```
**Solution** (cross-compilation with OpenSTLinux SDK): Source the SDK environment before building — the SDK sysroot includes OpenSSL headers and libraries:
```bash
source /opt/st/stm32mp1/<version>/environment-setup-cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi
make
```

### `stdint.h: No such file or directory` when cross-compiling

```
stdint.h:9:16: fatal error: stdint.h: No such file or directory
    9 | # include_next <stdint.h>
```

This error means the cross-compiler was invoked **without `--sysroot`**. The GCC wrapper `stdint.h` tries to `#include_next` the target sysroot's `stdint.h` but cannot find it.

**Cause**: Passing `CROSS_COMPILE=arm-ostl-linux-gnueabi-` when using the OpenSTLinux SDK. The Makefile then constructs `CC=arm-ostl-linux-gnueabi-gcc` without the `--sysroot` that the SDK normally provides.

**Solution**: Source the SDK environment first and run `make` **without** `CROSS_COMPILE`:
```bash
source /opt/st/stm32mp1/<version>/environment-setup-cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi
cd Linux
make
```
The SDK environment-setup script exports `CC` pre-configured with `--sysroot`, `-march`, `-mfpu`, and all other target flags. The Makefile honours this `CC` when it is already set in the environment.

### STSELib submodule missing

```
ERROR: STSELib submodule is not initialized!
```

**Solution**:
```bash
git submodule update --init Middleware/STSELib
```

---

## Additional Resources

- [STSAFE-A120 Product Page](https://www.st.com/en/secure-mcus/stsafe-a120.html)
- [STM32MP157C-DK2 User Manual](https://www.st.com/resource/en/user_manual/um2637-discovery-kits-with-stm32mp157-mpus-stmicroelectronics.pdf)
- [OpenSTLinux Distribution](https://www.st.com/en/embedded-software/stm32-mpu-openstlinux-distribution.html)
- [X-NUCLEO-ESE01A1 Expansion Board](https://www.st.com/en/evaluation-tools/x-nucleo-ese01a1.html)
- [STSELib Documentation](https://github.com/STMicroelectronics/STSELib)

---

*Copyright © 2024 STMicroelectronics – Licensed under the terms found in the LICENSE file.*




