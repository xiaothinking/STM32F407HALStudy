#include "protocol.h"
#include <stdio.h>
#include <string.h>
#include "stmflash.h"
#include "iap.h"
#include "crc32.h"

uint8_t g_uart1_cmd_addr;
uint8_t g_uart1_cmd_flag;
uint8_t g_uart1_cmd_num;
uint8_t g_uart1_cmd_ack;
uint8_t g_uart1_cmd;
uint8_t g_cmd_addr=1;

uint8_t ar_board_version[4] = {0x86, 0x01, 0x02, 0x06}; /**/
uint8_t ar_firmware_crc32[4];
uint8_t ar_facory_id[4] = {0x04, 0x00, 0x00, 0x00};
uint32_t g_download_addr = UPDATA_BACKA_ADDRESS;

uint8_t ar_rec_buff[128];
uint8_t g_erase_cnt = 0;
uint8_t g_erase_cnt_1 = 0;
uint8_t g_erase_cnt_2 = 0;
uint16_t g_rec_data_flg = 0;
uint16_t g_rec_fulldata_len = 0;
uint16_t g_rec_fulldata_cnt = 0;
uint16_t g_rec_data_len = 0;
uint16_t g_rec_data_cnt = 0;
uint16_t g_rec_data_cnt_1 = 0;
uint16_t g_rec_data_cnt_2 = 0;
uint32_t g_rec_addr = UPDATA_BACKA_ADDRESS;

uint16_t modbus_crc16(uint8_t const *p_data, uint32_t size, uint16_t const *p_crc)
{
    uint16_t crc16 = (p_crc == NULL) ? 0xFFFF : *p_crc;

    if (p_data == NULL)
    {
        return crc16;
    }

    for (uint32_t i = 0; i < size; i++)
    {
        crc16 ^= p_data[i];

        for (uint32_t j = 0; j < 8; j++)
        {
            if (crc16 & 0x01)
            {
                crc16 = crc16 >> 1;
                crc16 ^= 0xA001;
            }
            else
            {
                crc16 = crc16 >> 1;
            }
        }
    }

    return crc16;
}

uint8_t protocol_check(uint8_t * pbuff,uint8_t buff_len)
{
	  uint8_t  m_data_len = 0;
    uint16_t m_bcc = 0;
    uint8_t  status = 1;
    g_uart1_cmd_addr = pbuff[0];
    g_uart1_cmd_flag = pbuff[1];
    m_data_len = pbuff[2];
    g_uart1_cmd_num = pbuff[3];
	  g_uart1_cmd = pbuff[4];
    g_uart1_cmd_ack = pbuff[5];

    if (g_uart1_cmd_addr == g_cmd_addr)
    {
        if (g_uart1_cmd_flag == 0x7F)
        {
            if (g_uart1_cmd_ack == 0xFF)
            {
                m_bcc = modbus_crc16(pbuff, (m_data_len - 2), NULL);
                if (m_bcc == pbuff[m_data_len - 1] * 0x0100 + pbuff[m_data_len - 2])
                {
                    status = 1;
                }
                else
                {
                    status = 2;        
                }
            }
            else
            {
                status = 3;
            }
        }
        else
        {
            status = 4;
        }
    }
    else
    {
        status = 5;
    }
    return status;
}

void uart3_send_chars(uint8_t *pst, uint16_t data_len)
{
    if (pst != NULL)
    {
        if (data_len != 0)
        {
//          RS485_EN1; /*RS485 can send*/
//			    HAL_Delay(5);
//					HAL_UART_Transmit_DMA(&huart1, pst, data_len);
            HAL_UART_Transmit(&huart1, pst, data_len,0xFFFF);
//          RS485_DIS1; /*RS485 can receive*/
//			    HAL_Delay(5);
        }
        else
        {
        }
    }
    else
    {
    }
}

