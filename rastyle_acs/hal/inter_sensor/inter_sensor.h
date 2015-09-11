//*****************************************************************************
//  Copyright (C) 2015 rastyle¡£com
//	File name: inter_sensor.h
//	Author:	whl
//	Description: create rastyle inter_sensor head file
//	Others:
//	History: 2015-8-25 create by whl
//*****************************************************************************

#ifndef ACS_INTER_SENSOR_HH
#define ACS_INTER_SENSOR_HH



/*
* turn on wind motor
*/
int acs_get_inter_temperature_and_humidity(uint8_t * tem,uint8_t  *humidity);

int acs_get_inter_pm2_5(uint8_t * unit,float * pm2_5,uint8_t * voc);

int acs_get_inter_laser_pm2_5(
		uint16_t *pm1_0_1,
		uint16_t *pm2_5_1,
		uint16_t * pm10_1,
		uint16_t * pm1_0_2,
		uint16_t * pm2_5_2,
		uint16_t * pm10_2
		);








#endif
