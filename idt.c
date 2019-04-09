#include "idt.h"

#include <linux/slab.h>
#include <linux/string.h>

struct idt_reg get_idtr(void) {
    struct idt_reg idtr;
    asm("sidt %0"
        : "=m" (idtr));
    return idtr;
}

void *get_isr(struct idt_entry *idte) {
    return (idte->off_high << 32) | (idte->off_mid << 16) | (idte->off_low);
}

void set_isr(struct idt_entry *idte, void *addr) {
    idte->off_low =  (u16)((u64)addr);
    idte->off_mid =  (u16)((u64)addr >> 16);
    idte->off_high = (u32)((u64)addr >> 32);
}



struct idt_entry *old_idt;
struct idt_entry *mod_idt;

void idt_setup(void) {
    struct idt_reg idtr = get_idtr();
    size_t size = idtr.limit * sizeof(struct idt_entry);

    old_idt = kmalloc(size, GFP_KERNEL);
    mod_idt = kzalloc(size, GFP_KERNEL);

    memcpy(old_idt, idtr.base, size);
}

void idt_rollback(void) {
    struct idt_reg idtr = get_idtr();
    size_t size = idtr.limit * sizeof(struct idt_entry);

    memcpy(idtr.base, old_idt, size);

    kfree(old_idt);
    kfree(mod_idt);
}

void hook_idte(struct idt_entry entry, int n) {
    struct idt_reg idtr = get_idtr();
    mod_idt[n] = entry;
    memcpy(&idtr.base[n], &mod_idt[n], sizeof(struct idt_entry));
}

void unhook_idte(struct idt_entry entry, int n) {
    struct idt_reg idtr = get_idtr();
    memcpy(&idtr.base[n], &old_idt[n], sizeof(struct idt_entry));
    memset(&mod_idt[n], 0, sizeof(struct idt_entry));
}

void hook_idte_stub(void *addr, int n) {
    struct idt_reg idtr = get_idtr();
    memcpy(&mod_idt[n], &old_idt[n], sizeof(struct idt_entry));
    set_isr(&mod_idt[n], addr);
    memcpy(&idtr.base[n], &mod_idt[n], sizeof(struct idt_entry));
}
