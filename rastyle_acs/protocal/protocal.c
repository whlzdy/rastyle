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
#include "protocal.h"


uint8_t  g_frame_data[PROTOCAL_FRAME_MAX_LENGTH] = {0};

/*
 *  seliaze protocal frame data
*/
uint8_t* seliaze_protocal_data(uint8_t * body_data,uint16_t length,enum protcal_type type,uint32_t user_id)
{
	uint16_t offset = 0;
	uint16_t tmp_16 = 0;
	//memset global data buffer
	memset(g_frame_data,0,PROTOCAL_FRAME_MAX_LENGTH);
	//build frame header
	tmp_16 = (uint16_t)START_FLAGS;
	memcpy(g_frame_data,&tmp_16,sizeof(uint16_t));
	offset += 2;
	g_frame_data[offset] = (uint8_t)type;
	offset += 1;
	g_frame_data[offset] = 0 ;//reserved
	offset +=1;
	memcpy(g_frame_data+offset,&user_id,sizeof(uint32_t));
	offset += 4;
	tmp_16 = length+PROTOCAL_FRAME_STABLE_LENGTH;
	memcpy(g_frame_data+offset,&tmp_16,sizeof(uint16_t));
	offset += 2;
	//add body data
	memcpy(g_frame_data+offset,body_data,length);
	offset += length;
	//add frame endtmp_16
	g_frame_data[offset] = (uint8_t)END_FLAGS;
	//return data pointer
	return g_frame_data;
}

/*
 * deseliaze protocal frame data
*/
char* deseliaze_protocal_data(uint8_t * body_data,uint16_t length)
{
    uint32_t userid = 0;
	uint16_t offset = 0;
	uint16_t tmp_16 = 0;
	uint16_t i  =0;
	//memset global data buffer16
    // for(i = 0;i < length;i++)
    //{
    //	printf("data index is %d value is %d \n",i,body_data[i]);
    // }
	memcpy(&userid,body_data+4,sizeof(uint32_t));
	//printf("user id  is %d \n",userid);
	memset(g_frame_data,0,PROTOCAL_FRAME_MAX_LENGTH);
    //get data length
	memcpy(&tmp_16,body_data+8,sizeof(uint16_t));
	//printf("frame length is %d \n",tmp_16);
	memcpy(g_frame_data,body_data+10,tmp_16);
	return g_frame_data;
}





