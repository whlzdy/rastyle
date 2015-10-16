//*****************************************************************************
//  Copyright (C) 2015 rastyle銆俢om
//	File name: common_utils.h
//	Author:	whl
//	Description: create rastyle common_utils head file
//	Others:
//	History: 2015-9-16 create by whl
//*****************************************************************************
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<memory.h>
#include<openssl/rsa.h>
#include<openssl/pem.h>
#include<openssl/bio.h>
#include<openssl/err.h>

#include "../../systemconfig.h"




//#define CLOUD_PRIVATE_KEY "-----BEGIN RSA PRIVATE KEY-----\nMIICdgIBADANBgkqhkiG9w0BAQEFAASCAmAwggJcAgEAAoGBAJwGVAz6hDDQKPEhKndlq/fRs/DE\nUxHBIhlI075rQ7ZExjRVteYNnYbU4s20vR551TMPnWuuSaGk/Xg6vFhYPTqH4JhncUP8iRRWMrLA\nzuLdB5nQEx/3F3NL7XetGYfEcumnQO9gbFApftIt43Hsp8fff0wUGZQZfaN8SJPF+jI9AgMBAAEC\ngYBhO6hhtjVvXipFq0+VvOzgsfUblJ3YMK4ZZ8Ndpec6zb1fnBKR3eZPwpfdbkd1mKN/ALowAxur\n+vBjXudFfygYNy3aJwkA+vexvyUSmuoRCkVeRgqI133t6KAdP9ChRKzZhuqG096a2IVXMbLrg1aJ\nEc0ZhkYOMDLo7R3sr/8wAQJBAOyuJ5RQ236/89s3zpiVgHTLZvOhL8J+KXN6dGySlLjvs/5yOGKV\nokLzjBz7gIQcMgrsWLeXk6RahGcAsgSW0gECQQCowrxPx0nw5otu6khO6Amv3SvoQvW/CvUJA5eu\nGyg3CYUwos1qivTpHN6zAoMQWndaW77tPWnvUd/DVuOJOig9AkB6jYcjDmdJj31mwtPwQGF+mKAb\ngRgo3oj6KatFWPU3ea87DsvRiGe3wUKI81XNkof7Dfy2O/+QN+qdJ0gGhk4BAkB4WY/T0QqhGrib\nG8b/RqkJ2s+iLoZ0fIrsp7w29LUEMuziF2gor/oieG00+fif4/Qz4zPQmjh+7EOIWeGgjnH9AkEA\npoL8GlgMdrcNE33N5J8mYrPZl8T7os6CC0d5loEDYP88bObAIsjD0SaxODlGADaysfr0aqZ1ZjYp\n7rrdj7txpQ==\n-----END RSA PRIVATE KEY-----\n"


#define PRIVATEKEY "-----BEGIN RSA PRIVATE KEY-----\n"\
"MIICXgIBAAKBgQDFCHp1zloS2nCnLPbU1FtYc2EdSxSThEjefk/khTzFl5xx8JRf\n"\
"rP0+RrPsXqzODC4H1nHxz/BZO9NZ3Onu2UGy+5PN8s3sQE3lKbuBJYkrEUdb7q+n\n"\
"SPKtYvDQ04BhUU6opfbR6cyc0H/seDU7QSJeVyobQYUaXDiqlkTIL2oILwIDAQAB\n"\
"AoGBAK6ywcHKYNrui4vutjwmvYdDZgwPEgNp+yIrZLL939BiiPgNk8DxJOkez1D4\n"\
"8McQzY9teEIrWLQF83NuCI0aXMDCwcWNENkmQRW3Vv9vSv575/VnWf3bDODltyfQ\n"\
"miiO41Y18xU0cyTTVPKzHGPr00MXJ3RAJqHnuiW6LOawnKDBAkEA7CwzANi0dSg5\n"\
"2KYmCuQd/xM/janjTkEbzHzCmJUIZQJfJlu5R7J1A0IGunDwaUg9bm6TBzp62+3Y\n"\
"kP7rSJBj5wJBANWTGIt5KhZZeqRUC0PDvcr/m6sXiosdS15p/InS1FzwyXdjXEUW\n"\
"/tYiHZqYt25ixu+zJYJNJtAWCPc3X54ksHkCQQCfz6zXpsw99vFly9A0UHaH00OP\n"\
"HeWlX2NrNQUM8NQUXc17DFhMztJlWPlqkd/srih7oLE3E/hs9/4y53zYxnQDAkEA\n"\
"wmSfq5s+HpeVFGtW0XghkNp00xabSKICTs5DSSM3ndygU2WBv3T9wOPOdD9afkOz\n"\
"9MrnciFNGYtk37dr9aJJaQJAWfBm33YzCMRNZdOq8vZ7YEhaSjIrvWU/JMRIJz+O\n"\
"ndPo1DhnJBHZJpNvcXJxPuy3wv5XZuP6N5krMcYTpLvffQ==\n"\
"-----END RSA PRIVATE KEY-----\n"

