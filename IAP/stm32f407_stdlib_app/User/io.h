#ifndef __io_h__
#define __io_h__				 
#include "stm32f4xx.h"


#define  Beep_ON              GPIO_WriteBit(GPIOF, GPIO_Pin_9,Bit_SET)//PA15
#define  Beep_OFF             GPIO_WriteBit(GPIOF, GPIO_Pin_9,Bit_RESET)

#define  WDI_High             GPIO_WriteBit(GPIOB, GPIO_Pin_1,Bit_SET)//PB1
#define  WDI_Low              GPIO_WriteBit(GPIOB, GPIO_Pin_1,Bit_RESET)//PB1

#define  Usart3_Tx_EN         GPIO_WriteBit(GPIOD, GPIO_Pin_10,Bit_SET)//PD10
#define  Usart3_Rx_EN         GPIO_WriteBit(GPIOD, GPIO_Pin_10,Bit_RESET)

#define  _74HC245_4_Enable    GPIO_WriteBit(GPIOE, GPIO_Pin_1,Bit_RESET)//PE1
#define  _74HC245_4_Disable   GPIO_WriteBit(GPIOE, GPIO_Pin_1,Bit_SET)

extern u8  GetCarbinetNumber(void);








extern void  IO_Init(void);
extern void WDTFeed(void);












#endif

