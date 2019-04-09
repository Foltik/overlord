#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include "idt.h"

MODULE_LICENSE("GPL"); // nt torvalds

static int __init overlord_init(void) {
    idt_setup();
    printk(KERN_INFO "[Overlord]: Hello World!\n");
    return 0;
}

static void __exit overlord_exit(void){
    idt_rollback();
    printk(KERN_INFO "[Overlord]: Goodbye World!\n");
}

module_init(overlord_init);
module_exit(overlord_exit);
