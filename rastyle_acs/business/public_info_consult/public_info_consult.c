//*****************************************************************************
//  Copyright (C) 2015 rastyle。com
//	File name: public_info_consult.c
//	Author:	whl
//	Description: create rastyle public_info_consult.c file
//	Others:
//	History: 2015-7-23 create by whl
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

static char report_keys_update_msg[] = "INFEX_SYKUD:KEY=ajHL823iasdfUIKadfg;";


static char report_public_consult_msg[] = "INFEX_SERCF:室内温度=Indoor_temp|100|-50|°C,室内湿度=Indoor_Humidity|100|0|%,室内CO2=Indoor_CO2|5000|0|ppm,室内甲醛=Indoor_HCHO|80|0|mg/m3,室内PM2.5=Indoor_PM2.5|300|0|μg/m3,室内VOC=Indoor_VOC|3|0|NULL,"\
		                            "室外温度=Outdoor_temp|100|-50|°C,室外湿度=Outdoor_Humidity|100|0|%,"\
		                            "室外PM2.5=Outdoor_PM2.5|300|0|μg/m3,室外PM1.0=Outdoor_PM1.0|300|0|μg/m3,室外PM10=Outdoor_PM10|300|0|μg/m3,"\
		                            "室外PM2.5(大气)=Outdoor_PM2.5|300|0|μg/m3,室外PM1.0(大气)=Outdoor_PM1.0|300|0|μg/m3,室外PM10(大气)=Outdoor_PM10.0|300|0|μg/m3;";


static char public_consult_confirm_msg[] = "Confirm;";


/*
** public information consult
*/
void public_information_consult(int sockfd)
{
	char recv_msg[1024] = {0};
	int sendbytes,recvbytes;
    printf("entry public information consult begin \n");
    //s1:report keys update message
    if ((sendbytes =
    			send(sockfd,
    			  seliaze_protocal_data(report_keys_update_msg,(uint16_t)strlen(report_keys_update_msg),public_consult,TEST_USER_ID),
    			  strlen(report_keys_update_msg)+PROTOCAL_FRAME_STABLE_LENGTH, 0)) == -1)
    	{
    		perror("report public keys message s1 falied !\n");
    		return;
    	}
    //s2:receive server keys ack message
    recvbytes = recv(sockfd,recv_msg,1024,0);
    if(recvbytes < 0)
    {
    	perror("receive server ack message failed \n");
    	return;
    }
    printf("acs receive server keys ack message is %s \n",deseliaze_protocal_data(recv_msg,recvbytes));
    //s3:get server timeout time
    memset(recv_msg,0,1024);
    recvbytes = recv(sockfd,recv_msg,1024,0);
    if(recvbytes < 0)
    {
    	perror("receive server ack message failed \n");
    	return;
    }
    printf("acs receive server timeout message is %s \n",deseliaze_protocal_data(recv_msg,recvbytes));
    //need compare receive message
    if ((sendbytes =
       			send(sockfd,
       			  seliaze_protocal_data(public_consult_confirm_msg,(uint16_t)strlen(public_consult_confirm_msg),public_consult,TEST_USER_ID),
       			  strlen(public_consult_confirm_msg)+PROTOCAL_FRAME_STABLE_LENGTH, 0)) == -1)
	{
		perror("report public keys message s3 falied !\n");
		return;
	}
    //s4:get server interval time
    memset(recv_msg,0,1024);
    recvbytes = recv(sockfd,recv_msg,1024,0);
	if(recvbytes < 0)
	{
		perror("receive server ack message interval failed \n");
		return;
	}
	printf("acs receive server interval message is %s \n",deseliaze_protocal_data(recv_msg,recvbytes));
	//need compare receive message
	if ((sendbytes =
				send(sockfd,
				  seliaze_protocal_data(public_consult_confirm_msg,(uint16_t)strlen(public_consult_confirm_msg),public_consult,TEST_USER_ID),
				  strlen(public_consult_confirm_msg)+PROTOCAL_FRAME_STABLE_LENGTH, 0)) == -1)
	{
		perror("report public keys message s4 falied !\n");
		return;
	}
	//s5:report acs sensor data
	if ((sendbytes =
					send(sockfd,
					  seliaze_protocal_data(report_public_consult_msg,(uint16_t)strlen(report_public_consult_msg),public_consult,TEST_USER_ID),
					  strlen(report_public_consult_msg)+PROTOCAL_FRAME_STABLE_LENGTH, 0)) == -1)
	{
		perror("report public keys message s4 falied !\n");
		return;
	}
	memset(recv_msg,0,1024);
	recvbytes = recv(sockfd,recv_msg,1024,0);
	if(recvbytes < 0)
	{
		perror("receive server ack message failed \n");
		return;
	}
	//int i;
	//for( i = 0;i < recvbytes;i++)
	//{
	//	printf("recvbytes %d is %d \n",i,recv_msg[i]);
	//}
	printf("acs receive server ack message is %s recvbytes is %d \n",deseliaze_protocal_data(recv_msg,recvbytes),recvbytes);
    printf("exit  public information consult end \n");
}
