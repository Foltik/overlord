#include "idt.h"

#include <linux/slab.h>
#include <linux/string.h>

#include "msr.h"

struct idt_reg get_idtr(void) {
    struct idt_reg idtr;
    asm("sidt %0"
        : "=m" (idtr));
    return idtr;
}

void *get_isr(struct idt_entry *idte) {
    return (void*)(((u64)idte->off_high << 32) | ((u64)idte->off_mid << 16) | ((u64)idte->off_low));
}

void set_isr(struct idt_entry *idte, void *addr) {
    idte->off_low =  (u16)((u64)addr);
    idte->off_mid =  (u16)((u64)addr >> 16);
    idte->off_high = (u32)((u64)addr >> 32);
}



struct idt_entry *original_idt;

void idt_setup(void) {
    struct idt_reg idtr = get_idtr();
    size_t size = idtr.limit * sizeof(struct idt_entry);

    original_idt = kmalloc(size, GFP_KERNEL);
    memcpy(original_idt, idtr.base, size);
}

void idt_rollback(void) {
    struct idt_reg idtr = get_idtr();
    size_t size = idtr.limit * sizeof(struct idt_entry);

    disable_write_protect();
    memcpy(idtr.base, original_idt, size);
    enable_write_protect();

    kfree(original_idt);
}

void hook_idte(struct idt_entry entry, int n) {
    struct idt_reg idtr = get_idtr();

    disable_write_protect();
    idtr.base[n] = entry;
    enable_write_protect();
}

void unhook_idte(int n) {
    struct idt_reg idtr = get_idtr();

    disable_write_protect();
    idtr.base[n] = original_idt[n];
    enable_write_protect();
}

void hook_idte_stub(void *addr, int n) {
    struct idt_reg idtr = get_idtr();

    disable_write_protect();
    set_isr(&idtr.base[n], addr);
    enable_write_protect();
}
