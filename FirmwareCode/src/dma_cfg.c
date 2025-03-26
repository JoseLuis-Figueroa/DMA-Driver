/**
 * @file dma_cfg.c
 * @author Jose Luis Figueroa
 * @brief This module contains the implementation for the Direct Memory
 * Access peripheral configuration.
 * @version 1.1
 * @date 2025-03-24
 * 
 * @copyright Copyright (c) 2025 Jose Luis Figueroa. MIT License.
 * 
*/
/*****************************************************************************
* Module Includes
*****************************************************************************/
#include "dma_cfg.h"

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
 * The following array contains the configuration data for each DMA peripheral.
 * Each row represent a single DMA stream. Each column is representing a 
 * member of the DmaConfig_t structure. This table is read in by DMA_init, where
 * each peripheral is then set up based on this table.
 */
const DmaConfig_t DmaConfig[] = 
{
/*                                                          
 *  Stream          Channel        Direction                MemorySize
 *  PeripheralSize         MemoryIncrement              PeripheralIncrement
 *  FifoMode                      FifoThreshold
 *                
*/ 
   {DMA1_STREAM_6, DMA_CHANNEL_4, DMA_MEMORY_TO_PERIPHERAL, DMA_MEMORY_SIZE_8,
   DMA_PERIPHERAL_SIZE_8, DMA_MEMORY_INCREMENT_ENABLED, DMA_PERIPHERAL_INCREMENT_DISABLED,
   DMA_FIFO_DIRECT_MODE_ENABLED, DMA_FIFO_THRESHOLD_FULL},
   {DMA1_STREAM_5, DMA_CHANNEL_4, DMA_PERIPHERAL_TO_MEMORY, DMA_MEMORY_SIZE_8,
   DMA_PERIPHERAL_SIZE_8, DMA_MEMORY_INCREMENT_ENABLED, DMA_PERIPHERAL_INCREMENT_DISABLED,
   DMA_FIFO_DIRECT_MODE_ENABLED, DMA_FIFO_THRESHOLD_FULL},
};
/*****************************************************************************
 * Function Prototypes
*****************************************************************************/

/*****************************************************************************
 * Function Definitions
*****************************************************************************/
/*****************************************************************************
 * Function: DMA_configGet()
 */
/**
 * \b Description
 * This function is used to initialize the DMA peripheral based on the
 * configuration table defined in dma_cfg module.
 * 
 * PRE-CONDITION: The configuration table must be populated (sizeof>0). <br>
 * 
 * POST-CONDITION: A constant pointer to the first member of the configuration
 * table is returned. <br>
 * 
 * @return A pointer to the configuration table. <br>
 * 
 * \b Example:
 * @code
 * const DmaConfig_t * const DmaConfig = DMA_configGet();
 * size_t configSize = DMA_configSizeGet();
 * 
 * DMA_init(UsartConfig, configSize);
 * @endcode
 * 
 * @see DMA_configGet
 * @see DMA_getConfigSize
 * @see DMA_init
 * @see DMA_transferConfig
 * 
 */
const DmaConfig_t * const DMA_configGet(void)
{
    /* The cast is performed to ensure that the address of the first element 
     * of configuration table is returned as a constant pointer and not a
     * pointer that can be modified
    */
    return (const DmaConfig_t *)&DmaConfig[0];   
}

/*****************************************************************************
 * Function: DMA_getConfigSize()
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
 * const DmaConfig_t * const DmaConfig = DMA_configGet();
 * size_t configSize = DMA_configSizeGet();
 * 
 * DMA_init(UsartConfig, configSize);
 * @endcode
 * 
 * @see DMA_configGet
 * @see DMA_getConfigSize
 * @see DMA_init
 * @see DMA_transferConfig
 * 
*****************************************************************************/
size_t DMA_configSizeGet(void)
{
   return sizeof(DmaConfig)/sizeof(DmaConfig[0]);
}