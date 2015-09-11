//*****************************************************************************
//  Copyright (C) 2015 rastyle。com
//	File name: acs_client.c
//	Author:	whl
//	Description: create rastyle acs_client.c file
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
#include <pthread.h>
#include <sys/stat.h>	   //for file operate
#include <fcntl.h>
#include <signal.h>
#include <sys/syscall.h>
#include <linux/unistd.h>
#include <sys/signal.h>
#include <sys/ipc.h>	   //for msg
#include <sys/msg.h>

#include "acs_client.h"
#include "../common_utils.h"
#include "../systemconfig.h"
#include "../message.h"
#include "../protocal/protocal.h"
#include "../hal/zigbee/sensor.h"
#include "../device_control/device_control.h"
#include "../sqlite/sqlite.h"



static int sockfd ;   //global


uint8_t acs_client_mode = 0x0; //0x01:正常模式 ;0x02:实时模式;0x04:异常模式


static char finish_msg[] = "Configuration_Information_Update_Finish;";
static char confirm_msg[] = "Confirm;";
static char fail_msg[] = "Fail;";


extern void acs_client_abnormal_mode_handle(int sockfd,char * data,int length);
extern void acs_real_time_handle(int sockfd,char * data,int length);
extern void acs_normal_mode_report(int sockfd,char * data,int length);




static void handle_socket_reconnection(void)
{
	printf("... acs client is reconnetion ... \n");
    close(sockfd);
}

static char report_app_msg[1024] = {0};

int get_record_callback (void * data, int col_count, char ** col_values, char ** col_Name)
{
	 int rc ;
	 int i,offset = 0;
	 char tmp[100] = {0};
	 char header[] = "INFEX_PLINF:";
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
	 //printf("report_app_msg is %s \n",report_app_msg);
	 rc = send(sockfd,
				seliaze_protocal_data(report_app_msg,strlen(report_app_msg),info_sync,TEST_USER_ID),
				strlen(report_app_msg)+PROTOCAL_FRAME_STABLE_LENGTH,
				MSG_NOSIGNAL);
	 return 0;
}


int get_climate_callback (void * data, int col_count, char ** col_values, char ** col_Name)
{
	 int rc ;
	 int i,offset = 0;
	 char tmp[100] = {0};
	 char header[] = "INFEX_ECINF:";
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
	 //printf("report_app_msg is %s \n",report_app_msg);
	 rc = send(sockfd,
				seliaze_protocal_data(report_app_msg,strlen(report_app_msg),info_sync,TEST_USER_ID),
				strlen(report_app_msg)+PROTOCAL_FRAME_STABLE_LENGTH,
				MSG_NOSIGNAL);
	 return 0;
}


/************************************************************************/
//                    acs_client_normal_thread
//            Desc  :  acs client normal communcate to with server
//            Parma :  'args' is process file path(Temporarily)
//            Author:  whl
//            Date  :  2015-7-20
/************************************************************************/
void *acs_client_thread(void *args)
{
	int userid;
	char username[100] = {0};
	int packlength = 0;
	int sendbytes,recvbytes;
	struct hostent *host;
	struct sockaddr_in serv_addr;
	char recv_msg[65536] = {0};
	char buffer[65536] = {0};
	int rc;
	sleep(5);
Reconnetion:
	/*address resolution */
	if ((host = gethostbyname(RASTYLE_ACS_CLIENT_IPADDR)) == NULL)
	{
		perror("gethostbyname failed \n");
		TSleep(30);
		goto Reconnetion;
	}
	/*set sockaddr_in params*/
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(RASTYLE_ACS_CLIENT_PORT);
	serv_addr.sin_addr = *((struct in_addr *)host->h_addr);
	bzero(&(serv_addr.sin_zero), 8);

	/*create socket*/
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket failed \n ");
		TSleep(30);
		goto Reconnetion;
	}
	//connet to the server
    if(connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(struct sockaddr))== -1)
	{
    	printf("server ip id %s port is %d \n",RASTYLE_ACS_CLIENT_IPADDR,RASTYLE_ACS_CLIENT_PORT);
		perror("connect");
	    TSleep(30);
	    goto Reconnetion;
	}
    // set_keepalive(sockfd,1,5,5,5);//5,5,5
    // bind to sensor thread
    acs_client_bind2_sensor(sockfd,acs_normal_mode_report,acs_client_abnormal_mode_handle,acs_real_time_handle);
	printf("socket sucessful to connect to server\n");
    build_acs_connection(sockfd);
	printf("acs connetion completed \n");
	public_information_consult(sockfd);
	//update user table
	acs_update_user_from_cloud(sockfd);
	printf("public information consult  completed \n");
	while(1)
	{
		//printf("acs client is waiting receive data ...\n");
		memset(recv_msg,0,65536);
		//recvbytes = recv(sockfd, recv_msg, sizeof(recv_msg), MSG_NOSIGNAL);
		recvbytes = acs_tcp_receive(sockfd, recv_msg,&packlength );
		if(recvbytes == -1 )
		{
			TusSleep(10000); //set time interval
			continue;
		}
		memset(buffer,0,sizeof(buffer));
		memcpy(buffer,deseliaze_protocal_data(recv_msg,recvbytes),strlen(deseliaze_protocal_data(recv_msg,recvbytes))-1);//delete end flag
		printf("cloud control message is %s len is %d\n",buffer,strlen(buffer));
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
			acs_client_mode &= 0x00;
			continue;

		}
		acs_client_mode |= 0x01; //active normal mode
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
				 handle_socket_reconnection();
				 goto Reconnetion;

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
				 handle_socket_reconnection();
				 goto Reconnetion;

			}
		}
		else if(strcmp(buffer,"State_synchronization_request;") == 0)
		{
			//send plan table
			char sqldata[] = "select * from acs_plan_task_table;";
			char sqldata2[] = "select * from acs_climate_data_table ;";
			sqlite_get_record_data(ACS_CONFIG_DATEBASE,sqldata,get_record_callback,&sockfd);
			//send climate
			sqlite_get_record_data(ACS_CONFIG_DATEBASE,sqldata2,get_climate_callback,&sockfd);

			//send finish msg
			rc = send(sockfd,
						seliaze_protocal_data(finish_msg,strlen(finish_msg),info_sync,TEST_USER_ID),
						strlen(finish_msg)+PROTOCAL_FRAME_STABLE_LENGTH,
						MSG_NOSIGNAL
						);
			if(rc == -1)
			{
				 printf("sockfd is %d left as send failed 2 ",sockfd);
				 handle_socket_reconnection();
				 goto Reconnetion;

			}
		}
		else
		{
			//device controlacs_plan_task
			acs_handle_device_crontrol(sockfd,buffer);
		}
		TusSleep(10000); //set time interval
	}
}
