#include <linux/init.h>
#include <linux/kernel.h> 
#include <linux/module.h> 
#include <linux/interrupt.h>    /* for tasklets API */ 
 
char tasklet_data[]="Sample string as argument, but it could be any data"; 
 
/* Tasklet handler, that just print the data */ 
void tasklet_work(unsigned long data) 
{ 
    pr_info("[%s] %s\n", __FUNCTION__, (char *)data);
} 
 
DECLARE_TASKLET(my_tasklet, tasklet_work, (unsigned long) tasklet_data); 
 
static int __init my_init(void) 
{ 
    /* 
     * Schedule the handler. 
     * Tasklet are also scheduled from interrupt handler 
     */ 
    pr_info("[%s] Scheduling tasklet\n", __FUNCTION__);
    tasklet_schedule(&my_tasklet); 
    return 0; 
} 
 
void my_exit(void) 
{ 
    pr_info("[%s] Module cleanup\n", __FUNCTION__);
    tasklet_kill(&my_tasklet); 
} 
 
module_init(my_init); 
module_exit(my_exit); 
MODULE_AUTHOR("Przemyslaw Gasior"); 
MODULE_LICENSE("GPL"); 