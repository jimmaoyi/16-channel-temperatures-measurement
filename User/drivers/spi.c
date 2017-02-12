#include "stm32f10x.h"
#include "spi.h"
#include "bsp_timer.h"
//以下是SPI模块的初始化代码，配置成主机模式，访问SD Card/W25X16/24L01/JF24C							  
//SPI口初始化
//这里针是对SPI1的初始化
#define MAX31855_FAULT   (1 << 16)
#define SCV_FAULT        (1 << 2)
#define SCG_FAULT        (1 << 1)
#define OC_FAULT         (1 << 0)

int16_t tmp_int;
int16_t tmp_tc;

void MF_delay(unsigned int ns)
{
  unsigned int i;
  for(;ns>0;ns--);
  {
    for(i=0;i<10;i++);
    //__nop(); 
    
  }
}

void SPI1_Init(void)
{  
  GPIO_InitTypeDef GPIO_InitStructure;    
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 ; //1
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_10 | GPIO_Pin_13 | GPIO_Pin_2 | GPIO_Pin_9 | GPIO_Pin_0 | GPIO_Pin_1  | GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_11 | GPIO_Pin_8 | GPIO_Pin_4 ; //5|2|4|10|
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //
  GPIO_Init(GPIOC, &GPIO_InitStructure); 
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_6 | GPIO_Pin_4 | GPIO_Pin_2 | GPIO_Pin_0 ;  //7|3|11|12|13|14
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_1 | GPIO_Pin_7 | GPIO_Pin_5 | GPIO_Pin_3 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE); 
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE); //禁止JTAG功能
  
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12 | GPIO_Pin_1 |GPIO_Pin_3 | GPIO_Pin_4 |GPIO_Pin_5 |GPIO_Pin_7 |GPIO_Pin_8  |GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_15;;	 //IO-->PF0?￠PF1 ???ú????
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //í?íìê?3?
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO?ú?ù?è?a50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);			 //?ù?Yéè?¨2?êy3?ê??ˉPF0?￠PF1  
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);  //
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_10 | GPIO_Pin_0 | GPIO_Pin_9 | GPIO_Pin_6 ;  //6|8|9|15|16
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //
  GPIO_Init(GPIOB, &GPIO_InitStructure);   
  
  GPIO_SetBits(GPIOB,GPIO_Pin_4);  //1
  GPIO_SetBits(GPIOC,GPIO_Pin_12);  //2
  GPIO_SetBits(GPIOC,GPIO_Pin_10);  //3  
  GPIO_SetBits(GPIOC,GPIO_Pin_9);  //4
  
  GPIO_SetBits(GPIOC,GPIO_Pin_7);  //5
  GPIO_SetBits(GPIOB,GPIO_Pin_15);  //6
  GPIO_SetBits(GPIOB,GPIO_Pin_12);  //7  
  GPIO_SetBits(GPIOB,GPIO_Pin_11);  //8
  
  GPIO_SetBits(GPIOB,GPIO_Pin_1);  //9
  GPIO_SetBits(GPIOC,GPIO_Pin_5);  //10
  GPIO_SetBits(GPIOA,GPIO_Pin_7);  //11
  GPIO_SetBits(GPIOA,GPIO_Pin_5);  //12
  
  GPIO_SetBits(GPIOA,GPIO_Pin_3);  //13
  GPIO_SetBits(GPIOA,GPIO_Pin_1);  //14
  GPIO_SetBits(GPIOC,GPIO_Pin_13);  //15 
  GPIO_SetBits(GPIOB,GPIO_Pin_7);  //16
  
  GPIO_ResetBits(GPIOB,GPIO_Pin_13); 
  
}   

