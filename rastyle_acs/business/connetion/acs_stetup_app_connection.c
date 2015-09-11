//*****************************************************************************
//  Copyright (C) 2015 rastyle¡£com
//	File name: connetion.c
//	Author:	whl
//	Description: create rastyle connetion.c file
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


#include "../../protocal/protocal.h"

#define BUFFER_SIZE (65536+20)
#define VERYFY_STRING_LENGTH 1024

//build connect setups use to test
static char setup_username_msg[] = "UserName;";
static char setup_pwd_msg[]      = "Pwd;";
static char setup_confirm_msg[]  = "Confirm;";


/*
 * build connection process
*/

void stetup_acs_app_connection(int sockfd)
{
	int sendbytes,recvbytes;
	char  buf[BUFFER_SIZE] = {0} ;
	char  verify_string_array[VERYFY_STRING_LENGTH] = {0};
	//build acs connect
	printf("entry build acs app connection begin \n");
	recvbytes = recv(sockfd,verify_string_array,VERYFY_STRING_LENGTH,0);
	if(recvbytes <= 0)
	{
		perror("acs could not received app msg!\n");
		return;
	}
	printf("acs receive app length is %d message string is %s\n",recvbytes,deseliaze_protocal_data(verify_string_array,recvbytes));
	//acs send username
	if ((sendbytes = send(sockfd,
				seliaze_protocal_data(setup_username_msg,strlen(setup_username_msg),connection,TEST_USER_ID),
				strlen(setup_username_msg)+PROTOCAL_FRAME_STABLE_LENGTH, 0)) < 0)
	{
			perror("send setup_connect_msg falied ! failed");
			return;
	}
	//acs received username
	recvbytes = recv(sockfd,buf,1024,0);
	if(recvbytes < 0)
	{
		perror("acs could received server verify string failed 2 \n");
		return;
	}
    //need check username
	//acs send pwd
    printf("acs receive app username length is %d  msg is %s\n",recvbytes,deseliaze_protocal_data(buf,recvbytes));
	send(sockfd,
		seliaze_protocal_data(setup_pwd_msg,strlen(setup_pwd_msg),connection,TEST_USER_ID),
		strlen(setup_pwd_msg)+PROTOCAL_FRAME_STABLE_LENGTH,
		0
		);
    recvbytes = recv(sockfd,buf,1024,0);
	if(recvbytes < 0)
	{
		perror("acs could received server verify string failed 2 \n");
		return;
	}
	printf("acs receive app pwd length is %d  msg is %s\n",recvbytes,deseliaze_protocal_data(buf,recvbytes));
	//send confirm
	send(sockfd,
			seliaze_protocal_data(setup_confirm_msg,strlen(setup_confirm_msg),connection,TEST_USER_ID),
			strlen(setup_confirm_msg)+PROTOCAL_FRAME_STABLE_LENGTH,
			0
			);
	printf("acs - app connetion sucessfull !\n");
	printf("exit build acs app connection end \n");
}
