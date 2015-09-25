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
#define ACS_USER_DATA         "acs_user_table"         //user table name
#define ACS_PLAN_TASK         "acs_plan_task_table"         //acs plan task table name
#define ACS_CLIMATE_DATA      "acs_climate_data_table"      //acs climate data table name



//define systeme config info
#define ACS_DEV_UID     "abc124"



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
"MIICXgIBAAKBgQDFCHp1zloS2nCnLPbU1FtYc2EdSxSThEjefk/khTzFl5xx8JRf\n"\
"rP0+RrPsXqzODC4H1nHxz/BZO9NZ3Onu2UGy+5PN8s3sQE3lKbuBJYkrEUdb7q+n\n"\
"SPKtYvDQ04BhUU6opfbR6cyc0H/seDU7QSJeVyobQYUaXDiqlkTIL2oILwIDAQAB\n"\
"AoGBAK6ywcHKYNrui4vutjwmvYdDZgwPEgNp+yIrZLL939BiiPgNk8DxJOkez1D4\n"\
"8McQzY9teEIrWLQF83NuCI0aXMDCwcWNENkmQRW3Vv9vSv575/VnWf3bDODltyfQ\n"\
"miiO41Y18xU0cyTTVPKzHGPr00MXJ3RAJqHnuiW6LOawnKDBAkEA7CwzANi0dSg5\n"\
"2KYmCuQd/xM/janjTkEbzHzCmJUIZQJfJlu5R7J1A0IGunDwaUg9bm6TBzp62+3Y\n"\
"kP7rSJBj5wJBANWTGIt5KhZZeqRUC0PDvcr/m6sXiosdS15p/InS1FzwyXdjXEUW\n"\
"/tYiHZqYt25ixu+zJYJNJtAWCPc3X54ksHkCQQCfz6zXpsw99vFly9A0UHaH00OP\n"\
"HeWlX2NrNQUM8NQUXc17DFhMztJlWPlqkd/srih7oLE3E/hs9/4y53zYxnQDAkEA\n"\
"wmSfq5s+HpeVFGtW0XghkNp00xabSKICTs5DSSM3ndygU2WBv3T9wOPOdD9afkOz\n"\
"9MrnciFNGYtk37dr9aJJaQJAWfBm33YzCMRNZdOq8vZ7YEhaSjIrvWU/JMRIJz+O\n"\
"ndPo1DhnJBHZJpNvcXJxPuy3wv5XZuP6N5krMcYTpLvffQ==\n"\
"-----END RSA PRIVATE KEY-----\n"

#define CLOUD_PUBLIC_KEY "-----BEGIN PUBLIC KEY-----\n"\
"MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDZQy8Fa+AeEBSVuH0+qF1jFLvb\n"\
"sZQadOlsgfFL0mTsAGNkg2XsSofUm7qTGZlbfAeDUnvwGryTLcc4aNFAAzMpsBkM\n"\
"tun/53eTd4XXMFovjCrFBBvyJZPVqw9uVx1OJwNpI0aVveF4fRMjQI1P/bE+I6kc\n"\
"D3GQPfNKXshHB9JaPQIDAQAB\n"\
"-----END PUBLIC KEY-----\n"

#define ACS_IOS_PUBLIC_KEY "-----BEGIN PUBLIC KEY-----\n"\
"MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDLshBthcdJIidPRDPxki+hHOKx\n"\
"AEgz9SG2cM9uaAjG6OWV7pgtFrZWGGDUg9lG1Mm9HI/N8Y/hOX4mIhiQNDwOB/gD\n"\
"g7sM0hzXTOe0j8XsM8OjisuHgnzHb0r26oGAlEV+/oHUtK5x7FvikrARE8XdCs4u\n"\
"wkG4xyx//4LhWnFVLQIDAQAB\n"\
"-----END PUBLIC KEY-----\n"



#define ACS_ENCRYPT_FLAG   0
#define BUFFER_SIZE (65536+20)


#endif
