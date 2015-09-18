//*****************************************************************************
//  Copyright (C) 2015 rastyle。com
//	File name: sensor_protocol.h
//	Author:	whl
//	Description: create rastyle sensor.c  used to read sensor data
//	Others:
//	History: 2015-8-4 create by whl
//*****************************************************************************

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/stat.h>	   //for file operate
#include <fcntl.h>
#include <signal.h>
#include <sys/syscall.h>
#include <linux/unistd.h>
#include <sys/signal.h>
#include <sys/msg.h>
#include <termios.h>
#include <sys/select.h>
#include <linux/serial.h>
#include <time.h>

#include "sensor.h"
#include "../../systemconfig.h"
#include "../../protocal/sensor_protocol.h"
#include "../../common_utils.h"
#include "../../sqlite/sqlite.h"
#include "../inter_sensor/inter_sensor.h"


#define READ_SENSOR_SLEEP_TIME  500

extern int zigbee_fd;

extern uint8_t acs_client_mode;

acs_client_abnormal_handle tfnabnormal_handle;
acs_client_realtime_handle tfnrealtime_handle;
acs_client_normal_handle tfnnormal_handle;

static int acs_client_fd;

int acs_normal_report_inteval = 25;//s
int acs_real_time_inteval = 30;    //s



 tACS_server_fd_msg_list acs_app_handle_list = {0};  //used to handle app report normal data


void acs_client_bind2_sensor(int sockfd,acs_client_normal_handle normal_handle,acs_client_abnormal_handle abnormal_handle,acs_client_realtime_handle realtime_handle)
{
	printf("acs client register sensor callback \n");
	acs_client_fd  = sockfd;
	tfnabnormal_handle = abnormal_handle;
	tfnrealtime_handle = realtime_handle;
	tfnnormal_handle = normal_handle;
}

void acs_server_bind2_sensor(int sockfd,acs_server_realtime_handle tfnrealtime_handle)
{
	printf("sockfd is % d acs_server_bind2_sensor entry \n",sockfd);
	acs_app_handle_list.app_conn_list[acs_app_handle_list.app_conn_count].fd = sockfd;
	acs_app_handle_list.app_conn_list[acs_app_handle_list.app_conn_count].tfnrealtime_handle = tfnrealtime_handle;
	acs_app_handle_list.app_conn_count ++;
}



/*
 * serilaze frame data
*/
int serilaze_app_sensor_data (char *data_msg,long datetime,char * header)
{
	 int i,offset = 0;
	 char tmp[100] = {0};
	 strcat(data_msg+offset,header);
	 offset += strlen(header);

     memset(tmp,0,100);
	 sprintf(tmp,"%s=%d,", "datetime",datetime);
	 strcat(data_msg+offset,tmp);
	 offset+=strlen(tmp);

     memset(tmp,0,100);
	 sprintf(tmp,"%s=%d,", "温度",rand()%40+1);
	 strcat(data_msg+offset,tmp);
	 offset+=strlen(tmp);

     memset(tmp,0,100);
	 sprintf(tmp,"%s=%d,", "湿度",rand()%100+1);
	 strcat(data_msg+offset,tmp);
	 offset+=strlen(tmp);

     memset(tmp,0,100);
	 sprintf(tmp,"%s=%d,", "CO2",rand()%60+1);
	 strcat(data_msg+offset,tmp);
	 offset+=strlen(tmp);

     memset(tmp,0,100);
	 sprintf(tmp,"%s=%d,", "PM2_5",rand()%100+1);
	 strcat(data_msg+offset,tmp);
	 offset+=strlen(tmp);

     memset(tmp,0,100);
	 sprintf(tmp,"%s=%d", "甲醛",rand()%90+1);
	 strcat(data_msg+offset,tmp);
	 offset+=strlen(tmp);

	 strcat(data_msg+offset,";");
	 return 0;
}

