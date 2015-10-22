//*****************************************************************************
//  Copyright (C) 2015 rastyle¡£com
//	File name: acs_bluetooth.c
//	Author:	whl
//	Description: create rastyle acs_bluetooth.c file
//	Others:
//	History: 2015-8-10 create by whl
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
#include <sys/time.h>
#include <stdbool.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <termios.h>
#include <sys/select.h>
#include <linux/serial.h>

#include "../systemconfig.h"
#include "../protocal/protocal.h"
#include "../protocal/sensor_protocol.h"
#include "../common_utils.h"
#include "acs_bluetooth.h"

//at commond lines

static char cmd_msg1[] = "at+ab config var30=1 \r\n at+Ab reset \r\n";
static char cmd_msg3[] = "at+ab config \r\n";
static char cmd_msg4[] = "at+ab factoryinit \r\n";

static char acs_confim_msg[] = "Confirm;";


extern void bluetooth_connection_setup(int bluetooth_fd,acs_bluetooth_get_frame_data tfn_get_frame_data);
extern void bluetooth_first_connection(int bluetooth_fd,acs_bluetooth_get_frame_data tfn_get_frame_data);
extern void bluetooth_connetion_message_config(int bluetooth_fd,acs_bluetooth_get_frame_data tfn_get_frame_data);
extern void bluetooth_public_infor_consult(int bluetooth_fd,acs_bluetooth_get_frame_data tfn_get_frame_data);


#define READ_BLUETOOTH_SLEEP_TIME    50000

#define    Ios_BLE_FLAG      "-BLE-BypassMode-"
#define    Android_SPP_FLAG  "-BypassMode-"

extern int bluetooth_fd;


static void test_bluetooth(int devfd)
{
	char frame_msg[65536] = {0};
	int num;
	while(1)
	{
		 num=read(devfd,frame_msg,sizeof(frame_msg));
		 if(num > 0)
		 {
			 printf("bluetooth cmd num is %d msg is %s\n",num,frame_msg);
		 }

	}
}

/*
 * android get bluetooth frame data
*/
int acs_bluetooth_android_get_frame_data(int devfd,uint8_t *data,uint16_t * length)
{
#if 0
	int i;
	int offset = 0;
	uint8_t  buffer[2]  = {0};
	uint8_t  acs_protocal_frame[65536] = {0};
	uint8_t  receive_flag = 0 ,discard_count = 0;
	struct timeval tv;
	tv.tv_sec  = 10;
	tv.tv_usec = 0;
	uint16_t packLength;
	test_bluetooth(devfd);
	while(1)
	{
		 //get zigbee data
		 memset(buffer,0,sizeof(buffer));
		 int ret = read(devfd, buffer, sizeof(buffer));
		 if(ret > 0)
		 {
			//start flag
			if(buffer[0] == 0x00 && buffer[1] == 0x00)
			{
				memcpy(acs_protocal_frame+offset,buffer,ret);
				offset += ret;
				receive_flag = 1;
				//printf("start ,offset is %d\n",offset);
				TusSleep(READ_BLUETOOTH_SLEEP_TIME);
				continue;
			}
			//end flag
			if(buffer[0] == 0x17)
			{
				memcpy(acs_protocal_frame+offset,buffer,ret);
				offset += ret;
				receive_flag = 0;
				//printf("end ,offset is %d\n",offset);
				goto handle_whole_data;
			}
			if(receive_flag)
			{
				//receive body data
				memcpy(acs_protocal_frame+offset,buffer,ret);
				offset+=ret;
				//printf("reciveing... ,offset is %d\n",offset);
				//TusSleep(READ_BLUETOOTH_SLEEP_TIME);
				continue;
			}
		 }
		 else
		 {
			 TusSleep(READ_BLUETOOTH_SLEEP_TIME);
			 continue;
		 }
	 }
handle_whole_data:
	memcpy(&packLength,&acs_protocal_frame[8],sizeof(uint16_t));
	printf("packLength is %d  offset is %d \n",packLength,offset);
	if(packLength == offset)
	{
		memcpy(data,acs_protocal_frame,offset);
		*length = packLength;
		return 0;
	}
	else
	{
		return  -3;
	}
#endif
	char frame_msg[65536] = {0};
	int num;
	//while(1)
	//{
		num=read(devfd,frame_msg,sizeof(frame_msg));
		if(num > 0)
		{
			printf("num is %d \n",num);
			 //printf("bluetooth cmd num is %d msg is %s\n",num,frame_msg);
			memcpy(data,frame_msg,num);
			*length = num;
			return 0;
		}
		else
		{
			return -1;
		}

	//}


}


