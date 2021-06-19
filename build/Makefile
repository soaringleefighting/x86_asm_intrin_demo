#########################################################################
## Function: makefile for x86_asm_intrin_demo project
## Platform: linux
## Author: 	 lipeng
## Modified: 2020.9.1 created
#########################################################################

platform ?= x86_32
PUREC ?= 0
DEBUG ?= 0

#包含编译配置(系统或架构相关宏以及编译链接选项等配置)
include config.mk

#指定后缀名和伪目标
.SUFFIXES: .c,.o,.cpp,.S,.s  
.PHONY: all,clean

#设置相关路径
vpath %.h ./
ROOTSRC = ./../x86_asm_demo
DEMOSRC = ./../x86_asm_demo
SRCDIR  = ./../x86_asm_demo
INCLUDES = ./../x86_asm_demo 
INCLUDES += -I./../x86_asm_demo

#设置工具的相关参数
ifdef USE_EMCC
BIN_TARGET = ./../../bin/demo.js
else
BIN_TARGET = ./../bin/demo
endif

#设置编译的相关参数
CFLAGS := -Wall $(DEBUG_FLAGS) -I$(INCLUDES) -w
CFLAGS += $(EXTRA_CFLAGS) 

CXXFLAGS := -Wall   

LDFLAGS := -Wall
LDFLAGS += $(EXTRA_LFLAGS) 

ARFLAGS := -crus 

ASMFLAGS += $(EXTRA_AFLAGS)

#以下获取OBJS文件的方式适用于含有少量文件的编译
SRCS = $(DEMOSRC)/os_time_sdk.c			      
SRCS += $(DEMOSRC)/demo.c  							\
		$(SRCDIR)/ff_pred_dc.c						
			
ifeq ($(platform), x86_32)
SRCS += $(SRCDIR)/ff_pred_dc_sse2_intrinsic.c
ifeq ($(PUREC), 0)
SRCS += $(SRCDIR)/ff_pred_dc_x86.asm				\
		$(SRCDIR)/ff_pred_dc_x86_org.asm
endif
endif

ifeq ($(platform), arm64)
SRCS += $(SRCDIR)/ff_pred_dc_neon_intrinsic.c
endif

#ifeq ($(platform), x86_64)
#SRCS += $(SRCDIR)/x86/x264_lowres_transpose_x86.asm
#endif

	   
OBJS = $(patsubst %.S,%.o, $(patsubst %.asm,%.o, $(patsubst %.cpp,%.o, $(patsubst %.c,%.o,$(SRCS)))))

#编译规则
all: clean $(BIN_TARGET)
$(BIN_TARGET):	$(OBJS)
#	$(error $(SRCS))
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) -c $< $(CFLAGS) -o $@
%.o: %.cpp
	$(CXX) -c $< $(CXXFLAGS) -o $@
		
%.o:%.asm
	$(ASM) $< $(ASMFLAGS) -o $@	
%.o:%.S
	$(ASM) $< $(ASMFLAGS) -c -o $@
%.o:%.s
	$(ASM) $< $(ASMFLAGS) -c -o $@	
		
clean:
	-rm	$(OBJS)
	-rm $(BIN_TARGET)
