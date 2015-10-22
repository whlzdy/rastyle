//*****************************************************************************
//  Copyright (C) 2015 rastyle¡£com
//	File name: systemconfig.h
//	Author:	whl
//	Description: create rastyle system config file
//	Others:
//	History: 2015-7-21 create by whl
//*****************************************************************************

#ifndef SYSTEMCONFIG_HH
#define SYSTEMCONFIG_HH

#include <stdio.h>



typedef enum _tagencrypt
{
	no_encrypt  = 0x00,
	rsa_encrypt = 0x03,
	des_encrypt = 0x01
}eEncodeType;


#define PTH_PRIO_HIGHEST           	85     //define pthread priority
#define PTH_PRIO_HIGH_1             80
#define PTH_PRIO_HIGH           	75     //define pthread priority
#define PTH_PRIO_MIDDLE            	70     //define pthread priority
#define PTH_PRIO_LOW            	65     //define pthread priority
#define PTH_PRIO_LOWEST             60     //define pthread priority



//driver file description

#define RS485_1   "/dev/ttySAC2"           //near the power rs485
#define RS485_2   "/dev/ttySAC1"           //anther rs485  wind monitor 38400
#define ZIGBEE    "/dev/ttyUSB0"           //use to receive zigbee data
#define BLUETOOTH "/dev/ttyUSB1"           //receive bluetooth data

#define SENSOR_MAX_WAIT_TIME  15           //15 seconds
#define BLUETOOTH_MAX_WAIT_TIME  10        //10 seconds


//define sqlite
#define ACS_CONFIG_DATEBASE   "/sddisk/acs_config.db"       //datebase name
#define ABNORMAL_SENSOR_DATA  "abnormal_sensor_data_table"  //abnormal table name
#define ACS_USER_DATA         "acs_user_table"              //user table name
#define ACS_PLAN_TASK         "acs_plan_task_table"         //acs plan task table name
#define ACS_CLIMATE_DATA      "acs_climate_data_table"      //acs climate data table name



//define systeme config info
//#define ACS_DEV_UID     "abc124" //for develop_board "abc124" ,"abc1234"
#define TEST_USER_ID     0xa    //actual point to deviceid  for develop_board "0xa","0x01"



//vsrsion information
#define SOFT_VERISION    "0.0.0.1"
#define HARD_VERISION    "0.0.0.0"


#define DEVICE_ID        "rastyle-0001"




#define ACS_PUBLIC_KEY   "-----BEGIN PUBLIC KEY-----\n"\
"MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDZQy8Fa+AeEBSVuH0+qF1jFLvb\n"\
"sZQadOlsgfFL0mTsAGNkg2XsSofUm7qTGZlbfAeDUnvwGryTLcc4aNFAAzMpsBkM\n"\
"tun/53eTd4XXMFovjCrFBBvyJZPVqw9uVx1OJwNpI0aVveF4fRMjQI1P/bE+I6kc\n"\
"D3GQPfNKXshHB9JaPQIDAQAB\n"\
"-----END PUBLIC KEY-----\n"


#define ACS_PRIVATE_KEY "-----BEGIN RSA PRIVATE KEY-----\n"\
"MIICdwIBADANBgkqhkiG9w0BAQEFAASCAmEwggJdAgEAAoGBANlDLwVr4B4QFJW4\n"\
"fT6oXWMUu9uxlBp06WyB8UvSZOwAY2SDZexKh9SbupMZmVt8B4NSe/AavJMtxzho\n"\
"0UADMymwGQy26f/nd5N3hdcwWi+MKsUEG/Ilk9WrD25XHU4nA2kjRpW94Xh9EyNA\n"\
"jU/9sT4jqRwPcZA980peyEcH0lo9AgMBAAECgYAjZGJgi7Yyr4Pq7+bXMi4kWdhQ\n"\
"bZkmxizQVEqug1OVbmrCkPUljO6bn9LA68ajtIy0w9cUwgY+I5L0iX6s4tiOgR7+\n"\
"WWbgGuXU582ep/7JZmBFlXxhSJAy/EIIn8A99gPbsh/Yxk7MkYG+grRdwZ/c3thF\n"\
"pXf0z6DP2teHbxqssQJBAOxKt/wuW3Zdp4gmKyWNlFZV7UL3NMP39BAqj/xCqczH\n"\
"tDvrpSrm1+l380p+0j2OuLEcvvg4YVe/xezXIbcVgYsCQQDrYiZ9B5Jd968uVHye\n"\
"9HlS8yc6C+1RE2dNFObAsjhkjG1NhE5UEXtzIZC6zrIrO/mh/9W2Hi0WN13MmPG+\n"\
"zHxXAkBQMk/djFJIiyLIJ8QvG61D3CgJuWM0O9ye/AMgZ/O/Z+w5BhJpzC1ttONi\n"\
"3kY21FDE8++wyVnocVMYvi9YNzlfAkEApPziP4c8KBOBvjbIN17jX6pCF6TejpLM\n"\
"OLJq4CzL7GamAsy3XqP2ZQfk+5wANN/QJaY+tW3HaqgaXlviLzwD9wJBAJ/6aJkS\n"\
"zBCGIB7gYHz72FrH+tB6m5zh46H+qHawxM9rkZaka9jJmiURiEW1xxN2iymPHNnk\n"\
"dhw3I50ZxVdk2jM=\n"\
"-----END RSA PRIVATE KEY-----\n"


#define CLOUD_PUBLIC_KEY "-----BEGIN PUBLIC KEY-----\n"\
"MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCzSqwq82zwFj4bnYhUf3zFoY+0\n"\
"TU8Zu6I4zZBbsJPQc8UIuWsZ0mnuISGVi2IpNoyVaKJyaABahG9ch0A4p7oE1Bb8\n"\
"/1LsI29ksQucCP07faFpXPAWG/gqs2iyBiS+9f9JbzNnLtPNiwNokbKK/7tLRJYH\n"\
"UwWk9ZQmH+zuYsz+aQIDAQAB\n"\
"-----END PUBLIC KEY-----\n"


#define ACS_IOS_PUBLIC_KEY "-----BEGIN PUBLIC KEY-----\n"\
"MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDLshBthcdJIidPRDPxki+hHOKx\n"\
"AEgz9SG2cM9uaAjG6OWV7pgtFrZWGGDUg9lG1Mm9HI/N8Y/hOX4mIhiQNDwOB/gD\n"\
"g7sM0hzXTOe0j8XsM8OjisuHgnzHb0r26oGAlEV+/oHUtK5x7FvikrARE8XdCs4u\n"\
"wkG4xyx//4LhWnFVLQIDAQAB\n"\
"-----END PUBLIC KEY-----\n"


#define ACS_ANDROID_PUBLIC_KEY "-----BEGIN PUBLIC KEY-----\n"\
"MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCebNwDfLpLunTDiYyO32PcfnYM\n"\
"1GmBT7CzKa6zVMDH+HqTAptZIfYBdUOt+FQDIzlKguqFJzkbqQZ3dOmEkXEtcJEZ\n"\
"syovqCODKdjHoavRKRKzVzRQcgG4yENkna3215XnCUNiYC8u1b558jmcZWnb2VZs\n"\
"suObHtGusaz6yfe/EwIDAQAB\n"\
"-----END PUBLIC KEY-----\n"



#define ACS_ENCRYPT_FLAG   0      //enrypt switch


#define BUFFER_SIZE (65536+20)




#endif
