/* Includes ------------------------------------------------------------------*/

#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "bsp_usart.h"
#include <stdio.h>
#include <string.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint8 Count1 = 0 , serialNum = 0 ;
static uint8 USART1ReceiveBuffer[10] = {0};
/* Private function prototypes -----------------------------------------------*/ 
uint8 USART1MemoryBuffer[10] = {0};
uint8 HaveReceivedNewCommand = 0;
/* Private functions ---------------------------------------------------------*/

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
 
  if(0 == GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3))
      serialNum += 1;
  
  if(0 == GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12))
      serialNum += 2;
  
  if(0 == GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2))
      serialNum += 4;
  
  if(0 == GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1))
      serialNum += 8;
  
  if(0 == GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0))
      serialNum += 32;
  
  if(0 == GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5))
      serialNum += 64;
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
 
    if((Count1 == 1) && (USART1ReceiveBuffer[0] != 0xfe))
    {
      Count1 = 0;
    }  
    
    if(Count1 > 7)                 
    {       
      if((4 == USART1ReceiveBuffer[1]) && (serialNum == USART1ReceiveBuffer[2]))
      //if(4 == USART1ReceiveBuffer[1])
      {
        memcpy(USART1MemoryBuffer,&USART1ReceiveBuffer,8);  
        memset(USART1ReceiveBuffer,0,8); 
        Count1 = 0;     
        HaveReceivedNewCommand = 1;
      }         
      else
      {
        Count1 = 0;
      }     
    }  
  }
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