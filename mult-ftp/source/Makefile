CC = gcc

SOURCE_DIR = $(PWD)
ROOT_DIR = $(SOURCE_DIR)/../..
COMPONENTS_DIR = $(ROOT_DIR)/components
COMMON_DIR = $(ROOT_DIR)/common

APP = mftp

BUILD_DIR = ../build

SRCS  = $(SOURCE_DIR)/main.c
SRCS += $(SOURCE_DIR)/fileopt/*.c
SRCS += $(COMPONENTS_DIR)/structure/*.c
SRCS += $(COMPONENTS_DIR)/transmission/*.c
SRCS += $(COMPONENTS_DIR)/transmission/serial/*.c
SRCS += $(COMPONENTS_DIR)/transmission/udp/*.c
SRCS += $(COMPONENTS_DIR)/transmission/tcp/*.c
SRCS += $(COMPONENTS_DIR)/dirtraval/*.c

CFLAGS  = -Wall -Werror
CFLAGS += -g
CFLAGS += -O0
CFLAGS += -I $(SOURCE_DIR)/fileopt
CFLAGS += -I $(COMMON_DIR)
CFLAGS += -I $(COMPONENTS_DIR)/structure
CFLAGS += -I $(COMPONENTS_DIR)/transmission
CFLAGS += -I $(COMPONENTS_DIR)/transmission/msg
CFLAGS += -I $(COMPONENTS_DIR)/transmission/serial
CFLAGS += -I $(COMPONENTS_DIR)/transmission/udp
CFLAGS += -I $(COMPONENTS_DIR)/transmission/tcp
CFLAGS += -I $(COMPONENTS_DIR)/dirtraval

ALL : compile_check compile

clean : clean_build

include $(ROOT_DIR)/env.mk
