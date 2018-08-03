#include "usr_can.h"
#include "stm32f10x_rcc.h"
#include "usr_usart.h"
#include "printf.h"
#include "string.h"
#include "systick.h"


static uint8_t Is_CharInStr(uint8_t c, uint8_t *str, int str_len)
{
	for(;str_len>0;str_len--)
	{
		if(c==str[str_len-1]) return 1;
	}
	return 0;
}


static void usr_CAN_GPIOConfig()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);
	//Initialize CAN Rx Pin
	GPIO_InitStructure.GPIO_Pin=CAN1Rx_Pin;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(CAN1Port,&GPIO_InitStructure);
	//Initialize CAN Tx pin
	GPIO_InitStructure.GPIO_Pin=CAN1Tx_Pin;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Init(CAN1Port,&GPIO_InitStructure);
}


/*
 * @ usr_CAN_Config:
 * 		This function is to configure the settings of CAN bus mode, CAN filter mode, GPIO, RCC clock
 *
 * */
void usr_CAN_Config()
{

	int i=0;
	 /****************CAN1 GPIO config***************************/
	usr_CAN_GPIOConfig();

    /****************CAN1  RCC config***************************/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);
    usr_CanTxMessage.DLC=8;
    usr_CanTxMessage.ExtId=0x0000;
    usr_CanTxMessage.StdId=0x0000;
    usr_CanTxMessage.IDE=CAN_Id_Standard;
    usr_CanTxMessage.RTR=CAN_RTR_Data;//if RTR is remote, CAN doesn't send Data bits
    for(i=0;i<8;i++){

    	//clean buffer data
    	usr_CanRxMessage.Data[i]=0;
    	usr_CanTxMessage.Data[i]='K'+i;
    	usr_CanTxData[i]=0;
    	usr_CanRxData[i]=0;
    }

	/*
	 * CAN configuration table：
	 *		-no bus-off management
	 *		-no wake up mode
	 *		-use normal operating mode
	 *		-disable FIFO lock mode, enable new message enter
	 *		-Baud rate =125k
	 *		-open mailbox0 empty interrupt and FIFO0 new message interrupt
	 *	Initialize the CAN_Prescaler member
								  1
		  	BaudRate	= ------------------					= 1/8us = 125KHz
							NominalBitTime

		    NominalBitTime = 1 x tq + tBS1 + tBS2				= 1us + 5usec +2usec = 8us
		    with:
				tBS1 = tq x (TS1[3:0] + 1),						= 5usec
				tBS2 = tq x (TS2[2:0] + 1),						= 3usec
				tq = (BRP[9:0] + 1) x tPCLK                    	= 27.77777nsec x 36 = 1usec
				tPCLK = time period of the APB1 clock, 36MHz   	= 27.77777nsec
				BRP[9:0], TS1[3:0] and TS2[2:0] are defined in the CAN_BTR Register.
				where tq refers to the Time quantum

	 * */
    CAN_DeInit(USR_CAN);
	CAN_InitStructure.CAN_ABOM=DISABLE;			//disable bus-off management
	CAN_InitStructure.CAN_AWUM=DISABLE;			//disable wake up mode
	CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;	//Normal mode
	CAN_InitStructure.CAN_NART=ENABLE;			//disable no auto retransmission mode. CAN transmits data until success
	CAN_InitStructure.CAN_RFLM=DISABLE;			//disable FIFO lock mode
	CAN_InitStructure.CAN_TTCM=DISABLE;			//disable time trigger communication mode
	CAN_InitStructure.CAN_TXFP=DISABLE;			//FIFO transmission priority is based on identifier num
	CAN_InitStructure.CAN_BS1=CAN_BS1_5tq;
	CAN_InitStructure.CAN_BS2=CAN_BS2_2tq;
	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;
	CAN_InitStructure.CAN_Prescaler=35;

	if(CAN_Init(USR_CAN, &CAN_InitStructure)==CAN_InitStatus_Failed)
	{
			printf("CAN initialization Fail!\n");
	}
		else{
			printf("CAN initialization Succeeds!\n");
		}
	CAN_ITConfig(USR_CAN,CAN_IT_FMP0,ENABLE);	//enable interrupt for mailbox0, FIFO0

	/****************CAN Filter initialization*********************/
	/*Here are the settings for a filter, including:
	 * 		FIFO number, Filter number, Filter scale, Mask ID, LIst ID, Filter mode (list mode or mask mode)
	 *If an identifier can pass a filter, the filter number  will be stored in Filter match index (FMI)
	 *For an identifier which can pass several filters, the filter number to be stored in FMI can be chosen
	 *For by following rules:
	 * 1. filter scale priority: 32- bit filter > 16-bit filter
	 * 2. Mode priority: List mode > mask mode
	 * 3. Filter number priority: low number > higher number
	 * 4. priority of rules: Filter Scale > Mode > Filter number
	 * CAN Filter configuration table
	 * 		-Filter numebr =1
	 * 		-scale =32bits
	 * 		-List mode ID= 0x00000000
	 * 		-Mask mode ID=0x00000000
	 * 		-FIFO= FIFO0
	 * 		-mode= mask mode
	 *
	 * */
	CAN_Filter1.CAN_FilterNumber=1;
	CAN_Filter1.CAN_FilterMode=CAN_FilterMode_IdMask;
	CAN_Filter1.CAN_FilterScale=CAN_FilterScale_32bit;
	CAN_Filter1.CAN_FilterIdHigh=0x0000;
	CAN_Filter1.CAN_FilterIdLow=0x0000;
	CAN_Filter1.CAN_FilterMaskIdHigh=0x0000;
	CAN_Filter1.CAN_FilterMaskIdLow=0x0000;
	CAN_Filter1.CAN_FilterFIFOAssignment=USR_FIFO;
	CAN_Filter1.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&CAN_Filter1);


}


