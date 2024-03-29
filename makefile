#########################################
# 
# Quark Player Makefile
#
# @author aokihu <aokihu@gmail.com>
# @version v2.2.2
#
#########################################

#
# 将命令程序全部用变量代替
#
OS:=$(OS)
CC:=$(CC)
PKGCONFIG:=$(PKGCONFIG)
CFLAGS:=$(CFLAGS)
LIBTOOL:=$(LIBTOOL)
PKG_LIB_DEPENDS:=gstreamer-1.0 glib-2.0 gio-2.0

#
# 检测OS名称,Darwin(Mac OS) - Linux(Linux)
#
ifndef OS
OS:=$(shell uname)
endif

# 
# CFLAGS 参数设置
# 
CFLAGS += -Wall -std=c11

ifdef DEBUG
CFLAGS += -ggdb -DDEBUG
endif



# 编译器针对操作系统进行设定
ifeq ($(OS), Linux)
	ifndef CC
		CC:=gcc
	endif

	ifndef PKGCONFIG
		PKGCONFIG=pkg-config
	endif
endif

ifeq ($(OS), Darwin)
	ifndef CC
		CC:=clang
	endif
	ifndef PKGCONFIG
		PKGCONFIG=/usr/local/bin/pkg-config
	endif
endif

# 增加GLib和GStreamer的C连接库和头文件
CFLAGS += $(shell $(PKGCONFIG) --cflags --libs $(PKG_LIB_DEPENDS))

# 添加CFLAGS参数
CC += $(CFLAGS)

# 连接器根据操作系统进行设定
ifeq ($(OS), Linux)
	ifndef LIBTOOL
		LIBTOOL:=libtool
	endif
  COMPLIER:=$(LIBTOOL) --mode=link --tag=CXX $(CC)
else
  COMPLIER:=$(CC)
endif


# 
# 目录变量设定
# 
obj := obj
src := ./
vpath %.h $(src)/headers
vpath %.c $(src)/libs
vpath %.c $(src)
DIRS := obj

targets := main
targets += $(basename $(shell ls $(src)/libs))
objects := $(addsuffix .o,$(targets))

# 主生成目标
# 生成名字是terminal的程序文件
quark : $(DIRS) $(objects)
	$(COMPLIER) -o quark $(addprefix $(obj)/, $(objects));

$(DIRS):
	mkdir $@

# 生成object文件
$(objects) : %.o : %.c
	$(CC) -c $< -o $(obj)/$@

.PHONY : clean test

test:
	@echo $(OS)
	@echo $(CC)
	@echo $(LIBTOOL)

clean:
	rm -rf $(DIRS)
	rm quark