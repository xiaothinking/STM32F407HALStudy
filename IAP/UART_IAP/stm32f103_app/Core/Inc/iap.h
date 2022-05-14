#ifndef __IAP_H
#define __IAP_H
#include "stm32f1xx_hal.h"

typedef  void (*iapfun)(void);				//定义一个函数类型的参数.

void iap_load_app(uint32_t appxaddr);
void copy_dataa_to_app_addr(uint32_t datalen);
void copy_app_to_datab_addr(void);
void copy_datab_to_app_addr(void);

#endif

