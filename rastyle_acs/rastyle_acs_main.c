//*****************************************************************************
//  Copyright (C) 2015 rastyle。com
//	File name: rastyle_acs_main.c
//	Author:	whl
//	Description: create main file
//	Others:
//	History: 2015-7-20 create by whl
//*****************************************************************************
#include <stdio.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#include <sys/select.h>
#include <stdint.h>
#include <sys/stat.h>	   //for file operate
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/syscall.h>
#include <linux/unistd.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>
#include <sys/msg.h>
#include <sys/signal.h>
#include <sys/ipc.h>	   //for msg
#include <sys/msg.h>
#include <linux/wireless.h>



#include "acs_client/acs_client.h"
#include "acs_server/acs_server.h"
#include "common_utils.h"
#include "systemconfig.h"
#include "sqlite/sqlite.h"
#include "hal/wind_motor/wind_motor.h"
#include "hal/inter_sensor/inter_sensor.h"


int zigbee_fd;
int bluetooth_fd;
int wind_motor_fd;
int inter_sensor_fd;


static char cmd_config_name[] = "AT+AB config DeviceName=Ras-01-0021 \r\n at+Ab reset \r\n";

#define IW_INTERFACE "wlan0"
extern int errno;

struct iwreq wreq;

void wifi_test()
{
	int i;
	int sockfd;
	printf("wifi entry .... \n");
	memset(&wreq, 0, sizeof(struct iwreq));
	sprintf(wreq.ifr_name, IW_INTERFACE);//?
	/*create a socket to get info from ioctl*/
	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		//fprintf(stderr, "Cannot open socket /n");
		//fprintf(stderr, "errno = %d /n", errno);
		//fprintf(stderr, "Error description is : %s/n",strerror(errno));
		perror("Cannot creat socket,error");
		return;
	}
	else {
		printf("Socket opened successfully \n");
	}
	/*create a socket to get info from ioctl*/
	char buffer[32];
	memset(buffer, 0, 32);
	wreq.u.essid.pointer = buffer;//if not write these codes , the program maybe wrong.
	wreq.u.essid.length = 32;
	/*get ESSID from ioctl*/
	if (ioctl(sockfd,SIOCGIWESSID, &wreq) == -1) {
		perror("IOCTL SIOCGIWESSID Failed,error");
		return;
	}
	else {
		printf("IOCTL SIOCGIWESSID Successfull\n");
	}
	/*get ESSID from ioctl*/
	/*output current connecting essid*/
	printf("%s\n",wreq.u.essid.pointer);
	/*output current connecting essid*/
	/*get AP info from ioctl*/
	if (ioctl(sockfd,SIOCGIWAP, &wreq) == -1) {
		perror("IOCTL SIOCGIWAP Failed,error");
		return;
	}
	else {
		printf("IOCTL SIOCGIWAP Successfull\n");
	}
	/*get AP info from ioctl*/
	/* output current connecting AP MAC*/
	for (i = 0; i < 6; i++) {
		unsigned char *APaddr = (unsigned char *) wreq.u.ap_addr.sa_data;
		printf("%02x \n", (int) APaddr[i]);//mac[i] means 1 byte ,i.e. 8 bits.
		if (i != 5)
			printf("%c", ':');
		 else
			printf("\n");
	}
	/* output current connecting AP MAC*/

}

/*
 * system_init
 */
void system_init(void)
{
	char sSQL[1024] = {0};
	 //create sqlite table
	create_acs_plan_task_table(ACS_CONFIG_DATEBASE);
	create_acs_climate_table(ACS_CONFIG_DATEBASE);
	create_acs_user_table(ACS_CONFIG_DATEBASE);
	create_acs_abnormal_table(ACS_CONFIG_DATEBASE);
	//wifi_test();
	//return;
	printf("system init begin ...... \n");
	//config wind motor
	wind_motor_fd = open(RS485_2, O_RDWR | O_NOCTTY);
	if (wind_motor_fd < 0) {
		perror("wind motor fd open failed !\n");
		return ;
	}
	set_opt(wind_motor_fd,38400,8,'N',1);
	printf("config wind motor sucessfully \n");
	//config outer sensor
	inter_sensor_fd = open(RS485_1, O_RDWR | O_NOCTTY);
	if (inter_sensor_fd < 0) {
		perror("inter sensor  fd open failed !\n");
		return ;
	}
	set_opt(inter_sensor_fd,38400,8,'N',1);
	printf("config inter sensor sucessfully \n");
    //config zigbee
	zigbee_fd = open(ZIGBEE, O_RDWR | O_NOCTTY);
	if (zigbee_fd < 0) {
		perror("sensor fd open failed !\n");
		return ;
	}
	set_opt(zigbee_fd,38400,8,'N',1);
	printf("config zigbee sucessfully ,zigbee_fd is %d\n",zigbee_fd);
	//config bluetooth
	bluetooth_fd = open(BLUETOOTH, O_RDWR | O_NOCTTY);
	if (bluetooth_fd < 0) {
			perror("bluetooth fd open failed !\n");
			return ;
		}
	set_opt(bluetooth_fd,115200,8,'N',1);
	write(bluetooth_fd, cmd_config_name, sizeof(cmd_config_name));
	printf("config bluetooth sucessfully bluetooth_fd is %d  \n",bluetooth_fd);
	//create realtime plan task
	sprintf(sSQL,"insert into %s (Planname,Objectadd1,volume1,Objectadd2,volume2,Objectadd3,volume3)values('%s','0x001C','65','0x001D','53','0x001E','78');",
			ACS_PLAN_TASK,"realtime");
	fprintf(stderr,"CONCL_PLADD sql is %s \n",sSQL);
	acs_sqlite_exec_sql(ACS_CONFIG_DATEBASE,sSQL);
	printf("system init succssfully \n");
}




