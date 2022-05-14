//#include <stdint.h>
#include "crc32.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


uint8_t buffer[1024];
uint8_t send[1024];
uint8_t temp_buff[255]={0x00,0x7F,0x00,0x01,0xA3,0xFF};
uint16_t temp_chk;
uint32_t crc=0xFFFFFFFF;

FILE *fp_src, *fp_dst;
int main(void)
{
	int len = 0;
	int index, i;
	uint8_t bts[2];
	int count = 0;	
	//storage log into flash file
	fp_src = fopen ("src.bin", "rb");
	if (fp_src == NULL){
		printf("error 1\r\n");
		return -1;
	}	
	fp_dst = fopen ("dst.txt", "w+");
	if (fp_dst == NULL){
		printf("error 2\r\n");
		return -1;
	}
	crc32_init(0x4C11DB7);		
	while(1){
		for(i=0;i<128; i++){
			len = fread(bts,1,1, fp_src);
			if(len == 0)
				break;
			sprintf(&buffer[i*3], "%02x ", bts[0]);
			count++;
            temp_buff[i]=bts[0];
		}
       

        /*temp_buff[2]=count+13; //指令长度
        temp_buff[6]= 0xC4;               //固件长度
        temp_buff[7]= 0x06;
		temp_buff[8]= 0x00;
		temp_buff[9]= 0x00;
        temp_buff[10]=count;    //数据长度

		for(i=0;i<temp_buff[10];i++)
		{
          temp_buff[11+i] = buffer[i*3]; 
		}
         
		temp_chk = modbus_crc16(temp_buff, temp_buff[2] - 2, NULL);
        temp_buff[temp_buff[2]-2] = temp_chk % 0x0100;
        temp_buff[temp_buff[2]-1] = temp_chk / 0x0100;

        for(i=0;i<temp_buff[2];i++)
		{
			sprintf(&send[i*3], "%02x ", temp_buff[i]);
		}*/

		if(count != 0){	
			fwrite(buffer, 1, count*3, fp_dst);
			fwrite("\n",1,2,fp_dst);
			crc = crc32(crc, temp_buff, count);
			count = 0;
		}else{
			break;
		}
	
	}
	fclose(fp_src);
	fclose(fp_dst);
	crc^=0xFFFFFFFF;
	printf("crc32=%08x\n",crc);
	printf("done.\n");
	

	return 0;
	
}

