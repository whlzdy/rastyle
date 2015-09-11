//*****************************************************************************
//  Copyright (C) 2015 rastyle¡£com
//	File name: zigbee.h
//	Author:	whl
//	Description: create rastyle zigbee.h  used to read outer sensor data
//	Others:
//	History: 2015-9-5 create by whl
//*****************************************************************************

#ifndef ZIGBEE_HH
#define ZIGBEE_HH



int acs_get_zigbee_tmp_and_humidity(
	  uint16_t * indoor_tmp,
	  uint16_t * indoor_humidity
	  );


int acs_get_zigbee_pm_2_5(
		uint8_t  * voc,
		float * indoor_pm2_5
		);

int acs_get_zigbee_cO2(float * cO2);

int acs_get_zigbee_hcho(float * hcho);











#endif
