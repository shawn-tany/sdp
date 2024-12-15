CC = gcc

ifeq (${ROOT_DIR},)
	ROOT_DIR = $(PWD)/../../../../..
endif

SOURCE_DIR = $(ROOT_DIR)/components/transmission/base/udp/example

COMMON_DIR = $(ROOT_DIR)/common
COMPONENTS_DIR = $(ROOT_DIR)/components

APP = udp_demo

BUILD_DIR = $(SOURCE_DIR)/build

SRCS  = $(SOURCE_DIR)/main.c
SRCS += $(SOURCE_DIR)/../udp.c

CFLAGS  = -Wall -Werror
CFLAGS += -g
CFLAGS += -O0
CFLAGS += -I $(SOURCE_DIR)/..
CFLAGS += -I $(COMMON_DIR)

ALL : compile_check compile

clean : clean_build

include $(ROOT_DIR)/env.mk
