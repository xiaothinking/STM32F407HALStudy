#include "stmflash.h"

iapfun jump2app; 
__asm void MSR_MSP(uint32_t addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}

void STMFLASH_EraseSec(uint32_t Start_Page,uint32_t End_Page)
{

	FLASH_EraseInitTypeDef FlashEraseInit;
//	HAL_StatusTypeDef FlashStatus=HAL_OK;
	uint32_t PAGEError=0;
	HAL_FLASH_Unlock();

	FlashEraseInit.TypeErase=FLASH_TYPEERASE_PAGES;       //擦除类型,页擦除
	FlashEraseInit.PageAddress=Start_Page;   //要擦除的起始页
	FlashEraseInit.NbPages=(End_Page-Start_Page)/FLASH_PAGE_SIZE;      //要擦除的页数

	
	HAL_FLASHEx_Erase(&FlashEraseInit,&PAGEError);
	
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
uint8_t STMFLASH_Write(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite)	
{
	uint32_t addrx=0;
	uint32_t endaddr=0;	

	addrx=WriteAddr;				//写入的起始地址
	
	endaddr=WriteAddr+NumToWrite;	//写入的结束地址
	
	HAL_FLASH_Unlock();	//解锁
	
	while(addrx<endaddr)//写数据
	{
 
     if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,addrx,*pBuffer)!=HAL_OK)//写入数据
			{ 
				return 1;	//写入异常
			}
		addrx+=2;
		pBuffer++;
	} 
	HAL_FLASH_Lock();//上锁

	return 0;
}


uint8_t STMFLASH_ReadByte(uint32_t faddr)
{
	return *(volatile uint8_t*)faddr; 
}

uint16_t STMFLASH_ReadHalfWord(uint32_t faddr)
{
	return *(uint16_t*)faddr; 
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
void STMFLASH_ReadNByte(uint32_t ReadAddr,uint8_t *pBuffer,uint32_t NumToRead)   	
{
	uint32_t i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadByte(ReadAddr);//读取1个字节.
		ReadAddr++;
	}
}
void STMFLASH_ReadNHalfWord(uint32_t ReadAddr,uint16_t *pBuffer,uint32_t NumToRead)   	
{
	uint32_t i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//读取2个字节.
		ReadAddr+=2;
	}
}

void STMFLASH_ReadNWord(uint32_t ReadAddr,uint32_t *pBuffer,uint32_t NumToRead)   	
{
	uint32_t i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadWord(ReadAddr);//读取4个字节.
		ReadAddr+=4;
	}
}


void iap_load_app(uint32_t appxaddr)
{
	if(((*(volatile uint32_t*)appxaddr)&0x2FFE0000)==0x20000000)	//检查栈顶地址是否合法.//256K:0x2FFC0000  128K:0x2FFE0000 可统一最大
	{ 
//		print_log("jump to app\n");
		jump2app=(iapfun)*(volatile uint32_t*)(appxaddr+4);		//用户代码区第二个字为程序开始地址(复位地址)		
		MSR_MSP(*(volatile uint32_t*)appxaddr);					//初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
//		HAL_DeInit();
		jump2app();									//跳转到APP.
	}

}		 


