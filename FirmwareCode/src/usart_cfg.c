/**
 * @file usart_cfg.c
 * @author Jose Luis Figueroa
 * @brief This module contains the implementation for the Universal 
 * Synchronous/Asynchronous Receiver Transmitter peripheral configuration.
 * @version 1.1
 * @date 2025-03-24
 * 
 * @copyright Copyright (c) 2025 Jose Luis Figueroa. MIT License.
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
   {USART_PORT_2, USART_WORD_LENGTH_8, USART_STOP_BITS_1, USART_PARITY_DISABLED,
   USART_RX_ENABLED, USART_TX_ENABLED, USART_RX_DMA_ENABLED, 
   USART_TX_DMA_ENABLED, USART_ENABLED, USART_BAUD_RATE_9600},
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
 * PRE-CONDITION: The configuration table needs to be populated (sizeof > 0) <br>
 * 
 * POST-CONDITION: A constant pointer to the first member of the configuration
 * table is returned. <br>
 * 
 * @return A pointer to the configuration table. <br>
 * 
 * \b Example:
 * @code
 * #define SYSTEM_CLOCK    16000000
 * #define APB1_CLOCK      SYSTEM_CLOCK
 *
 * const UsartConfig_t * const UsartConfig = Usart_configGet();
 * size_t configSize = USART_configSizeGet();
 * 
 * USART_init(UsartConfig, APB1_CLOCK, configSize);
 * @endcode
 * 
 * @see USART_configGet
 * @see USART_configSizeGet
 * @see USART_init
 * @see USART_transmit
 * @see USART_receive
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

/*****************************************************************************
 * Function: USART_getConfigSize()
*/
/**
*\b Description:
 * This function is used to get the size of the configuration table.
 * 
 * PRE-CONDITION: configuration table needs to be populated (sizeof > 0) <br>
 * 
 * POST-CONDITION: The size of the configuration table will be returned. <br>
 * 
 * @return The size of the configuration table.
 * 
 * \b Example: 
 * @code
 * #define SYSTEM_CLOCK    16000000
 * #define APB1_CLOCK      SYSTEM_CLOCK
 *
 * const UsartConfig_t * const UsartConfig = Usart_configGet();
 * size_t configSize = USART_configSizeGet();
 * 
 * USART_init(UsartConfig, APB1_CLOCK, configSize);
 * @endcode
 * 
 * @see USART_configGet
 * @see USART_configSizeGet
 * @see USART_init
 * @see USART_transmit
 * @see USART_receive
 * @see USART_registerWrite
 * @see USART_registerRead
 * 
*****************************************************************************/
size_t USART_configSizeGet(void)
{
   return sizeof(UsartConfig)/sizeof(UsartConfig[0]);
}