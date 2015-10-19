//*****************************************************************************
//  Copyright (C) 2015 rastyle。com
//	File name: protocal.h
//	Author:	whl
//	Description: create rastyle protocol head file
//	Others:
//	History: 2015-7-20 create by whl
//*****************************************************************************
#include <stdio.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/select.h>
#include <stdint.h>
#include <sys/stat.h>	   //for file operate
#include <fcntl.h>
#include <errno.h>
#include <string.h>		   //for sig
#include <signal.h>
#include <sys/syscall.h>
#include <linux/unistd.h>
#include <sys/signal.h>
#include <sys/ipc.h>	   //for msg
#include <sys/msg.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>   //for tcp
#include <termios.h>
#include <sys/select.h>
#include <linux/serial.h>

/*
 * thread create function
 */
int AIM_pThreadCreate_auto(pthread_t* id, pthread_attr_t tattr,
                      int MyPriority, void *(*myThread)(void *), void* arg )
{
	int ret=0;
	struct sched_param param;
	//pthread_t ppid = pthread_self();
	ret = pthread_attr_init(&tattr);
	if (ret)
	{
             printf("pthread_attr_init error!\n");
             return 1;
    }
	ret = pthread_attr_setschedpolicy(&tattr, SCHED_RR);
	/* set the priority; others are unchanged */
	param.sched_priority = MyPriority;
	/* set the new scheduling param */
	ret = pthread_attr_setschedparam (&tattr, &param);
	pthread_attr_setinheritsched(&tattr,PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setdetachstate(&tattr, PTHREAD_CREATE_DETACHED);  //system auto recovery
	ret = pthread_attr_getschedparam (&tattr, &param);
	printf("priority:%d\n",param);

	/*create pthread*/
	//printf("11 Create pthread id=%d\n",&id);
    ret = pthread_create(id, &tattr, myThread, arg);
	printf("Create pthread ret=%d\n",ret);
	//printf("22 Create pthread id=%d\n",&id);
	if (ret)
	{
	    printf("Create pthread error!\n");
		return 1;
	}
	printf("Create pthread ppip=%d\n",(int)pthread_self());
    return 1;
}



/*
 * thread create function
 */
int AIM_pThreadCreate(pthread_t* id, pthread_attr_t tattr,
                      int MyPriority, void *(*myThread)(void *), void* arg )
{
	int ret=0;
	struct sched_param param;
	//pthread_t ppid = pthread_self();
	ret = pthread_attr_init(&tattr);
	if (ret)
	{
             printf("pthread_attr_init error!\n");
             return 1;
    }
	ret = pthread_attr_setschedpolicy(&tattr, SCHED_RR);
	/* set the priority; others are unchanged */
	param.sched_priority = MyPriority;
	/* set the new scheduling param */
	ret = pthread_attr_setschedparam (&tattr, &param);
	pthread_attr_setinheritsched(&tattr,PTHREAD_EXPLICIT_SCHED);
	//pthread_attr_setdetachstate(&tattr, PTHREAD_CREATE_DETACHED);  //system auto recovery
	ret = pthread_attr_getschedparam (&tattr, &param);
	printf("priority:%d\n",param);

	/*create pthread*/
	//printf("11 Create pthread id=%d\n",&id);
    ret = pthread_create(id, &tattr, myThread, arg);
	printf("Create pthread ret=%d\n",ret);
	//printf("22 Create pthread id=%d\n",&id);
	if (ret)
	{
	    printf("Create pthread error!\n");
		return 1;
	}
	printf("Create pthread ppip=%d\n",(int)pthread_self());
    return 1;
}

/*
* timer usec
*/
int TusSleep(long us) {
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = us;
    return select(0, NULL, NULL, NULL, &tv);
}

/*
* timer sec
*/
int TSleep(long us) {
    struct timeval tv;
    tv.tv_sec = us;
    tv.tv_usec = 0;
    return select(0, NULL, NULL, NULL, &tv);
}


/*
 * query hardware is abnormal
 * 1 reprent abnormal
 * 0 reprent normal
*/
int acs_is_abnormal(void)
{
	return 0;
}



int acs_tcp_send(int client_sock,char* info,int length)
{
	 fd_set output;
	 struct timeval tv;
	 tv.tv_sec = 20;
	 tv.tv_usec = 0;
	 FD_ZERO(&output);
	 FD_SET(client_sock, &output);
	 int ret = select(client_sock+1, NULL, &output, NULL, &tv);
	 if (ret < 0) {
		 return -1;
	 } else if (ret == 0) {
		 return 0;
	 }
	 else
	 {
		 return send(client_sock,info,length,0);
	 }

}


/*
 * acs_tcp_receive   tcp server receive
*/
int acs_tcp_receive(int client_sock,char* info,int *length)
{
	  fd_set output;
	  struct timeval tv;
	  tv.tv_sec = 20;
	  tv.tv_usec = 0;
	  FD_ZERO(&output);
	  FD_SET(client_sock, &output);
	  int ret = select(client_sock+1, NULL, &output, NULL, &tv);
	  if (ret < 0) {
			return -1;
	  } else if (ret == 0) {
			return 0;
	  }
	  else
	  {
		  //rastyle protocal
		  char frame_msg[65536] = {0};
		  int num = 0;
		  uint16_t packLength = 0;
		  num = recv(client_sock,frame_msg,10,0);//MSG_NOSIGNAL
		  if(num < 0)
		  {
			  printf("acs_tcp_receive ret 1 is %d \n",num);
			  return -1;
		  }
		  if(num != 10)
		  {
			  return -2;
		  }
		  memcpy(&packLength,&frame_msg[8],sizeof(uint16_t));
		  // printf("packLength is %d \n",packLength);
		  num = 0;
		  num = recv(client_sock,frame_msg+10,packLength-10,0);
		  if(num < 0)
		  {
			  printf("acs_tcp_receive ret 2 is %d \n",num);
			  return -1;
		  }
		  if(num < packLength-10)
		  {
			  return -3;
		  }
		  memcpy(info,frame_msg,packLength);
		  *length = packLength;
		  return packLength ;
	  }
}


/* Setting SO_TCP KEEPALIVE */
//int keep_alive = 1;//设定KeepAlive
//int keep_idle = 1;//开始首次KeepAlive探测前的TCP空闭时间
//int keep_interval = 1;//两次KeepAlive探测间的时间间隔
//int keep_count = 3;//判定断开前的KeepAlive探测次数
void set_keepalive(int fd, int keep_alive, int keep_idle, int keep_interval, int keep_count)
{
	int opt = 1;
	if(keep_alive)
	{
		if(setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE,
								(void*)&keep_alive, sizeof(keep_alive)) == -1)
		{
				fprintf(stderr,
						"setsockopt SOL_SOCKET::SO_KEEPALIVE failed, %s\n",strerror(errno));
		}
		if(setsockopt(fd, SOL_TCP, TCP_KEEPIDLE,
								(void *)&keep_idle,sizeof(keep_idle)) == -1)
		{
				fprintf(stderr,
						"setsockopt SOL_TCP::TCP_KEEPIDLE failed, %s\n", strerror(errno));
		}
		if(setsockopt(fd,SOL_TCP,TCP_KEEPINTVL,
								(void *)&keep_interval, sizeof(keep_interval)) == -1)
		{
				fprintf(stderr,
						 "setsockopt SOL_tcp::TCP_KEEPINTVL failed, %s\n", strerror(errno));
		}
		if(setsockopt(fd,SOL_TCP,TCP_KEEPCNT,
								(void *)&keep_count,sizeof(keep_count)) == -1)
		{
				fprintf(stderr,
						"setsockopt SOL_TCP::TCP_KEEPCNT failed, %s\n", strerror(errno));
		}
	}
}

