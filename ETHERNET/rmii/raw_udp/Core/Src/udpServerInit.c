/**
  ******************************************************************************
  * @file    LwIP/LwIP_UDP_Echo_Server/Src/udp_echoserver.c
  * @author  MCD Application Team
  * @brief   UDP echo server
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <string.h>
#include <stdio.h>
#include "udpServerInit.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DEST_IP_ADDR0   (uint8_t) 192
#define DEST_IP_ADDR1   (uint8_t) 168
#define DEST_IP_ADDR2   (uint8_t) 1
#define DEST_IP_ADDR3   (uint8_t) 104

#define UDP_SERVER_PORT    8089   /* define the UDP local connection port */
#define UDP_CLIENT_PORT    8089   /* define the UDP remote connection port */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
struct udp_pcb *upcb;
/* Private function prototypes -----------------------------------------------*/
void udp_echoserver_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initialize the server application.
  * @param  None
  * @retval None
  */
void udpServerInit(void)
{
   ip_addr_t DestIPaddr;
   err_t err;
   
   /* Create a new UDP control block  */
   upcb = udp_new();
   
   if (upcb)
   {
     /* Bind the upcb to the UDP_PORT port */
     /* Using IP_ADDR_ANY allow the upcb to be used by any local interface */
      err = udp_bind(upcb, IP_ADDR_ANY, UDP_SERVER_PORT);
      /*assign destination IP address */
      IP4_ADDR( &DestIPaddr, DEST_IP_ADDR0, DEST_IP_ADDR1, DEST_IP_ADDR2, DEST_IP_ADDR3 );
    
    /* configure destination IP address and port */
//      err= udp_connect(upcb, &DestIPaddr, UDP_CLIENT_PORT);
      if(err == ERR_OK)
      {
        /* Set a receive callback for the upcb */
        udp_recv(upcb, udp_echoserver_receive_callback, NULL);
      }
   }
}

/**
  * @brief This function is called when an UDP datagrm has been received on the port UDP_PORT.
  * @param arg user supplied argument (udp_pcb.recv_arg)
  * @param pcb the udp_pcb which received data
  * @param p the packet buffer that was received
  * @param addr the remote IP address from which the packet was received
  * @param port the remote port from which the packet was received
  * @retval None
  */
void udp_echoserver_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
  volatile uint8_t Destip[4];
  
  Destip[0] = (uint8_t)((uint32_t)(addr->addr) >> 24);
  Destip[1] = (uint8_t)((uint32_t)(addr->addr) >> 16);
  Destip[2] = (uint8_t)((uint32_t)(addr->addr) >> 8);
  Destip[3] = (uint8_t)((uint32_t)(addr->addr));
  
  if((Destip[3] != DEST_IP_ADDR0)&&(Destip[2] != DEST_IP_ADDR1)&&(Destip[1] != DEST_IP_ADDR2)&&(Destip[0] != DEST_IP_ADDR3))
  {
     pbuf_free(p); 
  }

  if(port == UDP_CLIENT_PORT)
  {
     pbuf_free(p);
  }
  else
  {
     pbuf_free(p);
  }    
}

/**
  * @brief This function is called when an UDP datagrm has been received on the port UDP_PORT.
  * @param arg user supplied argument (udp_pcb.recv_arg)
  * @param pcb the udp_pcb which received data
  * @param p the packet buffer that was received
  * @param addr the remote IP address from which the packet was received
  * @param port the remote port from which the packet was received
  * @retval None
  */
void udp_echoclient_send(char *send,uint16_t size,uint16_t port)
{
  struct pbuf *p;
  
  /* allocate pbuf from pool*/
  p = pbuf_alloc(PBUF_TRANSPORT,size, PBUF_POOL);
  
  if (p != NULL)
  {
    /* copy data to pbuf */
    pbuf_take(p,send,size);
    
    /* send udp data */
    udp_send(upcb, p); 
    
    /* free pbuf */
    pbuf_free(p);
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