/*
 * serilaze frame data
*/
static int serilaze_inter_sensor_data (
		char *data_msg,
		uint8_t tem,
		uint8_t humidity,
		uint16_t pm1_0_1,
		uint16_t pm2_5_1,
		uint16_t pm10_1,
		uint16_t pm1_0_2,
		uint16_t pm2_5_2,
		uint16_t pm10_2,
		uint16_t  indoor_tmp,
		uint16_t  indoor_humidity,
        uint8_t   voc,
		float indoor_pm2_5,
		float cO2,
		float hcho,
		long datetime,
		char * header
		 )
{
	 int i,offset = 0;
	 char tmp[100] = {0};
	 strcat(data_msg+offset,header);
	 offset += strlen(header);

     memset(tmp,0,100);
	 sprintf(tmp,"%s=%d,", "datetime",datetime);
	 strcat(data_msg+offset,tmp);
	 offset+=strlen(tmp);

     memset(tmp,0,100);
	 sprintf(tmp,"%s=%d,", "室外温度",tem);
	 strcat(data_msg+offset,tmp);
	 offset+=strlen(tmp);

     memset(tmp,0,100);
	 sprintf(tmp,"%s=%d,", "室外湿度",humidity);
	 strcat(data_msg+offset,tmp);
	 offset+=strlen(tmp);

     memset(tmp,0,100);
	 sprintf(tmp,"%s=%d,", "室外PM1.0",pm1_0_1);
	 strcat(data_msg+offset,tmp);
	 offset+=strlen(tmp);

     memset(tmp,0,100);
	 sprintf(tmp,"%s=%d,", "室外PM2.5",pm2_5_1);
	 strcat(data_msg+offset,tmp);
	 offset+=strlen(tmp);

     memset(tmp,0,100);
	 sprintf(tmp,"%s=%d,", "室外PM10",pm10_1);
	 strcat(data_msg+offset,tmp);
	 offset+=strlen(tmp);

     memset(tmp,0,100);
	 sprintf(tmp,"%s=%d,", "室外PM1.0(大气)",pm1_0_2);
	 strcat(data_msg+offset,tmp);
	 offset+=strlen(tmp);

     memset(tmp,0,100);
	 sprintf(tmp,"%s=%d,", "室外大厅PM2.5(大气)",pm2_5_2);
	 strcat(data_msg+offset,tmp);
	 offset+=strlen(tmp);

     memset(tmp,0,100);
	 sprintf(tmp,"%s=%d,", "室外大厅PM10(大气)",pm10_2);
	 strcat(data_msg+offset,tmp);
	 offset+=strlen(tmp);

     memset(tmp,0,100);
	 sprintf(tmp,"%s=%d,", "室内温度",indoor_tmp);
	 strcat(data_msg+offset,tmp);
	 offset+=strlen(tmp);

     memset(tmp,0,100);
	 sprintf(tmp,"%s=%d,", "室内湿度",indoor_humidity);
	 strcat(data_msg+offset,tmp);
	 offset+=strlen(tmp);

     memset(tmp,0,100);
	 sprintf(tmp,"%s=%d,", "室内VOC",voc);
	 strcat(data_msg+offset,tmp);
	 offset+=strlen(tmp);

     memset(tmp,0,100);
	 sprintf(tmp,"%s=%1.f,", "室内PM2.5",indoor_pm2_5);
	 strcat(data_msg+offset,tmp);
	 offset+=strlen(tmp);

     memset(tmp,0,100);
	 sprintf(tmp,"%s=%d,", "室内CO2",(int)cO2);
	 strcat(data_msg+offset,tmp);
	 offset+=strlen(tmp);

     memset(tmp,0,100);
	 sprintf(tmp,"%s=%1.3f", "室内甲醛",hcho);
	 strcat(data_msg+offset,tmp);
	 offset+=strlen(tmp);

	 strcat(data_msg+offset,";");
	 return 0;
}

