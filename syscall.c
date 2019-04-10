#include "syscall.h"

#include <linux/slab.h>
#include <linux/kallsyms.h>
#include <linux/unistd.h>

#include "msr.h"

void **original_syscalls;
void **syscalls;
size_t syscalls_length;
size_t syscalls_size;

void syscall_setup(void) {
    syscalls = (void**)kallsyms_lookup_name("sys_call_table");
    syscalls_length = 313;
    syscalls_size = syscalls_length * sizeof(void*);

    original_syscalls = kmalloc(syscalls_size, GFP_KERNEL);
    memcpy(original_syscalls, syscalls, syscalls_size);
}

void syscall_rollback(void) {
    disable_write_protect();
    memcpy(syscalls, original_syscalls, syscalls_size);
    enable_write_protect();

    kfree(original_syscalls);
}

void hook_syscall(void *addr, int n) {
    disable_write_protect();
    syscalls[n] = addr;
    enable_write_protect();
}

void unhook_syscall(int n) {
    disable_write_protect();
    syscalls[n] = original_syscalls[n];
    enable_write_protect();
}
