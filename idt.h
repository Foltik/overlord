#ifndef OVERLORD_IDT_H
#define OVERLORD_IDT_H

#include <linux/types.h>

struct idt_entry {
    u16 off_low;
    u16 selector;
    u8 unused0;
    u8 type_attr;
    u16 off_mid;
    u32 off_high;
    u32 unused1;
};

struct idt_reg {
    u16 limit;
    struct idt_entry *base;
};

// IDT helpers
struct idt_reg get_idtr(void);
void* get_isr(struct idt_entry *idte);
void set_isr(struct idt_entry *idte, void* addr);

// Hooking
extern struct idt_entry *original_idt;
extern struct idt_entry *idt;
extern size_t idt_length;
extern size_t idt_size;

void idt_setup(void);
void idt_rollback(void);
void hook_idte(struct idt_entry entry, int n);
void hook_idte_stub(void* addr, int n);
void unhook_idte(int n);

#endif
