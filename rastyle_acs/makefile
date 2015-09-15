#*****************************************************************************
#   Copyright (C) 2015 rastyle¡£com
#	File name: makefile
#	Author:	whl
#	Description: create rastyle project makefile
#	Others:
#   History: 2015-7-20 create by whl
#*****************************************************************************

CC  		  = arm-arago-linux-gnueabi-gcc
CFLAGS  	  = -g -O
#LIBS    	  = -lpthread -lm -Wall -ldl -lsqlite3  -lssl -lcrypto
LIBS    	  = -lpthread -lm -Wall -ldl -lsqlite3 
DIRS          = . ./acs_client ./acs_serve ./protocal ./guard ./hal ./openssl ./sqlite ./business/config_status_sync ./business/connetion \
                  ./business/device_control ./business/normal_report ./business/public_info_consult ./business/real_time_report \
                  ./business/sign_out  ./hal/zigbee/  ./acs_server_bluetooth/ ./device_control/ ./hal/wind_motor ./hal/inter_sensor/
                  
OBJS 		  =	rastyle_acs_main.o common_utils.o udp_broadcast.o ./protocal/protocal.o ./protocal/sensor_protocol.o ./business/connetion/connetion.o ./acs_client/abnormal_mode_report.o \
				 ./business/real_time_report/real_time_report.o ./business/normal_report/normal_mode_report.o ./business/public_info_consult/public_info_consult.o ./business/public_info_consult/app_public_info_consult.o	\
				 ./acs_client/acs_client.o  ./acs_server/app_handle.o ./acs_server/acs_server.o ./business/connetion/acs_stetup_app_connection.o ./hal/zigbee/sensor.o \
                 ./acs_server_bluetooth/bluetoot_public_infor_consult.o ./acs_server_bluetooth/bluetooth_conn_msg_config.o  ./acs_server_bluetooth/bluetooth_conn_setup.o \
                 ./acs_server_bluetooth/bluetooth_first_conn.o ./acs_server_bluetooth/acs_bluetooth.o ./sqlite/sqlite.o ./device_control/device_control.o ./hal/wind_motor/wind_motor.o ./hal/inter_sensor/inter_sensor.o \
                 ./hal/zigbee/zigbee.o ./acs_client/acs_user_update.o
                 
INCLUDES 	  = -I . 
TARGETS		  = air_acs
SOURCES 	  = $(OBJS:.o=.c)

APP_HANDLE_OBJ = ./acs_server/app_handle.c acs_stetup_app_connection.o app_public_info_consult.o 
ACS_CLENT_OBJ  = ./acs_client/acs_client.c protocal.o connetion.o abnormal_mode_report.o real_time_report.o  public_info_consult.o normal_mode_report.o
ACS_SERVER_OBJ = ./acs_server/acs_server.c app_handle.o 
ACS_SENSOR_OBJ = ./hal/zigbee/sensor.c sensor_protocol.o 
ACS_BLUETOOTH_OBJ = ./acs_server_bluetooth/acs_bluetooth.c bluetoot_public_infor_consult.o bluetooth_conn_msg_config.o bluetooth_conn_setup.o bluetooth_first_conn.o

TARGETS: $(OBJS) 
	$(CC) $(CFLAGS) -o $(TARGETS) $(OBJS) $(LIBS)
	
rastyle_acs_main.o : rastyle_acs_main.c
udp_broadcast.o : udp_broadcast.c
common_utils.o : common_utils.c
protocal.o  :  ./protocal/protocal.c
connetion.o : ./business/connetion/connetion.c
abnormal_mode_report.o : ./acs_client/abnormal_mode_report.c
real_time_report.o : ./business/real_time_report/real_time_report.c
normal_mode_report.o : ./business/normal_report/normal_mode_report.c
public_info_consult.o : ./business/public_info_consult/public_info_consult.c
app_public_info_consult.o :./business/public_info_consult/app_public_info_consult.c
acs_stetup_app_connection.o : ./business/connetion/acs_stetup_app_connection.c
sensor_protocol.o : ./protocal/sensor_protocol.c
#acs_bluetooth.o : ./acs_server_bluetooth/acs_bluetooth.c
bluetoot_public_infor_consult.o : ./acs_server_bluetooth/bluetoot_public_infor_consult.c 
bluetooth_conn_msg_config.o : ./acs_server_bluetooth/bluetooth_conn_msg_config.c 
bluetooth_conn_setup.o : ./acs_server_bluetooth/bluetooth_conn_setup.c
bluetooth_first_conn.o : ./acs_server_bluetooth/bluetooth_first_conn.c
sqlite.o  : ./sqlite/sqlite.c
device_control.o : ./device_control/device_control.c
wind_motor.o : ./hal/wind_motor/wind_motor.c
inter_sensor.o : ./hal/inter_sensor/inter_sensor.c
zigbee.o       : ./hal/zigbee/zigbee.c
acs_user_update.o : ./acs_client/acs_user_update.c

app_handle.o : $(APP_HANDLE_OBJ)
	$(CC) -c ./acs_server/app_handle.c
acs_client.o : $(ACS_CLENT_OBJ) 
	$(CC) -c ./acs_client/acs_client.c
acs_server.o : $(ACS_SERVER_OBJ) 
	$(CC) -c ./acs_server/acs_server.c
sensor.o : $(ACS_SENSOR_OBJ)
	$(CC) -c ./hal/zigbee/sensor.c
acs_bluetooth.o : $(ACS_BLUETOOTH_OBJ)
	$(CC) -c ./acs_server_bluetooth/acs_bluetooth.c

.PHONY : clean
clean:
	rm -f $(TARGETS) *.o
	for subdir in $(DIRS);do\
	    rm -f $${subdir}/*.o;\
	done   

