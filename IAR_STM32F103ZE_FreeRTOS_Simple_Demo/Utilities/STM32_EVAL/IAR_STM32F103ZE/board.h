/**
*****************************************************************************
**
**  File        : board.h
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
#ifndef __BOARD_H
#define __BOARD_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes */
#include "stm32f10x.h"


/* Typedefs */
 typedef enum
 {
   LED1 = 0,
   LED2 = 1,
   LED3 = 2,
   LED4 = 3,
 } Led_TypeDef;


/* Definitions */
#define LEDn                        4
#define LED1_GPIO_PORT              GPIOF
#define LED1_GPIO_CLK               RCC_APB2Periph_GPIOF
#define LED1_GPIO_PIN               GPIO_Pin_6

#define LED2_GPIO_PORT              GPIOF
#define LED2_GPIO_CLK               RCC_APB2Periph_GPIOF
#define LED2_GPIO_PIN               GPIO_Pin_7

#define LED3_GPIO_PORT              GPIOF
#define LED3_GPIO_CLK               RCC_APB2Periph_GPIOF
#define LED3_GPIO_PIN               GPIO_Pin_8

#define LED4_GPIO_PORT              GPIOF
#define LED4_GPIO_CLK               RCC_APB2Periph_GPIOF
#define LED4_GPIO_PIN               GPIO_Pin_9


 /* Functions */
void STM_EVAL_LEDInit(Led_TypeDef Led);
void STM_EVAL_LEDOn(Led_TypeDef Led);
void STM_EVAL_LEDOff(Led_TypeDef Led);
void STM_EVAL_LEDToggle(Led_TypeDef Led);

#ifdef __cplusplus
}
#endif

#endif /* __BOARD_H */
