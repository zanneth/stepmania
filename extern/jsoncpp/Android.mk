LOC_JSON_PATH:= $(call my-dir)
LOCAL_PATH := $(LOC_JSON_PATH)

include $(CLEAR_VARS)

LOCAL_MODULE:= jsoncpp
LOCAL_SRC_FILES := src/lib_json/json_reader.cpp \
		   src/lib_json/json_value.cpp \
		   src/lib_json/json_writer.cpp

LOCAL_C_INCLUDES := $(LOC_JSON_PATH)/include
LOCAL_EXPORT_C_INCLUDES := $(LOC_JSON_PATH)/include

include $(BUILD_STATIC_LIBRARY)
