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
 * PRE-CONDITION: The MCU clocks must be configured and enabled.
 * PRE-CONDITION: The DMA_PORTS_NUMBER > 0 
 * PRE-CONDITION: The configuration table must be populated with valid values.
 * 
 * POST-CONDITION: The DMA peripheral is set up with the configuration
 * table.
 * 
 * @param[in]   Config is a pointer to the configuration table that contains
 * the initialization for the peripheral.
 * 
 * @return void
 * 
 * \b Example:
 * @code
 * const DmaConfig_t * const DmaConfig = Dma_configGet();
 * DMA_Init(DmaConfig);
 * @endcode
 * 
 * @see DMA_configGet
 * @see DMA_Init
 * @see DMA_transferConfig
 * 
 */
void DMA_init(const DmaConfig_t * const Config)
{
    /* Loop through all the elements of the configuration table. */
    for(uint8_t i=0; i<DMA_USED_PORTS; i++)
    {
        /*Review if the DMA stream is correct*/
        if(Config[i].Stream >= DMA_STREAM_MAX)
        {
            errorCodeFlag = DMA_ERROR_CODE_STREAM;
            assert(errorCodeFlag != DMA_ERROR_CODE_STREAM);
        }

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
            errorCodeFlag = DMA_ERROR_CODE_CHANNEL;
            assert(errorCodeFlag != DMA_ERROR_CODE_CHANNEL);
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
            errorCodeFlag = DMA_ERROR_CODE_DIRECTION;
            assert(errorCodeFlag != DMA_ERROR_CODE_DIRECTION);
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
            errorCodeFlag = DMA_ERROR_CODE_MEMORY_SIZE;
            assert(errorCodeFlag != DMA_ERROR_CODE_MEMORY_SIZE);
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
            errorCodeFlag = DMA_ERROR_CODE_PERIPHERAL_SIZE;
            assert(errorCodeFlag != DMA_ERROR_CODE_PERIPHERAL_SIZE);
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
            errorCodeFlag = DMA_ERROR_CODE_MEMORY_INCREMENT;
            assert(errorCodeFlag != DMA_ERROR_CODE_MEMORY_INCREMENT);
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
            errorCodeFlag = DMA_ERROR_CODE_PERIPHERAL_INCREMENT;
            assert(errorCodeFlag != DMA_ERROR_CODE_PERIPHERAL_INCREMENT);
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
            errorCodeFlag = DMA_ERROR_CODE_FIFO_MODE;
            assert(errorCodeFlag != DMA_ERROR_CODE_FIFO_MODE);
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
            errorCodeFlag = DMA_ERROR_CODE_FIFO_THRESHOLD;
            assert(errorCodeFlag != DMA_ERROR_CODE_FIFO_THRESHOLD);
        }

    }

}

/*****************************************************************************
 * Function: DMA_transferConfig()
 *//**
 * \b Description:
 * This function is used to configure the DMA peripheral for a transfer.
 * 
 * PRE-CONDITION: The DMA peripheral must be initialized.
 * PRE-CONDITION: The DMA_PORTS_NUMBER > 0 
 * 
 * POST-CONDITION: The DMA peripheral is set up for a transfer.
 * 
 * @param[in]   Stream is the DMA stream to configure.
 * @param[in]   peripheral is the register of the peripheral.
 * @param[in]   memory is the space of the memory.
 * @param[in]   length is the number of data to transfer.
 * 
 * @return void
 * 
 * \b Example:
 * @code
 * const DmaConfig_t * const DmaConfig = Dma_configGet();
 * DMA_Init(DmaConfig);
 * DMA_transferConfig(DmaConfig);
 * @endcode
 * 
 * @see DMA_configGet
 * @see DMA_Init
 * @see DMA_transferConfig
 * 
 */
void DMA_transferConfig(const DmaStream_t Stream, volatile uint32_t * const peripheral,
const uint32_t * memory, const uint32_t length)
{
    /* Set the memory address */
    *streamMemory0Address[Stream] = (uint32_t)memory;

    /* Set the peripheral address */
    *streamPeripheralAddress[Stream] = (uint32_t)&peripheral;

    /* Set the number of data */
    *streamNumberOfData[Stream] = length;

    /* Enable the stream */
    *streamControlRegister[Stream] |= DMA_SxCR_EN;
}