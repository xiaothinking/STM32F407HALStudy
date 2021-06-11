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
#include "usart.h"
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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, col_1_Pin|col_2_Pin|col_3_Pin|col_4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PEPin PEPin PEPin PEPin */
  GPIO_InitStruct.Pin = col_1_Pin|col_2_Pin|col_3_Pin|col_4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PEPin PEPin PEPin PEPin */
  GPIO_InitStruct.Pin = row_1_Pin|row_2_Pin|row_3_Pin|row_4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */
/***
 *函数名：KEY_ROW_SCAN
 *功  能：按键行扫描
 *返回值：1~4，对应1~4行按键位置
 */
uint8_t Key_row[1]={0xff};   //保存按键行扫描情况的状态数组
char KEY_ROW_SCAN(void)
{
    //读出行扫描状态
    Key_row[0] = HAL_GPIO_ReadPin(GPIOE,row_1_Pin)<<3;
    Key_row[0] = Key_row[0] | (HAL_GPIO_ReadPin(GPIOE,row_2_Pin)<<2);
    Key_row[0] = Key_row[0] | (HAL_GPIO_ReadPin(GPIOE,row_3_Pin)<<1);
    Key_row[0] = Key_row[0] | (HAL_GPIO_ReadPin(GPIOE,row_4_Pin));
    
    if(Key_row[0] != 0x0f)         //行扫描有变化，判断该列有按键按下
    {
      HAL_Delay(10);                    //消抖
      if(Key_row[0] != 0x0f)
        {   
                //printf("Key_Row_DATA = 0x%x\r\n",Key_row[0]);
                switch(Key_row[0])
                {
                    case 0x07:         //0111 判断为该列第1行的按键按下
                        return 1;
                    case 0x0b:         //1011 判断为该列第2行的按键按下
                        return 2;
                    case 0x0d:         //1101 判断为该列第3行的按键按下
                        return 3;
                    case 0x0e:         //1110 判断为该列第4行的按键按下
                        return 4;
                    default :
                        return 0;
                }
        }
        else return 0;
    }
    else return 0;
}

/***
 *函数名：KEY_SCAN
 *功  能：4*4按键扫描
 *返回值：0~16，对应16个按键
 */
char KEY_SCAN(void)
{    
    char Key_Num=0;       //1-16对应的按键数
    char key_row_num=0;        //行扫描结果记录
    
    KEY_CLO1_OUT_LOW;        
    if( (key_row_num=KEY_ROW_SCAN()) != 0 )
    { 
        while(KEY_ROW_SCAN() != 0);  //消抖
        Key_Num = 0 + key_row_num;
        //printf("Key_Clo_1\r\n");
    }
    KEY_CLO1_OUT_HIGH;
    
    KEY_CLO2_OUT_LOW;        
    if( (key_row_num=KEY_ROW_SCAN()) != 0 )
    { 
        while(KEY_ROW_SCAN() != 0);
        Key_Num = 4 + key_row_num;
        //printf("Key_Clo_2\r\n");
    }
    KEY_CLO2_OUT_HIGH;
    
    KEY_CLO3_OUT_LOW;    
    if( (key_row_num=KEY_ROW_SCAN()) != 0 )
    { 
        while(KEY_ROW_SCAN() != 0);
    Key_Num = 8 + key_row_num;
        //printf("Key_Clo_3\r\n");
    }
    KEY_CLO3_OUT_HIGH;
    
    KEY_CLO4_OUT_LOW;    
    if( (key_row_num=KEY_ROW_SCAN()) != 0 )
    {
//        Key_row[0] = HAL_GPIO_ReadPin(GPIOE,KEY_col0_Pin)<<3;
//        Key_row[0] = Key_row[0] | (HAL_GPIO_ReadPin(GPIOE,KEY_col1_Pin)<<2);
//        Key_row[0] = Key_row[0] | (HAL_GPIO_ReadPin(GPIOE,KEY_col2_Pin)<<1);
//        Key_row[0] = Key_row[0] | (HAL_GPIO_ReadPin(GPIOE,KEY_col3_Pin));
//        printf("Key_Clo4_DATA = 0x%x\r\n",Key_row[0]);
        while(KEY_ROW_SCAN() != 0);
        Key_Num = 12 + key_row_num;
        //printf("Key_Clo_4\r\n");
    }
    KEY_CLO4_OUT_HIGH;
    
    return Key_Num;
}

/***
 *函数名：KEY_ROW_SCAN
 *功  能：执行按下按键后的操作
 *返回值：无
 */
void HW_KEY_FUNCTION(void)
{
    char key_confirm;
    key_confirm = KEY_SCAN();
    if( 0 < key_confirm  && key_confirm < 17 )
    {
        printf("Key_NUM = %d \r\n",key_confirm); //按下1-16个按键的操作
        printf("= = = = = = = = = = = \r\n");
    }
}
/* USER CODE END 2 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
