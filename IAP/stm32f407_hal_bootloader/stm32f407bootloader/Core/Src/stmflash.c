#include "stmflash.h"
#include "stm32f407xx.h"
#include "usart.h"	
	
iapfun jump2app; 
__asm void MSR_MSP(uint32_t addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}
uint32_t STMFLASH_GetFlashSector(uint32_t _num)
{
	if(_num==0)return FLASH_SECTOR_0;
	else if(_num==1)return FLASH_SECTOR_1;
	else if(_num==2)return FLASH_SECTOR_2;
	else if(_num==3)return FLASH_SECTOR_3;
	else if(_num==4)return FLASH_SECTOR_4;
	else if(_num==5)return FLASH_SECTOR_5;
	else if(_num==6)return FLASH_SECTOR_6;
	else if(_num==7)return FLASH_SECTOR_7;
	else if(_num==8)return FLASH_SECTOR_8;
	else if(_num==9)return FLASH_SECTOR_9;
	else if(_num==10)return FLASH_SECTOR_10;
	return FLASH_SECTOR_11;	
}


void STMFLASH_EraseSec(uint32_t Start_Sector,uint32_t End_Sector)
{

	FLASH_EraseInitTypeDef FlashEraseInit;
//	HAL_StatusTypeDef FlashStatus=HAL_OK;
	uint32_t SectorError=0;
	HAL_FLASH_Unlock();

	FlashEraseInit.TypeErase=FLASH_TYPEERASE_SECTORS;       //擦除类型，扇区擦除 
	FlashEraseInit.Sector=STMFLASH_GetFlashSector(Start_Sector);   //要擦除的扇区
	FlashEraseInit.NbSectors=End_Sector-Start_Sector+1;      //一次擦除扇区数量
	FlashEraseInit.VoltageRange=FLASH_VOLTAGE_RANGE_3;      //电压范围，VCC=2.7~3.6V之间!!
	
	HAL_FLASHEx_Erase(&FlashEraseInit,&SectorError);
	
	//FLASH_WaitForLastOperation(FLASH_WAITETIME);                //等待上次操作完成
		
	HAL_FLASH_Lock();//上锁

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
uint8_t STMFLASH_Write(uint32_t WriteAddr,uint8_t *pBuffer,uint16_t NumToWrite)	
{
	uint32_t addrx=0;
	uint32_t endaddr=0;	

	addrx=WriteAddr;				//写入的起始地址
	
	endaddr=WriteAddr+NumToWrite;	//写入的结束地址
	
	HAL_FLASH_Unlock();	//解锁
	
	while(addrx<endaddr)//写数据
	{
 
        if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE,addrx,*pBuffer)!=HAL_OK)//写入数据
			{ 
				return 1;	//写入异常
			}
		addrx++;
		pBuffer++;
	} 
	HAL_FLASH_Lock();//上锁

	return 0;
}


uint8_t STMFLASH_ReadByte(uint32_t faddr)
{
	return *(uint8_t*)faddr; 
}

//读取指定地址的字(32位数据) 
//faddr:读地址 
//返回值:对应数据.
uint32_t STMFLASH_ReadWord(uint32_t faddr)
{
	return *(volatile uint32_t*)faddr; 
}

//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//pBuffer:数据指针
//NumToRead:字(32位)数
void STMFLASH_Read(uint32_t ReadAddr,uint8_t *pBuffer,uint16_t NumToRead)   	
{
		uint16_t i;
		for(i=0;i<NumToRead;i++)
		{
				pBuffer[i]=STMFLASH_ReadByte(ReadAddr);//读取4个字节.
				ReadAddr++;
		}
}
void iap_load_app(uint32_t appxaddr)
{
	if(((*(volatile uint32_t*)appxaddr)&0x2FFE0000)==0x20000000)	//检查栈顶地址是否合法.//256K:0x2FFC0000  128K:0x2FFE0000 可统一最大
	{ 
		printf("runing1\n");
		jump2app=(iapfun)*(volatile uint32_t*)(appxaddr+4);		//用户代码区第二个字为程序开始地址(复位地址)		
		MSR_MSP(*(volatile uint32_t*)appxaddr);					//初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
//		HAL_DeInit();
		jump2app();									//跳转到APP.
	}

}		 

void Copy_Bin_Data_To_App(uint32_t Dest_Addr,uint32_t Source_Addr, uint32_t _Lenth)
{
		uint32_t _cnt;
		
		HAL_FLASH_Unlock();						//解锁
//		FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
//										FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 

		for(_cnt=0;_cnt<_Lenth;_cnt++)
		{
//			 WDTFeed();
//			 FLASH_ProgramByte(Dest_Addr +_cnt, STMFLASH_ReadByte(Source_Addr +_cnt));
			HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE,Dest_Addr +_cnt,STMFLASH_ReadByte(Source_Addr +_cnt));
		}
		
		HAL_FLASH_Lock();//上锁
//		while(FLASH_GetFlagStatus(FLASH_FLAG_BSY)==1);
}






