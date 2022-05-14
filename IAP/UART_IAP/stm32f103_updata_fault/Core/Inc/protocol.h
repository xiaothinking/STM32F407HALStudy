#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include "usart.h"

#define PROTOCOL_SIGN       0x7F
#define GET_FWUPDATA_STAUS  0xA2   /*????????*/
#define SEND_FWUPDATA       0xA3   /*????????*/

void uart1_handle(uint8_t * pbuff,uint8_t buff_len);



#endif /* __PROTOCOL_H__ */

