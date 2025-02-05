/**
 * @file main.c
 * @author Jose Luis Figueroa
 * @brief LFX Coding Challenge: DMA-based UART Communication
 * 
 * Challenge Overview:
 * Design and implement a program for a Nucleo-F401RE board of the STM32F4
 * microcontroller family from STMicroelectronics to demonstrate Direct 
 * Memory Access (DMA) for efficient data transfer between UART peripheral
 * and transmitter and receiver buffers defined in data memory (RAM). 
 * The goal is to bypass the CPU for data handling while ensuring robust 
 * UART communication. Participants are restricted from using HAL libraries 
 * and must directly manipulate hardware registers. You can use the register 
 * map header file.
 * 
 * Challenge Requirements:
 * 1. Direct DMA Implementation:
 * Configure the DMA controller to:
 *      o Transmit a predefined data buffered in RAM via UART.
 *      o Simultaneously receive data from the UART and store it in a separate 
 *        buffer.
 * 2. Custom Register-Level Programming:
 * Write all configurations (UART, DMA, clock, GPIO) directly using hardware
 * registers. HAL library usage is prohibited.
 * 3. Visualization in Simulator:
 * Test the code on the simulator. Step wise run the code in the debugger and
 * see the register values being updated. Check the DMASTATUS register for the
 * state of the transfer.
 * 4. Challenge Objectives:
 *      o Properly configure UART and DMA without using HALfunctions.
 *      o Ensure efficient data handling with minimal CPU involvement.
 *      o Demonstrate understanding of the peripherals and DMA operation.
 * 
 * Input and Output:
 * Input: A predefined string to be transmitted through UART.
 * Output: The same string received back through UART and stored in a buffer.
 * (For simulation, you can use loopback mode if needed.)
 * 
 * Deliverables:
 * 1. Source Code: A complete program written in C.
 * 2. Video Submission:
 *      o Explain the code's functionality and implementation.
 *      o Demonstrate the program's working using the simulator.
 * 3. Optional Documentation: A brief text or PDF document summarizing the 
 *    key points of the implementation.
 * 
 * @version 1.0
 * @date 2025-01-24
 * @note 
 * 
 * @copyright Copyright (c) 2023 Jose Luis Figueroa. MIT License.
 * 
*/
/*****************************************************************************
* Includes
*****************************************************************************/
#include<stdio.h>
#include<stdint.h>
#include "usart.h"
#include "dio.h"
#include "dma.h"

/*****************************************************************************
 * Preprocessor Constants
******************************************************************************/
#define SYSTEM_CLOCK    16000000
#define APB1_CLOCK      SYSTEM_CLOCK

/*****************************************************************************
 * Preprocessor variables
******************************************************************************/
const char txBuffer[14] = "Hello World!\n";
char rxBuffer;

int main(void)
{   /*Enable clock access to GPIOA, USART2, and DMA1*/
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;

    /*Get the address of the configuration table for DIO*/
    const DioConfig_t * const DioConfig = DIO_configGet();
    /*Initialize the DIO pins according to the configuration table*/
    DIO_init(DioConfig);

    /*Get the address of the configuration table for USART*/
    const UsartConfig_t * const UsartConfig = USART_configGet();
    /*Initialize the USART peripheral according to the configuration table*/
    USART_init(UsartConfig, APB1_CLOCK);

    /*Get the address of the configuration table for DMA*/
    const DmaConfig_t * const DmaConfig = DMA_configGet();
    /*Initialize the DMA peripheral according to the configuration table*/
    DMA_init(DmaConfig);

    /*Configure the DMA peripheral (USART_TX) for a transfer to memory*/
    DMA_transferConfig(DMA1_STREAM_6, (uint32_t*)&USART2->DR, (uint32_t*)&txBuffer[0], sizeof(txBuffer));

    /*Configure the DMA peripheral (USART_RX) for receiving data from memory*/
    DMA_transferConfig(DMA1_STREAM_5, (uint32_t*)&USART2->DR, (uint32_t*)&rxBuffer, sizeof(rxBuffer));

    while(1)
    {
        asm("nop");
    }
    
    return 0;
}