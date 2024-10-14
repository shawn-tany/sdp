CC = gcc

ifeq (${ROOT_DIR},)
	ROOT_DIR = $(PWD)/../../..
endif

SOURCE_DIR = $(ROOT_DIR)/components/event/example
COMMON_DIR = $(ROOT_DIR)/common
COMPONENTS_DIR = $(ROOT_DIR)/components

BUILD_DIR = $(SOURCE_DIR)/build
APP = event_monitor

SRCS  = $(SOURCE_DIR)/main.c
SRCS += $(wildcard $(SOURCE_DIR)/../*.c)
SRCS += $(wildcard $(COMMON_DIR)/*.c)
SRCS += $(wildcard $(COMPONENTS_DIR)/ipc/pqueue/*.c)

CFLAGS  = -Wall -Werror
CFLAGS += -g
CFLAGS += -O0
CFLAGS += -I $(SOURCE_DIR)/..
CFLAGS += -I $(COMMON_DIR)
CFLAGS += -I $(COMPONENTS_DIR)/ipc/pqueue

ALL : compile_check compile

clean : clean_build

include $(ROOT_DIR)/env.mk
