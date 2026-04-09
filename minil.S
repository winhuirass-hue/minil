/* minil.S  — single-file mini libc (x86-64 Linux) */
/* syscall-only, no glibc, no stdio, no stdlib */

.global _start
.global read, write, open, close
.global fork, execve, waitpid
.global _exit, getpid, gethostname
.global unlink, rename
.global pipe, dup2, chdir, getcwd, geteuid
.global environ

.section .text

/* ---------------- entry point ---------------- */
.extern main
_start:
    xor %rbp, %rbp

    /* ----- extract envp from stack ----- */
    mov (%rsp), %rdi        # rdi = argc
    lea 8(%rsp), %rsi       # rsi = argv
    lea 8(%rsi,%rdi,8), %rdx  # rdx = &argv[argc]
    add $8, %rdx            # skip NULL → envp

    mov %rdx, environ(%rip) # environ = envp

    call main

    mov %rax, %rdi
    mov $60, %rax            # SYS_exit
    syscall


/* ---------------- syscalls ---------------- */
environ:
    .quad 0

/* ssize_t read(int fd, void *buf, size_t n) */
read:
    mov $0, %rax           /* SYS_read */
    syscall
    ret

/* ssize_t write(int fd, const void *buf, size_t n) */
write:
    mov $1, %rax           /* SYS_write */
    syscall
    ret

/* int open(const char *path, int flags, int mode) */
open:
    mov $2, %rax           /* SYS_open */
    syscall
    ret

/* int close(int fd) */
close:
    mov $3, %rax           /* SYS_close */
    syscall
    ret

/* int fork(void) */
fork:
    mov $57, %rax          /* SYS_fork */
    syscall
    ret

/* int execve(const char *p, char *const a[], char *const e[]) */
execve:
    mov $59, %rax          /* SYS_execve */
    syscall
    ret

/* int waitpid(int pid, int *status, int options) */
waitpid:
    mov $61, %rax          /* SYS_wait4 */
    syscall
    ret

/* void _exit(int code) */
_exit:
    mov $60, %rax          /* SYS_exit */
    syscall

/* int getpid(void) */
getpid:
    mov $39, %rax          /* SYS_getpid */
    syscall
    ret

/* int gethostname(char *buf, size_t len) */
gethostname:
    mov $170, %rax         /* SYS_gethostname */
    syscall
    ret

/* int unlink(const char *path) */
unlink:
    mov $87, %rax          /* SYS_unlink */
    syscall
    ret

/* int rename(const char *old, const char *new) */
rename:
    mov $82, %rax          /* SYS_rename */
    syscall
    ret

pipe:
    mov $22, %rax     # SYS_pipe
    syscall
    ret

dup2:
    mov $33, %rax     # SYS_dup2
    syscall
    ret

chdir:
    mov $80, %rax     # SYS_chdir
    syscall
    ret

getcwd:
    mov $79, %rax     # SYS_getcwd
    syscall
    ret

geteuid:
    mov $107, %rax    # SYS_geteuid
    syscall
    ret
