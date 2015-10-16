//*****************************************************************************
//  Copyright (C) 2015 rastyle¡£com
//	File name: normal_mode_report.c
//	Author:	whl
//	Description: create rastyle normal_mode_report.c file
//	Others:
//	History: 2015-7-20 create by whl
//*****************************************************************************
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>


#include "../../protocal/protocal.h"
#include "../../common_utils.h"
#include "../../systemconfig.h"





void acs_normal_mode_report(int sockfd,char * data,int length,eEncodeType encode_type)
{
	int sendbytes,recvbytes;
	if ((sendbytes = send(sockfd,
				seliaze_protocal_data_for_encrypt(data,length,normal,TEST_USER_ID,encode_type),
				length+PROTOCAL_FRAME_STABLE_LENGTH, MSG_NOSIGNAL)) == -1)
	{
		perror("acs client report normal message falied \n !");
		return;
	}
	printf("acs client is report normal data... \n");

}
