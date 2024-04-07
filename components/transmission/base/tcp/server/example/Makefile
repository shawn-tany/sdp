CC = gcc

CUR_DIR = $(PWD)
ROOT_DIR = $(CUR_DIR)/../../../../../..
COMMON_DIR = $(ROOT_DIR)/common
COMPONENTS_DIR = $(ROOT_DIR)/components

APP = tcp_server_demo

BUILD_DIR = ./build

SRCS  = $(CUR_DIR)/main.c
SRCS += $(CUR_DIR)/../tcp_server.c
SRCS += $(COMPONENTS_DIR)/structure/sdp_queue.c
SRCS += $(COMPONENTS_DIR)/threadpool/thread_pool.c

CFLAGS  = -Wall -Werror
CFLAGS += -g
CFLAGS += -O0
CFLAGS += -I $(CUR_DIR)/..
CFLAGS += -I $(COMMON_DIR)
CFLAGS += -I $(COMPONENTS_DIR)/structure
CFLAGS += -I $(COMPONENTS_DIR)/threadpool
CFLAGS += -lpthread

ALL : compile_check compile

clean : clean_build

include $(ROOT_DIR)/env.mk
