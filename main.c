#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL"); // nt torvalds

static int __init overlord_init(void) {
    printk(KERN_INFO "[Overlord]: Hello World!\n");
    return 0;
}

static void __exit overlord_exit(void) {
    printk(KERN_INFO "[Overlord]: Goodbye World!\n");
}

module_init(overlord_init);
module_exit(overlord_exit);
