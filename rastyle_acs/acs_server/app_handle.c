//*****************************************************************************
//  Copyright (C) 2015 rastyle¡£com
//	File name: app_handle.c
//	Author:	whl
//	Description: create rastyle app handle.c file
//	Others:
//	History: 2015-7-29 create by whl
//*****************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>	   //for file operate
#include <fcntl.h>
#include <signal.h>
#include <sys/syscall.h>
#include <linux/unistd.h>
#include <sys/signal.h>
#include <sys/ipc.h>	   //for msg
#include <sys/msg.h>


#include "acs_server.h"
#include "../message.h"
#include "../common_utils.h"
#include "../protocal/protocal.h"
#include "../hal/zigbee/sensor.h"
#include "../systemconfig.h"
#include "../device_control/device_control.h"
#include "../sqlite/sqlite.h"



static char finish_msg[] = "Configuration_Information_Update_Finish;";
static char confirm_msg[] = "Confirm;";
static char fail_msg[] = "Fail;";

extern fd_set inset;
extern tACS_server_fd_msg_list acs_app_handle_list;

static char report_app_msg[1024] = {0};



/*
* app real time handle call back
*/
void acs_app_realtime_handle(int sockfd,char *data,int length)
{
	int rc;
	 // report real time data
	//fprintf(stderr,"server is to sockfd %d report data .is %s \n",sockfd,data);
	rc = send(sockfd,
		 seliaze_protocal_data(data,strlen(data),real_time,TEST_USER_ID),
		 length+PROTOCAL_FRAME_STABLE_LENGTH,
		 MSG_NOSIGNAL);
}



int get_app_record_callback (void * data, int col_count, char ** col_values, char ** col_Name)
{

	 int rc ;
	 int i,offset = 0;
	 char tmp[100] = {0};
	 char header[] = "INFEX_ECINF:";
	 int sockfd = (*(int *)data);
	 memset(report_app_msg,0,1024);
	 strcat(report_app_msg+offset,header);
	 offset += strlen(header);
	 for( i=0; i < 1; i++)
	 {
		memset(tmp,0,100);
		sprintf(tmp,"%s=%s,", col_Name[i], col_values[i] == 0 ? "NULL" : col_values[i] );
		strcat(report_app_msg+offset,tmp);
		offset+=strlen(tmp);
	 }
	 for( i = 1; i < 7; i+=2)
	 {
		memset(tmp,0,100);
		sprintf(tmp,"%s=%s,", col_values[i], col_values[i+1] == 0 ? "NULL" : col_values[i+1] );
		strcat(report_app_msg+offset,tmp);
		offset+=strlen(tmp);
	 }
	 for( i = 7; i < col_count-1; i++)
	 {
		memset(tmp,0,100);
		sprintf(tmp,"%s=%s,", col_Name[i], col_values[i] == 0 ? "NULL" : col_values[i] );
		strcat(report_app_msg+offset,tmp);
		offset+=strlen(tmp);
	 }
	 memset(tmp,0,100);
	 sprintf(tmp,"%s=%s", col_Name[i], col_values[i] == 0 ? "NULL" : col_values[i] );
	 strcat(report_app_msg+offset,tmp);
	 offset+=strlen(tmp);
	 strcat(report_app_msg+offset,";");
	 rc = send(sockfd,
				seliaze_protocal_data(report_app_msg,strlen(report_app_msg),info_sync,TEST_USER_ID),
				strlen(report_app_msg)+PROTOCAL_FRAME_STABLE_LENGTH,
				MSG_NOSIGNAL
				);
	 return 0;
}


int get_app_climate_callback (void * data, int col_count, char ** col_values, char ** col_Name)
{
	 int rc ;
	 int i,offset = 0;
	 char tmp[100] = {0};
	 char header[] = "INFEX_CLINF:";
	 int sockfd = (*(int *)data);
	 memset(report_app_msg,0,1024);
	 strcat(report_app_msg+offset,header);
	 offset += strlen(header);
	 for( i=0; i < col_count-1; i++){
		memset(tmp,0,100);
	    sprintf(tmp,"%s=%s,", col_Name[i], col_values[i] == 0 ? "NULL" : col_values[i] );
	    strcat(report_app_msg+offset,tmp);
	    offset+=strlen(tmp);
	  }
	 memset(tmp,0,100);
	 sprintf(tmp,"%s=%s", col_Name[i], col_values[i] == 0 ? "NULL" : col_values[i] );
	 strcat(report_app_msg+offset,tmp);
	 offset+=strlen(tmp);
	 strcat(report_app_msg+offset,";");
	 rc = send(
			 sockfd,
			 seliaze_protocal_data(report_app_msg,strlen(report_app_msg),info_sync,TEST_USER_ID),
			 strlen(report_app_msg)+PROTOCAL_FRAME_STABLE_LENGTH,
			 MSG_NOSIGNAL
			 );
	 return 0;
}




