/**
  ******************************************************************************
  * @file    gpio.h
  * @brief   This file contains all the function prototypes for
  *          the gpio.c file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
char KEY_SCAN(void);
char KEY_ROW_SCAN(void);
void HW_KEY_FUNCTION(void);

#define KEY_CLO1_OUT_LOW  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_8,GPIO_PIN_RESET) 
#define KEY_CLO2_OUT_LOW  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_9,GPIO_PIN_RESET)
#define KEY_CLO3_OUT_LOW  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_10,GPIO_PIN_RESET)
#define KEY_CLO4_OUT_LOW  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_RESET)

#define KEY_CLO1_OUT_HIGH  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_8,GPIO_PIN_SET) 
#define KEY_CLO2_OUT_HIGH  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_9,GPIO_PIN_SET)
#define KEY_CLO3_OUT_HIGH  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_10,GPIO_PIN_SET)
#define KEY_CLO4_OUT_HIGH  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_SET)
/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
