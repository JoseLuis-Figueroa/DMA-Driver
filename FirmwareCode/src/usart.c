/**
 * @file usart.c
 * @author Jose Luis Figueroa 
 * @brief The implementation for the USART driver.
 * @version 1.1
 * @date 2025-03-24
 * 
 * @copyright Copyright (c) 2025 Jose Luis Figueroa. MIT License.
 * 
*/
/*****************************************************************************
* Includes
*****************************************************************************/
#include "usart.h"        /*For this modules definitions*/

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
/* Defines a array of pointers to the USART control register 1*/
static uint32_t volatile * const controlRegister1[USART_PORTS_NUMBER] =
{
    (uint32_t*)&USART1->CR1, (uint32_t*)&USART2->CR1, (uint32_t*)&USART6->CR1
};

/* Defines a array of pointers to the USART control register 2*/
static uint32_t volatile * const controlRegister2[USART_PORTS_NUMBER] =
{
    (uint32_t*)&USART1->CR2, (uint32_t*)&USART2->CR2, (uint32_t*)&USART6->CR2
};

/* Defines a array of pointers to the USART control register 3*/
static uint32_t volatile * const controlRegister3[USART_PORTS_NUMBER] =
{
    (uint32_t*)&USART1->CR3, (uint32_t*)&USART2->CR3, (uint32_t*)&USART6->CR3
};

/* Defines a array of pointers to the USART baud rate register*/
static uint32_t volatile * const baudRateRegister[USART_PORTS_NUMBER] =
{
    (uint32_t*)&USART1->BRR, (uint32_t*)&USART2->BRR, (uint32_t*)&USART6->BRR
};

/* Defines a array of pointers to the USART status register*/
static uint32_t volatile * const statusRegister[USART_PORTS_NUMBER] =
{
    (uint32_t*)&USART1->SR, (uint32_t*)&USART2->SR, (uint32_t*)&USART6->SR
};

/* Defines a array of pointers to the USART data register*/
static uint32_t volatile * const dataRegister[USART_PORTS_NUMBER] =
{
    (uint32_t*)&USART1->DR, (uint32_t*)&USART2->DR, (uint32_t*)&USART6->DR
};

/*****************************************************************************
* Function Prototypes
*****************************************************************************/
static uint16_t USART_baudRateCalculate(const uint32_t peripheralClock, 
const uint32_t BaudRate);

