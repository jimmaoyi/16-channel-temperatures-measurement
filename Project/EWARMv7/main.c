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

uint8 HaveReceivedNewCommand = 1;
uint16 Display_coinNum = 0 ;

DisplayRefreshLedStatuse_TypeDef RefreshLedStatuse; //定义枚举变量刷新LED的显示状态

  max31855_status i;
  float thermocouple1, temperature1,thermocouple2, temperature2, thermocouple3, temperature3,thermocouple4, temperature4,thermocouple5, temperature5,thermocouple6, temperature6, thermocouple7, temperature7,thermocouple8, temperature8, thermocouple9, temperature9,thermocouple10, temperature10,thermocouple11, temperature11, thermocouple12, temperature12,thermocouple13, temperature13,thermocouple14, temperature14, thermocouple15, temperature15,thermocouple16, temperature16;

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
   //i = max31855_temp_fah(&temperature,&thermocouple);
  }   
}

/******************* (C) COPYRIGHT 2015 WuLianGroup ********END OF FIL*********/ 


