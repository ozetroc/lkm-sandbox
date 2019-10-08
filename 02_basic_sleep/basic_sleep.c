/**
 * @file basic_sleep.c
 * @author Przemyslaw Gasior
 * @brief An basic loadable kernel module which shows functionality of workqueues
 * Two types of works are scheduled, classical and delayed.
 * Counting semaphore is added to prevent module cleanup with active work handlers.
 * @version 0.1
 * @date 2019-10-08
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
#include <linux/semaphore.h>


#define SEMAPHORE_CNT   2

/* Setup input argument for kernel module */
static DECLARE_WAIT_QUEUE_HEAD(my_wq); 
static int condition = 0;
static struct work_struct wrk;
static struct delayed_work wrk_del;
struct semaphore mr_sem;

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
   down_interruptible(&mr_sem);
   pr_info("[%s] Semaphore count - %d\n", __FUNCTION__, mr_sem.count); 
   pr_info("[%s] Waitqueue module handler - %d\n", __FUNCTION__, condition); 
   struct my_works_struct *work1_loc = container_of(work, struct my_works_struct, wrk_struct);
   struct my_works_struct *work2_loc = container_of((struct delayed_work*)work, struct my_works_struct, dwrk_struct);
   /* Pointers check */
/*    pr_alert("work1_loc:\t\t%d \n", work1_loc);
   pr_alert("work2_loc:\t\t%d \n", work2_loc);
   pr_alert("work1:\t\t\t%d \n", work1);
   pr_alert("work:\t\t\t%d \n", work);
   pr_alert("&work1->wrk_struct:\t%d \n", &work1->wrk_struct);
   pr_alert("&work1->dwrk_struct:\t%d \n", &work1->dwrk_struct); */

   if(work == &work1->wrk_struct)
   {
      pr_info("[%s] Normal work schedule", __FUNCTION__);
   }else if(work == &work1->dwrk_struct)
   {
      pr_info("[%s] Delay work schedule", __FUNCTION__);
   }

   msleep(3000); 
   pr_info("[%s] Wake up the sleeping module - %d\n", __FUNCTION__, condition); 
   condition++; 
   wake_up_interruptible(&my_wq); 
   
/*    if(!(condition > 3))
   {
      pr_info("Scheduling next work - %d\n", condition);
      schedule_work(&wrk);
   }else
   {
      pr_info("Exiting scheduling chain - %d\n", condition);
   } */
   up(&mr_sem);
   
} 

/**
 * @brief Init function
 */
static int __init mod_init(void){
   pr_info("[%s] Wait queue example\n", __FUNCTION__); 

   /* Allocating memory for structures */
   work1 = kmalloc(sizeof(*work1), GFP_KERNEL);
 
   /* Initialize semaphore */
   sema_init(&mr_sem, SEMAPHORE_CNT);

   /* Initialize work structures */
   INIT_WORK(&work1->wrk_struct, work_handler);
   INIT_DELAYED_WORK(&work1->dwrk_struct, work_handler);
   work1->parameter_wrk = 1;
   work1->parameter_dwrk = 0;

   /* Schedule work without delay */
   schedule_work(&work1->wrk_struct); 

   /* Schedule work with 1.5s delay */
   schedule_delayed_work(&work1->dwrk_struct, 1.5*HZ);
 
   /* Go to sleep - blocking. During insmod, there is also block in terminal (init not finished) */
   pr_info("[%s] Going to sleep\n", __FUNCTION__); 
   wait_event_interruptible(my_wq, condition); 
 
   /* Woken up from sleep */
   pr_info("[%s] Woken up by the work job\n", __FUNCTION__); 
   return 0; 
}

/**
 * @brief Exit function
 */
static void __exit mod_exit(void){
   pr_info("[%s] Module cleanup\n", __FUNCTION__);
   pr_info("[%s] Semaphore count - %d\n", __FUNCTION__, mr_sem.count);
   cancel_delayed_work_sync(&work1->dwrk_struct);
   flush_scheduled_work();
   while(mr_sem.count != SEMAPHORE_CNT);
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