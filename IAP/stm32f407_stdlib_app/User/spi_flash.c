/* Includes ------------------------------------------------------------------*/
#include "spi_flash.h"

u32 FlashID = 0;

/*******************************************************************************
* Description    : 初始化
*******************************************************************************/
void SPI_FLASH_Configuration(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  RCC_APB1PeriphClockCmd(SPI_FLASH_SPI_CLK, ENABLE);
  SPI_FLASH_IO_Init();  
  
  SPI_FLASH_CS_HIGH();
 
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;

  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI_FLASH_SPI, &SPI_InitStructure);
  
  SPI_Cmd(SPI_FLASH_SPI, ENABLE);
}

/*******************************************************************************
* Description    : 查询ID自检
*******************************************************************************/
u8 SPI_FLASH_SelfCheck(void)
{

  FlashID = SPI_FLASH_ReadID();
  if (FlashID == SPI_FLASH_M25P16_ID)
  {
    return 1;
  }
  else
  {
    return 0;
  }

}

/*******************************************************************************
* Description    : 扇区擦除
*******************************************************************************/
void SPI_FLASH_EraseSector(uint32_t SectorAddr)
{
  SPI_FLASH_WriteEnable();
  SPI_FLASH_CS_LOW();
  SPI_FLASH_SendByte(SPI_FLASH_CMD_SE);
  SPI_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
  SPI_FLASH_SendByte((SectorAddr & 0xFF00) >> 8);
  SPI_FLASH_SendByte(SectorAddr & 0xFF);
  SPI_FLASH_CS_HIGH();
  SPI_FLASH_WaitForWriteEnd();
}
/*******************************************************************************
* Description    : 整片擦除
*******************************************************************************/
void SPI_FLASH_EraseBulk(void)
{
  SPI_FLASH_WriteEnable();
  SPI_FLASH_CS_LOW();
  SPI_FLASH_SendByte(SPI_FLASH_CMD_BE);
  SPI_FLASH_CS_HIGH();
  SPI_FLASH_WaitForWriteEnd();
}
/*******************************************************************************
* Description    : 页写入
*******************************************************************************/
void SPI_FLASH_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
  SPI_FLASH_WriteEnable();
  SPI_FLASH_CS_LOW();
  SPI_FLASH_SendByte(SPI_FLASH_CMD_WRITE);
  SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
  SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
  SPI_FLASH_SendByte(WriteAddr & 0xFF);
  while (NumByteToWrite--)
  {
    SPI_FLASH_SendByte(*pBuffer);
    pBuffer++;
  }
  SPI_FLASH_CS_HIGH();
  SPI_FLASH_WaitForWriteEnd();
}

