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





char* rsa_decrypt_private(unsigned char *enc,int enc_len,char* private_key_str,int p_len,int *dec_len);
char* rsa_decrypt_public(unsigned char *enc,int enc_len,char* private_key_str,int p_len,int *dec_len);
char* rsa_encrypt_private(unsigned char*txt,int txt_len,char* public_key_str,int p_len,int* enc_len);
char* rsa_encrypt_public(unsigned char*txt,int txt_len,char* public_key_str,int p_len,int* enc_len);








#endif

