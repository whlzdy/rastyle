//*****************************************************************************
//  Copyright (C) 2015 rastyle¡£com
//	File name: device_control.h
//	Author:	whl
//	Description: create rastyle device_control head file
//	Others:
//	History: 2015-8-24 create by whl
//*****************************************************************************
#ifndef RASTYLE_ACS_DEVICE_CONTROL_HH
#define RASTYLE_ACS_DEVICE_CONTROL_HH





/*
 * acs handle device control
 * buffer : control message
 * sockfd : socket file desciption
*/
void acs_handle_device_crontrol(int sockfd,char * buffer);







#endif
