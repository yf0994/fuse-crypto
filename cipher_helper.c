#include "cipher_helper.h"

char* userKey = "01234567890123456789";
void initKey(char* userKey){

}

void encrypts(const char *in, unsigned char *out, unsigned char* key){
    AES_KEY ak;
    AES_set_encrypt_key((unsigned char*)userKey, 128, &ak);
    AES_encrypt((unsigned char *)in, out, &ak);
}

void decrypts(const char *in, unsigned char *out, unsigned char* key){
    AES_KEY ak;
    AES_set_decrypt_key((unsigned char*)userKey, 128, &ak);
    AES_decrypt((unsigned char *)in, out, &ak);
}