static uint32_t max31855_read(uint8 channal)
{
  int8_t       i;        
  uint32_t      raw_reading = 0 ;
  raw_reading = 0 ;
 
  DelayMs(100);
  switch(channal){
  case 1:
    GPIOB->BRR = GPIO_Pin_4;
    break;  
  case 2:
    GPIOC->BRR = GPIO_Pin_12;
    break;  
  
  case 3:
    GPIOC->BRR = GPIO_Pin_10;
    break;
  case 4:
    GPIOC->BRR = GPIO_Pin_9;
    break;
  case 5:
    GPIOC->BRR = GPIO_Pin_7;
    break;  
  case 6:
    GPIOB->BRR = GPIO_Pin_15;
    break;  
  
  case 7:
    GPIOB->BRR = GPIO_Pin_12;
    break;
  case 8:
    GPIOB->BRR = GPIO_Pin_11;
    break;
  case 9:
    GPIOB->BRR = GPIO_Pin_1;
    break;  
  case 10:
    GPIOC->BRR = GPIO_Pin_5;
    break;  
  
  case 11:
    GPIOA->BRR = GPIO_Pin_7;
    break;
  case 12:
    GPIOA->BRR = GPIO_Pin_5;
    break;
  case 13:
    GPIOA->BRR = GPIO_Pin_3;
    break;  
  case 14:
    GPIOA->BRR = GPIO_Pin_1;
    break;  
  
  case 15:
    GPIOC->BRR = GPIO_Pin_13;
    break;
  case 16:
    GPIOB->BRR = GPIO_Pin_7;
    break;
    
  default:
    break;
  }
  //CLR_SPI_CS;

  MF_delay(1000);

 
  for(i=31;i>=0;i--){
    GPIO_SetBits(GPIOB,GPIO_Pin_13); 
    //SET_SPI_CLK;
    MF_delay(1000);
    
    switch(channal){
    case 1:
      if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2))
      raw_reading |= ((uint32_t)1 << i); 
      break;
    case 2:
      if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_11))
      raw_reading |= ((uint32_t)1 << i); 
      break;
    case 3:
      if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11))
      raw_reading |= ((uint32_t)1 << i); 
      break;
    case 4:
      if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8))
      raw_reading |= ((uint32_t)1 << i); 
      break;
    case 5:
      if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_6))
      raw_reading |= ((uint32_t)1 << i); 
      break;
    case 6:
      if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14))
      raw_reading |= ((uint32_t)1 << i); 
      break;
    case 7:
      if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8))
      raw_reading |= ((uint32_t)1 << i); 
      break;
    case 8:
      if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10))
      raw_reading |= ((uint32_t)1 << i); 
      break;
    case 9:
      if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0))
      raw_reading |= ((uint32_t)1 << i); 
      break;
    case 10:
      if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4))
      raw_reading |= ((uint32_t)1 << i); 
      break;
    case 11:
      if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6))
      raw_reading |= ((uint32_t)1 << i); 
      break;
    case 12:
      if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4))
      raw_reading |= ((uint32_t)1 << i); 
      break;
    case 13:
      if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2))
      raw_reading |= ((uint32_t)1 << i); 
      break;
    case 14:
      if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0))
      raw_reading |= ((uint32_t)1 << i); 
      break;
    case 15:
      if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9))
      raw_reading |= ((uint32_t)1 << i); 
      break;
    case 16:
      if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6))
      raw_reading |= ((uint32_t)1 << i); 
      break;
    
    default:
      break;
    } 
    GPIO_ResetBits(GPIOB,GPIO_Pin_13); 
    //CLR_SPI_CLK;
    MF_delay(1000);
  }

  switch(channal){
  case 1:
    GPIOB->BSRR = GPIO_Pin_4;
    break;  
  case 2:
    GPIOC->BSRR = GPIO_Pin_12;
    break;  
  
  case 3:
    GPIOC->BSRR = GPIO_Pin_10;
    break;
  case 4:
    GPIOC->BSRR = GPIO_Pin_9;
    break;  
  case 5:
    GPIOC->BSRR = GPIO_Pin_7;
    break;  
  case 6:
    GPIOB->BSRR = GPIO_Pin_15;
    break;
  case 7:
    GPIOB->BSRR = GPIO_Pin_12;
    break;
  case 8:
    GPIOB->BSRR = GPIO_Pin_11;
    break;
  case 9:
    GPIOB->BSRR = GPIO_Pin_1;
    break;  
  case 10:
    GPIOC->BSRR = GPIO_Pin_5;
    break;  
  
  case 11:
    GPIOA->BSRR = GPIO_Pin_7;
    break;
  case 12:
    GPIOA->BSRR = GPIO_Pin_5;
    break;
  case 13:
    GPIOA->BSRR = GPIO_Pin_3;
    break;  
  case 14:
    GPIOA->BSRR = GPIO_Pin_1;
    break;  
  
  case 15:
    GPIOC->BSRR = GPIO_Pin_13;
    break;
  case 16:
    GPIOB->BSRR = GPIO_Pin_7;
    break;
    
  default:
    break;
  }
  return raw_reading;
}

