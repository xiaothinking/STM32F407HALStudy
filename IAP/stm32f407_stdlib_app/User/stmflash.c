#include "stmflash.h"
#include "io.h"
 iapfun jump2app; 
//��ת��Ӧ�ó����
//appxaddr:�û�������ʼ��ַ.
void iap_load_app(u32 appxaddr)
{
	//u32 temp;
	delay_ms(20);
	//__disable_irq();   // �ر����ж�
	//SCB->VTOR = STM32_FLASH_BASE | 0x20000;//����ƫ����
	NVIC_SetVectorTable(NVIC_VectTab_FLASH,0x20000);	 	 
	//temp=*(vu32*)appxaddr;
	if(((*(vu32*)appxaddr)&0x2FFC0000)==0x20000000)	//���ջ����ַ�Ƿ�Ϸ�.//256K:0x2FFC0000  128K:0x2FFE0000 ��ͳһ���
	{ 
		jump2app=(iapfun)*(vu32*)(appxaddr+4);		//�û��������ڶ�����Ϊ����ʼ��ַ(��λ��ַ)		
		MSR_MSP(*(vu32*)appxaddr);					//��ʼ��APP��ջָ��(�û��������ĵ�һ�������ڴ��ջ����ַ)
		jump2app();									//��ת��APP.
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
		//FLASH_EraseSector(FLASH_Sector_12, VoltageRange_3);//�洢��
		FLASH_Lock();//����
		while(FLASH_GetFlagStatus(FLASH_FLAG_BSY)==1);
}

u8 STMFLASH_ReadByte(u32 faddr)
{
	return *(u8*)faddr; 
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


//u8 verify_Status;

u8 STMFLASH_Write(u32 WriteAddr,u8 *pBuffer,u16 NumToWrite)	
{
	u32 addrx=0;
	u32 endaddr=0;	

	addrx=WriteAddr;				//д�����ʼ��ַ
	
	endaddr=WriteAddr+NumToWrite;	//д��Ľ�����ַ
	
	FLASH_Unlock();	
	//����
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 
	
	while(addrx<endaddr)//д����
	{
		if(FLASH_ProgramByte(addrx,*pBuffer)!=FLASH_COMPLETE)//д������
		{ 
			return 1;	//д���쳣
		}
		addrx++;
		pBuffer++;
	} 
	FLASH_Lock();//����

	return 0;
}


//��ָ����ַ��ʼ����ָ�����ȵ�����
//ReadAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��
void STMFLASH_Read(u32 ReadAddr,u8 *pBuffer,u16 NumToRead)   	
{
		u16 i;
		for(i=0;i<NumToRead;i++)
		{
				pBuffer[i]=STMFLASH_ReadByte(ReadAddr);//��ȡ4���ֽ�.
				ReadAddr++;
		}
}
void Copy_Bin_Data_To_App(u32 Dest_Addr,u32 Source_Addr, u32 _Lenth)
{
		u32 _cnt;
		
		FLASH_Unlock();						//����
		FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
										FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 

		for(_cnt=0;_cnt<_Lenth;_cnt++)
		{
			 WDTFeed();
			 FLASH_ProgramByte(Dest_Addr +_cnt, STMFLASH_ReadByte(Source_Addr +_cnt));
		}
		
		FLASH_Lock();//����
		while(FLASH_GetFlagStatus(FLASH_FLAG_BSY)==1);
}













