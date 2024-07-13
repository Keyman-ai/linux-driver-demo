#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>
// #include <linux/delay.h>

int irq;

// 软中断处理程序
void testsoft_func(struct softirq_action *softirq_action)
{
  printk("This is testsoft_func\n");
}

irqreturn_t test_interrupt(int irq, void *args)
{
  printk("This is test_interrupt\n");
  raise_softirq(TEST_SOFTIRQ); // 触发软中断
  return IRQ_RETVAL(IRQ_HANDLED);
}

static int interrupt_irq_init(void)
{
  int ret;
  irq = gpio_to_irq(101); // 将GPIO映射为中断号
  printk("irq is %d\n", irq);
  // 请求中断
  ret = request_irq(irq, test_interrupt, IRQF_TRIGGER_RISING, "test", NULL);
  if (ret < 0)
  {
    printk("request_irq is error\n");
    return -1;
  }
  // 注册软中断处理函数
  open_softirq(TEST_SOFTIRQ, testsoft_func);
  return 0;
}

static void interrupt_irq_exit(void)
{
  free_irq(irq, NULL); // 释放中断
  printk("bye bye\n");
}

module_init(interrupt_irq_init);
module_exit(interrupt_irq_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("enlai");