#include "stmflash.h"
	

void STMFLASH_EraseSec(uint32_t Start_Page,uint32_t End_Page)
{

	FLASH_EraseInitTypeDef FlashEraseInit;

	uint32_t PAGEError=0;
	HAL_FLASH_Unlock();

	FlashEraseInit.TypeErase=FLASH_TYPEERASE_PAGES;       //擦除类型,页擦除
	FlashEraseInit.PageAddress=Start_Page;   //要擦除的起始页
	FlashEraseInit.NbPages=(End_Page - Start_Page) / FLASH_PAGE_SIZE;      //要擦除的页数

	HAL_FLASHEx_Erase(&FlashEraseInit,&PAGEError);
		
	HAL_FLASH_Lock();//上锁

}


//从指定地址开始写入指定长度的数据
uint8_t STMFLASH_Write_HalfWord(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite)	
{
	uint32_t addrx=0;
	uint32_t endaddr=0;	

	addrx=WriteAddr;				//写入的起始地址
	
	endaddr=WriteAddr+2*NumToWrite;	//写入的结束地址
	
	HAL_FLASH_Unlock();	//解锁
	
	while(addrx<endaddr)//写数据
	{
 
     if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,addrx,*pBuffer)!=HAL_OK)//写入数据
			{ 
				return 1;	//写入异常
			}
		addrx=addrx+2;
		pBuffer++;
	} 
	HAL_FLASH_Lock();//上锁

	return 0;
}

uint8_t STMFLASH_Write_Word(uint32_t WriteAddr,uint32_t *pBuffer,uint16_t NumToWrite)	
{
	uint32_t addrx=0;
	uint32_t endaddr=0;	

	addrx=WriteAddr;				//写入的起始地址
	
	endaddr=WriteAddr+4*NumToWrite;	//写入的结束地址
	
	HAL_FLASH_Unlock();	//解锁
	
	while(addrx<endaddr)//写数据
	{
 
     if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,addrx,*pBuffer)!=HAL_OK)//写入数据
			{ 
				return 1;	//写入异常
			}
		addrx=addrx+4;
		pBuffer++;
	} 
	HAL_FLASH_Lock();//上锁

	return 0;
}

uint8_t STMFLASH_ReadByte(uint32_t faddr)
{
	return *(uint8_t*)faddr; 
}

uint16_t STMFLASH_ReadHalfWord(uint32_t faddr)
{
	return *(uint16_t*)faddr; 
}

uint32_t STMFLASH_ReadWord(uint32_t faddr)
{
	return *(volatile uint32_t*)faddr; 
}

void STMFLASH_ReadNByte(uint32_t ReadAddr,uint8_t *pBuffer,uint32_t NumToRead)   	
{
	uint32_t i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadByte(ReadAddr);
		ReadAddr++;	
	}
}

void STMFLASH_ReadNHalfWord(uint32_t ReadAddr,uint16_t *pBuffer,uint32_t NumToRead)   	
{
	uint32_t i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);
		ReadAddr=ReadAddr+2;	
	}
}

void STMFLASH_ReadNWord(uint32_t ReadAddr,uint32_t *pBuffer,uint32_t NumToRead)   	
{
	uint32_t i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadWord(ReadAddr);
		ReadAddr=ReadAddr+4;;	
	}
}






