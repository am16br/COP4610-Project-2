#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

#include <linux/timekeeping32.h>
#include <linux/time32.h>

MODULE_LICENSE("Dual BSD/GPL");

#define BUF_LEN 100

static struct proc_dir_entry* proc_entry;

static char msg[BUF_LEN];
static struct timespec start_time; // start time upon initialization

static ssize_t procfile_read(struct file* file, char * ubuf, size_t count, loff_t *ppos)
{
    struct timespec cur_time; // current time
    struct timespec el_time;  // elapsed time
	int len = 0;

	if (*ppos > 0 || count < len)
		return 0;

    cur_time = current_kernel_time();
    el_time = timespec_sub(cur_time, start_time);
    len += sprintf(msg, "current time %ld.%09ld\n", cur_time.tv_sec, cur_time.tv_nsec);
    len += sprintf(msg + len, "elapsed time %ld.%09ld\n", el_time.tv_sec, el_time.tv_nsec);

	if (copy_to_user(ubuf, msg, len))
		return -EFAULT;

	*ppos = len;

	return len;
}


static struct file_operations procfile_fops = {
	.owner = THIS_MODULE,
	.read = procfile_read,
};

static int my_timer_init(void)
{
	proc_entry = proc_create("timer", 0666, NULL, &procfile_fops);
    start_time = current_kernel_time();

	if (proc_entry == NULL)
		return -ENOMEM;
	
	return 0;
}

static void my_timer_exit(void)
{
	proc_remove(proc_entry);
	return;
}


module_init(my_timer_init);
module_exit(my_timer_exit);

