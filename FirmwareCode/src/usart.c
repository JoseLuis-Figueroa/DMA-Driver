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