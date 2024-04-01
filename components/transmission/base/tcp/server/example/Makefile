CC = gcc

SOURCE_DIR = $(PWD)
ROOT_DIR = $(SOURCE_DIR)/../../../../../..
COMMON_DIR = $(ROOT_DIR)/common
COMPONENTS_DIR = $(ROOT_DIR)/components

APP = tcp_server_demo

BUILD_DIR = ./build

SRCS  = $(SOURCE_DIR)/*.c
SRCS += $(SOURCE_DIR)/../*.c
SRCS += $(COMPONENTS_DIR)/structure/sdp_queue.c
SRCS += $(COMPONENTS_DIR)/threadpool/thread_pool.c

CFLAGS  = -Wall -Werror
CFLAGS += -g
CFLAGS += -O0
CFLAGS += -I $(SOURCE_DIR)/..
CFLAGS += -I $(COMMON_DIR)
CFLAGS += -I $(COMPONENTS_DIR)/structure
CFLAGS += -I $(COMPONENTS_DIR)/threadpool
CFLAGS += -lpthread

ALL : compile_check compile

clean : clean_build

include $(ROOT_DIR)/env.mk
