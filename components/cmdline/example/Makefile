CC = gcc

ifeq (${ROOT_DIR},)
	ROOT_DIR = $(PWD)/../../..
endif

SOURCE_DIR = $(ROOT_DIR)/components/cmdline/example

COMPONENTS_DIR = $(ROOT_DIR)/components
COMMON_DIR = $(ROOT_DIR)/common

APP = cli

BUILD_DIR = $(SOURCE_DIR)/build

SRCS  = $(SOURCE_DIR)/main.c
SRCS += $(wildcard $(SOURCE_DIR)/../*.c)
SRCS += $(COMPONENTS_DIR)/structure/sdp_trie.c
SRCS += $(COMPONENTS_DIR)/structure/sdp_stack.c

CFLAGS  = -g
CFLAGS += -I $(COMMON_DIR)
CFLAGS += -I $(COMPONENTS_DIR)/structure
CFLAGS += -I $(SOURCE_DIR)/..

ALL : compile_check compile

clean : clean_build

include $(ROOT_DIR)/env.mk
