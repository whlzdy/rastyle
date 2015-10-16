//*****************************************************************************
//  Copyright (C) 2015 rastyle¡£com
//	File name: abnormal_mode_report.c
//	Author:	whl
//	Description: create rastyle acs_client.c file
//	Others:
//	History: 2015-7-20 create by whl
//*****************************************************************************

#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "../protocal/protocal.h"
#include "../message.h"
#include "../common_utils.h"






/*
 * acs client handle abnormal mode
*/
void acs_client_abnormal_mode_handle(int sockfd,char *data,int length,eEncodeType encode_type)
{
	int sendbytes,recvbytes;
	if ((sendbytes =
			send(sockfd,
				 seliaze_protocal_data_for_encrypt(data,length,abnormal,TEST_USER_ID,encode_type),
			  length+PROTOCAL_FRAME_STABLE_LENGTH, MSG_NOSIGNAL)) == -1)
	{
		perror("acs client report normal message falied \n !");
		return;
	}
	printf("acs client is reporting  abnormal  data ! \n");
}
