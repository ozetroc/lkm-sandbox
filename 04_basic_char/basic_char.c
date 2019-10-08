#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <linux/string.h>
#include <linux/device.h>
#include <linux/kobject.h>
#include <linux/kernel.h>
#include <linux/string.h>

#define MY_NAME "my_character_driver"

static dev_t first;
static unsigned int count = 1;
static struct cdev *my_cdev;
static struct class *my_class;
static char *my_buf;
static size_t my_curr_size;

static int my_open(struct inode *inode, struct file *file)
{
	return 0;
}

static int my_release(struct inode *inode, struct file *file)
{
	return 0;
}

static ssize_t my_read(struct file *file, char __user *buf, size_t lbuf, loff_t *ppos)
{
	int n = 0;
	if(my_buf == NULL || *ppos >= strlen(my_buf))
		return 0;
	if(lbuf > my_curr_size)
		lbuf = my_curr_size;
	n = lbuf - copy_to_user(buf, my_buf, lbuf);
	*ppos += lbuf;
	return n;
}

static ssize_t my_write(struct file *file, const char __user *buf, size_t lbuf, loff_t *ppos)
{
	int n;
	if(my_buf != NULL)
		kfree(my_buf);
	if(!(my_buf = kmalloc(lbuf, GFP_KERNEL)))
		return -1;
	n = lbuf - copy_from_user(my_buf, buf, lbuf);
	my_curr_size = lbuf;
	my_buf[my_curr_size - 1] = '\0';
	*ppos = 0;
	return n;
}

static const struct file_operations my_fops = {
	.owner = THIS_MODULE,
	.read = my_read,
	.write = my_write,
	.open = my_open,
	.release = my_release,
};

static int __init my_init(void)
{
	if(alloc_chrdev_region(&first, 0, count, MY_NAME))
		goto err1;

	if(!(my_cdev = cdev_alloc()))
		goto err2;

	cdev_init(my_cdev, &my_fops);
	if(cdev_add(my_cdev, first, count))
		goto err3;

	if(!(my_class = class_create(THIS_MODULE, "my_first_char_device_class")))
		goto err3;

	if(!device_create(my_class, NULL, first, NULL, MY_NAME))
		goto err4;

	my_buf = NULL;
	my_curr_size = 0;
	pr_info("%s: successfully loaded\n", MY_NAME);
	return 0;

err4:
	pr_err("err4\n");
	class_destroy(my_class);

err3:
	pr_err("err3\n");
	cdev_del(my_cdev);

err2:
	pr_err("err2\n");
	unregister_chrdev_region(first, count);

err1:
	pr_err("err1\n");
	kfree(my_buf);
	pr_err("Failed to load driver\n");

	return -1;
}

static void __exit my_exit(void)
{
	kfree(my_buf);
	device_destroy(my_class, first);
	class_destroy(my_class);
	cdev_del(my_cdev);
	unregister_chrdev_region(first, count);
	pr_info("%s: successfully unloaded\n", MY_NAME);
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");