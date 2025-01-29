/**
 * @file dma_cfg.h
 * @author Jose Luis Figueroa
 * @brief This module contains interface definitions for the Direct 
 * Memory Access (DMA) configuration. This is the header file for the 
 * definition of the interface for retrieving the DMA configuration table.
 * @version 0.1
 * @date 2025-01-29
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef DMA_CFG_H_
#define DMA_CFG_H_

/*****************************************************************************
 * Includes
 *****************************************************************************/

/*****************************************************************************
 * Preprocessor Constants
******************************************************************************/
/**
 * Defines the number of the DMA streams on the processor.
*/
#define DMA_PORTS_NUMBER 16U

/** 
 * Set the value according with the number of the DMA streams used.
*/
#define DMA_USED_PORTS 2U

/*****************************************************************************
 * Typedefs
******************************************************************************/
/**
 * Defines the DMA streams contained on the MCU device. It is used to specify
 * the specific DMA streams to configure the register map.
*/
typedef enum
{
    DMA1_STREAM_0,       /**< DMA Stream 0 */
    DMA1_STREAM_1,       /**< DMA Stream 1 */
    DMA1_STREAM_2,       /**< DMA Stream 2 */
    DMA1_STREAM_3,       /**< DMA Stream 3 */
    DMA1_STREAM_4,       /**< DMA Stream 4 */
    DMA1_STREAM_5,       /**< DMA Stream 5 */
    DMA1_STREAM_6,       /**< DMA Stream 6 */
    DMA1_STREAM_7,       /**< DMA Stream 7 */
    DMA2_STREAM_0,       /**< DMA Stream 8 */
    DMA2_STREAM_1,       /**< DMA Stream 9 */
    DMA2_STREAM_2,      /**< DMA Stream 10 */
    DMA2_STREAM_3,      /**< DMA Stream 11 */
    DMA2_STREAM_4,      /**< DMA Stream 12 */
    DMA2_STREAM_5,      /**< DMA Stream 13 */
    DMA2_STREAM_6,      /**< DMA Stream 14 */
    DMA2_STREAM_7,      /**< DMA Stream 15 */
    DMA2_STREAM_MAX      /**< Defines the maximum DMA stream */
}DmaStream_t;

/*
 * Defines the DMA channel selection.
*/
typedef enum
{
    DMA_CHANNEL_0,       /**< DMA Channel 0 */
    DMA_CHANNEL_1,       /**< DMA Channel 1 */
    DMA_CHANNEL_2,       /**< DMA Channel 2 */
    DMA_CHANNEL_3,       /**< DMA Channel 3 */
    DMA_CHANNEL_4,       /**< DMA Channel 4 */
    DMA_CHANNEL_5,       /**< DMA Channel 5 */
    DMA_CHANNEL_6,       /**< DMA Channel 6 */
    DMA_CHANNEL_7,       /**< DMA Channel 7 */
    DMA_CHANNEL_MAX      /**< Defines the maximum DMA channel */
}DmaChannel_t;

/**
 * Defines the DMA data transfer direction.
*/
typedef enum
{
    DMA_PERIPHERAL_TO_MEMORY,   /**< Defines the peripheral to memory transfer*/
    DMA_MEMORY_TO_PERIPHERAL,   /**< Defines the memory to peripheral transfer*/
    DMA_MEMORY_TO_MEMORY,       /**< Defines the memory to memory transfer*/
    DMA_DIRECTION_MAX           /**< Defines the maximum DMA direction*/
}DmaDirection_t;

/**
 * Defines the DMA memory data size.
*/
typedef enum
{
    DMA_MEMORY_SIZE_8,    /**< Defines the 8-bit transfer size */
    DMA_MEMORY_SIZE_16,   /**< Defines the 16-bit transfer size */
    DMA_MEMORY_SIZE_32,   /**< Defines the 32-bit transfer size */
    DMA_MEMORY_SIZE_MAX   /**< Defines the maximum transfer size */
}DmaMemorySize_t;

/**
 * Defines the DMA peripheral data size.
*/
typedef enum
{
    DMA_PERIPHERAL_SIZE_8,    /**< Defines the 8-bit transfer size */
    DMA_PERIPHERAL_SIZE_16,   /**< Defines the 16-bit transfer size */
    DMA_PERIPHERAL_SIZE_32,   /**< Defines the 32-bit transfer size */
    DMA_PERIPHERAL_SIZE_MAX   /**< Defines the maximum transfer size */
}DmaPeripheralSize_t;

/**
 * Defines the DMA memory increment mode.
*/ 
typedef enum
{
    DMA_MEMORY_INCREMENT_DISABLED,  /**< Defines the memory increment disabled */
    DMA_MEMORY_INCREMENT_ENABLED,   /**< Defines the memory increment enabled */
    DMA_MEMORY_INCREMENT_MAX        /**< Defines the maximum memory increment */
}DmaMemoryIncrement_t;

/**
 * Defines the DMA peripheral increment mode.
*/
typedef enum
{
    DMA_PERIPHERAL_INCREMENT_DISABLED,  /**< Defines the peripheral increment disabled */
    DMA_PERIPHERAL_INCREMENT_ENABLED,   /**< Defines the peripheral increment enabled */
    DMA_PERIPHERAL_INCREMENT_MAX        /**< Defines the maximum peripheral increment */
}DmaPeripheralIncrement_t;

/**
 * Defines the DMA FIFO direct mode disable. 
*/
typedef enum
{
    DMA_FIFO_DIRECT_MODE_DISABLED,  /**< Defines the direct mode disabled */
    DMA_FIFO_DIRECT_MODE_ENABLED,   /**< Defines the direct mode enabled */
    DMA_FIFO_DIRECT_MODE_MAX        /**< Defines the maximum direct mode */
}DmaFifoMode_t;

/**
 * Defines the DMA FIFO threshold level.
*/
typedef enum
{
    DMA_FIFO_THRESHOLD_1_4,   /**< Defines the FIFO threshold 1/4 */
    DMA_FIFO_THRESHOLD_1_2,   /**< Defines the FIFO threshold 1/2 */
    DMA_FIFO_THRESHOLD_3_4,   /**< Defines the FIFO threshold 3/4 */
    DMA_FIFO_THRESHOLD_FULL,  /**< Defines the FIFO threshold full */
    DMA_FIFO_THRESHOLD_MAX    /**< Defines the maximum FIFO threshold */
}DmaFifoThreshold_t;