//*****************************************************************************
//  Copyright (C) 2015 rastyle��com
//	File name: common_utils.h
//	Author:	whl
//	Description: create rastyle common_utils head file
//	Others:
//	History: 2015-7-20 create by whl
//*****************************************************************************

#ifndef   RASTYLE_COMMON_UTILS_HH
#define   RASTYLE_COMMON_UTILS_HH


#include <pthread.h>

/*
 * thread create function
 */
int AIM_pThreadCreate(pthread_t *id, pthread_attr_t tattr,int MyPriority, void *(*myThread)(void *), void* arg);

int AIM_pThreadCreate_auto(pthread_t *id, pthread_attr_t tattr,int MyPriority, void *(*myThread)(void *), void* arg);
/*
* timer usec
*/
int TusSleep(long us);

/*
* timer sec
*/
int TSleep(long us);


/*
 * query hardware is abnormal
 * 1 reprent abnormal
 * 0 reprent normal
*/
int acs_is_abnormal(void);

int acs_tcp_send(int client_sock,char* info,int length);

int acs_tcp_receive(int client_sock,char* info,int *length);

//int keep_alive = 1;//�趨KeepAlive
//int keep_idle = 1;//��ʼ�״�KeepAlive̽��ǰ��TCP�ձ�ʱ��
//int keep_interval = 1;//����KeepAlive̽����ʱ����
//int keep_count = 3;//�ж��Ͽ�ǰ��KeepAlive̽�����
void set_keepalive(int fd, int keep_alive, int keep_idle, int keep_interval, int keep_count);



uint16_t CRC16(uint8_t *pCheck, uint32_t count);


int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop);


#endif
