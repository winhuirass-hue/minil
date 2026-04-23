/* This software is dedicated to the public domain under CC0 1.0 Universal. */
/* See LICENCE.md for full legal text. */

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

.extern __init_array_start
.extern __init_array_end

call_init_array:
#if defined(__x86_64__)
        leaq __init_array_start(%rip), %rdi
        leaq __init_array_end(%rip), %rsi
1:
        cmp %rdi, %rsi
        je 2f
        movq (%rdi), %rax
        addq $8, %rdi
        call *%rax
        jmp 1b
2:
        ret

#else

        movl $__init_array_start, %esi
        movl $__init_array_end, %edi
1:
        cmpl %esi, %edi
        je 2f
        movl (%esi), %eax
        addl $4, %esi
        call *%eax
        jmp 1b
2:
        ret

#endif

_start:

#if defined(__x86_64__)

/* ---------- x86-64 ---------- */

        xorq    %rbp, %rbp

        movq    (%rsp), %rdi            /* argc */
        leaq    8(%rsp), %rsi           /* argv */
        leaq    8(%rsi,%rdi,8), %rdx    /* envp */

        movq    %rdx, environ(%rip)

        call    call_init_array
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
        call    call_init_array
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

        movl    4(%esp), %ebx           /* fd */
        movl    8(%esp), %ecx           /* buf */
        movl    12(%esp), %edx          /* len */
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

        movl    4(%esp), %ebx
        movl    8(%esp), %ecx
        movl    12(%esp), %edx
        movl    $3, %eax
        int     $0x80

#endif
        ret


        .global _exit
_exit:
#if defined(__x86_64__)
        /* rdi already contains status */
        movq    $60, %rax               /* SYS_exit */
        syscall
#else
        movl    4(%esp), %ebx
        movl    $1, %eax
        int     $0x80
#endif
