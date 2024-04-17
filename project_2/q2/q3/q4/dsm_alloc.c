#include <linux/module.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "dsm_module"
#define MAX_MEM_SIZE (1024 * 1024) 

static int major_num;
static void *shared_memory;

static int dsm_open(struct inode *, struct file *);
static int dsm_release(struct inode *, struct file *);
static ssize_t dsm_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t dsm_write(struct file *, const char __user *, size_t, loff_t *);

static struct file_operations fops = {
    .open = dsm_open,
    .release = dsm_release,
    .read = dsm_read,
    .write = dsm_write,
};

static int __init dsm_module_init(void) 
{
    major_num = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_num < 0) 
    {
        printk(KERN_ALERT "Failed to register a major number\n");
        return major_num;
    }
    
    shared_memory = kmalloc(MAX_MEM_SIZE, GFP_KERNEL);
    if (!shared_memory) {
        unregister_chrdev(major_num, DEVICE_NAME);
        printk(KERN_ALERT "Failed to allocate shared memory\n");
        return -ENOMEM;
    }

    printk(KERN_INFO "DSM module registered with major number %d\n", major_num);
    return 0;
}

static void __exit dsm_module_exit(void) 
{
    kfree(shared_memory);
    
    unregister_chrdev(major_num, DEVICE_NAME);
    printk(KERN_INFO "Unregistered DSM module\n");
}

static int dsm_open(struct inode *inodep, struct file *filep) 
{
    printk(KERN_INFO "DSM device opened\n");
    return 0;
}

static int dsm_release(struct inode *inodep, struct file *filep) 
{
    printk(KERN_INFO "DSM device closed\n");
    return 0;
}

static ssize_t dsm_read(struct file *filep, char __user *buffer, size_t len, loff_t *offset) {
    if (copy_to_user(buffer, shared_memory + *offset, len) != 0)
        return -EFAULT; 
    
    *offset += len;
    return len;
}

static ssize_t dsm_write(struct file *filep, const char __user *buffer, size_t len, loff_t *offset) 
{
    if (copy_from_user(shared_memory + *offset, buffer, len) != 0)
        return -EFAULT; 
    *offset += len;
    return len;
}

module_init(dsm_module_init);
module_exit(dsm_module_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Distributed Shared Memory Module");
MODULE_VERSION("6.5.0-26-generic");
