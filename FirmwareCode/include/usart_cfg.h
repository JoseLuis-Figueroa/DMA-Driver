/**
 * @file usart_cfg.h
 * @author Jose Luis Figueroa 
 * @brief This module contains interface definitions for the USART
 * configuration. This is the header file for the definition of the
 * interface for retrieving the Universal Synchronous/Asynchronous Receiver
 * Transmitter configuration table.
 * @version 1.0
 * @date 2025-01-26
 * 
 * @copyright Copyright (c) 2023 Jose Luis Figueroa. MIT License.
 * 
 */
#ifndef USART_CFG_H_
#define USART_CFG_H_

/*****************************************************************************
 * Includes
******************************************************************************/

/*****************************************************************************
 * Preprocessor Constants
******************************************************************************/
/**
 * Defines the number of USART peripherals on the processor.
*/
#define USART_PORTS_NUMBER 3U

/** 
 * Set the value according with the number of USART peripheral
*/
#define USART_USED_PORTS 1U

/*****************************************************************************
 * Typedefs
******************************************************************************/
/**
 * Defines the USART ports contained on the MCU device. It is used to specify
 * the specific USART peripheral to configure the register map.
*/
typedef enum
{
    USART1,      /**< USART1 */
    USART2,      /**< USART2 */
    USART6,      /**< USART6 */
    USART_MAX    /**< Defines the maximum USART port*/
}UsartPort_t;

/**
 * Defines the USART data word length.
*/
typedef enum
{
    USART_WORD_LENGTH_8,   /**< Defines bits: start 1, data 8, stop n*/
    USART_WORD_LENGTH_9,   /**< Defines bits: start 1, data 9, stop n*/
    USART_WORD_LENGTH_MAX  /**< Defines the maximum data bits*/
}UsartWordLength_t;

/**
 * Defines the USART stop bits.
*/
typedef enum
{
    USART_STOP_BITS_1,     /**< Defines 1 stop bit*/
    USART_STOP_BITS_0_5,   /**< Defines 0.5 stop bits*/
    USART_STOP_BITS_2,     /**< Defines 2 stop bits*/
    USART_STOP_BITS_1_5,   /**< Defines 1.5 stop bits*/
    USART_STOP_BITS_MAX    /**< Defines the maximum stop bits*/
}UsartStopBits_t;

/**
 * Defines the USART parity.
*/ 
typedef enum
{
    USART_PARITY_DISABLED,  /**< Defines no parity*/
    USART_PARITY_ENABLED,   /**< Defines even parity*/
    USART_PARITY_MAX        /**< Defines the maximum parity*/
}UsartParity_t;

/**
 * Defines the USART RX mode.
*/
typedef enum
{
    USART_RX_DISABLED,      /**< Defines the RX disabled*/
    USART_RX_ENABLED,       /**< Defines the RX enabled*/
    USART_RX_MAX            /**< Defines the maximum RX mode*/
}UsartRx_t;

/**
 * Defines the USART TX mode.
*/
typedef enum
{
    USART_TX_DISABLED,      /**< Defines the TX disabled*/
    USART_TX_ENABLED,       /**< Defines the TX enabled*/
    USART_TX_MAX            /**< Defines the maximum TX mode*/
}UsartTx_t;

/**
 * Defines the USART RX DMA mode.
*/
typedef enum
{
    USART_RX_DMA_DISABLED,  /**< Defines the RX DMA disabled*/
    USART_RX_DMA_ENABLED,   /**< Defines the RX DMA enabled*/
    USART_RX_DMA_MAX        /**< Defines the maximum RX DMA mode*/
}UsartRxDma_t;

/**
 * Defines the USART TX DMA mode.
*/
typedef enum
{
    USART_TX_DMA_DISABLED,  /**< Defines the TX DMA disabled*/
    USART_TX_DMA_ENABLED,   /**< Defines the TX DMA enabled*/
    USART_TX_DMA_MAX        /**< Defines the maximum TX DMA mode*/
}UsartTxDma_t;

/**
 * Defines the USART enable. 
*/
typedef enum
{
    USART_DISABLED,         /**< Defines the USART disabled*/
    USART_ENABLED,          /**< Defines the USART enabled*/
    USART_MAX               /**< Defines the maximum USART enable*/
}UsartEnable_t;

/**
 * Defines the USART baud rates.
*/
typedef enum
{
    USART_BAUD_RATE_9600   = 9600,   /**< Defines the baud rate 9600*/
    USART_BAUD_RATE_19200  = 19200,  /**< Defines the baud rate 19200*/
    USART_BAUD_RATE_38400  = 38400,  /**< Defines the baud rate 38400*/
    USART_BAUD_RATE_57600  = 57600,  /**< Defines the baud rate 57600*/
    USART_BAUD_RATE_115200 = 115200, /**< Defines the baud rate 115200*/
    USART_BAUD_RATE_MAX              /**< Defines the maximum baud rate*/
}UsartBaudRate_t;

/**
 * Defines the Universal Synchronous/Asynchronous Receiver Transmitter
 * configuration table. This table is used to configure the USART 
 * peripheral in the Dio_Init function.
*/
typedef struct
{
    UsartPort_t         Port;       /**< USART port*/
    UsartWordLength_t   WordLength; /**< 8 data bits or 9 data bits*/
    UsartStopBits_t     StopBits;   /**< 1, 0.5, 2, or 1.5 stop bits*/
    UsartParity_t       Parity;     /**< Enable or disable parity bit*/
    UsartRx_t           Rx;         /**< Enable or disable RX mode*/
    UsartTx_t           Tx;         /**< Enable or disable TX mode*/
    UsartRxDma_t        RxDma;      /**< Enable or disable RX DMA mode*/
    UsartTxDma_t        TxDma;      /**< Enable or disable TX DMA mode*/
    UsartEnable_t       Enable;     /**< USART or disable enable*/
    UsartBaudRate_t     BaudRate;   /**< USART baud rate*/
}UsartConfig_t;

/**
 * Defines the error code list for the USART configuration module.
 */
typedef enum
{
    USART_ERROR_CODE_NONE,       /**< No error*/
    USART_ERROR_CODE_PORT,       /**< Invalid port*/
    USART_ERROR_CODE_WORD_LENGTH,/**< Invalid word length*/
    USART_ERROR_CODE_STOP_BITS,  /**< Invalid stop bits*/
    USART_ERROR_CODE_PARITY,     /**< Invalid parity*/
    USART_ERROR_CODE_RX,         /**< Invalid RX mode*/
    USART_ERROR_CODE_TX,         /**< Invalid TX mode*/
    USART_ERROR_CODE_RX_DMA,     /**< Invalid RX DMA mode*/
    USART_ERROR_CODE_TX_DMA,     /**< Invalid TX DMA mode*/
    USART_ERROR_CODE_ENABLE,     /**< Invalid enable*/
    USART_ERROR_CODE_BAUD_RATE,  /**< Invalid baud rate*/
    USART_ERROR_CODE_MAX         /**< Maximum error*/
}UsartCodeError_t;

/*****************************************************************************
 * Function Prototypes
 *****************************************************************************/
#ifdef __cplusplus
extern "C"{
#endif

const UsartConfig_t * const USART_configGet(void);

#ifdef __cplusplus
} // extern C
#endif

#endif /*USART_CFG_H_*/

