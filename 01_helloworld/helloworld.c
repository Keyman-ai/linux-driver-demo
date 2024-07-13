#include <linux/module.h>
#include <linux/kernel.h> 
static int __init helloworld_init(void)        
{
	printk(KERN_EMERG "helloworld_init\r\n");
	return 0;
}
static void __exit helloworld_exit(void)    
{
	printk(KERN_EMERG "helloworld_exit\r\n");
}

module_init(helloworld_init);     //入口 
module_exit(helloworld_exit);     //出口
MODULE_LICENSE("GPL v2");    
MODULE_AUTHOR("topeet"); 
