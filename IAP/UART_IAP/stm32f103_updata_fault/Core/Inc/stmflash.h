#ifndef __STMFLASH_H
#define __STMFLASH_H
#include "stm32f1xx_hal.h"


typedef  void (*iapfun)(void);				//定义一个函数类型的参数.

#define  APP_START_ADDRESS         0x08005000  //APP起始地址(0x08005000-0x08014000),60KB
#define  UPDATA_BACKA_ADDRESS      0x08014000  //接收上位机下发的数据的起始地址，A区(0x08014000~0x08023000),60KB
#define  UPDATA_BACKB_ADDRESS      0x08023000  //存储当前app区域代码于flash的起始地址，B区(0x08023000~0x08032000),60KB
#define  UPDATA_BACKC_ADDRESS      0x08032000  //固件升级标志位的起始地址，C区(0x08032000~0x08032800),2KB
#define  UPDATA_BACKD_ADDRESS      0x08032800  //固件升级异常标志位起始地址，D区(0x08032800~0x08033000),2KB

#define  SIZE_60KB        0xF000
#define  SIZE_1KB         0x400
#define  SIZE_2KB         0x800
#define  SIZE_4KB         0x1000  /*4K*/

void iap_load_app(uint32_t appxaddr);
void iap_write_appbin(uint32_t appxaddr,uint8_t *appbuf,uint32_t appsize);
void copy_dataa_to_app_addr(uint32_t datalen);
void copy_app_to_datab_addr(void);
void copy_datab_to_app_addr(void);

void STMFLASH_EraseSec(uint32_t Start_Page,uint32_t End_Page);
uint8_t STMFLASH_Write(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite);
uint8_t STMFLASH_ReadByte(uint32_t faddr);
uint16_t STMFLASH_ReadHalfWord(uint32_t faddr);
uint32_t STMFLASH_ReadWord(uint32_t faddr);
void STMFLASH_ReadNByte(uint32_t ReadAddr,uint8_t *pBuffer,uint32_t NumToRead);
void STMFLASH_ReadNHalfWord(uint32_t ReadAddr,uint16_t *pBuffer,uint32_t NumToRead);  
void STMFLASH_ReadNWord(uint32_t ReadAddr,uint32_t *pBuffer,uint32_t NumToRead);

#endif