extern void *acs_udp_broadcast_thread(void *args);   //udp broadcast thread main function
extern void *acs_read_sensor_thread(void *args);     //read sensor data and store data to sqlite
extern void *acs_server_bluetooth_thread(void *args);//read bluetooth data

/*
 *system main entry *
        系统main函数入口点
*/
int main(int args,void **arg)
{
#if 0

	 system_init();
	 //printf("电机测试开始启动。。。\n");
	 //printf("acs_get_inter_temperature_and_humidity %d \n",acs_get_inter_temperature_and_humidity());
	 //usleep(50000);
	 //printf("acs_get_inter_pm2_5 %d \n",acs_get_inter_pm2_5());
	 //printf("acs_get_inter_laser_pm2_5 %d \n",acs_get_inter_laser_pm2_5());
	 //return -2;
	// printf("acs_wind_motor_on %d \n",acs_wind_motor_on());

     //printf("acs_wind_motor_off %d \n",acs_wind_motor_off());
	// printf("acs_wind_motor_change_rate %d\n",acs_wind_motor_change_rate(80));
	 //printf("acs_wind_motor_off %d \n",acs_wind_motor_off());
	 //printf("args is %d \n" ,args);
	 uint8_t  unit;
	 float  pm2_5;
	 uint8_t  voc;
	 uint16_t  indoor_tmp;
	 uint16_t  indoor_humidity;
	 float indoor_pm2_5;
	 float cO2;
	 float hcho;
	 //acs_get_inter_pm2_5(&unit,&pm2_5,&voc);
	 //return 0;


	 //acs_get_zigbee_tmp_and_humidity(
	 //	  & indoor_tmp,
	//	  & indoor_humidity
	 // );
	// return 0 ;
	 //sleep(1);
	 acs_get_zigbee_pm_2_5(
	 	 &voc,
	     &indoor_pm2_5
	 	);
	// sleep(1);
	// return;
	//acs_get_zigbee_cO2(&cO2);
	// sleep(1);
	//acs_get_zigbee_hcho(&hcho);
	 return 0;

#endif
	 //return -1;
   	 pthread_t acs_client_id,acs_server_id,udp_broadcast_id,acs_sensor_id,acs_bluetooth_id;
	 pthread_attr_t acs_client_id_attr,acs_server_id_attr,udp_broadcast_attr,acs_sensor_attr,acs_bluetooth_attr;
	 //system init
	 system_init();
	 //wifi_test();
	 //sleep(5);
	 //printf("acs_wind_motor_on %d \n",acs_wind_motor_on());
	 //printf("acs_wind_motor_change_rate %d\n",acs_wind_motor_change_rate(rate));
     //thread create
	 AIM_pThreadCreate(&acs_client_id, acs_client_id_attr, PTH_PRIO_HIGH, acs_client_thread, NULL);
     printf("create acs client process sucessfully !\n");
	 AIM_pThreadCreate(&acs_server_id, acs_server_id_attr, PTH_PRIO_HIGHEST, acs_server_tcp_thread, NULL);
	 printf("create acs server process sucessfully !\n");
	 AIM_pThreadCreate(&udp_broadcast_id, udp_broadcast_attr,PTH_PRIO_LOWEST, acs_udp_broadcast_thread, NULL);
	 printf("create acs udp broadcast process sucessfully !\n");
     AIM_pThreadCreate(&acs_sensor_id, acs_sensor_attr,  PTH_PRIO_HIGH_1 , acs_read_sensor_thread, NULL);
	 printf("create acs sensor read  process sucessfully !\n");
	 //AIM_pThreadCreate(&acs_bluetooth_id, acs_bluetooth_attr, PTH_PRIO_LOWEST, acs_server_bluetooth_thread, NULL);
	// printf("create acs bluetooth  process sucessfully !\n");
	 if(pthread_join(acs_server_id, NULL))
	   perror("acs server Join thread main_ctrl error");
     if(pthread_join(acs_client_id, NULL))
      perror("acs client Join thread main_ctrl error");
     if(pthread_join(udp_broadcast_id, NULL))
   	   perror("udp broadcast Join thread main_ctrl error");
     if(pthread_join(acs_sensor_id, NULL))
      perror("sensor read Join thread main_ctrl error");
    // if(pthread_join(acs_bluetooth_id, NULL))
      // perror("bluetooth Join thread main_ctrl error");

     pthread_attr_destroy(&acs_client_id_attr);
     pthread_attr_destroy(&acs_server_id_attr);
     pthread_attr_destroy(&udp_broadcast_attr);
     pthread_attr_destroy(&acs_sensor_attr);
    // pthread_attr_destroy(&acs_bluetooth_attr);
	 return 0;
}







