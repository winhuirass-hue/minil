/* ========================================================================== *
 * minil - Minimal Linux User-Space Runtime                                   *
 * This file - Header-Only Runtime (minil.S as a single .h)                  *
 * -------------------------------------------------------------------------- *
 * This software is dedicated to the public domain under CC0 1.0 Universal.   *
 * See LICENCE.md for full legal text.                                        *
 * ========================================================================== */

#ifndef MINIL_H
#define MINIL_H

#include "arch.h"

/* ====================== bss ====================== */

__asm__ (
        "       .section .bss                           \n"
#if defined(__x86_64__) || defined(__aarch64__) || defined(__riscv)
        "       .balign 8                               \n"
        "environ:                                       \n"
        "       .quad 0                                 \n"
#else
        "       .balign 4                               \n"
        "environ:                                       \n"
        "       .long 0                                 \n"
#endif
);

/* ====================== rodata ====================== */

__asm__ (
        "       .section .rodata                        \n"
        "nl:                                            \n"
        "       .byte 10                                \n"
        "       .section .note.GNU-stack,\"\",@progbits \n"
);

/* ====================== call_init_array ====================== */

__asm__ (
        "       .section .text                          \n"
        "       .extern __init_array_start              \n"
        "       .extern __init_array_end                \n"

        "call_init_array:                               \n"
#if defined(__x86_64__)
        "       pushq   %r12                            \n"
        "       pushq   %r13                            \n"
        "       leaq    __init_array_start(%rip), %r12  \n"
        "       leaq    __init_array_end(%rip),   %r13  \n"
        "1:                                             \n"
        "       cmpq    %r13, %r12                      \n"
        "       je      2f                              \n"
        "       movq    (%r12), %rax                    \n"
        "       addq    $8, %r12                        \n"
        "       call    *%rax                           \n"
        "       jmp     1b                              \n"
        "2:                                             \n"
        "       popq    %r13                            \n"
        "       popq    %r12                            \n"
        "       ret                                     \n"
#elif defined(__i386__)
        "       pushl   %ebx                            \n"
        "       pushl   %esi                            \n"
        "       movl    $__init_array_start, %ebx       \n"
        "       movl    $__init_array_end,   %esi       \n"
        "1:                                             \n"
        "       cmpl    %esi, %ebx                      \n"
        "       je      2f                              \n"
        "       movl    (%ebx), %eax                    \n"
        "       addl    $4, %ebx                        \n"
        "       call    *%eax                           \n"
        "       jmp     1b                              \n"
        "2:                                             \n"
        "       popl    %esi                            \n"
        "       popl    %ebx                            \n"
        "       ret                                     \n"
#elif defined(__aarch64__)
        "       stp     x19, x20, [sp, -32]!            \n"
        "       stp     x21, x30, [sp, 16]              \n"
        "       adrp    x19, __init_array_start         \n"
        "       add     x19, x19, :lo12:__init_array_start \n"
        "       adrp    x20, __init_array_end           \n"
        "       add     x20, x20, :lo12:__init_array_end \n"
        "1:                                             \n"
        "       cmp     x19, x20                        \n"
        "       b.eq    2f                              \n"
        "       ldr     x21, [x19], 8                   \n"
        "       blr     x21                             \n"
        "       b       1b                              \n"
        "2:                                             \n"
        "       ldp     x21, x30, [sp, 16]              \n"
        "       ldp     x19, x20, [sp], 32              \n"
        "       ret                                     \n"
#elif defined(__riscv)
        "       addi    sp, sp, -32                     \n"
        "       sd      s0, 0(sp)                       \n"
        "       sd      s1, 8(sp)                       \n"
        "       sd      s2, 16(sp)                      \n"
        "       sd      ra, 24(sp)                      \n"
        "       lla     s0, __init_array_start          \n"
        "       lla     s1, __init_array_end            \n"
        "1:                                             \n"
        "       beq     s0, s1, 2f                      \n"
        "       ld      s2, 0(s0)                       \n"
        "       addi    s0, s0, 8                       \n"
        "       jalr    s2                              \n"
        "       j       1b                              \n"
        "2:                                             \n"
        "       ld      s0, 0(sp)                       \n"
        "       ld      s1, 8(sp)                       \n"
        "       ld      s2, 16(sp)                      \n"
        "       ld      ra, 24(sp)                      \n"
        "       addi    sp, sp, 32                      \n"
        "       ret                                     \n"
#endif
);

/* ====================== _start ====================== */

