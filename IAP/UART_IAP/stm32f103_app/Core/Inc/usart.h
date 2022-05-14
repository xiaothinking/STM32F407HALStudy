/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;

/* USER CODE BEGIN Private defines */
#define   UART1SIZE                 256
#define DMA_UART1_RX_FINISH         0X01 
#define DMA_UART1_RX_NO_FINISH      0X00

extern uint8_t usart1_rx_flag;
extern uint8_t usart1_dma_rx_buffer[UART1SIZE];
extern uint8_t usart1_protocol_buffer[UART1SIZE];
extern uint8_t usart1_dma_tx_buffer[UART1SIZE];
extern uint16_t usart1_dma_rx_len;
/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);

/* USER CODE BEGIN Prototypes */
void UsartReceive_IDLE(UART_HandleTypeDef *huart);
void uart1_handle(uint8_t * pbuff,uint8_t buff_len);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
