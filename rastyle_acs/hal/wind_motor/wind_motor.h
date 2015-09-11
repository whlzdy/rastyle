//*****************************************************************************
//  Copyright (C) 2015 rastyle¡£com
//	File name: wind_motor.h
//	Author:	whl
//	Description: create rastyle wind motor head file
//	Others:
//	History: 2015-8-25 create by whl
//*****************************************************************************

#ifndef ACS_WIND_MOTOR_HH
#define ACS_WIND_MOTOR_HH



/*
* turn on wind motor
*/
int acs_wind_motor_on();


/*
* turn off wind motor
*/
int acs_wind_motor_off();


/*
* change wind motor rate
* note : rate (1~100)
*/
int acs_wind_motor_change_rate(uint8_t rate);










#endif
