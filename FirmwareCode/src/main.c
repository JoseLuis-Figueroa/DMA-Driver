/**
 * @file main.c
 * @author Jose Luis Figueroa
 * @brief UART Communication using the Direct Memory Access (DMA).
 * Design and implement a program for a Nucleo-F401RE board to demonstrate 
 * Direct  Memory Access (DMA) for efficient data transfer between UART 
 * peripheral and transmitter and receiver buffers defined in data memory  
 * (RAM). The goal is to bypass the CPU for data handling while ensuring 
 * robust  UART communication. The DMA controller is configured to:
 *     o Transmit a predefined data buffered in RAM via UART.
 *     o Simultaneously receive data from the UART and store it in a separate 
 *       buffer.
 *     o Write all configurations (UART, DMA, clock, GPIO) directly using 
 *       hardware registers. 
 * 
 * @version 1.1
 * @date 2025-03-25
 * @note 
 * 
 * @copyright Copyright (c) 2025 Jose Luis Figueroa. MIT License.
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

    while(1)
    {
        asm("nop");
    }
    
    return 0;
}