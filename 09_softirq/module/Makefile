export ARCH=arm64#设置平台架构
export CROSS_COMPILE=aarch64-linux-gnu-#交叉编译器前缀
obj-m := interrupt.o
KDIR :=/home/topeet/Linux/linux_sdk/kernel    #这里是你的内核目录                                                                                                                            
PWD ?= $(shell pwd)
all:
	make -C $(KDIR) M=$(PWD) modules    #make操作
clean:
	make -C $(KDIR) M=$(PWD) clean    #make clean操作