__asm__ (
        "       .global _start                          \n"
        "       .global environ                         \n"
        "       .extern main                            \n"

        "_start:                                        \n"
#if defined(__x86_64__)
        "       cld                                     \n"
        "       xorq    %rbp, %rbp                      \n"
        "       movq    %rsp, %r12                      \n"  /* original stack */
        "       movq    (%r12), %r13                    \n"  /* argc */
        "       leaq    8(%r12), %r14                   \n"  /* argv */
        "       leaq    8(%r14,%r13,8), %r15            \n"  /* envp */
        "       movq    %r15, environ(%rip)             \n"
        "       andq    $-16, %rsp                      \n"
        "       subq    $8, %rsp                        \n"
        "       call    call_init_array                 \n"
        "       addq    $8, %rsp                        \n"
        "       movq    %r13, %rdi                      \n"  /* argc */
        "       movq    %r14, %rsi                      \n"  /* argv */
        "       movq    %r15, %rdx                      \n"  /* envp */
        "       call    main                            \n"
        "       movq    %rax, %rdi                      \n"  /* return code */
        "       movq    $60, %rax                       \n"  /* SYS_EXIT */
        "       syscall                                 \n"
#elif defined(__i386__)
        "       cld                                     \n"
        "       xorl    %ebp, %ebp                      \n"
        "       movl    (%esp), %eax                    \n"  /* argc */
        "       leal    4(%esp), %ecx                   \n"  /* argv */
        "       leal    4(%ecx,%eax,4), %edx            \n"  /* envp */
        "       movl    %edx, environ                   \n"
        "       pushl   %eax                            \n"
        "       pushl   %ecx                            \n"
        "       pushl   %edx                            \n"
        "       call    call_init_array                 \n"
        "       popl    %edx                            \n"
        "       popl    %ecx                            \n"
        "       popl    %eax                            \n"
        "       pushl   %edx                            \n"
        "       pushl   %ecx                            \n"
        "       pushl   %eax                            \n"
        "       call    main                            \n"
        "       addl    $12, %esp                       \n"
        "       movl    %eax, %ebx                      \n"  /* return code */
        "       movl    $1, %eax                        \n"  /* SYS_EXIT */
        "       int     $0x80                           \n"
#elif defined(__aarch64__)
        "       ldr     x19, [sp]                       \n"  /* argc */
        "       add     x20, sp, 8                      \n"  /* argv */
        "       add     x21, x20, x19, lsl 3            \n"
        "       add     x21, x21, 8                     \n"  /* envp */
        "       adrp    x8, environ                     \n"
        "       str     x21, [x8, :lo12:environ]        \n"
        "       mov     x8, sp                          \n"
        "       and     x8, x8, -16                     \n"
        "       mov     sp, x8                          \n"
        "       bl      call_init_array                 \n"
        "       mov     x0, x19                         \n"  /* argc */
        "       mov     x1, x20                         \n"  /* argv */
        "       mov     x2, x21                         \n"  /* envp */
        "       bl      main                            \n"
        "       mov     x0, x0                          \n"  /* return code */
        "       mov     x8, #93                         \n"  /* SYS_EXIT */
        "       svc     #0                              \n"
#elif defined(__riscv)
        "       ld      s0, 0(sp)                       \n"  /* argc */
        "       addi    s1, sp, 8                       \n"  /* argv */
        "       slli    t0, s0, 3                       \n"
        "       add     s2, s1, t0                      \n"
        "       addi    s2, s2, 8                       \n"  /* envp */
        "       lla     t1, environ                     \n"
        "       sd      s2, 0(t1)                       \n"
        "       li      t0, -16                         \n"
        "       and     sp, sp, t0                      \n"
        "       call    call_init_array                 \n"
        "       mv      a0, s0                          \n"  /* argc */
        "       mv      a1, s1                          \n"  /* argv */
        "       mv      a2, s2                          \n"  /* envp */
        "       call    main                            \n"
        "       mv      a0, a0                          \n"  /* return code */
        "       li      a7, 93                          \n"  /* SYS_EXIT */
        "       ecall                                   \n"
#endif
);

/* ====================== write ====================== */

__asm__ (
        "       .global write                           \n"
        "write:                                         \n"
#if defined(__i386__)
        "       pushl   %ebx                            \n"
        "       movl    8(%esp), %ebx                   \n"
        "       movl    12(%esp), %ecx                  \n"
        "       movl    16(%esp), %edx                  \n"
        "       movl    $4, %eax                        \n"
        "       int     $0x80                           \n"
        "       popl    %ebx                            \n"
#elif defined(__x86_64__)
        "       movq    $1, %rax                        \n"
        "       syscall                                 \n"
#elif defined(__aarch64__)
        "       mov     x8, #64                         \n"
        "       svc     #0                              \n"
#elif defined(__riscv)
        "       li      a7, 64                          \n"
        "       ecall                                   \n"
#endif
        "       ret                                     \n"
);

