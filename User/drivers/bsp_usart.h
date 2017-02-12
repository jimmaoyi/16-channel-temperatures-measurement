  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _bsp_usart_H
#define	_bsp_usart_H
  
#ifdef __cplusplus
extern "C" {
#endif
  
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "main.h"
/* Exported types ------------------------------------------------------------*/
extern uint8 USART1MemoryBuffer[10];
extern uint8 HaveReceivedNewCommand;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern void bsp_InitUart(void);
extern void SendMessage(uint8 *Buffer1, uint8 length);
  
#ifdef __cplusplus
}
#endif

#endif /* _bsp_usart_H */

