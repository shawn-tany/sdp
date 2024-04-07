CC = gcc

CUR_DIR = $(PWD)
ROOT_DIR = $(CUR_DIR)/../../..

BUILD_DIR = ./build
APP = binfo

CREATE_LIB_DIR = ../lib
CREATE_LIB = boardinfo-linux

OUTSRCS = $(CUR_DIR)/main.c

SRCS  = $(CUR_DIR)/main.c
SRCS += $(CUR_DIR)/../boardinfo.c

CFLAGS  = -Wall -Werror
CFLAGS += -g
CFLAGS += -O0
CFLAGS += -I $(CUR_DIR)/..

ALL : build_lib_check build_lib compile_check compile

clean : clean_build

include $(ROOT_DIR)/env.mk
