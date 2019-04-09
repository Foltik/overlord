#ifndef OVERLORD_SYSCALL_H
#define OVERLORD_SYSCALL_H

void *load_syscall_table(void);
void *load_syscall_ptr(u16 offset);

void hijack_syscall_table(void);
void restore_syscall_table(void);

#endif
