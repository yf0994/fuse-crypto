#ifndef __CRYPTO_IO_H
#define __CRYPTO_IO_H

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <android/log.h>

#define LOG_TAG DEBUG
#define LOGE(fmt, args) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, fmt, ##args)
#define AES_BLOCK_SIZE 16

int getFileSize(int fd);

off_t cryptoWrite(int fd, const char* data, int length, off_t offset, unsigned char* key);

off_t cryptoRead(int fd, __u8* data, int length, off_t offset, unsigned char* key);

#endif