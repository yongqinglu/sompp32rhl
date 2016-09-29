#!/usr/bin/env make

TARGET		=Terminal


################ ONLY CHANGE STUFF BELOW IF YOU REALLY HAVE ###################


CC			=g++
CFLAGS		=-Wno-endif-labels -O2 $(DBG_FLAGS) $(INCLUDES)
LDFLAGS		=-enable-auto-import $(LIBRARIES)

INSTALL		=install
############ global stuff -- overridden by ../Makefile

ROOT_DIR	?= $(PWD)/../..
BUILD_DIR   ?= $(ROOT_DIR)/build
SRC_DIR		?= $(ROOT_DIR)/src
ST_DIR		?= $(ROOT_DIR)/Smalltalk
EX_DIR		?= $(ROOT_DIR)/Examples
TEST_DIR	?= $(ROOT_DIR)/TestSuite
DIR_OMR ?= $(ROOT_DIR)/../omr32
############# include path
CFLAGS +=-I${SRC_DIR}/glue
CFLAGS +=-I${DIR_OMR}/include_core
CFLAGS +=-I${DIR_OMR}/gc/include
CFLAGS +=-I${DIR_OMR}/gc/base
CFLAGS +=-I${DIR_OMR}/gc/verbose
CFLAGS +=-I${DIR_OMR}/gc/verbose/handler_standard
CFLAGS +=-I${DIR_OMR}/gc/stats
CFLAGS +=-I${DIR_OMR}/gc/structs
CFLAGS +=-I${DIR_OMR}/gc/base/standard
CFLAGS +=-I${DIR_OMR}/gc/startup

INCLUDES		=-I$(SRC_DIR)
LIBRARIES		=-L$(ROOT_DIR)

ifneq ($(OS),)
# only Windows has OS predefined.
	LIBS		=-lm -l../../CPPSOM -l../../PrimitiveCore
endif

all: $(TARGET).csp

debug : DBG_FLAGS=-DDEBUG -g
debug: all

profiling : DBG_FLAGS=-g -pg
profiling : LDFLAGS+=-pg
profiling: all

$(TARGET).csp:
	$(CC) $(CFLAGS) $(LDFLAGS) -shared -g \
		$(PWD)/$(TARGET).cpp -o $(TARGET).csp $(LIBS)
	
clean:
	rm -Rf $(TARGET).csp