/*
 * read specific sensor data
*/
void *acs_read_sensor_thread(void *args)
{
	int errno,i,offset;
    uint8_t tem,humidity;
    uint16_t pm1_0_1,pm2_5_1,pm10_1,pm1_0_2,pm2_5_2,pm10_2;
	uint8_t  buffer[2]  = {0};
	uint8_t  receive_flag = 0 ,discard_count = 0;
	int sensor_fd,ret;
	struct timeval tv;
    time_t now;
    time_t last1,last2,last3;
    uint8_t flag1 = 1,flag2 = 1,flag3 = 1,flag4 = 1;
    struct tm *tm_now;
    char datetime[200];
    char sensor_data[1024];
    char sensor_data_2[1024];
    char sensor_data_3[1024];
    uint16_t  indoor_tmp = 29,last_indoor_tmp;
    uint16_t  indoor_humidity = 54,last_indoor_humidity;
	uint8_t   voc = 3,last_voc;
	float indoor_pm2_5 = 58,last_indoor_pm2_5;
	float cO2 = 56,last_cO2;
	float hcho = 28,last_hcho;
	//read zigbee data
    //sensor_fd = zigbee_fd;
	//printf("acs is read sensor is %s  \n",ZIGBEE);
	//tv.tv_sec  = SENSOR_MAX_WAIT_TIME;
	//tv.tv_usec = 0;
	offset = 0;
	while(1)
	{
		time(&now);
		//printf("now is %d \n ",now);
		//tm_now = localtime(&now);
		//printf("tm_now is %d \n ",tm_now);
		//strftime(datetime, 200, "%Y-%m-%d %H.%M.%S", tm_now);
		//printf("now datetime : %s\n", datetime);

		memset(sensor_data_2,0,1024);
		memset(sensor_data_3,0,1024);
#if 0
		serilaze_app_sensor_data (sensor_data_2,now,"DISPL_RTDMS:");
		serilaze_app_sensor_data(sensor_data_3,now,"DISPL_NMDMS:");
#endif
		acs_get_inter_temperature_and_humidity(&tem,&humidity);
		usleep(50000);
#if 1
		ret = acs_get_zigbee_tmp_and_humidity(
			  & indoor_tmp,
		      & indoor_humidity
			  );
		if(ret == 0)
		{
			last_indoor_tmp = indoor_tmp;
			last_indoor_humidity = indoor_humidity;
		}
		ret = acs_get_zigbee_pm_2_5(
				&voc,
				&indoor_pm2_5
				);
		if(ret == 0)
		{
			last_voc = voc;
			last_indoor_pm2_5 = indoor_pm2_5;
		}
		ret = acs_get_zigbee_cO2(&cO2);
		if(ret == 0)
		{
			last_cO2 = cO2;
		}
		ret = acs_get_zigbee_hcho(&hcho);
		if(ret == 0)
		{
			last_hcho = hcho;
		}
#endif
		//printf("11111 \n");
		acs_get_inter_laser_pm2_5(&pm1_0_1,&pm2_5_1,&pm10_1,&pm1_0_2,&pm2_5_2,&pm10_2);
		//printf("2222222 \n");
		serilaze_inter_sensor_data(sensor_data_2,tem,humidity,pm1_0_1,pm2_5_1,pm10_1,pm1_0_2,pm2_5_2,pm10_2,\
				last_indoor_tmp,last_indoor_humidity,last_voc,last_indoor_pm2_5,last_cO2,last_hcho,now,"DISPL_RTDMS:");
		//printf("3333333 \n");
		serilaze_inter_sensor_data(sensor_data_3,tem,humidity,pm1_0_1,pm2_5_1,pm10_1,pm1_0_2,pm2_5_2,pm10_2,\
				last_indoor_tmp, last_indoor_humidity,last_voc,last_indoor_pm2_5,last_cO2,last_hcho,now,"DISPL_NMDMS:");
		//printf("%s \n",sensor_data_2);
		//printf("acs_client_mode is %d in sensor  \n ",acs_client_mode);
		//printf("4444444 \n");
		if(acs_client_mode & 0x01)
		{
			if(flag1 || difftime(now,last1) >= acs_normal_report_inteval)
			{
				flag1 = 0;
				//normal mode
				tfnnormal_handle(acs_client_fd,sensor_data_3,strlen(sensor_data_3));
				memcpy(&last1,&now,sizeof(now));
			}
		}
	    if(acs_client_mode >> 1 )
		{
			if(flag2 || difftime(now,last2) >= acs_real_time_inteval)
			{
				//realtime mode
				tfnrealtime_handle(acs_client_fd,sensor_data_2,strlen(sensor_data_2));
				memcpy(&last2,&now,sizeof(now));
			}
		}
	    if(acs_client_mode  >> 2)
		{
			//abnormal mode
			tfnabnormal_handle(acs_client_fd,NULL,0);
		}
		//handle server realtime mode
		int i ;
		//printf("fasdfsa,app count is %d \n",acs_app_handle_list.app_conn_count);
		for(i = 0;i<acs_app_handle_list.app_conn_count;i++)
		{
			//report data to app
			acs_app_handle_list.app_conn_list[i].tfnrealtime_handle(acs_app_handle_list.app_conn_list[i].fd,sensor_data_2,strlen(sensor_data_2));
		}
		//printf("sensor thread is survive \n");
		TSleep(5);
	}
}
