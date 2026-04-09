/* minil.S  — single-file mini libc (x86-64 Linux) */
/* syscall-only, no glibc, no stdio, no stdlib */

.global _start
.global read, write, open, close
.global fork, execve, waitpid
.global _exit, getpid, gethostname
.global unlink, rename

.section .text

/* ---------------- entry point ---------------- */
.extern main
_start:
    xor %rbp, %rbp
    call main
    mov %rax, %rdi
    mov $60, %rax          /* SYS_exit */
    syscall

/* ---------------- syscalls ---------------- */

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
