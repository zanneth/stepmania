LOC_PCRE_PATH:= $(call my-dir)
LOCAL_PATH := $(LOC_PCRE_PATH)

include $(CLEAR_VARS)

LOCAL_MODULE:= libpcre
LOCAL_SRC_FILES := chartables.c \
		   get.c \
		   maketables.c \
		   pcre.c \
		   study.c

LOCAL_C_INCLUDES := $(LOC_PCRE_PATH)
LOCAL_EXPORT_C_INCLUDES := $(LOC_PCRE_PATH)

include $(BUILD_STATIC_LIBRARY)

