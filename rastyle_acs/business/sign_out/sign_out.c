//*****************************************************************************
//  Copyright (C) 2015 rastyle¡£com
//	File name: sign_out.c
//	Author:	whl
//	Description: create rastyle connetion.c file
//	Others:
//	History: 2015-7-23 create by whl
//*****************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "../../protocal/protocal.h"


static char sign_out_msg[] = "Disconnect;";


void sign_out(int sockfd)
{
	printf("entry sign_out begin \n");
	send(sockfd,seliaze_pgrotocal_data(sign_out_msg,(uint16_t)strlen(sign_out_msg),connection,TEST_USER_ID),\
				  strlen(sign_out_msg)+PROTOCAL_FRAME_STABLE_LENGTH, 0);
	printf("exit sign_out end \n");
}



