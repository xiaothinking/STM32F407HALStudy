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
  HAL_GPIO_WritePin(GPIOF, LED1_Pin|LED2_Pin, GPIO_PIN_SET);

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
unsigned char key_driver(void)
{
    static unsigned char key_state = key_state_0;
	  static unsigned int  key_time = 0;
    unsigned char key_press, key_return = N_key;

    key_press = key_input;                  // ������IO��ƽ

    switch (key_state)
    {
      case key_state_0:                            // ������ʼ̬
        if (!key_press) key_state = key_state_1;   //�������£�״̬ת��������������ȷ��״̬
        break;
      
      case key_state_1:                 // ����������ȷ��̬
        if (!key_press)
        {
             key_time = 0;              //  
             key_state = key_state_2;   // ������Ȼ���ڰ��£�������ɣ�״̬ת�������¼�ʱ��ļ�ʱ״̬�������صĻ����޼��¼�
        }
        else
             key_state = key_state_0;   // ������̧��ת����������ʼ̬���˴���ɺ�ʵ�������������ʵ�����İ��º��ͷŶ��ڴ������ġ�
        break;
      
      case key_state_2:                //���¼�ʱ��ļ�ʱ״̬
        if(key_press)
        {
             key_return = S_key;        // ��ʱ�����ͷţ�˵���ǲ���һ�ζ̲���������S_key
             key_state = key_state_0;   // ת����������ʼ̬
        }
        else if (++key_time >= 3000)    // �������£���ʱ��1ms��1msΪ������ѭ��ִ�м����
        {
             key_return = L_key;        // ����ʱ��>3000ms���˰���Ϊ�������������س����¼�
             key_state = key_state_3;   // ת�����ȴ������ͷ�״̬
        }
        break;

      case key_state_3:                          // �ȴ������ͷ�״̬����״ֻ̬�����ް����¼�
        if (key_press) key_state = key_state_0;  //�������ͷţ�ת����������ʼ̬
        break;
    }
    return key_return;
}

void OnOffPer1ms (void)                   
{ 
  unsigned char key;
	static unsigned char keygscmd=0;
	static unsigned char keyglcmd=0;
	static unsigned int led1_count=0;
	static unsigned int led2_count=0;
	
	key=key_driver();
	if(key==S_key)
  {
		if(++keygscmd>2)
    {
			keygscmd=0;
	  }
	}
	else if(key==L_key)
  {
		if(++keyglcmd>2)
    {
			keyglcmd=0;
	  }
	}
	
	
	if(keygscmd==0)
  {
		 HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
	}
	else if(keygscmd==1)
  {
		 HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
	}
	else if(keygscmd==2)
  {
			if(++led1_count>1000)
			{
				led1_count=0;
			}
			if(led1_count<500)
      {
				HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
			}
      else
      {
				HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
			}
	}
  
	if(keyglcmd==0)
  {
		 HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
	}
	else if(keyglcmd==1)
  {
		 HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
	}
	else if(keyglcmd==2)
  {
			if(++led2_count>1000)
			{
				led2_count=0;
			}
			if(led2_count<500)
      {
				HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
			}
      else
      {
				HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
			}
	}
}

/* USER CODE END 2 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
