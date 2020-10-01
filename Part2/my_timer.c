//compile: /usr/src/my_timer > sudo make
//load module: /usr/src/my_timer > sudo insmod my_timer.ko
//check module is loaded: > lsmod | grep my_timer
//check kernal log: > dmesg
//unload kernal module: > sudo rmmod my_timer

#include <linux/init.h>		//modules
#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/semaphore.h>	//locks
#include <linux.list.h>		//linked lists
#include <linux/string.h>	//string functions
#include <linux/proc_fs.h>//file system calls
#include <linux/uaccess.h>//memory copy from kernel <-> userspace

MODULE_LICENSE("dual BSD/GPL");

static struct file_operations procfile_fops = {
	.owner = THIS_MODULE,
	.read = procfile_read,             //fill in callbacks to read/write functions
	.write = procfile_write,
};

#define BUF_LEN 100     //max length of read/write message

static struct proc_dir_entry* proc_entry;//pointer to proc entry

static char msg[BUF_LEN];//buffer to store read/write message
static int procfs_buf_len;//variable to hold length of message

//void * kmalloc (size_t size, gfp_t flags);
//char_ptr = kmalloc (sizeof(char) * 20, __GFP_RECLAIM);

//READ
static ssize_t procfile_read(struct file* file, char * ubuf, size_t count, loff_t *ppos)
{
printk(KERN_INFO "proc_read\n");
procfs_buf_len = strlen(msg);

//check for last access time
if (*ppos > 0 || count < procfs_buf_len)    //check if data already read and if space in user buffer
	return 0;
if (copy_to_user(ubuf, msg, procfs_buf_len))    //send data to user buffer
	return -EFAULT;

*ppos = procfs_buf_len;//update position

printk(KERN_INFO "gave to user %s\n", msg);

return procfs_buf_len;     //return number of characters read
}

//write
static ssize_t procfile_write(struct file* file, const char * ubuf, size_t count, loff_t* ppos)
{
printk(KERN_INFO "proc_write\n");
//("current time: %s", current_kernel_time());
//("elapsed time: %s", current_kernel_time()-access_time);

//write min(user message size, buffer length) characters
if (count > BUF_LEN)
	procfs_buf_len = BUF_LEN;
else
	procfs_buf_len = count;

copy_from_user(msg, ubuf, procfs_buf_len);

printk(KERN_INFO "got from user: %s\n", msg);

return procfs_buf_len;
}

//memcopy-- Kernal -> User
unsigned long copy_to_user (void __user *to, const void *from, unsigned long size)
//User -> Kernal
unsigned long copy_from_user (void *to, const void __user* from, unsigned long size)


/******************************************/
static int timer_init(void)
{
	double access_time = current_kernel_time();

	P(insmod);	//my_timer loaded
	//create proc entry: /proc/timer
	proc_entry = proc_create("timer", 0666, NULL, &procfile_fops);
	if(proc_entry == NULL)
		return -ENOMEM;
	V(rrmod);	//my_timer unloaded

	return 0;
}
module_init(timer_init);

static int timer_exit(void)
{
	P(rrmod);      //my_timer unloaded
        //proc/timer removed
	proc_remove(proc_entry);
	V(insmod);       //my_timer loaded

	return;
}
module_exit(timer_exit);
