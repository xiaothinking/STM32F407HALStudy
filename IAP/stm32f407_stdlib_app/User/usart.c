#include "usart.h"
#include "sys.h"
#include <stdio.h>
#include <string.h>
#include "io.h"
#include "stm32f4xx_it.h"
#include "stmflash.h"
#include "stm32f4xx_flash.h"
u8  UART3_Buffer[UART_RX_BUFFER_SIZE];
u8  u8buffer[UART_RX_BUFFER_SIZE];
u8  UART3_BufferSign = 0;
u8  UART3_WriteIndex = 0;
u8  RunMode = DetectionMode;
u8  Cmd0[14] = {0x1B,0x73,0x42,0x45,0x92,0x46,0x53,0x2D,0x41,0x56,0x31,0x30,0x00,0x43};
u8  Cmd1[14] = {0x1B,0x73,0x42,0x45,0x92,0x46,0x53,0x2D,0x42,0x56,0x31,0x30,0x00,0x43};
void USART3_IRQHandler(void)
{
  if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
  {  	
		 UART3_BufferSign = 1;
		 Uart3Delay = 5;
     UART3_Buffer[UART3_WriteIndex++] = USART_ReceiveData(USART3); 
	  
	   if(UART3_WriteIndex >= UART_RX_BUFFER_SIZE)
	   {
	        UART3_WriteIndex = 0;
	   } 	 
     //USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	}
}
void USART3_Configuration(void)										
{	
	  NVIC_InitTypeDef NVIC_InitStructure;
   	USART_InitTypeDef USART_InitStructure;
 	  GPIO_InitTypeDef GPIO_InitStructure;
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	  GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3);
	  /* 配置 USART TxRx  */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
  
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		
    USART_Init(USART3, &USART_InitStructure);   
	  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);				 //打开接收中断
	  USART_Cmd(USART3, ENABLE);									 	
 
    /* 配置中断优先级 */
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;				
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;		 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			 
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				
    NVIC_Init(&NVIC_InitStructure);	
								 	
}
/*****************************DQ.elc******************************************
* Function Name  : USART3_SendByte
* Description    : 发送字符
* Input          : 8位字符
* Output         : None
* Return         : None
******************************DQ.elc*****************************************/
void USART3_SendByte(u16 Data)
{ 
   while (!(USART3->SR & USART_FLAG_TXE));  //!(USART5->SR & 
   USART3->DR = (Data & (uint16_t)0x01FF);	 
   //ITM_SendChar(Data);
}
/*****************************DQ.elc******************************************
* Function Name  : USART3Write
* Description    : 发送字符串
* Input          : 8位字符串
* Output         : None
* Return         : None
******************************DQ.elc*****************************************/
void USART3Write(u8* data,u16 len)
{
	u16 i;
	Usart3_Tx_EN;
	delay_ms(1);
	for (i=0; i<len; i++)
	{
		USART3_SendByte(data[i]);
	}
	delay_ms(5);
	Usart3_Rx_EN;
	delay_ms(5);
}
void response_pc_command(u8 Addr,u8 Boxnum,u8 Cmd,u8 Len, u8 *pudata)
{
		u8 i,bcc = 0;
		bcc = FrameHeader^Addr^Boxnum^Cmd^Len;
	  Usart3_Tx_EN;
		delay_ms(1);
	  USART3_SendByte(FrameHeader); 
		USART3_SendByte(Addr);
		USART3_SendByte(Boxnum);
		USART3_SendByte(Cmd);
	  USART3_SendByte(Len);
		for(i=0;i<Len;i++)
		{
			 USART3_SendByte(pudata[i]);	
			 bcc ^= pudata[i];
		}
		USART3_SendByte(bcc);
		USART3_SendByte(FrameTail);
		delay_ms(5);
		Usart3_Rx_EN;
		delay_ms(5);
}
u8 Check_PC_Command(u8  *pudata)
{
		u8  addr, boxnum ,len, cmd,DataLen,i,chk = 0;
		addr   = pudata[1];
		boxnum = pudata[2];
		cmd    = pudata[3];
		len    = pudata[4];
		chk   ^= pudata[0]^addr^boxnum^cmd^len;
		if(pudata[0] == FrameHeader)
		{
	      if(addr == GetCarbinetNumber() || addr == 0x00 || addr == 0xFF) 
				{
						switch(cmd)
						{
								
								case UpgradeFirmware:
								case GetFirmwareRunMode:						  
										 DataLen = 1;
										 break;				 
								case EraseFirmware:
									   DataLen = 0x0E;
										 break;	
								case TransferFirmware:
									   len = DataLen;
									   break;
								default :
										 break;
						}	
						if(len == DataLen)
						{
								for(i=0;i<len;i++)
								{
									 chk ^= pudata[i+5]; 
								}	
								if(chk == pudata[i+5])
								{
									 if( pudata[i+6] == FrameTail)
									 {
											return 1;   
									 }	
									 else
									 {
											return 0;
									 }									 
								}	
								else
								{
									 return 0; 
								}								
						}
						else
						{
								return 0; 
						}
				}	
				else
				{
						return 0; 
				}			
		}
		else
		{
				return 0; 
		}

}
