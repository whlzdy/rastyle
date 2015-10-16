//*****************************************************************************
//  Copyright (C) 2015 rastyle¡£com
//	File name: protocal.h
//	Author:	whl
//	Description: create rastyle protocol head file
//	Others:
//	History: 2015-7-20 create by whl
//*****************************************************************************

#ifndef   RASTYLE_PROTOCAL_HH
#define   RASTYLE_PROTOCAL_HH

#include <stdlib.h>
#include "../systemconfig.h"

#define PROTOCAL_FRAME_HEAD_LENGTH        10      //head length
#define PROTOCAL_FRAME_STABLE_LENGTH      11
#define PROTOCAL_FRAME_MAX_LENGTH      (65536+11) //accord rastyle protocal
#define START_FLAGS                      0x0
#define END_FLAGS                        0x17

#define TEST_USER_ID   0xa    //actual point to deviceid



/*
 * protocal type
*/
enum protcal_type
{
	connection     = 0x00,     //connet message (0000 0000)
	public_consult = 0x10,     //public consult (0000 0001)
	normal         = 0x20,     //normal mode    (0000 0010)
	abnormal       = 0x30,     //abnormal mode  (0000 0011)
    real_time      = 0x40,     //real time mode (0000 0100)
    device_control = 0x50,     //device control (0000 0101)
    info_sync      = 0x60      //infor sync     (0000 0110)
};



/*
 *  seliaze protocal frame data
*/
uint8_t* seliaze_protocal_data(uint8_t * body_data,uint16_t length,enum protcal_type type,uint32_t user_id);

uint8_t* seliaze_protocal_data_for_encrypt(uint8_t * body_data,uint16_t length,enum protcal_type type,uint32_t user_id,eEncodeType encode_type);
/*
 * deseliaze protocal frame data
*/
char* deseliaze_protocal_data(uint8_t * body_data,uint16_t length);


char * deseliaze_protocal_encode_data(uint8_t * body_data,uint16_t length,uint16_t * encode_len);


/*
* acs get user id
*/
int acs_get_user_id(uint8_t * body_data,uint16_t length);


int acs_verify_pwd(char * username,char * pwd);


int acs_verify_user_name(int protocal_userid,char * username);


void acs_get_user_rsa_public_key(int userid,char * user_public_key);



#endif
