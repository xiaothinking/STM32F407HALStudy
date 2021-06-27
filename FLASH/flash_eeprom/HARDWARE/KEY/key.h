#ifndef __KEY_H
#define __KEY_H	 
#include "main.h"
#include "stm32f4xx_hal.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//按键输入驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

/*下面的方式是通过直接操作库函数方式读取IO*/
#define KEY0 		HAL_GPIO_ReadPin(KEY0_GPIO_Port,KEY0_Pin)     //PE4
#define KEY1 		HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin)	    //PE3 
#define KEY2 		HAL_GPIO_ReadPin(KEY2_GPIO_Port,KEY2_Pin)     //PE2
#define WK_UP 	HAL_GPIO_ReadPin(KEY_UP_GPIO_Port,KEY_UP_Pin)	  //PA0


#define KEY0_PRES 	1
#define KEY1_PRES	  2
#define KEY2_PRES	  3
#define WKUP_PRES   4

uint8_t KEY_Scan(uint8_t);  		//按键扫描函数	

#endif
