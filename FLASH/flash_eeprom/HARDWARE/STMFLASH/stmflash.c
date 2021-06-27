#include "stmflash.h"
	

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
void STMFLASH_Read(uint32_t ReadAddr,uint32_t *pBuffer,uint32_t NumToRead)   	
{
	uint32_t i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadWord(ReadAddr);//��ȡ4���ֽ�.
		ReadAddr+=4;//ƫ��4���ֽ�.	
	}
}








