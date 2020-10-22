#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>//file system calls
#include <linux/uaccess.h>//memory copy from kernel <-> userspace
#include <linux/time.h>

MODULE_LICENSE("Dual BSD/GPL");

#define BUF_LEN 100     //max length of read/write message

static struct proc_dir_entry* proc_entry;//pointer to proc entry

static char msg[BUF_LEN];//buffer to store read/write message
static int procfs_buf_len;//variable to hold length of message
static struct timespec my_current_time, my_elapsed_time;


static ssize_t procfile_read(struct file* file, char * ubuf, size_t count, loff_t *ppos){

	if (my_current_time.tv_sec != 0) {
		my_elapsed_time = current_kernel_time();
		my_elapsed_time.tv_sec -= my_current_time.tv_sec;
		my_elapsed_time.tv_nsec -= my_current_time.tv_nsec;
		if(my_elapsed_time.tv_nsec < 0) {
			my_elapsed_time.tv_sec -= 1;
			my_elapsed_time.tv_nsec += 1000000000;
		}
	}

	my_current_time = current_kernel_time();
	printk(KERN_INFO "current time: %d.%d", (int)my_current_time.tv_sec, (int)my_current_time.tv_nsec);

	if (my_elapsed_time.tv_sec == 0) {
		printk(KERN_INFO "elapsed time: %d.%d", (int)my_elapsed_time.tv_sec, (int)my_elapsed_time.tv_nsec);
	}
	sprintf(msg, "current time: %d.%d\nelapsed time: %d.%d\n",(int)my_current_time.tv_sec, (int)my_current_time.tv_nsec,(int)my_elapsed_time.tv_sec, (int)my_elapsed_time.tv_nsec);

	procfs_buf_len = strlen(msg);
	if (*ppos > 0 || count < procfs_buf_len)    //check if data already read and if space in user buffer
		return 0;

	if (copy_to_user(ubuf, msg, procfs_buf_len))    //send data to user buffer
		return -EFAULT;

	*ppos = procfs_buf_len;//update position

	return procfs_buf_len;     //return number of characters read
}

static ssize_t procfile_write(struct file* file, const char * ubuf, size_t count, loff_t* ppos)
{
	//write min(user message size, buffer length) characters
	if (count > BUF_LEN)
		procfs_buf_len = BUF_LEN;
	else
		procfs_buf_len = count;
	copy_from_user(msg, ubuf, procfs_buf_len);
	return procfs_buf_len;
}

static struct file_operations procfile_fops = {
	.owner = THIS_MODULE,
	.read = procfile_read,             //fill in callbacks to read/write functions
	.write = procfile_write
};

static int hello_init(void){
	//proc_create(filename, permissions, parent, pointer to fops)
	proc_entry = proc_create("timer", 0666, NULL, &procfile_fops);

	if (proc_entry == NULL)
		return -ENOMEM;

	return 0;
}

static void hello_exit(void){
	proc_remove(proc_entry);
	return;
}

module_init(hello_init);
module_exit(hello_exit);
