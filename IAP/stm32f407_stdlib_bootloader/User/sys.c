#include"sys.h"

__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}

void SoftReset(void)
{
	__set_FAULTMASK(1);      // �ر������ж�
	NVIC_SystemReset();// ��λ
}

void delay_init(u8 SYSCLK)
{
	SysTick->CTRL&=0xfffffffb;//ѡ���ڲ�ʱ�� HCLK/8
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
//��168M������,Nms<=790 
void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;//ʱ�����
	SysTick->VAL =0x00;           //��ռ�����
	SysTick->CTRL=0x01 ;          //��ʼ����  
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��   
	SysTick->CTRL=0x00;       //�رռ�����
	SysTick->VAL =0X00;       //��ռ�����	  	    
}   
//��ʱus								   
void delay_us(u32 Nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=Nus*fac_us; //ʱ�����	  		 
	SysTick->VAL=0x00;        //��ռ�����
	SysTick->CTRL=0x01 ;      //��ʼ���� 	 
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��   
	SysTick->CTRL=0x00;       //�رռ�����
	SysTick->VAL =0X00;       //��ռ�����	    
}

void RCC_Configuration(void)
{
	ErrorStatus HSEStartUpStatus;
	RCC_ClocksTypeDef RCC_Clocks;

	RCC_DeInit();												      //ʱ�ӿ��ƼĴ���ȫ���ָ�Ĭ��ֵ
  RCC_HSEConfig(RCC_HSE_ON);								//�ⲿ����ʱ��Դ������8M����
  HSEStartUpStatus = RCC_WaitForHSEStartUp();				//�ȴ��ⲿʱ�Ӿ���
  if(HSEStartUpStatus == SUCCESS)							//���ʱ�������ɹ�
  {
    RCC_HCLKConfig(RCC_SYSCLK_Div1);						//����AHB�豸ʱ��Ϊϵͳʱ��1��Ƶ
    RCC_PCLK2Config(RCC_HCLK_Div2);							//����APB2�豸ʱ��ΪHCLKʱ��2��Ƶ
    RCC_PCLK1Config(RCC_HCLK_Div4);							//����APB1�豸ʱ��ΪHCLKʱ��4��Ƶ
    FLASH_SetLatency(FLASH_Latency_5);						//�趨�ڲ�FLASH�ĵ���ʱ����Ϊ5����
    FLASH_PrefetchBufferCmd(ENABLE);	            //ʹ��FLASHԤ��ȡ������
    RCC_PLLConfig(RCC_PLLSource_HSE,8,336,2,7);	//VCO����HSE8��Ƶ��VCO���336��Ƶ��SYSCLKΪVCO2��Ƶ��USBʱ��VCO7��Ƶ
																								//VCO�����336M   SYSCLK=336/2=168    USB 48M
    RCC_PLLCmd(ENABLE);										     //ʹ��PLLʱ��
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)		//�ȴ�PLLʱ���������׼������
    {
    }
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);				//ʹ��PLLʱ����Ϊϵͳʱ��Դ
    while(RCC_GetSYSCLKSource() != 0x08)					//����ϵͳ����ʱ��Դȷ��Ϊ�ⲿ���پ���
    {
    }
		
    RCC_GetClocksFreq(&RCC_Clocks);
  }										
}
