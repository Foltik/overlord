#ifndef OVERLORD_SYSCALL_H
#define OVERLORD_SYSCALL_H

// Helpers
void **get_syscall_table(void);

// Hooking
extern void **original_syscall_table;
void syscall_setup(void);
void syscall_rollback(void);
void hook_syscall(void *addr, int n);
void unhook_syscall(int n);

#endif
