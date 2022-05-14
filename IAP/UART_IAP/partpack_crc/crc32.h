#ifndef __CRC32_H
#define __CRC32_H

//#include "main.h"

typedef unsigned int  uint32_t;
typedef unsigned short int  uint16_t;
typedef unsigned char uint8_t;

void crc32_init(uint32_t poly);
uint32_t crc32(uint32_t crc, void* input, uint32_t len);
uint16_t modbus_crc16(uint8_t const *p_data, uint32_t size, uint16_t const *p_crc);

#endif

