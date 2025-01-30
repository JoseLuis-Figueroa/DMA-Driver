/**
 * @file dma.c
 * @author Jose Luis Figueroa 
 * @brief The implementation for the Direct Memory Access driver.
 * @version 1.0
 * @date 2025-01-29
 * 
 * @copyright Copyright (c) 2023 Jose Luis Figueroa. MIT License.
 * 
 */
/*****************************************************************************
* Includes
*****************************************************************************/
#include "dma.h"        /*For this modules definitions*/

/*****************************************************************************
* Module Preprocessor Constants
*****************************************************************************/

/*****************************************************************************
* Module Preprocessor Macros
*****************************************************************************/

/*****************************************************************************
* Module Typedefs
*****************************************************************************/
/**
 * Defines the error code list for the DMA configuration module.
 */
typedef enum
{
    DMA_ERROR_CODE_STREAM=1,        /**< Invalid stream*/
    DMA_ERROR_CODE_CHANNEL,         /**< Invalid channel*/
    DMA_ERROR_CODE_DIRECTION,       /**< Invalid direction*/
    DMA_ERROR_CODE_MEMORY_SIZE,     /**< Invalid memory size*/
    DMA_ERROR_CODE_PERIPHERAL_SIZE, /**< Invalid peripheral size*/
    DMA_ERROR_CODE_MEMORY_INCREMENT,/**< Invalid memory increment*/
    DMA_ERROR_CODE_PERIPHERAL_INCREMENT, /**< Invalid peripheral increment*/
    DMA_ERROR_CODE_FIFO_MODE,       /**< Invalid FIFO mode*/
    DMA_ERROR_CODE_FIFO_THRESHOLD,  /**< Invalid FIFO threshold*/
    DMA_ERROR_CODE_MAX              /**< Maximum error*/
}DmaCodeError_t;

/*****************************************************************************
 * Module Variable Definitions
 * *****************************************************************************/
/* Defines a array of pointers to the DMA stream x control register */
static uint32_t volatile * const streamControlRegister[DMA_PORTS_NUMBER] =
{
    (uint32_t*)&DMA1_Stream0->CR, (uint32_t*)&DMA1_Stream1->CR, 
    (uint32_t*)&DMA1_Stream2->CR, (uint32_t*)&DMA1_Stream3->CR,
    (uint32_t*)&DMA1_Stream4->CR, (uint32_t*)&DMA1_Stream5->CR,
    (uint32_t*)&DMA1_Stream6->CR, (uint32_t*)&DMA1_Stream7->CR,
    (uint32_t*)&DMA2_Stream0->CR, (uint32_t*)&DMA2_Stream1->CR,
    (uint32_t*)&DMA2_Stream2->CR, (uint32_t*)&DMA2_Stream3->CR,
    (uint32_t*)&DMA2_Stream4->CR, (uint32_t*)&DMA2_Stream5->CR,
    (uint32_t*)&DMA2_Stream6->CR, (uint32_t*)&DMA2_Stream7->CR
};

