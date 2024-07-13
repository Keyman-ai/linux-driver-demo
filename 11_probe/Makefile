export ARCH=arm64#设置平台架构
export CROSS_COMPILE=aarch64-linux-gnu-#交叉编译器前缀
obj-m += probe.o	#platform_device.c对应.o文件的名称。名称要保持一致。
KDIR :=/home/topeet/Linux/linux_sdk/kernel    #内核源码所在虚拟机ubuntu的实际路径
PWD ?= $(shell pwd)
all:
	make -C $(KDIR) M=$(PWD) modules    #make操作
clean:
	make -C $(KDIR) M=$(PWD) clean    #make clean操作

