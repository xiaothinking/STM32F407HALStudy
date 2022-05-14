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
	if(((*(volatile uint32_t*)appxaddr)&0x2FFE0000)==0x20000000)	//���ջ����ַ�Ƿ�Ϸ�.//256K:0x2FFC0000  128K:0x2FFE0000 ��ͳһ���
	{ 
		printf("jump to app\n");
		jump2app=(iapfun)*(volatile uint32_t*)(appxaddr+4);		//�û��������ڶ�����Ϊ����ʼ��ַ(��λ��ַ)		
		MSR_MSP(*(volatile uint32_t*)appxaddr);					//��ʼ��APP��ջָ��(�û��������ĵ�һ�������ڴ��ջ����ַ)
		jump2app();									//��ת��APP.
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
	uint32_t  cnt;
	uint32_t m_data_loop;
	
	m_data_loop = datalen/2+datalen%2;
	

	STMFLASH_EraseSec(APP_START_ADDRESS,UPDATA_BACKA_ADDRESS);//����APP��
	
	HAL_FLASH_Unlock();	//����
	
	for(cnt=0;cnt<m_data_loop;cnt++)//д����
	{
     if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,APP_START_ADDRESS+2*cnt,STMFLASH_ReadHalfWord(UPDATA_BACKA_ADDRESS+2*cnt))!=HAL_OK)//д������
			{ 
				return ;	//д���쳣
			}
	} 
	
	HAL_FLASH_Lock();//����  

}

void copy_app_to_datab_addr(void)
{
	uint32_t  cnt;

	STMFLASH_EraseSec(UPDATA_BACKB_ADDRESS,UPDATA_BACKC_ADDRESS);//����B��
	
	HAL_FLASH_Unlock();	//����
	
	for(cnt=0;cnt<SIZE_60KB/2;cnt++)//д����
	{
     if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,UPDATA_BACKB_ADDRESS+2*cnt,STMFLASH_ReadHalfWord(APP_START_ADDRESS+2*cnt))!=HAL_OK)//д������
			{ 
				printf("copy app to b error\n");
				return ;	//д���쳣
			}
	} 
	
	HAL_FLASH_Lock();//����  
	printf("copy app to b ok\n");
}

void copy_datab_to_app_addr(void)
{
		uint32_t  cnt;

	STMFLASH_EraseSec(APP_START_ADDRESS,UPDATA_BACKA_ADDRESS);//����APP��
	
	HAL_FLASH_Unlock();	//����
	
	for(cnt=0;cnt<SIZE_60KB/2;cnt++)//д����
	{
     if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,APP_START_ADDRESS+2*cnt,STMFLASH_ReadHalfWord(UPDATA_BACKB_ADDRESS+2*cnt))!=HAL_OK)//д������
			{ 
				printf("copy b to app error\n");
				return ;	//д���쳣
			}
	} 
	
	HAL_FLASH_Lock();//����  
	printf("copy b to app ok\n");
}