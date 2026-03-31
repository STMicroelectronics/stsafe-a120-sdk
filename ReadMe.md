# STSAFE-A Software Development Kit

The **STSAFE-A Software Development Kit** offers developers a collection of application examples demonstrating the use of the STSAFE-A secure element product family from STMicroelectronics.  
These examples can serve as software reference implementations for integrating STSAFE-A120 devices on host microcontroller platforms.

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
|       MIDDLEWARE       |   |            STSELib          |       STM32Cryptolib          |   |
|                        |   |                             |           (CMOX)              |   |
|                        |   |                             |                               |   |
|                        |   +-------------------------------------------------------------+   |
|                        |                                                                     |
|========================|=====================================================================|
|                        |                                                                     |
|                        |   +-------------------------------------------------------------+   |
|        PLATFORM        |   |                       NUCLEO-L452RE                         |   |
|                        |   +-------------------------------------------------------------+   |
|                        |                                                                     |
+------------------------+---------------------------------------------------------------------+

```

## Hardware and Software Prerequisites

The following hardware (HW) and software (SW) prerequisites are required to work with this package:

- **Hardware prerequisites**
  - [NUCLEO-L452RE](https://www.st.com/en/evaluation-tools/nucleo-l452re.html) STM32 development board  
  - [X-NUCLEO-ESE01A1](https://www.st.com/en/evaluation-tools/x-nucleo-ese01a1.html#overview) Nucleo expansion board

- **Software prerequisites**
  - One of the following compatible toolchains/IDEs:
    - [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html)  
    - [Keil uVision 5.37](https://www.st.com/en/partner-products-and-services/arm-keil-mdk.html)  
    - [IAR EWARM 9.40.1](https://www.st.com/en/partner-products-and-services/iar-embedded-workbench-for-arm.html)
  - [X-CUBE-CRYPTOLIB 4.5.0](https://www.st.com/en/embedded-software/x-cube-cryptolib.html)

Please refer to the STSAFE-A SDK wiki pages for information on how to get started with the package.

## Documentation

A dedicated [wiki page](https://github.com/STMicroelectronics/stsafe-a120-sdk/wiki) is available on GitHub.  
This wiki can be used as an entry point for embedded system developers. It covers the application hardware and software description, and provides developers with product information and standard STSAFE-A use cases.
