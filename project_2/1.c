#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/version.h>
#include <linux/vmalloc.h> 

#define DSM_SIZE (PAGE_SIZE * 4)  
struct dsm_data 
{
    int data;
};

struct dsm_data *dsm_ptr;

static int __init dsm_init(void)
{
    dsm_ptr = (struct dsm_data *)vmalloc(DSM_SIZE);
    if (!dsm_ptr)
    {
        printk(KERN_ERR "Failed to allocate DSM memory\n");
        return -ENOMEM;
    }

    dsm_ptr->data = 0;

    printk(KERN_INFO "DSM module initialized\n");
    return 0;
}

static void __exit dsm_exit(void)
{
    vfree(dsm_ptr);

    printk(KERN_INFO "DSM module removed\n");
}

module_init(dsm_init);
module_exit(dsm_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sai krishnan");
MODULE_DESCRIPTION("Distributed Shared Memory (DSM) Module");
MODULE_VERSION("6.5.0-26-generic");

