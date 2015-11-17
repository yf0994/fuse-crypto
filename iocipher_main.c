//
// Created by fengyin on 15-11-17.
//

#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <jni.h>
#include <android/log.h>

#define LOG_TAG "IOCIPHER"

#define LOGE(fmt, args) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, fmt, ##args)

jint file_open(JNIEnv* env, jclass clazz, jstring javaFilePath){
    return -1;
}

jint file_write(JNIEnv* env, jclass claszz, jint fd, jbyteArray javaBytes, jint size, jint offset){
    return -1;
}

jint file_read(JNIEnv* env, jclass clazz, jint fd, jbyteArray javaBytes, jint size, jint offset){
    return -1;
}

jint file_close(JNIEnv* env, jclass claszz, jint fd){
    return -1;
}

static JNINativeMethod gMethod[] = {
        {"file_open", "(Ljava/lang/String;)I", (void*)file_open},
        {"file_write", "(I[BII)I", (void*)file_write},
        {"file_read", "(I[BII)I", (void*)file_read},
        {"file_close", "(I)I", (void*)file_close},
};


int onLoad(JavaVM* vm, JNIEnv* env, void* reserved) {
    jclass clazz = (*env) -> FindClass(env,  "com/example/fengyin/iocipher/IOUtil");
    if(clazz == NULL){
        return JNI_FALSE;
    }

    if((*env) -> RegisterNatives(env, clazz, gMethod, sizeof(gMethod) / sizeof(gMethod[0])) != JNI_OK){
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv* env = NULL;
    if((*vm) -> GetEnv(vm, (void**)&env, JNI_VERSION_1_6) != JNI_OK){
        abort();
    }
    onLoad(vm, env, reserved);
    return JNI_VERSION_1_6;
}
