/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "stm32f10x_flash.h"
#include "string.h"
#include "main.h"
#include "bsp_usart.h"
#include "bsp_timer.h"
#include "spi.h"

/* Private typedef -----------------------------------------------------------*/
max31855_status i;
float thermocouple1, temperature1,thermocouple2, temperature2, thermocouple3, temperature3,thermocouple4, temperature4,thermocouple5, temperature5,thermocouple6, temperature6, thermocouple7, temperature7,thermocouple8, temperature8, thermocouple9, temperature9,thermocouple10, temperature10,thermocouple11, temperature11, thermocouple12, temperature12,thermocouple13, temperature13,thermocouple14, temperature14, thermocouple15, temperature15,thermocouple16, temperature16;
int16_t TC1,TC2,TC3,TC4,TC5,TC6,TC7,TC8,TC9,TC10,TC11,TC12,TC13,TC14,TC15,TC16,ICT1,ICT2,ICT3,ICT4,ICT5,ICT6,ICT7,ICT8,ICT9,ICT10,ICT11,ICT12,ICT13,ICT14,ICT15,ICT16;
static uint8 Buffer[50]={0};

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

void ReportMessage()
{ 
  Buffer[0] = USART1MemoryBuffer[0];
  Buffer[1] = USART1MemoryBuffer[1];
  
  if(1 == USART1MemoryBuffer[5]){
    Buffer[2] = 2;
    if(0 == USART1MemoryBuffer[3]){      
      Buffer[3] = TC1 >> 8;
      Buffer[4] = TC1;
    }
    if(1 == USART1MemoryBuffer[3]){
      Buffer[3] = TC2 >> 8;
      Buffer[4] = TC2;
    }  
    if(2 == USART1MemoryBuffer[3]){
      Buffer[3] = TC3 >> 8;
      Buffer[4] = TC3;
    }
    if(3 == USART1MemoryBuffer[3]){
      Buffer[3] = TC4 >> 8;
      Buffer[4] = TC4;
    }
    if(4 == USART1MemoryBuffer[3]){
      Buffer[3] = TC5 >> 8;
      Buffer[4] = TC5;
    }
    if(5 == USART1MemoryBuffer[3]){
      Buffer[3] = TC6 >> 8;
      Buffer[4] = TC6;
    }
    if(6 == USART1MemoryBuffer[3]){
      Buffer[3] = TC7 >> 8;
      Buffer[4] = TC7;
    }
    if(7 == USART1MemoryBuffer[3]){
      Buffer[3] = TC8 >> 8;
      Buffer[4] = TC8;
    }
    if(8 == USART1MemoryBuffer[3]){
      Buffer[3] = TC9 >> 8;
      Buffer[4] = TC9;
    }
    if(9 == USART1MemoryBuffer[3]){
      Buffer[3] = TC10 >> 8;
      Buffer[4] = TC10;
    }
    if(10 == USART1MemoryBuffer[3]){
      Buffer[3] = TC11 >> 8;
      Buffer[4] = TC11;
    }
    if(11 == USART1MemoryBuffer[3]){
      Buffer[3] = TC12 >> 8;
      Buffer[4] = TC12;
    }
    if(12 == USART1MemoryBuffer[3]){
      Buffer[3] = TC13 >> 8;
      Buffer[4] = TC13;
    }
    if(13 == USART1MemoryBuffer[3]){
      Buffer[3] = TC14 >> 8;
      Buffer[4] = TC14;
    }
    if(14 == USART1MemoryBuffer[3]){
      Buffer[3] = TC15 >> 8;
      Buffer[4] = TC15;
    }
    if(15 == USART1MemoryBuffer[3]){
      Buffer[3] = TC16 >> 8;
      Buffer[4] = TC16;
    }
    
    Buffer[5] = MBcrc16(Buffer, 5)&0XFF;
    Buffer[6] = (MBcrc16(Buffer, 5)>>8)&0XFF;  
    SendMessage(Buffer,7);
  }
  else if(0x10== USART1MemoryBuffer[5]){  
    Buffer[2] = 32;
    Buffer[3] = TC1 >> 8;
    Buffer[4] = TC1;
    Buffer[5] = TC2 >> 8;
    Buffer[6] = TC2;
    Buffer[7] = TC3 >> 8;
    Buffer[8] = TC3;
    Buffer[9] = TC4 >> 8;
    Buffer[10] = TC4;
    Buffer[11] = TC5 >> 8;
    Buffer[12] = TC5;
    Buffer[13] = TC6 >> 8;
    Buffer[14] = TC6;
    Buffer[15] = TC7 >> 8;
    Buffer[16] = TC7;
    Buffer[17] = TC8 >> 8;
    Buffer[18] = TC8;
    Buffer[19] = TC9 >> 8;
    Buffer[20] = TC9;
    Buffer[21] = TC10 >> 8;
    Buffer[22] = TC10;
    Buffer[23] = TC11 >> 8;
    Buffer[24] = TC11;
    Buffer[25] = TC12 >> 8;
    Buffer[26] = TC12;
    Buffer[27] = TC13 >> 8;
    Buffer[28] = TC13;
    Buffer[29] = TC14 >> 8;
    Buffer[30] = TC14;
    Buffer[31] = TC15 >> 8;
    Buffer[32] = TC15;
    Buffer[33] = TC16 >> 8;
    Buffer[34] = TC16;
    
    Buffer[35] = MBcrc16(Buffer, 35)&0XFF;
    Buffer[36] = (MBcrc16(Buffer, 35)>>8)&0XFF;  
    SendMessage(Buffer,37);
  }    
}

