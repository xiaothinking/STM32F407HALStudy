#include"sys.h"

__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}

void SoftReset(void)
{
	__set_FAULTMASK(1);      // 关闭所有中端
	NVIC_SystemReset();// 复位
}

void delay_init(u8 SYSCLK)
{
	SysTick->CTRL&=0xfffffffb;//选择内部时钟 HCLK/8
	fac_us=SYSCLK/8;		    
	fac_ms=(u16)fac_us*1000;
}								  
void delay_s(u16 ns)
{	 		  	  
	u16 i;
	u16 temp;	
  temp=ns*2;	
	 for(i=0;i<temp;i++)
	 {
		 delay_ms(500);
	 }
}   
//对168M条件下,Nms<=790 
void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;//时间加载
	SysTick->VAL =0x00;           //清空计数器
	SysTick->CTRL=0x01 ;          //开始倒数  
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//等待时间到达   
	SysTick->CTRL=0x00;       //关闭计数器
	SysTick->VAL =0X00;       //清空计数器	  	    
}   
//延时us								   
void delay_us(u32 Nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=Nus*fac_us; //时间加载	  		 
	SysTick->VAL=0x00;        //清空计数器
	SysTick->CTRL=0x01 ;      //开始倒数 	 
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//等待时间到达   
	SysTick->CTRL=0x00;       //关闭计数器
	SysTick->VAL =0X00;       //清空计数器	    
}

void RCC_Configuration(void)
{
	ErrorStatus HSEStartUpStatus;
	RCC_ClocksTypeDef RCC_Clocks;

	RCC_DeInit();												      //时钟控制寄存器全部恢复默认值
  RCC_HSEConfig(RCC_HSE_ON);								//外部高速时钟源开启（8M晶振）
  HSEStartUpStatus = RCC_WaitForHSEStartUp();				//等待外部时钟就绪
  if(HSEStartUpStatus == SUCCESS)							//如果时钟启动成功
  {
    RCC_HCLKConfig(RCC_SYSCLK_Div1);						//定义AHB设备时钟为系统时钟1分频
    RCC_PCLK2Config(RCC_HCLK_Div2);							//定义APB2设备时钟为HCLK时钟2分频
    RCC_PCLK1Config(RCC_HCLK_Div4);							//定义APB1设备时钟为HCLK时钟4分频
    FLASH_SetLatency(FLASH_Latency_5);						//设定内部FLASH的的延时周期为5周期
    FLASH_PrefetchBufferCmd(ENABLE);	            //使能FLASH预存取缓冲区
    RCC_PLLConfig(RCC_PLLSource_HSE,8,336,2,7);	//VCO输入HSE8分频，VCO输出336倍频，SYSCLK为VCO2分频，USB时钟VCO7分频
																								//VCO输出：336M   SYSCLK=336/2=168    USB 48M
    RCC_PLLCmd(ENABLE);										     //使能PLL时钟
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)		//等待PLL时钟设置完成准备就绪
    {
    }
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);				//使用PLL时钟作为系统时钟源
    while(RCC_GetSYSCLKSource() != 0x08)					//返回系统所用时钟源确认为外部高速晶振。
    {
    }
		
    RCC_GetClocksFreq(&RCC_Clocks);
  }										
}
