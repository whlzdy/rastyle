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
char setup_connect_msg[] = "INFPL_CONRQ:Linux 3.2,1514.564,2514.325,Fresh_air_system,qwierygd8123asdfkoe;";
char setup_confim_msg[]  = "INFEX_SAFTE:INF=ALSKDFJEOIRTU01Q89234UERT0UAQWIOWUDEFJJQOWIEJFROIDJFFOVIQWUE8RUOUWIQUWER2322039;";
char setup_ack_msg[] = "Confirm;";


/*
 * build connection process
*/

void build_acs_connection(int sockfd)
{
	int sendbytes,recvbytes;
	char  buf[BUFFER_SIZE] = {0} ;
	char  verify_string_array[VERYFY_STRING_LENGTH] = {0};
	//build acs connect
	printf("entry build acs connection begin \n");
	if ((sendbytes =
	    	send(sockfd,
			  seliaze_protocal_data(setup_connect_msg,(uint16_t)strlen(setup_connect_msg),connection,TEST_USER_ID),
			  strlen(setup_connect_msg)+PROTOCAL_FRAME_STABLE_LENGTH, 0)) == -1)
	{
		perror("send setup_connect_msg falied !\n");
		return;
	}
	recvbytes = recv(sockfd,verify_string_array,VERYFY_STRING_LENGTH,0);
	if(recvbytes < 0)
	{
		perror("acs could not received server verify string failed !\n");
		return;
	}
	printf("acs receive server length is %d verify string is %s\n",recvbytes,deseliaze_protocal_data(verify_string_array,recvbytes));
	//acs decryption
	if ((sendbytes = send(sockfd,
				seliaze_protocal_data(setup_confim_msg,strlen(setup_confim_msg),connection,TEST_USER_ID),
				strlen(setup_confim_msg)+PROTOCAL_FRAME_STABLE_LENGTH, 0)) < 0)
	{
			perror("send setup_connect_msg falied ! failed");
			return;
	}
	//acs receive confirm message
	recvbytes = recv(sockfd,buf,1024,0);
	if(recvbytes < 0)
	{
		perror("acs could received server verify string failed 2 \n");
		return;
	}
//	if(strcmp(deseliaze_protocal_data(buf,10),"Confirm;") == 0)
//	{
	   printf("acs receive server length is %d ack msg is %s\n",recvbytes,deseliaze_protocal_data(buf,recvbytes));
		send(sockfd,
			seliaze_protocal_data(setup_ack_msg,strlen(setup_ack_msg),connection,TEST_USER_ID),
			strlen(setup_ack_msg)+PROTOCAL_FRAME_STABLE_LENGTH,
			0
			);
	//}
	//else
	//{
	//	printf("acs receivied server syn message wrong \n");
	//	return;
	//}
	printf("connetion sucessfull !\n");
	printf("exit build acs connection end \n");
}
