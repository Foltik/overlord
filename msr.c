#include "msr.h"

#include <linux/types.h>
#include <linux/linkage.h>
#include <asm/special_insns.h>

void enable_write_protect(void) {
    write_cr0(read_cr0() | 0x10000);
}

void disable_write_protect(void) {
    write_cr0(read_cr0() & ~0x10000);
}
