//*****************************************************************************
//  Copyright (C) 2015 rastyle¡£com
//	File name: protocal.h
//	Author:	whl
//	Description: create rastyle protocol head file
//	Others:
//	History: 2015-7-20 create by whl
//*****************************************************************************
#ifndef RASTYLE_ACS_SERVER_HH
#define RASTYLE_ACS_SERVER_HH




#define ACS_SERVER_TCP_PORT        9681
#define ACS_SERVER_MAX_CONNS       20






void *acs_server_tcp_thread(void *args);



#endif
