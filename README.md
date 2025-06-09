# UART Communication Using DMA

This repository focuses on implementing **UART communication using Direct Memory Access (DMA)**. The primary objective is to design and develop a program that enables efficient **data transfer between the UART peripheral and memory buffers (RAM)** without CPU intervention. By leveraging DMA, the system can offload data handling tasks from the CPU, improving overall performance and reducing power consumption. This approach ensures **seamless UART communication** while optimizing system resources, making it a crucial technique for embedded systems that require high-speed data transfer with minimal latency.

The system leverages **reusable DMA, UART, and DIO drivers**, written in **Embedded C (bare-metal)**, and adheres to **MISRA-C** guidelines. This design is highly reusable, scalable, and suitable for performance-constrained embedded systems.

---

## Table of Contents

1. [About the Project](#about-the-project)  
2. [Project Status](#project-status)  
3. [Getting Started](#getting-started)  
   1. [Dependencies](#dependencies)  
   2. [Getting the Source](#getting-the-source)  
   3. [Building](#building)  
   4. [Running Tests](#running-tests)  
   5. [Installation](#installation)  
4. [Usage](#usage)  
5. [Release Process](#release-process)  
6. [How to Get Help](#how-to-get-help)  
7. [Contributing](#contributing)  
8. [Further Reading](#further-reading)  
9. [License](#license)  
10. [Authors](#authors)  
11. [Acknowledgments](#acknowledgments)  

---

## About the Project

This project demonstrates **UART data transfer using DMA** to offload data handling from the CPU. The implementation directly configures **USART2** (PA2 = TX, PA3 = RX) and uses DMA streams for bidirectional communication between memory and the UART peripheral.

The architecture supports **layered firmware abstraction** (Drivers ↔ HAL ↔ Application) and promotes **modularity** and **testability**.

### Key Features

- DMA-based full-duplex UART communication.  
- Efficient transmission/reception without CPU overhead.  
- Dynamic DMA stream configuration via `DMA_transferConfig()`.  
- 8-bit data size for both memory and peripheral.  
- Reusable, MISRA-compliant embedded drivers.

### Technical Details

- **Microcontroller Platform:** STM32F401RE (ARM Cortex-M4).  
- **MCU Board:** Nucleo-F401RE.  
- **Language:** Embedded C (Bare-Metal).  
- **Coding Standard:** MISRA-C.  
- **Tools:**
  - **IDE:** VS Code with PlatformIO.  
  - **Toolchain:** GNU ARM Embedded.  
  - **Terminal:** PuTTY (for UART serial interface).  
  - **Documentation:** Doxygen.  

**[Back to top](#table-of-contents)**

---

## Project Status

![Build Status](https://img.shields.io/badge/build-passing-brightgreen)

Current Version: `v1.1`  
Tested On: `Nucleo-F401RE`  

- ✅ DMA Driver Integration  
- ✅ UART Initialization  
- ✅ UART DMA TX/RX Transfer  
- ✅ Verified with PuTTY Serial Monitor
- ⏳ Unit Testing (Planned).

**[Back to top](#table-of-contents)**

---

## Getting Started

This section helps you set up and run the project on supported STM32 MCUs.

### Dependencies

Install the following tools:
- [Visual Studio Code](https://code.visualstudio.com/) with [PlatformIO extension](https://platformio.org/install)  
- [GNU ARM Embedded Toolchain](https://developer.arm.com/downloads/-/gnu-rm)  
- [STM32Cube Drivers](https://www.st.com/en/ecosystems/stm32cube.html) (via PlatformIO)  
- [Doxygen](https://doxygen.nl/manual/install.html)  

### Getting the Source

Clone the GitHub repository:

```bash
git clone https://github.com/JoseLuis-Figueroa/DMA-Driver
cd DMA-Driver
```

### Building

You can build the project using either the **PlatformIO extension in Visual Studio Code** or the **PlatformIO CLI**.

- **Build using the PlatformIO Toolbar (GUI):** If you're using VS Code with the PlatformIO extension, simply click the "_Build_" button in the PlatformIO toolbar. This will compile your project using the selected environment in your `platformio.ini`.

- **Build using the PlatformIO CLI:** From the project's root directory, run the following command to build all environments defined in `platformio.ini`:

```
pio run
```

- **Build specific environments in CLI:** run the following command to build a specific environment:

```
pio run -e nucleo_f401re
```

### Running Tests

You can test on actual hardware by uploading the code using the **PlatformIO extension in Visual Studio Code** or the **PlatformIO CLI**:

- **Run and debug using the PlatformIO Toolbar (GUI):** If you're using VS Code with the PlatformIO extension, simply click the "_Run and Debug_" button in the PlatformIO toolbar. This will run and debug your project using the selected environment in your `platformio.ini`.

- **Build using the PlatformIO CLI:** From the project's root directory, run the following command to upload all environments defined in `platformio.ini`:

```
pio run --target upload
```

You can verify output with **PuTTY** or another serial terminal.

### Installation

No additional installation required. Flash the firmware directly via ST-Link (automatically handled by PlatformIO).

**[Back to top](#table-of-contents)**

---

## Usage

The firmware initializes all necessary peripherals and configures **DMA for memory ↔ UART data transfer** using `DMA_transferConfig()`.

### UART Settings

- **Port:** _USART2._
- **TX Pin:** _PA2._  
- **RX Pin:** _PA3._  
- **Word Length:** _8 bits._  
- **Stop Bits:** _1._  
- **Parity:** _None_  
- **Mode:** _TX & RX enabled._  
- **DMA Mode:** _Enabled for both TX and RX._  

### DMA Settings

| DMA Role | Stream       | Channel | Direction              | Memory Size | Peripheral Size  |
|----------|--------------|---------|------------------------|-------------|------------------|
| TX       | DMA1_Stream6 | Ch. 4   | Peripheral → Memory    | 8-bit       | 8-bit            |
| RX       | DMA1_Stream5 | Ch. 4   | Memory → Peripheral    | 8-bit       | 8-bit            |


### Application Implementation 

The `DIO_init`, `USART_init`, and `DMA_init` functions initialize the peripherals for data transfer.

```c
    /*Get the address of the configuration table for DIO*/
    const DioConfig_t * const DioConfig = DIO_configGet();
    /*Get the size of the configuration table*/
    size_t configSizeDio = DIO_configSizeGet();
    /*Initialize the DIO pins according to the configuration table*/
    DIO_init(DioConfig, configSizeDio);

    /*Get the address of the configuration table for USART*/
    const UsartConfig_t * const UsartConfig = USART_configGet();
    /*Get the size of the configuration table*/
    size_t configSizeUsart = USART_configSizeGet();
    /*Initialize the USART peripheral according to the configuration table*/
    USART_init(UsartConfig, configSizeUsart, APB1_CLOCK);

    /*Get the address of the configuration table for DMA*/
    const DmaConfig_t * const DmaConfig = DMA_configGet();
    /*Get the size of the configuration table*/
    size_t configSizeDma = DMA_configSizeGet();
    /*Initialize the DMA peripheral according to the configuration table*/
    DMA_init(DmaConfig, configSizeDma);
```

### DMA Transfer Configuration

The `DmaTxConfig` and `DmaRxConfig` structures define the configuration parameters used by the `DMA_transferConfig` function to set up the data transfer process.
```c
  /*Configure the DMA peripheral for a transfer to memory*/
    DmaTransferConfig_t DmaTxConfig =
    {
        .Stream = DMA1_STREAM_6,
        .peripheral = (uint32_t*)&USART2->DR,
        .memory = (uint32_t*)&txBuffer[0],
        .length = sizeof(txBuffer)/sizeof(txBuffer[0])
    };

    /*Configure the DMA peripheral for receiving data from memory*/
    DmaTransferConfig_t DmaRxConfig =
    {
        .Stream = DMA1_STREAM_5,
        .peripheral = (uint32_t*)&USART2->DR,
        .memory = (uint32_t*)&rxBuffer,
        .length = sizeof(rxBuffer)/sizeof(rxBuffer)
    };

    /*Configure the DMA peripheral (USART_TX) for a transfer to memory*/
    DMA_transferConfig(&DmaTxConfig);

    /*Configure the DMA peripheral (USART_RX) for receiving data from memory*/
    DMA_transferConfig(&DmaRxConfig);
```

The following video showcases the UART implementation using Direct Memory Access (DMA) with the **PuTTY** program for serial communication. It illustrates the step-by-step updates of UART and DMA **registers** on the DMA_transferConfig function, as well as the real-time modifications of the transmit (**txBuffer**) and receive (**rxBuffer**) buffers in RAM.

![Implementation](https://github.com/JoseLuis-Figueroa/DMA-Driver/blob/main/Documentation/doxygen/images/Output_gif.gif)

## Release Process

### Versioning

We use [Semantic Versioning](http://semver.org/).  
Latest release: **v1.1.**

### Payload

- `v1.0`: Initial UART-DMA driver release.
- `v1.1`: Updated documentation, structural improvements, and design by contract enhancements.

**[Back to top](#table-of-contents)**

---

## How to Get Help

- [Open an issue](https://github.com/JoseLuis-Figueroa/adxl345-driver/issues)
- Message me via my GitHub profile.

---

## Contributing

Contributions are welcome!  
Please refer to [CONTRIBUTING.md](Documentation/CONTRIBUTING.md) for details.

**[Back to top](#table-of-contents)**

---

## Further Reading

- [STM32F4 Reference Manual](https://www.st.com/resource/en/reference_manual/dm00031020.pdf)
- [MISRA-C Guidelines](https://www.misra.org.uk/)
- [Jacob Beningo: Reusable Firmware Development](https://www.beningo.com/)

---

## License

Licensed under the MIT License – see [LICENSE.md](LICENSE) for details.

---

## Authors

**[JoseLuis-Figueroa](https://github.com/JoseLuis-Figueroa)** – Developer & Maintainer

---

## Acknowledgments

- [Beningo Embedded Group](https://www.beningo.com/)
- [PlatformIO](https://platformio.org/)

**[Back to top](#table-of-contents)**

