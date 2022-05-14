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

	FlashEraseInit.TypeErase=FLASH_TYPEERASE_PAGES;       //��������,ҳ����
	FlashEraseInit.PageAddress=Start_Page;   //Ҫ��������ʼҳ
	FlashEraseInit.NbPages=(End_Page-Start_Page)/FLASH_PAGE_SIZE;      //Ҫ������ҳ��

	
	HAL_FLASHEx_Erase(&FlashEraseInit,&PAGEError);
	
	//FLASH_WaitForLastOperation(FLASH_WAITETIME);                //�ȴ��ϴβ������
		
	HAL_FLASH_Lock();//����

}


//��ָ����ַ��ʼд��ָ�����ȵ�����
//WriteAddr:��ʼ��ַ(�˵�ַ����Ϊ2�ı���!!)
//pBuffer:����ָ��
//NumToWrite:����(16λ)��(����Ҫд���16λ���ݵĸ���.)
//#if STM32_FLASH_SIZE<256
//#define STM_SECTOR_SIZE 1024 //�ֽ�
//#else 
//#define STM_SECTOR_SIZE	2048
//#endif	
uint8_t STMFLASH_Write(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite)	
{
	uint32_t addrx=0;
	uint32_t endaddr=0;	

	addrx=WriteAddr;				//д�����ʼ��ַ
	
	endaddr=WriteAddr+NumToWrite;	//д��Ľ�����ַ
	
	HAL_FLASH_Unlock();	//����
	
	while(addrx<endaddr)//д����
	{
 
     if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,addrx,*pBuffer)!=HAL_OK)//д������
			{ 
				return 1;	//д���쳣
			}
		addrx+=2;
		pBuffer++;
	} 
	HAL_FLASH_Lock();//����

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

//��ȡָ����ַ����(32λ����) 
//faddr:����ַ 
//����ֵ:��Ӧ����.
uint32_t STMFLASH_ReadWord(uint32_t faddr)
{
	return *(volatile uint32_t*)faddr; 
}

//��ָ����ַ��ʼ����ָ�����ȵ�����
//ReadAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToRead:��(32λ)��
void STMFLASH_ReadNByte(uint32_t ReadAddr,uint8_t *pBuffer,uint32_t NumToRead)   	
{
	uint32_t i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadByte(ReadAddr);//��ȡ1���ֽ�.
		ReadAddr++;
	}
}
void STMFLASH_ReadNHalfWord(uint32_t ReadAddr,uint16_t *pBuffer,uint32_t NumToRead)   	
{
	uint32_t i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//��ȡ2���ֽ�.
		ReadAddr+=2;
	}
}

void STMFLASH_ReadNWord(uint32_t ReadAddr,uint32_t *pBuffer,uint32_t NumToRead)   	
{
	uint32_t i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadWord(ReadAddr);//��ȡ4���ֽ�.
		ReadAddr+=4;
	}
}


void iap_load_app(uint32_t appxaddr)
{
	if(((*(volatile uint32_t*)appxaddr)&0x2FFE0000)==0x20000000)	//���ջ����ַ�Ƿ�Ϸ�.//256K:0x2FFC0000  128K:0x2FFE0000 ��ͳһ���
	{ 
//		print_log("jump to app\n");
		jump2app=(iapfun)*(volatile uint32_t*)(appxaddr+4);		//�û��������ڶ�����Ϊ����ʼ��ַ(��λ��ַ)		
		MSR_MSP(*(volatile uint32_t*)appxaddr);					//��ʼ��APP��ջָ��(�û��������ĵ�һ�������ڴ��ջ����ַ)
//		HAL_DeInit();
		jump2app();									//��ת��APP.
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
*	������: copy_data_to_app2_addr
*	��  ��: ��A����ռ����ݿ�����APP��
*	��  ��: uint32_t datalen��Ҫ����������
*	��  ��:
*	��  ��: 
*	�޶���¼��
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

	STMFLASH_EraseSec(APP_START_ADDRESS,UPDATA_BACKA_ADDRESS);//����APP��
	
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
*	������: copy_data_to_app2_addr
*	��  ��: ��APP�ռ����ݿ�����B��
*	��  ��: uint32_t datalen��Ҫ����������
*	��  ��:
*	��  ��: 
*	�޶���¼��
*			 
**********************************************************************/
void copy_app_to_datab_addr(void)
{
	uint32_t i;
	uint16_t ar_flash_buff[SIZE_1KB];
	
	STMFLASH_EraseSec(UPDATA_BACKB_ADDRESS,UPDATA_BACKC_ADDRESS);//����B��
	
	for(i=0;i<SIZE_60KB/SIZE_2KB;i++)
  {
		 STMFLASH_ReadNHalfWord(APP_START_ADDRESS+SIZE_2KB*i,ar_flash_buff,SIZE_1KB);
		 STMFLASH_Write(UPDATA_BACKB_ADDRESS+SIZE_2KB*i,ar_flash_buff,SIZE_1KB);;	
	} 
	
}

/*********************************************************************
*
*	������: copy_data_to_app2_addr
*	��  ��: ��B�����е����ݿ�����APP��
*	��  ��: 
*	��  ��:
*	��  ��: 
*	�޶���¼��
*			 
**********************************************************************/
void copy_datab_to_app_addr(void)
{
	uint32_t i;
	uint16_t ar_flash_buff[SIZE_1KB];
	
	STMFLASH_EraseSec(APP_START_ADDRESS,UPDATA_BACKA_ADDRESS);//����APP��
	
	for(i=0;i<SIZE_60KB/SIZE_2KB;i++)
  {
		 STMFLASH_ReadNHalfWord(UPDATA_BACKB_ADDRESS+SIZE_2KB*i,ar_flash_buff,SIZE_1KB);

		 STMFLASH_Write(APP_START_ADDRESS+SIZE_2KB*i,ar_flash_buff,SIZE_1KB);;	
	} 
	
}



