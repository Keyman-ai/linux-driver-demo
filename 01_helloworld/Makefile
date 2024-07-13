#!/bin/bash
export ARCH=arm64
export CROSS_COMPILE=aarch64-linux-gnu-
obj-m += helloworld.o    #此处要和你的驱动源文件同名
KDIR :=/home/topeet/Linux/rk356x_linux/kernel    #这里是你的内核目录
PWD ?= $(shell pwd)
all:
	make -C $(KDIR) M=$(PWD) modules    #make操作
clean:
	make -C $(KDIR) M=$(PWD) clean    #make clean操作
