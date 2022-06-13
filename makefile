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
CFLAGS:=$(CFLAGS)
LIBTOOL:=$(LIBTOOL)
LIB-DEPENDS:=gstreamer-1.0 glib-2.0 gio-2.0

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
	CC:=gcc
	PKGCONFIG=pkg-config
endif

ifeq ($(OS), Darwin)
	CC:=clang
	PKGCONFIG=/usr/local/bin/pkg-config
endif

# 增加GLib和GStreamer的C连接库和头文件
ifdef PKG_CONFIG_PREFIX
CFLAGS += $(shell $(PKGCONFIG) --define-variable=prefix=$(PKG_CONFIG_PREFIX) --cflags --libs $(LIB-DEPENDS))
else
CFLAGS += $(shell $(PKGCONFIG) --cflags --libs $(LIB-DEPENDS))
endif


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