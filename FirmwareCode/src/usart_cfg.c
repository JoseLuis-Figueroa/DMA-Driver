/**
 * @file usart_cfg.c
 * @author Jose Luis Figueroa
 * @brief This module contains the implementation for the Universal 
 * Synchronous/Asynchronous Receiver Transmitter peripheral configuration.
 * @version 1.0
 * @date 2025-01-27
 * 
 * @copyright Copyright (c) 2023 Jose Luis Figueroa. MIT License.
 * 
 */
/*****************************************************************************
* Module Includes
*****************************************************************************/
#include "usart_cfg.h"

/*****************************************************************************
* Module Preprocessor Constants
*****************************************************************************/

/*****************************************************************************
* Module Preprocessor Macros
*****************************************************************************/

/*****************************************************************************
* Module Typedefs
*****************************************************************************/

/*****************************************************************************
* Module Variable Definitions
*****************************************************************************/
/**
 * The following array contains the configuration data for each USART
 * peripheral. Each row represent a single USART peripheral. Each column is 
 * representing a member of the UsartConfig_t structure. This table is read 
 * in by Usart_Init, where each peripheral is then set up based on this table.
 */
const UsartConfig_t UsartConfig[] = 
{
/*                                                          
 *  Port    WordLength    StopBits    BaudRate    RxMode    TxMode    RxDma    TxDma    Enable
 *                
*/ 
   {USART2, USART_WORD_LENGTH_8, USART_STOP_BITS_1, USART_PARITY_DISABLED,
   USART_RX_ENABLED, USART_TX_ENABLED, USART_RX_DMA_DISABLED, 
   USART_TX_DMA_DISABLED, USART_ENABLED, USART_BAUD_RATE_9600},
};  
