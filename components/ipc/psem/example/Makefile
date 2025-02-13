CC = gcc

ifeq (${ROOT_DIR},)
	ROOT_DIR = $(PWD)/../../../..
endif

SOURCE_DIR = $(ROOT_DIR)/components/ipc/psem/example

COMPONENTS_DIR = $(ROOT_DIR)/components
COMMON_DIR = $(ROOT_DIR)/common

APP = psem_test

BUILD_DIR = $(SOURCE_DIR)/build

SRCS  = $(SOURCE_DIR)/main.c
SRCS += $(wildcard $(SOURCE_DIR)/../*.c)

CFLAGS  = -g
CFLAGS += -I $(COMMON_DIR)
CFLAGS += -I $(SOURCE_DIR)/..

LIBS = -lpthread

ALL : compile_check compile

clean : clean_build

include $(ROOT_DIR)/env.mk
