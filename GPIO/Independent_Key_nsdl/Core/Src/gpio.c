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
        else if (++key_time >= 1000)    // �������£���ʱ��1ms��1msΪ������ѭ��ִ�м����
        {
             key_return = L_key;        // ����ʱ��>1000ms���˰���Ϊ�������������س����¼�
             key_state = key_state_3;   // ת�����ȴ������ͷ�״̬
        }
        break;

      case key_state_3:                          // �ȴ������ͷ�״̬����״ֻ̬�����ް����¼�
        if (key_press) key_state = key_state_0;  //�������ͷţ�ת����������ʼ̬
        break;
    }
    return key_return;
}

unsigned char key_read(void)
{
    static unsigned char key_m = key_state_0;
	  static unsigned int key_time_1 = 0;
    unsigned char key_return = N_key,key_temp;
     
    key_temp = key_driver();
     
    switch(key_m)
    {
        case key_state_0:
            if (key_temp == S_key )
            {
                 key_time_1 = 0; // ��1�ε����������أ����¸�״̬�жϺ����Ƿ����˫��
                 key_m = key_state_1;
            }
            else
                 key_return = key_temp;        // �����޼�������������ԭ�¼�
            break;

        case key_state_1:
            if (key_temp == S_key)             // ��һ�ε���������϶�<500ms��
            {
                 key_return = D_key;           // ����˫�����¼����س�ʼ״̬
                 key_m = key_state_0;
            }
            else                                
            {                         // ����500ms�ڿ϶������Ķ����޼��¼�����Ϊ 
                                      // ����>1000ms����1sǰ�Ͳ㷵�صĶ����޼�
                 if(++key_time_1 >= 500)
                 {
                     key_return = S_key; //500ms��û���ٴγ��ֵ����¼���������һ�� 
                                         // �ĵ����¼�
                     key_m = key_state_0;   // ���س�ʼ״̬
                 }
             }
             break;
    }
    return key_return;
}   

void OnOffPer1ms (void)                   
{ 
  unsigned char key;
	key=key_read();
	if(key==S_key)
  {
		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);		
	}
	else if(key==D_key)
  {
		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);				
	}
	else if(key==L_key)
  {
		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);	
	}

}

/* USER CODE END 2 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
