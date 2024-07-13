#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/ioport.h>

#define MEM_START_ADDR 0xFDD60000
#define MEM_END_ADDR   0xFDD60004
#define IRQ_NUMBER     101

static struct resource my_resources[] = {
    {
        .start = MEM_START_ADDR,    // 内存资源起始地址
        .end = MEM_END_ADDR,        // 内存资源结束地址
        .flags = IORESOURCE_MEM,    // 标记为内存资源
    },
    {
        .start = IRQ_NUMBER,        // 中断资源号
        .end = IRQ_NUMBER,          // 中断资源号
        .flags = IORESOURCE_IRQ,    // 标记为中断资源
    },
};

static void my_platform_device_release(struct device *dev)
{
    // 释放资源的回调函数
}

static struct platform_device my_platform_device = {
    .name = "my_platform_device",                  // 设备名称
    .id = -1,                                      // 设备ID
    .num_resources = ARRAY_SIZE(my_resources),     // 资源数量
    .resource = my_resources,                      // 资源数组
    .dev.release = my_platform_device_release,     // 释放资源的回调函数
};

static int __init my_platform_device_init(void)
{
    int ret;

    ret = platform_device_register(&my_platform_device);   // 注册平台设备
    if (ret) {
        printk(KERN_ERR "Failed to register platform device\n");
        return ret;
    }

    printk(KERN_INFO "Platform device registered\n");
    return 0;
}

static void __exit my_platform_device_exit(void)
{
    platform_device_unregister(&my_platform_device);   // 注销平台设备
    printk(KERN_INFO "Platform device unregistered\n");
}

module_init(my_platform_device_init);
module_exit(my_platform_device_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("topeet");

