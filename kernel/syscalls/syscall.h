#ifndef SYSCALL_H
#define SYSCALL_H

#include <stdint.h>

#define SYS_WRITE 1
#define SYS_PS 2
#define SYS_LS 3

void syscall_init(void);

int32_t syscall_dispatch(uint32_t number, uint32_t arg1, uint32_t arg2, uint32_t arg3);

int32_t syscall(uint32_t number, uint32_t arg1, uint32_t arg2, uint32_t arg3);

#endif
