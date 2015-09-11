//*****************************************************************************
//  Copyright (C) 2015 rastyle¡£com
//	File name: acs_bluetooth.h
//	Author:	whl
//	Description: create rastyle acs_bluetooth.h head file
//	Others:
//	History: 2015-8-10 create by whl
//*****************************************************************************
#ifndef ACS_BLUETOOTH
#define ACS_BLUETOOTH


typedef enum App_bluetooth_type
{
	ios = 0,
	android,
	unknow
}App_bluetooth_type;


typedef int (*acs_bluetooth_get_frame_data)(int devfd,uint8_t *data,uint16_t * length);

/*
 * acs server bluetooth thread main function
*/
void *acs_server_bluetooth_thread(void *args);





#endif
