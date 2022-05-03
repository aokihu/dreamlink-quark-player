# 检测OS名称,Darwin(Mac OS) - Linux(Linux)
OS := $(shell uname)

# 
# CFLAGS 参数设置
# 
CFLAGS += -Wall -std=c11

ifdef DEBUG
CFLAGS += -g
endif

# 编译器针对操作系统进行设定
ifeq ($(OS), Linux)
	cc := gcc
	PKGCONFIG=pkg-config
endif

ifeq ($(OS), Darwin)
	cc := clang
	PKGCONFIG=/usr/local/bin/pkg-config
endif


# 交叉编译器前缀
ifdef CROSS_COMPLIER
	CC = $(CROSS_COMPLIER)-$(cc)
else 
	CC = $(cc)
endif

# 增加GLib和GStreamer的C连接库和头文件
CFLAGS += $(shell $(PKGCONFIG) --cflags --libs gstreamer-1.0 glib-2.0 gio-2.0)
# 添加CFLAGS参数
CC += $(CFLAGS)

# 连接器根据操作系统进行设定
ifeq ($(OS), Linux)
  LD := libtool --mode=link $(CC)
else
  LD := $(CC)
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
	$(LD) -o quark $(addprefix $(obj)/, $(objects));

$(DIRS):
	mkdir $@

# 生成object文件
$(objects) : %.o : %.c
	$(CC) -c $< -o $(obj)/$@

.PHONY : clean

clean:
	rm -rf $(DIRS)