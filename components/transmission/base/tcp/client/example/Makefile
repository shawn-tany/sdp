CC = gcc

ifeq (${ROOT_DIR},)
	ROOT_DIR = $(PWD)/../../../../../..
endif

SOURCE_DIR = $(ROOT_DIR)/components/transmission/base/tcp/client/example

COMMON_DIR = $(ROOT_DIR)/common
COMPONENTS_DIR = $(ROOT_DIR)/components

APP = tcp_client_demo

BUILD_DIR = $(SOURCE_DIR)/build

SRCS  = $(SOURCE_DIR)/main.c
SRCS += $(SOURCE_DIR)/../tcp_client.c
SRCS += $(COMPONENTS_DIR)/structure/sdp_queue.c

CFLAGS  = -Wall -Werror
CFLAGS += -g
CFLAGS += -O0
CFLAGS += -I $(SOURCE_DIR)/..
CFLAGS += -I $(COMMON_DIR)
CFLAGS += -I $(COMPONENTS_DIR)/structure

ALL : compile_check compile

clean : clean_build

include $(ROOT_DIR)/env.mk
