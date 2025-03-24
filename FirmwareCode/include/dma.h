/**
 * @file dma.h
 * @author Jose Luis Figueroa 
 * @brief The interface definition for the Direct Memory Access (DMA). 
 * This is the header file for the definition of the  interface for a DMA
 * on a standard microcontroller.
 * @version 1.1
 * @date 2025-03-24
 * 
 * @copyright Copyright (c) 2025 Jose Luis Figueroa. All rights reserved.
 * 
 */
#ifndef DMA_H_
#define DMA_H_

/*****************************************************************************
 * Includes
 *****************************************************************************/
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include "dma_cfg.h"    /*For DMA configuration*/
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
    DmaStream_t Stream;                 /**< DMA stream */
    volatile uint32_t * peripheral;     /**< Register of the peripheral */
    uint32_t * memory;                  /**< Space of the memory */
    uint32_t length;                    /**< Number of data to transfer */
}DmaTransferConfig_t;

/*****************************************************************************
* Variables
*****************************************************************************/

/*****************************************************************************
 * Function Prototypes
*****************************************************************************/
#ifdef __cplusplus
extern "C"{
#endif

void DMA_init(const DmaConfig_t * const Config);
void DMA_transferConfig(const DmaTransferConfig_t * const TransferConfig);

#ifdef __cplusplus
} // extern C
#endif

#endif /* DMA_H_ */
