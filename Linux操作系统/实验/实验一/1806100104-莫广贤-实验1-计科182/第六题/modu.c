#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>
static int __init hello_init(void)
{
	printk("<1>Hello World! from the kernel space...\n");
	return 0;
}
static void __exit hello_cleanup(void)
{
	printk("<1>Goodbye World! leaving kernel space...\n");
}
module_init(hello_init);	//模块加载函数
module_exit(hello_cleanup);	//模块卸载函数
MODULE_LICENSE("GPL");
