#include <linux/init.h> 
#include <linux/kernel.h> 
#include <linux/module.h> 
#include <linux/timer.h> 
#include <linux/hrtimer.h> 
#include <linux/ktime.h>

/* Structure for regular timer */
static struct timer_list my_timer; 

/* Structure for high resolution timer */
unsigned long timer_interval_ns = 1000e6;
static struct hrtimer my_hrtimer;

/**
 * @brief Callback function for regular timer
 */
void my_timer_callback(struct timer_list *t) 
{ 
    int retval;
    pr_info("%s called (%ld).\n", __FUNCTION__, jiffies);
    retval = mod_timer( &my_timer, jiffies + msecs_to_jiffies(3000) ); 
    if (retval) 
        pr_info("Timer firing failed\n"); 
} 

/**
 * @brief Callback function for high resolution timer
 */
enum hrtimer_restart my_hrtimer_callback(struct hrtimer *timer_for_restart) 
{ 
  	ktime_t currtime , interval;
  	currtime  = ktime_get();
  	interval = ktime_set(0,timer_interval_ns); 
  	hrtimer_forward(timer_for_restart, currtime , interval);
    pr_info("%s hrcalled (%ld).\n", __FUNCTION__, jiffies);
	return HRTIMER_RESTART;
} 

/**
 * @brief Module init function
 */
static int __init my_init(void) 
{ 
    int retval; 
    ktime_t ktime;
    printk("Timer module loaded\n"); 
 
    /* Setup regular timer */
    timer_setup(&my_timer, my_timer_callback, 0); 
    pr_info("Setup timer to fire in 300ms (%ld)\n", jiffies); 
    retval = mod_timer( &my_timer, jiffies + msecs_to_jiffies(300) ); 
    if (retval) 
    {
        printk("Timer firing failed\n"); 
    }

    /* Setup high resolution timer */
    ktime = ktime_set( 0, timer_interval_ns );
	hrtimer_init( &my_hrtimer, CLOCK_MONOTONIC, HRTIMER_MODE_REL );
	my_hrtimer.function = &my_hrtimer_callback;
 	hrtimer_start( &my_hrtimer, ktime, HRTIMER_MODE_REL );

    return 0; 
} 
  
/**
 * @brief Module exit function
 */
static void my_exit(void) 
{ 
    int retval; 

    retval = del_timer(&my_timer); 
    /* Is timer still active (1) or no (0) */ 
    if (retval) 
        pr_info("The timer is still in use...\n"); 

  	retval = hrtimer_cancel( &my_hrtimer );
  	if (retval) 
        pr_info("The hrtimer was still in use...\n");
 
    pr_info("Timer module unloaded\n"); 
} 
 
module_init(my_init); 
module_exit(my_exit); 
MODULE_AUTHOR("Przemyslaw Gasior"); 
MODULE_DESCRIPTION("Standard timers example"); 
MODULE_LICENSE("GPL"); 
