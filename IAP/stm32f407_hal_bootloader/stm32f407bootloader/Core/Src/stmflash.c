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

	FlashEraseInit.TypeErase=FLASH_TYPEERASE_SECTORS;       //�������ͣ��������� 
	FlashEraseInit.Sector=STMFLASH_GetFlashSector(Start_Sector);   //Ҫ����������
	FlashEraseInit.NbSectors=End_Sector-Start_Sector+1;      //һ�β�����������
	FlashEraseInit.VoltageRange=FLASH_VOLTAGE_RANGE_3;      //��ѹ��Χ��VCC=2.7~3.6V֮��!!
	
	HAL_FLASHEx_Erase(&FlashEraseInit,&SectorError);
	
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
uint8_t STMFLASH_Write(uint32_t WriteAddr,uint8_t *pBuffer,uint16_t NumToWrite)	
{
	uint32_t addrx=0;
	uint32_t endaddr=0;	

	addrx=WriteAddr;				//д�����ʼ��ַ
	
	endaddr=WriteAddr+NumToWrite;	//д��Ľ�����ַ
	
	HAL_FLASH_Unlock();	//����
	
	while(addrx<endaddr)//д����
	{
 
        if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE,addrx,*pBuffer)!=HAL_OK)//д������
			{ 
				return 1;	//д���쳣
			}
		addrx++;
		pBuffer++;
	} 
	HAL_FLASH_Lock();//����

	return 0;
}


uint8_t STMFLASH_ReadByte(uint32_t faddr)
{
	return *(uint8_t*)faddr; 
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
void STMFLASH_Read(uint32_t ReadAddr,uint8_t *pBuffer,uint16_t NumToRead)   	
{
		uint16_t i;
		for(i=0;i<NumToRead;i++)
		{
				pBuffer[i]=STMFLASH_ReadByte(ReadAddr);//��ȡ4���ֽ�.
				ReadAddr++;
		}
}
void iap_load_app(uint32_t appxaddr)
{
	if(((*(volatile uint32_t*)appxaddr)&0x2FFE0000)==0x20000000)	//���ջ����ַ�Ƿ�Ϸ�.//256K:0x2FFC0000  128K:0x2FFE0000 ��ͳһ���
	{ 
		printf("runing1\n");
		jump2app=(iapfun)*(volatile uint32_t*)(appxaddr+4);		//�û��������ڶ�����Ϊ����ʼ��ַ(��λ��ַ)		
		MSR_MSP(*(volatile uint32_t*)appxaddr);					//��ʼ��APP��ջָ��(�û��������ĵ�һ�������ڴ��ջ����ַ)
//		HAL_DeInit();
		jump2app();									//��ת��APP.
	}

}		 

void Copy_Bin_Data_To_App(uint32_t Dest_Addr,uint32_t Source_Addr, uint32_t _Lenth)
{
		uint32_t _cnt;
		
		HAL_FLASH_Unlock();						//����
//		FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
//										FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 

		for(_cnt=0;_cnt<_Lenth;_cnt++)
		{
//			 WDTFeed();
//			 FLASH_ProgramByte(Dest_Addr +_cnt, STMFLASH_ReadByte(Source_Addr +_cnt));
			HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE,Dest_Addr +_cnt,STMFLASH_ReadByte(Source_Addr +_cnt));
		}
		
		HAL_FLASH_Lock();//����
//		while(FLASH_GetFlagStatus(FLASH_FLAG_BSY)==1);
}






