/**
 * @file dma.h
 * @author Jose Luis Figueroa 
 * @brief The interface definition for the Direct Memory Access (DMA). 
 * This is the header file for the definition of the  interface for a DMA
 * on a standard microcontroller.
 * @version 1.0
 * @date 2025-01-29
 * 
 * @copyright Copyright (c) 2023 Jose Luis Figueroa. All rights reserved.
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
#define DMA_ERROR_CODE_NONE   0x0000U /**< No error code*/

/*****************************************************************************
* Configuration Constants
*****************************************************************************/

/*****************************************************************************
* Macros
*****************************************************************************/

/*****************************************************************************
* Typedefs
*****************************************************************************/

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
void DMA_transferConfig(const DmaStream_t Stream, const uint32_t * data, const uint32_t length);

#ifdef __cplusplus
} // extern C
#endif

#endif /* DMA_H_ */
