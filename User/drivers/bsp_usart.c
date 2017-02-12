/* Includes ------------------------------------------------------------------*/

#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "bsp_usart.h"
#include <stdio.h>
#include <string.h>
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8 Count1 = 0;
uint8 Count2 = 0;
/* Private function prototypes -----------------------------------------------*/ 
uint8 USART1ReceiveBuffer[50] = {0};
uint8 USART1MemoryBuffer[50] = {0};



/* Private functions ---------------------------------------------------------*/
uint16 MBcrc16(uint8 *ptr, int len);
void SendMessage(uint8 *Buffer, uint8 length);

/*
*********************************************************************************************************
*	函 数 名: bsp_InitUart
*	功能说明: 初始化CPU的USART1串口硬件设备
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitUart(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* 第1步：打开GPIO和USART部件的时钟 */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
  
#ifdef  BSP_USART1
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  
  /* 第2步：将USART Tx的GPIO配置为推挽复用模式 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0 ;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;				
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				
  NVIC_Init(&NVIC_InitStructure);	
  
  
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART1, &USART_InitStructure);
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接收中断
  
  USART_Cmd(USART1, ENABLE);
  /* 
  CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
  如下语句解决第1个字节无法正确发送出去的问题：
  清发送完成标志，Transmission Complete flag 
  */
  USART_ClearFlag(USART1, USART_FLAG_TC);
#endif  

}

/**
* @fun    void USART1_IRQHandler
* @brief  串口1中断处理函数
*         2015/12/23 星期三,Administrator
* @param  None
*
* @retval 
*/
void USART1_IRQHandler(void)
{
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)           //&& 0 == HaveReceivedNewCommand)
  {
    USART1ReceiveBuffer[Count1] = USART_ReceiveData(USART1);
    Count1++;
 
    if((Count1 == 1) && (USART1ReceiveBuffer[0] != 0xaa))
    {
      Count1 = 0;
    }  
    
    if(Count1 > 5)                  //PACKAGE LESS 15 BYTES
    { 
      //received respond from esp
      if(0x1== USART1ReceiveBuffer[3] && USART1ReceiveBuffer[1]==0X55)
      {
        //RFID HAS BEEN SENT TO CLOUD SERVER，respond received
        Count1 = 0;
        HaveReceivedNewCommand = 1;   
      
      }         //NOT FOLLOW ELSE!!
     
      //received IP info  
      if(0x2== USART1ReceiveBuffer[3] && USART1ReceiveBuffer[1]==0X55)
      {
        if((USART1ReceiveBuffer[4] + 5) == Count1)
        { 
          //IP info received
          memcpy(USART1MemoryBuffer,&USART1ReceiveBuffer[5],USART1ReceiveBuffer[4]);
          memset(USART1ReceiveBuffer,0,Count1); 
          Count1 = 0;
          HaveReceivedNewCommand = 2;   
        } 
      }         //NOT FOLLOW ELSE!!
      
     //received coinNum to display  
      if(0x3== USART1ReceiveBuffer[3] && USART1ReceiveBuffer[1]==0X55)
      {
        if((USART1ReceiveBuffer[4] + 5) == Count1)
        { 
          //IP info received
          memcpy(USART1MemoryBuffer,&USART1ReceiveBuffer[5],USART1ReceiveBuffer[4]);
          memset(USART1ReceiveBuffer,0,Count1); 
          Count1 = 0;
          HaveReceivedNewCommand = 3;   
        } 
      }         //NOT FOLLOW ELSE!!
      
    }  
    
    if(Count1 > 15)                     //PACKAGE MORE THAN 15 BYTES
    { 
      //received RFIDpay info
      if(0x10== USART1ReceiveBuffer[3] && USART1ReceiveBuffer[1]==0X55)
      {
        if((USART1ReceiveBuffer[4] + 5) == Count1)
        {   
            //received RFIDpay info
            memcpy(USART1MemoryBuffer,&USART1ReceiveBuffer[5],USART1ReceiveBuffer[4]);
            memset(USART1ReceiveBuffer,0,Count1); 
            Count1 = 0;
            HaveReceivedNewCommand = 15;        
        }
      }
      else
      {
          Count1 = 0;
      }
      
      //received PULSEset info  0x11
    }
  }
}

/**
* @fun    uint16 MBcrc16
* @brief  16位CRC校验
*         2015/12/23 星期三,Administrator
* @param  uint8 *ptr
* @param  int len
*
* @retval CRC校验值
*/
uint16 MBcrc16(uint8 *ptr, int len)
{
  uint8 i;
  uint16 crc = 0xffff;
  if(len == 0)
  {
    len = 1;
  }
  while(len--)
  {
    crc ^= *ptr;
    for(i = 0; i < 8; i++)
    {
      if(crc & 1)
      {
        crc>>=1;
        crc ^= 0xa001;		
      }
      else
      {
        crc >>=1;
      }
    }
    ptr++;
  }
  return (crc);
}


/**
* @fun    void InitativeReportMessage(StatusMsgTypeDef StatusMsg,uint8 FunctionCode)
* @brief  主动向2530上报当前状态信息
*         2015/12/23 星期三,Administrator
* @param  
* @param  
* @param    
*
* @retval 
*/
uint8 Buffer[32]={0};

void InitativeReportMessage(uint8 *preSendData,uint8 FunctionCode)
{
  Buffer[0] = 0XAA;
  Buffer[1] = 0X55;
  Buffer[2] = 0X01;
  Buffer[3] = FunctionCode;
  Buffer[4] = 16;    //data length
  
  Buffer[5] = preSendData[0];
  Buffer[6] = preSendData[1];
  
  Buffer[7] = preSendData[2];
  Buffer[8] = preSendData[3];
  
  Buffer[9] = preSendData[4];
  Buffer[10] = preSendData[5]; 
  Buffer[11] = preSendData[6];
  Buffer[12] = preSendData[7];
  
  Buffer[13] = preSendData[8];
  Buffer[14] = preSendData[9];
  
  Buffer[15] = preSendData[10];
  Buffer[16] = preSendData[11];
  Buffer[17] = preSendData[12];
  Buffer[18] = preSendData[13];
  
  Buffer[19] = preSendData[14];
  Buffer[20] = preSendData[15];
  
  Buffer[21] = MBcrc16(Buffer, 21)&0XFF;
  Buffer[22] = (MBcrc16(Buffer, 21)>>8)&0XFF;
  
  SendMessage(Buffer,23); 
}

void SendMessage(uint8 *Buffer1, uint8 length)
{
  uint8 i;
  for(i=0;i<length;i++)
  {
    USART_SendData(USART1,Buffer1[i]);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
  }
}

void MOD_TC_normal_respond(uint8* temp_data,uint8 FunctionCode)
{
  Buffer[0] = 0XAA;
  Buffer[1] = 0X55;
  Buffer[2] = 0X01;
  Buffer[3] = FunctionCode;
  Buffer[4] = temp_data[0];		
  Buffer[5] = temp_data[1];		//-数据长度
  Buffer[6] = temp_data[2];
  Buffer[7] = temp_data[3];
  
  //-CRC16
  Buffer[8] = MBcrc16(Buffer, 8)&0XFF;
  Buffer[9] = (MBcrc16(Buffer, 8)>>8)&0XFF;
  
  SendMessage(Buffer,10);
  
}

