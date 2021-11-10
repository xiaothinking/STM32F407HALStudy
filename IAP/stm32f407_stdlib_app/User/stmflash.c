#include "stmflash.h"
#include "io.h"
 iapfun jump2app; 
//跳转到应用程序段
//appxaddr:用户代码起始地址.
void iap_load_app(u32 appxaddr)
{
	//u32 temp;
	delay_ms(20);
	//__disable_irq();   // 关闭总中断
	//SCB->VTOR = STM32_FLASH_BASE | 0x20000;//设置偏移量
	NVIC_SetVectorTable(NVIC_VectTab_FLASH,0x20000);	 	 
	//temp=*(vu32*)appxaddr;
	if(((*(vu32*)appxaddr)&0x2FFC0000)==0x20000000)	//检查栈顶地址是否合法.//256K:0x2FFC0000  128K:0x2FFE0000 可统一最大
	{ 
		jump2app=(iapfun)*(vu32*)(appxaddr+4);		//用户代码区第二个字为程序开始地址(复位地址)		
		MSR_MSP(*(vu32*)appxaddr);					//初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
		jump2app();									//跳转到APP.
		SoftReset();
	}
	else
	{
		SoftReset();
	}
}		 

uint32_t STMFLASH_GetFlashSector(u32 _num)
{
	if(_num==0)return FLASH_Sector_0;
	else if(_num==1)return FLASH_Sector_1;
	else if(_num==2)return FLASH_Sector_2;
	else if(_num==3)return FLASH_Sector_3;
	else if(_num==4)return FLASH_Sector_4;
	else if(_num==5)return FLASH_Sector_5;
	else if(_num==6)return FLASH_Sector_6;
	else if(_num==7)return FLASH_Sector_7;
	return FLASH_Sector_7;	
}


void STMFLASH_EraseSec(u32 Start_Sector,u32 End_Sector)
{
		u32 i;
		FLASH_Unlock();
		FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
										FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 		
		for(i=Start_Sector;i<=End_Sector;i++)
		{
			FLASH_EraseSector(STMFLASH_GetFlashSector(i), VoltageRange_3);
		}
		//FLASH_EraseSector(FLASH_Sector_12, VoltageRange_3);//存储区
		FLASH_Lock();//上锁
		while(FLASH_GetFlagStatus(FLASH_FLAG_BSY)==1);
}

u8 STMFLASH_ReadByte(u32 faddr)
{
	return *(u8*)faddr; 
}

//从指定地址开始写入指定长度的数据
//WriteAddr:起始地址(此地址必须为2的倍数!!)
//pBuffer:数据指针
//NumToWrite:半字(16位)数(就是要写入的16位数据的个数.)
//#if STM32_FLASH_SIZE<256
//#define STM_SECTOR_SIZE 1024 //字节
//#else 
//#define STM_SECTOR_SIZE	2048
//#endif	


//u8 verify_Status;

u8 STMFLASH_Write(u32 WriteAddr,u8 *pBuffer,u16 NumToWrite)	
{
	u32 addrx=0;
	u32 endaddr=0;	

	addrx=WriteAddr;				//写入的起始地址
	
	endaddr=WriteAddr+NumToWrite;	//写入的结束地址
	
	FLASH_Unlock();	
	//解锁
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 
	
	while(addrx<endaddr)//写数据
	{
		if(FLASH_ProgramByte(addrx,*pBuffer)!=FLASH_COMPLETE)//写入数据
		{ 
			return 1;	//写入异常
		}
		addrx++;
		pBuffer++;
	} 
	FLASH_Lock();//上锁

	return 0;
}


//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//pBuffer:数据指针
//NumToWrite:半字(16位)数
void STMFLASH_Read(u32 ReadAddr,u8 *pBuffer,u16 NumToRead)   	
{
		u16 i;
		for(i=0;i<NumToRead;i++)
		{
				pBuffer[i]=STMFLASH_ReadByte(ReadAddr);//读取4个字节.
				ReadAddr++;
		}
}
void Copy_Bin_Data_To_App(u32 Dest_Addr,u32 Source_Addr, u32 _Lenth)
{
		u32 _cnt;
		
		FLASH_Unlock();						//解锁
		FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
										FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 

		for(_cnt=0;_cnt<_Lenth;_cnt++)
		{
			 WDTFeed();
			 FLASH_ProgramByte(Dest_Addr +_cnt, STMFLASH_ReadByte(Source_Addr +_cnt));
		}
		
		FLASH_Lock();//上锁
		while(FLASH_GetFlagStatus(FLASH_FLAG_BSY)==1);
}













