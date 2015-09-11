//*****************************************************************************
//  Copyright (C) 2015 rastyle¡£com
//	File name: protocal.h
//	Author:	whl
//	Description: create rastyle protocol head file
//	Others:
//	History: 2015-7-20 create by whl
//*****************************************************************************

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>


#include "sensor_protocol.h"


/*
* deseliaze sensor data
* return 0 reprent sucessful,-1 reprent fail
*/
#if 0
int deseliaze_sensor_data(
		uint8_t * data,
		uint8_t length,
		uint8_t * sensor_addr,
		uint16_t temp_data[],
		uint8_t *  temp_count,
		float  pm2_5_data[],
		uint8_t* pm2_5_count,
		uint16_t infrared_data[],
		uint8_t * infrared_count,
		uint16_t  formaldehyde[],
		uint8_t * formaldehyde_count
		);
{
	uint8_t i,sensor_type,j;
	uint8_t sensor_count,sensor_per_bytes;
	uint8_t frame_length;
	uint8_t offset = 0,tmp = 0;
	//clear count to  0
	(*temp_count) = 0;
	(*pm2_5_count) = 0;
	(*infrared_count) = 0;
	(*formaldehyde_count) = 0;
	offset += 2;
	*sensor_addr = data[offset];     //sensor addr
	printf("sensor addr is %x \n",*sensor_addr);
	offset += 1;
	frame_length = data[offset];     //frame length
	printf("sensor frame length is %x \n",frame_length);
	offset += 2;
	sensor_count = data[offset];     //sensor count
	printf("sensor count is %x \n",sensor_count);
	offset += 1;
	sensor_per_bytes = data[offset]; //sensor per bytes
	printf("sensor per bytes is %x \n",sensor_per_bytes);
	offset += 1;                     //pointer point to data infor
	for(i = 0;i < sensor_count;i++)
	{
		sensor_type = data[offset];  //get each sensor sensor_type
	    offset +=1 ;                 //pointer point to data infor
		switch(sensor_type)
		{
		 	 case Temperature:
		 		  memcpy(&(temp_data[*temp_count]),data+offset,sizeof(uint16_t));
		 		  (*temp_count)++;
		 		  memcpy(&(temp_data[*temp_count]),data+offset+2,sizeof(uint16_t));
		 		  (*temp_count)++;
		 		  printf("Temperature data is %x ,%x \n",temp_data[0],temp_data[1]);
		 		  printf("temp_count is %d \n",*temp_count);
		 		  break;
		 	 case PM2_5 :
		 		  for(j = 0;j < 4;j++)
		 		  {
		 			 pm2_5_data[(*pm2_5_count)++] = data[offset+j];
		 			 printf("PM2_5 data is %x \n",pm2_5_data[j]);
		 		  }
		 		  printf("pm2_5_count is %d \n",*pm2_5_count);
		 		  break;
		 	 case Infrared:
		 		  memcpy(&(infrared_data[*infrared_count]),data+offset,sizeof(uint16_t));
		 		  (*infrared_count)++;
		 		  memcpy(&(infrared_data[*infrared_count]),data+offset+2,sizeof(uint16_t));
		 		  (*infrared_count)++;
		 		  printf("Infrared data is %x ,%x \n",infrared_data[0],infrared_data[1]);
		 		  printf("Infrared_count is %d \n",*infrared_count);
		 		  break;
		 	 case Formaldehyde:
		 		  for(j = 0;j < 4;j++)
				  {
		 			 formaldehyde[(*formaldehyde_count)++] = data[offset+j];
		 			 printf("Formaldehyde data is %x \n",formaldehyde[j]);
				  }
		 		  printf("formaldehyde_count is %d \n",*formaldehyde_count);
		 	      break;
			 default:
				printf("sensor protocol is wrong ! \n");
				return -1;
		}
	    offset += sensor_per_bytes;
	}

}
#endif
