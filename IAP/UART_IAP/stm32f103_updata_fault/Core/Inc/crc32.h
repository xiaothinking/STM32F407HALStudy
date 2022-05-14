#ifndef __CRC32_H
#define __CRC32_H

#include "main.h"

void crc32_init(uint32_t poly);
uint32_t crc32(uint32_t crc, void* input, uint32_t len);

#endif

