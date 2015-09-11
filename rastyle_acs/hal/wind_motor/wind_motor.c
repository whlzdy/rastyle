//*****************************************************************************
//  Copyright (C) 2015 rastyle。com
//	File name: wind_motor.c
//	Author:	whl
//	Description: create rastyle wind_motor.c file
//	Others:
//	History: 2015-8-25 create by whl
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
#include "wind_motor.h"


extern int wind_motor_fd;

static uint8_t control_msg_buf[255]  = {0};

#define WIND_MOTOR_MAX_WAIT_TIME  5


typedef enum ctrl_direction
{
	SET_WRITE = 0x0,
	SET_READ
}t485_ctrl_direction;





//=========================wind_motor_control_protocal_frame===========================
//SOI	VER	 ADD LENGTH	CID	 DATAINFO	CHKSUM	EOI
//2B	1B	 1B	  1B    1B	 [LENGTH]B	 2B	    2B
//=====================================================================================
//CID : 0x01  表示电机开机指令
//CID : 0x02  表示电机关机指令
//CID : 0x03  表示电机控制档位指令，只对开机状态有效，参数为：1~99 档位值。
//=====================================================================================
/*
 * seliaze control message
*/
static int _acs_seliaze_wind_control_msg(uint8_t cid,uint8_t * param_data,uint8_t param_length)
{
	uint16_t crc16 = 0;
	uint8_t offset = 0;
	memset(control_msg_buf,0,255);
	//soi start flag
	control_msg_buf[0] = 0xff;
	control_msg_buf[1] = 0xfa;
	offset += 2;
	//ver 0x01
	control_msg_buf[offset] = 0x01;
	offset++;
	//addr
	control_msg_buf[offset] = 0x00;
	offset++;
	//length
	control_msg_buf[offset] = param_length;
	offset++;
	//cid
	control_msg_buf[offset] = cid;
	offset++;
	//data_info
	if(param_length > 0)
	{
		memcpy(control_msg_buf+offset,param_data,param_length);
		offset += param_length;
	}
	//crc16
	memcpy(control_msg_buf+offset,&crc16,sizeof(uint16_t));
	offset += 2;
	//EOI
	control_msg_buf[offset] = 0xff;
	offset +=1;
	control_msg_buf[offset] = 0xfe;
	offset +=1;
	return offset;
}

int acs_get_wind_motor_frame(uint8_t * data,uint8_t length)
{
	int i;
	int offset = 0;
	uint8_t  buffer[2]  = {0};
	uint8_t  acs_protocal_frame[255] = {0};
	uint8_t  receive_flag = 0 ,discard_count = 0;
	struct timeval tv;
	tv.tv_sec  = WIND_MOTOR_MAX_WAIT_TIME;
	tv.tv_usec = 0;
	while(1)
	{
		fd_set input;
		FD_ZERO(&input);
		FD_SET(wind_motor_fd, &input);
		int ret = select(wind_motor_fd+1, &input, NULL, NULL, &tv);
		if (ret < 0)
		{
			printf("read wind motor error \n");
			return -1;
		}
		else if (ret == 0)
		{
			printf("read wind motor timeout \n");
			return -2;
		}
		else
		{
			 //get zigbee data
			 memset(buffer,0,sizeof(buffer));
			 ret = read(wind_motor_fd, buffer, sizeof(buffer));
			 if(ret > 0)
			 {
				//start flag
				if(buffer[0] == 0xff && buffer[1] == 0xfa)
				{
					memcpy(acs_protocal_frame+offset,buffer,ret);
					offset += ret;
					receive_flag = 1;
					printf("start ,offset is %d\n",offset);
					continue;
				}
				//end flag
				if(buffer[0] == 0XFF && buffer[1] == 0xfe)
				{

					memcpy(acs_protocal_frame+offset,buffer,ret);
					offset += ret;
					receive_flag = 0;
					printf("end ,offset is %d\n",offset);
					goto handle_whole_data;
				}
				if(receive_flag)
				{

					//receive body data
					memcpy(acs_protocal_frame+offset,buffer,ret);
					offset+=ret;
					printf("reciveing... ,offset is %d\n",offset);
					continue;
				}
			 }
		}
handle_whole_data:
		printf("acs read %d length wind motor data \n",offset);
	   // for(i = 0;i<offset;i++)
	   // {
	   // 	printf("index is %d data is %d\n",i,acs_protocal_frame[i]);
	   // }
		if(offset < 10)
		{
			offset = 0;
			memset(acs_protocal_frame,0,255);
			printf("wind motor 485 dicard count is %d \n",discard_count);
			continue;
		}
		memcpy(data,acs_protocal_frame,offset);
		return 0;
	 }
}

static void s485_ctl(int fd,int ctl_flag)
//ctl_flag:SET_READ,SET_WRITE
{
	unsigned long val;
	ioctl(fd,ctl_flag,&val);
}

/*
* turn on wind motor
*/
int acs_wind_motor_on()
{
	int val,i;
	int ret,len;
	uint8_t data[255] = {0};
	uint8_t tmp_cid ;
	//s485_ctl(wind_motor_fd,SET_WRITE);
	len = _acs_seliaze_wind_control_msg(0x01,NULL,0);
	tmp_cid = control_msg_buf[5];
	//send turn on control message
	ret = write(wind_motor_fd, control_msg_buf, len);
	if(ret < 0)
	{
		return ret;
	}
	//get response
	//s485_ctl(wind_motor_fd,SET_READ);
	acs_get_wind_motor_frame(data,sizeof(data));
	if(data[5] == tmp_cid)
	{
		return 0;
	}
	else
	{
		return -1;
	}

}

/*
* turn off wind motor
*/
int acs_wind_motor_off()
{
	int ret,len;
	uint8_t data[255] = {0};
	uint8_t tmp_cid ;
	//s485_ctl(wind_motor_fd,SET_WRITE);
	len = _acs_seliaze_wind_control_msg(0x02,NULL,0);
	tmp_cid = control_msg_buf[5];
	//send turn off control message
	ret = write(wind_motor_fd, control_msg_buf, len);
	if(ret < 0)
	{
		return ret;
	}
	//get response
	//s485_ctl(wind_motor_fd,SET_READ);
	acs_get_wind_motor_frame(data,sizeof(data));
	if(data[5] == tmp_cid)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}




/*
* change wind motor rate
* note : rate (1~100)
*/
int acs_wind_motor_change_rate(uint8_t rate)
{
	int ret,len;
	uint8_t param_data[255] = {0};
	uint8_t data[255] = {0};
	uint8_t tmp_cid ;
	if(rate > 100 ||  rate < 0)
	{
		return -1;
	}
	param_data[0] = rate;
	len = _acs_seliaze_wind_control_msg(0x03,param_data,1);
	tmp_cid = control_msg_buf[5];
	//send control message
	//s485_ctl(wind_motor_fd,SET_WRITE);
	ret = write(wind_motor_fd, control_msg_buf, len);
	if(ret < 0)
	{
		return ret;
	}
	//get response
	acs_get_wind_motor_frame(data,sizeof(data));
	if(data[5] == tmp_cid)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}





