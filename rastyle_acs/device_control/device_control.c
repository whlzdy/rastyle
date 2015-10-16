//*****************************************************************************
//  Copyright (C) 2015 rastyle。com
//	File name: device_control.h
//	Author:	whl
//	Description: create rastyle device_control。c file
//	Others:
//	History: 2015-8-24 create by whl
//*****************************************************************************
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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


#include "../message.h"
#include "../common_utils.h"
#include "../protocal/protocal.h"
#include "../hal/zigbee/sensor.h"
#include "../systemconfig.h"
#include "../hal/wind_motor/wind_motor.h"

static char confirm_msg[] = "Confirm;";
static char fail_msg[] = "Fail;";

extern uint8_t acs_client_mode; //0x01:正常模式 ;0x02:实时模式;0x04:异常模式




/*
*  handle to app/server device control order
*/
void acs_handle_device_crontrol(int sockfd,char * buffer)
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
			if(strcmp(token,"CONCL_PLMOD") == 0)
			{
				strcat(order_msg,"CONCL_PLMOD");
			}
			else if(strcmp(token,"CONCL_PLDEL") == 0)
			{
				strcat(order_msg,"CONCL_PLDEL");
			}
			else if(strcmp(token,"CONCL_PLADD") == 0)
			{
				printf("first CONCL_PLADD entry \n");
				strcat(order_msg,"CONCL_PLADD");
			}
			else if(strcmp(token,"CONCL_CLDEL")==0)
			{
				strcat(order_msg,"CONCL_CLDEL");
			}
			else if(strcmp(token,"CONCL_CLMOD")==0)
			{
				strcat(order_msg,"CONCL_CLMOD");
			}
			else if(strcmp(token,"CONCL_CLADD")==0)
			{
				strcat(order_msg,"CONCL_CLADD");
			}
			else if(strcmp(token,"CONCL_SMCHI")==0)
			{
				strcat(order_msg,"CONCL_SMCHI");
			}
			else if(strcmp(token,"CONCL_FANCR")==0)
			{
				strcat(order_msg,"CONCL_FANCR");
			}
			else if(strcmp(token,"CONCL_BAPCR")==0)
			{
				strcat(order_msg,"CONCL_BAPCR");
			}
			else if(strcmp(token,"CONCL_NAPCR")==0)
			{
				strcat(order_msg,"CONCL_NAPCR");
			}
			else if(strcmp(token,"CONCL_CLSEL")==0)
			{
				strcat(order_msg,"CONCL_CLSEL");
			}
			else if(strcmp(token,"CONCL_DTMRQ")==0)
			{
				printf("first CONCL_DTMRQ entry \n");
				strcat(order_msg,"CONCL_DTMRQ");
			}
			else if(strcmp(token,"CONAU_USADD") == 0)
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
				printf("acs received the wrong control message order !\n");
				rc = send(sockfd,
							seliaze_protocal_data(fail_msg,strlen(fail_msg),device_control,TEST_USER_ID),
							strlen(fail_msg)+PROTOCAL_FRAME_STABLE_LENGTH,
							MSG_NOSIGNAL);
				if(rc == -1)
				{
					 printf("sockfd is %d left as send failed 100 \n",sockfd);
					 close(sockfd);
				}
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
		else if(strcmp(order_msg,"CONCL_CLDEL") == 0 || strcmp(order_msg,"CONCL_PLDEL") == 0 || strcmp(order_msg,"CONAU_USDEL") == 0)
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
	//printf("data hanlde over end while \n");
	//handle data to sqlite
	if(strcmp(order_msg,"CONCL_PLMOD") == 0)
	{
		printf( "CONCL_PLMOD handle entry \n");
		if(strcmp(sensor_data_list[0].data,"realtime") == 0)
		{
			printf("app could not modify realtime plan task \n");
			return;
		}
		else
		{
			sprintf(sSQL,"update %s set Cycle = '%s',STATE = '%s',beginningtime = '%s', Stoptime = '%s' where Planname = '%s';",ACS_PLAN_TASK,sensor_data_list[1].data,\
					sensor_data_list[2].data,sensor_data_list[3].data,sensor_data_list[4].data,sensor_data_list[0].data);
		}
	}
	else if(strcmp(order_msg,"CONCL_PLDEL") == 0)
	{
		printf( "CONCL_PLDEL handle entry \n");
		//fprintf(stderr,"CONCL_PLDEL sensor_data_list[0].data is %s \n",sensor_data_list[0].data);
		if(strcmp(sensor_data_list[0].data,"realtime") == 0)
		{
			printf("app could not delete realtime plan task \n");
			return;
		}
		else
		{
			sprintf(sSQL,"delete from %s where Planname = '%s';",ACS_PLAN_TASK,sensor_data_list[0].data);
			//fprintf(stderr,"CONCL_PLDEL sql is %s \n",sSQL);
		}
	}
	else if(strcmp(order_msg,"CONCL_PLADD") == 0)
	{
		printf( "CONCL_PLADD handle entry \n");
		if(strcmp(sensor_data_list[0].data,"realtime") == 0)
		{
			printf("app could not create  realtime plan task \n");
			return;
		}
		else
		{
			//"create table acs_plan_task (Planname varchar(20) PRIMARY KEY,Cycle varchar(20),STATE varchar(20),beginningtime datetime ,Stoptime datetime);";
			sprintf(sSQL,"insert into %s (Planname,Cycle,STATE,beginningtime,Stoptime,Objectadd1,Objectadd2,Objectadd3)values('%s','%s','%s','%s','%s','0x001C','0x001D','0x001E');",ACS_PLAN_TASK,sensor_data_list[0].data, \
							sensor_data_list[1].data,sensor_data_list[2].data,sensor_data_list[3].data,sensor_data_list[4].data);
			//fprintf(stderr,"CONCL_PLADD sql is %s \n",sSQL);
		}
	}
	else if(strcmp(order_msg,"CONCL_CLDEL")==0)
	{
		printf( "CONCL_CLDEL handle entry \n");
		sprintf(sSQL,"delete from %s where Climatename = '%s';",ACS_CLIMATE_DATA,sensor_data_list[0].data);
		//acs_sqlite_exec_sql(ACS_CONFIG_DATEBASE,sSQL);
	}
	else if(strcmp(order_msg,"CONCL_CLMOD")==0)
	{
		printf( "CONCL_CLMOD handle entry \n");
		sprintf(sSQL,"update %s set CO2Concentration = '%s',Temperature = '%s', Humidity = '%s' where Climatename = '%s';",ACS_CLIMATE_DATA,sensor_data_list[1].data,\
						sensor_data_list[2].data,sensor_data_list[3].data,sensor_data_list[0].data);
		//acs_sqlite_exec_sql(ACS_CONFIG_DATEBASE,sSQL);
	}
	else if(strcmp(order_msg,"CONCL_CLADD")==0)
	{
		printf( "CONCL_CLADD handle entry \n");
		// "create table acs_climate_data_table (Climatename varchar(20) PRIMARY KEY,CO2Concentration varchar(20),Temperature varchar(20),Humidity varchar(20));";
		sprintf(sSQL,"insert into %s values('%s','%s','%s','%s');",ACS_CLIMATE_DATA,sensor_data_list[0].data, \
				sensor_data_list[1].data,sensor_data_list[2].data,sensor_data_list[3].data);
		//fprintf(stderr,"CONCL_CLADD sql is %s \n",sSQL);
		//acs_sqlite_exec_sql(ACS_CONFIG_DATEBASE,sSQL);
	}
	else if(strcmp(order_msg,"CONCL_SMCHI")==0)
	{
		//CONCL_SMCHI: Planname=plantest1,STATE=ACTIVE;
		printf("CONCL_SMCHI handle entry \n");
		sprintf(sSQL,"update %s set SMCSTATE = '%s' where Planname = '%s';",ACS_PLAN_TASK,sensor_data_list[1].data,sensor_data_list[0].data);
		//acs_sqlite_exec_sql(ACS_CONFIG_DATEBASE,sSQL);

	}
	else if(strcmp(order_msg,"CONCL_FANCR")==0)
	{
		//Objectadd1 volume1
		printf( "CONCL_FANCR handle entry \n");
	   //"create table acs_wind_addr_table (Planname varchar(20) PRIMARY KEY,Objectaddr1 integer,volume1 integer,"\
				"Objectaddr2 integer,volume2 integer,Objectaddr3 integer,volume3 integer);";

		sprintf(sSQL,"update  %s set volume1 = '%s' where Planname = '%s' and Objectadd1 = '%s';",ACS_PLAN_TASK, \
				sensor_data_list[2].data,sensor_data_list[0].data,sensor_data_list[1].data);
		//printf("CONCL_FANCR sql is %s \n",sSQL);
		//acs_sqlite_exec_sql(ACS_CONFIG_DATEBASE,sSQL);
		//control wind motor
	    if(strcmp(sensor_data_list[0].data,"realtime") == 0)
	    {
	    	int rate = atoi(sensor_data_list[2].data);
	    	printf("control wind motor is %d \n",rate);
	    	acs_wind_motor_change_rate((uint8_t)rate);
	    }

	}
	else if(strcmp(order_msg,"CONCL_BAPCR")==0)
	{
		//Objectadd3 volume3
		printf( "CONCL_BAPCR handle entry \n");
	   //"create table acs_wind_addr_table (Planname varchar(20) PRIMARY KEY,Objectaddr1 integer,volume1 integer,"\
				"Objectaddr2 integer,volume2 integer,Objectaddr3 integer,volume3 integer);";
		sprintf(sSQL,"update  %s set volume3 = '%s' where Planname = '%s' and  Objectadd3 = '%s';",ACS_PLAN_TASK, \
				sensor_data_list[2].data,sensor_data_list[0].data,sensor_data_list[1].data);
		//acs_sqlite_exec_sql(ACS_CONFIG_DATEBASE,sSQL);
		//control wind motor
		if(strcmp(sensor_data_list[0].data,"realtime") == 0)
		{
			int rate = atoi(sensor_data_list[2].data);
			printf("control wind motor is %d \n",rate);
			acs_wind_motor_change_rate((uint8_t)rate);
		}

	}
	else if(strcmp(order_msg,"CONCL_NAPCR")==0)
	{
		//Objectadd2 volume2
		printf( "CONCL_NAPCR handle entry \n");
	    //"create table acs_wind_addr_table (Planname varchar(20) PRIMARY KEY,Objectaddr1 integer,volume1 integer,"\
				"Objectaddr2 integer,volume2 integer,Objectaddr3 integer,volume3 integer);";
		sprintf(sSQL,"update  %s set volume2 = '%s' where Planname = '%s' and  Objectadd2 = '%s';",ACS_PLAN_TASK, \
				sensor_data_list[2].data,sensor_data_list[0].data,sensor_data_list[1].data);
		//acs_sqlite_exec_sql(ACS_CONFIG_DATEBASE,sSQL);
		//control wind motor
		if(strcmp(sensor_data_list[0].data,"realtime") == 0)
		{
			int rate = atoi(sensor_data_list[2].data);
			printf("control wind motor is %d \n",rate);
			acs_wind_motor_change_rate((uint8_t)rate);
		}

	}
	else if(strcmp(order_msg,"CONCL_CLSEL")==0)
	{
		printf( "CONCL_CLSEL handle entry \n");
		//CONCL_CLSEL: Planname=plantest1,Climatename=三亚;
		sprintf(sSQL,"update  %s set Climatename = '%s' where Planname = '%s';",ACS_PLAN_TASK,sensor_data_list[1].data,sensor_data_list[0].data);
		//acs_sqlite_exec_sql(ACS_CONFIG_DATEBASE,sSQL);
	}
	else if(strcmp(order_msg,"CONCL_DTMRQ")==0)
	{
		printf( "CONCL_DTMRQ handle entry \n");
		rc = send(sockfd,
					seliaze_protocal_data(confirm_msg,strlen(confirm_msg),real_time,TEST_USER_ID),
					strlen(confirm_msg)+PROTOCAL_FRAME_STABLE_LENGTH,
					MSG_NOSIGNAL);
		if(rc == -1)
		{
			 printf("sockfd is %d left as send failed 101 \n ",sockfd);
			 close(sockfd);
		}
        if(strcmp(sensor_data_list[1].data,"ACTIVE") == 0)
        {
        	acs_client_mode |= 0x02; //0x01:正常模式 ;0x02:实时模式;0x04:异常模式
        }
        else if(strcmp(sensor_data_list[1].data,"INACTIVE") == 0)
        {
        	acs_client_mode &= 0x01; //bug
        }
		return;
	}
	else if(strcmp(order_msg,"CONAU_USADD") == 0)
	{
		printf( "CONAU_USADD handle entry \n");
		sprintf(sSQL,"insert into %s values('%s','%s','%s','%s','%s');",ACS_USER_DATA,sensor_data_list[1].data, \
						sensor_data_list[0].data,sensor_data_list[2].data,sensor_data_list[3].data,sensor_data_list[4].data);
		//fprintf(stderr,"CONAU_USADD sql is %s \n",sSQL);
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
		//fprintf(stderr,"CONAU_USMOD sql is %s \n",sSQL);
	}
	else if(strcmp(order_msg,"CONAU_USDEL") == 0)
	{
		printf( "CONAU_USDEL handle entry \n");
		sprintf(sSQL,"delete from %s where Username = '%s';",ACS_USER_DATA,sensor_data_list[0].data);
		//fprintf(stderr,"CONAU_USDEL sql is %s \n",sSQL);
	}
	acs_sqlite_exec_sql(ACS_CONFIG_DATEBASE,sSQL);
	//printf("acs receive app data is %s \n",deseliaze_protocal_data((uint8_t *)recv_msg,recvbytes));
	rc = send(sockfd,
			seliaze_protocal_data(confirm_msg,strlen(confirm_msg),device_control,TEST_USER_ID),
			strlen(confirm_msg)+PROTOCAL_FRAME_STABLE_LENGTH,
			MSG_NOSIGNAL);
	if(rc == -1)
	{
		 printf("sockfd is %d left as send failed 102 ",sockfd);
		 close(sockfd);
	}
}
