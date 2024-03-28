CC = gcc

SOURCE_DIR = $(PWD)
ROOT_DIR = $(SOURCE_DIR)/../../..

APP = binfo

BUILD_DIR = ./build

SRCS  = $(SOURCE_DIR)/*.c
SRCS += $(SOURCE_DIR)/../*.c

CFLAGS  = -Wall -Werror
CFLAGS += -g
CFLAGS += -O0
CFLAGS += -I $(SOURCE_DIR)/..

ALL : compile_check compile

clean : clean_build

include $(ROOT_DIR)/env.mk
