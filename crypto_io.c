#include "crypto_io.h"
//#include "cipher_helper.h"

static int validbuffer(unsigned char* buffer){
	int i;
	for(i = 0; i < AES_BLOCK_SIZE; i++){
		if(buffer[i] == '\0'){
			return i;
		}
	}
	return AES_BLOCK_SIZE;
}

int file_open(const char* filepath){
    int fd = open(filepath, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    return fd == -1 ? -1 : fd;
}


int getFileSize(int fd){
	struct stat st;
	if(fstat(fd, &st) == -1){
		ERROR("get file size error!");
		return -1;
	}
	return st.st_size;
}

off_t cryptoWrite(int fd, const char* data, int length, off_t offset, unsigned char* key){
	int i;
    off_t blockNum,remainder;
    unsigned char buffer[AES_BLOCK_SIZE];
    unsigned char ciphertext[AES_BLOCK_SIZE];
    int ret = 0;
    int filesize = getFileSize(fd);

    if(offset > filesize){
        blockNum = offset / AES_BLOCK_SIZE;
        remainder = offset % AES_BLOCK_SIZE;
        if(remainder != 0){
            blockNum += 1;
        }
        offset = blockNum * AES_BLOCK_SIZE;
    } else {
        offset = filesize;
    }
    off_t dataBlockNum = length / AES_BLOCK_SIZE;
    off_t dataRemainder = length % AES_BLOCK_SIZE;
    if(dataRemainder != 0){
        dataBlockNum += 1;
    }
    memset(buffer, 0, AES_BLOCK_SIZE);
    for(i = 0; i < dataBlockNum; i++){
        memcpy(buffer, data + ret, AES_BLOCK_SIZE);
        if((i == dataBlockNum - 1) && (dataRemainder != 0)){
            encrypts((const char*)buffer, ciphertext, key);
        } else {
            encrypts((const char*)buffer, ciphertext, key);
        }
        ret += AES_BLOCK_SIZE;
        pwrite(fd, ciphertext, AES_BLOCK_SIZE, offset);
        offset += AES_BLOCK_SIZE;
        memset(buffer, 0, AES_BLOCK_SIZE);
        memset(ciphertext, 0, AES_BLOCK_SIZE);
    }
    return ret;
}

off_t cryptoRead(int fd, __u8* data, int length, off_t offset, unsigned char* key){
	int partialOffset = offset % AES_BLOCK_SIZE;
    off_t blockNum = offset / AES_BLOCK_SIZE;
    if(partialOffset != 0){
        blockNum -= 1;
    }
    offset = blockNum * AES_BLOCK_SIZE;   
    off_t dataBlockNum = length / AES_BLOCK_SIZE;
    off_t dataPartialOffset = length % AES_BLOCK_SIZE;
    if(dataPartialOffset != 0){
        dataBlockNum -= 1;
    }
    int i;
    unsigned char buffer[AES_BLOCK_SIZE];
    unsigned char plaintext[AES_BLOCK_SIZE];
    int ret = 0;
    for(i = 0; i < dataBlockNum; i++){
        pread(fd, buffer, AES_BLOCK_SIZE, offset);
        offset += AES_BLOCK_SIZE;
        decrypts((const char*)buffer, plaintext, key);
        int copylen = validbuffer(plaintext);
        memcpy(data + ret, plaintext, copylen);
        ret += copylen;
        memset(buffer, 0, AES_BLOCK_SIZE);
        memset(plaintext, 0, AES_BLOCK_SIZE);
    }
    return ret;
}

int main(int argc, char* argv[]){
    int fd = file_open("/sdcard/demo.txt");
    const char* str = "const char* buffer = \"2012年7月4日 - 使用OPENSSL库进行AES256位对称加解密的例子程序... 使用OPENSSL库进行AES256位对称加解密的例子程序资源积分:0分 下载次数:187 资源类型:代码类 资源...\";";
    off_t offset = 0;
    for(int i = 0; i < 4; i++){
        int ret = cryptoWrite(fd, str, strlen(str), offset);
        offset += ret;
    }

    struct stat st;
    if(fstat(fd, &st) == -1){
        return -1;
    }

    size_t fileSize = st.st_size;
    

    return 0;
}