/*****************************************************************************
* Function Definitions
*****************************************************************************/
/*****************************************************************************
 * Function: USART_init()
*//**
    *\b Description:
    * This function is used to initialize the USART based on the configuration
    * table defined in usart_cfg module.
    * 
    * PRE-CONDITION: The MCU clocks must be configured and enabled. <br>
    * PRE-CONDITION: Configuration table needs to be populated (sizeof>0) <br>
    * PRE-CONDITION: The USART_PORTS_NUMBER > 0 <br>
    * PRE-CONDITION: The setting is within the maximum values (USART_MAX). <br>
    * 
    * POST-CONDITION: The USART peripheral is set up with the configuration
    * table. <br>
    * 
    * @param[in]   Config is a pointer to the configuration table that contains
    * the initialization for the peripheral.
    * @param[in]   peripheralClock is the frequency of the system clock.
    * @param[in]   configSize is the size of the configuration table.
    * 
    * @return void
    * 
    * \b Example:
    * @code
    * #define SYSTEM_CLOCK    16000000
    * #define APB1_CLOCK      SYSTEM_CLOCK
    *
    * const UsartConfig_t * const UsartConfig = USART_configGet();
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
void USART_init(const UsartConfig_t * const Config, 
    size_t configSize, const uint32_t peripheralClock)
{
    /* Loop through all the elements of the configuration table. */
    for(uint8_t i=0; i<configSize; i++)
    {
        /* Prevent to assign a value out of the range of the port and pin.*/
        assert(Config[i].Port < USART_PORT_MAX);

        /* Set the configuration of the USART on the control register 1*/
        /* Set the word length */
        if(Config[i].WordLength == USART_WORD_LENGTH_9)
        {
            *controlRegister1[Config[i].Port] |= USART_CR1_M;
        }
        else if(Config[i].WordLength == USART_WORD_LENGTH_8)
        {
            *controlRegister1[Config[i].Port] &= ~USART_CR1_M;
        }
        else 
        {
            assert(Config[i].WordLength < USART_WORD_LENGTH_MAX);
        }

        /* Set the number of stop bits */
        if(Config[i].StopBits == USART_STOP_BITS_1)
        {
            *controlRegister2[Config[i].Port] &= ~USART_CR2_STOP_0;
            *controlRegister2[Config[i].Port] &= ~USART_CR2_STOP_1;
        }
        else if(Config[i].StopBits == USART_STOP_BITS_0_5)
        {
            *controlRegister2[Config[i].Port] |= USART_CR2_STOP_0;
            *controlRegister2[Config[i].Port] &= ~USART_CR2_STOP_1;
        }
        else if(Config[i].StopBits == USART_STOP_BITS_2)
        {
            *controlRegister2[Config[i].Port] &= ~USART_CR2_STOP_0;
            *controlRegister2[Config[i].Port] |= USART_CR2_STOP_1;
        }
        else if(Config[i].StopBits == USART_STOP_BITS_1_5)
        {
            *controlRegister2[Config[i].Port] |= USART_CR2_STOP_0;
            *controlRegister2[Config[i].Port] |= USART_CR2_STOP_1;
        }
        else
        {
            assert(Config[i].StopBits < USART_STOP_BITS_MAX);
        }

        /* Set the parity */
        if(Config[i].Parity == USART_PARITY_ENABLED)
        {
            *controlRegister1[Config[i].Port] |= USART_CR1_PCE;
        }
        else if(Config[i].Parity == USART_PARITY_DISABLED)
        {
            *controlRegister1[Config[i].Port] &= ~USART_CR1_PCE;
        }
        else
        {
            assert(Config[i].Parity < USART_PARITY_MAX);
        }

        /* Set the RX mode */
        if(Config[i].Rx == USART_RX_ENABLED)
        {
            *controlRegister1[Config[i].Port] |= USART_CR1_RE;
        }
        else if(Config[i].Rx == USART_RX_DISABLED)
        {
            *controlRegister1[Config[i].Port] &= ~USART_CR1_RE;
        }
        else
        {
            assert(Config[i].Rx < USART_RX_MAX);
        }

        /* Set the TX mode */
        if(Config[i].Tx == USART_TX_ENABLED)
        {
            *controlRegister1[Config[i].Port] |= USART_CR1_TE;
        }
        else if(Config[i].Tx == USART_TX_DISABLED)
        {
            *controlRegister1[Config[i].Port] &= ~USART_CR1_TE;
        }
        else
        {
            assert(Config[i].Tx < USART_TX_MAX);
        }

        /* Set the configuration of the USART on the control register 3*/
        /* Set the RX DMA mode */
        if(Config[i].RxDma == USART_RX_DMA_ENABLED)
        {
            *controlRegister3[Config[i].Port] |= USART_CR3_DMAR;
        }
        else if(Config[i].RxDma == USART_RX_DMA_DISABLED)
        {
            *controlRegister3[Config[i].Port] &= ~USART_CR3_DMAR;
        }
        else
        {
            assert(Config[i].RxDma < USART_RX_DMA_MAX);
        }

        /* Set the TX DMA mode */
        if(Config[i].TxDma == USART_TX_DMA_ENABLED)
        {
            *controlRegister3[Config[i].Port] |= USART_CR3_DMAT;
        }
        else if(Config[i].TxDma == USART_TX_DMA_DISABLED)
        {
            *controlRegister3[Config[i].Port] &= ~USART_CR3_DMAT;
        }
        else
        {
            assert(Config[i].TxDma < USART_TX_DMA_MAX);
        }

        /* Set the enable */
        if(Config[i].Enable == USART_ENABLED)
        {
            *controlRegister1[Config[i].Port] |= USART_CR1_UE;
        }
        else if(Config[i].Enable == USART_DISABLED)
        {
            *controlRegister1[Config[i].Port] &= ~USART_CR1_UE;
        }
        else
        {
            assert(Config[i].Enable < USART_UE_MAX);
        }

        /* Set the configuration of the USART on the Baud Rate Register*/
        /* Set the baud rate */
        if(Config[i].BaudRate == USART_BAUD_RATE_9600)
        {
            *baudRateRegister[Config[i].Port] = USART_baudRateCalculate(peripheralClock, 9600);
        }
        else if(Config[i].BaudRate == USART_BAUD_RATE_19200)
        {
            *baudRateRegister[Config[i].Port] = USART_baudRateCalculate(peripheralClock, 19200);
        }
        else if(Config[i].BaudRate == USART_BAUD_RATE_38400)
        {
            *baudRateRegister[Config[i].Port] = USART_baudRateCalculate(peripheralClock, 38400);
        }
        else if(Config[i].BaudRate == USART_BAUD_RATE_57600)
        {
            *baudRateRegister[Config[i].Port] = USART_baudRateCalculate(peripheralClock, 57600);
        }
        else if(Config[i].BaudRate == USART_BAUD_RATE_115200)
        {
            *baudRateRegister[Config[i].Port] = USART_baudRateCalculate(peripheralClock, 115200);
        }
        else 
        {
            assert(Config[i].BaudRate < USART_BAUD_RATE_MAX);
        }
    
    }
}

