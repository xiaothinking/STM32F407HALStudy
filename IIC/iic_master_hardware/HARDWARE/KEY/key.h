#ifndef __KEY_H
#define __KEY_H	 
#include "main.h"
#include "stm32f4xx_hal.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//����������������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

/*����ķ�ʽ��ͨ��ֱ�Ӳ����⺯����ʽ��ȡIO*/
#define KEY0 		HAL_GPIO_ReadPin(KEY0_GPIO_Port,KEY0_Pin) //PE4
#define KEY1 		HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin)	//PE3 


#define KEY0_PRES 	1
#define KEY1_PRES	  2
#define KEY2_PRES	  3
#define WKUP_PRES   4

u8 KEY_Scan(u8);  		//����ɨ�躯��	

#endif
