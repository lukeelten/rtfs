#include <linux/init.h>
#include <linux/module.h>

static int __init rtfs_init(void) {
    pr_debug("rtfs module loaded\n");
    return 0;
}

static void __exit rtfs_exit(void) {
    pr_debug("rtfs module unloaded\n");
}

module_init(rtfs_init);
module_exit(rtfs_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("tddi");
