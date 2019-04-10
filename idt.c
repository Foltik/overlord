#include "idt.h"

#include <linux/slab.h>
#include <linux/string.h>

#include "msr.h"

void *get_isr(struct idt_entry *idte) {
    return (void*)(((u64)idte->off_high << 32) | ((u64)idte->off_mid << 16) | ((u64)idte->off_low));
}

void set_isr(struct idt_entry *idte, void *addr) {
    idte->off_low =  (u16)((u64)addr);
    idte->off_mid =  (u16)((u64)addr >> 16);
    idte->off_high = (u32)((u64)addr >> 32);
}



struct idt_entry *original_idt;
struct idt_entry *idt;
size_t idt_length;
size_t idt_size;

void idt_setup(void) {
    struct idt_reg idtr;
    asm("sidt %0"
        : "=m" (idtr));

    idt_length = idtr.limit;
    idt_size = idt_length * sizeof(struct idt_entry);

    original_idt = kmalloc(idt_size, GFP_KERNEL);
    memcpy(original_idt, idt, idt_size);
}

void idt_rollback(void) {
    disable_write_protect();
    memcpy(idt, original_idt, idt_size);
    enable_write_protect();

    kfree(original_idt);
}

void hook_idte(struct idt_entry entry, int n) {
    disable_write_protect();
    idt[n] = entry;
    enable_write_protect();
}

void unhook_idte(int n) {
    disable_write_protect();
    idt[n] = original_idt[n];
    enable_write_protect();
}

void hook_idte_stub(void *addr, int n) {
    disable_write_protect();
    set_isr(&idt[n], addr);
    enable_write_protect();
}
