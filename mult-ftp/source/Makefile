CC = gcc

APP = mftp

COMPONENTS_DIR = ../components
COMMON_DIR = ../common
SOURCE_DIR = .

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

ALL :
	$(CC) $(SRCS) $(CFLAGS) -o $(APP)

clean :
	rm -rf $(APP)
