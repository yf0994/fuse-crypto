#ifndef __CIPHER_HELPER_H
#define __CIPHER_HELPER_H

#include <openssl/aes.h>
//#include <openssl/sha.h>

void initKey(char* userKey);

void encrypts(const char *in, unsigned char *out, unsigned char* key);

void decrypts(const char *in, unsigned char *out, unsigned char* key);

#endif