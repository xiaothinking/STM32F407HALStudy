/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPI_FLASH_H
#define __SPI_FLASH_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define  FLASH_WRITE_ADDRESS      0x10000
#define  FLASH_READ_ADDRESS       FLASH_WRITE_ADDRESS
#define  FLASH_SECTOR_TO_ERASE    FLASH_WRITE_ADDRESS




/* M25P SPI Flash supported commands */  
#define SPI_FLASH_CMD_WRITE          0x02  /* Write to Memory instruction */
#define SPI_FLASH_CMD_WRSR           0x01  /* Write Status Register instruction */
#define SPI_FLASH_CMD_WREN           0x06  /* Write enable instruction */
#define SPI_FLASH_CMD_READ           0x03  /* Read from Memory instruction */
#define SPI_FLASH_CMD_RDSR           0x05  /* Read Status Register instruction  */
#define SPI_FLASH_CMD_RDID           0x9F  /* Read identification */
#define SPI_FLASH_CMD_SE             0xD8  /* Sector Erase instruction */
#define SPI_FLASH_CMD_BE             0xC7  /* Bulk Erase instruction */

#define SPI_FLASH_WIP_FLAG           0x01  /* Write In Progress (WIP) flag */

#define SPI_FLASH_DUMMY_BYTE         0xA5
#define SPI_FLASH_SPI_PAGESIZE       0x100

#define SPI_FLASH_M25P128_ID         0x202018
#define SPI_FLASH_M25P64_ID          0x202017
#define SPI_FLASH_M25P16_ID          0x202015 
 
 
#define SPI_FLASH_SPI                SPI2
#define SPI_FLASH_SPI_CLK            RCC_APB1Periph_SPI2
 
 
 
/* Exported macro ------------------------------------------------------------*/
/* Select SPI_FLASH: Chip Select pin*/
#define SPI_FLASH_CS_LOW()       GPIO_ResetBits(GPIOB, GPIO_Pin_12)
#define SPI_FLASH_CS_HIGH()      GPIO_SetBits(GPIOB, GPIO_Pin_12)   

/* Exported functions ------------------------------------------------------- */

/* High layer functions  */
void SPI_FLASH_Configuration(void);
u8 SPI_FLASH_SelfCheck(void);

void SPI_FLASH_EraseSector(uint32_t SectorAddr);
void SPI_FLASH_EraseBulk(void);
void SPI_FLASH_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void SPI_FLASH_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void SPI_FLASH_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
 u32 SPI_FLASH_ReadID(void);
void SPI_FLASH_StartReadSequence(uint32_t ReadAddr);

/* Low layer functions */
  u8 SPI_FLASH_ReadByte(void);
  u8 SPI_FLASH_SendByte(uint8_t byte);
 u16 SPI_FLASH_SendHalfWord(uint16_t HalfWord);
void SPI_FLASH_WriteEnable(void);
void SPI_FLASH_WaitForWriteEnd(void);
void SPI_FLASH_IO_Init(void); 


#endif /* __SPI_FLASH_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