/* ====================== read ====================== */

__asm__ (
        "       .global read                            \n"
        "read:                                          \n"
#if defined(__i386__)
        "       pushl   %ebx                            \n"
        "       movl    8(%esp), %ebx                   \n"
        "       movl    12(%esp), %ecx                  \n"
        "       movl    16(%esp), %edx                  \n"
        "       movl    $3, %eax                        \n"
        "       int     $0x80                           \n"
        "       popl    %ebx                            \n"
#elif defined(__x86_64__)
        "       movq    $0, %rax                        \n"
        "       syscall                                 \n"
#elif defined(__aarch64__)
        "       mov     x8, #63                         \n"
        "       svc     #0                              \n"
#elif defined(__riscv)
        "       li      a7, 63                          \n"
        "       ecall                                   \n"
#endif
        "       ret                                     \n"
);

/* ====================== _exit ====================== */

__asm__ (
        "       .global _exit                           \n"
        "_exit:                                         \n"
#if defined(__i386__)
        "       movl    4(%esp), %ebx                   \n"
        "       movl    $1, %eax                        \n"
        "       int     $0x80                           \n"
#elif defined(__x86_64__)
        "       movq    $60, %rax                       \n"
        "       syscall                                 \n"
#elif defined(__aarch64__)
        "       mov     x8, #93                         \n"
        "       svc     #0                              \n"
#elif defined(__riscv)
        "       li      a7, 93                          \n"
        "       ecall                                   \n"
#endif
        "_exit_hang:                                    \n"
        "       jmp     _exit_hang                      \n"
);

/* ====================== open ====================== */

__asm__ (
        "       .global open                            \n"
        "open:                                          \n"
#if defined(__i386__)
        "       pushl   %ebx                            \n"
        "       movl    8(%esp), %ebx                   \n"  /* pathname */
        "       movl    12(%esp), %ecx                  \n"  /* flags */
        "       movl    16(%esp), %edx                  \n"  /* mode */
        "       movl    $5, %eax                        \n"  /* SYS_OPEN */
        "       int     $0x80                           \n"
        "       popl    %ebx                            \n"
#elif defined(__x86_64__)
        "       movq    $2, %rax                        \n"  /* SYS_OPEN */
        "       syscall                                 \n"
#elif defined(__aarch64__)
        "       mov     x3, x2                          \n"  /* mode -> ARG3 */
        "       mov     x2, x1                          \n"  /* flags -> ARG2 */
        "       mov     x1, x0                          \n"  /* pathname -> ARG1 */
        "       mov     x0, #-100                       \n"  /* AT_FDCWD */
        "       mov     x8, #56                         \n"  /* SYS_OPENAT */
        "       svc     #0                              \n"
#elif defined(__riscv)
        "       mv      a3, a2                          \n"  /* mode -> ARG3 */
        "       mv      a2, a1                          \n"  /* flags -> ARG2 */
        "       mv      a1, a0                          \n"  /* pathname -> ARG1 */
        "       li      a0, -100                        \n"  /* AT_FDCWD */
        "       li      a7, 56                          \n"  /* SYS_OPENAT */
        "       ecall                                   \n"
#endif
        "       ret                                     \n"
);

/* ====================== println ====================== */