#define PUBLICKEY  "-----BEGIN PUBLIC KEY-----\n"\
"MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDZQy8Fa+AeEBSVuH0+qF1jFLvb\n"\
"sZQadOlsgfFL0mTsAGNkg2XsSofUm7qTGZlbfAeDUnvwGryTLcc4aNFAAzMpsBkM\n"\
"tun/53eTd4XXMFovjCrFBBvyJZPVqw9uVx1OJwNpI0aVveF4fRMjQI1P/bE+I6kc\n"\
"D3GQPfNKXshHB9JaPQIDAQAB\n"\
"-----END PUBLIC KEY-----\n"


#define IOS_PRIVATE_KEY  "-----BEGIN RSA PRIVATE KEY-----\nMIICXQIBAAKBgQC5h1Pe6MGFb9ZV0u/HvzQ5RAq0m330USYWRcsqJtiE21uQpFSU\n0aaj8VLQ2rUn7RWbEnkXhPPlP67X9gnrnlNm4+Lo3ZnCpmfXfdDc92J22XoVP1TZ\nNn2054DqYdXTjvNrE20Cz6hhledjJixrG/IwCCc0zknRmFG3x3qYNCB8LQIDAQAB\nAoGAPS/ie0usN0PRHnD435mxsYk9Np9phOAWfZX7ESXKltZ5dlScCJC0MrFuLK9U\nu5w3WfNXuGgCwAKmuVbNosWOn3phDS0O9/kTQpitzm65SVfhD01zUB3tUPzDRn1M\nYb3xGujBGsyY1NVypQlNm3+Xkk/0YilbS23eAcIcGw4bZEkCQQDmskWwu3xEo0eV\n9/yP04d15gepgEyH76NZsg/i9joyp0yvWL7mA5OXZIxgqQWNm5uSFwjeWoo3ftUM\ndjfH9GyHAkEAzeDKC+qCdH2ekchaRlnBBF69gzpFvocR1law/J/jhmGqP6V02zjQ\n87ESqAN+aiE2EZe3Cc8NqN3hqcYJ7xGSqwJAcaZVitFyUOYXeBwpQjm1PDHMiIZ6\nLWWzFZcRGICw/w9ISgHwWaRda3HPbjcWU0Lbi0bpyZDVEq3bCz6aEFmrtQJBAMsI\nL1l3CPZG2DVex6BvbUmZAP5EXnmaDL47Okm4iJKDCHnB+y8ipfZdn09wOqABZqgM\n3abfudHaF2ztb+iDhQcCQQCVy9UJ/xvKG/fWZjbhbJBU3kBRnxv7QWwvxrP5T77f\nMJ1XriWNUqjh71+1mTY1FZFQvbLJd20/DgNIDcrpwTiw\n-----END RSA PRIVATE KEY-----\n"
#define IOS_PUBLIC_KEY "-----BEGIN PUBLIC KEY-----\nMIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQC5h1Pe6MGFb9ZV0u/HvzQ5RAq0\nm330USYWRcsqJtiE21uQpFSU0aaj8VLQ2rUn7RWbEnkXhPPlP67X9gnrnlNm4+Lo\n3ZnCpmfXfdDc92J22XoVP1TZNn2054DqYdXTjvNrE20Cz6hhledjJixrG/IwCCc0\nzknRmFG3x3qYNCB8LQIDAQAB\n-----END PUBLIC KEY-----\n"

#define TEST_KEY "-----BEGIN PUBLIC KEY-----\n"\
"MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQC5G/QDl6FxjRQErChEBFWsHwJO\n"\
"XiR3VCa1JceiYJpPNzO+WfGMo50ezGCMnSatXrUXSw7FKDZ14RU3bFKp9Er0isD7\n"\
"AwgLjcXUzMLbC7EWvu1j0JM/KkQWn12C5hQ54XiooKRibVduon6NtvQtOvh2T7HK\n"\
"CpNZQN5CM59uojLAUwIDAQAB\n"\
"-----END PUBLIC KEY-----\n"


