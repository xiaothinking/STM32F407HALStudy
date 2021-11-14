#ifndef __UDP_DEMO_H
#define __UDP_DEMO_H
#include "stm32f4xx.h"
//#include "lwip_comm.h"
#include "main.h"
#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"
#include "lwip/ip_addr.h"
	   
#define DEST_IP_ADDR0   (uint8_t) 192
#define DEST_IP_ADDR1   (uint8_t) 168
#define DEST_IP_ADDR2   (uint8_t) 1
#define DEST_IP_ADDR3   (uint8_t) 104
 
#define UDP_DEMO_RX_BUFSIZE		2000	//定义udp最大接收数据长度 
#define UDP_DEMO_PORT			8089	//定义udp连接的端口 

 
void udp_demo_test(void);
void udp_demo_recv(void *arg,struct udp_pcb *upcb,struct pbuf *p, ip_addr_t *addr,uint16_t port);
void udp_demo_senddata(struct udp_pcb *upcb);
void udp_demo_connection_close(struct udp_pcb *upcb);

#endif

