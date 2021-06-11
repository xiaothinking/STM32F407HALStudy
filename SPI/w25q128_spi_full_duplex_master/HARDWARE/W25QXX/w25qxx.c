/************************************************
W25Q16容量为2M字节,共有32个Block,512个Sector 
4Kbytes为一个Sector
16个扇区为1个Block
************************************************/
#include "w25qxx.h" 
#include "spi.h"
#include "main.h" 
#include <stdint.h>

/*********************************************************************
*
*	function name  : SPI_W25X_ReadSR   
*	function       : read W25Q16 status register
*	input          :
*	output         :  nonce
*	return         :  byte
*	revised record ：1.
*                  2.
*remarks         :  
BIT:   7    6    5    4    3    2    1    0
       SPR  RV   TB   BP2  BP1  BP0  WEL  BUSY
SPR:默认0,状态寄存器保护位,配合WP使用
TB,BP2,BP1,BP0:FLASH区域写保护设置
WEL:写使能锁定
BUSY:忙标记位(1,忙;0,空闲)
默认:0x00
**********************************************************************/
u8  SPI_W25X_ReadSR(void)   
{  
  u8  byte=0;
	   
  W25XCS_L;  /*enable chip selection*/
  SPI1_ReadWriteByte(W25X_ReadStatusReg);  /*send read status register command */    
  byte=SPI1_ReadWriteByte(0Xff);           /*read one byte */ 
  W25XCS_H;	   /*disable chip selection*/

  return byte;   
} 
/*********************************************************************
*
*	function name  : SPI_W25X_Write_SR   
*	function       : write W25Q16 status register
*	input          : sr
*	output         :  nonce
*	return         :  nonce
*	revised record ：1.
*                  2.
*remarks         :  

**********************************************************************/
void SPI_W25X_Write_SR(u8  sr)   
{   
  W25XCS_L;	 /*enable chip selection*/
  SPI1_ReadWriteByte(W25X_WriteStatusReg);  /*send write status register command */   
  SPI1_ReadWriteByte(sr);                   /*write one byte*/   
  W25XCS_H;	  /*disable chip selection*/  
}   

/*********************************************************************
*
*	function name  : SPI_W25X_Write_Enable   
*	function       : enble W25Q16 write
*	input          : nonce
*	output         : nonce
*	return         : nonce
*	revised record ：1.
*                  2.
*remarks         :  
**********************************************************************/
void SPI_W25X_Write_Enable(void)   
{
  W25XCS_L;   /*enable chip selection*/
  SPI1_ReadWriteByte(W25X_WriteEnable);    //send write enable   
  W25XCS_H;  /*disable chip selection*/
} 

/*********************************************************************
*
*	function name  : SPI_W25X_Write_Disable   
*	function       : disable W25Q16 write
*	input          : nonce
*	output         : nonce
*	return         : nonce
*	revised record ：1.
*                  2.
*remarks         :  
**********************************************************************/
void SPI_W25X_Write_Disable(void)   
{  
  W25XCS_L;  /*enable chip selection*/
  SPI1_ReadWriteByte(W25X_WriteDisable);   //send write disable     
  W25XCS_H;	  /*disable chip selection*/
} 			    

/*********************************************************************
*
*	function name  : SPI_W25X_ReadID   
*	function       : read w25q16 ID 
*	input          : nonce
*	output         : nonce
*	return         : Temp
*	revised record ：1.
*                  2.
*remarks         :  ID:0XEF14
**********************************************************************/
u16 SPI_W25X_ReadID(void)
{
  u16 Temp = 0;
		  
  W25XCS_L;	 /*enable chip selection*/
  SPI1_ReadWriteByte(W25X_ManufactDeviceID);
  SPI1_ReadWriteByte(0x00); 	    
  SPI1_ReadWriteByte(0x00); 	    
  SPI1_ReadWriteByte(0x00); 	 			   
  Temp|=SPI1_ReadWriteByte(0xFF)<<8;  
  Temp|=SPI1_ReadWriteByte(0xFF);	 
  W25XCS_H;	    /*disable chip selection*/
  
  return Temp;
}   		    

