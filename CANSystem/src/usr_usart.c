#include "stm32f10x.h"
#include "usr_usart.h"
#include "systick.h"
uint8_t USART1_Request_Update_Flag=0;

/**
 * @brief USART_Setup
 *
 */
void putc ( void* p, char c)
	{
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}
	USART_SendData(USART1, c);
	}

void USART_Setup()
{


    USART_Clock_Config();
    USART_GPIO_Config();
    USART_Config();




}

/**
 * @brief USART_Clock_Config
 */
void USART_Clock_Config()
{
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_AFIO, ENABLE);
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
}


/**
 * @brief USART_GPIO_Config
 *  GPIO:
 *  PA10: RXD in floating 50MHz
 *  PA9 :TXD out PP 50MHz
 */
void USART_GPIO_Config()
{


    GPIO_InitTypeDef GPIO_Struct;
    /*********************USART1 RXD***************************/
     GPIO_Struct.GPIO_Mode=GPIO_Mode_IN_FLOATING;
     GPIO_Struct.GPIO_Pin=GPIO_Pin_10;
     GPIO_Struct.GPIO_Speed=GPIO_Speed_50MHz;
     GPIO_Init(GPIOA,&GPIO_Struct);
    /*********************USART1 TXD***************************/
     GPIO_Struct.GPIO_Mode=GPIO_Mode_AF_PP;
     GPIO_Struct.GPIO_Pin=GPIO_Pin_9;
     GPIO_Struct.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_Struct);

}
/**
 * @brief USART_Config
 */

void USART_Config()
{
    /*Set the USART to 115200 band rate, no parity, stop bit 1,
     * 8 bit length
       */
    USART_InitTypeDef USART1_InitStruct;
//    USART_ClockInitTypeDef USART_Struct;
    USART_Clock_Config();
    USART_GPIO_Config();
    USART1_InitStruct.USART_BaudRate=115200;
    USART1_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
    USART1_InitStruct.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
    USART1_InitStruct.USART_Parity=USART_Parity_No;
    USART1_InitStruct.USART_StopBits=USART_StopBits_1;
    USART1_InitStruct.USART_WordLength=USART_WordLength_8b;
    USART_Init(USART1,&USART1_InitStruct);
//    USART_DMACmd(USART1,USART_DMAReq_Rx|USART_DMAReq_Tx,ENABLE);
    /***************USART DMA ENABLE***************/
//    USART1->CR3|=USART_DMAReq_Rx|USART_DMAReq_Tx;
//    USART_ITConfig(USART1,USART_IT_IDLE,ENABLE); //enable idle interrupt
//    USART_ClearFlag(USART1,USART_FLAG_TC);

    /***********Enable USART1***************/
    USART1->CR1 |=USART_CR1_RXNEIE;     //enable RDR not empty interrupt
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
void USART_SENDBUFFER(USART_TypeDef *USARTx, uint8_t *Buffer, int buffersize)
{


    while(*Buffer!='\0')
    {

        if(buffersize<=0) break;
        buffersize--;
        //waiting for sending completed
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
        while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)==RESET);
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


void USART_RECEIVEBUFFER(USART_TypeDef *USARTx, uint8_t *ReceiveBuffer,int8_t BufferSize, uint16_t timeout)
{


    int8_t bfcounter=BufferSize;
    uint8_t temp=timeout;
    uint8_t *cleanbuffer=ReceiveBuffer;
    //clean buffer
    while((int8_t)--BufferSize>=0)
    {
        *(cleanbuffer+BufferSize)='\0';
    }
    BufferSize= bfcounter;
    bfcounter=0;

    while(1)
    {
        /******************start counter if it doesn't get data***********************/
        if(USART_GetFlagStatus(USARTx,USART_FLAG_RXNE)!=RESET)
        {
            timeout=temp;
            if(bfcounter<BufferSize-1) //check whether data out of buffer
            {

               *ReceiveBuffer=USARTx->DR;//receive data

                if(*ReceiveBuffer=='\0')
                {

                    break;
                }
                ReceiveBuffer++;
                bfcounter++;

            }
            else
            {
                //if the data is larger than buffer,clean buffer
                USART_ClearITPendingBit(USARTx,USART_IT_RXNE);

                break;
            }

        }
        else
        {
            if(timeout==0)
            {
                break;
            }
            timeout--;
        }
    }



}

