//*****************************************************************************
//  Copyright (C) 2015 rastyle¡£com
//	File name: common_utils.h
//	Author:	whl
//	Description: create rastyle common_utils head file
//	Others:
//	History: 2015-9-16 create by whl
//*****************************************************************************
#ifndef ACS_RSA_HH
#define ACS_RSA_HH




char *js_private_decrypt(const char *cipher_text,int length, char *private_key);

char *js_public_encrypt(const char *plain_text,int * length, char *public_key);

char *js_public_decrypt(const char *cipher_text, int encode_len, char *public_key);

char *js_private_encrypt(const char *plain_text,int * encode_len  char *private_key);







#endif

