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
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//UDP 测试代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/8/15
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//*******************************************************************************
//修改信息
//无
////////////////////////////////////////////////////////////////////////////////// 	   
 
//UDP接收数据缓冲区
uint8_t udp_demo_recvbuf[UDP_DEMO_RX_BUFSIZE];	//UDP接收数据缓冲区 
////UDP发送数据内容
//const uint8_t *tcp_demo_sendbuf="Explorer STM32F407 UDP demo send data\r\n";

//UDP 测试全局状态标记变量
//bit7:没有用到
//bit6:0,没有收到数据;1,收到数据了.
//bit5:0,没有连接上;1,连接上了.
//bit4~0:保留
uint8_t udp_demo_flag;

 
//UDP测试
struct udp_pcb *udppcb;
void udp_demo_test(void)
{
 	err_t err;
//	struct udp_pcb *udppcb;  	//定义一个TCP服务器控制块
	ip_addr_t rmtipaddr;  	//远端ip地址	
 	
	udppcb=udp_new();
	if(udppcb)//创建成功
	{ 
		IP4_ADDR( &rmtipaddr, DEST_IP_ADDR0, DEST_IP_ADDR1, DEST_IP_ADDR2, DEST_IP_ADDR3 );
		err=udp_connect(udppcb,&rmtipaddr,UDP_DEMO_PORT);//UDP客户端连接到指定IP地址和端口号的服务器
		if(err==ERR_OK)
		{
			err=udp_bind(udppcb,IP_ADDR_ANY,UDP_DEMO_PORT);//绑定本地IP地址与端口号
			if(err==ERR_OK)	//绑定完成
			{
				udp_recv(udppcb,udp_demo_recv,NULL);//注册接收回调函数 
				udp_demo_flag |= 1<<5;			//标记已经连接上
			}
		}		
	}

} 

//UDP回调函数
void udp_demo_recv(void *arg,struct udp_pcb *upcb,struct pbuf *p, ip_addr_t *addr,uint16_t port)
{
	uint32_t data_len = 0;
	struct pbuf *q;
	if(p!=NULL)	//接收到不为空的数据时
	{
		memset(udp_demo_recvbuf,0,UDP_DEMO_RX_BUFSIZE);  //数据接收缓冲区清零
		for(q=p;q!=NULL;q=q->next)  //遍历完整个pbuf链表
		{
			//判断要拷贝到UDP_DEMO_RX_BUFSIZE中的数据是否大于UDP_DEMO_RX_BUFSIZE的剩余空间，如果大于
			//的话就只拷贝UDP_DEMO_RX_BUFSIZE中剩余长度的数据，否则的话就拷贝所有的数据
			if(q->len > (UDP_DEMO_RX_BUFSIZE-data_len)) memcpy(udp_demo_recvbuf+data_len,q->payload,(UDP_DEMO_RX_BUFSIZE-data_len));//拷贝数据
			else memcpy(udp_demo_recvbuf+data_len,q->payload,q->len);
			data_len += q->len;  	
			if(data_len > UDP_DEMO_RX_BUFSIZE) break; //超出TCP客户端接收数组,跳出	
		}
		upcb->remote_ip=*addr; 				//记录远程主机的IP地址
		upcb->remote_port=port;  			//记录远程主机的端口号
//		lwipdev.remoteip[0]=upcb->remote_ip.addr&0xff; 		//IADDR4
//		lwipdev.remoteip[1]=(upcb->remote_ip.addr>>8)&0xff; //IADDR3
//		lwipdev.remoteip[2]=(upcb->remote_ip.addr>>16)&0xff;//IADDR2
//		lwipdev.remoteip[3]=(upcb->remote_ip.addr>>24)&0xff;//IADDR1 
		udp_demo_flag|=1<<6;	//标记接收到数据了
		pbuf_free(p);//释放内存
	}else
	{
		udp_disconnect(upcb); 
		udp_demo_flag &= ~(1<<5);	//标记连接断开
	} 
} 
//UDP服务器发送数据
void udp_demo_senddata(struct udp_pcb *upcb)
{
	struct pbuf *ptr;
	ptr=pbuf_alloc(PBUF_TRANSPORT,strlen((char*)udp_demo_recvbuf),PBUF_POOL); //申请内存
	if(ptr)
	{
		pbuf_take(ptr,(char*)udp_demo_recvbuf,strlen((char*)udp_demo_recvbuf)); //将tcp_demo_sendbuf中的数据打包进pbuf结构中
		udp_send(upcb,ptr);	//udp发送数据 
		pbuf_free(ptr);//释放内存
	} 
} 
//关闭UDP连接
void udp_demo_connection_close(struct udp_pcb *upcb)
{
	udp_disconnect(upcb); 
	udp_remove(upcb);			//断开UDP连接 
	udp_demo_flag &= ~(1<<5);	//标记连接断开
}

























