#include "iap.h"
#include "stmflash.h"
#include "usart.h"

iapfun jump2app; 
__asm void MSR_MSP(uint32_t addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}

void iap_load_app(uint32_t appxaddr)
{
	if(((*(volatile uint32_t*)appxaddr)&0x2FFE0000)==0x20000000)	//检查栈顶地址是否合法.//256K:0x2FFC0000  128K:0x2FFE0000 可统一最大
	{ 
		printf("jump to app\n");
		jump2app=(iapfun)*(volatile uint32_t*)(appxaddr+4);		//用户代码区第二个字为程序开始地址(复位地址)		
		MSR_MSP(*(volatile uint32_t*)appxaddr);					//初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
		jump2app();									//跳转到APP.
	}

}		 


/*********************************************************************
*
*	函数名: copy_data_to_app2_addr
*	功  能: 将A区域空间数据拷贝到APP区
*	输  入: uint32_t datalen需要拷贝的数据
*	输  出:
*	返  回: 
*	修订记录：
*			 
**********************************************************************/
void copy_dataa_to_app_addr(uint32_t datalen)
{
	uint32_t  cnt;
	uint32_t m_data_loop;
	
	m_data_loop = datalen/2+datalen%2;
	

	STMFLASH_EraseSec(APP_START_ADDRESS,UPDATA_BACKA_ADDRESS);//擦除APP区
	
	HAL_FLASH_Unlock();	//解锁
	
	for(cnt=0;cnt<m_data_loop;cnt++)//写数据
	{
     if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,APP_START_ADDRESS+2*cnt,STMFLASH_ReadHalfWord(UPDATA_BACKA_ADDRESS+2*cnt))!=HAL_OK)//写入数据
			{ 
				return ;	//写入异常
			}
	} 
	
	HAL_FLASH_Lock();//上锁  

}

void copy_app_to_datab_addr(void)
{
	uint32_t  cnt;

	STMFLASH_EraseSec(UPDATA_BACKB_ADDRESS,UPDATA_BACKC_ADDRESS);//擦除B区
	
	HAL_FLASH_Unlock();	//解锁
	
	for(cnt=0;cnt<SIZE_60KB/2;cnt++)//写数据
	{
     if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,UPDATA_BACKB_ADDRESS+2*cnt,STMFLASH_ReadHalfWord(APP_START_ADDRESS+2*cnt))!=HAL_OK)//写入数据
			{ 
				printf("copy app to b error\n");
				return ;	//写入异常
			}
	} 
	
	HAL_FLASH_Lock();//上锁  
	printf("copy app to b ok\n");
}

void copy_datab_to_app_addr(void)
{
		uint32_t  cnt;

	STMFLASH_EraseSec(APP_START_ADDRESS,UPDATA_BACKA_ADDRESS);//擦除APP区
	
	HAL_FLASH_Unlock();	//解锁
	
	for(cnt=0;cnt<SIZE_60KB/2;cnt++)//写数据
	{
     if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,APP_START_ADDRESS+2*cnt,STMFLASH_ReadHalfWord(UPDATA_BACKB_ADDRESS+2*cnt))!=HAL_OK)//写入数据
			{ 
				printf("copy b to app error\n");
				return ;	//写入异常
			}
	} 
	
	HAL_FLASH_Lock();//上锁  
	printf("copy b to app ok\n");
}