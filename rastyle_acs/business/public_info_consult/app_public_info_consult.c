//*****************************************************************************
//  Copyright (C) 2015 rastyle。com
//	File name: app_public_info_consult.c
//	Author:	whl
//	Description: create rastyle public_info_consult.c file
//	Others:
//	History: 2015-8-19 create by whl
//*****************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "../../protocal/protocal.h"  //rasytye protocal
#include "../../systemconfig.h"
#include "../../openssl/rsa/acs_rsa.h"



static char report_keys_update_msg[] = "INFEX_SYKCF:KEY=ajHL823iasdfUIKadfg;";


static char report_2_sensor_msg[] = "INFEX_SERCF:室内温度=Indoor_temp|100|-50|°C,室内湿度=Indoor_Humidity|100|0|%,室内CO2=Indoor_CO2|5000|0|ppm,室内甲醛=Indoor_HCHO|80|0|mg/m3,室内PM2.5=Indoor_PM2.5|300|0|μg/m3,室内VOC=Indoor_VOC|3|0|NULL,"\
		                            "室外温度=Outdoor_temp|100|-50|°C,室外湿度=Outdoor_Humidity|100|0|%,"\
		                            "室外PM2.5=Outdoor_PM2.5|300|0|μg/m3,室外PM0.3=Outdoor_PM0.3|300|0|μg/m3,室外PM10=Outdoor_PM10|300|0|μg/m3;";
		                          //  "室外PM2.5(大气)=Outdoor_PM2.5|300|0|μg/m3,室外PM1.0(大气)=Outdoor_PM1.0|300|0|μg/m3,室外PM10(大气)=Outdoor_PM10.0|300|0|μg/m3;";

static char report_2_app_timeout[] = "INFEX_MRMDA:TIMDL=5;";
static char report_2_app_record[] = "INFEX_DRPER:PERIOD=15;";
static char report_2_app_user[] = "INFEX_OAREQ:role=Administrator;";

static char public_consult_confirm_msg[] = "Confirm;";


//char ACS_DES_KEY[48] = {0};

