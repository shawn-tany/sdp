CC = gcc

CUR_DIR = $(PWD)
ROOT_DIR = $(CUR_DIR)/../../..
COMPONENTS_DIR = $(ROOT_DIR)/components

APP = dirtraval

BUILD_DIR = ./build

SRCS  = $(CUR_DIR)/*.c
SRCS += $(CUR_DIR)/../*.c
SRCS += $(COMPONENTS_DIR)/structure/sdp_queue.c
SRCS += $(COMPONENTS_DIR)/structure/sdp_stack.c

CFLAGS  = -Wall -Werror
CFLAGS += -g
CFLAGS += -O0
CFLAGS += -I $(CUR_DIR)/..
CFLAGS += -I $(COMPONENTS_DIR)/structure

ALL : compile_check compile

clean : clean_build

include $(ROOT_DIR)/env.mk