#define LIURUIKEY "-----BEGIN PUBLIC KEY-----\nMIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCdZga7+ctcmDyMqB7yqNhU8hYM\nlp/F/TOaFxxd9qo9+eMlF5eATn6Cy52En3YcWTfN2Jp9itvBv5agnY9vHc5ZvjmG\n2Gh4NpiTF7L/b8zGrF9POiAzjQfxqkurSFLniXIDf2HRwwu3ojPbbTdvyfWC8Kio\nxTzA1KqKjA3hDO/DwQIDAQAB\n-----END PUBLIC KEY-----\n"

#define BUFFSIZE 1024

#define JSMIN(X, Y) (((X) < (Y)) ? (X) : (Y))


#if 0

/*
 * rsa public decrypt
*/
char* rsa_decrypt_public(unsigned char *enc,int enc_len,char* private_key_str,int p_len,int *dec_len)
{
    RSA* rsa;
    int rsa_len;
    char *p_de;
    #if 1
   // private_key = rsa_key_seliaze(private_key_str);
    BIO* p_bio = BIO_new_mem_buf(private_key_str, -1);
    rsa = PEM_read_bio_RSAPublicKey(p_bio, NULL, 0, NULL); //
    if ( rsa == NULL ) {
        printf("RSA is NULL\n");
        return NULL;
    }
    #else
    FILE* file=fopen("/tmp/r.key","r");
    rsa=PEM_read_RSA_PUBKEY(file,NULL,NULL,NULL);
    #endif
    rsa_len=RSA_size(rsa);
    p_de=(unsigned char *)calloc(rsa_len+1,1);

    printf("rsa length = %d\n",rsa_len);
    int rc=0;
    rc = RSA_public_decrypt(rsa_len,(unsigned char *)enc,(unsigned char*)p_de,rsa,RSA_PKCS1_PADDING); //RSA_public_decrypt RSA_private_decrypt
    if ( rc<=0 ) {
        int e=ERR_get_error();
        printf("error code is:%s\n",ERR_error_string(e,NULL));
        return NULL;
    }

    RSA_free(rsa);
    printf("plain = %s\n",p_de);
    *dec_len = rc;
    return p_de;
 }

/*
 * rsa private encrypt
*/
char* rsa_encrypt_private(unsigned char*txt,int txt_len,char* public_key_str,int p_len,int* enc_len)
{
    RSA* rsa;
    int rsa_len;
    char *p_en;
    #if 1
    //public_key = rsa_key_seliaze(public_key_str);
    BIO* p_bio = BIO_new_mem_buf(public_key_str, -1);
    printf("rsa_encrypt is %p \n",p_bio);
    rsa = PEM_read_bio_RSAPrivateKey(p_bio, NULL, NULL, NULL); //PEM_read_bio_RSAPrivateKey
    if ( rsa == NULL ) {
        printf("RSA is NULL\n");
        return NULL;
    }
    #else
    FILE* file=fopen("/tmp/r_pub.key","r");
    rsa=PEM_read_RSAPrivateKey(file,NULL,NULL,NULL);//PEM_read_RSAPrivateKey
    #endif
    rsa_len=RSA_size(rsa);
    p_en=(unsigned char *)calloc(rsa_len+1,1);
    //printf("rsa length = %d\n",rsa_len);
    int rc=0;
    if((rc=RSA_private_encrypt(txt_len,(unsigned char *)txt,(unsigned char*)p_en,rsa,RSA_PKCS1_PADDING))<=0) {
        int e=ERR_get_error();
        printf("error code is:%s\n",ERR_error_string(e,NULL));
        return NULL;
    }
    RSA_free(rsa);
    *enc_len = rc;
    return p_en;
}
/*
*  private key decrypt
*/
char* rsa_decrypt_private(unsigned char *enc,int enc_len,char* private_key_str,int p_len,int *dec_len)
{
	int rsa_private_len;
    RSA* rsa;
    int rsa_len;
    char *p_de;
    #if 1
   // private_key = rsa_key_seliaze(private_key_str);
    BIO* p_bio = BIO_new_mem_buf(private_key_str, -1);
    rsa = PEM_read_bio_RSAPrivateKey(p_bio, NULL, 0, NULL); //
    if ( rsa == NULL ) {
        printf("RSA is NULL\n");
        return NULL;
    }
    #else
    FILE* file=fopen("/tmp/r.key","r");
    rsa=PEM_read_RSA_PUBKEY(file,NULL,NULL,NULL);
    #endif
    rsa_len=RSA_size(rsa);


    p_de=(unsigned char *)calloc(rsa_len+1,1);

    printf("rsa length = %d\n",rsa_len);
    int rc=0;
    rc = RSA_private_decrypt(rsa_len,(unsigned char *)enc,(unsigned char*)p_de,rsa,RSA_PKCS1_PADDING);
    if ( rc<=0 ) {
        int e=ERR_get_error();
        printf("error code is:%s\n",ERR_error_string(e,NULL));
        return NULL;
    }

    RSA_free(rsa);
    printf("plain = %s\n",p_de);
    *dec_len = rc;
    return p_de;
 }

