/**
 * @file dma.c
 * @author Jose Luis Figueroa 
 * @brief The implementation for the Direct Memory Access driver.
 * @version 1.1
 * @date 2025-03-24
 * 
 * @copyright Copyright (c) 2025 Jose Luis Figueroa. MIT License.
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

/*****************************************************************************
* Function Prototypes
*****************************************************************************/

/*****************************************************************************
 * Function Definitions
*****************************************************************************/
/*****************************************************************************
 * Function: DMA_init()
 *//**
 * \b Description:
 * This function is used to initialize the DMA peripheral based on the
 * configuration table defined in dma_cfg module.
 * 
 * PRE-CONDITION: The MCU clocks must be configured and enabled. <br>
 * PRE-CONDITION: Configuration table needs to be populated (sizeof>0) <br>
 * PRE-CONDITION: The DMA_PORTS_NUMBER > 0 <br>
 * PRE-CONDITION: The setting is within the maximum values (DMA_MAX). <br>
 * 
 * POST-CONDITION: The DMA peripheral is set up with the configuration
 * table. <br>
 * 
 * @param[in]   Config is a pointer to the configuration table that contains
 * the initialization for the peripheral.
 * @param[in]   configSize is the size of the configuration table.
 * 
 * @return void
 * 
 * \b Example:
 * @code
 * const DmaConfig_t * const DmaConfig = DMA_configGet();
 * size_t configSize = DMA_configSizeGet();
 * 
 * DMA_init(DmaConfig, configSize);
 * @endcode
 * 
 * @see DMA_configGet
 * @see DMA_getConfigSize
 * @see DMA_init
 * @see DMA_transferConfig
 * 
*****************************************************************************/
void DMA_init(const DmaConfig_t * const Config, size_t configSize)
{
    /* Loop through all the elements of the configuration table. */
    for(uint8_t i=0; i<configSize; i++)
    {
        /*Review if the DMA port is correct*/
        assert(Config[i].Stream < DMA_PORTS_NUMBER);

        /* Set the configuration of the DMA on the control register */
        /* Set the channel of the stream*/
        if(Config[i].Channel == DMA_CHANNEL_0)
        {
            *streamControlRegister[Config[i].Stream] &= ~DMA_SxCR_CHSEL_0;
            *streamControlRegister[Config[i].Stream] &= ~DMA_SxCR_CHSEL_1;
            *streamControlRegister[Config[i].Stream] &= ~DMA_SxCR_CHSEL_2;
        }
        else if(Config[i].Channel == DMA_CHANNEL_1)
        {
            *streamControlRegister[Config[i].Stream] |= DMA_SxCR_CHSEL_0;
            *streamControlRegister[Config[i].Stream] &= ~DMA_SxCR_CHSEL_1;
            *streamControlRegister[Config[i].Stream] &= ~DMA_SxCR_CHSEL_2;
        }
        else if(Config[i].Channel == DMA_CHANNEL_2)
        {
            *streamControlRegister[Config[i].Stream] &= ~DMA_SxCR_CHSEL_0;
            *streamControlRegister[Config[i].Stream] |= DMA_SxCR_CHSEL_1;
            *streamControlRegister[Config[i].Stream] &= ~DMA_SxCR_CHSEL_2;
        }
        else if(Config[i].Channel == DMA_CHANNEL_3)
        {
            *streamControlRegister[Config[i].Stream] |= DMA_SxCR_CHSEL_0;
            *streamControlRegister[Config[i].Stream] |= DMA_SxCR_CHSEL_1;
            *streamControlRegister[Config[i].Stream] &= ~DMA_SxCR_CHSEL_2;
        }
        else if(Config[i].Channel == DMA_CHANNEL_4)
        {
            *streamControlRegister[Config[i].Stream] &= ~DMA_SxCR_CHSEL_0;
            *streamControlRegister[Config[i].Stream] &= ~DMA_SxCR_CHSEL_1;
            *streamControlRegister[Config[i].Stream] |= DMA_SxCR_CHSEL_2;
        }
        else if(Config[i].Channel == DMA_CHANNEL_5)
        {
            *streamControlRegister[Config[i].Stream] |= DMA_SxCR_CHSEL_0;
            *streamControlRegister[Config[i].Stream] &= ~DMA_SxCR_CHSEL_1;
            *streamControlRegister[Config[i].Stream] |= DMA_SxCR_CHSEL_2;
        }
        else if(Config[i].Channel == DMA_CHANNEL_6)
        {
            *streamControlRegister[Config[i].Stream] &= ~DMA_SxCR_CHSEL_0;
            *streamControlRegister[Config[i].Stream] |= DMA_SxCR_CHSEL_1;
            *streamControlRegister[Config[i].Stream] |= DMA_SxCR_CHSEL_2;
        }
        else if(Config[i].Channel == DMA_CHANNEL_7)
        {
            *streamControlRegister[Config[i].Stream] |= DMA_SxCR_CHSEL_0;
            *streamControlRegister[Config[i].Stream] |= DMA_SxCR_CHSEL_1;
            *streamControlRegister[Config[i].Stream] |= DMA_SxCR_CHSEL_2;
        }
        else
        {
            assert(Config[i].Channel < DMA_CHANNEL_MAX);
        }
        
        /* Set the direction of the stream*/
        if(Config[i].Direction == DMA_PERIPHERAL_TO_MEMORY)
        {
            *streamControlRegister[Config[i].Stream] &= ~DMA_SxCR_DIR_0;
            *streamControlRegister[Config[i].Stream] &= ~DMA_SxCR_DIR_1;
        }
        else if(Config[i].Direction == DMA_MEMORY_TO_PERIPHERAL)
        {
            *streamControlRegister[Config[i].Stream] |= DMA_SxCR_DIR_0;
            *streamControlRegister[Config[i].Stream] &= ~DMA_SxCR_DIR_1;
        }
        else if(Config[i].Direction == DMA_MEMORY_TO_MEMORY)
        {
            *streamControlRegister[Config[i].Stream] &= ~DMA_SxCR_DIR_0;
            *streamControlRegister[Config[i].Stream] |= DMA_SxCR_DIR_1;
        }
        else
        {
            assert(Config[i].Direction < DMA_DIRECTION_MAX);
        }

        /* Set the memory data size */
        if(Config[i].MemorySize == DMA_MEMORY_SIZE_8)
        {
            *streamControlRegister[Config[i].Stream] &= ~DMA_SxCR_MSIZE_0;
            *streamControlRegister[Config[i].Stream] &= ~DMA_SxCR_MSIZE_1;
        }
        else if(Config[i].MemorySize == DMA_MEMORY_SIZE_16)
        {
            *streamControlRegister[Config[i].Stream] |= DMA_SxCR_MSIZE_0;
            *streamControlRegister[Config[i].Stream] &= ~DMA_SxCR_MSIZE_1;
        }
        else if(Config[i].MemorySize == DMA_MEMORY_SIZE_32)
        {
            *streamControlRegister[Config[i].Stream] &= ~DMA_SxCR_MSIZE_0;
            *streamControlRegister[Config[i].Stream] |= DMA_SxCR_MSIZE_1;
        }
        else
        {
            assert(Config[i].MemorySize < DMA_MEMORY_SIZE_MAX);
        }

        /* Set the peripheral data size */
        if(Config[i].PeripheralSize == DMA_PERIPHERAL_SIZE_8)
        {
            *streamControlRegister[Config[i].Stream] &= ~DMA_SxCR_PSIZE_0;
            *streamControlRegister[Config[i].Stream] &= ~DMA_SxCR_PSIZE_1;
        }
        else if(Config[i].PeripheralSize == DMA_PERIPHERAL_SIZE_16)
        {
            *streamControlRegister[Config[i].Stream] |= DMA_SxCR_PSIZE_0;
            *streamControlRegister[Config[i].Stream] &= ~DMA_SxCR_PSIZE_1;
        }
        else if(Config[i].PeripheralSize == DMA_PERIPHERAL_SIZE_32)
        {
            *streamControlRegister[Config[i].Stream] &= ~DMA_SxCR_PSIZE_0;
            *streamControlRegister[Config[i].Stream] |= DMA_SxCR_PSIZE_1;
        }
        else
        {
            assert(Config[i].PeripheralSize < DMA_PERIPHERAL_SIZE_MAX);
        }

        /* Set the memory increment mode*/
        if(Config[i].MemoryIncrement == DMA_MEMORY_INCREMENT_DISABLED)
        {
            *streamControlRegister[Config[i].Stream] &= ~DMA_SxCR_MINC;
        }
        else if(Config[i].MemoryIncrement == DMA_MEMORY_INCREMENT_ENABLED)
        {
            *streamControlRegister[Config[i].Stream] |= DMA_SxCR_MINC;
        }
        else
        {
            assert(Config[i].MemoryIncrement < DMA_MEMORY_INCREMENT_MAX);
        }

        /* Set the peripheral increment mode*/
        if(Config[i].PeripheralIncrement == DMA_PERIPHERAL_INCREMENT_DISABLED)
        {
            *streamControlRegister[Config[i].Stream] &= ~DMA_SxCR_PINC;
        }
        else if(Config[i].PeripheralIncrement == DMA_PERIPHERAL_INCREMENT_ENABLED)
        {
            *streamControlRegister[Config[i].Stream] |= DMA_SxCR_PINC;
        }
        else
        {
            assert(Config[i].PeripheralIncrement < DMA_PERIPHERAL_INCREMENT_MAX);
        }

        /* Set the configuration of the DMA on the FIFO control register */
        /* Set the FIFO direct mode disable*/
        if(Config[i].FifoMode == DMA_FIFO_DIRECT_MODE_DISABLED)
        {
            *streamFifoRegister[Config[i].Stream] |= DMA_SxFCR_DMDIS;
        }
        else if(Config[i].FifoMode == DMA_FIFO_DIRECT_MODE_ENABLED)
        {
            *streamFifoRegister[Config[i].Stream] &= ~DMA_SxFCR_DMDIS;
        }
        else
        {
            assert(Config[i].FifoMode < DMA_FIFO_DIRECT_MODE_MAX);
        }

        /* Set the FIFO threshold level*/
        if(Config[i].FifoThreshold == DMA_FIFO_THRESHOLD_1_4)
        {
            *streamFifoRegister[Config[i].Stream] &= ~DMA_SxFCR_FTH_0;
            *streamFifoRegister[Config[i].Stream] &= ~DMA_SxFCR_FTH_1;
        }
        else if(Config[i].FifoThreshold == DMA_FIFO_THRESHOLD_1_2)
        {
            *streamFifoRegister[Config[i].Stream] |= DMA_SxFCR_FTH_0;
            *streamFifoRegister[Config[i].Stream] &= ~DMA_SxFCR_FTH_1;
        }
        else if(Config[i].FifoThreshold == DMA_FIFO_THRESHOLD_3_4)
        {
            *streamFifoRegister[Config[i].Stream] &= ~DMA_SxFCR_FTH_0;
            *streamFifoRegister[Config[i].Stream] |= DMA_SxFCR_FTH_1;
        }
        else if(Config[i].FifoThreshold == DMA_FIFO_THRESHOLD_FULL)
        {
            *streamFifoRegister[Config[i].Stream] |= DMA_SxFCR_FTH_0;
            *streamFifoRegister[Config[i].Stream] |= DMA_SxFCR_FTH_1;
        }
        else
        {
            assert(Config[i].FifoThreshold < DMA_FIFO_THRESHOLD_MAX);
        }

    }

}

