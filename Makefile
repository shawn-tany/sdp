ROOT_DIR := $(PWD)

## components #
SUB_DIRS = 

include .config

ifeq (${COMPONENTS_BOARDINFO}, yes)
	SUB_DIRS += $(ROOT_DIR)/components/boardinfo/example/ 
endif

ifeq (${COMPONENTS_CMDLINE}, yes)
	SUB_DIRS += $(ROOT_DIR)/components/cmdline/example/ 
endif

ifeq (${COMPONENTS_DIRTRAVER}, yes)
	SUB_DIRS += $(ROOT_DIR)/components/dirtraval/example/ 
endif

ifeq (${COMPONENTS_THREADPOOL}, yes)
	SUB_DIRS += $(ROOT_DIR)/components/threadpool/example/ 
endif

ifeq (${COMPONENTS_TRANSMISSION_BASE_TCP_SERVER}, yes)
	SUB_DIRS += $(ROOT_DIR)/components/transmission/base/tcp/server/example/ 
endif

ifeq (${COMPONENTS_TRANSMISSION_BASE_TCP_CLIENT}, yes)
	SUB_DIRS += $(ROOT_DIR)/components/transmission/base/tcp/client/example/ 
endif

MAKE = make

MAKE_FLAGS = --no-print-directory 

ALL : 
	@for subdir in ${SUB_DIRS};														\
	do																				\
		${MAKE} ${MAKE_FLAGS} -C $${subdir} ROOT_DIR=${ROOT_DIR} || (exit 3);		\
	done

clean : 
	@for subdir in ${SUB_DIRS};														\
	do																				\
		${MAKE} ${MAKE_FLAGS} -C $${subdir} clean ROOT_DIR=${ROOT_DIR} || (exit 3);	\
	done