/*
 * thread main function
*/
void *handle_app_request_thread(void *args)
{
	int  userid;
	char username[100] = {0};
	char buffer[65536] = {0};
	char recv_msg[65536] = {0};
	char tmp_msg[1024] = {0};
	int sockfd,rc;
	int recvbytes ;
	int length;
	int ret;
	sockfd = *(((thread_arg*)args)->psockfd);
	printf("app handle thread sockfd is %d created \n",sockfd);
	//s1 : setup connection
    ret = stetup_acs_app_connection(sockfd);
    if(ret != 0)
    {
    	return 0;
    }
	//s2 : public message consult
    app_public_information_consult(sockfd);
    //s3 : register sensor callback
    acs_server_bind2_sensor(sockfd,acs_app_realtime_handle);
	while(1)
	{
		//s3 : receive meesage and message
		memset(recv_msg,0,65536);
		//recvbytes = recv(sockfd,  recv_msg, sizeof(recv_msg),MSG_NOSIGNAL);
		recvbytes = acs_tcp_receive(sockfd, recv_msg, &length);
		if(recvbytes == -1)
		{
		     printf("sockfd is %d left as recv failed ",sockfd);
			 FD_CLR(sockfd,&inset);
			 close(sockfd);
			 acs_app_handle_list.app_conn_count--;
			 break;
		}
		if(length > 0)
		{
			memset(buffer,0,65536);
			memcpy(buffer,deseliaze_protocal_data(recv_msg,recvbytes),strlen(deseliaze_protocal_data(recv_msg,recvbytes))-1);//delete end flag
			printf("app control message is %s len is %d\n",buffer,strlen(buffer));
			 //need check username  query user table
			userid = acs_get_user_id(buffer,recvbytes);
			strcat(username,deseliaze_protocal_data(buffer,recvbytes));
			if(acs_verify_user_name(userid,username) != 0);
			{
				printf("username is not match userid!  \n");
				send(sockfd,
						seliaze_protocal_data(fail_msg,strlen(fail_msg),connection,TEST_USER_ID),
						strlen(fail_msg)+PROTOCAL_FRAME_STABLE_LENGTH,
						0
						);
				continue;
			}
			if(strcmp(buffer,"Apply_for_control_authority;") == 0)
			{
				//send confirm msg
				rc = send(sockfd,
						seliaze_protocal_data(confirm_msg,strlen(confirm_msg),device_control,TEST_USER_ID),
						strlen(confirm_msg)+PROTOCAL_FRAME_STABLE_LENGTH,
						MSG_NOSIGNAL);
				if(rc == -1)
				{
					 printf("sockfd is %d left as send failed 2 ",sockfd);
					 FD_CLR(sockfd,&inset);
					 close(sockfd);
					 acs_app_handle_list.app_conn_count--;
					 break;
				}
			}
			else if(strcmp(buffer,"Undo_for_control_authority;") == 0)
			{
				//send confirm msg
				rc = send(sockfd,
						seliaze_protocal_data(confirm_msg,strlen(confirm_msg),device_control,TEST_USER_ID),
						strlen(confirm_msg)+PROTOCAL_FRAME_STABLE_LENGTH,
						MSG_NOSIGNAL);
				if(rc == -1)
				{
					 printf("sockfd is %d left as send failed 2 ",sockfd);
					 FD_CLR(sockfd,&inset);
					 close(sockfd);
					 acs_app_handle_list.app_conn_count--;
					 break;
				}
			}
			else if(strcmp(buffer,"State_synchronization_request;") == 0)
			{
				char sqldata[] = "select * from acs_plan_task_table;";
				char sqldata2[] = "select * from acs_climate_data_table ;";
				//send plan table
			    sqlite_get_record_data(ACS_CONFIG_DATEBASE,sqldata,get_app_record_callback,&sockfd);
				//send climate
				sqlite_get_record_data(ACS_CONFIG_DATEBASE,sqldata2,get_app_climate_callback,&sockfd);
				//send finish msg
				rc = send(sockfd,
							seliaze_protocal_data(finish_msg,strlen(finish_msg),info_sync,TEST_USER_ID),
							strlen(finish_msg)+PROTOCAL_FRAME_STABLE_LENGTH,
							MSG_NOSIGNAL
							);
				if(rc == -1)
				{
					 FD_CLR(sockfd,&inset);
					 close(sockfd);
					 acs_app_handle_list.app_conn_count--;
					 break;
				}
			}
			else
			{
				printf("acs receive app device control message is %s \n",buffer);
				//device controlacs_plan_task
				acs_handle_device_crontrol(sockfd,buffer);
			}
		}
		TusSleep(500);
	}
	printf("server handle_app_request_thread sockfd %d quit \n",sockfd);
}
