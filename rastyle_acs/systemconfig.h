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
#define SOFT_VERISION   "0.0.0.1"
#define HARD_VERISION   "0.0.0.0"




#endif
