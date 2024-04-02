CC = gcc

SOURCE_DIR = $(PWD)
ROOT_DIR = $(SOURCE_DIR)/../../..
COMPONENTS_DIR = $(ROOT_DIR)/components
COMMON_DIR = $(ROOT_DIR)/common

APP = cli

BUILD_DIR = ./build

SRCS  = $(SOURCE_DIR)/main.c
SRCS += $(SOURCE_DIR)/../*.c
SRCS += $(COMPONENTS_DIR)/structure/*.c

CFLAGS  = -g
CFLAGS += -I $(COMMON_DIR)
CFLAGS += -I $(COMPONENTS_DIR)/structure
CFLAGS += -I $(SOURCE_DIR)/..

ALL : compile_check compile

clean : clean_build

include $(ROOT_DIR)/env.mk