/* Defines a array of pointers to the DMA stream x FIFO address register */
static uint32_t volatile * const streamFifoRegister[DMA_PORTS_NUMBER] =
{
    (uint32_t*)&DMA1_Stream0->FCR, (uint32_t*)&DMA1_Stream1->FCR, 
    (uint32_t*)&DMA1_Stream2->FCR, (uint32_t*)&DMA1_Stream3->FCR,
    (uint32_t*)&DMA1_Stream4->FCR, (uint32_t*)&DMA1_Stream5->FCR,
    (uint32_t*)&DMA1_Stream6->FCR, (uint32_t*)&DMA1_Stream7->FCR,
    (uint32_t*)&DMA2_Stream0->FCR, (uint32_t*)&DMA2_Stream1->FCR,
    (uint32_t*)&DMA2_Stream2->FCR, (uint32_t*)&DMA2_Stream3->FCR,
    (uint32_t*)&DMA2_Stream4->FCR, (uint32_t*)&DMA2_Stream5->FCR,
    (uint32_t*)&DMA2_Stream6->FCR, (uint32_t*)&DMA2_Stream7->FCR
};
/* Defines a array of pointers to the DMA stream x memory 0 address*/
static uint32_t volatile * const streamMemory0Address[DMA_PORTS_NUMBER] =
{
    (uint32_t*)&DMA1_Stream0->M0AR, (uint32_t*)&DMA1_Stream1->M0AR, 
    (uint32_t*)&DMA1_Stream2->M0AR, (uint32_t*)&DMA1_Stream3->M0AR,
    (uint32_t*)&DMA1_Stream4->M0AR, (uint32_t*)&DMA1_Stream5->M0AR,
    (uint32_t*)&DMA1_Stream6->M0AR, (uint32_t*)&DMA1_Stream7->M0AR,
    (uint32_t*)&DMA2_Stream0->M0AR, (uint32_t*)&DMA2_Stream1->M0AR,
    (uint32_t*)&DMA2_Stream2->M0AR, (uint32_t*)&DMA2_Stream3->M0AR,
    (uint32_t*)&DMA2_Stream4->M0AR, (uint32_t*)&DMA2_Stream5->M0AR,
    (uint32_t*)&DMA2_Stream6->M0AR, (uint32_t*)&DMA2_Stream7->M0AR
};

/* Defines a array of pointers to the DMA stream x peripheral address*/
static uint32_t volatile * const streamPeripheralAddress[DMA_PORTS_NUMBER] =
{
    (uint32_t*)&DMA1_Stream0->PAR, (uint32_t*)&DMA1_Stream1->PAR, 
    (uint32_t*)&DMA1_Stream2->PAR, (uint32_t*)&DMA1_Stream3->PAR,
    (uint32_t*)&DMA1_Stream4->PAR, (uint32_t*)&DMA1_Stream5->PAR,
    (uint32_t*)&DMA1_Stream6->PAR, (uint32_t*)&DMA1_Stream7->PAR,
    (uint32_t*)&DMA2_Stream0->PAR, (uint32_t*)&DMA2_Stream1->PAR,
    (uint32_t*)&DMA2_Stream2->PAR, (uint32_t*)&DMA2_Stream3->PAR,
    (uint32_t*)&DMA2_Stream4->PAR, (uint32_t*)&DMA2_Stream5->PAR,
    (uint32_t*)&DMA2_Stream6->PAR, (uint32_t*)&DMA2_Stream7->PAR
};

/* Defines a array of pointers to the DMA stream x number of data register*/
static uint32_t volatile * const streamNumberOfData[DMA_PORTS_NUMBER] =
{
    (uint32_t*)&DMA1_Stream0->NDTR, (uint32_t*)&DMA1_Stream1->NDTR, 
    (uint32_t*)&DMA1_Stream2->NDTR, (uint32_t*)&DMA1_Stream3->NDTR,
    (uint32_t*)&DMA1_Stream4->NDTR, (uint32_t*)&DMA1_Stream5->NDTR,
    (uint32_t*)&DMA1_Stream6->NDTR, (uint32_t*)&DMA1_Stream7->NDTR,
    (uint32_t*)&DMA2_Stream0->NDTR, (uint32_t*)&DMA2_Stream1->NDTR,
    (uint32_t*)&DMA2_Stream2->NDTR, (uint32_t*)&DMA2_Stream3->NDTR,
    (uint32_t*)&DMA2_Stream4->NDTR, (uint32_t*)&DMA2_Stream5->NDTR,
    (uint32_t*)&DMA2_Stream6->NDTR, (uint32_t*)&DMA2_Stream7->NDTR
};

/* Defines the error code flag */
static volatile uint16_t errorCodeFlag = DMA_ERROR_CODE_NONE; /**< Error code flag*/