//*****************************************************************************
//  Copyright (C) 2015 rastyle。com
//	File name: app_handle.c
//	Author:	whl
//	Description: create rastyle app handle.c file
//	Others:
//	History: 2015-7-29 create by whl
//*****************************************************************************

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <sys/param.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <net/if_arp.h>
#ifdef SOLARIS
#include <sys/sockio.h>
#endif
#define MAXINTERFACES 16 /* 最大接口数 */




#include "acs_server/acs_server.h"
#include "hal/zigbee/sensor.h"


#include "message.h"
#include "common_utils.h"


static char broadcast_msg[] = "2514.325,1514.564,";

#define MCAST_INTERVAL    3

static char devid[] = "abc124";


static void acs_get_ipaddr_and_bcastaddr(char * ipaddr ,char * bcastaddr)
{
	  register int fd, intrface, retn = 0;
	  struct ifreq buf[MAXINTERFACES];
	  struct arpreq arp;
	  struct ifconf ifc;
	  if ((fd = socket (AF_INET, SOCK_DGRAM, 0)) >= 0)
	  {
		  ifc.ifc_len = sizeof buf;
		  ifc.ifc_buf = (caddr_t) buf;
		  if (!ioctl (fd, SIOCGIFCONF, (char *) &ifc))
		  {
			  //获取接口数量信息
			  intrface = ifc.ifc_len / sizeof (struct ifreq);
			  //根据借口信息循环获取设备IP和MAC地址
			  while ( (intrface--) > 0)
			  {
					//获取设备名称
					//printf ("net device %s\n", buf[intrface].ifr_name);
					if(strcmp(buf[intrface].ifr_name,"eth0") == 0)
					{
						//获取当前网卡的IP地址
						if (!(ioctl (fd, SIOCGIFADDR, (char *) &buf[intrface])))
						{
						   printf("===IP address is:");
						   puts((char *)inet_ntoa(((struct sockaddr_in*)(&buf[intrface].ifr_addr))->sin_addr));
						   sprintf(ipaddr,"%s",(char *)inet_ntoa(((struct sockaddr_in*)(&buf[intrface].ifr_addr))->sin_addr));
						}
						//广播地址
						if (! (ioctl(fd, SIOCGIFBRDADDR, (char *) &buf[intrface])))
						{
						   printf("===Broadcast Address:%s\n",(char*)inet_ntoa(((struct sockaddr_in*) (&buf[intrface].ifr_addr))->sin_addr));
						   sprintf(bcastaddr,"%s",(char*)inet_ntoa(((struct sockaddr_in*) (&buf[intrface].ifr_addr))->sin_addr));
						}

					}
			  } //while
		  }
		  close (fd);
	  }
}



void *acs_udp_broadcast_thread(void *args)
{
	struct ifaddrs *addrs = NULL ,*tmp = NULL;
    //s1:setup udp socket
	int sockdf;
	struct sockaddr_in mcast_addr;
	int on=1;
    int n ;
    time_t now;
    struct tm *tm_now;
    char datetime[200];
    char udp_msg[1024] = {0};
    char tmp_msg[100] = {0};
    char ip_msg[100] = {0};
    char bcast_msg[100] = {0};
    //get ip address and bcast address
    acs_get_ipaddr_and_bcastaddr(ip_msg ,bcast_msg);
	//create udp socket
	sockdf = socket(AF_INET,SOCK_DGRAM, 0);
	if (sockdf == -1)
	{
		perror("create udp socket");
		exit(-1);
	}
	setsockopt(sockdf,SOL_SOCKET,SO_REUSEADDR | SO_BROADCAST,&on,sizeof(on));
	memset(&mcast_addr,0, sizeof(mcast_addr));
	mcast_addr.sin_family = AF_INET;
	mcast_addr.sin_addr.s_addr  = inet_addr(bcast_msg);
	mcast_addr.sin_port = htons(ACS_SERVER_TCP_PORT);
	//s2:interval 3s report broadcast message
	while(1)
	{
		time(&now);
		//tm_now = localtime(&now);
	    //strftime(datetime, 200, "%Y-%m-%d %H:%M:%S", tm_now);
		sprintf(tmp_msg,"%d",now);
	    memset(udp_msg,0,1024);
	    strcat(udp_msg,broadcast_msg);
	    strcat(udp_msg,ip_msg);
	    strcat(udp_msg+strlen(udp_msg),",");
	    strcat(udp_msg,devid);
	    strcat(udp_msg+strlen(udp_msg),",");
	    strcat(udp_msg+strlen(udp_msg),tmp_msg);
	    strcat(udp_msg+strlen(udp_msg),";");
	    n = sendto(sockdf, udp_msg,strlen(udp_msg), 0,(struct sockaddr*)&mcast_addr,sizeof(mcast_addr)) ;
		if( n < 0)
		{
			perror("udp sendto ");
		    exit(-1);
		}
		//printf("acs is broadcast udp message \n");
		TSleep(MCAST_INTERVAL);    /*等待一段时间*/
	}
}
