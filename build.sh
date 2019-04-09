#!/bin/bash

MAP=/lib/modules/$(uname -r)/build/System.map
SYSCALL_TABLE=0x$(grep "R sys_call_table" $MAP | awk '{print $1}')

> offsets.h
echo "#define OFFSET_SYSCALL_TABLE $SYSCALL_TABLE" >> offsets.h

make