/*********************************************************************
*
*	function name  : SPI_W25X_Read   
*	function       : reading data of specified length at specified address 
*	input          : 1 pBuffer ：data store
                   2 ReadAddr: address to start reading(24bit)
                   3 NumByteToRead：Bytes to read(最大65535)
*	output         : nonce
*	return         : nonce
*	revised record ：1.
*                  2.
*remarks         :  
**********************************************************************/
void SPI_W25X_Read(u8  *pBuffer,u32 ReadAddr,u16 NumByteToRead)   
{ 
  u16 i;
	    												    
  W25XCS_L;
  SPI1_ReadWriteByte(W25X_ReadData);
  SPI1_ReadWriteByte((u8 )((ReadAddr)>>16));
  SPI1_ReadWriteByte((u8 )((ReadAddr)>>8));   
  SPI1_ReadWriteByte((u8 )ReadAddr);   
    
  for(i=0;i<NumByteToRead;i++)
  { 
    pBuffer[i]=SPI1_ReadWriteByte(0XFF);
  }
  W25XCS_H;
}  
//-----------------------------------------------------------------------------
//在一页(0~65535)内写入少于256个字节的数据
//从指定地址开始写入最大256字节的数据
//pBuffer:数据存储区地址
//WriteAddr:写入区首地址
//NumByteToWrite:待写入的字节数(最大256,且不得超过本页的剩余字节数)
void SPI_W25X_Write_Page(u8 * pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
  u16 i;
	  
  SPI_W25X_Write_Enable();				 //使能片选
  W25XCS_L;
  SPI1_ReadWriteByte(W25X_PageProgram);
  SPI1_ReadWriteByte((u8 )((WriteAddr)>>16));
  SPI1_ReadWriteByte((u8 )((WriteAddr)>>8));   
  SPI1_ReadWriteByte((u8 )WriteAddr);   
  
  for(i=0;i<NumByteToWrite;i++)
  {
    SPI1_ReadWriteByte(pBuffer[i]);
  }

  W25XCS_H;	 //关闭片选
  SPI_W25X_Wait_Busy();				 //等待写入结束
} 
//-----------------------------------------------------------------------------
//无检验写SPI FLASH 
//必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
//具有自动换页功能 
//在指定地址开始写入指定长度的数据,但是要确保地址不越界!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)
//CHECK OK
void SPI_W25X_Write_NoCheck(u8 * pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 			 		 
  u16 pageremain;
		   
  pageremain=256-WriteAddr%256;
  if(NumByteToWrite<=pageremain)
    pageremain=NumByteToWrite;        //不大于256字节
  while(1)
  {	   
    SPI_W25X_Write_Page(pBuffer,WriteAddr,pageremain);
    if(NumByteToWrite==pageremain)
	  break;   //写入结束
    else
    {
      pBuffer+=pageremain;
      WriteAddr+=pageremain;	

      NumByteToWrite-=pageremain;	  //减去已经写入的字节
      if(NumByteToWrite>256)
	    pageremain=256;       
      else 
	    pageremain=NumByteToWrite; 	  //不够256个字节
    }
  }
} 
//-----------------------------------------------------------------------------
//写W25X  
//在指定地址开始写入指定长度的数据
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)  		   
u8  W25X_BUF[4096];
void SPI_W25X_Write(u8  *pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 
  u32 secpos;
  u16 secoff;
  u16 secremain;	   
  u16 i;    

  secpos=WriteAddr/4096;      //扇区地址
  secoff=WriteAddr%4096;      //在扇区内的偏移
  secremain=4096-secoff;      //扇区剩余空间

  if(NumByteToWrite<=secremain)
    secremain=NumByteToWrite; //不大于4096字节
  while(1) 
  {	
    SPI_W25X_Read(W25X_BUF,secpos*4096,4096);//读该扇区
    for(i=0;i<secremain;i++)   //校验
    {
      if(W25X_BUF[secoff+i]!=0XFF)
	    break;     //需要擦除  	  
    }
    
	if(i<secremain)//需要擦除
    {
      SPI_W25X_Erase_Sector(secpos);  //擦除该扇区
      for(i=0;i<secremain;i++)	       //复制
      {
        W25X_BUF[i+secoff]=pBuffer[i];	  
      }
      SPI_W25X_Write_NoCheck(W25X_BUF,secpos*4096,4096); //写扇区 

    }
	else 
	  SPI_W25X_Write_NoCheck(pBuffer,WriteAddr,secremain); //已经擦除,直接写入剩余扇区
		
    if(NumByteToWrite==secremain)
      break;  //写入完成
    else      //写入未完
    {
      secpos++; //扇区地址增1
      secoff=0; //偏移位置为0 	 

      pBuffer+=secremain;
      WriteAddr+=secremain;
      NumByteToWrite-=secremain;
      
	  if(NumByteToWrite>4096)
        secremain=4096;	           //下一扇区仍不够
      else 
        secremain=NumByteToWrite;  //下一扇区可以写完
    }	 
  }
}
//-----------------------------------------------------------------------------
//擦除整个芯片
//整片擦除时间:
//W25X16:25s 
//W25X32:40s 
//W25X64:40s 
//等待时间超长...
void SPI_W25X_Erase_Chip(void)   
{                                             
  SPI_W25X_Write_Enable();                  //SET WEL 
  SPI_W25X_Wait_Busy();   
  W25XCS_L;	 //使能片选
  SPI1_ReadWriteByte(W25X_ChipErase);        //发送片擦除命令  
  W25XCS_H;	     //关闭片选
  SPI_W25X_Wait_Busy();   				     //等待芯片擦除结束
}   
//-----------------------------------------------------------------------------
//擦除一个扇区
//Dst_Addr:扇区地址 0~511 for w25x16
//擦除一个山区的最少时间:150ms
void SPI_W25X_Erase_Sector(u32 Dst_Addr)   
{   
  Dst_Addr*=4096;
  SPI_W25X_Write_Enable();                  //SET WEL 	 
  SPI_W25X_Wait_Busy();   
  W25XCS_L;	 //使能片选
  SPI1_ReadWriteByte(W25X_SectorErase);      //发送扇区擦除指令 
  SPI1_ReadWriteByte((u8 )((Dst_Addr)>>16));  //发送24bit地址    
  SPI1_ReadWriteByte((u8 )((Dst_Addr)>>8));   
  SPI1_ReadWriteByte((u8 )Dst_Addr);  
  W25XCS_H;	     //关闭片选
  SPI_W25X_Wait_Busy();   				     //等待擦除完成
}  
//-----------------------------------------------------------------------------
//等待空闲
void SPI_W25X_Wait_Busy(void)   
{   
  while ((SPI_W25X_ReadSR()&0x01)==0x01);   // 等待BUSY位清空
}  
//-----------------------------------------------------------------------------
//进入掉电模式
void SPI_W25X_PowerDown(void)   
{ 
  W25XCS_L;	 //使能片选
  SPI1_ReadWriteByte(W25X_PowerDown);        //发送掉电命令  
  W25XCS_H;	     //关闭片选
   HAL_Delay(3);            //等待TPD  
}   
//-----------------------------------------------------------------------------
//唤醒
void SPI_W25X_WAKEUP(void)   
{  
  W25XCS_L;	   //使能片选
  SPI1_ReadWriteByte(W25X_ReleasePowerDown);   //  send W25X_PowerDown command 0xAB    
  W25XCS_H;	       //关闭片选
  HAL_Delay(3);                  //等待TRES1
}   


























