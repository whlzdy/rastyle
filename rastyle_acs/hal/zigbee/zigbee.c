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
#include <time.h>
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
	int ret = -1;
	int i;
	int offset = 0;
	uint8_t  buffer[32]  = {0};
	uint8_t  acs_protocal_frame[255] = {0};
	uint8_t  receive_flag = 0 ,discard_count = 0;
	ret = read(zigbee_fd,buffer,32);
	if(ret < 0)
	{
		return ret;
	}
	memcpy(data,buffer,ret);
    return ret;
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
	time_t t_start, t_now;
	len = _acs_seliaze_zigbee_sensor_control_msg(0x20);
	tmp_cid = zigbee_control_msg_buf[5];
	//send turn on control message
	ret = write(zigbee_fd, zigbee_control_msg_buf, len);
	if(ret < 0)
	{
		return ret;
	}
	t_start = time(NULL) ;
    while(1)
    {
		t_now = time(NULL);
		if(difftime(t_now,t_start) > 10)
		{
			return -10;
		}
    	//get response
		ret = acs_get_zigbee_frame(data,sizeof(data));
		if(ret < 0)
		{
			return ret;
		}
		//for(i = 0;i<ret;i++ )
		//{
		//	printf("tmp and humility  index is %d is %x \n",i,data[i]);
		//}
		if(data[5] == tmp_cid && data[3] == 0x20)
		{
			memcpy(indoor_tmp,&data[6],sizeof(uint16_t));
			memcpy(indoor_humidity,&data[8],sizeof(uint16_t));
			//printf("zigbee温度是%d，湿度是%d\n",*indoor_tmp,*indoor_humidity);
			return 0;
		}
		else
		{
			TusSleep(500000);
			continue;
		}
		TusSleep(500000);
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
	time_t t_start, t_now;
	len = _acs_seliaze_zigbee_sensor_control_msg(0x21);
	tmp_cid = zigbee_control_msg_buf[5];
	//send turn on control message
	ret = write(zigbee_fd, zigbee_control_msg_buf, len);
	if(ret < 0)
	{
		return ret;
	}
	t_start = time(NULL) ;
    while(1)
    {
		t_now = time(NULL);
		if(difftime(t_now,t_start) > 10)
		{
			return -10;
		}
    	//get response
		ret = acs_get_zigbee_frame(data,sizeof(data));
		if(ret < 0)
		{
			return ret;
		}
		//for(i = 0;i<ret;i++ )
		//{
		///	printf("pm2.5  index is %d is %x \n",i,data[i]);
		//}
		if(data[5] == tmp_cid && data[3] == 0x21)
		{
			* voc = data[9];
			* indoor_pm2_5 = (data[7] +data[8] * 0.01)*10;
			//printf("zigbee voc 是%d，pm2.5 是%f\n",*voc,*indoor_pm2_5);
			return 0;
		}
		else
		{
			TusSleep(500000);
			continue;
		}
		TusSleep(500000);
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
	time_t t_start, t_now;
	len = _acs_seliaze_zigbee_sensor_control_msg(0x22);
	tmp_cid = zigbee_control_msg_buf[5];
	//send turn on control message
	ret = write(zigbee_fd, zigbee_control_msg_buf, len);
	if(ret < 0)
	{
		return ret;
	}
	t_start = time(NULL) ;
    while(1)
    {
		t_now = time(NULL);
		if(difftime(t_now,t_start) > 10)
		{
			return -10;
		}
    	//get response
		ret = acs_get_zigbee_frame(data,sizeof(data));
		if(ret < 0)
		{
			return ret;
		}
		//for(i = 0;i<ret;i++ )
		//{
		////	printf("co2 index is %d is %x \n",i,data[i]);
		//}
		if(data[5] == tmp_cid && data[3] == 0x22)
		{
			//气体浓度值 = 浓度值高位 * 256 + 浓度值低位
			memcpy(&high,&data[6],2);
			memcpy(&low,&data[8],2);
			*cO2 = high * 256 + low;
			//printf("zigbee cO2 is %f \n",*cO2);
			return 0;
		}
		else
		{
			TusSleep(500000);
			continue;
		}
		TusSleep(500000);
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
	float tmp ;
	time_t t_start, t_now;
	len = _acs_seliaze_zigbee_sensor_control_msg(0x23);
	tmp_cid = zigbee_control_msg_buf[5];
	//for(i = 0;i<len;i++ )
	//{
	///	printf("hcho send index is %d is %x \n",i,zigbee_control_msg_buf[i]);
	//}
	//send turn on control message
	ret = write(zigbee_fd, zigbee_control_msg_buf, len);
	if(ret < 0)
	{
		return ret;
	}
	t_start = time(NULL) ;
	while(1)
	{
		t_now = time(NULL);
		if(difftime(t_now,t_start) > 10)
		{
			return -10;
		}
		//get response
		ret = acs_get_zigbee_frame(data,sizeof(data));
		if(ret < 0)
		{
			return ret;
		}

		if(data[5] == tmp_cid && data[3] == 0x23)
		{
			//气体浓度值 = 浓度值高位 * 256 + 浓度值低位
#if 0
			for(i = 0;i<ret;i++ )
			{
				printf("hcho  index is %d is %x \n",i,data[i]);
			}
#endif
			tmp = data[8]*256+data[9];
			*hcho = tmp/1340;

			//printf("zigbee hcho is %f\n",*hcho);
			return 0;
		}
		else
		{
			TusSleep(500000);
			continue;
		}
		TusSleep(500000);
	}

}
