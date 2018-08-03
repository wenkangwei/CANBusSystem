#ifndef USR_USART_H
#define USR_USART_H

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"
#include "printf.h"

/*****Definition ******/
#define USART1_GPIO  GPIOA
#define USART1_TXD_Pin      GPIO_Pin_9
#define USART1_RXD_Pin      GPIO_Pin_10

#define USART1_REMAP_GPIO  GPIOB
#define USART1_REMAP_TXD_Pin      GPIO_Pin_6
#define USART1_REMAP_RXD_Pin      GPIO_Pin_7

#define USART2_NREMAP_GPIO         GPIOA
#define USART2_NREMAP_CTS_Pin      GPIO_Pin_0
#define USART2_NREMAP_RTS_Pin      GPIO_Pin_1
#define USART2_NREMAP_TXD_Pin      GPIO_Pin_2
#define USART2_NREMAP_RXD_Pin      GPIO_Pin_3
#define USART2_NREMAP_CK_Pin        GPIO_Pin_4

#define USART2_REMAP_GPIO         GPIOD
#define USART2_REMAP_CTS_Pin      GPIO_Pin_3
#define USART2_REMAP_RTS_Pin      GPIO_Pin_4
#define USART2_REMAP_TXD_Pin      GPIO_Pin_5
#define USART2_REMAP_RXD_Pin      GPIO_Pin_6
#define USART2_REMAP_CK_Pin        GPIO_Pin_7


#define USART3_NREMAP_GPIO         GPIOB
#define USART3_NREMAP_CTS_Pin      GPIO_Pin_13
#define USART3_NREMAP_RTS_Pin      GPIO_Pin_14
#define USART3_NREMAP_TXD_Pin      GPIO_Pin_10
#define USART3_NREMAP_RXD_Pin      GPIO_Pin_11
#define USART3_NREMAP_CK_Pin        GPIO_Pin_12

#define USART3_REMAP_GPIO         GPIOD
#define USART3_REMAP_CTS_Pin      GPIO_Pin_3
#define USART3_REMAP_RTS_Pin      GPIO_Pin_4
#define USART3_REMAP_TXD_Pin      GPIO_Pin_5
#define USART3_REMAP_RXD_Pin      GPIO_Pin_6
#define USART3_REMAP_CK_Pin        GPIO_Pin_7

#define USART_TXD_Mode             GPIO_Mode_AF_PP
#define USART_RxD_Mode             GPIO_Mode_AIN
#define RCC_USART1_CLOCK           RCC_APB2Periph_GPIOA
#define RCC_USART2_CLOCK           RCC_APB2Periph_GPIOA
#define RCC_USART3_CLOCK           RCC_APB2Periph_GPIOB
/****************************Buffer definition*********************************/






/********************functions declaration***********************/

void USART_Config();
void USART_GPIO_Config();
void USART_RECEIVEBUFFER(USART_TypeDef *USARTx, uint8_t *ReceiveBuffer, uint8_t BufferSize, uint8_t timeout);
uint8_t USART_RECEIVECHAR(USART_TypeDef *USARTx);
void USART_SENDBUFFER(USART_TypeDef *USARTx, uint8_t *Buffer);
void USART_SENDCHAR(USART_TypeDef *USARTx, uint8_t *Data);
void USART_SENDString(USART_TypeDef *USARTx, char Buffer[]);
void putc ( void* p, char c);
#endif // USR_USART_H

