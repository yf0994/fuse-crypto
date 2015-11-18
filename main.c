//
//  main.c
//  CryptoDemo
//
//  Created by yinfeng on 15/10/19.
//  Copyright (c) 2015年 singuloid. All rights reserved.
//

#include <stdio.h>
#include <openssl/evp.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <openssl/aes.h>
#include <android/log.h>

#define LOG_TAG "DEBUG"

#define LOGE(fmt, args) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, fmt, ##args)
//#include "crypto.h"
//#include "rawio.h"
unsigned char *key = "01234567890123456789012345678901";

/* A 128 bit IV */
unsigned char *iv = "01234567890123456";

#define BLOCK_SIZE 16

int file_open(const char* filepath){
    int fd = open(filepath, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    return fd == -1 ? -1 : fd;
}

ssize_t getsize(int fd){
    struct stat st;
    if(fstat(fd, &st) == -1){
        return 0;
    }
    return st.st_size;
}

int validbuffer(char * buffer){
    int i;
    for(i = 0; i < BLOCK_SIZE; i++){
        if(buffer[i] == '\0'){
            return i;
        }
    }
    return BLOCK_SIZE;
}

ssize_t file_pread(int fd, char* data, ssize_t length, off_t offset){
    int filesize = getsize(fd);
    if(offset > filesize){
        return 0;
    }

    int partialOffset = offset % BLOCK_SIZE;
    off_t blockNum = offset / BLOCK_SIZE;
    if(partialOffset != 0){
        blockNum -= 1;
    }
    offset = blockNum * BLOCK_SIZE;
    off_t dataBlockNum = length / BLOCK_SIZE;
    off_t dataPartialOffset = length % BLOCK_SIZE;
    if(dataPartialOffset != 0){
        dataBlockNum -= 1;
    }
    int i;
    unsigned char buffer[BLOCK_SIZE];
    unsigned char plaintext[BLOCK_SIZE];
    int ret = 0;
    int temp = offset;
    for(i = 0; i < dataBlockNum; i++){
        if((temp + (i + 1) * BLOCK_SIZE) <= filesize){
            pread(fd, buffer, BLOCK_SIZE, offset);
            offset += BLOCK_SIZE;
            decrypts(buffer, plaintext);
            int copylen = validbuffer(plaintext);
            memcpy(data + ret, plaintext, copylen);
            ret += copylen;
            memset(buffer, 0, BLOCK_SIZE);
            memset(plaintext, 0, BLOCK_SIZE);
        }
    }
    return ret;
}

ssize_t file_pwrite(int fd, const char* data, ssize_t length, off_t offset){
    int i;
    off_t blockNum = 0;
    off_t remainder = 0;
    unsigned char buffer[BLOCK_SIZE];
    unsigned char ciphertext[BLOCK_SIZE];
    int ret = 0;
    int filesize = getsize(fd);
    // printf("0000    %d   %d   %d\n", length, offset, filesize);
    // off_t lastFileBlock = filesize / BLOCK_SIZE;
//    ssize_t lastBlockSize = filesize % BLOCK_SIZE;
    if(offset > filesize){
        blockNum = offset / BLOCK_SIZE;
        remainder = offset % BLOCK_SIZE;
        if(remainder != 0){
            blockNum += 1;
        }
        offset = blockNum * BLOCK_SIZE;
    } else {
        offset = filesize;
    }

    off_t dataBlockNum = length / BLOCK_SIZE;
    off_t dataRemainder = length % BLOCK_SIZE;
    if(dataRemainder != 0){
        dataBlockNum += 1;
    }
    char tempBuffer[dataBlockNum * BLOCK_SIZE];

    memset(buffer, 0, BLOCK_SIZE);
    memset(tempBuffer, 0, dataBlockNum * BLOCK_SIZE);
    memcpy(tempBuffer, data, length);
    // printf("ok--- %d  %d   %d\n", blockNum, length, offset);
    for(i = 0; i < dataBlockNum; i++){
        memcpy(buffer, tempBuffer + ret, BLOCK_SIZE);
        encrypts(buffer, ciphertext);
        ret += BLOCK_SIZE;
        pwrite(fd, ciphertext, BLOCK_SIZE, offset);
        offset += BLOCK_SIZE;
        memset(buffer, 0, BLOCK_SIZE);
        memset(ciphertext, 0, BLOCK_SIZE);
    }
    return ret;
}

void handleErrors(void)
{
    ERR_print_errors_fp(stderr);
    abort();
}

int encrypts(const char *in, char *out)
{
    AES_KEY ak;
    AES_set_encrypt_key(key, 128, &ak);
    AES_encrypt((unsigned char *)in, (unsigned char *)out, &ak);
    return 1;
}

int decrypts(const char *in, char *out)
{
    AES_KEY ak;
    AES_set_decrypt_key(key, 128, &ak);
    AES_decrypt((unsigned char *)in, (unsigned char *)out, &ak);
    return 1;
}



int main(int argc, const char * argv[]) {

// ************************************************************************************************************************    
//     int fd = file_open("/sdcard/demo.txt");
//     const char* buffer = "2012年7月4日 - 使用OPENSSL库进行AES256位对称加解密的例子程序... 使用OPENSSL库进行AES256位对称加解密的例子程序资源积分:0分 下载次数:187 资源类型:代码类 资源...1";
//     // printf("%d\n", strlen(buffer));
//     // const char* data = "Hello world12345123";
//     int result = 0;
//     off_t offset = 0;
//     int i = 0;
//     int ret = 0;
//     for(i = 0; i < 160; i++){
//         result = file_pwrite(fd, buffer, strlen(buffer), offset);
//         offset += strlen(buffer);
//     }
//     struct stat st;
//     fstat(fd, &st);
//     printf("%d\n", st.st_size);
//     unsigned char ciphertext[st.st_size];
//     memset(ciphertext, 0, st.st_size);
//     offset = 0;
//     result = 0;
//     ret = 0;
// //    int blockNum = st.st_size / BLOCK_SIZE;
//     int buffersize = BLOCK_SIZE * 64 * 4 ;
//     char outbuffer[buffersize];
//     memset(outbuffer, 0, buffersize);
//     int tmp = 0;
//     int fd1 = open("/sdcard/demo1.txt", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
//     while((result = file_pread(fd, outbuffer, buffersize, offset)) > 0){
//         memcpy(ciphertext + tmp, outbuffer, result);
//         tmp += result;
// //        printf("%d\n", tmp);
//         offset += buffersize;
//         write(fd1, outbuffer, result);
//         memset(outbuffer, 0, buffersize);
//     }
//     // printf("%s\n", ciphertext);






/**************************************************************************************************************************************/    
    struct stat st;
    int fd = open("/sdcard/0.png", O_RDONLY);
    int fd1 = open("/sdcard/2.png", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    fstat(fd, &st);
    int ret = 0;
    printf("[%lld]\n", st.st_size);
    int length = 4096;
    char originbuffer[4096];
    off_t offset = 0;
    int result = 0;
    while((ret = read(fd, originbuffer, length))){
        result = file_pwrite(fd1, originbuffer, ret, offset);
        offset += result; 
    }

    
    printf("123\n" );

    
    int i;
    // for(i = 0; i < 2; i++){


    //     file_pwrite(fd, buffer, strlen(buffer), offset);
    //     offset += strlen(buffer);
    //            // file_pwrite(fd, data, strlen(data), offset);
    //            // offset += strlen(data);
    // }

    memset(&st, 0, sizeof(struct stat));
    if(fstat(fd1, &st) < 0){
        printf("error\n");
    }
    printf("%d\n", st.st_size);
    unsigned char ciphertext[st.st_size];
    memset(ciphertext, 0, st.st_size);
    int fd2 = open("/sdcard/3.png", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    char tmpbuffer[BLOCK_SIZE];
    offset = 0;
    result = 0;
    ret = 0;
//    int blockNum = st.st_size / BLOCK_SIZE;
    int buffersize = BLOCK_SIZE * 64 * 4 ;
    char outbuffer[buffersize];
    memset(outbuffer, 0, buffersize);
    int tmp = 0;
    while((result = file_pread(fd1, outbuffer, buffersize, offset)) > 0){
        memcpy(ciphertext + tmp, outbuffer, result);
        tmp += result;
//        printf("%d\n", tmp);
        offset += buffersize;
        write(fd2, outbuffer, result);
        memset(outbuffer, 0, buffersize);
    }
    // printf("%s\n", ciphertext);
    return 0;
}
