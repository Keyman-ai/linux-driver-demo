#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/platform.h>
#include <linux/of.h>
#include <linux/pwm.h>
#include <linux/uaccess.h>
#include <linux/timer.h>
#include <linux/gpio/consumer.h>
#include <linux/slab.h>



static dev_t dev_num;//定义dev_t类型（32位大小）的变量dev_num,用来存放设备号
static struct cdev cdev_test;//定义cdev结构体类型的变量cdev_test

struct pwm_led_data{
    int sum_count;
    int hight_count;
    struct gpio_desc* gpiod;
    struct hrtimer pwm_timer;
    int time;
};

struct pwm_led_data* data;

enum hrtimer_restart pwm_timer_func(struct hrtimer* timer)
{
    static int timer_count = 0;
    struct pwm_led_data* mydata = container_of(timer, struct pwm_led_data, pwm_timer);

    if(timer_count == mydata->sum_count){
        gpiod_set_vcalue(mydata->gpiod, 1);
        timer_count = 0;
    }

    if(timer_count == mydata->hight_count){
        gpiod_set_vcalue(mydata->gpiod, 0);
        timer_count = 0;
    }

    if(mydata->hight_count == 0){
        timer_count = 0;
    }
    timer_count++;
}

static struct file_operations cdev_test_ops = {
	.owner=THIS_MODULE,//将owner字段指向本模块，可以避免在模块的操作正在被使用时卸载该模块
};//定义file_operations结构体类型的变量cdev_test_ops

const struct of_device_id sg90_of_device_id[] = {
    {.compatible = "sg90"},
    {}
};

struct platform_driver sg90_platform_driver{
    .driver = {
        .name = sg90,
        .of_match_table = sg90_of_device_id,
    },

    .probe = sg90_probe,
    .remove = sg90_remove,

};

int sg90_probe(struct platform_device* dev){
    
    int ret;//定义int类型变量ret，进行函数返回值判断
    int major,minor;//定义int类型的主设备号major和次设备号minor
    ret = alloc_chrdev_region(&dev_num,0,1,"sg90");//自动获取设备号，设备名为chrdev_name
    if (ret < 0){
        printk("alloc_chrdev_region is error\n");
    }

    data = kmalloc(sizeof(struct pwm_led_data), GFP_KERNEl);
    data->sum_count = 20;
    data->hight_count = 10;
    printk("alloc_register_region is ok\n");
    major = MAJOR(dev_num);//使用MAJOR()函数获取主设备号
    minor = MINOR(dev_num);//使用MINOR()函数获取次设备号
    printk("major is %d\n",major);
    printk("minor is %d\n",minor);          	
    cdev_init(&cdev_test,&cdev_test_ops);//使用cdev_init()函数初始化cdev_test结构体，并链接到cdev_test_ops结构体
	cdev_test.owner = THIS_MODULE;//将owner字段指向本模块，可以避免在模块的操作正在被使用时卸载该模块
    ret = cdev_add(&cdev_test,dev_num,1);//使用cdev_add()函数进行字符设备的添加
    if(ret < 0 ){
        printk("cdev_add is error\n");
    }
    printk("cdev_add is ok\n");


    data->gpiod = gpiod_get(&pdev->dev, "led", GPIOD_OUT_HIGH);
    gpiod_set_value(data->gpiod, 1);  //输出高电平

    data->time = ktime_set(0, 1000000);

    hrtimer_init(&data->pwm_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
    data->pwm_timer.function = pwm_timer_func;

    hrtimer_start(&data->pwm_timer, data->time, HRTIMER_MODE_REL); 
    return 0;
}

int sg90_remove(struct platform_device* dev){
    return 0;
}

static int __init module_cdev_init(void)//驱动入口函数
{
    i

    platform_driver_regisiter(&sg90_platform_driver);
    return 0;
}

static void __exit module_cdev_exit(void)//驱动出口函数
{
    cdev_del(&cdev_test);//使用cdev_del()函数进行字符设备的删除
    unregister_chrdev_region(dev_num,1);//释放字符驱动设备号 
    platform_driver_unregisiter(&sg90_platform_driver);
    printk("module exit \n");

}

module_init(module_cdev_init);//注册入口函数
module_exit(module_cdev_exit);//注册出口函数
MODULE_LICENSE("GPL v2");//同意GPL开源协议
MODULE_AUTHOR("topeet");  //作者信息  

