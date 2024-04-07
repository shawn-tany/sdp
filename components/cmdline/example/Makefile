CC = gcc

CUR_DIR = $(PWD)
ROOT_DIR = $(CUR_DIR)/../../..
COMPONENTS_DIR = $(ROOT_DIR)/components
COMMON_DIR = $(ROOT_DIR)/common

APP = cli

BUILD_DIR = ./build

SRCS  = $(CUR_DIR)/main.c
SRCS += $(CUR_DIR)/../sdp_cli.c
SRCS += $(CUR_DIR)/../sdp_cli_ch.c
SRCS += $(CUR_DIR)/../sdp_cli_cmd.c
SRCS += $(CUR_DIR)/../sdp_cli_cmdfunc.c
SRCS += $(CUR_DIR)/../sdp_cli_line.c
SRCS += $(COMPONENTS_DIR)/structure/sdp_trie.c
SRCS += $(COMPONENTS_DIR)/structure/sdp_stack.c

CFLAGS  = -g
CFLAGS += -I $(COMMON_DIR)
CFLAGS += -I $(COMPONENTS_DIR)/structure
CFLAGS += -I $(CUR_DIR)/..

ALL : compile_check compile

clean : clean_build

include $(ROOT_DIR)/env.mk
