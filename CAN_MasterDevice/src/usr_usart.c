#include "usr_usart.h"





void putc ( void* p, char c)
	{
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}
	USART_SendData(USART1, c);
	}


/**
 * @brief USART_GPIO_Config
 */
void USART_GPIO_Config()
{

	GPIO_InitTypeDef GPIO_Struct;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
    /*********************USART1 RXD***************************/
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);

    GPIO_Struct.GPIO_Mode=GPIO_Mode_IN;
     GPIO_Struct.GPIO_Pin=USART1_RXD_Pin;
     GPIO_Struct.GPIO_Speed=GPIO_Speed_50MHz;
     GPIO_Struct.GPIO_PuPd=GPIO_PuPd_NOPULL;
     GPIO_Struct.GPIO_OType=GPIO_OType_PP;

     GPIO_Init(GPIOA,&GPIO_Struct);


    /*********************USART1 TXD***************************/
     GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
     GPIO_Struct.GPIO_Mode=GPIO_Mode_AF;
     GPIO_Struct.GPIO_Pin=USART1_TXD_Pin;
     GPIO_Struct.GPIO_Speed=GPIO_Speed_50MHz;
     GPIO_Struct.GPIO_PuPd=GPIO_PuPd_NOPULL;
     GPIO_Struct.GPIO_OType=GPIO_OType_PP;

     GPIO_Init(GPIOA,&GPIO_Struct);
}
/**
 * @brief USART_Config
 */

void USART_Config()
{
    USART_InitTypeDef USART1_InitStruct;
	GPIO_InitTypeDef GPIO_Struct;
//    USART_ClockInitTypeDef USART_Struct;

	/*********************USART1 RXD***************************/

	USART_GPIO_Config();
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1,ENABLE);
	/*Initailize Usart1 */
    USART1_InitStruct.USART_BaudRate=115200;
    USART1_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
    USART1_InitStruct.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
    USART1_InitStruct.USART_Parity=USART_Parity_No;
    USART1_InitStruct.USART_StopBits=USART_StopBits_1;
    USART1_InitStruct.USART_WordLength=USART_WordLength_8b;
    USART_Init(USART1,&USART1_InitStruct);
    USART_DMACmd(USART1,USART_DMAReq_Rx|USART_DMAReq_Tx,ENABLE);
    /***************USART DMA ENABLE***************/
    USART1->CR3|=USART_DMAReq_Rx|USART_DMAReq_Tx;
    USART_ITConfig(USART1,USART_IT_IDLE,ENABLE); //enable idle interrupt
    USART_ClearFlag(USART1,USART_FLAG_TC);

    /***********Enable USART1***************/

    USART1->CR1|= USART_CR1_UE;

}


/**
 * @brief USART1_SENDBUFFER
 * @param USARTx
 * @param Buffer
 */
void USART_SENDCHAR(USART_TypeDef *USARTx, uint8_t *Data)
{

    /****************check whether transmition is completed*********************/
        //waiting for sending completed
        while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)==RESET);
      USART_SendData(USARTx,*Data);


}


/**
 * @brief USART1_SENDBUFFER
 * @param USARTx
 * @param Buffer
 */
void USART_SENDBUFFER(USART_TypeDef *USARTx, uint8_t *Buffer)
{

    /****************check whether transmition is completed*********************/
    while(*Buffer!='\0')
    {
        //waiting for sending completed
        while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)==RESET);
      USART_SENDCHAR(USARTx,Buffer);
       Buffer++;

     }

}



/**
 * @brief USART_SENDString
 * @param USARTx
 * @param Buffer
 */
void USART_SENDString(USART_TypeDef *USARTx, char Buffer[])
{

    /****************check whether transmition is completed*********************/
    while(*Buffer!='\0')
    {
        //waiting for sending completed
        while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)==RESET){}
            USART_SendData(USARTx,*Buffer);
            Buffer++;

     }

}


/**
 * @brief USART1_RECEIVEBUFFER
 * @param USARTx
 * @return
 */
uint8_t USART_RECEIVECHAR(USART_TypeDef *USARTx)
{
    uint8_t data;
    //waiting or receive completed
    while(USART_GetFlagStatus(USARTx,USART_FLAG_RXNE)==RESET);

    data=USARTx->DR;

    return data;
}


void USART_RECEIVEBUFFER(USART_TypeDef *USARTx, uint8_t *ReceiveBuffer, uint8_t BufferSize, uint8_t timeout)
{

    uint8_t data=0;
    uint8_t bfcounter=0;
//    uint8_t *p;
    //waiting for data

  /*  p=ReceiveBuffer;*/ // return the pointer to the RecBuffer[0]
    while(1)
    {
        /******************start counter if it doesn't get data***********************/
        if(USART_GetFlagStatus(USARTx,USART_FLAG_RXNE)!=RESET)
        {

            if(bfcounter<BufferSize-1) //check whether data out of buffer
            {

                data=USARTx->DR;//receive data
               *ReceiveBuffer=data;

                if(data=='\0')
                {

                    break;
                }
                ReceiveBuffer++;
                bfcounter++;

            }

        }
    }



}