__asm__ (
        "       .global println                         \n"
        "println:                                       \n"
#if defined(__x86_64__)
        "       xorq    %rcx, %rcx                      \n"
        ".Llen:                                         \n"
        "       cmpb    $0, (%rdi,%rcx)                 \n"
        "       je      .Lwrite                         \n"
        "       incq    %rcx                            \n"
        "       jmp     .Llen                           \n"
        ".Lwrite:                                       \n"
        "       movq    %rcx, %rdx                      \n"  /* len */
        "       movq    %rdi, %rsi                      \n"  /* buf */
        "       movq    $1, %rdi                        \n"  /* fd = stdout */
        "       movq    $1, %rax                        \n"
        "       syscall                                 \n"
        "       movq    $1, %rdi                        \n"
        "       lea     nl(%rip), %rsi                  \n"
        "       movq    $1, %rdx                        \n"
        "       movq    $1, %rax                        \n"
        "       syscall                                 \n"
        "       ret                                     \n"
#elif defined(__i386__)
        "       pushl   %ebx                            \n"
        "       pushl   %esi                            \n"
        "       movl    12(%esp), %esi                  \n"  /* s */
        "       xorl    %ecx, %ecx                      \n"
        ".Llen32:                                       \n"
        "       cmpb    $0, (%esi,%ecx)                 \n"
        "       je      .Lwrite32                       \n"
        "       incl    %ecx                            \n"
        "       jmp     .Llen32                         \n"
        ".Lwrite32:                                     \n"
        "       movl    %ecx, %edx                      \n"
        "       movl    %esi, %ecx                      \n"
        "       movl    $1, %ebx                        \n"
        "       movl    $4, %eax                        \n"
        "       int     $0x80                           \n"
        "       movl    $1, %ebx                        \n"
        "       leal    nl, %ecx                        \n"
        "       movl    $1, %edx                        \n"
        "       movl    $4, %eax                        \n"
        "       int     $0x80                           \n"
        "       popl    %esi                            \n"
        "       popl    %ebx                            \n"
        "       ret                                     \n"
#elif defined(__aarch64__)
        "       mov     x3, 0                           \n"
        ".LlenA64:                                      \n"
        "       ldrb    w4, [x0, x3]                    \n"
        "       cbz     w4, .LwriteA64                  \n"
        "       add     x3, x3, 1                       \n"
        "       b       .LlenA64                        \n"
        ".LwriteA64:                                    \n"
        "       mov     x2, x3                          \n"  /* len */
        "       mov     x1, x0                          \n"  /* buf */
        "       mov     x0, 1                           \n"  /* fd */
        "       mov     x8, #64                         \n"
        "       svc     #0                              \n"
        "       mov     x0, 1                           \n"
        "       adrp    x1, nl                          \n"
        "       add     x1, x1, :lo12:nl                \n"
        "       mov     x2, 1                           \n"
        "       mov     x8, #64                         \n"
        "       svc     #0                              \n"
        "       ret                                     \n"
#elif defined(__riscv)
        "       li      t0, 0                           \n"
        ".LlenRV:                                       \n"
        "       add     t1, a0, t0                      \n"
        "       lb      t2, 0(t1)                       \n"
        "       beqz    t2, .LwriteRV                   \n"
        "       addi    t0, t0, 1                       \n"
        "       j       .LlenRV                         \n"
        ".LwriteRV:                                     \n"
        "       mv      a2, t0                          \n"  /* len */
        "       mv      a1, a0                          \n"  /* buf */
        "       li      a0, 1                           \n"  /* fd */
        "       li      a7, 64                          \n"
        "       ecall                                   \n"
        "       li      a0, 1                           \n"
        "       lla     a1, nl                          \n"
        "       li      a2, 1                           \n"
        "       li      a7, 64                          \n"
        "       ecall                                   \n"
        "       ret                                     \n"
#endif
);

/* ====================== print ====================== */

__asm__ (
        "       .global print                           \n"
        "print:                                         \n"
#if defined(__x86_64__)
        "       xorq    %rcx, %rcx                      \n"
        ".LlenP:                                        \n"
        "       cmpb    $0, (%rdi,%rcx)                 \n"
        "       je      .LwriteP                        \n"
        "       incq    %rcx                            \n"
        "       jmp     .LlenP                          \n"
        ".LwriteP:                                      \n"
        "       movq    %rcx, %rdx                      \n"  /* len */
        "       movq    %rdi, %rsi                      \n"  /* buf */
        "       movq    $1, %rdi                        \n"  /* fd = stdout */
        "       movq    $1, %rax                        \n"
        "       syscall                                 \n"
        "       ret                                     \n"
#elif defined(__i386__)
        "       pushl   %ebx                            \n"
        "       pushl   %esi                            \n"
        "       movl    12(%esp), %esi                  \n"  /* s */
        "       xorl    %ecx, %ecx                      \n"
        ".LlenP32:                                      \n"
        "       cmpb    $0, (%esi,%ecx)                 \n"
        "       je      .LwriteP32                      \n"
        "       incl    %ecx                            \n"
        "       jmp     .LlenP32                        \n"
        ".LwriteP32:                                    \n"
        "       movl    %ecx, %edx                      \n"
        "       movl    %esi, %ecx                      \n"
        "       movl    $1, %ebx                        \n"
        "       movl    $4, %eax                        \n"
        "       int     $0x80                           \n"
        "       popl    %esi                            \n"
        "       popl    %ebx                            \n"
        "       ret                                     \n"
#elif defined(__aarch64__)
        "       mov     x3, 0                           \n"
        ".LlenPA64:                                     \n"
        "       ldrb    w4, [x0, x3]                    \n"
        "       cbz     w4, .LwritePA64                 \n"
        "       add     x3, x3, 1                       \n"
        "       b       .LlenPA64                       \n"
        ".LwritePA64:                                   \n"
        "       mov     x2, x3                          \n"  /* len */
        "       mov     x1, x0                          \n"  /* buf */
        "       mov     x0, 1                           \n"  /* fd */
        "       mov     x8, #64                         \n"
        "       svc     #0                              \n"
        "       ret                                     \n"
#elif defined(__riscv)
        "       li      t0, 0                           \n"
        ".LlenPRV:                                      \n"
        "       add     t1, a0, t0                      \n"
        "       lb      t2, 0(t1)                       \n"
        "       beqz    t2, .LwritePRV                  \n"
        "       addi    t0, t0, 1                       \n"
        "       j       .LlenPRV                        \n"
        ".LwritePRV:                                    \n"
        "       mv      a2, t0                          \n"  /* len */
        "       mv      a1, a0                          \n"  /* buf */
        "       li      a0, 1                           \n"  /* fd */
        "       li      a7, 64                          \n"
        "       ecall                                   \n"
        "       ret                                     \n"
#endif
);

