/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, LED1_Pin|LED2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = KEY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(KEY_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PFPin PFPin */
  GPIO_InitStruct.Pin = LED1_Pin|LED2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */
/***********************************************************************************************************************
* Function Name: �����жϺ���
* Description  : 
* Arguments    : 
* Return Value : 
***********************************************************************************************************************/
struct KeyIn{
  uint8_t  Sample: 1;
  uint8_t  Lv:     1;
  uint8_t  PreLv:  1;
  uint8_t  Rise:   1;
  uint8_t  Fall:   1;
};

struct KeyIn KeyGo, KeyHL;
uint8_t KeyGoLvOld=0;
uint8_t KeyGoLvNew = 0;
uint8_t KeyGoLv = 0;
uint8_t KeyGoCmdLv = 0;
uint32_t KeyGoPwrAlt = 0;
uint8_t IOKeepCnt = 0;
void OnOffPer1ms (void)                   
{ 
  KeyGoLvOld = KeyGoLvNew;
  KeyGoLvNew = KEYGO_IN;                      //��ȡ����IO�ڣ�����IO������Ϊ����ģʽ
  if(KeyGoLvNew == KeyGoLvOld)
    {
     if(++IOKeepCnt>=15)
       {
        KeyGoLv = KeyGoLvNew;                     //ǰ������ֵ���
        IOKeepCnt = 15;
       }
    }
   else
   
		  IOKeepCnt = 0; 
			KeyGo.Rise = 0;                             // �����������
			KeyGo.Fall = 0;                             // �����������
			KeyGo.PreLv = KeyGo.Lv;                     // �����ϴ�ֵ
			KeyGo.Lv = KeyGoLv;
			if(KeyGo.Lv != KeyGo.PreLv)                 // �籾�β�ͬ���ϴΣ��ж��Ǻ�������
			{
				if(KeyGo.Lv == 1)                         // ����Ϊ��
					KeyGo.Rise = 1;                         // ������
				else                                      // ����Ϊ�½���
					KeyGo.Fall = 1;
			}
			if(KeyGo.Fall == 1)                         //������½���
			{
				KeyGo.Fall = 0;                           //�����־
				if(KeyGoCmdLv == 0)
					KeyGoCmdLv = 1;
				else if(KeyGoCmdLv == 1)
					KeyGoCmdLv = 2;
				else if(KeyGoCmdLv == 2)
					KeyGoCmdLv = 3;
				else
					KeyGoCmdLv = 0;                          //�����½��ظ�����λ
			}
			
		 if(KeyGoCmdLv == 0)
			 {
					HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);  
					HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET); 
			 }	 
		 else if(KeyGoCmdLv == 1)
			 {
					HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);  
					HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET); 
			 }  
		 else if(KeyGoCmdLv == 2)
			 {
					HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);  
					HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
			 }
		 else
			 {
					HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);  
					HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
			 }
		 
	


		 
}

/* USER CODE END 2 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
