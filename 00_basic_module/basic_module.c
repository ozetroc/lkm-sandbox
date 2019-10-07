/**
 * @file basic_module.c
 * @author Przemyslaw Gasior
 * @brief An basic loadable kernel module which displays a message in dmesg (i.e. /var/log/kern.log file).
 * There is possibility to pass an argument which is also displayed later on.
 * @version 0.1
 * @date 2019-06-21
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

/* Setup module parameters */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Przemyslaw Gasior");
MODULE_DESCRIPTION("A basic LKM.");
MODULE_VERSION("0.1");

/* Setup input argument for kernel module */
static char *text = "there";  
module_param(text, charp, S_IRUGO);
MODULE_PARM_DESC(text, "The text displayed in kernel log");

/**
 * @brief Init function
 */
static int __init basic_module_init(void){
   printk(KERN_INFO "basic_module: Hello %s from the kernel side!\n", text);
   return 0;
}

/**
 * @brief Exit function
 */
static void __exit basic_module_exit(void){
   printk(KERN_INFO "basic_module: Goodbye %s from the kernel side!\n", text);
}

/**
 * @brief Driver initialization entry point 
 */
module_init(basic_module_init);
module_exit(basic_module_exit);
