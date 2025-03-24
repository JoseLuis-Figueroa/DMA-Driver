/**
 * @file usart.h
 * @author Jose Luis Figueroa 
 * @brief The interface definition for the Universal Synchronous/Asynchronous 
 * Receiver Transmitter. This is the header file for the definition of the 
 * interface for a USART on a standard microcontroller.
 * @version 1.1
 * @date 2025-03-24
 * 
 * @copyright Copyright (c) 2025 Jose Luis Figueroa. All rights reserved.
 * 
 */
#ifndef USART_H_
#define USART_H_

/*****************************************************************************
* Includes
*****************************************************************************/
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include "usart_cfg.h"  /*For usart configuration*/
#include "stm32f4xx.h"  /*Microcontroller family header*/

/*****************************************************************************
* Preprocessor Constants
*****************************************************************************/

/*****************************************************************************
* Configuration Constants
*****************************************************************************/

/*****************************************************************************
* Macros
*****************************************************************************/

/*****************************************************************************
* Typedefs
*****************************************************************************/
typedef struct 
{
    UsartPort_t Port;       /**< USART port*/
    uint8_t *data;          /**< Data to be transmitted*/
}UsartTransferConfig_t;

/*****************************************************************************
* Variables
*****************************************************************************/

/*****************************************************************************
 * Function Prototypes
*****************************************************************************/
#ifdef __cplusplus
extern "C"{
#endif

void USART_init(const UsartConfig_t * const Config, 
const uint32_t peripheralClock, size_t configSize);  
void USART_transmit(const UsartTransferConfig_t * const TransferConfig);
void USART_receive(const UsartTransferConfig_t * const TransferConfig);
void USART_registerWrite(const uint32_t address, const uint32_t value);
uint32_t USART_registerRead(const uint32_t address);

#ifdef __cplusplus
} // extern C
#endif

#endif /*USART_H_*/