/*******************************************************************************
* Description    : 不限长数组
*******************************************************************************/
void SPI_FLASH_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

  Addr = WriteAddr % SPI_FLASH_SPI_PAGESIZE;													
  count = SPI_FLASH_SPI_PAGESIZE - Addr;
  NumOfPage =  NumByteToWrite / SPI_FLASH_SPI_PAGESIZE;
  NumOfSingle = NumByteToWrite % SPI_FLASH_SPI_PAGESIZE;

  if (Addr == 0) 
  {
    if (NumOfPage == 0) 
    {
      SPI_FLASH_WritePage(pBuffer, WriteAddr, NumByteToWrite);
    }
    else 
    {
      while (NumOfPage--)
      {
        SPI_FLASH_WritePage(pBuffer, WriteAddr, SPI_FLASH_SPI_PAGESIZE);
        WriteAddr +=  SPI_FLASH_SPI_PAGESIZE;
        pBuffer += SPI_FLASH_SPI_PAGESIZE;
      }

      SPI_FLASH_WritePage(pBuffer, WriteAddr, NumOfSingle);
    }
  }
  else 
  {
    if (NumOfPage == 0) 
    {
      if (NumOfSingle > count) 
      {
        temp = NumOfSingle - count;

        SPI_FLASH_WritePage(pBuffer, WriteAddr, count);
        WriteAddr +=  count;
        pBuffer += count;

        SPI_FLASH_WritePage(pBuffer, WriteAddr, temp);
      }
      else
      {
        SPI_FLASH_WritePage(pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else 
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / SPI_FLASH_SPI_PAGESIZE;
      NumOfSingle = NumByteToWrite % SPI_FLASH_SPI_PAGESIZE;

      SPI_FLASH_WritePage(pBuffer, WriteAddr, count);
      WriteAddr +=  count;
      pBuffer += count;

      while (NumOfPage--)
      {
        SPI_FLASH_WritePage(pBuffer, WriteAddr, SPI_FLASH_SPI_PAGESIZE);
        WriteAddr +=  SPI_FLASH_SPI_PAGESIZE;
        pBuffer += SPI_FLASH_SPI_PAGESIZE;
      }

      if (NumOfSingle != 0)
      {
        SPI_FLASH_WritePage(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
}

/*******************************************************************************
* Description    : 读不限长数组
*******************************************************************************/
void SPI_FLASH_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
  SPI_FLASH_CS_LOW();
  SPI_FLASH_SendByte(SPI_FLASH_CMD_READ);
  SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  SPI_FLASH_SendByte(ReadAddr & 0xFF);
  while (NumByteToRead--) 
  {
    *pBuffer = SPI_FLASH_SendByte(SPI_FLASH_DUMMY_BYTE);
    pBuffer++;
  }
  SPI_FLASH_CS_HIGH();
}
/*******************************************************************************
* Description    : 读ID
*******************************************************************************/
uint32_t SPI_FLASH_ReadID(void)
{
  uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;
  SPI_FLASH_CS_LOW();
  SPI_FLASH_SendByte(0x9F);
  Temp0 = SPI_FLASH_SendByte(SPI_FLASH_DUMMY_BYTE);
  Temp1 = SPI_FLASH_SendByte(SPI_FLASH_DUMMY_BYTE);
  Temp2 = SPI_FLASH_SendByte(SPI_FLASH_DUMMY_BYTE);
  SPI_FLASH_CS_HIGH();
  Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;
  return Temp;
}
/*******************************************************************************
* Description    : 写地址
*******************************************************************************/
void SPI_FLASH_StartReadSequence(uint32_t ReadAddr)
{
  SPI_FLASH_CS_LOW();
  SPI_FLASH_SendByte(SPI_FLASH_CMD_READ);
  SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  SPI_FLASH_SendByte(ReadAddr & 0xFF);
	
}
/*******************************************************************************
* Description    : 读字节
*******************************************************************************/
uint8_t SPI_FLASH_ReadByte(void)
{
  return (SPI_FLASH_SendByte(SPI_FLASH_DUMMY_BYTE));
}

/*******************************************************************************
* Description    : 写字节
*******************************************************************************/
uint8_t SPI_FLASH_SendByte(uint8_t byte)
{
  while (SPI_I2S_GetFlagStatus(SPI_FLASH_SPI, SPI_I2S_FLAG_TXE) == RESET);
  SPI_I2S_SendData(SPI_FLASH_SPI, byte);
  while (SPI_I2S_GetFlagStatus(SPI_FLASH_SPI, SPI_I2S_FLAG_RXNE) == RESET);
	return SPI_I2S_ReceiveData(SPI_FLASH_SPI);
	
}
/*******************************************************************************
* Description    : 写半字
*******************************************************************************/
uint16_t SPI_FLASH_SendHalfWord(uint16_t HalfWord)
{
  while (SPI_I2S_GetFlagStatus(SPI_FLASH_SPI, SPI_I2S_FLAG_TXE) == RESET);
  SPI_I2S_SendData(SPI_FLASH_SPI, HalfWord);
  while (SPI_I2S_GetFlagStatus(SPI_FLASH_SPI, SPI_I2S_FLAG_RXNE) == RESET);
  return SPI_I2S_ReceiveData(SPI_FLASH_SPI);
}
/*******************************************************************************
* Description    : 写使能
*******************************************************************************/
void SPI_FLASH_WriteEnable(void)
{
  SPI_FLASH_CS_LOW();
  SPI_FLASH_SendByte(SPI_FLASH_CMD_WREN);
  SPI_FLASH_CS_HIGH();
}
/*******************************************************************************
* Description    : 等待写结束
*******************************************************************************/
void SPI_FLASH_WaitForWriteEnd(void)
{
  uint8_t flashstatus = 0;
  SPI_FLASH_CS_LOW();
  SPI_FLASH_SendByte(SPI_FLASH_CMD_RDSR);
  do
  {
    flashstatus = SPI_FLASH_SendByte(SPI_FLASH_DUMMY_BYTE);
  }
  while ((flashstatus & SPI_FLASH_WIP_FLAG) == SET); 
  SPI_FLASH_CS_HIGH();
}
/*******************************************************************************
* Description    : IO初始化
*******************************************************************************/
void SPI_FLASH_IO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
 
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;      
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /*!< Configure SPI_FLASH  CS pin in output pushpull mode ********************/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
