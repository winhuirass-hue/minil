/* This software is dedicated to the public domain under CC0 1.0 Universal. */
/* See LICENCE.l for full legal text. */

        .global _start
        .global environ
        .extern main

/* ====================== bss ====================== */
        .section .bss
#if defined(__x86_64__)
environ:
        .quad 0
#else
environ:
        .long 0
#endif


/* ====================== text ====================== */
        .section .text

_start:

#if defined(__x86_64__)

/* ---------- x86-64 ---------- */

        xorq    %rbp, %rbp

        movq    (%rsp), %rdi            /* argc */
        leaq    8(%rsp), %rsi           /* argv */
        leaq    8(%rsi,%rdi,8), %rdx    /* envp */

        movq    %rdx, environ(%rip)

        call    main

        movq    %rax, %rdi
        movq    $60, %rax               /* SYS_exit */
        syscall

#else   /* ---------- i386 ---------- */

        xorl    %ebp, %ebp

        movl    (%esp), %eax            /* argc */
        leal    4(%esp), %ecx           /* argv */
        leal    4(%ecx,%eax,4), %edx    /* envp */

        movl    %edx, environ

        pushl   %edx
        pushl   %ecx
        pushl   %eax
        call    main

        movl    %eax, %ebx
        movl    $1, %eax                /* SYS_exit */
        int     $0x80

#endif


/* ====================== syscalls ====================== */

        .global write
write:
#if defined(__x86_64__)
        movq    $1, %rax                /* SYS_write */
        syscall
#else
        movl    $4, %eax                /* SYS_write */
        int     $0x80
#endif
        ret


        .global read
read:
#if defined(__x86_64__)
        movq    $0, %rax                /* SYS_read */
        syscall
#else
        movl    $3, %eax                /* SYS_read */
        int     $0x80
#endif
        ret


        .global _exit
_exit:
#if defined(__x86_64__)
        movq    $60, %rax               /* SYS_exit */
        syscall
#else
        movl    $1, %eax
        int     $0x80
#endif
