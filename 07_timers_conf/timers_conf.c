#include <linux/init.h> 
#include <linux/kernel.h> 
#include <linux/module.h> 
#include <linux/timer.h> 
#include <linux/hrtimer.h> 
#include <linux/ktime.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/device.h>

#define MY_NAME "my_timers_with_sysfs"

/* Structure for regular timer */
static struct timer_list my_timer; 

/* Structure for high resolution timer */
static unsigned long timer_interval_ns = 1000e6;
static struct hrtimer my_hrtimer;

/* Class */
static struct class *my_class;

/* Configurable intervals */
static int interval_timer = 3000;
static int jiffies_cnt = 1;


static ssize_t my_show(struct class *cls, struct class_attribute *attr, char *buf)
{
	if(jiffies_cnt)
    {
        pr_info("Read1\n");
		return sprintf(buf, "%d\n", interval_timer);
    }

    pr_info("Read2\n");
	return sprintf(buf, "disabled\n");
}

static ssize_t my_store(struct class *cls, struct class_attribute *attr, const char *buf, size_t count)
{
    if(kstrtoint(buf, 10, &interval_timer) != 0)
        pr_err("%s: Unknown command\n", MY_NAME);

    pr_info("Write\n");
	return count;
}

static struct class_attribute jiffies_cnt_attr = __ATTR(jiffies_cnt, 0660, my_show, my_store);



/**
 * @brief Callback function for regular timer
 */
void my_timer_callback(struct timer_list *t) 
{ 
    int retval;
    pr_info("%s called (%ld).\n", __FUNCTION__, jiffies);
    retval = mod_timer( &my_timer, jiffies + msecs_to_jiffies(interval_timer) ); 
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

    if(!(my_class = class_create(THIS_MODULE, "my_timers_with_sysfs")))
        goto err3;
    if(class_create_file(my_class, &jiffies_cnt_attr))
		goto err5;
    
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

    err5:
	    pr_err("err5\n");
	    class_destroy(my_class);

    err3:
        pr_err("err3\n");

    pr_err("Failed to load driver\n");
    return -1;
} 
  
/**
 * @brief Module exit function
 */
static void my_exit(void) 
{ 
    int retval; 

    class_remove_file(my_class, &jiffies_cnt_attr);
    class_destroy(my_class);
    pr_info("%s: successfully unloaded\n", MY_NAME);
    
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
