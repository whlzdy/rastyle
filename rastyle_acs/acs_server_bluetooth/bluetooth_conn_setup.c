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

static char username_msg[] = "UserName;";
static char confim_msg[] = "Confirm;";

void bluetooth_connection_setup(int bluetooth_fd,acs_bluetooth_get_frame_data tfn_get_frame_data)
{
	uint8_t data[1024] = {0};
	uint16_t length  = 0;
	tfn_get_frame_data(bluetooth_fd,data,&length);
	printf("acs bluetooth at connection setup receive 1 len is %d message is %s \n",length,deseliaze_protocal_data(data,length));
	write(bluetooth_fd,
		   seliaze_protocal_data(username_msg,(uint16_t)strlen(username_msg),connection,TEST_USER_ID),
		   strlen(username_msg)+PROTOCAL_FRAME_STABLE_LENGTH
		   );
	memset(data,0,1024);
	tfn_get_frame_data(bluetooth_fd,data,&length);
	printf("acs bluetooth at connection setup receive 2 len is %d message is %s \n",length,deseliaze_protocal_data(data,length));
	write(bluetooth_fd,
		   seliaze_protocal_data(confim_msg,(uint16_t)strlen(confim_msg),connection,TEST_USER_ID),
		   strlen(confim_msg)+PROTOCAL_FRAME_STABLE_LENGTH
		   );

}
