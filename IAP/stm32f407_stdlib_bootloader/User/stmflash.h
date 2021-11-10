#ifndef __STMFLASH_H__
#define __STMFLASH_H__
#include "sys.h"  

typedef  void (*iapfun)(void);				//定义一个函数类型的参数.

#define  APP_START_ADDRESS         0x8020000
#define  UPGRADE_START_ADDRESS     0x8040000
#define  UPGRATDE_SIGN_ADDRESS     0x8060000

extern void iap_load_app(u32 appxaddr);
extern u8 STMFLASH_Write(u32 WriteAddr,u8 *pBuffer,u16 NumToWrite);		//从指定地址开始写入指定长度的数据
extern void STMFLASH_Read(u32 ReadAddr,u8 *pBuffer,u16 NumToRead);   		//从指定地址开始读出指定长度的数据
extern void STMFLASH_EraseSec(u32 Start_Sector,u32 End_Sector);
extern u8 STMFLASH_ReadByte(u32 faddr);
extern void Copy_Bin_Data_To_App(u32 Dest_Addr,u32 Source_Addr, u32 _Lenth);						   
#endif

















