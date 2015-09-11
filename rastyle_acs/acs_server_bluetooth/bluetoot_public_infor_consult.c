//*****************************************************************************
//  Copyright (C) 2015 rastyle¡£com
//	File name: acs_bluetooth.c
//	Author:	whl
//	Description: create rastyle acs_bluetooth.c file
//	Others:
//	History: 2015-8-10 create by whl
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
#include "../protocal/protocal.h"
#include "acs_bluetooth.h"


static char confirm_msg[] = "Confirm;";
static char time_out_msg[] = "INFEX_MRMDA:TIMDL=5;";
static char period_msg[] = "INFEX_DRPER:PERIOD=15;";
static char user_limit_msg[] = "INFEX_OAREQ:role=Administrator;";
static char sensor_data_rule[] = "INFEX_SERCF:´óÌüCO2= Indoor_CO2|5000|0|ppm, ´óÌü¼×È©= Indoor_HCHO|80|0|mg/m3, "\
		" ´óÌüPM2.5= Indoor_PM2.5|300|0|¦Ìg/m3, ´óÌüPM0.3= Indoor_PM0.3|300|0|¦Ìg/m3, ´óÌüVOC= Indoor_VOC|3|0|NULL, "\
		"´óÌüÎÂ¶È= Indoor_temp|100|-50|¡ãC, ´óÌüÊª¶È= Indoor_Humidity|100|0|%,´óÌüPM0.3_2= Indoor_PM0.3|300|0|¦Ìg/m3,ÊÒÍâPM2.5= "\
		"Outdoor_PM2.5|300|0|¦Ìg/m3, ÊÒÍâPM0.3= Outdoor_PM0.3|300|0|¦Ìg/m3;";


void bluetooth_public_infor_consult(int bluetooth_fd,acs_bluetooth_get_frame_data tfn_get_frame_data)
{
	uint8_t data[1024] = {0};
	uint16_t length  = 0;
	write(bluetooth_fd,
		   seliaze_protocal_data(time_out_msg,(uint16_t)strlen(time_out_msg),public_consult,TEST_USER_ID),
		   strlen(time_out_msg)+PROTOCAL_FRAME_STABLE_LENGTH
		   );
	memset(data,0,1024);
	tfn_get_frame_data(bluetooth_fd,data,&length);
	printf("acs bluetooth at public infor receive 1 len is %d message is %s \n",length,deseliaze_protocal_data(data,length));
	write(bluetooth_fd,
		   seliaze_protocal_data(period_msg,(uint16_t)strlen(period_msg),public_consult,TEST_USER_ID),
		   strlen(period_msg)+PROTOCAL_FRAME_STABLE_LENGTH
		   );
	memset(data,0,1024);
	tfn_get_frame_data(bluetooth_fd,data,&length);
	printf("acs bluetooth at public infor receive 2 len is %d message is %s \n",length,deseliaze_protocal_data(data,length));
	write(bluetooth_fd,
			   seliaze_protocal_data(user_limit_msg,(uint16_t)strlen(user_limit_msg),public_consult,TEST_USER_ID),
			   strlen(user_limit_msg)+PROTOCAL_FRAME_STABLE_LENGTH
			   );
	memset(data,0,1024);
	tfn_get_frame_data(bluetooth_fd,data,&length);
	printf("acs bluetooth at public infor receive 3 len is %d message is %s \n",length,deseliaze_protocal_data(data,length));
	write(bluetooth_fd,
			   seliaze_protocal_data(sensor_data_rule,(uint16_t)strlen(sensor_data_rule),public_consult,TEST_USER_ID),
			   strlen(sensor_data_rule)+PROTOCAL_FRAME_STABLE_LENGTH
			   );
	memset(data,0,1024);
	tfn_get_frame_data(bluetooth_fd,data,&length);
	printf("acs bluetooth at public infor receive 4 len is %d message is %s \n",length,deseliaze_protocal_data(data,length));
}
