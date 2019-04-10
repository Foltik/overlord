#ifndef OVERLORD_SYSCALL_H
#define OVERLORD_SYSCALL_H

#include <linux/linkage.h>
#include <linux/ptrace.h>
#include <linux/types.h>

typedef asmlinkage long (*syscall_t)(struct pt_regs *regs);

extern void **original_syscalls;
extern void **syscalls;
extern size_t syscalls_length;
extern size_t syscalls_size;

void syscall_setup(void);
void syscall_rollback(void);
void hook_syscall(syscall_t fn, int n);
void unhook_syscall(int n);
syscall_t orig_syscall(int n);

#endif
