//*****************************************************************************
//  Copyright (C) 2015 rastyle¡£com
//	File name: acs_user_update.c
//	Author:	whl
//	Description: create rastyle acs_user_update.c
//	Others:
//	History: 2015-9-6 create by whl
//*****************************************************************************
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "../protocal/protocal.h"
#include "../systemconfig.h"
#include "../hal/zigbee/sensor.h"


static char acs_update_request_msg[] = "Apply_for_UserInf_Update;";       //use to begin update user table
static char acs_update_finish_flag[] = "User_Information_Update_Finish;"; //user update finish flag from cloud
static char acs_update_confim_msg[]  = "Confirm;";
static char acs_update_fail_msg[]    = "Fail;";



/*
 * acs_handle_user_update_message
*/
static void acs_handle_user_update_message(int sockfd,char * buffer)
{
	int rc;
	char sSQL[1024] = {0};
    int first_flag = 1;
	tSensor_data sensor_data_list[10] = {0};
	char order_msg[20] = {0};
	int sensor_count = 0;
	char *buf;
	char *token,*token2,*name;
	buf = buffer;
	while((token = strsep(&buf, ":")) != NULL)
	{
		if(first_flag)
		{
			printf("%s\n", token);
			//judge order type first
			if(strcmp(token,"CONAU_USADD") == 0)
			{
				strcat(order_msg,"CONAU_USADD");
			}
			else if(strcmp(token,"CONAU_USMOD") == 0)
			{
				strcat(order_msg,"CONAU_USMOD");
			}
			else if(strcmp(token,"CONAU_USDEL") == 0)
			{
				strcat(order_msg,"CONAU_USDEL");
			}
			else
			{
				printf("acs received the wrong cloud handle user order \n");
				return;
			}
			first_flag = 0;
		}
		if(strstr(token,","))
		{
			while((token2 = strsep(&token, ",")) != NULL)
			{
				//printf("%s\n", token2);
				name = strsep(&token2,"=");
				if(strstr(token2,";"))
				{
					token2 = strsep(&token2,";");
				}
				memcpy(sensor_data_list[sensor_count].name,name,strlen(name));
				memcpy(sensor_data_list[sensor_count].data,token2,strlen(token2));
				sensor_count++;
				//fprintf(stderr, "CGI[name ] :%s\n", name);
				//fprintf(stderr, "CGI[value] :%s\n", token2);
			}
		}
		else if(strcmp(order_msg,"CONAU_USDEL") == 0)
		{
			//printf("%s\n", token);
			if(strstr(token,"="))
			{
				name = strsep(&token, "=");
				token2 = strsep(&token,";");
				memcpy(sensor_data_list[sensor_count].name,name,strlen(name));
				memcpy(sensor_data_list[sensor_count].data,token2,strlen(token2));
				sensor_count++;
				//fprintf(stderr, "CGI[name ] :%s\n", name);
				//fprintf(stderr, "CGI[value] :%s\n", token2);
			}
		}

	}
	printf("data hanlde over end while \n");
	//handle data to sqlite
	if(strcmp(order_msg,"CONAU_USADD") == 0)
	{
		printf( "CONAU_USADD handle entry \n");
		sprintf(sSQL,"insert into %s values('%s','%s','%s','%s','%s','%s');",ACS_USER_DATA,sensor_data_list[2].data, \
						sensor_data_list[0].data,sensor_data_list[1].data,sensor_data_list[3].data,sensor_data_list[4].data,sensor_data_list[5].data);
		fprintf(stderr,"CONAU_USADD sql is %s \n",sSQL);
	}
	else if(strcmp(order_msg,"CONAU_USMOD") == 0)
	{
		printf( "CONAU_USMOD handle entry \n");
		if(strcmp(sensor_data_list[1].name,"authorization") == 0)
		{
			sprintf(sSQL,"update %s set authorization = '%s',isop = '%s' where Username = '%s';",ACS_USER_DATA,sensor_data_list[1].data,\
											sensor_data_list[2].data,sensor_data_list[0].data);
		}
		else if(strcmp(sensor_data_list[1].name,"PWD") == 0)
		{
			sprintf(sSQL,"update %s set PWD = '%s',Publickey = '%s' where Username = '%s';",ACS_USER_DATA,sensor_data_list[1].data,\
											sensor_data_list[2].data,sensor_data_list[0].data);
		}
		fprintf(stderr,"CONAU_USMOD sql is %s \n",sSQL);
	}
	else if(strcmp(order_msg,"CONAU_USDEL") == 0)
	{
		printf( "CONAU_USDEL handle entry \n");
		sprintf(sSQL,"delete from %s where Username = '%s';",ACS_USER_DATA,sensor_data_list[0].data);
		fprintf(stderr,"CONAU_USDEL sql is %s \n",sSQL);
	}

	acs_sqlite_exec_sql(ACS_CONFIG_DATEBASE,sSQL);

}


/*
 * acs update user information from cloud
*/
void acs_update_user_from_cloud(int sockfd)
{
	int rc;
	int recvbytes,packlength;
	char recv_msg[1024] = {0};
	char buffer[65536]  = {0};
	printf("acs client is update user from cloud begin ...\n");
	memset(recv_msg,0,1024);
	rc = send(sockfd,
			  seliaze_protocal_data(acs_update_request_msg,strlen(acs_update_request_msg),public_consult,TEST_USER_ID),
			  strlen(acs_update_request_msg)+PROTOCAL_FRAME_STABLE_LENGTH,
			  MSG_NOSIGNAL);
	if(rc == -1)
	{
		printf("223354 \n");
		return;

	}
	while(1)
	{
		memset(recv_msg,0,1024);
		recvbytes = acs_tcp_receive(sockfd, recv_msg,&packlength);
		if(recvbytes == -1 )
		{
			perror("acs receved cloud update user message ack failed \n");
			return;
		}
		if(packlength < 0)
		{
			return;
		}
		memset(buffer,0,65536);
		memcpy(buffer,deseliaze_protocal_data(recv_msg,recvbytes),strlen(deseliaze_protocal_data(recv_msg,recvbytes))-1);
		printf("cloud update user table control message is %s len is %d\n",buffer,strlen(buffer));
		if(strcmp(buffer,"User_Information_Update_Finish;") == 0)
		{
			printf("acs client update user sucess and end \n");
			break;
		}
		else
		{
			acs_handle_user_update_message(sockfd,buffer);
		}
		printf("acs client is update user from cloud end !\n");
	}

}
