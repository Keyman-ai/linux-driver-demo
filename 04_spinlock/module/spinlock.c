#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/kdev_t.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/spinlock.h>

static spinlock_t spinlock_test;//定义spinlock_t类型的自旋锁变量spinlock_test
static int flag = 1;//定义flag标准为，flag等于1表示设备没有被打开，等于0则证明设备已经被打开了
static int open_test(struct inode *inode,struct file *file)
{
	//printk("\nthis is open_test \n");
	spin_lock(&spinlock_test);//自旋锁加锁
	if(flag != 1){//判断标志位flag的值是否等于1
		spin_unlock(&spinlock_test);//自旋锁解锁
		return -EBUSY;
	 }
	flag = 0;//将标志位的值设置为0
	spin_unlock(&spinlock_test);//自旋锁解锁
	return 0;
}

static ssize_t read_test(struct file *file,char __user *ubuf,size_t len,loff_t *off)
{
	int ret;
	char kbuf[10] = "topeet";//定义char类型字符串变量kbuf
	printk("\nthis is read_test \n");
	ret = copy_to_user(ubuf,kbuf,strlen(kbuf));//使用copy_to_user接收用户空间传递的数据
	if (ret != 0){
		printk("copy_to_user is error \n");
	}
	printk("copy_to_user is ok \n");
	return 0;
}
static char kbuf[10] = {0};//定义char类型字符串全局变量kbuf
static ssize_t write_test(struct file *file,const char __user *ubuf,size_t len,loff_t *off)
{
	int ret;
	ret = copy_from_user(kbuf,ubuf,len);//使用copy_from_user接收用户空间传递的数据
	if (ret != 0){
		printk("copy_from_user is error\n");
	}
	if(strcmp(kbuf,"topeet") == 0 ){//如果传递的kbuf是topeet就睡眠四秒钟
		ssleep(4);
	}
	else if(strcmp(kbuf,"itop") == 0){//如果传递的kbuf是itop就睡眠两秒钟
		ssleep(2);
	}
	printk("copy_from_user buf is %s \n",kbuf);
	return 0;
}
static int release_test(struct inode *inode,struct file *file)
{
	printk("\nthis is release_test \n");
	spin_lock(&spinlock_test);//自旋锁加锁
	flag = 1;
	spin_unlock(&spinlock_test);//自旋锁解锁
	return 0;
}

struct chrdev_test {
       dev_t dev_num;//定义dev_t类型变量dev_num来表示设备号
       int major,minor;//定义int类型的主设备号major和次设备号minor
       struct cdev cdev_test;//定义struct cdev 类型结构体变量cdev_test，表示要注册的字符设备
       struct class *class_test;//定于struct class *类型结构体变量class_test，表示要创建的类
};
struct chrdev_test dev1;//创建chrdev_test类型的
struct file_operations fops_test = {
      .owner = THIS_MODULE,//将owner字段指向本模块，可以避免在模块的操作正在被使用时卸载该模块
      .open = open_test,//将open字段指向open_test(...)函数
      .read = read_test,//将read字段指向read_test(...)函数
      .write = write_test,//将write字段指向write_test(...)函数
      .release = release_test,//将release字段指向release_test(...)函数
};
 
static int __init atomic_init(void)
{
	spin_lock_init(&spinlock_test);
	if(alloc_chrdev_region(&dev1.dev_num,0,1,"chrdev_name") < 0 ){//自动获取设备号，设备名chrdev_name
		printk("alloc_chrdev_region is error \n");
	}
	printk("alloc_chrdev_region is ok \n");
	dev1.major = MAJOR(dev1.dev_num);//使用MAJOR()函数获取主设备号
	dev1.minor = MINOR(dev1.dev_num);//使用MINOR()函数获取次设备号
	printk("major is %d,minor is %d\n",dev1.major,dev1.minor);
	cdev_init(&dev1.cdev_test,&fops_test);//使用cdev_init()函数初始化cdev_test结构体，并链接到fops_test结构体
	dev1.cdev_test.owner = THIS_MODULE;//将owner字段指向本模块，可以避免在模块的操作正在被使用时卸载该模块
	cdev_add(&dev1.cdev_test,dev1.dev_num,1);//使用cdev_add()函数进行字符设备的添加
	dev1.class_test = class_create(THIS_MODULE,"class_test");//使用class_create进行类的创建，类名称为class_test
	device_create(dev1.class_test,0,dev1.dev_num,0,"device_test");//使用device_create进行设备的创建，设备名称为device_test
	return 0;
}

static void __exit atomic_exit(void)
{
	device_destroy(dev1.class_test,dev1.dev_num);//删除创建的设备
	class_destroy(dev1.class_test);//删除创建的类
	cdev_del(&dev1.cdev_test);//删除添加的字符设备cdev_test
	unregister_chrdev_region(dev1.dev_num,1);//释放字符设备所申请的设备号
	printk("module exit \n");
}
module_init(atomic_init);
module_exit(atomic_exit)
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("enlai");
