/* a minil c library */
.intel_syntax noprefix
.global _start
.global environ

.extern main

.section .bss
#if defined(__x86_64__)
environ:
    .quad 0
#else
environ:
    .long 0
#endif

.section .text

_start:

#if defined(__x86_64__)

    xor rbp, rbp

    mov rdi, [rsp]              /* argc */
    lea rsi, [rsp+8]            /* argv */
    lea rdx, [rsi+rdi*8+8]      /* envp */

    mov [rel environ], rdx

    call main

    mov rdi, rax
    mov rax, 60                 
    syscall

#else   /* ==================== i386 ==================== */

    xor ebp, ebp

    mov eax, [esp]              /* argc */
    lea ecx, [esp+4]            /* argv */
    lea edx, [ecx+eax*4+4]      /* envp */

    mov [environ], edx

    push edx
    push ecx
    push eax
    call main

    mov ebx, eax
    mov eax, 1                  /* SYS_exit */
    int 0x80

#endif


/* ====================== syscalls ====================== */

.global write
write:
#if defined(__x86_64__)
    mov rax, 1                  /* SYS_write */
    syscall
#else
    mov eax, 4                  /* SYS_write */
    int 0x80
#endif
    ret


.global read
read:
#if defined(__x86_64__)
    mov rax, 0                  /* SYS_read */
    syscall
#else
    mov eax, 3                  /* SYS_read */
    int 0x80
#endif
    ret


.global _exit
_exit:
#if defined(__x86_64__)
    mov rax, 60                 /* SYS_exit */
    syscall
#else
    mov eax, 1
    int 0x80
#endif
