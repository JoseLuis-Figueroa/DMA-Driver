/**
 * @file usart.c
 * @author Jose Luis Figueroa 
 * @brief The implementation for the USART driver.
 * @version 1.0
 * @date 2025-01-27
 * 
 * @copyright Copyright (c) 2023 Jose Luis Figueroa. MIT License.
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
    * PRE-CONDITION: The MCU clocks must be configured and enabled.
    * PRE-CONDITION: The USART_PORTS_NUMBER > 0 
    * PRE-CONDITION: Configuration table needs to be populated (sizeof>0) <br>
    * 
    * POST-CONDITION: The USART peripheral is set up with the configuration
    * table.
    * 
    * @param[in]   Config is a pointer to the configuration table that contains
    * the initialization for the peripheral.
    * @param[in]   peripheralClock is the frequency of the system clock.
    * 
    * @return void
    * 
    * \b Example:
    * @code
    * const UsartConfig_t * const UsartConfig = Usart_configGet();
    * USART_Init(UsartConfig);
    * @endcode
    * 
    * @see USART_ConfigGet
    * @see USART_Init
    * @see USART_Transmit
    * @see USART_Receive
    * @see USART_registerWrite
    * @see USART_registerRead
    * 
*****************************************************************************/
void USART_init(const UsartConfig_t * const Config, const uint32_t peripheralClock)
{
    /* Loop through all the elements of the configuration table. */
    for(uint8_t i=0; i<USART_USED_PORTS; i++)
    {
        /*Review if the USART port is correct*/
        if(Config[i].Port >= USART_MAX)
        {
            errorCodeFlag = USART_ERROR_CODE_PORT;
        }

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
        else if(Config[i].WordLength >= USART_WORD_LENGTH_MAX)
        {
            errorCodeFlag = USART_ERROR_CODE_WORD_LENGTH;
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
        else if(Config[i].StopBits >= USART_STOP_BITS_MAX)
        {
            errorCodeFlag = USART_ERROR_CODE_STOP_BITS;
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
        else if(Config[i].Parity >= USART_PARITY_MAX)
        {
            errorCodeFlag = USART_ERROR_CODE_PARITY;
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
        else if(Config[i].Rx >= USART_RX_MAX)
        {
            errorCodeFlag = USART_ERROR_CODE_RX;
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
        else if(Config[i].Tx >= USART_TX_MAX)
        {
            errorCodeFlag = USART_ERROR_CODE_TX;
        }

        /* Set the RX DMA mode */
        if(Config[i].RxDma == USART_RX_DMA_ENABLED)
        {
            *controlRegister3[Config[i].Port] |= USART_CR3_DMAR;
        }
        else if(Config[i].RxDma == USART_RX_DMA_DISABLED)
        {
            *controlRegister3[Config[i].Port] &= ~USART_CR3_DMAR;
        }
        else if(Config[i].RxDma >= USART_RX_DMA_MAX)
        {
            errorCodeFlag = USART_ERROR_CODE_RX_DMA;
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
        else if(Config[i].TxDma >= USART_TX_DMA_MAX)
        {
            errorCodeFlag = USART_ERROR_CODE_TX_DMA;
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
        else if(Config[i].Enable >= USART_MAX)
        {
            errorCodeFlag = USART_ERROR_CODE_ENABLE;
        }

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
        else if(Config[i].BaudRate >= USART_BAUD_RATE_MAX)
        {
            errorCodeFlag = USART_ERROR_CODE_BAUD_RATE;
        }
    
    }
}

/*****************************************************************************
 * Function: USART_transmit()
 * 
 *//**
    * \b Description:
    * This function is used to transmit data over the USART bus.
    * 
    * PRE-CONDITION: The USART peripheral must be initialized (USART_init).
    * PRE-CONDITION: The data must be populated.
    * 
    * POST-CONDITION: The data is transmitted over the USART bus.
    * 
    * @param[in]   Port is the USART port to transmit data. Populate using the 
    *              UsartPort_t Port enum.
    * @param[in]   Data is the data to be transmitted.
    * 
    * @return void
    * 
    * \b Example:
    * @code
    * USART_transmit(USART2, "H");
    * @endcode
    * 
    * @see USART_ConfigGet
    * @see USART_Init
    * @see USART_Transmit
    * @see USART_Receive
    * @see USART_registerWrite
    * @see USART_registerRead
*****************************************************************************/
void USART_transmit(const UsartPort_t Port, const uint8_t * const data)
{
    /* Wait for the transmit buffer to be empty */
    while(!(*statusRegister[Port] & USART_SR_TXE))
    {
        asm("nop");
    }

    /* Transmit the data */
    *dataRegister[Port] = *data;
}

/*****************************************************************************
 * Function: USART_receive()
 *//**
    * \b Description:
    * This function is used to receive data over the USART bus.
    * 
    * PRE-CONDITION: The USART peripheral must be initialized (USART_init).
    * 
    * POST-CONDITION: The data is received over the USART bus.
    * 
    * @param[in]   Port is the USART port to receive data. Populate using the 
    *              UsartPort_t Port enum.
    * 
    * @return void
    * 
    * \b Example:
    * @code
    * USART_receive(USART2);
    * @endcode
    * 
    * @see USART_ConfigGet
    * @see USART_Init
    * @see USART_Transmit
    * @see USART_Receive
    * @see USART_registerWrite
    * @see USART_registerRead
    * 
*****************************************************************************/
void USART_receive(const UsartPort_t Port, uint8_t * const data)
{
    /* Wait for the receive buffer to be full */
    while(!(*statusRegister[Port] & USART_SR_RXNE))
    {
        asm("nop");
    }

    /* Read the data */
    *data = *dataRegister[Port];
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
    * PRE-CONDITION: The USART peripheral must be initialized (USART_init).
    * PRE-CONDITION: Address is within the boundaries of the USART register
    *                map.
    * 
    * POST-CONDITION: The data is written to the address.
    * 
    * @param[in]   address is the address of the register to write to.
    * @param[in]   value is the value to write to the USART register.
    * 
    * @return void
    * 
    * \b Example:
    * @code
    * USART_registerWrite(USART2, 0x01);
    * @endcode
    * 
    * @see USART_ConfigGet
    * @see USART_Init
    * @see USART_Transmit
    * @see USART_Receive
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
    * PRE-CONDITION: The USART peripheral must be initialized (USART_init).
    * PRE-CONDITION: Address is within the boundaries of the USART register
    *                map.
    * 
    * POST-CONDITION: The data is read from the address.
    * 
    * @param[in]   address is the address of the register to read from.
    * 
    * @return the value of the register.
    * 
    * \b Example:
    * @code
    * uint32_t value = USART_registerRead(USART2);
    * @endcode
    * 
    * @see USART_ConfigGet
    * @see USART_Init
    * @see USART_Transmit
    * @see USART_Receive
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