/*
 * rsa public key encrypt
 */
char* rsa_encrypt_public(unsigned char*txt,int txt_len,char* public_key_str,int p_len,int* enc_len)
{
	//char *public_key = "-----BEGIN RSA PUBLIC KEY-----\nMIGJAoGBAL331YpDOljAJznk4eNt0TfZJREYypIhWTN/gx0g1iUIaLPlFR7ydjaB\npd9V7G3GvvOf3mGijP+9LjKdgQ8p1pgDW7DeXZk2dTAeQ4hdY287/sw6NFKJxMXA\nFGoUdARObVespCZBdHSqo8kFMAjVGge6ZoH6nAjGzvIfijgsj+2jAgMBAAE=\n-----END RSA PUBLIC KEY-----\n";
	//char * private_key = "-----BEGIN RSA PRIVATE KEY-----\nMIICXQIBAAKBgQC999WKQzpYwCc55OHjbdE32SURGMqSIVkzf4MdINYlCGiz5RUe\n8nY2gaXfVextxr7zn95hooz/vS4ynYEPKdaYA1uw3l2ZNnUwHkOIXWNvO/7MOjRS\nicTFwBRqFHQETm1XrKQmQXR0qqPJBTAI1RoHumaB+pwIxs7yH4o4LI/towIDAQAB\nAoGBAI1ALF2EI2w+ZGxdzcBntXtLUI5n2qfReBwcogcUlWYv3Hp2yb+bFV7uA8IO\nh6AQeYd4xcffL+wwZJtqFb6Ko25XAei8Os3xjb9k5fCcyrmyY+5oeXdQHlcbd/f8\niy8/rOEHZTr4iBXe/8ADlQZlRUkYCblPZ4i4BgzBUB6HzhxhAkEA8wJRx/FjOo6F\noO1aTewbvFIv4Dckqq5j/pBu9fkv1AhMxSfdGnsYcuIn15Y1/RlnpxrmJNWgryvd\n+6LJGDgjWQJBAMgfoINe80YiPCdMoboMd/u1uf1BhwujbiJPSrS40lc3jfyPmHA4\n8hppo8QuELI4rXRE/im4c+zmyphxEyULpVsCQQDnD96JGin65MeE1AsYqpdYwmEJ\ndgVkUXt88wK+2ZizqMyubpAa/M6rdgTiRc7CASUAzF/myEYIKdLh0NAbOk3JAkAE\nxEQVfPh8bipSoU+k19EvzKdOcfYef9kKtirIXTKdYzRdlKoD2kdh+6wr6xD4vcLb\n5xzKr5sLRIAE24SiOEHLAkB1TBlvvvIltttSc9lOpq3UhmtHQJaS32lD2Lk2/zNx\nW6Jbsk+sCQXM0ww4GTCpHMISfokEPtqOPikPcVFs98Oj\n-----END RSA PRIVATE KEY-----\n";
    RSA* rsa;
    int rsa_len;
    char *p_en;
    #if 1
    //public_key = rsa_key_seliaze(public_key_str);
    BIO* p_bio = BIO_new_mem_buf(public_key_str, -1);
    printf("rsa_encrypt is %p \n",p_bio);
    rsa = PEM_read_bio_RSAPublicKey(p_bio, NULL, NULL, NULL); //PEM_read_bio_RSAPrivateKey
    if ( rsa == NULL ) {
        printf("RSA is NULL\n");
        return NULL;
    }
    #else
    FILE* file=fopen("/tmp/r_pub.key","r");
    rsa=PEM_read_RSAPrivateKey(file,NULL,NULL,NULL);//PEM_read_RSAPrivateKey
    #endif
    rsa_len=RSA_size(rsa);
    p_en=(unsigned char *)calloc(rsa_len+1,1);
    printf("rsa length = %d\n",rsa_len);
    int rc=0;
    if((rc=RSA_public_encrypt(txt_len,(unsigned char *)txt,(unsigned char*)p_en,rsa,RSA_PKCS1_PADDING))<=0) {
        int e=ERR_get_error();
        printf("error code is:%s\n",ERR_error_string(e,NULL));
        return NULL;
    }
    printf("rsa length = %d\n",strlen(p_en));
    RSA_free(rsa);
    *enc_len = rc;
    return p_en;
}
#endif

