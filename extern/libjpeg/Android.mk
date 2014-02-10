LOC_JPEG_PATH := $(call my-dir)
LOCAL_PATH := $(LOC_JPEG_PATH)

include $(CLEAR_VARS)

LOCAL_MODULE:= libjpeg
LOCAL_SRC_FILES := cdjpeg.c \
		  cjpeg.c \
		  ckconfig.c \
		  example.c \
		  jaricom.c \
		  jcapimin.c \
		  jcapistd.c \
		  jcarith.c \
		  jccoefct.c \
		  jccolor.c \
		  jcdctmgr.c \
		  jchuff.c \
		  jcinit.c \
		  jcmainct.c \
		  jcmarker.c \
		  jcmaster.c \
		  jcomapi.c \
		  jcparam.c \
		  jcprepct.c \
		  jcsample.c \
		  jctrans.c \
		  jdapimin.c \
		  jdapistd.c \
		  jdarith.c \
		  jdatadst.c \
		  jdatasrc.c \
		  jdcoefct.c \
		  jdcolor.c \
		  jddctmgr.c \
		  jdhuff.c \
		  jdinput.c \
		  jdmainct.c \
		  jdmarker.c \
		  jdmaster.c \
		  jdmerge.c \
		  jdpostct.c \
		  jdsample.c \
		  jdtrans.c \
		  jerror.c \
		  jfdctflt.c \
		  jfdctfst.c \
		  jfdctint.c \
		  jidctflt.c \
		  jidctfst.c \
		  jidctint.c \
		  jmemansi.c \
		  jmemmgr.c \
		  jmemname.c \
		  jmemnobs.c \
		  jquant1.c \
		  jquant2.c \
		  jutils.c \
		  

LOCAL_C_INCLUDES := $(LOC_JPEG_PATH)
LOCAL_EXPORT_C_INCLUDES := $(LOC_JPEG_PATH)

include $(BUILD_STATIC_LIBRARY)