void usr_CAN_SetMessgFrame(CanTxMsg *TxMessage,uint32_t stdID, uint32_t ExID,  uint8_t IDE, uint8_t RTR, uint8_t DataLen,uint8_t *Data )
{
	int i=0;
	TxMessage->StdId=stdID;
	TxMessage->ExtId=ExID;
	TxMessage->DLC=DataLen;
	TxMessage->RTR=RTR;	//remote transmission request: send request to another node to obtain the information
	TxMessage->IDE=IDE;	//identifier extension: the bit identify if the identifier is 11-bit or 29-bit
	for(i=0;i<DataLen;i++){
		TxMessage->Data[i]=Data[i];
	}
}

void usr_CAN_SetTxData(CanTxMsg *Message, uint8_t *Data, uint8_t DataLen)
{
	int i=0;
	Message->DLC=DataLen;
	for(i=0; i<DataLen;i++)
		Message->Data[i]=Data[i];
}

uint8_t usr_CANWaitTx(CAN_TypeDef *CANx, uint8_t mailbox)
{
	uint32_t count=0;
	uint8_t state=0;
//	wait for the completion of transmission
	while((state=CAN_TransmitStatus(CANx,mailbox))!=CAN_TxStatus_Ok&& count<0xFF)
	{
		Delay_us(1);
		count++;
	}


	return state;
}
/*
 * @brief:
 * @param CANx: x can be 0 or 1 depending on which CAN you use
 * @param *Data: address of the buffer storing 0~8-byte data
 * @param datalen: specify how many bytes in data buffer need to be sent
 * */
uint8_t usr_CAN_SendMessg(CAN_TypeDef *CANx,CanTxMsg *TxMessage)
{
	uint8_t mailbox;
	uint8_t state;
	/*Process of sending message:
	 * 	load data to buffer--> send request to mail box and wait for empty mailbox --> load data to mailbox and send
	 * 	--> check transmission status.
	 *
	 * */
	mailbox=CAN_Transmit(CANx,TxMessage);	//send Tx request to an empty mailbox
	if(mailbox != CAN_TxStatus_NoMailBox){
		//if it finds an empty mailbox, wait for transmission complete.
		state=usr_CANWaitTx(CANx,mailbox);
		switch(state)
		{
		case (CAN_TxStatus_Failed):
				printf("Transmission Fail !\n");
				break;
		case (CAN_TxStatus_Ok):
				printf("Message Transmitted \n");
				break;
		case (CAN_TxStatus_Pending):
				printf("Transmission is pending for mailbox: %d\n", mailbox);
				break;
		}

	}
	else{
		//if no mailbox is empty
		printf("Can't find an empty mailbox\n");
	}

	return mailbox;
}

/*
 * @brief : this function calculate the length of the texts and break them apart to send messages
 *@param	CANx: use CANx to transmit data. x can be 0 or 1
 *@param	*TxMessage: CAN transmission buffer
 *@param	*Text:	text string data required to transmitted
 *
 * */
