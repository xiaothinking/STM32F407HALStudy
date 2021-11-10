#ifndef __usart_h__
#define __usart_h__				 
#include "stm32f4xx.h"

#define  UART_RX_BUFFER_SIZE   300


#define  DetectionMode         0x00
#define  BootMode              0x01


#define  UpgradeFirmware               0x07//�̼�������ʼ
#define  GetFirmwareRunMode            0x08//��ѯ�̼�����ģʽ
#define  EraseFirmware                 0x09//�����̼�
#define  TransferFirmware              0x0C//����̼�


#define  FrameHeader                0xAA
#define  FrameTail                  0x55 

extern void USART3_Configuration(void);										
extern void USART3_SendByte(u16 Data);
extern void USART3Write(u8* data,u16 len);
extern void Uart3ReceiveHandle(void);









#endif

