#ifndef __24CXX_H_
#define __24CXX_H_

//#include "mysys.h"
#include "i2c.h"

#define delay_ms 		HAL_Delay


#define AT24C01		127		//PAGE_SIZE	8	byte
#define AT24C02		255		//PAGE_SIZE	8	byte
#define AT24C04		511		//PAGE_SIZE	16	byte
#define AT24C08		1023	//PAGE_SIZE	16	byte
#define AT24C16		2047	//PAGE_SIZE	16	byte
#define AT24C32		4095
#define AT24C64	  8191
#define AT24C128	16383
#define AT24C256	32767  
//������ʹ�õ���24c02�����Զ���EE_TYPEΪAT24C02
#define AT24CXX_TYPE									AT24C02
#define AT24CXX_ADDRESS								0XA0
#define AT24CXX_PAGE_SIZE							8

#if	(AT24CXX_TYPE < AT24C04)
#define AT24CXX_MEMADD_SIZE						I2C_MEMADD_SIZE_8BIT	
#else
#define AT24CXX_MEMADD_SIZE						I2C_MEMADD_SIZE_16BIT	
#endif


void AT24CXX_Init(void);
void AT24CXX_WriteByte(uint8_t Reg, uint8_t Value);
uint8_t AT24CXX_ReadByte(uint8_t Reg);
HAL_StatusTypeDef AT24CXX_PageWrite(uint16_t MemAddress, uint8_t* pBuffer, uint32_t BufferSize);
HAL_StatusTypeDef AT24CXX_WriteData(uint16_t MemAddress, uint8_t* pBuffer, uint32_t BufferSize);
HAL_StatusTypeDef AT24CXX_ReadData(uint16_t MemAddress, uint8_t* pBuffer, uint32_t BufferSize);
HAL_StatusTypeDef AT24CXX_IsDeviceReady(uint32_t Trials);

uint8_t AT24CXX_Check(void);



#endif