/*****************************************************************************
 * Function: USART_transmit()
 * 
 *//**
    * \b Description:
    * This function is used to transmit data over the USART bus. This function 
    * is used to send data specified by the UsartTransferConfig_t structure
    * which contains the port and data.
    * 
    * PRE-CONDITION: The USART peripheral must be initialized (USART_init). <br>
    * PRE-CONDITION: The data must be populated. <br>
    * PRE-CONDITION: UsartPortConfig_t must be populated (sizeof > 0). <br>
    * PRE-CONDITION: The Port is within the maximum UsartPort_t. <br>
    * 
    * POST-CONDITION: The data is transmitted over the USART bus.
    * 
    * @param[in]   TransferConfig is a pointer to the configuration table that
    *             contains the data for the data transfer.
    * 
    * @return void
    * 
    * \b Example:
    * @code
    * const char txBuffer[14] = "Hello World!\n";
    * UsartTransferConfig_t TransferConfig =
    * {
    *    .Port = USART2,
    *    .data = &txBuffer[0]
    * };
    * USART_transmit(&TransferConfig);
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
void USART_transmit(const UsartTransferConfig_t * const TransferConfig)
{
    /*Prevent to assign a value out of the range of the port.*/
    assert(TransferConfig->Port < USART_PORT_MAX);

    const uint8_t *auxiliarPointer = TransferConfig->data;
    while(*auxiliarPointer != '\0')
    {
        /* Wait for the transmit buffer to be empty */
        while(!(*statusRegister[TransferConfig->Port] & USART_SR_TXE))
        {
            asm("nop");
        }

        /* Transmit the data */
        *dataRegister[TransferConfig->Port] = *auxiliarPointer;
        auxiliarPointer++;
    }
}