void usr_CAN_SendTexts(CAN_TypeDef *CANx,CanTxMsg *TxMessage, const char *Text )
{
	//strlen(Text)+1 return total length of string including '\0'
	uint8_t	packet_num=(strlen(Text)+1)/8;				//the number of the 8-byte packects
	uint8_t size_LastPacket=(strlen(Text)+1)%8;			//the size of the last packet
	//check if the last packet 	is 0 bit.
	uint8_t total_packets=size_LastPacket==0? packet_num:packet_num+1;
	uint8_t *pt=(uint8_t *)Text;
	uint8_t index=0;
	printf("\n\nCAN Transmission Start. Length of data:%d packet: %d\n", strlen(Text)+1, total_packets);
	printf("Transmitting ");
	for(index=0;index<total_packets;index++)
	{
		if(index<packet_num){
			//load the packets to the buffer
			usr_CAN_SetTxData(TxMessage,&pt[8*index],8);
		}
		else{
			//send the last packet whose length may not be 8 bytes.
			usr_CAN_SetTxData(TxMessage,&pt[8*index],size_LastPacket);
		}
		usr_CAN_SendMessg(CANx,TxMessage);
	}
	printf("\nTransmission completed\n\n");

}





uint8_t Is_CANRxValid(CanRxMsg *RxMessage,uint32_t id)
{
	if(RxMessage->StdId!=id)
		return 0;
	else
		return 1;
}




/*
 *
 * */
void usr_CAN_RecMessg(CAN_TypeDef *CANx, CanRxMsg *RxMessage)
{
	int PendingMessage=0;
	/*Process of CAN receive message:
	 * 	message arrived mailbox -> check message validation -> FIFO read mailbox data to buffer
	 * -> filter message
	 *
	 * */
	PendingMessage=CAN_MessagePending(USR_CAN, USR_FIFO);
	printf("\nNumber of message pending: %d\n", PendingMessage);
	for(;PendingMessage>0;)
	{
		CAN_Receive(CANx,USR_FIFO ,RxMessage);
		memcpy((void *)usr_CanRxData, (void *)RxMessage->Data,RxMessage->DLC);
		printf("-->CAN Bus Message: StdID: %d, Data:%s, DLC: %d\n",RxMessage->StdId,
																usr_CanRxData,
																RxMessage->DLC);
		PendingMessage=CAN_MessagePending(USR_CAN, USR_FIFO);

	}

}


/*
 *
 *
 *
 * */
void usr_CAN_RecTexts(CAN_TypeDef *CANx, CanRxMsg *RxMessage, uint8_t *DataBuffer, uint8_t buffer_size)
{
	uint8_t index=0;
	uint8_t *strpt=DataBuffer;
	uint8_t packet_num=0, overflow_flag=0;
	int PendingMessage=0;
	/*Process of CAN receive message:
	 * 	message arrived mailbox -> check message validation -> FIFO read mailbox data to buffer
	 * -> filter message
	 *
	 * */
	PendingMessage=CAN_MessagePending(CANx, USR_FIFO);
	printf("Number of message pending: %d\n", PendingMessage);
	//check if all messages have been read and it reads the end of the text
	while(PendingMessage>0)
	{
		//receive packet
		CAN_Receive(CANx,USR_FIFO ,RxMessage);
		printf("CAN Packet[%d]:\tStdID: %d\tDLC: %d\n",packet_num,RxMessage->StdId,RxMessage->DLC);
		packet_num++;
		//load packet to buffer
		if(overflow_flag!=1){
		if(index<buffer_size){
				//check if the text exceeds the buffer after adding new packet
				if((index+RxMessage->DLC)<buffer_size){
				memcpy((void *)(strpt+index), (void *)RxMessage->Data,RxMessage->DLC);
				//update position to write data
				index+=RxMessage->DLC;
				}
				else{
					//if the text exceed the buffer, copy the remained text and set the last char '\0'
					memcpy((void *)(strpt+index), (void *)RxMessage->Data,(buffer_size-index));
					index=buffer_size;
					strpt[index-1]='\0';
					printf("Buffer is full !\n");
					overflow_flag=1;

				}

		}
		else{
				//if buffer overflow, print alarm message
				printf("Warning:   Buffer overflow! \n");
				//set boundary of the string
				DataBuffer[buffer_size-1]='\0';
				overflow_flag=1;
		}
		}
		else{
			CAN_FIFORelease(CANx,USR_FIFO);
		}

		PendingMessage=CAN_MessagePending(CANx, USR_FIFO);

	}
	printf("Packets amount:%d\n",packet_num);
	printf("----------------------Text -----------------------\n  %s\n",DataBuffer);
	printf("--------------------------------------------------\n");

}