void get_firmware_updata_status(uint8_t cmd_len,uint8_t* pudata,uint8_t number)
{
    uint8_t  m_resp_flag ;
    uint16_t temp_chk = 0x00;
    uint8_t  temp_send_data[12] = {0};

    uint8_t  ar_facory_id_temp[4];
    uint8_t  ar_hardware_version_temp[4];
    uint8_t  ar_software_version_temp[2]; 
    uint32_t m_firmware_len;
    uint8_t  m_hardware_version_buff[2];
    
    m_hardware_version_buff[0] = (ar_board_version[0]&0x03);
    m_hardware_version_buff[1] = ar_board_version[1];

    memcpy(ar_facory_id_temp,&pudata[1],4);  
    memcpy(ar_hardware_version_temp,&pudata[5],2);
    memcpy(ar_software_version_temp,&pudata[7],2);
    m_firmware_len = (uint32_t)pudata[9]+(((uint32_t)pudata[10])<<8)+(((uint32_t)pudata[11])<<16)+(((uint32_t)pudata[12])<<24);
    memcpy(ar_firmware_crc32,&pudata[13],4);

    if(memcmp(ar_facory_id,ar_facory_id_temp,4)!=0)
      {
          m_resp_flag=0x05;
      }
    else if(memcmp(m_hardware_version_buff,ar_hardware_version_temp,2)!=0)
      {
          m_resp_flag=0x04;   
      }
     else if(memcmp(&ar_board_version[2],ar_software_version_temp,2)==0)
      {
          m_resp_flag=0x02;   
      }
      else if(m_firmware_len>0x20000)
      {
          m_resp_flag=0x03; 
      }
      else
      {
          m_resp_flag = 0x00;
      }


    temp_send_data[0] = g_uart1_cmd_addr;
    temp_send_data[1] = PROTOCOL_SIGN;
    temp_send_data[2] = 0x09;
    temp_send_data[3] = number;
    temp_send_data[4] = GET_FWUPDATA_STAUS;
    temp_send_data[5] = m_resp_flag;
    temp_send_data[6] = cmd_len;
    temp_chk = modbus_crc16(temp_send_data, temp_send_data[2] - 2, NULL);
    temp_send_data[7] = temp_chk % 0x0100;
    temp_send_data[8] = temp_chk / 0x0100;

    uart3_send_chars(temp_send_data, temp_send_data[2]);
     
     if((m_resp_flag==0)&&(pudata[0]==0x01))
     {
        
		   STMFLASH_EraseSec(UPDATA_BACKA_ADDRESS,UPDATA_BACKB_ADDRESS);

     }
}