void iap_write_appbin(uint32_t appxaddr,uint8_t *appbuf,uint32_t appsize)
{
	uint32_t t;
	uint16_t i=0;
	uint16_t temp;
	uint32_t fwaddr=appxaddr;//???????
	uint8_t *dfu=appbuf;
	uint16_t iapbuf[1024];  
	
	for(t = 0;t < appsize; t+=2)
	{						    
		temp=(uint16_t)dfu[1]<<8;
		temp+=(uint16_t)dfu[0];	 
//    temp=(uint16_t)dfu[1];
//		temp+=(uint16_t)dfu[0]<<8;		
		dfu+=2;//??2???
		iapbuf[i++]=temp;	    
		if(i==1024)
		{
			i=0;
			STMFLASH_Write(fwaddr,iapbuf,1024);
			fwaddr+=2048;//??2048  16=2*8.?????2.
		}
	}
	if(i)
	{
	  STMFLASH_Write(fwaddr,iapbuf,i);//?????????????.
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
	uint8_t m_sector_num = 0;
	uint16_t m_num_of_single = 0;
	uint8_t m_sector_cnt = 0;
	uint8_t ar_2k_buf[2048];
	
	m_num_of_single = datalen % SIZE_2KB;
  m_sector_num = datalen/SIZE_2KB;

	STMFLASH_EraseSec(APP_START_ADDRESS,UPDATA_BACKA_ADDRESS);//擦除APP区
	
  if(m_num_of_single == 0)
		{
		  while((m_sector_cnt < m_sector_num))
			{
			  STMFLASH_ReadNByte(UPDATA_BACKA_ADDRESS+m_sector_cnt*SIZE_2KB,ar_2k_buf,SIZE_2KB);
				iap_write_appbin(APP_START_ADDRESS+m_sector_cnt*SIZE_2KB,ar_2k_buf,SIZE_2KB);
				m_sector_cnt++;
			}
		}
	else
		{
		  while((m_sector_cnt < m_sector_num))
			{
			  STMFLASH_ReadNByte(UPDATA_BACKA_ADDRESS+m_sector_cnt*SIZE_2KB,ar_2k_buf,SIZE_2KB);			
				iap_write_appbin(APP_START_ADDRESS+m_sector_cnt*SIZE_2KB,ar_2k_buf,SIZE_2KB);			
				m_sector_cnt++;
			}						
		  STMFLASH_ReadNByte(UPDATA_BACKA_ADDRESS+m_sector_cnt*SIZE_2KB,ar_2k_buf,m_num_of_single);
			iap_write_appbin(APP_START_ADDRESS+m_sector_cnt*SIZE_2KB,ar_2k_buf,m_num_of_single);			
		}

}

/*********************************************************************
*
*	函数名: copy_data_to_app2_addr
*	功  能: 将APP空间数据拷贝到B区
*	输  入: uint32_t datalen需要拷贝的数据
*	输  出:
*	返  回: 
*	修订记录：
*			 
**********************************************************************/
void copy_app_to_datab_addr(void)
{
	uint32_t i;
	uint16_t ar_flash_buff[SIZE_1KB];
	
	STMFLASH_EraseSec(UPDATA_BACKB_ADDRESS,UPDATA_BACKC_ADDRESS);//擦除B区
	
	for(i=0;i<SIZE_60KB/SIZE_2KB;i++)
  {
		 STMFLASH_ReadNHalfWord(APP_START_ADDRESS+SIZE_2KB*i,ar_flash_buff,SIZE_1KB);
		 STMFLASH_Write(UPDATA_BACKB_ADDRESS+SIZE_2KB*i,ar_flash_buff,SIZE_1KB);;	
	} 
	
}

/*********************************************************************
*
*	函数名: copy_data_to_app2_addr
*	功  能: 将B区域中的数据拷贝到APP区
*	输  入: 
*	输  出:
*	返  回: 
*	修订记录：
*			 
**********************************************************************/
void copy_datab_to_app_addr(void)
{
	uint32_t i;
	uint16_t ar_flash_buff[SIZE_1KB];
	
	STMFLASH_EraseSec(APP_START_ADDRESS,UPDATA_BACKA_ADDRESS);//擦除APP区
	
	for(i=0;i<SIZE_60KB/SIZE_2KB;i++)
  {
		 STMFLASH_ReadNHalfWord(UPDATA_BACKB_ADDRESS+SIZE_2KB*i,ar_flash_buff,SIZE_1KB);

		 STMFLASH_Write(APP_START_ADDRESS+SIZE_2KB*i,ar_flash_buff,SIZE_1KB);;	
	} 
	
}



