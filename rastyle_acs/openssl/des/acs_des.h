//*****************************************************************************
//  Copyright (C) 2015 rastyle¡£com
//	File name: acs_des.h
//	Author:	whl
//	Description: create rastyle acs_des.h head file
//	Others:
//	History: 2015-9-21 create by whl
//*****************************************************************************
#ifndef ACS_DES_HH
#define ACS_DES_HH



char * DES_Encrypt(const char * key,const char * cleartext, int * encode_size);

char * DES_Decrypt(const char * key,const char * ciphertext, int size);









#endif
