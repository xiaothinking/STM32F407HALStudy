#ifndef __W25QXX_H
#define __W25QXX_H	

#include "main.h"
#define	W25XCS_L	    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
#define	W25XCS_H	    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);

#define FLASH_ID 0XEF14   //W25X16µÄID


//W25X16Ö¸Áî±í
#define W25X_WriteEnable		0x06  //Ð´Ê¹ÄÜ
#define W25X_WriteDisable		0x04  //Ð´½ûÖ¹
#define W25X_ReadStatusReg		0x05  //¶Á×´Ì¬¼Ä´æÆ÷
#define W25X_WriteStatusReg		0x01  //Ð´×´Ì¬¼Ä´æÆ÷
#define W25X_ReadData			0x03  //¶ÁÊý¾Ý
#define W25X_FastReadData		0x0B  //¿ì¶Á
#define W25X_FastReadDual		0x3B  //¿ì¶ÁË«Êä³ö
#define W25X_PageProgram		0x02  //Ò³±à³Ì
#define W25X_BlockErase			0xD8  //¿é²Á³ý(64K)
#define W25X_SectorErase		0x20  //ÉÈÇø²Á³ý(4K)
#define W25X_ChipErase			0xC7  //Ð¾Æ¬²Á³ý
#define W25X_PowerDown			0xB9  //µôµç
#define W25X_ReleasePowerDown	0xAB  //ÊÍ·Åµôµç
#define W25X_DeviceID			0xAB  //¶ÁÆ÷¼þID
#define W25X_ManufactDeviceID	0x90  //¶ÁÖÆÔìID+Æ÷¼þID
#define W25X_JedecDeviceID		0x9F  //¶ÁJEDEC ID

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
















