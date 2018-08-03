#ifndef USR_CAN_H
#define USR_CAN_H

#include "stm32f4xx_can.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "usr_usart.h"
#include "printf.h"
#include "string.h"
#include "systick.h"

//privacy definition
#define CAN1Port			GPIOA			//remap CAN1 to PD0, PD1
#define CAN2Port			GPIOB
											//without using remapping: CAN_RX PA11, CAN_TX PA12
#define CAN1Rx_Pin			GPIO_Pin_11		//use GPIO remap2: CAN_RX PD0, CAN_TX PD1
#define CAN1Tx_Pin			GPIO_Pin_12
#define CAN1RX_PinSource	GPIO_PinSource11
#define CAN1TX_PinSource		GPIO_PinSource12

#define CAN2Rx_Pin			GPIO_Pin_12
#define CAN2Tx_Pin			GPIO_Pin_13
#define USR_FIFO			CAN_FIFO0		//set CAN FIFO to FIFO0
#define USR_CAN				CAN1
#define MAXRECBUFFER		30
//privacy variables
CanRxMsg usr_CanRxMessage;
CanTxMsg usr_CanTxMessage;
uint8_t  usr_CanRxData[MAXRECBUFFER];	//8-byte buffer for CAN data field
uint8_t  usr_CanTxData[8];
CAN_InitTypeDef CAN_InitStructure;

//CAN related functions
void usr_CAN_Config();
void usr_CAN_RecMessg(CAN_TypeDef *CANx, CanRxMsg *RxMessage);
uint8_t usr_CAN_SendMessg(CAN_TypeDef *CANx,CanTxMsg *TxMessage);
void usr_CAN_SetTxData(CanTxMsg *Message, uint8_t *Data, uint8_t DataLen);
void usr_CAN_SetMessgFrame(CanTxMsg *TxMessage,uint32_t stdID, uint32_t ExID,  uint8_t IDE, uint8_t RTR, uint8_t DataLen,uint8_t *Data );
uint8_t usr_CANWaitTx(CAN_TypeDef *CANx, uint8_t mailbox);
uint8_t Is_CANRxValid(CanRxMsg *RxMessage,uint32_t id);
void usr_CAN_SendTexts(CAN_TypeDef *CANx,CanTxMsg *TxMessage, const char *Text );
void usr_CAN_RecTexts(CAN_TypeDef *CANx, CanRxMsg *RxMessage, uint8_t *DataBuffer, uint8_t buffer_size);

#endif