/* ====================== math ====================== */

__asm__ (
        "       .global math_abs                        \n"
        "math_abs:                                      \n"
#if defined(__x86_64__)
        "       movl    %edi, %eax                      \n"
        "       testl   %eax, %eax                      \n"
        "       jge     1f                              \n"
        "       negl    %eax                            \n"
#elif defined(__i386__)
        "       movl    4(%esp), %eax                   \n"
        "       testl   %eax, %eax                      \n"
        "       jge     1f                              \n"
        "       negl    %eax                            \n"
#elif defined(__aarch64__)
        "       cmp     w0, 0                           \n"
        "       b.ge    1f                              \n"
        "       neg     w0, w0                          \n"
#elif defined(__riscv)
        "       bgez    a0, 1f                          \n"
        "       neg     a0, a0                          \n"
#endif
        "1:                                             \n"
        "       ret                                     \n"
);

__asm__ (
        "       .global math_max                        \n"
        "math_max:                                      \n"
#if defined(__x86_64__)
        "       movl    %edi, %eax                      \n"
        "       cmpl    %esi, %eax                      \n"
        "       jge     1f                              \n"
        "       movl    %esi, %eax                      \n"
#elif defined(__i386__)
        "       movl    4(%esp), %eax                   \n"
        "       cmpl    8(%esp), %eax                   \n"
        "       jge     1f                              \n"
        "       movl    8(%esp), %eax                   \n"
#elif defined(__aarch64__)
        "       cmp     w0, w1                          \n"
        "       b.ge    1f                              \n"
        "       mov     w0, w1                          \n"
#elif defined(__riscv)
        "       bge     a0, a1, 1f                      \n"
        "       mv      a0, a1                          \n"
#endif
        "1:                                             \n"
        "       ret                                     \n"
);

__asm__ (
        "       .global math_min                        \n"
        "math_min:                                      \n"
#if defined(__x86_64__)
        "       movl    %edi, %eax                      \n"
        "       cmpl    %esi, %eax                      \n"
        "       jle     1f                              \n"
        "       movl    %esi, %eax                      \n"
#elif defined(__i386__)
        "       movl    4(%esp), %eax                   \n"
        "       cmpl    8(%esp), %eax                   \n"
        "       jle     1f                              \n"
        "       movl    8(%esp), %eax                   \n"
#elif defined(__aarch64__)
        "       cmp     w0, w1                          \n"
        "       b.le    1f                              \n"
        "       mov     w0, w1                          \n"
#elif defined(__riscv)
        "       ble     a0, a1, 1f                      \n"
        "       mv      a0, a1                          \n"
#endif
        "1:                                             \n"
        "       ret                                     \n"
);

/* ====================== C declarations ====================== */

extern char   **environ;

long  write(int fd, const void *buf, __SIZE_TYPE__ count);
long  read(int fd, void *buf, __SIZE_TYPE__ count);
int   open(const char *pathname, int flags, int mode);
void  _exit(int status);
void  print(const char *s);
void  println(const char *s);
int   math_abs(int x);
int   math_max(int a, int b);
int   math_min(int a, int b);

#endif /* MINIL_H */