/*
 * js private key decrypt
*/
char * js_private_decrypt(const char *cipher_text,int length, char *private_key)
{
    RSA *rsa_privateKey = NULL;
    int rsa_private_len;
    int i;
    // private_key = rsa_key_seliaze(private_key_str);
    //BIO* p_bio = BIO_new_mem_buf(PRIVATE_KEY, -1);
    BIO* p_bio = BIO_new_mem_buf(private_key, -1);
    rsa_privateKey = PEM_read_bio_RSAPrivateKey(p_bio, NULL, 0, NULL); //
    if ( rsa_privateKey == NULL ) {
        printf("RSA is NULL\n");
        return NULL;
    }
    //printf("Cipher text: %s\n", cipher_text);
    rsa_private_len = RSA_size(rsa_privateKey);
   // printf("RSA private length: %d\n", rsa_private_len);
    size_t crypt_len = 0;
    //unsigned char *crypt = base64_decode(cipher_text, strlen(cipher_text), &crypt_len);
    char *crypt = (char *)cipher_text;
    crypt_len = length;
  //  printf("Decoded cipher: %s\nCrypt length: %ld\n", crypt, crypt_len);
    // If no static, it will cause "address of stack memory associated with local variable ...", which mean the variable will released from memory after the end of this function
    char *plain_char = malloc(crypt_len);
    memset(plain_char,0,crypt_len);
    // initialize
    strcpy(plain_char, "");
    char *err = NULL;
    for ( i = 0; i < crypt_len; i += rsa_private_len) {
        unsigned char *crypt_chunk = malloc(rsa_private_len + 1);
        memset(crypt_chunk,0,rsa_private_len + 1);
        memcpy(&crypt_chunk[0], &crypt[i], rsa_private_len);
       // printf("Crypt chunk: %s\n", crypt_chunk);
        unsigned char *result_chunk = malloc(crypt_len + 1);
        memset(result_chunk,0,crypt_len + 1);
        int result_length = RSA_private_decrypt(rsa_private_len, crypt_chunk, result_chunk, rsa_privateKey, RSA_PKCS1_PADDING);
        // chunk length should be the size of privatekey (in bytes) minus 11 (overhead during encryption)
        //  printf("Result chunk: %s\nChunk length: %d\n", result_chunk, result_length);
        free(crypt_chunk);
        // this is to omit the dummy character behind
        // i.e. Result chunk: ABC-1234567-201308101427371250-abcdefghijklmnopqrstuv\240Z
        //      Chunk length: 53
        //      New chunk: ABC-1234567-201308101427371250-abcdefghijklmnopqrstuv
        //
        // by copying the chunk to a temporary variable with an extra length (i.e. in this case is 54)
        // and then set the last character of temporary variable to NULL
        char tmp_result[result_length + 1];
        memcpy(tmp_result, result_chunk, result_length);
        tmp_result[result_length] = '\0';
       // printf("New chunk: %s\n", tmp_result);
        free(result_chunk);
        if (result_length == -1) {
            ERR_load_CRYPTO_strings();
            fprintf(stderr, "Error %s\n", ERR_error_string(ERR_get_error(), err));
            fprintf(stderr, "Error %s\n", err);
        }
        strcat(plain_char, tmp_result);
    }

    RSA_free(rsa_privateKey);
    //free(crypt);
   // printf("Final result: %s\n", plain_char);
    return plain_char;
}

