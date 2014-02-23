LOC_MAD_PATH:= $(call my-dir)
LOCAL_PATH := $(LOC_MAD_PATH)

include $(CLEAR_VARS)

LOCAL_MODULE := libmad

LOCAL_SRC_FILES := bit.c \
                   decoder.c \
                   fixed.c \
                   frame.c \
                   huffman.c \
                   layer12.c \
                   layer3.c \
                   stream.c \
                   synth.c \
                   timer.c \
                   version.c

LOCAL_C_INCLUDES := $(LOC_MAD_PATH)/mad_android
LOCAL_EXPORT_C_INCLUDES := $(LOC_MAD_PATH)

LOCAL_CFLAGS := -DHAVE_CONFIG_H -DFPM_DEFAULT
#-ffast-math -O3

include $(BUILD_STATIC_LIBRARY)