/*****************************************************************************
 * Function: DMA_transferConfig()
 *//**
 * \b Description:
 * This function is used to configure the DMA peripheral for a transfer. This 
 * function is used to send data specified by the DmaTransferConfig_t
 * structure which contains the Stream, peripheral, memory, and length.
 * 
 * PRE-CONDITION: The DMA peripheral must be initialized. <br>
 * PRE-CONDITION: DmaTransferConfig_t  must be populated (sizeof > 0). <br>
 * PRE-CONDITION: The DMA_PORTS_NUMBER > 0 
 * PRE-CONDITION: The Stream is within the maximum DMA_STREAM_MAX. <br>
 * 
 * POST-CONDITION: The DMA peripheral is set up for a transfer. <br>
 * 
 * @param[in]  TransferConfig is a pointer to the configuration table that
 *             contains the data for the DMA data transfer.
 * 
 * @return void
 * 
 * \b Example:
 * @code
 * const DmaConfig_t * const DmaConfig = DMA_configGet();
 * size_t configSize = DMA_configSizeGet();
 * 
 * DMA_init(DmaConfig, configSize);
 * 
 * DmaTransferConfig_t DmaTransferConfig =
 * {
 *      .Stream = DMA1_STREAM_6,
 *      .peripheral = (uint32_t*)&USART1->DR,
 *      .memory = (uint32_t*)&txBuffer[0],
 *      .length = sizeof(txBuffer)/sizeof(txBuffer[0])
 * };
 * 
 * DMA_transferConfig(&DmaTransferConfig);
 * @endcode
 * 
 * @see DMA_configGet
 * @see DMA_getConfigSize
 * @see DMA_init
 * @see DMA_transferConfig
 * 
*/
void DMA_transferConfig(const DmaTransferConfig_t * const TransferConfig)
{
    /* Set the memory address */
    *streamMemory0Address[TransferConfig->Stream] = (uint32_t)TransferConfig->memory;

    /* Set the peripheral address */
    *streamPeripheralAddress[TransferConfig->Stream] = (uint32_t)TransferConfig->peripheral;

    /* Set the number of data */
    *streamNumberOfData[TransferConfig->Stream] = TransferConfig->length;

    /* Enable the stream */
    *streamControlRegister[TransferConfig->Stream] |= DMA_SxCR_EN;
}