/*
 * js pubilc key encrypt
*/
char * js_public_encrypt(const char *plain_text,int * length, char *public_key)
{
    RSA *rsa_publicKey = NULL;
    int rsa_public_len;
    int i;
    //public_key = rsa_key_seliaze(public_key_str);
    BIO* p_bio = BIO_new_mem_buf(public_key, -1);
   // printf("rsa_encrypt is %p \n",p_bio);
    //rsa_publicKey = PEM_read_bio_RSAPublicKey(p_bio, NULL, NULL, NULL); //PEM_read_bio_RSAPrivateKey
    PEM_read_bio_RSA_PUBKEY(p_bio, &rsa_publicKey, NULL, NULL);
    if ( rsa_publicKey == NULL ) {
        printf("RSA is NULL\n");
        return NULL;
    }
    rsa_public_len = RSA_size(rsa_publicKey);
   // printf("RSA public length: %d\n", rsa_public_len);
    // 11 bytes is overhead required for encryption
    int chunk_length = rsa_public_len - 11;
    // plain text length
    int plain_char_len = (int)strlen(plain_text);
    // calculate the number of chunks
    int num_of_chunks = (int)(strlen(plain_text) / chunk_length) + 1;
    int total_cipher_length = 0;
    // the output size is (total number of chunks) x (the key length)
    int encrypted_size = (num_of_chunks * rsa_public_len);
    char *cipher_data = malloc(encrypted_size + 1);
    if (cipher_data) {
        memset(cipher_data,0,encrypted_size + 1);
    }else{
        return NULL;
    }
    char *err = NULL;
    for ( i = 0; i < plain_char_len; i += chunk_length) {

        // get the remaining character count from the plain text
        int remaining_char_count = plain_char_len - i;

        // this len is the number of characters to encrypt, thus take the minimum between the chunk count & the remaining characters
        // this must less than rsa_public_len - 11
        int len = JSMIN(remaining_char_count, chunk_length);
        unsigned char *plain_chunk = calloc(len + 1,1);
        memset(plain_chunk,0,len+1);
        // take out chunk of plain text
        memcpy(&plain_chunk[0], &plain_text[i], len);
//        printf("Plain chunk: %s\n", plain_chunk);
        unsigned char *result_chunk = calloc(rsa_public_len + 1,1);
        memset(result_chunk,0,rsa_public_len + 1);
        int result_length = RSA_public_encrypt(len, plain_chunk,result_chunk, rsa_publicKey, RSA_PKCS1_PADDING);

//        printf("Plain char len: %d\n", i);

      //  printf("\n\n%ld,鍔犲瘑鍓嶆暟鎹�s \n", strlen((char *)plain_chunk), plain_chunk);
       // printf("\鍔犲瘑缁撴灉闀垮害: %d\n", result_length);
       // printf("*********%d*****************\n", strlen(result_chunk));
        free(plain_chunk);
        if (result_length == -1) {
            ERR_load_CRYPTO_strings();
            fprintf(stderr, "Error %s\n", ERR_error_string(ERR_get_error(), err));
            fprintf(stderr, "Error %s\n", err);
            break;
        }
        memcpy(&cipher_data[total_cipher_length], &result_chunk[0], result_length);
    //    strcat(cipher_data, (const char *)result_chunk);
        total_cipher_length += result_length;
        free(result_chunk);
    }
   // printf("\n 鍔犲瘑鎬婚暱搴�d\n", total_cipher_length);
    cipher_data[total_cipher_length] = '\0';
    *length = encrypted_size;
    //printf("\n鍔犲瘑杩斿洖缁撴灉闀垮害%d\n", strlen(cipher_data));
    RSA_free(rsa_publicKey);
    return cipher_data;
}