/*****************************************************************************
 * Function: USART_receive()
 *//**
    * \b Description:
    * This function is used to initialize a data reception on the USART bus. 
    * This function is used to receive data specified by the 
    * UsartTransferConfig_t structure, which contains the port and data.
    * 
    * PRE-CONDITION: The USART peripheral must be initialized (USART_init). <br>
    * PRE-CONDITION: The data must be populated. <br>
    * PRE-CONDITION: UsartPortConfig_t must be populated. (sizeof > 0) <br>
    * PRE-CONDITION: The Port is within the maximum UsartPort_t. <br>
    * 
    * POST-CONDITION: The data is received over the USART bus.
    * 
    * @param[in]   TransferConfig is a pointer to the configuration table that
    *            contains the data for the data reception.
    * 
    * @return void
    * 
    * \b Example:
    * @code
    * char rxBuffer;
    * UsartTransferConfig_t TransferConfig =
    * {
    *    .Port = USART2,
    *    .data = &rxBuffer
    * };
    * USART_receive(&TransferConfig);
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
void USART_receive(const UsartTransferConfig_t * const TransferConfig)
{
    /* Wait for the receive buffer to be full */
    while(!(*statusRegister[TransferConfig->Port] & USART_SR_RXNE))
    {
        asm("nop");
    }

    /* Read the data */
    *TransferConfig->data = *dataRegister[TransferConfig->Port];
}

/*****************************************************************************
 * Function: USART_registerWrite()
 *//**
    * \b Description:
    * This function is used to directly address and modify a USART register.
    * The function should be used to access specialized functionality in 
    * the SPI peripheral that is not exposed by any other function of the
    * interface.
    * 
    * PRE-CONDITION: The USART peripheral must be initialized (USART_init).<br>
    * PRE-CONDITION: Address is within the boundaries of the USART register
    *                map. <br>
    * 
    * POST-CONDITION: The data is written to the address.<br>
    * 
    * @param[in]   address is the address of the register to write to.
    * @param[in]   value is the value to write to the USART register.
    * 
    * @return void
    * 
    * \b Example:
    * @code
    * USART_registerWrite(0x1000003, 0x01);
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
void USART_registerWrite(const uint32_t address, const uint32_t value)
{
    /* Write the value to the address */
    volatile uint32_t * const registerPointer = (uint32_t*)address;
    *registerPointer = value;
}

/*****************************************************************************
 * Function: USART_registerRead()
 *//**
    * \b Description:
    * This function is used to directly address and read a USART register.
    * The function should be used to access specialized functionality in 
    * the USART peripheral that is not exposed by any other function of the
    * interface.
    * 
    * PRE-CONDITION: The USART peripheral must be initialized (USART_init). <br>
    * PRE-CONDITION: Address is within the boundaries of the USART register
    *                map. <br>
    * 
    * POST-CONDITION: The data is read from the address. <br>
    * 
    * @param[in]   address is the address of the register to read from.
    * 
    * @return the value of the register.
    * 
    * \b Example:
    * @code
    * uint32_t value = USART_registerRead(0x10000003);
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
uint32_t USART_registerRead(const uint32_t address)
{
    /* Read the value from the address */
    volatile uint32_t * const registerPointer = (uint32_t*)address;
    return *registerPointer;
}

/*****************************************************************************
 * Function: USART_baudRateCalculate()
*//**
    *\b Description:
    * This function is used to calculate the baud rate based on the peripheral. 
    * 
    * PRE-CONDITION: The peripheral clock must be configured and enabled.
    * PRE-CONDITION: The baud rate must be defined.
    * 
    * POST-CONDITION: The baud rate is calculated.
    * 
    * @param[in]   peripheralClock is the frequency of the system clock.
    * @param[in]   BaudRate is the baud rate of the USART.
    * 
    * @return the calculated baud rate.
    * 
    * \b Example:
    * @code
    * uint16_t baudRate = USART_baudRateCalculate(peripheralClock, BaudRate);
    * @endcode
    * 
    * @see USART_ConfigGet
    * @see USART_ConfigSizeGet
    * @see USART_Init
    * @see USART_Transmit
    * @see USART_Receive
    * @see USART_registerWrite
    * @see USART_registerRead
    *
*****************************************************************************/
static uint16_t USART_baudRateCalculate(const uint32_t peripheralClock, const uint32_t BaudRate)
{
    return ((peripheralClock + (BaudRate/2U))/BaudRate);
}