#include "syscall.h"

#include <linux/kallsyms.h>

#include "msr.h"

void **get_syscall_table(void) {
    return kallsyms_lookup_name("sys_call_table");
}


void **original_syscall_table;

void syscall_setup(void) {
    void **syscall_table = get_syscall_table();

    original_syscall_table = kmalloc(???, GFP_KERNEL);
    memcpy(syscall_table, original_syscall_table, ???);
}

void syscall_rollback(void) {
    void **syscall_table = get_syscall_table();

    disable_write_protect();
    memcpy(syscall_table, original_syscall_table, ???);
    enable_write_protect();

    kfree(original_syscall_table);
}

void hook_syscall(void *addr, int n) {
    void **syscall_table = get_syscall_table();

    disable_write_protect();
    syscall_table[n] = addr;
    enable_write_protect();
}

void unhook_syscall(int n) {
    void **syscall_table = get_syscall_table();

    disable_write_protect();
    syscall_table[n] = original_syscall_table[n];
    enable_write_protect();
}
