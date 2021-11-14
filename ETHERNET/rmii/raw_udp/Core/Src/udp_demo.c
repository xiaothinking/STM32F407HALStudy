#include "udp_demo.h" 
//#include "delay.h"
//#include "uart.h"
//#include "main.h"
//#include "key.h"
//#include "lcd.h"
//#include "malloc.h"
#include "stdio.h"
#include "string.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//UDP ���Դ���	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/8/15
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//*******************************************************************************
//�޸���Ϣ
//��
////////////////////////////////////////////////////////////////////////////////// 	   
 
//UDP�������ݻ�����
uint8_t udp_demo_recvbuf[UDP_DEMO_RX_BUFSIZE];	//UDP�������ݻ����� 
////UDP������������
//const uint8_t *tcp_demo_sendbuf="Explorer STM32F407 UDP demo send data\r\n";

//UDP ����ȫ��״̬��Ǳ���
//bit7:û���õ�
//bit6:0,û���յ�����;1,�յ�������.
//bit5:0,û��������;1,��������.
//bit4~0:����
uint8_t udp_demo_flag;

 
//UDP����
struct udp_pcb *udppcb;
void udp_demo_test(void)
{
 	err_t err;
//	struct udp_pcb *udppcb;  	//����һ��TCP���������ƿ�
	ip_addr_t rmtipaddr;  	//Զ��ip��ַ	
 	
	udppcb=udp_new();
	if(udppcb)//�����ɹ�
	{ 
		IP4_ADDR( &rmtipaddr, DEST_IP_ADDR0, DEST_IP_ADDR1, DEST_IP_ADDR2, DEST_IP_ADDR3 );
		err=udp_connect(udppcb,&rmtipaddr,UDP_DEMO_PORT);//UDP�ͻ������ӵ�ָ��IP��ַ�Ͷ˿ںŵķ�����
		if(err==ERR_OK)
		{
			err=udp_bind(udppcb,IP_ADDR_ANY,UDP_DEMO_PORT);//�󶨱���IP��ַ��˿ں�
			if(err==ERR_OK)	//�����
			{
				udp_recv(udppcb,udp_demo_recv,NULL);//ע����ջص����� 
				udp_demo_flag |= 1<<5;			//����Ѿ�������
			}
		}		
	}

} 

//UDP�ص�����
void udp_demo_recv(void *arg,struct udp_pcb *upcb,struct pbuf *p, ip_addr_t *addr,uint16_t port)
{
	uint32_t data_len = 0;
	struct pbuf *q;
	if(p!=NULL)	//���յ���Ϊ�յ�����ʱ
	{
		memset(udp_demo_recvbuf,0,UDP_DEMO_RX_BUFSIZE);  //���ݽ��ջ���������
		for(q=p;q!=NULL;q=q->next)  //����������pbuf����
		{
			//�ж�Ҫ������UDP_DEMO_RX_BUFSIZE�е������Ƿ����UDP_DEMO_RX_BUFSIZE��ʣ��ռ䣬�������
			//�Ļ���ֻ����UDP_DEMO_RX_BUFSIZE��ʣ�೤�ȵ����ݣ�����Ļ��Ϳ������е�����
			if(q->len > (UDP_DEMO_RX_BUFSIZE-data_len)) memcpy(udp_demo_recvbuf+data_len,q->payload,(UDP_DEMO_RX_BUFSIZE-data_len));//��������
			else memcpy(udp_demo_recvbuf+data_len,q->payload,q->len);
			data_len += q->len;  	
			if(data_len > UDP_DEMO_RX_BUFSIZE) break; //����TCP�ͻ��˽�������,����	
		}
		upcb->remote_ip=*addr; 				//��¼Զ��������IP��ַ
		upcb->remote_port=port;  			//��¼Զ�������Ķ˿ں�
//		lwipdev.remoteip[0]=upcb->remote_ip.addr&0xff; 		//IADDR4
//		lwipdev.remoteip[1]=(upcb->remote_ip.addr>>8)&0xff; //IADDR3
//		lwipdev.remoteip[2]=(upcb->remote_ip.addr>>16)&0xff;//IADDR2
//		lwipdev.remoteip[3]=(upcb->remote_ip.addr>>24)&0xff;//IADDR1 
		udp_demo_flag|=1<<6;	//��ǽ��յ�������
		pbuf_free(p);//�ͷ��ڴ�
	}else
	{
		udp_disconnect(upcb); 
		udp_demo_flag &= ~(1<<5);	//������ӶϿ�
	} 
} 
//UDP��������������
void udp_demo_senddata(struct udp_pcb *upcb)
{
	struct pbuf *ptr;
	ptr=pbuf_alloc(PBUF_TRANSPORT,strlen((char*)udp_demo_recvbuf),PBUF_POOL); //�����ڴ�
	if(ptr)
	{
		pbuf_take(ptr,(char*)udp_demo_recvbuf,strlen((char*)udp_demo_recvbuf)); //��tcp_demo_sendbuf�е����ݴ����pbuf�ṹ��
		udp_send(upcb,ptr);	//udp�������� 
		pbuf_free(ptr);//�ͷ��ڴ�
	} 
} 
//�ر�UDP����
void udp_demo_connection_close(struct udp_pcb *upcb)
{
	udp_disconnect(upcb); 
	udp_remove(upcb);			//�Ͽ�UDP���� 
	udp_demo_flag &= ~(1<<5);	//������ӶϿ�
}

























