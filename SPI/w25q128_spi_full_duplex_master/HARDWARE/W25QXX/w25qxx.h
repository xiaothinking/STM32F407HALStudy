#ifndef __W25QXX_H
#define __W25QXX_H	

#include "main.h"
#define	W25XCS_L	    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
#define	W25XCS_H	    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);

#define FLASH_ID 0XEF14   //W25X16��ID


//W25X16ָ���
#define W25X_WriteEnable		0x06  //дʹ��
#define W25X_WriteDisable		0x04  //д��ֹ
#define W25X_ReadStatusReg		0x05  //��״̬�Ĵ���
#define W25X_WriteStatusReg		0x01  //д״̬�Ĵ���
#define W25X_ReadData			0x03  //������
#define W25X_FastReadData		0x0B  //���
#define W25X_FastReadDual		0x3B  //���˫���
#define W25X_PageProgram		0x02  //ҳ���
#define W25X_BlockErase			0xD8  //�����(64K)
#define W25X_SectorErase		0x20  //��������(4K)
#define W25X_ChipErase			0xC7  //оƬ����
#define W25X_PowerDown			0xB9  //����
#define W25X_ReleasePowerDown	0xAB  //�ͷŵ���
#define W25X_DeviceID			0xAB  //������ID
#define W25X_ManufactDeviceID	0x90  //������ID+����ID
#define W25X_JedecDeviceID		0x9F  //��JEDEC ID

extern uint16_t SPI_W25X_ReadID(void);
u8 SPI_W25X_ReadSR(void);
void SPI_W25X_Write_SR(unsigned char  sr);
void SPI_W25X_Write_Enable(void);
void SPI_W25X_Write_Disable(void);
void SPI_W25X_Read(unsigned char * pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead);
void SPI_W25X_Write(unsigned char * pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);
void SPI_W25X_Erase_Chip(void);
void SPI_W25X_Erase_Sector(uint32_t Dst_Addr);
void SPI_W25X_Wait_Busy(void);
void SPI_W25X_PowerDown(void);
void SPI_W25X_WAKEUP(void);

#endif
















