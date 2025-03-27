#  UART Communication Using DMA

## Introduction

This repository focuses on implementing **UART communication using Direct Memory Access (DMA)**. The primary objective is to design and develop a program that enables efficient **data transfer between the UART peripheral and memory buffers (RAM)** without CPU intervention. By leveraging DMA, the system can offload data handling tasks from the CPU, improving overall performance and reducing power consumption. This approach ensures **seamless UART communication** while optimizing system resources, making it a crucial technique for embedded systems that require high-speed data transfer with minimal latency.

This project configures the data transfer by integrating reusable **DIO, UART, and DMA** drivers. Developed in **embedded C (bare-metal)**, it directly interacts with the microcontroller's (MCU) register map, adhering to industry-standard coding practices such as **MISRA-C**.

## Project Overview

This repository focuses on creating **reusable drivers** and **Hardware Abstraction Layers (HALs)** for microcontrollers. The tools used in this project include:

- **Development Board:** _Nucleo-F401RE._
- **IDE/Debugger:** _Visual Studio Code (PlatformIO extension)._
- **Compiler Toolchain:** _GNU ARM Embedded Toolchain._

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


