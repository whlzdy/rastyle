//*****************************************************************************
//  Copyright (C) 2015 rastyle¡£com
//	File name: sensor_protocol.h
//	Author:	whl
//	Description: create rastyle sensor.h  used to read sensor data
//	Others:
//	History: 2015-8-4 create by whl
//*****************************************************************************

#ifndef SENSOR_HH
#define SENSOR_HH



typedef void (*acs_client_normal_handle)  (int sockfd,char *data,int length);
typedef void (*acs_client_abnormal_handle)(int sockfd,char *data,int length);
typedef void (*acs_client_realtime_handle)(int sockfd,char *data,int length);
typedef void (*acs_server_realtime_handle)(int sockfd,char *data,int length);


#define SENSOR_MAX_CHAR_COUNT   100
#define MAX_APP_CONN_NUM 20

typedef struct _acs_server_fd
{
	int fd;
	acs_server_realtime_handle tfnrealtime_handle;
}tACS_server_fd_msg;

typedef struct _acs_server_msg_list
{
	tACS_server_fd_msg app_conn_list[MAX_APP_CONN_NUM];
	int app_conn_count;
}tACS_server_fd_msg_list;


typedef struct _sensor_data
{
	char name[SENSOR_MAX_CHAR_COUNT];
	char data[SENSOR_MAX_CHAR_COUNT];
}tSensor_data;


void acs_client_bind2_sensor(int sockfd,acs_client_normal_handle normal_handle,acs_client_abnormal_handle abnormal_handle,acs_client_realtime_handle realtime_handle);

void acs_server_bind2_sensor(int sockfd,acs_server_realtime_handle tfnrealtime_handle);
/*
* acs_read_sensor_thread
*/
void *acs_read_sensor_thread(void *args);




#endif

