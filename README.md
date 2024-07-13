# linux-driver-demo
## Linux驱动框架
Linux 设备驱动程序是 Linux 内核中用于与硬件设备交互的模块。它们提供了硬件设备和操作系统之间的接口，使得应用程序能够通过标准的系统调用来访问硬件设备。以下是 Linux 设备驱动框架的概述，包括常见的驱动类型和驱动程序的基本结构。
### Linux 设备驱动框架
设备驱动类型
字符设备驱动程序（Character Device Drivers）:

处理字符流，如串口设备、键盘、鼠标等。
提供了 open, read, write, close, ioctl 等操作。

### 块设备驱动程序（Block Device Drivers）:

处理块数据，如硬盘、SSD 等。
提供了 open, close, ioctl, read, write, request 等操作。
使用缓冲区来优化读写性能。

### 网络设备驱动程序（Network Device Drivers）:

处理网络数据包传输，如以太网卡、Wi-Fi 适配器等。
提供了 open, stop, hard_start_xmit, get_stats, set_mac_address, do_ioctl 等操作。

### 驱动程序的基本结构
Linux 设备驱动程序通常包括以下几个部分：

初始化和清理:

module_init 和 module_exit 宏用于指定模块的初始化和清理函数，也是驱动的出入口，当用户态执行insmod和rmmod命令时会触发module_init和module_exit函数

### 设备文件操作:

定义设备文件操作结构体 file_operations，包含打开、读取、写入、关闭等操作。

### 设备注册和注销:

注册字符设备 register_chrdev。
注册块设备 register_blkdev。
注册网络设备 register_netdev。

### 中断处理:

注册中断处理程序 request_irq。

### 示例代码
以下是一个简单的字符设备驱动程序示例：

```c
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "mychardev"
#define BUFFER_SIZE 1024

static int major_number;
static char buffer[BUFFER_SIZE];
static int buffer_size = 0;

static int dev_open(struct inode *inodep, struct file *filep) {
    printk(KERN_INFO "Device opened\n");
    return 0;
}

static int dev_release(struct inode *inodep, struct file *filep) {
    printk(KERN_INFO "Device closed\n");
    return 0;
}

static ssize_t dev_read(struct file *filep, char *user_buffer, size_t len, loff_t *offset) {
    int bytes_to_read = len < buffer_size ? len : buffer_size;
    if (copy_to_user(user_buffer, buffer, bytes_to_read)) {
        return -EFAULT;
    }
    buffer_size = 0;  // Clear the buffer after reading
    return bytes_to_read;
}

static ssize_t dev_write(struct file *filep, const char *user_buffer, size_t len, loff_t *offset) {
    int bytes_to_write = len < BUFFER_SIZE ? len : BUFFER_SIZE;
    if (copy_from_user(buffer, user_buffer, bytes_to_write)) {
        return -EFAULT;
    }
    buffer_size = bytes_to_write;
    return bytes_to_write;
}

static struct file_operations fops = {
    .open = dev_open,
    .read = dev_read,
    .write = dev_write,
    .release = dev_release,
};

static int __init mychardev_init(void) {
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        printk(KERN_ALERT "Failed to register a major number\n");
        return major_number;
    }
    printk(KERN_INFO "Registered correctly with major number %d\n", major_number);
    return 0;
}

static void __exit mychardev_exit(void) {
    unregister_chrdev(major_number, DEVICE_NAME);
    printk(KERN_INFO "Device unregistered\n");
}

module_init(mychardev_init);
module_exit(mychardev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple Linux char driver");
MODULE_VERSION("0.1");
```

### 详细说明各部分内容：
初始化和清理函数:

mychardev_init 是模块加载时调用的初始化函数，使用 module_init 宏指定。
mychardev_exit 是模块卸载时调用的清理函数，使用 module_exit 宏指定。
设备文件操作函数:

dev_open 和 dev_release 用于处理设备文件的打开和关闭操作。
dev_read 和 dev_write 分别处理设备文件的读取和写入操作。
文件操作结构体:

file_operations 结构体 fops 定义了设备文件的操作函数。
设备注册和注销:

在初始化函数中调用 register_chrdev 注册字符设备，并在清理函数中调用 unregister_chrdev 注销设备。
这个简单的字符设备驱动程序展示了如何实现基本的文件操作，包括打开、读取、写入和关闭设备文件。你可以根据实际需求扩展这个示例，添加更多的功能和处理逻辑。

#### 其他具体的驱动代码请详见本仓库code




