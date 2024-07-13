#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/ioport.h>

static int my_platform_driver_probe(struct platform_device *pdev)
{
    struct resource *res_mem, *res_irq;

    // 方法1：直接访问 platform_device 结构体的资源数组
    if (pdev->num_resources >= 2) {
        struct resource *res_mem = &pdev->resource[0];
        struct resource *res_irq = &pdev->resource[1];

        // 使用获取到的硬件资源进行处理
        printk("Method 1: Memory Resource: start = 0x%llx, end = 0x%llx\n",
                res_mem->start, res_mem->end);
        printk("Method 1: IRQ Resource: number = %lld\n", res_irq->start);
    }

    // 方法2：使用 platform_get_resource() 获取硬件资源
    res_mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    if (!res_mem) {
        dev_err(&pdev->dev, "Failed to get memory resource\n");
        return -ENODEV;
    }

    res_irq = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
    if (!res_irq) {
        dev_err(&pdev->dev, "Failed to get IRQ resource\n");
        return -ENODEV;
    }

    // 使用获取到的硬件资源进行处理
    printk("Method 2: Memory Resource: start = 0x%llx, end = 0x%llx\n",
            res_mem->start, res_mem->end);
    printk("Method 2: IRQ Resource: number = %lld\n", res_irq->start);

    return 0;
}

static int my_platform_driver_remove(struct platform_device *pdev)
{
    // 设备移除操作
    return 0;
}

static struct platform_driver my_platform_driver = {
    .driver = {
        .name = "my_platform_device", // 与 platform_device.c 中的设备名称匹配
        .owner = THIS_MODULE,
    },
    .probe = my_platform_driver_probe,
    .remove = my_platform_driver_remove,
};

static int __init my_platform_driver_init(void)
{
    int ret;

    ret = platform_driver_register(&my_platform_driver); // 注册平台驱动
    if (ret) {
        printk("Failed to register platform driver\n");
        return ret;
    }

    printk("Platform driver registered\n");
    return 0;
}

static void __exit my_platform_driver_exit(void)
{
    platform_driver_unregister(&my_platform_driver); // 注销平台驱动
    printk("Platform driver unregistered\n");
}

module_init(my_platform_driver_init);
module_exit(my_platform_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("enlai");
