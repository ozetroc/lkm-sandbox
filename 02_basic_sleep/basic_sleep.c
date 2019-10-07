/**
 * @file basic_sleep.c
 * @author Przemyslaw Gasior
 * @brief An basic loadable kernel module which displays a message in dmesg (i.e. /var/log/kern.log file).
 * There is possibility to pass an argument which is also displayed later on.
 * @version 0.1
 * @date 2019-07-17
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include <linux/module.h> 
#include <linux/init.h> 
#include <linux/sched.h> 
#include <linux/time.h> 
#include <linux/delay.h> 
#include <linux/workqueue.h>
#include <linux/slab.h>



/* Setup input argument for kernel module */
static DECLARE_WAIT_QUEUE_HEAD(my_wq); 
static int condition = 0;
static struct work_struct wrk;
static struct delayed_work wrk_del;

/* Structure for passing arguments to the work handler */
struct my_works_struct
{
   int parameter_wrk;
   int parameter_dwrk;
   struct work_struct wrk_struct;
   struct delayed_work dwrk_struct;
};

struct my_works_struct *work1;

/**
 * @brief Work handler function
 */
static void work_handler(struct work_struct *work) 
{  
   pr_info("Waitqueue module handler %s - %d\n", __FUNCTION__, condition); 
   struct my_works_struct *work1_loc = container_of(work, struct my_works_struct, wrk_struct);
   struct my_works_struct *work2_loc = container_of((struct delayed_work*)work, struct my_works_struct, dwrk_struct);
   // pr_alert("work1_loc:\t\t%d \n", work1_loc);
   // pr_alert("work2_loc:\t\t%d \n", work2_loc);
   // pr_alert("work1:\t\t\t%d \n", work1);
   // pr_alert("work:\t\t\t%d \n", work);
   // pr_alert("work1->wrk_struct:\t%d \n", &work1->wrk_struct);
   // pr_alert("work1->dwrk_struct:\t%d \n", &work1->dwrk_struct);

   if(work == &work1->wrk_struct)
   {
      pr_info("Normal work schedule");
   }else if(work == &work2->wrk_struct)
   {
      pr_info("Delay work schedule");
   }

   msleep(3000); 
   pr_info("Wake up the sleeping module - %d\n", condition); 
   condition++; 
   wake_up_interruptible(&my_wq); 
   
   // if(!(condition > 3))
   // {
   //    pr_info("Scheduling next work - %d\n", condition);
   //    schedule_work(&wrk);
   // }else
   // {
   //    pr_info("Exiting scheduling chain - %d\n", condition);
   // }
   
} 

/**
 * @brief Init function
 */
static int __init mod_init(void){
   pr_info("Wait queue example\n"); 

   /* Allocating memory for structures */
   work1 = kmalloc(sizeof(*work1), GFP_KERNEL);
 
   INIT_WORK(&work1->wrk_struct, work_handler);
   work1->parameter_wrk = 1;
   work1->parameter_dwrk = 0;

   INIT_DELAYED_WORK(&work1->dwrk_struct, work_handler);
   schedule_work(&work1->wrk_struct); 
   /* Schedule work with 1s delay */
   schedule_delayed_work(&work1->dwrk_struct, HZ);
 
   pr_info("Going to sleep %s\n", __FUNCTION__); 
   wait_event_interruptible(my_wq, condition); 
 
   pr_info("woken up by the work job\n"); 
   return 0; 
}

/**
 * @brief Exit function
 */
static void __exit mod_exit(void){
   pr_info("Module cleanup\n");
   kfree(work1);
}

/**
 * @brief Driver initialization entry point 
 */
module_init(mod_init);
module_exit(mod_exit);

/* Setup module parameters */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Przemyslaw Gasior");
MODULE_DESCRIPTION("A basic timer LKM.");
MODULE_VERSION("0.1");