//*****************************************************************************
//  Copyright (C) 2015 rastyle。com
//	File name: acs_des.c
//	Author:	whl
//	Description: create rastyle acs_des.c head file
//	Others:
//	History: 2015-9-21 create by whl
//*****************************************************************************
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <openssl/des.h>
#include "acs_des.h"


/*
 * des encrypt
*/
char * DES_Encrypt(const char * key,const char * cleartext, int * encode_size)
{
	int encode_len = 0;
	int i,offset = 0;
	int key_len = 0;
    char * strCipherText = NULL;
	DES_cblock keyEncrypt;
	int size = strlen(cleartext);
	encode_len = (((size%8) == 0)?size:(size/8+1)*8);
	if(encode_size)
	{
		*encode_size = encode_len;
	}
	//printf("encode_len is %d \n",encode_len);
	strCipherText = malloc(encode_len);
	memset(keyEncrypt, 0, 8);
	memset(strCipherText,0,size);
	key_len = strlen(key);
	if (key_len <= 8)
		memcpy(keyEncrypt, key, key_len);
	else
		memcpy(keyEncrypt, key, 8);
    DES_key_schedule keySchedule;
	DES_set_key_unchecked(&keyEncrypt, &keySchedule);
	const_DES_cblock inputText;
	DES_cblock outputText;
	char* tmp = malloc(encode_len);
	memset(tmp, 0, encode_len);
	memcpy(tmp, cleartext, size);
   // int tmp_len = 0;
	for ( i = 0; i < encode_len / 8; i ++)
	{
	    //printf("encode i is %d \n",i);
		//tmp_len = (size - i*8) > 8 ? 8:size - i*8;
	    memcpy(inputText, tmp + i * 8, 8);
	    DES_ecb_encrypt(&inputText, &outputText, &keySchedule, DES_ENCRYPT);
	    //printf("encode outputText is %s \n",outputText);
	    memcpy(strCipherText+offset,outputText,8);
	    offset+=8;
	}
	free(tmp);
    return strCipherText;
}


/*
 * des decrypt
*/
char * DES_Decrypt(const char * key,const char * ciphertext, int size)
{
	int offset = 0,i = 0;
	int key_len = strlen(key);
    char * strClearText = malloc(size);
	DES_cblock keyEncrypt;
	memset(keyEncrypt, 0, 8);
	memset(strClearText,0,size);
	//printf("decode size is %d \n",size);
	if (key_len <= 8)
		memcpy(keyEncrypt, key, key_len);
	else
		memcpy(keyEncrypt, key, 8);
	DES_key_schedule keySchedule;
	DES_set_key_unchecked(&keyEncrypt, &keySchedule);
	const_DES_cblock inputText;
	DES_cblock outputText;
	for ( i = 0; i < size / 8; i ++)
	{
		memcpy(inputText, ciphertext + i * 8, 8);
		DES_ecb_encrypt(&inputText, &outputText, &keySchedule, DES_DECRYPT);
		memcpy(strClearText+offset,outputText,8);
		offset+=8;
	}
    return strClearText;
}








#if  0

static unsigned int get_file_size(const char *path)
{
    unsigned int filesize = -1;
    FILE *fp;
    fp = fopen(path, "r");
    if(fp == NULL)
        return filesize;
    fseek(fp, 0L, SEEK_END);
    filesize = ftell(fp);
    fclose(fp);
    return filesize;
}

#define filepath "/home/whl/data.data"
#define testpath "/home/whl/test.des"

int main()
{
	char key[]="123321RastyleDESKEY";
	char clear[]="罗国标罗国标罗国标罗国标罗国标罗国标罗国标罗国标罗国标罗国标罗国标罗国标罗国标罗国标罗国标罗国标罗国标罗国标罗国标罗国标罗国标罗国标罗国标罗国标罗国标罗国标罗国标罗国标罗国标罗国标罗国标罗国标罗国标罗国标罗国标罗国标罗国标罗国标罗国标罗国标罗国标罗国标罗国标罗国标";
	char *decrypted;
	char *encrypted;
	int filesize,num;
	int size;
	FILE *fp;
	//printf("In Main, Plain text\t : %s \n",clear);
	//encrypted=DES_Encrypt(key,clear,&size);
	//if((fp = fopen(filepath, "w+r"))==NULL)
	//{
	//		printf("cant open the file \n");
	//		exit(0);
	//}
	//fwrite(encrypted, sizeof(char), size, fp);
	//fclose(fp);
	filesize = get_file_size(filepath);
	encrypted = malloc(filesize);
	if((fp = fopen(filepath, "r+b"))==NULL)
	{
		printf("cant open the file \n");
		exit(0);
	}

	 num = fread(encrypted, sizeof(unsigned char), filesize, fp);
	 if(num != filesize)
	 {
		 printf("read failed num is %d \n",num);
		 fclose(fp);
		 return -1;
	}
	printf("begin decrypt \n");
	decrypted=DES_Decrypt(key,encrypted,filesize);
	//printf("In Main, Encrypted text\t : %s \n",encrypted);
	printf("In Main, Decrypted text\t : %s \n",decrypted);
	exit(0);
}

#endif

