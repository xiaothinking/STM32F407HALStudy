/************************************************
W25Q16����Ϊ2M�ֽ�,����32��Block,512��Sector 
4KbytesΪһ��Sector
16������Ϊ1��Block
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
*	revised record ��1.
*                  2.
*remarks         :  
BIT:   7    6    5    4    3    2    1    0
       SPR  RV   TB   BP2  BP1  BP0  WEL  BUSY
SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
TB,BP2,BP1,BP0:FLASH����д��������
WEL:дʹ������
BUSY:æ���λ(1,æ;0,����)
Ĭ��:0x00
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
*	revised record ��1.
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
*	revised record ��1.
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
*	revised record ��1.
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
*	revised record ��1.
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
*	input          : 1 pBuffer ��data store
                   2 ReadAddr: address to start reading(24bit)
                   3 NumByteToRead��Bytes to read(���65535)
*	output         : nonce
*	return         : nonce
*	revised record ��1.
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
//��һҳ(0~65535)��д������256���ֽڵ�����
//��ָ����ַ��ʼд�����256�ֽڵ�����
//pBuffer:���ݴ洢����ַ
//WriteAddr:д�����׵�ַ
//NumByteToWrite:��д����ֽ���(���256,�Ҳ��ó�����ҳ��ʣ���ֽ���)
void SPI_W25X_Write_Page(u8 * pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
  u16 i;
	  
  SPI_W25X_Write_Enable();				 //ʹ��Ƭѡ
  W25XCS_L;
  SPI1_ReadWriteByte(W25X_PageProgram);
  SPI1_ReadWriteByte((u8 )((WriteAddr)>>16));
  SPI1_ReadWriteByte((u8 )((WriteAddr)>>8));   
  SPI1_ReadWriteByte((u8 )WriteAddr);   
  
  for(i=0;i<NumByteToWrite;i++)
  {
    SPI1_ReadWriteByte(pBuffer[i]);
  }

  W25XCS_H;	 //�ر�Ƭѡ
  SPI_W25X_Wait_Busy();				 //�ȴ�д�����
} 
//-----------------------------------------------------------------------------
//�޼���дSPI FLASH 
//����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
//�����Զ���ҳ���� 
//��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)
//CHECK OK
void SPI_W25X_Write_NoCheck(u8 * pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 			 		 
  u16 pageremain;
		   
  pageremain=256-WriteAddr%256;
  if(NumByteToWrite<=pageremain)
    pageremain=NumByteToWrite;        //������256�ֽ�
  while(1)
  {	   
    SPI_W25X_Write_Page(pBuffer,WriteAddr,pageremain);
    if(NumByteToWrite==pageremain)
	  break;   //д�����
    else
    {
      pBuffer+=pageremain;
      WriteAddr+=pageremain;	

      NumByteToWrite-=pageremain;	  //��ȥ�Ѿ�д����ֽ�
      if(NumByteToWrite>256)
	    pageremain=256;       
      else 
	    pageremain=NumByteToWrite; 	  //����256���ֽ�
    }
  }
} 
//-----------------------------------------------------------------------------
//дW25X  
//��ָ����ַ��ʼд��ָ�����ȵ�����
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)  		   
u8  W25X_BUF[4096];
void SPI_W25X_Write(u8  *pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 
  u32 secpos;
  u16 secoff;
  u16 secremain;	   
  u16 i;    

  secpos=WriteAddr/4096;      //������ַ
  secoff=WriteAddr%4096;      //�������ڵ�ƫ��
  secremain=4096-secoff;      //����ʣ��ռ�

  if(NumByteToWrite<=secremain)
    secremain=NumByteToWrite; //������4096�ֽ�
  while(1) 
  {	
    SPI_W25X_Read(W25X_BUF,secpos*4096,4096);//��������
    for(i=0;i<secremain;i++)   //У��
    {
      if(W25X_BUF[secoff+i]!=0XFF)
	    break;     //��Ҫ����  	  
    }
    
	if(i<secremain)//��Ҫ����
    {
      SPI_W25X_Erase_Sector(secpos);  //����������
      for(i=0;i<secremain;i++)	       //����
      {
        W25X_BUF[i+secoff]=pBuffer[i];	  
      }
      SPI_W25X_Write_NoCheck(W25X_BUF,secpos*4096,4096); //д���� 

    }
	else 
	  SPI_W25X_Write_NoCheck(pBuffer,WriteAddr,secremain); //�Ѿ�����,ֱ��д��ʣ������
		
    if(NumByteToWrite==secremain)
      break;  //д�����
    else      //д��δ��
    {
      secpos++; //������ַ��1
      secoff=0; //ƫ��λ��Ϊ0 	 

      pBuffer+=secremain;
      WriteAddr+=secremain;
      NumByteToWrite-=secremain;
      
	  if(NumByteToWrite>4096)
        secremain=4096;	           //��һ�����Բ���
      else 
        secremain=NumByteToWrite;  //��һ��������д��
    }	 
  }
}
//-----------------------------------------------------------------------------
//��������оƬ
//��Ƭ����ʱ��:
//W25X16:25s 
//W25X32:40s 
//W25X64:40s 
//�ȴ�ʱ�䳬��...
void SPI_W25X_Erase_Chip(void)   
{                                             
  SPI_W25X_Write_Enable();                  //SET WEL 
  SPI_W25X_Wait_Busy();   
  W25XCS_L;	 //ʹ��Ƭѡ
  SPI1_ReadWriteByte(W25X_ChipErase);        //����Ƭ��������  
  W25XCS_H;	     //�ر�Ƭѡ
  SPI_W25X_Wait_Busy();   				     //�ȴ�оƬ��������
}   
//-----------------------------------------------------------------------------
//����һ������
//Dst_Addr:������ַ 0~511 for w25x16
//����һ��ɽ��������ʱ��:150ms
void SPI_W25X_Erase_Sector(u32 Dst_Addr)   
{   
  Dst_Addr*=4096;
  SPI_W25X_Write_Enable();                  //SET WEL 	 
  SPI_W25X_Wait_Busy();   
  W25XCS_L;	 //ʹ��Ƭѡ
  SPI1_ReadWriteByte(W25X_SectorErase);      //������������ָ�� 
  SPI1_ReadWriteByte((u8 )((Dst_Addr)>>16));  //����24bit��ַ    
  SPI1_ReadWriteByte((u8 )((Dst_Addr)>>8));   
  SPI1_ReadWriteByte((u8 )Dst_Addr);  
  W25XCS_H;	     //�ر�Ƭѡ
  SPI_W25X_Wait_Busy();   				     //�ȴ��������
}  
//-----------------------------------------------------------------------------
//�ȴ�����
void SPI_W25X_Wait_Busy(void)   
{   
  while ((SPI_W25X_ReadSR()&0x01)==0x01);   // �ȴ�BUSYλ���
}  
//-----------------------------------------------------------------------------
//�������ģʽ
void SPI_W25X_PowerDown(void)   
{ 
  W25XCS_L;	 //ʹ��Ƭѡ
  SPI1_ReadWriteByte(W25X_PowerDown);        //���͵�������  
  W25XCS_H;	     //�ر�Ƭѡ
   HAL_Delay(3);            //�ȴ�TPD  
}   
//-----------------------------------------------------------------------------
//����
void SPI_W25X_WAKEUP(void)   
{  
  W25XCS_L;	   //ʹ��Ƭѡ
  SPI1_ReadWriteByte(W25X_ReleasePowerDown);   //  send W25X_PowerDown command 0xAB    
  W25XCS_H;	       //�ر�Ƭѡ
  HAL_Delay(3);                  //�ȴ�TRES1
}   


























