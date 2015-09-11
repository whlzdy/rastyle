//*****************************************************************************
//  Copyright (C) 2015 rastyle。com
//	File name: zigbee.h
//	Author:	whl
//	Description: create rastyle zigbee.c  used to read outer sensor data
//	Others:
//	History: 2015-9-5 create by whl
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
#include "zigbee.h"

extern int zigbee_fd;
#define ZIGBEE_MAX_WAIT_TIME  5


static uint8_t zigbee_control_msg_buf[255]  = {0};

//=========================wind_motor_control_protocal_frame===========================
//SOI	VER	 ADD LENGTH	CID	 DATAINFO	CHKSUM	EOI
//2B	1B	 1B	  1B    1B	 [LENGTH]B	 2B	    2B
//=====================================================================================
//ADD：0x20，表示ZIGBEE，温湿度。响应时:
//ADD：0x21，表示ZIGBEE，PM2.5。响应时:
//ADD：0x22，表示ZIGBEE，红外。响应时:
//ADD：0x23，表示ZIGBEE，甲醛。响应时:
//=====================================================================================
/*
 * seliaze control message
*/
static int _acs_seliaze_zigbee_sensor_control_msg(uint8_t addr)
{
	uint16_t crc16 = 0;
	uint8_t offset = 0;
	memset(zigbee_control_msg_buf,0,255);
	//soi start flag
	zigbee_control_msg_buf[0] = 0xff;
	zigbee_control_msg_buf[1] = 0xfa;
	offset += 2;
	//ver 0x01
	zigbee_control_msg_buf[offset] = 0x01;
	offset++;
	//addr
	zigbee_control_msg_buf[offset] = addr;
	offset++;
	//length
	zigbee_control_msg_buf[offset] = 0x00;
	offset++;
	//cid
	zigbee_control_msg_buf[offset] = 0x00;
	offset++;
	//crc16
	memcpy(zigbee_control_msg_buf+offset,&crc16,sizeof(uint16_t));
	offset += 2;
	//EOI
	zigbee_control_msg_buf[offset] = 0xff;
	offset +=1;
	zigbee_control_msg_buf[offset] = 0xfe;
	offset +=1;
	return offset;
}


/*
* acs get zigbee frame data
*/
int acs_get_zigbee_frame(uint8_t * data,uint8_t length)
{
	int i;
	int offset = 0;
	uint8_t  buffer[2]  = {0};
	uint8_t  acs_protocal_frame[255] = {0};
	uint8_t  receive_flag = 0 ,discard_count = 0;
	struct timeval tv;
	tv.tv_sec  = ZIGBEE_MAX_WAIT_TIME;
	tv.tv_usec = 0;
	while(1)
	{
		fd_set input;
		FD_ZERO(&input);
		FD_SET(zigbee_fd, &input);
		int ret = select(zigbee_fd+1, &input, NULL, NULL, &tv);
		if (ret < 0)
		{
			printf("read zigbee error \n");
			return -1;
		}
		else if (ret == 0)
		{
			printf("read zigbee timeout \n");
			return -2;
		}
		else
		{
			 //get zigbee data
			 memset(buffer,0,sizeof(buffer));
			 ret = read(zigbee_fd, buffer, sizeof(buffer));
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
		printf("acs read %d length zigbee data \n",offset);
	    for(i = 0;i<offset;i++)
	    {
	    	printf("index is %d data is %x\n",i,acs_protocal_frame[i]);
	    }
		if(offset < 10)
		{
			offset = 0;
			memset(acs_protocal_frame,0,255);
			printf("zigbee dicard count is %d \n",discard_count);
			continue;
		}
		memcpy(data,acs_protocal_frame,offset);
		return 0;
	 }
}



int acs_get_zigbee_tmp_and_humidity(
		  uint16_t * indoor_tmp,
		  uint16_t * indoor_humidity
		  )
{
    // uint8_t tmp,humduty;
	int val,i;
	int ret,len;
	uint8_t data[255] = {0};
	uint8_t tmp_cid ;
	len = _acs_seliaze_zigbee_sensor_control_msg(0x20);
	tmp_cid = zigbee_control_msg_buf[5];
	//send turn on control message
	ret = write(zigbee_fd, zigbee_control_msg_buf, len);
	if(ret < 0)
	{
		return ret;
	}
	//get response
	acs_get_zigbee_frame(data,sizeof(data));
	if(data[5] == tmp_cid)
	{
	    memcpy(indoor_tmp,&data[6],sizeof(uint16_t));
	    memcpy(indoor_humidity,&data[8],sizeof(uint16_t));
		printf("zigbee温度是%d，湿度是%d\n",*indoor_tmp,*indoor_humidity);
		return 0;
	}
	else
	{
		return -1;
	}
}


int acs_get_zigbee_pm_2_5(
		uint8_t  * voc,
		float * indoor_pm2_5
		)
{
   // uint8_t voc,indoor_pm2_5;
	int val,i;
	int ret,len;
	uint8_t data[255] = {0};
	uint8_t tmp_cid ;
	len = _acs_seliaze_zigbee_sensor_control_msg(0x21);
	tmp_cid = zigbee_control_msg_buf[5];
	//send turn on control message
	ret = write(zigbee_fd, zigbee_control_msg_buf, len);
	if(ret < 0)
	{
		return ret;
	}
	//get response
	acs_get_zigbee_frame(data,sizeof(data));
	if(data[5] == tmp_cid)
	{
		* voc = data[9];
		* indoor_pm2_5 = data[7] +data[8] * 0.01;
		printf("zigbee voc 是%d，pm2.5 是%f\n",*voc,*indoor_pm2_5);
		return 0;
	}
	else
	{
		return -1;
	}
}

int acs_get_zigbee_cO2(float * cO2)
{
	// uint8_t voc,indoor_pm2_5;
	int val,i;
	int ret,len;
	uint8_t data[255] = {0};
	uint8_t tmp_cid ;
	uint16_t high,low;
	len = _acs_seliaze_zigbee_sensor_control_msg(0x22);
	tmp_cid = zigbee_control_msg_buf[5];
	//send turn on control message
	ret = write(zigbee_fd, zigbee_control_msg_buf, len);
	if(ret < 0)
	{
		return ret;
	}
	//get response
	acs_get_zigbee_frame(data,sizeof(data));
	if(data[5] == tmp_cid)
	{
		//气体浓度值 = 浓度值高位 * 256 + 浓度值低位
	    memcpy(&high,&data[6],2);
	    memcpy(&low,&data[8],2);
		*cO2 = high * 256 + low;
		printf("zigbee cO2 is %f \n",*cO2);
		return 0;
	}
	else
	{
		return -1;
	}
}

int acs_get_zigbee_hcho(float * hcho)
{
	// uint8_t voc,indoor_pm2_5;
	int val,i;
	int ret,len;
	uint8_t data[255] = {0};
	uint8_t tmp_cid ;
	uint16_t high,low;
	len = _acs_seliaze_zigbee_sensor_control_msg(0x23);
	tmp_cid = zigbee_control_msg_buf[5];
	//send turn on control message
	ret = write(zigbee_fd, zigbee_control_msg_buf, len);
	if(ret < 0)
	{
		return ret;
	}
	//get response
	acs_get_zigbee_frame(data,sizeof(data));
	if(data[5] == tmp_cid)
	{
		//气体浓度值 = 浓度值高位 * 256 + 浓度值低位
		memcpy(&high,&data[6],2);
		memcpy(&low,&data[8],2);
		*hcho = high * 256 + low;
		printf("zigbee hcho is %f \n",*hcho);
		return 0;
	}
	else
	{
		return -1;
	}
}
