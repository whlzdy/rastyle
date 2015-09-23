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
LIBS    	  =  /home/whl/my_openssl/lib/libssl.a  /home/whl/my_openssl/lib/libcrypto.a           
OBJS 		  =	acs_des.o             
INCLUDES 	  = -I /home/whl/my_openssl/include/
TARGETS		  = des_test
SOURCES 	  = $(OBJS:.o=.c)

TARGETS: $(OBJS) 
	$(CC) $(CFLAGS) -o $(TARGETS) $(OBJS) $(LIBS)
	
acs_des.o : $(LIBS)
	$(CC) -c acs_des.c
	

.PHONY : clean
clean:
	rm -f $(TARGETS) *.o


