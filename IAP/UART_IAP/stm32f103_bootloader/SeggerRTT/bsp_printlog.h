#ifndef BSP_PRINTLOG_H
#define BSP_PRINTLOG_H

#include "SEGGER_RTT.h"
#include "SEGGER_RTT_Conf.h"

#define BUFFER_INDEX 0

void print_log(const char * sFormat, ...);
void segger_rtt_init(char * str);

#endif

