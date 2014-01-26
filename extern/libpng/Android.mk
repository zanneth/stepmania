LOC_PNG_PATH:= $(call my-dir)
LOCAL_PATH := $(LOC_PNG_PATH)

include $(CLEAR_VARS)

LOCAL_MODULE := libpng
LOCAL_SRC_FILES := include/png.c \
                   include/pngerror.c \
                   include/pngget.c \
                   include/pngmem.c \
                   include/pngpread.c \
                   include/pngread.c \
                   include/pngrio.c \
                   include/pngrtran.c \
                   include/pngrutil.c \
                   include/pngset.c \
                   include/pngtest.c \
                   include/pngtrans.c \
                   include/pngvalid.c \
                   include/pngwio.c \
                   include/pngwrite.c \
                   include/pngwtran.c \
                   include/pngwutil.c

LOCAL_C_INCLUDES := $(LOC_PNG_PATH)/include
LOCAL_EXPORT_C_INCLUDES := $(LOC_PNG_PATH)/include
LOCAL_CFLAGS :=

include $(BUILD_STATIC_LIBRARY)
