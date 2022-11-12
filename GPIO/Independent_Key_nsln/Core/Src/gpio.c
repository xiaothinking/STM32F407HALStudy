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

    key_press = key_input;                  // 读按键IO电平

    switch (key_state)
    {
      case key_state_0:                            // 按键初始态
        if (!key_press) key_state = key_state_1;   //键被按下，状态转换到按键消抖和确认状态
        break;
      
      case key_state_1:                 // 按键消抖与确认态
        if (!key_press)
        {
             key_time = 0;              //  
             key_state = key_state_2;   // 按键仍然处于按下，消抖完成，状态转换到按下键时间的计时状态，但返回的还是无键事件
        }
        else
             key_state = key_state_0;   // 按键已抬起，转换到按键初始态。此处完成和实现软件消抖，其实按键的按下和释放都在此消抖的。
        break;
      
      case key_state_2:                //按下键时间的计时状态
        if(key_press)
        {
             key_return = S_key;        // 此时按键释放，说明是产生一次短操作，回送S_key
             key_state = key_state_0;   // 转换到按键初始态
        }
        else if (++key_time >= 3000)    // 继续按下，计时加1ms（1ms为本函数循环执行间隔）
        {
             key_return = L_key;        // 按下时间>3000ms，此按键为长按操作，返回长键事件
             key_state = key_state_3;   // 转换到等待按键释放状态
        }
        break;

      case key_state_3:                          // 等待按键释放状态，此状态只返回无按键事件
        if (key_press) key_state = key_state_0;  //按键已释放，转换到按键初始态
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
