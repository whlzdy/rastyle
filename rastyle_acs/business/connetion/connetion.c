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
#include "../../systemconfig.h"
#include "../../openssl/rsa/acs_rsa.h"

#define BUFFER_SIZE (65536+20)
#define VERYFY_STRING_LENGTH 1024

//build connect setups use to test
//char setup_connect_msg[] = "INFPL_CONRQ:Linux 3.2,1514.564,2514.325,Fresh_air_system,qwierygd8123asdfkoe;";
//char setup_confim_msg[]  = "INFEX_SAFTE:INF=ALSKDFJEOIRTU01Q89234UERT0UAQWIOWUDEFJJQOWIEJFROIDJFFOVIQWUE8RUOUWIQUWER2322039;";
char setup_ack_msg[] = "Confirm;";


static void serilze_setup_connect_msg(char *message)
{
	memset(message,0,strlen(message));
	strcat(message,"INFPL_CONRQ:Linux 3.2,");
	strcat(message,SOFT_VERISION);
	strcat(message,",");
	strcat(message,HARD_VERISION);
	strcat(message,",");
	strcat(message,"Fresh_air_system,");
	strcat(message,DEVICE_ID);
	strcat(message,";");
}

/*
 * build connection process
*/

void build_acs_connection(int sockfd)
{
	int packlength = 0,encode_length = 0,tmp = 0;
	int sendbytes,recvbytes;
	char *plain_text = NULL,*cipher_text = NULL;
	char  buf[BUFFER_SIZE] = {0} ;
	//build acs connect
	printf("entry build acs connection begin \n");
	//s1:serilze setup connect msg
	serilze_setup_connect_msg(buf);
	if ((sendbytes =
	    	send(sockfd,
			  seliaze_protocal_data(buf,(uint16_t)strlen(buf),connection,TEST_USER_ID),
			  strlen(buf)+PROTOCAL_FRAME_STABLE_LENGTH, 0)) == -1)
	{
		perror("send setup_connect_msg falied !\n");
		return;
	}
	//s2:receve encode message
	memset(buf,0,BUFFER_SIZE);
	recvbytes = acs_tcp_receive(sockfd, buf,&packlength);
	if(recvbytes <=-1)
	{
		printf("acs client receive cloud 1 ack failed \n");
		return ;
	}
	//s3:cloud_pubic_key decrypt and acs_private encry
	deseliaze_protocal_encode_data(buf,packlength,&encode_length);
	printf("encode_length is %d \n",encode_length);
	FILE *fp;
	if((fp = fopen("/home/whl/rsa.data", "w+r"))==NULL)
	{
		printf("cant open the file \n");
		exit(0);
	}
	fwrite(deseliaze_protocal_encode_data(buf,packlength,&encode_length), sizeof(char), encode_length, fp);
	fclose(fp);
	plain_text = js_public_decrypt(deseliaze_protocal_encode_data(buf,packlength,&tmp),encode_length,CLOUD_PUBLIC_KEY);//TEST_KEY CLOUD_PUBLIC_KEY
	printf("acs receive cloud decrypt plain text is %s \n",plain_text);
	//acs decryption
	tmp = 0;
	cipher_text = js_private_encrypt(plain_text,&tmp,ACS_PRIVATE_KEY);
	free(plain_text);
	if ((sendbytes = send(sockfd,
				seliaze_protocal_data(cipher_text,tmp,connection,TEST_USER_ID),
				tmp+PROTOCAL_FRAME_STABLE_LENGTH, 0)) < 0)
	{
			perror("send setup_connect_msg falied ! failed");
			return;
	}
	free(cipher_text);
	//s4:acs receive confirm message
	memset(buf,0,BUFFER_SIZE);
	recvbytes = recv(sockfd,buf,1024,0);
	if(recvbytes < 0)
	{
		perror("acs could received server verify string failed 2 \n");
		return;
	}
	if(strcmp(deseliaze_protocal_data(buf,recvbytes),"Confirm;") == 0)
	{
		printf("Confirm;\n");
	}
	else
	{
		printf("Fail;\n");
	}
	//send(sockfd,
	//	seliaze_protocal_data(setup_ack_msg,strlen(setup_ack_msg),connection,TEST_USER_ID),
	//	strlen(setup_ack_msg)+PROTOCAL_FRAME_STABLE_LENGTH,
	//	0
	//	);

	printf("connetion sucessfull !\n");
	printf("exit build acs connection end \n");
}
