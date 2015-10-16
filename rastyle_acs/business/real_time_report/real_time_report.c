//*****************************************************************************
//  Copyright (C) 2015 rastyle¡£com
//	File name: real_time_report.c
//	Author:	whl
//	Description: create rastyle real_time_report.c file
//	Others:
//	History: 2015-7-20 create by whl
//*****************************************************************************

#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "../../message.h"   //message type define
#include "../../protocal/protocal.h"



void  acs_real_time_handle(int sockfd,char * data,int length,eEncodeType encode_type)
{
	int sendbytes;
	if ((sendbytes =
			send(sockfd,
			   seliaze_protocal_data_for_encrypt(data,length,real_time,TEST_USER_ID,encode_type),
			  length+PROTOCAL_FRAME_STABLE_LENGTH, MSG_NOSIGNAL)) == -1)
	{
		perror("acs client report real time message falied \n !");
        return ;
	}
	printf("acs client is reporting  real time data ! \n");
}
