/**
  ******************************************************************************
  * File Name          : TIM.c
  * Description        : This file provides code for the configuration
  *                      of the TIM instances.
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
#include "tim.h"

/* USER CODE BEGIN 0 */
//捕获状态
//[7]:0,没有成功的捕获;1,成功捕获到一次.
//[6]:0,还没捕获到低电平;1,已经捕获到低电平了.
//[5:0]:捕获低电平后溢出的次数(对于32位定时器来说,1us计数器加1,溢出时间:4294秒)
uint8_t   TIM5CH1_CAPTURE_STA=0;	//输入捕获状态		    				
uint32_t  TIM5CH1_CAPTURE_VAL;	//输入捕获值(TIM2/TIM5是32位)
/* USER CODE END 0 */

TIM_HandleTypeDef htim5;

/* TIM5 init function */
void MX_TIM5_Init(void)
{
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  htim5.Instance = TIM5;
  htim5.Init.Prescaler = 84;
  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim5.Init.Period = 0xFFFFFFFF;
  htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_IC_Init(&htim5) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 8;
  if (HAL_TIM_IC_ConfigChannel(&htim5, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_TIM_IC_MspInit(TIM_HandleTypeDef* tim_icHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(tim_icHandle->Instance==TIM5)
  {
  /* USER CODE BEGIN TIM5_MspInit 0 */

  /* USER CODE END TIM5_MspInit 0 */
    /* TIM5 clock enable */
    __HAL_RCC_TIM5_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**TIM5 GPIO Configuration
    PA0-WKUP     ------> TIM5_CH1
    */
    GPIO_InitStruct.Pin = WK_UP_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM5;
    HAL_GPIO_Init(WK_UP_GPIO_Port, &GPIO_InitStruct);

    /* TIM5 interrupt Init */
    HAL_NVIC_SetPriority(TIM5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM5_IRQn);
  /* USER CODE BEGIN TIM5_MspInit 1 */

  /* USER CODE END TIM5_MspInit 1 */
  }
}

void HAL_TIM_IC_MspDeInit(TIM_HandleTypeDef* tim_icHandle)
{

  if(tim_icHandle->Instance==TIM5)
  {
  /* USER CODE BEGIN TIM5_MspDeInit 0 */

  /* USER CODE END TIM5_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM5_CLK_DISABLE();

    /**TIM5 GPIO Configuration
    PA0-WKUP     ------> TIM5_CH1
    */
    HAL_GPIO_DeInit(WK_UP_GPIO_Port, WK_UP_Pin);

    /* TIM5 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM5_IRQn);
  /* USER CODE BEGIN TIM5_MspDeInit 1 */

  /* USER CODE END TIM5_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
//定时器更新中断（计数溢出）中断处理回调函数， 该函数在HAL_TIM_IRQHandler中会被调用
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)//更新中断（溢出）发生时执行
{
	
	if((TIM5CH1_CAPTURE_STA&0X80)==0)//还未成功捕获
	{
			if(TIM5CH1_CAPTURE_STA&0X40)//已经捕获到高电平了
			{
				if((TIM5CH1_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
				{
					TIM5CH1_CAPTURE_STA|=0X80;		//标记成功捕获了一次
					TIM5CH1_CAPTURE_VAL=0XFFFFFFFF;
				}else TIM5CH1_CAPTURE_STA++;
			}	 
	}		
}

//定时器输入捕获中断处理回调函数，该函数在HAL_TIM_IRQHandler中会被调用
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)//捕获中断发生时执行
{
	if((TIM5CH1_CAPTURE_STA&0X80)==0)//还未成功捕获
	{
		if(TIM5CH1_CAPTURE_STA&0X40)		//捕获到一个下降沿 		
			{	  			
				TIM5CH1_CAPTURE_STA|=0X80;		//标记成功捕获到一次高电平脉宽
				TIM5CH1_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&htim5,TIM_CHANNEL_1);//获取当前的捕获值.
				TIM_RESET_CAPTUREPOLARITY(&htim5,TIM_CHANNEL_1);   //一定要先清除原来的设置！！
				TIM_SET_CAPTUREPOLARITY(&htim5,TIM_CHANNEL_1,TIM_ICPOLARITY_RISING);//配置TIM5通道1上升沿捕获
			}else  								//还未开始,第一次捕获上升沿
			{
				TIM5CH1_CAPTURE_STA=0;			//清空
				TIM5CH1_CAPTURE_VAL=0;
				TIM5CH1_CAPTURE_STA|=0X40;		//标记捕获到了上升沿
				__HAL_TIM_DISABLE(&htim5);        //关闭定时器5
				__HAL_TIM_SET_COUNTER(&htim5,0);
				TIM_RESET_CAPTUREPOLARITY(&htim5,TIM_CHANNEL_1);   //一定要先清除原来的设置！！
				TIM_SET_CAPTUREPOLARITY(&htim5,TIM_CHANNEL_1,TIM_ICPOLARITY_FALLING);//定时器5通道1设置为下降沿捕获
				__HAL_TIM_ENABLE(&htim5);//使能定时器5
			}		    
	}		
	
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
