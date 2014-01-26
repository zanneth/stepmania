LOC_LUA_PATH:= $(call my-dir)
LOCAL_PATH := $(LOC_LUA_PATH)

include $(CLEAR_VARS)

LOCAL_MODULE:= lua
LOCAL_SRC_FILES := src/lauxlib.c \
		  src/lmathlib.c \
		  src/loslib.c \
		  src/ltable.c \
		  src/print.c \
		  src/llex.c \
		  src/lvm.c \
		  src/lapi.c \
		  src/lobject.c \
		  src/ltm.c \
		  src/lundump.c \
		  src/lopcodes.c \
		  src/luac.c \
		  src/lbaselib.c \
		  src/lgc.c \
		  src/ltablib.c \
		  src/loadlib.c \
		  src/ldump.c \
		  src/lcode.c \
		  src/linit.c \
		  src/lzio.c \
		  src/lstate.c \
		  src/lparser.c \
		  src/ldo.c \
		  src/lstring.c \
		  src/lua.c \
		  src/lmem.c \
		  src/ldblib.c \
		  src/lstrlib.c \
		  src/liolib.c \
		  src/lfunc.c \
		  src/ldebug.c \
		  
LOCAL_C_INCLUDES := $(LOC_LUA_PATH)/src
LOCAL_EXPORT_C_INCLUDES := $(LOC_LUA_PATH)/src

include $(BUILD_STATIC_LIBRARY)
