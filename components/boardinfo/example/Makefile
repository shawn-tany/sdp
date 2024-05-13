CC = gcc

ifeq (${ROOT_DIR},)
	ROOT_DIR = $(PWD)/../../..
endif

SOURCE_DIR = $(ROOT_DIR)/components/boardinfo/example

BUILD_DIR = $(SOURCE_DIR)/build
APP = binfo

CREATE_LIB_DIR = $(SOURCE_DIR)/../lib
CREATE_LIB = boardinfo-linux

OUTSRCS = $(SOURCE_DIR)/main.c

SRCS  = $(SOURCE_DIR)/main.c
SRCS += $(SOURCE_DIR)/../boardinfo.c

CFLAGS  = -Wall -Werror
CFLAGS += -g
CFLAGS += -O0
CFLAGS += -I $(SOURCE_DIR)/..

ALL : build_lib_check build_lib compile_check compile

clean : clean_build

include $(ROOT_DIR)/env.mk
