#include"IO.h"
#include"sys.h"
#include"stm32f4xx_it.h"
#include"usart.h"

void  IO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	/*时钟使能*/
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	 
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
/*输出配置*/	
	 
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		/*PA端口*/
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
		/*PB端口*/
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
		/*PD端口*/
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	  /*PE端口*/
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

/*初始默认值*/
    Beep_OFF;
		Usart3_Rx_EN;
		
}	
void WDTFeed(void)
{  
    WDI_High;
    WDI_Low;
}	
u8  Get74hc245Value(void)
{
	u8 i,value = 0,temp = 0;
  u16 port_c = 0,port_d;
	port_c = GPIO_ReadInputData(GPIOC);
	port_c = (port_c>>10)&0x0007;
  port_d = GPIO_ReadInputData(GPIOD);
	port_d = (port_d<<3)&0x00F8;
	value = port_c|port_d;
	/*每一个位的顺序互换*/
	for(i=0;i<8;i++)
	{
	   temp |= (value>>i)&0x01;
		 if(i<7)
			temp <<= 1;  
	}
  return temp;
}
u8  GetCarbinetNumber(void)
{
	 u8 value = 0;
  _74HC245_4_Enable;
	 delay_ms(1);
	value = Get74hc245Value();
	value = (value&0xF0)>>4;
  _74HC245_4_Disable;	
	return value;
}
