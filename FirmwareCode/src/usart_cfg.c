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
 *  Port          WordLength        StopBits          Parity
 *  RxMode            TxMode                RxDma    
 *  TxDma                 USART Enabler     BaudRate 
 *                
*/ 
   {USART2, USART_WORD_LENGTH_8, USART_STOP_BITS_1, USART_PARITY_DISABLED,
   USART_RX_ENABLED, USART_TX_ENABLED, USART_RX_DMA_DISABLED, 
   USART_TX_DMA_DISABLED, USART_ENABLED, USART_BAUD_RATE_9600},
};  

/*****************************************************************************
 * Function Prototypes
*****************************************************************************/

/*****************************************************************************
 * Function Definitions
*****************************************************************************/
/*****************************************************************************
 * Function: Usart_configGet()
 */
/**
 * \b Description
 * This function is used to initialize the USART based on the configuration 
 * table defined in usart_cfg module.
 * 
 * PRE-CONDITION: The configuration table needs to be populated (sizeof > 0)
 * POST-CONDITION: A constant pointer to the first member of the configuration
 * table is returned.
 * @return A pointer to the configuration table.
 * 
 * \b Example:
 * @code
 * const UsartConfig_t * const UsartConfig = Usart_configGet();
 * 
 * USART_Init(UsartConfig);
 * @endcode
 * 
 * @see USART_Init
 * @see USART_Transmit
 * @see USART_Receive
 * @see USART_registerWrite
 * @see USART_registerRead
 * 
*****************************************************************************/
const UsartConfig_t * const USART_configGet(void)
{
    /* The cast is performed to ensure that the address of the first element 
     * of configuration table is returned as a constant pointer and not a
     * pointer that can be modified
    */
    return (const UsartConfig_t *)&UsartConfig[0];
    
}