/*
 *  CRC16
*/
uint16_t CRC16(uint8_t *pCheck, uint32_t count)
{
	uint16_t CRC;
	uint32_t  i,j;
	CRC = 0xFFFF;
	for(i=0; i<count; i++)
	{
		CRC ^= *(pCheck++);
		for(j=0; j<8; j++)
		{
			if(CRC & 1)
			{
				CRC >>= 1;
				CRC ^= 0xA001;
			}
			else
			{
				CRC >>= 1;
			}
		}
	}
	return CRC;
}

/*
 * set serial parameters
*/
int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop)
{
	struct termios newtio,oldtio;
	if  ( tcgetattr( fd,&oldtio)  !=  0) {
		perror("SetupSerial 1");
		return -1;
	}
	bzero( &newtio, sizeof( newtio ) );
	newtio.c_cflag  |=  CLOCAL | CREAD;
	newtio.c_cflag &= ~CSIZE;

	switch( nBits )
	{
	case 7:
		newtio.c_cflag |= CS7;
		break;
	case 8:
		newtio.c_cflag |= CS8;
		break;
	}

	switch( nEvent )
	{
	case 'O':
		newtio.c_cflag |= PARENB;
		newtio.c_cflag |= PARODD;
		newtio.c_iflag |= (INPCK | ISTRIP);
		break;
	case 'E':
		newtio.c_iflag |= (INPCK | ISTRIP);
		newtio.c_cflag |= PARENB;
		newtio.c_cflag &= ~PARODD;
		break;
	case 'N':
		newtio.c_cflag &= ~PARENB;
		break;
	}

   switch( nSpeed )
	{
	case 1200:
		cfsetispeed(&newtio, B1200);
		cfsetospeed(&newtio, B1200);
		break;
	case 2400:
		cfsetispeed(&newtio, B2400);
		cfsetospeed(&newtio, B2400);
		break;
	case 4800:
		cfsetispeed(&newtio, B4800);
		cfsetospeed(&newtio, B4800);
		break;
	case 9600:
		cfsetispeed(&newtio, B9600);
		cfsetospeed(&newtio, B9600);
		break;
	case 19200:
		cfsetispeed(&newtio, B19200);
		cfsetospeed(&newtio, B19200);
		break;
	case 38400:
		cfsetispeed(&newtio, B38400);
		cfsetospeed(&newtio, B38400);
		break;
	case 57600:
		cfsetispeed(&newtio, B57600);
		cfsetospeed(&newtio, B57600);
		break;
	case 115200:
		cfsetispeed(&newtio, B115200);
		cfsetospeed(&newtio, B115200);
		break;
	case 460800:
		cfsetispeed(&newtio, B460800);
		cfsetospeed(&newtio, B460800);
		break;
	default:
		cfsetispeed(&newtio, B9600);
		cfsetospeed(&newtio, B9600);
		break;
	}
	if( nStop == 1 )
		newtio.c_cflag &=  ~CSTOPB;
	else
		if ( nStop == 2 )
			newtio.c_cflag |=  CSTOPB;
	newtio.c_cc[VTIME]  = 0;
	newtio.c_cc[VMIN] = 0;


	//-----------------------//

    newtio.c_lflag &=~(ICANON | ECHO |ECHOE | ISIG );

    newtio.c_oflag &=~OPOST;

	//-----------------------//

	//tcflush(fd,TCIFLUSH);
	tcflush(fd, TCIOFLUSH);
	if((tcsetattr(fd,TCSANOW,&newtio))!=0)
	{
		perror("com set error");
		return -1;
	}
	return 0;
}

