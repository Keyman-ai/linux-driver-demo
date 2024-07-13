export ARCH=arm64
export CROSS_COMPILE=aarch64-linux-gnu-
obj-m += wq.o   
KDIR :=/home/topeet/Linux/linux_sdk/kernel    
PWD ?= $(shell pwd)
all:
	make -C $(KDIR) M=$(PWD) modules    
clean:
	make -C $(KDIR) M=$(PWD) clean    
