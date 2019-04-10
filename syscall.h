#ifndef OVERLORD_SYSCALL_H
#define OVERLORD_SYSCALL_H

#include <linux/types.h>

extern void **original_syscalls;
extern void **syscalls;
extern size_t syscalls_length;
extern size_t syscalls_size;

void syscall_setup(void);
void syscall_rollback(void);
void hook_syscall(void *addr, int n);
void unhook_syscall(int n);
void *orig_syscall(int n);

#endif