/*
 * ios get bluetooth frame data
*/
int acs_bluetooth_ios_get_frame_data(int devfd,uint8_t *data,uint16_t * length)
{
	 char frame_msg[65536] = {0};
	 int num = 0;
	 uint16_t packLength = 0;
	 uint16_t blelength  = 0;
	 uint16_t tmp_len    = 0;
	 uint16_t offset     = 0;
	 while(1)
	 {
		 //first frame data
		 num = read(devfd,frame_msg,20);
		 if(num < 20)
		 {
			 TusSleep(50000);
			 continue;
		 }
		 //calc whole frame data
		 memcpy(&packLength,&frame_msg[8],sizeof(uint16_t));
		 //printf("packLength is %d \n",packLength);
		 tmp_len = packLength-20;
		 offset = 20;
         while(tmp_len > 0)
         {
        	 blelength = tmp_len > 20 ? 20 :tmp_len;
			 num = read(devfd,frame_msg+offset,blelength);
			 if(num < blelength)
			 {
				 TusSleep(50000);
				 continue;
			 }
			 else
			 {
				 offset += blelength;
				 tmp_len -= blelength;
			 }
         }
         //handle whole data
		 memcpy(data,frame_msg,packLength);
		 *length = packLength;
		 break;
	 }
	 return 0;
}


/*
 *  acs get mobile phone type
*/
static App_bluetooth_type acs_get_mobile_phone_type()
{
	char buffer[1024] = {0};
	uint16_t length;
	int ret;
	char *stret = NULL;
	App_bluetooth_type bluetooth_type = unknow;
	while(1)
	{
		//printf("acs bluetooth is query ...\n");
		memset(buffer,0,sizeof(buffer));
		ret = read(bluetooth_fd, buffer, sizeof(buffer));
		if(ret > 0)
		{
			printf("acs_get_mobile_phone_type buffer is %s \n",buffer);
			if(stret=strstr(buffer,Ios_BLE_FLAG))
			{
				printf("current is ios bluetooth is connneted ! \n");
				bluetooth_type = ios;
				break;
			}
			else if(stret=strstr(buffer,Android_SPP_FLAG))
			{
				printf("current is android bluetooth is connneted ! \n");
				bluetooth_type = android;
				break;
			}
			else
			{
				TusSleep(READ_BLUETOOTH_SLEEP_TIME);
                continue;
			}
			//free(stret);
		}
		TusSleep(READ_BLUETOOTH_SLEEP_TIME);

	}
	//free(stret);
	return bluetooth_type;
}

extern void *acs_udp_broadcast_thread(void *args);   //udp broadcast thread main function
/*
 * bluetooth thread main function
*/
void *acs_server_bluetooth_thread(void *args)
{
  	pthread_t acs_udp_handle_id ;
	pthread_attr_t handle_udp_thread_attr;
	int flag = 1;
	char buffer[1024] = {0};
	enum App_bluetooth_type bluetooth_type = unknow;
	//read zigbee data
	printf("acs is read bluetooth  is %s  \n",BLUETOOTH);
	//write at cmd config
	write(bluetooth_fd, cmd_msg1, sizeof(cmd_msg1));
    sleep(5); //wait bluetooth ready
   // test_bluetooth(bluetooth_fd);
    while(1)
    {
    	printf("acs bluetooth is alive ...\n");
    	bluetooth_type =  acs_get_mobile_phone_type();
		if(bluetooth_type == unknow)
		{
			TusSleep(READ_BLUETOOTH_SLEEP_TIME);
			continue;
		}
		else if(bluetooth_type == android)
		{
			printf("android bluetooth path ... \n");
			//test_bluetooth(bluetooth_fd);
			// write(bluetooth_fd, cmd_msg3, sizeof(cmd_msg3));
			bluetooth_first_connection(bluetooth_fd,acs_bluetooth_android_get_frame_data);
			bluetooth_connetion_message_config(bluetooth_fd,acs_bluetooth_android_get_frame_data);
			//bluetooth_connection_setup(bluetooth_fd);
			//bluetooth_public_infor_consult(bluetooth_fd);
			bluetooth_type = unknow;
			//flag = 0;
			goto start_udp;

		}
		else if(bluetooth_type == ios)
		{
			printf("ios bluetooth path ... \n");
			 //handle
			// write(bluetooth_fd, cmd_msg3, sizeof(cmd_msg3));
			bluetooth_first_connection(bluetooth_fd,acs_bluetooth_ios_get_frame_data);
			bluetooth_connetion_message_config(bluetooth_fd,acs_bluetooth_ios_get_frame_data);
			//bluetooth_connection_setup(bluetooth_fd);
			//bluetooth_public_infor_consult(bluetooth_fd);
			bluetooth_type = unknow;
			//flag = 0;
			goto start_udp;
		}
start_udp:
		if(flag)
		{
			AIM_pThreadCreate(&acs_udp_handle_id, handle_udp_thread_attr, PTH_PRIO_LOWEST,acs_udp_broadcast_thread, NULL);
		    flag=0;
		    //reset bluetooth
		    TSleep(5);
		}
		write(bluetooth_fd, cmd_msg1, sizeof(cmd_msg1));


		TusSleep(READ_BLUETOOTH_SLEEP_TIME);
    }
}
