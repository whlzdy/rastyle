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
#include "../../openssl/rsa/acs_rsa.h"
#include "../../systemconfig.h"


#define VERYFY_STRING_LENGTH 1024

//build connect setups use to test
static char setup_username_msg[] = "UserName;";
static char setup_pwd_msg[]      = "Pwd;";
static char setup_confirm_msg[]  = "Confirm;";
static char setup_fail_msg[]    = "Fail;";



/*
 * build connection process
*/

int stetup_acs_app_connection(int sockfd,char * user_rsa_public_key)
{
	int packagelength = 0;
	char *tmp = NULL;
	char username[100] = {0};
	char pwd[100] = {0};
	int userid;
	int sendbytes,recvbytes;
	char  buf[BUFFER_SIZE] = {0} ;
	char  verify_string_array[VERYFY_STRING_LENGTH] = {0};
	char * token;
	char * plaintext = NULL;
	//build acs connect
	printf("entry build acs app connection begin \n");
	recvbytes = acs_tcp_receive(sockfd,verify_string_array,&packagelength);
	if(recvbytes < 0)
	{
		perror("acs could not received app msg!\n");
		return -1;
	}
	printf("acs receive app length is %d message string is %s\n",recvbytes,deseliaze_protocal_data(verify_string_array,recvbytes));
	//acs send username
	if(sendbytes = acs_tcp_send(sockfd,
		seliaze_protocal_data(setup_username_msg,strlen(setup_username_msg),connection,TEST_USER_ID),
		strlen(setup_username_msg)+PROTOCAL_FRAME_STABLE_LENGTH) < 0)
	{
			perror("send setup_connect_msg falied ! failed");
			return -1;
	}
	//acs received username
	packagelength = 0;
	recvbytes = acs_tcp_receive(sockfd,buf,&packagelength);
	if(recvbytes < 0)
	{
		perror("acs could received server verify string failed 2 \n");
		return -1;
	}
	printf("recvbytes is %d \n",recvbytes);
	//need check username  query user table
	userid = acs_get_user_id(buf,recvbytes);
	strcat(username,deseliaze_protocal_data(buf,recvbytes));
	tmp = username;
	token = strsep(&tmp, ";");
	//printf("token is %s \n",token);
	//printf("sssarusename is %s \n",username);
	if(acs_verify_user_name(userid,username) != 0)
	{
		printf("username is %s username is not match userid!  \n",username);
		acs_tcp_send(sockfd,
				seliaze_protocal_data(setup_fail_msg,strlen(setup_fail_msg),connection,TEST_USER_ID),
				strlen(setup_fail_msg)+PROTOCAL_FRAME_STABLE_LENGTH
				);
		return -1;
	}
	//need check username
	//acs send pwd
	printf("acs receive app username length is %d  msg is %s\n",recvbytes,deseliaze_protocal_data(buf,recvbytes));
	acs_get_user_rsa_public_key(userid,user_rsa_public_key);
	//printf("acs receive app username length is %d  msg is %s\n",recvbytes,deseliaze_protocal_data(buf,recvbytes));
	if(sendbytes = acs_tcp_send(sockfd,
		seliaze_protocal_data(setup_pwd_msg,strlen(setup_pwd_msg),connection,TEST_USER_ID),
		strlen(setup_pwd_msg)+PROTOCAL_FRAME_STABLE_LENGTH) < 0)
	{
		perror("send setup_pwd_msg falied ! ");
		return -1;
	}
	memset(buf,0,BUFFER_SIZE);
	//receive rsa encrypt password
	packagelength = 0;
	recvbytes = acs_tcp_receive(sockfd,buf,&packagelength);
	if(recvbytes < 0)
	{
		perror("acs could received app user password \n");
		return recvbytes;
	}

	//begin rsa decrypt
#ifdef ACS_ENCRYPT_FLAG
	int encode_len;
	//printf("acs receive app pwd length is %d  msg is %s\n",recvbytes,deseliaze_protocal_data(buf,recvbytes));
	//deseliaze_protocal_encode_data(buf,packagelength,&encode_len);
	encode_len = packagelength -11;
	//printf("encode_len is %d \n",encode_len);
	plaintext = js_public_decrypt(deseliaze_protocal_encode_data(buf,packagelength,NULL),encode_len, user_rsa_public_key);
	if(plaintext == NULL)
	{
		return -1;
	}
	//strcat(pwd,plaintext);//ACS_ANDROID_PUBLIC_KEY  ACS_IOS_PUBLIC_KEY
	memcpy(pwd,plaintext,strlen(plaintext));
	free(plaintext);

#else
	strcat(pwd,deseliaze_protocal_data(buf,packagelength));
#endif
	tmp = pwd;
	token = strsep(&tmp, ";");
	if(acs_verify_pwd(username,pwd) != 0)
	{
		printf("password wrong !\n");
		acs_tcp_send(sockfd,
				seliaze_protocal_data(setup_fail_msg,strlen(setup_fail_msg),connection,TEST_USER_ID),
				strlen(setup_fail_msg)+PROTOCAL_FRAME_STABLE_LENGTH
				);
		printf("acs - app connetion failed  !\n");
		return -1;
	}
	//printf("acs receive app pwd length is %d  msg is %s\n",recvbytes,deseliaze_protocal_data(buf,recvbytes));
	//send confirm
	acs_tcp_send(sockfd,
			seliaze_protocal_data(setup_confirm_msg,strlen(setup_confirm_msg),connection,TEST_USER_ID),
			strlen(setup_confirm_msg)+PROTOCAL_FRAME_STABLE_LENGTH);
	printf("acs - app connetion sucessfull !\n");
	printf("exit build acs app connection end \n");
	return 0;
}
