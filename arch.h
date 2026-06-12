#ifndef ARCH_H
#define ARCH_H

#if defined(__x86_64__)
    #define SYS_READ  0
    #define SYS_WRITE 1
    #define SYS_OPEN  2
    #define SYS_EXIT  60
    #define ASM_SYSCALL syscall
    #define SYSCALL_REG %rax
    #define ARG0 %rdi
    #define ARG1 %rsi
    #define ARG2 %rdx
#elif defined(__i386__)
    #define SYS_READ  3
    #define SYS_WRITE 4
    #define SYS_OPEN  5
    #define SYS_EXIT  1
    #define ASM_SYSCALL int $0x80
    #define SYSCALL_REG %eax
    #define ARG0 %ebx
    #define ARG1 %ecx
    #define ARG2 %edx
#elif defined(__aarch64__)
    #define SYS_READ  63
    #define SYS_WRITE 64
    #define SYS_OPENAT 56
    #define SYS_EXIT  93
    #define ASM_SYSCALL svc #0
    #define SYSCALL_REG x8
    #define AT_FDCWD -100
    #define ARG0 x0
    #define ARG1 x1
    #define ARG2 x2
#elif defined(__riscv)
    #define SYS_READ  63
    #define SYS_WRITE 64
    #define SYS_OPENAT 56
    #define SYS_EXIT  93
    #define ASM_SYSCALL ecall
    #define SYSCALL_REG a7
    #define AT_FDCWD -100
    #define ARG0 a0
    #define ARG1 a1
    #define ARG2 a2
#endif

#endif