/* Reads the MAX31855 and returns the internal and thermocouple
* temperature values to the provided pointers.  If either pointer
* is NULL, it'll skip that reading.
*
* Returns 0 on a successful reading.  If the fault bits are lit up,
* they'll be returned to the caller.
*/
// TODO - 2016/05/30 - this would be a great place to use cexceptions.
max31855_status max31855_temp_cel(float *t_int, float *t_tc,uint8 channal)
{
  uint32_t raw_reading;
  
  raw_reading = max31855_read(channal);
  
  
  /* Temperature reading from cold compensation junction. */
  if (0 != t_int) {
    
    
    tmp_int = MAX31855_TEMP_INT(raw_reading);
    
    /* Extend sign bit if need be. */
    if (MAX31855_INT_SIGN_NEG & raw_reading) {
      tmp_int |= 0xF000;
    }
    
    *t_int = (float) (tmp_int * MAX31855_INT_SCALE);
  }
  
  /* Compensated temperature from thermocouple tip. */
  if (0 != t_tc) {    
    
    tmp_tc = MAX31855_TEMP_TC(raw_reading);
    
    /* Extend sign bit if need be. */
    if (MAX31855_TC_SIGN_NEG & raw_reading) {
      tmp_tc |= 0xC000;
    }
    
    *t_tc = (float) (tmp_tc * MAX31855_TC_SCALE);
  }
  
   if (raw_reading & OC_FAULT) {
    return OC_FAULT;
  }
  if (raw_reading & SCG_FAULT) {
    return  SCG_FAULT;
  }
  if (raw_reading & SCV_FAULT) {
    return SCV_FAULT;
  }
  if (raw_reading & MAX31855_FAULT) {
    return 0x10;
  }
  return 0;
}

max31855_status max31855_temp_int16(int16_t *temperature_int, int16_t *temperature_tc,uint8 channal)
{
  uint32_t raw_reading;
  
  raw_reading = max31855_read(channal);
  
  
  /* Temperature reading from cold compensation junction. */
  if (0 != temperature_int) {
    
    
    tmp_int = MAX31855_TEMP_INT(raw_reading);
    
    /* Extend sign bit if need be. */
    if (MAX31855_INT_SIGN_NEG & raw_reading) {
      tmp_int |= 0xF000;
    }
    
    *temperature_int = tmp_int;
  }
  
  /* Compensated temperature from thermocouple tip. */
  if (0 != temperature_tc) {    
    
    tmp_tc = MAX31855_TEMP_TC(raw_reading);
    
    /* Extend sign bit if need be. */
    if (MAX31855_TC_SIGN_NEG & raw_reading) {
      tmp_tc |= 0xC000;
    }
    
    *temperature_tc = tmp_tc;
  }
  
   if (raw_reading & OC_FAULT) {
    return OC_FAULT;
  }
  if (raw_reading & SCG_FAULT) {
    return  SCG_FAULT;
  }
  if (raw_reading & SCV_FAULT) {
    return SCV_FAULT;
  }
  if (raw_reading & MAX31855_FAULT) {
    return 0x10;
  }
  return 0;
}

/* Read the external and, optionally, internal temperatures from the MAX31855
* and return them in Fahrenheit.  This is essentially a wrapper to the Celsius
* reading call does the "9/5ths plus 32" conversion that we all learned in
* grade school, at least you did if you grew up in a country that decided
* that the metric system was the tool of the devil.  *sigh* Ah well.  My
* car gets forty rods to the hogshead, and that's the way I likes it!
*/
max31855_status max31855_temp_fah(float *t_int, float *t_tc,uint8 channal)
{
  uint8_t status;
  
  status = max31855_temp_cel(t_int, t_tc, channal);
  if (status)
    return status;
  
  if(0 != t_int)
    *t_int = *t_int * (9.0 / 5.0) + 32.0;
  
  if (0 != t_tc)
    *t_tc = *t_tc * (9.0 / 5.0) + 32.0;
  
  return 0;
}

/* Read the external and, optionally, internal temperatures from the MAX31855
* and return them in Kelvin.  Also a wrapper to the Celsius routine.  It
* just adds 273.15 to the results.  But you knew that.
*/
max31855_status max31855_temp_kel(float *t_int, float *t_tc,uint8 channal)
{
  max31855_status status;
  
  status = max31855_temp_cel(t_int, t_tc, channal);
  if (status)
    return status;
  
  if(0 != t_int)
    *t_int = *t_int + 273.15;
  
  if(0 != t_tc)
    *t_tc = *t_tc + 273.15;
  
  return 0;
}






