/*
** public information consult
*/
void app_public_information_consult(int sockfd,char* user_rsa_public_key,char* des_key)
{
	char * token2 = NULL;
	char *name =NULL,*value = NULL;
	char *tmp = NULL,*token = NULL;
	int package_length = 0;
	char  buf[BUFFER_SIZE] = {0};
	char recv_msg[1024] = {0};
	int sendbytes,recvbytes;
    printf("entry app public information consult begin \n");
    //s1:receive server keys ack message
    recvbytes = acs_tcp_receive(sockfd,buf,&package_length);
	if(recvbytes < 0)
	{
		perror("receive app keys ack message failed in APP public info consult \n");
		return ;
	}
#ifdef ACS_ENCRYPT_FLAG
    //begin rsa decrypt
	int encode_len;
	//printf("acs receive app pwd length is %d  msg is %s\n",recvbytes,deseliaze_protocal_data(buf,recvbytes));
	//deseliaze_protocal_encode_data(buf,package_length,&encode_len);
	encode_len = package_length -11;
	tmp = js_public_decrypt(deseliaze_protocal_encode_data(buf,package_length,NULL),encode_len, user_rsa_public_key);//ACS_ANDROID_PUBLIC_KEY  ACS_IOS_PUBLIC_KEY
	printf("acs receive app des keys message is %s",tmp);
	int first_flag = 1;
	while((token = strsep(&tmp, ":")) != NULL)
	{
		if(first_flag)
		{
			printf("%s\n", token);
			first_flag = 0;
		}
		//printf("%s\n", token);
		if(strstr(token,"="))
		{
			name = strsep(&token, "=");
			token2 = strsep(&token,";");
			//memcpy(ACS_DES_KEY,token2,strlen(token2));
			memcpy(des_key,token2,strlen(token2));
			printf("acs des key is %s\n",des_key);
		}
	}
	free(tmp);

#else
	printf("acs receive app keys des key message is %s \n",deseliaze_protocal_data(recv_msg,recvbytes));
#endif
	//s2:report keys update message
    if ((sendbytes =
			send(sockfd,
			  seliaze_protocal_data(report_keys_update_msg,(uint16_t)strlen(report_keys_update_msg),public_consult,TEST_USER_ID),
			  strlen(report_keys_update_msg)+PROTOCAL_FRAME_STABLE_LENGTH, 0)) == -1)
	{
		perror("report app public keys message s1 falied !\n");
		return;
	}
    //s3:report timeout
    if ((sendbytes =
   			send(sockfd,
   			  seliaze_protocal_data(report_2_app_timeout,(uint16_t)strlen(report_2_app_timeout),public_consult,TEST_USER_ID),
   			  strlen(report_2_app_timeout)+PROTOCAL_FRAME_STABLE_LENGTH, 0)) == -1)
   	{
   		perror("report app  timeout  message  falied !\n");
   		return;
   	}
    //s4:get app timeout time ack
    memset(recv_msg,0,1024);
    recvbytes = recv(sockfd,recv_msg,1024,0);
    if(recvbytes < 0)
    {
    	perror("receive app timeout ack message failed \n");
    	return;
    }
    printf("acs receive app timeout ack is %s \n",deseliaze_protocal_data(recv_msg,recvbytes));
    //s5:report record time
    if ((sendbytes =
      			send(sockfd,
      			  seliaze_protocal_data(report_2_app_record,(uint16_t)strlen(report_2_app_record),public_consult,TEST_USER_ID),
      			  strlen(report_2_app_record)+PROTOCAL_FRAME_STABLE_LENGTH, 0)) == -1)
      	{
      		perror("report public keys message s1 falied !\n");
      		return;
      	}
    //s6:get app record time ack
    memset(recv_msg,0,1024);
    recvbytes = recv(sockfd,recv_msg,1024,0);
    if(recvbytes < 0)
    {
    	perror("receive app record time ack message failed \n");
    	return;
    }
    printf("acs receive app record time ack is %s \n",deseliaze_protocal_data(recv_msg,recvbytes));
    //s7:report app 2 user
    if ((sendbytes =
				send(sockfd,
				  seliaze_protocal_data(report_2_app_user,(uint16_t)strlen(report_2_app_user),public_consult,TEST_USER_ID),
				  strlen(report_2_app_user)+PROTOCAL_FRAME_STABLE_LENGTH, 0)) == -1)
		{
			perror("report app 2 user failed !\n");
			return;
		}
     //s8:get app record time ack
     memset(recv_msg,0,1024);
     recvbytes = recv(sockfd,recv_msg,1024,0);
     if(recvbytes < 0)
     {
    	 perror("receive app 2 user message failed \n");
    	 return;
     }
     printf("acs receive app 2 user message ack is %s \n",deseliaze_protocal_data(recv_msg,recvbytes));
     //s9:report sensor rule
	 if ((sendbytes =
				send(sockfd,
				  seliaze_protocal_data(report_2_sensor_msg,(uint16_t)strlen(report_2_sensor_msg),public_consult,TEST_USER_ID),
				  strlen(report_2_sensor_msg)+PROTOCAL_FRAME_STABLE_LENGTH, 0)) == -1)
		{
			perror("report app sensor rule failed !\n");
			return;
		}
	 //s10:get app sensor rule ack
	 memset(recv_msg,0,1024);
	 recvbytes = recv(sockfd,recv_msg,1024,0);
	 if(recvbytes < 0)
	 {
		 perror("receive app sensor rule message failed \n");
		 return;
	 }
	 printf("acs receive app sensor rule message ack is %s \n",deseliaze_protocal_data(recv_msg,recvbytes));

     printf("exit app public information consult end \n");
}
