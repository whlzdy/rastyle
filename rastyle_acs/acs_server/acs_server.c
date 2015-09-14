//*****************************************************************************
//  Copyright (C) 2015 rastyle¡£com
//	File name: protocal.h
//	Author:	whl
//	Description: create rastyle protocol head file
//	Others:
//	History: 2015-7-20 create by whl
//*****************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>
#include <sys/stat.h>	   //for file operate
#include <fcntl.h>
#include <signal.h>
#include <sys/syscall.h>
#include <linux/unistd.h>
#include <sys/signal.h>
#include <sys/ipc.h>	   //for msg
#include <sys/msg.h>

#include "acs_server.h"
#include "../common_utils.h"
#include "../systemconfig.h"
#include "../message.h"
#include "../protocal/protocal.h"


extern void *handle_app_request_thread(void *args);

#define MAX_SOCK_FD FD_SETSIZE

fd_set inset;

/*
* acs server tcp thread
*/
void *acs_server_tcp_thread(void *args)
{
   	 pthread_t acs_server_handle_id ;
	 pthread_attr_t handle_app_thread_attr;
	 fd_set tmpinset;
	 int sockfd,client_fd,fd;
	 int yes = 1,priority = PTH_PRIO_LOWEST;
	 struct sockaddr_in addr;
	 socklen_t addrlen;
	 thread_arg arg;
	 int conn_count = 0;
	 sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	 printf("server local sockfd is %d\n",sockfd);
	 if(sockfd == -1) {
		  perror("fail to socket()");
		  exit(-1);
	 }
	 if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,
				   (char *) &yes, sizeof(yes)) == -1) {
		  perror("setsockop error\n");
		  close(sockfd);
		  exit(-1);
	 }
	 memset(&addr, 0, sizeof(addr));
	 addr.sin_family = AF_INET;
	 addr.sin_port = htons(ACS_SERVER_TCP_PORT);
	 addr.sin_addr.s_addr = INADDR_ANY;
	 if(bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
		  close(sockfd);
		  perror("fail to bind");
		  exit(-1);
	 }
     //set server listen max connnetion nums
	 if(listen(sockfd, ACS_SERVER_MAX_CONNS) == -1) {
		  close(sockfd);
		  perror("fail to listen");
		  exit(-1);
	 }
     FD_ZERO(&inset);
     FD_SET(sockfd,&inset);
	 while(1)
	 {
		tmpinset = inset;
		//printf("wait for client to request connection\n");
		if (!(select(MAX_SOCK_FD, &tmpinset, NULL, NULL, NULL) > 0)) //0
		{
			 TusSleep(5000);
			 //perror("acs server select");
			 continue;
		}
		if (FD_ISSET(sockfd, &tmpinset) > 0)
		{
			client_fd = accept(sockfd, (struct sockaddr *)&addr, &addrlen);
			if(client_fd == -1)
			{
				printf("accept fail\n");
				TusSleep(1000);
				continue;
			}
			FD_SET(client_fd, &inset);
			conn_count++;
			//printf("new socket connect to client,the socket fd is %d\n",client_fd);
			arg.psockfd = &client_fd;
			AIM_pThreadCreate_auto(&acs_server_handle_id, handle_app_thread_attr, priority--,handle_app_request_thread, &arg);
		}
		TusSleep(5000);
      }
}