void transfer_firmware(uint8_t* pudata,uint8_t number)
{  
    uint8_t  m_resp_flag ;
    uint16_t temp_chk = 0x00;
    uint8_t  temp_send_data[12] = {0};
    uint32_t m_firmware_len;
    uint32_t m_data_offset;
    uint32_t m_next_data_offset;
    uint8_t m_data_len;
    uint8_t ar_upsign_buff[12];
    uint8_t ar_updata_buff[255];
    uint32_t crc=0xFFFFFFFF;
    uint32_t i;
    uint8_t j;
    uint32_t loop_len;
    uint32_t loopend_len;
    uint8_t ar_crc32_temp[4];

    m_firmware_len = (uint32_t)pudata[0]+(((uint32_t)pudata[1])<<8)+(((uint32_t)pudata[2])<<16)+(((uint32_t)pudata[3])<<24);
    m_data_offset  = (uint32_t)pudata[4]+(((uint32_t)pudata[5])<<8)+(((uint32_t)pudata[6])<<16)+(((uint32_t)pudata[7])<<24);
    m_data_len = pudata[8];

    g_download_addr = UPDATA_BACKA_ADDRESS+m_data_offset;

    
    g_rec_fulldata_cnt = m_firmware_len / 128;
    g_rec_fulldata_len = m_firmware_len % 128;
    g_rec_data_cnt = (m_data_offset + m_data_len) / 128;
    g_rec_data_len = (m_data_offset + m_data_len) % 128;
    g_rec_data_flg = m_data_offset / 128;
    g_rec_data_cnt_1 = g_rec_data_cnt_2;
    g_rec_data_cnt_2 = g_rec_data_cnt;
    memcpy(&ar_rec_buff[m_data_offset - g_rec_data_flg * 128], &pudata[9], m_data_len);//???????
		if(g_rec_data_cnt_2 - g_rec_data_cnt_1)
		{
			STMFLASH_Write_HalfWord(g_rec_addr + g_rec_data_flg * 128, (uint16_t*)ar_rec_buff,64);
			memcpy(ar_rec_buff, &pudata[9 + m_data_len - g_rec_data_len], g_rec_data_len);
		}

    if(m_firmware_len == g_download_addr+m_data_len - UPDATA_BACKA_ADDRESS)	//??????,?????????????? 
	  {	
			  if(g_rec_fulldata_len != 0)/*????????????*/
				{
					STMFLASH_Write_HalfWord(g_rec_addr + g_rec_data_cnt * 128,(uint16_t*)ar_rec_buff, (g_rec_fulldata_len/2+g_rec_fulldata_len%2));
				}
        m_resp_flag = 0xA1;										 																					 
	  }	
    else /*if(m_data_len ==64)*/
    {
         m_resp_flag = 0x00;	
    }																																					 
	   
    temp_send_data[0] = g_uart1_cmd_addr;
    temp_send_data[1] = PROTOCOL_SIGN;
    temp_send_data[2] = 0x0C;
    temp_send_data[3] = number;
    temp_send_data[4] = SEND_FWUPDATA;
    temp_send_data[5] = m_resp_flag;
    m_next_data_offset = m_data_offset+m_data_len;
    temp_send_data[6] = (uint8_t)(0x000000FF & m_next_data_offset);
    temp_send_data[7] = (uint8_t)((0x0000FF00 & m_next_data_offset)>>8);
    temp_send_data[8] = (uint8_t)((0x00FF0000 & m_next_data_offset)>>16);
    temp_send_data[9] = (uint8_t)((0xFF000000 & m_next_data_offset)>>24);
    temp_chk = modbus_crc16(temp_send_data, temp_send_data[2] - 2, NULL);
    temp_send_data[10] = temp_chk % 0x0100;
    temp_send_data[11] = temp_chk / 0x0100;

    uart3_send_chars(temp_send_data, temp_send_data[2]);
     
     if(m_resp_flag==0xA1)
     {

        loop_len=m_firmware_len/255;
        loopend_len=m_firmware_len%255;

        crc32_init(0x4C11DB7);
        for(i=0;i<loop_len;i++)
        {
          for(j=0;j<255;j++)
          {   
              ar_updata_buff[j]=STMFLASH_ReadByte(UPDATA_BACKA_ADDRESS+255*i+j);            
          }
           
           crc = crc32(crc, ar_updata_buff, 255);
        }
        for(j=0;j<loopend_len;j++)
        {
           ar_updata_buff[j]=STMFLASH_ReadByte(UPDATA_BACKA_ADDRESS+255*loop_len+j);
        }
        
        crc = crc32(crc, ar_updata_buff, loopend_len);
        crc ^= 0xFFFFFFFF;
        ar_crc32_temp[0] = (uint8_t)(0x000000FF & crc);
        ar_crc32_temp[1] = (uint8_t)((0x0000FF00 & crc)>>8);
        ar_crc32_temp[2] = (uint8_t)((0x00FF0000 & crc)>>16);
        ar_crc32_temp[3] = (uint8_t)((0xFF000000 & crc)>>24); 
        

       if(memcmp(ar_firmware_crc32,ar_crc32_temp,4)==0)  /*??????CRC32????*/
         {
            //printf("crc32 check ok\n");
           
		       STMFLASH_EraseSec(7,7);//????7
		       memset(ar_upsign_buff,0x55,4);//???????0x55555555
           memcpy(&ar_upsign_buff[4],(uint8_t*)&crc,4);
		       memcpy(&ar_upsign_buff[8],(uint8_t*)&m_firmware_len,4);
					 STMFLASH_Write_HalfWord(UPDATA_BACKC_ADDRESS,(uint16_t*)ar_upsign_buff,6);
		       NVIC_SystemReset();//?????,???boot?????		    
         }
     }

}

void uart1_handle(uint8_t * pbuff,uint8_t buff_len)
{
	if (usart1_rx_flag == DMA_UART1_RX_FINISH)
		{
			usart1_rx_flag = DMA_UART1_RX_NO_FINISH;
			if(protocol_check(pbuff,buff_len) == 1)
				{
					switch (g_uart1_cmd)
						{
						  case GET_FWUPDATA_STAUS:   
								 get_firmware_updata_status(pbuff[2],&pbuff[6],pbuff[3]);
                 break;
							
							case SEND_FWUPDATA:   
								transfer_firmware(&pbuff[6],pbuff[3]);
                 break;
							
							default:
								
								break;
						}
				}
			
    }
}