/*
 * js public decrypt
*/
char * js_public_decrypt(const char *cipher_text, int encode_len, char *public_key)
{
    RSA *rsa_publicKey = NULL;
    int rsa_public_len;
    int i;
    //printf("js_public_decrypt public_key is %s \n",public_key);
    BIO* p_bio = BIO_new_mem_buf(public_key, -1);
    //BIO* p_bio = BIO_new_mem_buf(public_key, strlen(public_key));
    // printf("rsa_encrypt is %p \n",p_bio);
    //rsa_publicKey = PEM_read_bio_RSAPublicKey(p_bio, NULL, NULL, NULL); //PEM_read_bio_RSAPrivateKey PEM_read_RSA_PUBKEY()
     //PEM_read_bio_RSAPublicKey(p_bio, &rsa_publicKey, 0, NULL); //PEM_read_bio_RSAPrivateKey PEM_read_RSA_PUBKEY()
    //printf("111\n");
    PEM_read_bio_RSA_PUBKEY(p_bio, &rsa_publicKey, NULL, NULL);
    if ( rsa_publicKey == NULL ) {
	   printf("RSA is NULL\n");
	   return NULL;
    }
    //printf("222\n");
    // printf("Cipher text: %s\n", cipher_text);
    rsa_public_len = RSA_size(rsa_publicKey);
    //printf("RSA public length: %d\n", rsa_public_len);
    size_t crypt_len = (size_t)encode_len;
    char *crypt =(char *) cipher_text;
   // unsigned char *crypt = base64_decode(cipher_text, strlen(cipher_text), &crypt_len);
   // printf("Decoded cipher: %s\nCrypt length: %ld\n", crypt, crypt_len);
    // If no static, it will cause "address of stack memory associated with local variable ...", which mean the variable will released from memory after the end of this function
    char *plain_char = malloc(crypt_len);
    //printf("plain_char is %p,crypt_len is %ld",plain_char,(int)crypt_len);
    // initialize
    //printf("333\n");
    strcpy(plain_char, "");
    char *err = NULL;
    for ( i = 0; i < crypt_len; i += rsa_public_len) {
        unsigned char *crypt_chunk = malloc(rsa_public_len + 1);
       // printf("444\n");
        memcpy(&crypt_chunk[0], &crypt[i], rsa_public_len);
        //printf("Crypt chunk: %s\n", crypt_chunk);
        unsigned char *result_chunk = malloc(crypt_len + 1);
       // printf("555\n");
        int result_length = RSA_public_decrypt(rsa_public_len, crypt_chunk, result_chunk, rsa_publicKey, RSA_PKCS1_PADDING);//RSA_NO_PADDING RSA_PKCS1_PADDING
        // chunk length should be the size of publickey (in bytes) minus 11 (overhead during encryption)
       // printf("Result chunk: %s\nChunk length: %d\n", result_chunk, result_length);

        free(crypt_chunk);

        // this is to omit the dummy character behind
        // i.e. Result chunk: ABC-1234567-201308101427371250-abcdefghijklmnopqrstuv\240Z
        //      Chunk length: 53
        //      New chunk: ABC-1234567-201308101427371250-abcdefghijklmnopqrstuv
        //
        // by copying the chunk to a temporary variable with an extra length (i.e. in this case is 54)
        // and then set the last character of temporary variable to NULL
        char tmp_result[result_length + 1];
        //printf("666\n");
        memcpy(tmp_result, result_chunk, result_length);
        tmp_result[result_length] = '\0';
        //printf("New chunk: %s\n", tmp_result);

        free(result_chunk);

        if (result_length == -1) {
            ERR_load_CRYPTO_strings();
            fprintf(stderr, "Error %s\n", ERR_error_string(ERR_get_error(), err));
            fprintf(stderr, "Error %s\n", err);
        }

        strcat(plain_char, tmp_result);
    }
    //printf("777\n");
    RSA_free(rsa_publicKey);
   //free(crypt);
   // printf("Final result: %s\n", plain_char);

    return plain_char;
}

/*
 * js private key encrypt
*/
char * js_private_encrypt(const char *plain_text,int * encode_len ,char *private_key)
{
	RSA *rsa_privateKey = NULL;
	int rsa_private_len;
	int i;
	// private_key = rsa_key_seliaze(private_key_str);
	//BIO* p_bio = BIO_new_mem_buf(PRIVATE_KEY, -1);
	BIO* p_bio = BIO_new_mem_buf(private_key, -1);
    rsa_privateKey = PEM_read_bio_RSAPrivateKey(p_bio, NULL, 0, NULL); //
	if ( rsa_privateKey == NULL ) {
		   printf("RSA is NULL\n");
		   return NULL;
	}
    rsa_private_len = RSA_size(rsa_privateKey);
   // printf("RSA private length: %d\n", rsa_private_len);
    // 11 bytes is overhead required for encryption
    int chunk_length = rsa_private_len - 11;
    // plain text length
    int plain_char_len = (int)strlen(plain_text);
    // calculate the number of chunks
    int num_of_chunks = (int)(strlen(plain_text) / chunk_length) + 1;
    int total_cipher_length = 0;
    // the output size is (total number of chunks) x (the key length)
    int encrypted_size = (num_of_chunks * rsa_private_len);
    unsigned char *cipher_data = malloc(encrypted_size + 1);
    char *err = NULL;
    for ( i = 0; i < plain_char_len; i += chunk_length) {
        // get the remaining character count from the plain text
        int remaining_char_count = plain_char_len - i;
        // this len is the number of characters to encrypt, thus take the minimum between the chunk count & the remaining characters
        // this must less than rsa_private_len - 11
        int len = JSMIN(remaining_char_count, chunk_length);
        unsigned char *plain_chunk = malloc(len + 1);
        memset(plain_chunk,0,len + 1);
        // take out chunk of plain text
        memcpy(&plain_chunk[0], &plain_text[i], len);
       // printf("Plain chunk: %s\n", plain_chunk);
        unsigned char *result_chunk = malloc(rsa_private_len + 1);
        memset(result_chunk,0,rsa_private_len + 1);
        int result_length = RSA_private_encrypt(len, plain_chunk, result_chunk, rsa_privateKey, RSA_PKCS1_PADDING); // RSA_NO_PADDING RSA_PKCS1_PADDING
       // printf("Encrypted Result chunk: %s\nEncrypted Chunk length: %d\n", result_chunk, result_length);
        free(plain_chunk);
        if (result_length == -1) {
            ERR_load_CRYPTO_strings();
            fprintf(stderr, "Error %s\n", ERR_error_string(ERR_get_error(), err));
            fprintf(stderr, "Error %s\n", err);
        }
        memcpy(&cipher_data[total_cipher_length], &result_chunk[0], result_length);
        total_cipher_length += result_length;
        free(result_chunk);
    }
    //printf("Total cipher length: %d\n", total_cipher_length);

    RSA_free(rsa_privateKey);
    // size_t total_len = 0;
    // char *encrypted = base64_encode(cipher_data, encrypted_size, &total_len);
    // printf("Final result: %s\n Final result length: %zu\n", encrypted, total_len);
     *encode_len = encrypted_size;
    //free(cipher_data);

    return cipher_data;
}

