/*
******************************************************************************
File:     main.c
Info:     Generated by Atollic TrueSTUDIO(R) 9.0.0   2018-03-05

The MIT License (MIT)
Copyright (c) 2018 STMicroelectronics

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

******************************************************************************
*/

/* Includes */
#include <stddef.h>
#include "string.h"
#include "usr_usart.h"
#include "usr_can.h"
#include "stm32f4xx.h"
#include "printf.h"
#include "stm32f4xx_it.h"

/* Private macro */
/* Private variables */
 USART_InitTypeDef USART_InitStructure;
#define CAN_Text_Mode_Normal "Normal mode"
#define CAN_Text_Mode_LoopBack "Loopback mode"
#define CAN_Text_Mode_Silence "Silence mode"
 enum {
 	CAN_Normal=0,CAN_Loopback=2,CAN_Silent=1
 }CAN_Mode;

 int can_mode=0, mailbox=0;
 char *can_mode_text=CAN_Text_Mode_Normal;
/* Private function prototypes */
 void NVIC_Configuration(void);
/* Private functions */
 void GPIO_Config()
 {
	 GPIO_InitTypeDef GPIO_InitStructure;
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);

	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);



 }


/**
**===========================================================================
**
**  Abstract: main program
**
**===========================================================================
*/
int main(void)
{
	int i=0;
	SysTick_Init();
	GPIO_Config();
	NVIC_Configuration();
	USART_Config();
	init_printf(NULL,putc);
	usr_CAN_Config();


  /* TODO - Add your application code here */
	printf("\nCAN testing start.\n");
  /* Infinite loop */
	Delay_ms(10);
	while (1)
  {

	  if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)!=RESET){
		  //switch CAN mode
		  can_mode=(can_mode+1)%3;

		  if(can_mode ==CAN_Normal)
		  {
			  //Normal mode settings
			  if(CAN_OperatingModeRequest(CAN1,CAN_OperatingMode_Initialization)!=CAN_ModeStatus_Failed){
					CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;
					usr_CanTxMessage.RTR=CAN_RTR_Remote;// request data from slave
								if(CAN_Init(CAN1,&CAN_InitStructure)==CAN_InitStatus_Success){
									 printf("CAN mode : Normal \n");
									 can_mode_text=CAN_Text_Mode_Normal;
									 //reset received buffer
									 CAN_FIFORelease(USR_CAN,USR_FIFO);

								}

			  }
			  else{printf("CAN mode request fail\n");}

		  }
		  else if(can_mode==CAN_Loopback){
			  //Loop back mode settings
			if(CAN_OperatingModeRequest(CAN1,CAN_OperatingMode_Initialization)!=CAN_ModeStatus_Failed)
			{
				CAN_InitStructure.CAN_Mode=CAN_Mode_LoopBack;
				usr_CanTxMessage.RTR=CAN_RTR_Data;		//send data to test loop back mode
				if(CAN_Init(CAN1,&CAN_InitStructure)==CAN_InitStatus_Success){
				  printf("CAN mode : LoopBack \n");
				  can_mode_text=CAN_Text_Mode_LoopBack;
				  CAN_FIFORelease(USR_CAN,USR_FIFO);

				}
			}
			 else{printf("CAN mode request fail\n");}
		  }
		  else{
			  // Silence mode
		  			if(CAN_OperatingModeRequest(CAN1,CAN_OperatingMode_Initialization)!=CAN_ModeStatus_Failed)
		  			{
		  				CAN_InitStructure.CAN_Mode=CAN_Mode_Silent;
		  				usr_CanTxMessage.RTR=CAN_RTR_Remote;
		  				if(CAN_Init(CAN1,&CAN_InitStructure)==CAN_InitStatus_Success){
		  				  printf("CAN mode : Silence \n");
		  				can_mode_text=CAN_Text_Mode_Silence;
		  				 CAN_FIFORelease(USR_CAN,USR_FIFO);

		  				}
		  			}
		  			 else{printf("CAN mode request fail\n");}
		  		  }
	  }
	 Delay_ms(800);

	  //if it is in Loopback or normal mode, send data periodically
	  if(can_mode==CAN_Loopback || can_mode==CAN_Normal){
	  	 mailbox= CAN_Transmit(USR_CAN,&usr_CanTxMessage);
	  	 i=0;
	  	 while(CAN_TransmitStatus(USR_CAN,mailbox)!=CAN_TxStatus_Ok && i<0xFF){
		Delay_ms(2);
		i++;
	  	 }
		printf("TX state: %d---------mode: %s\n",CAN_TransmitStatus(USR_CAN,mailbox),can_mode_text);
		Delay_ms(1000);
	  }




  }
}



void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_4);
	/* Enable CAN1 RX0 interrupt IRQ channel */
	NVIC_InitStructure.NVIC_IRQChannel=CAN1_RX0_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

void CAN1_RX0_IRQHandler(void)
{

	int i=0,pendingMessg=0,j=0;
		if(CAN_GetITStatus(USR_CAN,CAN_IT_FMP0)!=RESET){

			pendingMessg= CAN_MessagePending(USR_CAN,USR_FIFO);
			printf("CAN Master receive interrupt:%d\n",pendingMessg);
			for(j=0;j<pendingMessg;j++){
			CAN_Receive(USR_CAN,USR_FIFO,&usr_CanRxMessage);
			printf("remote STD: %d, RTR:%d  DLC:%d-------Mode: %s\n",usr_CanRxMessage.StdId, usr_CanRxMessage.RTR,usr_CanRxMessage.DLC,can_mode_text);
			//check if it is a data request
			//if it is not a request
			if(usr_CanRxMessage.RTR==CAN_RTR_DATA){
			for(i=0;i<8;i++)
			{
				printf("%d ",usr_CanRxMessage.Data[i]);
			}
			printf("\n");
			}
			else {
				//if it is in normal mode and received request, then send data
				usr_CanTxMessage.RTR=CAN_RTR_DATA;
				CAN_Transmit(USR_CAN,&usr_CanTxMessage);
				i=0;
				while(CAN_TransmitStatus(USR_CAN,mailbox)!=CAN_TxStatus_Ok && i<0xFF)i++;
				printf("TX state: %d---------mode: %s\n",CAN_TransmitStatus(USR_CAN,mailbox),CAN_Text_Mode_Normal);
			}
			}
					CAN_ClearITPendingBit(USR_CAN,CAN_IT_FMP0);
		}


}

