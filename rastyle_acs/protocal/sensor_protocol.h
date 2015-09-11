//*****************************************************************************
//  Copyright (C) 2015 rastyle��com
//	File name: sensor_protocol.h
//	Author:	whl
//	Description: create rastyle sensor protococl.h  use rs485
//	Others:
//	History: 2015-8-4 create by whl
//*****************************************************************************

#ifndef   RASTYLE_PROTOCAL_HH
#define   RASTYLE_PROTOCAL_HH

#include <stdlib.h>



/*
 * sensor type define
*/
enum sensor_type
{
	Temperature = 0x01,    //�¶ȴ�����
	Humidity,              //ʪ�ȴ�����
	PM2_5,                 //PM2.5������
	Infrared,              //���⴫����
	Formaldehyde           //��ȩ������
};

/*
* deseliaze sensor data
*/
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





#endif
