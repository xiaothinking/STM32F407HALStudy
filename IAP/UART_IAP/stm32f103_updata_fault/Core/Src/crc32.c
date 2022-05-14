#include "crc32.h"
//#include <stdio.h>
//#include <stdlib.h>
 
//typedef unsigned int  uint32_t;
//typedef unsigned char uint8_t;


static uint32_t table[256];
 
 
//位逆转
static uint32_t bitrev(uint32_t input, int bw)
{
 int i;
 uint32_t var;
 var = 0;
 for(i=0; i<bw; i++)
 {
 if(input & 0x01)
 {
 var |= 1<<(bw - 1 - i);
 }
 input >>= 1;
 }
 return var;
}
 
 
//码表生成
void crc32_init(uint32_t poly)
{
 int i;
 int j;
 uint32_t c;
 
 
 poly = bitrev(poly, 32);
 for(i=0; i<256; i++)
 {
 c = i;
 for (j=0; j<8; j++)
 {
 c = (c & 1) ? (poly ^ (c >> 1)) : (c >> 1);
 }
 table[i] = c;
 }
}
 
 
//计算CRC
uint32_t crc32(uint32_t crc, void* input, uint32_t len)
{
 uint32_t i;
 uint8_t index;
 uint8_t *p;
 p = (uint8_t*)input;
 for(i=0; i<len; i++)
 {
 index = (*p ^ crc);
 crc = (crc >> 8) ^ table[index];
 p++;
 }
 return crc;
}
 
 
// //测试用例
// void main(void)
// {
//  uint32_t crc=0xFFFFFFFF;
//  uint8_t ar_temp[]={"1234567890"};
//  uint8_t i;
//  uint8_t fw_len;
//  uint8_t loop_len;
//  uint8_t loopend_len;

//  fw_len=strlen(ar_temp);
//  loop_len=fw_len/3;
//  loopend_len=fw_len%3;

//  crc32_init(0x4C11DB7);
//  for(i=0;i<loop_len;i++)
//  {
//    crc = crc32(crc, &ar_temp[3*i], 3);
//  }
//  crc = crc32(crc, &ar_temp[3*loop_len], loopend_len);
 
//  printf("CRC32 = 0x%08X\n", crc ^ 0xFFFFFFFF);
//  system("pause");
// }