int main(void)
{  

  SystemInit();
  bsp_InitTimer();    
  Timer2_Init(19, 2399);        // 计数频率 = 72000/(7199+1) = 10KHz，计数到(19+1)为2ms  
  Timer3_Init(19, 2399);        //,2ms also 
            
  SPI1_Init();                  //spi and led gpio 

  bsp_InitUart();   
 
  TIM_Cmd(TIM2, DISABLE); 
  TIM_Cmd(TIM3, DISABLE);  

  while(1)  { 
    /*
    i = max31855_temp_cel(&temperature1,&thermocouple1,1);
    i = max31855_temp_cel(&temperature2,&thermocouple2,2);
    i = max31855_temp_cel(&temperature3,&thermocouple3,3);
    i = max31855_temp_cel(&temperature4,&thermocouple4,4);
    
    i = max31855_temp_cel(&temperature5,&thermocouple5,5);
    i = max31855_temp_cel(&temperature6,&thermocouple6,6);
    i = max31855_temp_cel(&temperature7,&thermocouple7,7);
    i = max31855_temp_cel(&temperature8,&thermocouple8,8);
    
    i = max31855_temp_cel(&temperature9,&thermocouple9,9);
    i = max31855_temp_cel(&temperature10,&thermocouple10,10);
    i = max31855_temp_cel(&temperature11,&thermocouple11,11);
    i = max31855_temp_cel(&temperature12,&thermocouple12,12);
    
    i = max31855_temp_cel(&temperature13,&thermocouple13,13);
    i = max31855_temp_cel(&temperature14,&thermocouple14,14);
    i = max31855_temp_cel(&temperature15,&thermocouple15,15);
    i = max31855_temp_cel(&temperature16,&thermocouple16,16);
    */
    i = max31855_temp_int16(&ICT1,&TC1,1);
      
    if(1 == HaveReceivedNewCommand){
      HaveReceivedNewCommand = 0;
      ReportMessage();      
    }
    
    i = max31855_temp_int16(&ICT2,&TC2,2);
    
    if(1 == HaveReceivedNewCommand){
      HaveReceivedNewCommand = 0;
      ReportMessage();      
    }
    
    i = max31855_temp_int16(&ICT3,&TC3,3);
    
    if(1 == HaveReceivedNewCommand){
      HaveReceivedNewCommand = 0;
      ReportMessage();      
    }
    
    i = max31855_temp_int16(&ICT4,&TC4,4);
    
    if(1 == HaveReceivedNewCommand){
      HaveReceivedNewCommand = 0;
      ReportMessage();      
    }
    
    i = max31855_temp_int16(&ICT5,&TC5,5);
    
    if(1 == HaveReceivedNewCommand){
      HaveReceivedNewCommand = 0;
      ReportMessage();      
    }
    i = max31855_temp_int16(&ICT6,&TC6,6);
    
    if(1 == HaveReceivedNewCommand){
      HaveReceivedNewCommand = 0;
      ReportMessage();      
    }
    i = max31855_temp_int16(&ICT7,&TC7,7);
    
    if(1 == HaveReceivedNewCommand){
      HaveReceivedNewCommand = 0;
      ReportMessage();      
    }
    
    i = max31855_temp_int16(&ICT8,&TC8,8);
    
    if(1 == HaveReceivedNewCommand){
      HaveReceivedNewCommand = 0;
      ReportMessage();      
    }
    
    i = max31855_temp_int16(&ICT9,&TC9,9);
    
    if(1 == HaveReceivedNewCommand){
      HaveReceivedNewCommand = 0;
      ReportMessage();      
    }
    
    i = max31855_temp_int16(&ICT10,&TC10,10);
    
    if(1 == HaveReceivedNewCommand){
      HaveReceivedNewCommand = 0;
      ReportMessage();      
    }
    
    i = max31855_temp_int16(&ICT11,&TC11,11);
    
    if(1 == HaveReceivedNewCommand){
      HaveReceivedNewCommand = 0;
      ReportMessage();      
    }
    
    i = max31855_temp_int16(&ICT12,&TC12,12);
    
    if(1 == HaveReceivedNewCommand){
      HaveReceivedNewCommand = 0;
      ReportMessage();      
    }
    
    i = max31855_temp_int16(&ICT13,&TC13,13);
    
    if(1 == HaveReceivedNewCommand){
      HaveReceivedNewCommand = 0;
      ReportMessage();      
    }
    
    i = max31855_temp_int16(&ICT14,&TC14,14);
    
    if(1 == HaveReceivedNewCommand){
      HaveReceivedNewCommand = 0;
      ReportMessage();      
    }
    
    i = max31855_temp_int16(&ICT15,&TC15,15);
    
    if(1 == HaveReceivedNewCommand){
      HaveReceivedNewCommand = 0;
      ReportMessage();      
    }
    
    i = max31855_temp_int16(&ICT16,&TC16,16);
    
    if(1 == HaveReceivedNewCommand){
      HaveReceivedNewCommand = 0;
      ReportMessage();      
    }
  }   
}