//*****************************************************************************
//  Copyright (C) 2015 rastyle。com
//	File name:  inter_sensor.c
//	Author:	whl
//	Description: create rastyle inter_sensor.c file
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
#include "inter_sensor.h"


static uint8_t control_msg_buf[255]  = {0};

#define WIND_MOTOR_MAX_WAIT_TIME  5

extern int inter_sensor_fd;



//=========================wind_motor_control_protocal_frame===========================
//SOI	VER	 ADD LENGTH	CID	 DATAINFO	CHKSUM	EOI
//2B	1B	 1B	  1B    1B	 [LENGTH]B	 2B	    2B
//=====================================================================================
//ADD：0x1a，表示PM2.5传感器 。     响应时:
//ADD：0x1b，表示温度/湿度传感器。   响应时：LEN：2，一个代表温度，一个代表湿度。
//ADD：0x1c，表示风机转速。          响应时:
//ADD：0x1d，表示噪声传感器。        响应时:
//ADD：0x1e，表示风速传感器 。       响应时:
//ADD：0x1f， 表示激光PM2.5传感器 。响应时:
//=====================================================================================
/*
 * seliaze control message
*/
static int _acs_seliaze_inter_sensor_control_msg(uint8_t addr)
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
	control_msg_buf[offset] = addr;
	offset++;
	//length
	control_msg_buf[offset] = 0x00;
	offset++;
	//cid
	control_msg_buf[offset] = 0x00;
	offset++;
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

static int acs_get_inter_sensor_frame(uint8_t * data,uint8_t length)
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
		FD_SET(inter_sensor_fd, &input);
		int ret = select(inter_sensor_fd+1, &input, NULL, NULL, &tv);
		if (ret < 0)
		{
			printf("read sensor error \n");
			exit(-1);
		}
		else if (ret == 0)
		{
			printf("read sensor timeout \n");
		}
		else
		{
			 //get zigbee data
			 memset(buffer,0,sizeof(buffer));
			 ret = read(inter_sensor_fd, buffer, sizeof(buffer));
			 if(ret > 0)
			 {
				//start flag
				if(buffer[0] == 0xff && buffer[1] == 0xfa)
				{
					memcpy(acs_protocal_frame+offset,buffer,ret);
					offset += ret;
					receive_flag = 1;
					//printf("start ,offset is %d\n",offset);
					continue;
				}
				//end flag
				if(buffer[0] == 0XFF && buffer[1] == 0xfe)
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
					continue;
				}
			 }
		}
handle_whole_data:
	   //printf("acs read %d length wind motor data \n",offset);
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

/*
* turn on wind motor
*/
int acs_get_inter_temperature_and_humidity(uint8_t * tem,uint8_t  *humidity)
{
   // uint8_t tmp,humduty;
	int val,i;
	int ret,len;
	uint8_t data[255] = {0};
	uint8_t tmp_cid ;
	len = _acs_seliaze_inter_sensor_control_msg(0x1b);
	tmp_cid = control_msg_buf[5];
	//send turn on control message
	ret = write(inter_sensor_fd, control_msg_buf, len);
	if(ret < 0)
	{
		return ret;
	}
	//get response
	acs_get_inter_sensor_frame(data,sizeof(data));
	if(data[5] == tmp_cid)
	{
		//
	    *tem = data[6];
	    *humidity = data[7];
	    //printf("温度是%d，湿度是%d\n",*tem,*humidity);
	    return 0;
	}
	else
	{
		return -1;
	}

}




int acs_get_inter_laser_pm2_5(
		uint16_t *pm1_0_1,
		uint16_t *pm2_5_1,
		uint16_t * pm10_1,
		uint16_t * pm1_0_2,
		uint16_t * pm2_5_2,
		uint16_t * pm10_2
		)
{
    //uint16_t pm1_0_1,pm2_5_1,pm10_1,pm1_0_2,pm2_5_2,pm10_2;
    int val,i;
    int ret,len;
    uint8_t data[255] = {0};
    uint8_t tmp_cid ;
    len = _acs_seliaze_inter_sensor_control_msg(0x1f);
    tmp_cid = control_msg_buf[5];
    //send turn on control message
    ret = write(inter_sensor_fd, control_msg_buf, len);
    if(ret < 0)
    {
         return ret;
    }
    //get response
    acs_get_inter_sensor_frame(data,sizeof(data));
    if(data[5] == tmp_cid)
    {
       // for(i = 0;i< data[4];i++)
       // {
        //    printf("index is %d,value is %d \n",i,data[6+i]);
        //}
        *pm1_0_1 = data[6] << 8 | data[7];
        *pm2_5_1 = data[8] << 8 | data[9];
        *pm10_1  = data[10] << 8 | data[11];
        *pm1_0_2 = data[12] << 8 | data[13];
        *pm2_5_2 = data[14] << 8 | data[15];
        *pm10_2 =  data[16] << 8 | data[17];
        //printf("pm1_0_1 is %d ug/m3,,pm2_5_1 is %d ug/m3,pm10_1 is %d ug/m3,pm1_0_2 is %d ug/m3,pm2_5_2 is %d ug/m3,pm10_2 is %d ug/m3\n",\
                pm1_0_1,pm2_5_1,pm10_1,pm1_0_2,pm2_5_2,pm10_2);
        return 0;
    }
    else
    {
        return -1;
    }
 }


int acs_get_inter_pm2_5(uint8_t * unit,float * pm2_5,uint8_t * voc)
{
	int val,i;
	int ret,len;
	uint8_t data[255] = {0};
	uint8_t test_data[4] = {236,81,56,63};
	float tmp;
	uint8_t tmp_cid ;
	len = _acs_seliaze_inter_sensor_control_msg(0x1a);
	tmp_cid = control_msg_buf[5];
	//send turn on control message
	ret = write(inter_sensor_fd, control_msg_buf, len);
	if(ret < 0)
	{
		 return ret;
	}
	//get response
	acs_get_inter_sensor_frame(data,sizeof(data));
	if(data[5] == tmp_cid)
	{
	    for(i = 0;i< data[4];i++)
		{
		   printf("index is %d,value is %d \n",i,data[6+i]);
		}
		*unit = data[6];
		//memcpy(pm2_5,&data[7],sizeof(float));
		*pm2_5 =  data[7] + data[8] * 0.01;
		*voc  = data[9];
		memcpy(test_data,&tmp,sizeof(float));
		printf("tmp is %f \n",tmp);
		printf("unit is %d,pm2.5 is %f,voc is %d \n",*unit,*pm2_5,*voc);
		return 0;
	}
	else
	{
		return -1;
	}
}


