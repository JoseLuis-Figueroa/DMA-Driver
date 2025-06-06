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








## Settings Configuration

The implementation configures the **USART2** port (PA2=TX, and PA3=RX) as a **UART interface** to use the **DMA** for efficient data transfer. The UART settings include:

- **Data word length:** _start bit 1, data bits 8, and stop bits (TBD)._
- **Stop bits:** _1 bit._
- **Parity bit:** _1 bit._
- **Rx mode:** _Enabled._
- **Tx mode:** _Enabled._
- **TX DMA mode:** _Enabled._
- **RX DMA mode:** _Enabled._

The DMA is configured as follows:  
- **Channel:** _Channel 4._
- **Stream 6**: Transfers peripheral data (**UART as TX**) to **memory (RAM)**.  
- **Stream 5**: Receives data from **memory (RAM)** to the **peripheral (UART RX)**.  
- **Memory data size:** _8 bits._
- **Peripheral data size:** _8 bits._
- **Data Managment mode:** _FIFO._ 

## Application Implementation 

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

## Conclusion

This project demonstrates the efficient use of Direct Memory Access (DMA) for UART communication. By leveraging DMA, data transfers occur independently of the CPU, reducing processing overhead and improving system efficiency. The implementation highlights the seamless interaction between the UART peripheral, DMA controller, and memory buffers, ensuring reliable data transmission and reception. This approach is particularly beneficial for embedded systems requiring high-speed, low-latency communication while optimizing CPU resources for other critical tasks.


