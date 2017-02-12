/**
  ******************************************************************************
  * @file    main.h
  * @author  Wireless Protocol Stack Develop-Dept.
  * @version --
  * @date    2016/1/14 ÐÇÆÚËÄ,Administrator
  * @brief  This file 
  *
  * @verbatim  
  *
  ******************************************************************************
  * @attention
  *     
  *     
  *
  * <h2><center>&copy; COPYRIGHT 2015 WuLianGroup</center></h2>
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _main_H
#define	_main_H
  
#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
/* Exported types ------------------------------------------------------------*/
typedef enum
{
  currentIP2 = (uint8)0x05,
  currentIP1 = (uint8)0x04,
  currentIP3 = (uint8)0x06,
  currentIP4 = (uint8)0x07,  
  NoneDispIP1 = (uint8)0x08, 
  NoneDispIP2 = (uint8)0x09, 
  NoneDispIP3 = (uint8)0x0a, 
  payError = (uint8)0x0b, 
  findCard = (uint8)0x0c,
  NoneDispBalance = (uint8)0x00,  
  CardBalance = (uint8)0x01,
  WIFIdisconnect = (uint8)0x02,
  DECcoin = (uint8)0x03   
}DisplayRefreshLedStatuse_TypeDef;


extern uint8 HaveReceivedNewCommand;
extern uint16 Display_coinNum;

extern DisplayRefreshLedStatuse_TypeDef RefreshLedStatuse;

#ifdef __cplusplus
}
#endif

#endif /* _main_H */
/******************* (C) COPYRIGHT 2015 WuLianGroup ********END OF FIL*********/