#if 0

unsigned int get_file_size(const char *path)
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

static char fllepath[] = "/home/whl/noHeader.data";
static char testpath[] = "/home/whl/ecodata";




int main(void)
{
    char *source="hello rasyle! ajfajfjalfjalsfjaklsjflajfasjflajslfdkjsssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss;hello world";
    char *ptr_en,*ptr_de;
    int enc_len,dec_len;
    int filesize = 0;
    unsigned char *encode_data = NULL;
    FILE *fp;
    int num;
    int i;
    //printf("source is,len si %d\n",strlen(source));
    //ptr_en=rsa_encrypt_private(source,strlen(source),PRIVATEKEY,0,&enc_len); //PRIVATE_KEY PUBLIC_KEY
     printf("begin private encrypte  ...\n");
     ptr_en=js_private_encrypt(source,&enc_len,ACS_PRIVATE_KEY); //PRIVATE_KEY PUBLIC_KEY
   // ptr_de=js_public_decrypt(ptr_en, enc_len,TEST_KEY); //PRIVATE_KEY PUBLIC_KEY
     //ptr_en=rsa_encrypt_public(source,strlen(source),PUBLIC_KEY,0,&enc_len);
   // printf("after decrypt:%s\n",ptr_de);
   // return 0;

	//if((fp = fopen(fllepath, "w+r"))==NULL)
	////{
	//	printf("cant open the file \n");
	//	exit(0);
	//}

   // fwrite(ptr_en, sizeof(char), enc_len, fp);

//    fclose(fp);

#if 0
    printf("fllepath is %s \n",fllepath);
    filesize =  get_file_size(fllepath);
    encode_data =(unsigned char *) malloc(filesize);
    printf("file size is %d \n",filesize);

 	if((fp = fopen(fllepath, "r+b"))==NULL)
    {
 		printf("cant open the file \n");
 		exit(0);
 	}
 	 num = fread(encode_data, sizeof(unsigned char), filesize, fp);
 	 if(num != filesize)
 	 {
 		 printf("read failed num is %d \n",num);
 		 fclose(fp);
 		 return -1;
 	 }
    fclose(fp);
#endif
   // ptr_en=my_private_encrypt(source,OPENSSLKEY);
    //printf("after encrypt:%s,encrypt len is %d\n",ptr_en,strlen(ptr_en));
    //ptr_de=rsa_decrypt_public(ptr_en,strlen(ptr_en),PUBLIC_KEY,0,&dec_len);
    printf("begin public decrypt  ...\n");
    ptr_de = js_public_decrypt(ptr_en, enc_len, ACS_PUBLIC_KEY);
   // ptr_de=js_private_decrypt(ptr_en,enc_len);
   // ptr_de=js_private_decrypt(encode_data,filesize);
   // ptr_de=rsa_decrypt_private(ptr_en,strlen(ptr_en),PRIVATE_KEY,0,&dec_len);
   // ptr_de=my_public_decrypt(ptr_en,PUBLICKEY);
    printf("after decrypt:%s\n",ptr_de);
    if(ptr_en!=NULL)
    {
        //free(ptr_en);
    }   
    if(ptr_de!=NULL)
    {
        free(ptr_de);
    }   
    //free(encode_data);
    return 0;
}
#endif


