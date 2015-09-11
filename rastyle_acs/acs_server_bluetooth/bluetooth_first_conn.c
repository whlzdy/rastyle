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


static char init_request[] = "Initialize_request;";
static char acs_config_msg[] = "INFEX_COINF: SWVER=1514.564,HWVER=2514.325,"\
		"                   EquipmentID= qwierygd8123asdfkoe, Publickey= 5d8f8g5da5d6gda654842ddsa8eqt453ghv354a8efdsxsa51368efgdadwe8543;";
static char acs_hard_msg[] = "INFEX_CCHIF:ADOSK354KADSOAXO=First_level_filter, A5D8EFGDADXFGREF=Last_level_filter;";
static char acs_confim_msg[] = "Confirm;";
static char acs_wifi_msg[] = "INFPL_SLIST:Rastyle1| WPA2-PSK[AES]|-50dbm, Rastyle2| WPA-PSK[TKIP]|-70dbm;";

void bluetooth_first_connection(int bluetooth_fd,acs_bluetooth_get_frame_data tfn_get_frame_data)
{
	uint8_t  data[1024] = {0};
	uint16_t length  = 0;
    char buffer[1024] = {0};
	printf("init_request len is %d \n",strlen(init_request));
	tfn_get_frame_data(bluetooth_fd,data,&length);
	printf("acs bluetooth at first connection receive 1 len is %d message is %s \n",length,deseliaze_protocal_data(data,length));
#if 0
	memset(buffer,0,sizeof(buffer));
	memcpy(buffer,seliaze_protocal_data(init_request,(uint16_t)strlen(init_request),connection,TEST_USER_ID),strlen(init_request)+PROTOCAL_FRAME_STABLE_LENGTH);//delete end flag
	int ios;
	for(ios = 0;ios < strlen(init_request)+PROTOCAL_FRAME_STABLE_LENGTH;ios++)
	{
		printf("buffer ios index is %d data is %x \n ",ios,buffer[ios]);
	}
	//write(bluetooth_fd,buffer,20);
	//write(bluetooth_fd,buffer+20,strlen(init_request)+PROTOCAL_FRAME_STABLE_LENGTH-20);
	return;
#endif

	write(bluetooth_fd,
		  seliaze_protocal_data(init_request,(uint16_t)strlen(init_request),connection,TEST_USER_ID),
		   strlen(init_request)+PROTOCAL_FRAME_STABLE_LENGTH
		   );

	printf("acs bluetooth at first connection send init request message \n");
	memset(data,0,sizeof(data));
	tfn_get_frame_data(bluetooth_fd,data,&length);
	printf("acs bluetooth at first connection receive 2 len is %d message is %s \n",length,deseliaze_protocal_data(data,length));
	write(bluetooth_fd,
		    seliaze_protocal_data(acs_config_msg,(uint16_t)strlen(acs_config_msg),connection,TEST_USER_ID),
	        strlen(acs_config_msg)+PROTOCAL_FRAME_STABLE_LENGTH
		   );
	memset(data,0,sizeof(data));
	tfn_get_frame_data(bluetooth_fd,data,&length);
	printf("acs bluetooth at first connection receive 3 len is %d message is %s \n",length,deseliaze_protocal_data(data,length));
}
