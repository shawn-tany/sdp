CC = gcc

ifeq (${ROOT_DIR},)
	ROOT_DIR = $(PWD)/../../..
endif

SOURCE_DIR = $(ROOT_DIR)/components/logger/example
COMMON_DIR = $(ROOT_DIR)/common

BUILD_DIR = $(SOURCE_DIR)/build
APP = logger_print

SRCS  = $(SOURCE_DIR)/main.c
SRCS += $(wildcard $(SOURCE_DIR)/../*.c)
SRCS += $(COMMON_DIR)/module.c

CFLAGS  = -Wall -Werror
CFLAGS += -g
CFLAGS += -O0
CFLAGS += -I $(SOURCE_DIR)/..
CFLAGS += -I $(COMMON_DIR)

ALL : compile_check compile

clean : clean_build

include $(ROOT_DIR)/env.mk
