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
#include "../systemconfig.h"


static uint8_t  g_frame_data[PROTOCAL_FRAME_MAX_LENGTH] = {0};

/*
 *  seliaze protocal frame data
*/
uint8_t* seliaze_protocal_data_for_encrypt(uint8_t * body_data,uint16_t length,enum protcal_type type,uint32_t user_id,eEncodeType encode_type)
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
	switch(encode_type)
	{
		case no_encrypt:
			g_frame_data[offset] = (int)no_encrypt ;
			break;
		case rsa_encrypt:
			g_frame_data[offset] = (int)rsa_encrypt;
			break;
		case des_encrypt:
			g_frame_data[offset] = (int)des_encrypt ;
			break;
	}
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
	//reseved
	g_frame_data[offset] = 0 ;
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
	memcpy(g_frame_data,body_data+10,tmp_16-PROTOCAL_FRAME_STABLE_LENGTH);
	return g_frame_data;
}


char * deseliaze_protocal_encode_data(uint8_t * body_data,uint16_t length,uint16_t * encode_len)
{
    uint32_t userid = 0;
	uint16_t offset = 0;
	uint16_t tmp_16 = 0;
	memcpy(&userid,body_data+4,sizeof(uint32_t));
	//printf("user id  is %d \n",userid);
	memset(g_frame_data,0,PROTOCAL_FRAME_MAX_LENGTH);
    //get data length
	memcpy(&tmp_16,body_data+8,sizeof(uint16_t));
	//printf("frame length is %d \n",tmp_16);
	memcpy(g_frame_data,body_data+10,tmp_16-11);
	if(encode_len)
	{
		*encode_len = tmp_16-11;
	}
	return g_frame_data;
}

int acs_get_user_id(uint8_t * body_data,uint16_t length)
{
    uint32_t userid = 0;
	memcpy(&userid,body_data+4,sizeof(uint32_t));
	return userid;
}

static int select_very_callback(void * data, int col_count, char ** col_values, char ** col_Name)
{
	  int i;
	  //printf("data is %p col_count is %d \n",data,col_count);
	  for( i=0; i < col_count; i++)
	  {
			printf( "%s = %s\n", col_Name[i], col_values[i] == 0 ? "NULL" : col_values[i] );
			if(data != NULL )
			{
				 // printf("col_values[i] length is %d \n",strlen(col_values[i]));
				 strcat((char *)data,col_values[i]);
			}

	  }
	  return 0;
}

/*
 * acs to verify username
 * 0 : sucess -1 :fail
*/
int acs_verify_user_name(int protocal_userid,char * username)
{
	char value[1024] = {0};
	char sSQL[100] = {0};
	printf("username is %s \n",username);
	sprintf(sSQL,"select UserID from acs_user_table where Username = '%s';",username);
	sqlite_get_record_data(ACS_CONFIG_DATEBASE,sSQL,select_very_callback,&value);
	printf("protocal_userid is %d \n",protocal_userid);
	printf("table useid is %d \n",atoi(value));
	if(protocal_userid == atoi(value))
	{
		printf("username is correct \n");
		return 0;
	}
	else
	{
		return -1;
	}
}


/*
 * acs to verify password
*/
int acs_verify_pwd(char * username,char * pwd)
{
	char value[1024] = {0};
	char sSQL[100] = {0};
	printf("\npwd is %s\n",pwd);
	sprintf(sSQL,"select PWD from acs_user_table where Username = '%s';",username);
	sqlite_get_record_data(ACS_CONFIG_DATEBASE,sSQL,select_very_callback,&value);
	if(strcmp(pwd,value) == 0)
	{
		printf("password is correct ! \n");
		return 0;
	}
	else
	{
		return -1;
	}

}

/*
 * acs get user rsa publick key
 * note: only in username and password verify ok
*/
void acs_get_user_rsa_public_key(int userid,char * user_public_key)
{
	char sSQL[1024] = {0};
	//printf("user_public_key is %p \n ",user_public_key);
	//printf("acs_get_user_rsa_public_key current userid is %d \n",userid);
	sprintf(sSQL,"select Publickey from acs_user_table where UserID = '%d';",userid);
	//printf("sSQL is %s \n",sSQL);
	sqlite_get_record_data(ACS_CONFIG_DATEBASE,sSQL,select_very_callback,user_public_key);
}


