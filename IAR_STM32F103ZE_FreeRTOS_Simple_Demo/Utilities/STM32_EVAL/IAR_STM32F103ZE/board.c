/**
*****************************************************************************
**
**  File        : board.c
**
**  Abstract    : Board functions for IAR STM32F103ZE.
**
**  Functions   : STM_EVAL_LEDInit
**                STM_EVAL_LEDOn
**                STM_EVAL_LEDOff
**                STM_EVAL_LEDToggle
**
**  Environment : Atollic TrueSTUDIO/STM32
**                STMicroelectronics STM32F10x Standard Peripherals Library
**
**  Distribution: The file is distributed “as is,” without any warranty
**                of any kind.
**
**  (c)Copyright Atollic AB.
**  You may use this file as-is or modify it according to the needs of your
**  project. Distribution of this file (unmodified or modified) is not
**  permitted. Atollic AB permit registered Atollic TrueSTUDIO(R) users the
**  rights to distribute the assembled, compiled & linked contents of this
**  file as part of an application binary file, provided that it is built
**  using the Atollic TrueSTUDIO(R) toolchain.
**
**
*****************************************************************************
*/

/* Includes */
#include "board.h"

/* Private typedef */
GPIO_TypeDef* GPIO_PORT[LEDn] = {LED1_GPIO_PORT, LED2_GPIO_PORT,
		                         LED3_GPIO_PORT, LED4_GPIO_PORT};
const uint16_t GPIO_PIN[LEDn] = {LED1_GPIO_PIN, LED2_GPIO_PIN,
                                 LED3_GPIO_PIN, LED4_GPIO_PIN};
const uint32_t GPIO_CLK[LEDn] = {LED1_GPIO_CLK, LED2_GPIO_CLK,
                                 LED3_GPIO_CLK, LED4_GPIO_CLK};

/* Functions */


/**
**===========================================================================
**
**  Abstract: Initialise port for selected LED
**
**  Parameter: Led (can be LED1 to LED8)
**
**===========================================================================
*/
void STM_EVAL_LEDInit(Led_TypeDef Led)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* Enable the GPIO_LED Clock */
  RCC_APB2PeriphClockCmd(GPIO_CLK[Led], ENABLE);

  /* Configure the GPIO_LED pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_PIN[Led];
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIO_PORT[Led], &GPIO_InitStructure);
}


/**
**===========================================================================
**
**  Abstract: Turns selected LED On
**
**  Parameter: Led (can be LED1 to LED8)
**
**===========================================================================
*/
void STM_EVAL_LEDOn(Led_TypeDef Led)
{
  GPIO_PORT[Led]->BSRR = GPIO_PIN[Led]; 
}

/**
**===========================================================================
**
**  Abstract: Turns selected LED Off
**
**  Parameter: Led (can be LED1 to LED8)
**
**===========================================================================
*/
void STM_EVAL_LEDOff(Led_TypeDef Led)
{
  GPIO_PORT[Led]->BRR = GPIO_PIN[Led];
}

/**
**===========================================================================
**
**  Abstract: Toggles selected LED
**
**  Parameter: Led (can be LED1 to LED8)
**
**===========================================================================
*/
void STM_EVAL_LEDToggle(Led_TypeDef Led)
{
  GPIO_PORT[Led]->ODR ^= GPIO_PIN[Led];
}

