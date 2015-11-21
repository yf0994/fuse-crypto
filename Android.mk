LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := iocipher_main.c
LOCAL_MODULE := iocipher

#LOCAL_SHARED_LIBRARIES := libc libcutils

LOCAL_LBLIBS := -llog

include $(BUILD_SHARED_LIBRARY)


include $(CLEAR_VARS)

LOCAL_SRC_FILES := main.c

LOCAL_MODULE := demo

LOCAL_LDLIBS := -llog -lcrypto -lcutils

include $(BUILD_EXECUTABLE)
