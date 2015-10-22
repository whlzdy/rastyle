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

#include "../systemconfig.h"


static char init_request[] = "Initialize_request;";
//static char acs_config_msg[] = "INFEX_COINF: SWVER=1514.564,HWVER=2514.325,"\
//		"                   EquipmentID= qwierygd8123asdfkoe, Publickey= 5d8f8g5da5d6gda654842ddsa8eqt453ghv354a8efdsxsa51368efgdadwe8543;";
//static char acs_hard_msg[] = "INFEX_CCHIF:ADOSK354KADSOAXO=First_level_filter, A5D8EFGDADXFGREF=Last_level_filter;";
//static char acs_confim_msg[] = "Confirm;";
//static char acs_wifi_msg[] = "INFPL_SLIST:Rastyle1| WPA2-PSK[AES]|-50dbm, Rastyle2| WPA-PSK[TKIP]|-70dbm;";

static char build_acs_config_msg(char * acs_config_msg)
{
	char tmp[100] = {0};
	strcat(acs_config_msg,"INFEX_COINF:");
	sprintf(tmp,"SWVER=%s,",SOFT_VERISION);
	strcat(acs_config_msg,tmp);
	memset(tmp,0,100);
	sprintf(tmp,"HWVER=%s,",HARD_VERISION);
	strcat(acs_config_msg,tmp);
	memset(tmp,0,100);
	sprintf(tmp,"EquipmentID=%s,",DEVICE_ID);
	strcat(acs_config_msg,tmp);
	memset(tmp,0,100);
	sprintf(tmp,"Publickey=%s;",ACS_PUBLIC_KEY);
	strcat(acs_config_msg,tmp);
}

void bluetooth_first_connection(int bluetooth_fd,acs_bluetooth_get_frame_data tfn_get_frame_data)
{
	char acs_config_msg[1024] = {0};
	uint8_t  data[1024] = {0};
	uint16_t length  = 0;
    char buffer[1024] = {0};
    int ret;
	printf("bluetooth_first_connection \n");
	while((ret = tfn_get_frame_data(bluetooth_fd,data,&length)) < 0)
	{
		TusSleep(50000);
	}
	printf("acs bluetooth at first connection receive 1 len is %d message is %s \n",length,deseliaze_protocal_data(data,length));
	write(bluetooth_fd,
		  seliaze_protocal_data(init_request,(uint16_t)strlen(init_request),connection,TEST_USER_ID),
		   strlen(init_request)+PROTOCAL_FRAME_STABLE_LENGTH
		   );
	printf("acs bluetooth at first connection send init request message \n");
	memset(data,0,sizeof(data));
	while((ret= tfn_get_frame_data(bluetooth_fd,data,&length)) < 0 )
	{
		TusSleep(50000);
	}
	printf("acs bluetooth at first connection receive 2 len is %d message is %s \n",length,deseliaze_protocal_data(data,length));
	build_acs_config_msg(acs_config_msg);
	write(bluetooth_fd,
		    seliaze_protocal_data(acs_config_msg,(uint16_t)strlen(acs_config_msg),connection,TEST_USER_ID),
	        strlen(acs_config_msg)+PROTOCAL_FRAME_STABLE_LENGTH
		   );
	memset(data,0,sizeof(data));
	while((ret = tfn_get_frame_data(bluetooth_fd,data,&length)) < 0)
	{
		TusSleep(50000);
	}
	printf("acs bluetooth at first connection receive 3 len is %d message is %s \n",length,deseliaze_protocal_data(data,length));
}
