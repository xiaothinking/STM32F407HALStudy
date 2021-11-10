#ifndef __sys_h__
#define __sys_h__				 
#include "stm32f4xx.h"
#include <stdio.h>
//#define FlashStartAddr  0x0803F800



static u8  fac_us=0;//us延时倍乘数
static u16 fac_ms=0;//ms延时倍乘数

void RCC_Configuration(void);


void Delay(vu32 nCount);
void delay_init(u8 SYSCLK);
void delay_s(u16 ns);
void delay_ms(u16 nms);
void delay_us(u32 Nus);
__asm void MSR_MSP(u32 addr);
void SoftReset(void);












#endif

