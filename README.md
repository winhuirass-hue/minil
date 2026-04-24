# minil

<p align="center">
  <img src="https://img.shields.io/badge/arch-x86__64%20%7C%20i386-blue.svg">
  <img src="https://img.shields.io/badge/libc-none-red.svg">
</p>

**minil** is a minimal Linux user‑space runtime written in pure assembly and
freestanding C.  
It runs **without glibc, crt objects, stdio, stdlib, or C++ runtime**.

The project demonstrates full control over the Linux process entry point,
ABI, and system calls.

---

## Features

- Custom `_start` (no `crt1.o`)
- No glibc / libc / libgcc / libstdc++
- Direct Linux syscalls
  - x86‑64: `syscall`
  - i386: `int 0x80`
- Manual `argc`, `argv`, `envp` extraction
- Proper `environ` initialization
- One `.S` file with C preprocessor for multi‑arch
- Freestanding C and C++ application support

---

## File Layout

---

## How It Works

1. Linux kernel jumps to `_start`
2. `_start` reads `argc`, `argv`, `envp` from stack
3. Global `environ` is set manually
4. `main()` is called directly
5. Process exits via `_exit` syscall

---

## Build

Single‑command build (recommended):

```bash
gcc -nostdlib -no-pie minil.S app.c -O2 -ffreestanding -o app
```

### 32‑bit (i386)

```bash
gcc -m32 -nostdlib -no-pie minil.S app.c -O2 -ffreestanding -o app32
```

---

## Example `app.c`

```c
long write(int, const void*, unsigned long);
void _exit(int);

int main(void) {
#if defined(__x86_64__)
    const char msg[] = "Hello from 64 bit minil\n";
#else
    const char msg[] = "Hello from 32 bit minil\n";
#endif
    write(1, msg, sizeof(msg) - 1);
    _exit(0);
}
```

---

## Use Cases

- Minimal ELF utilities
- Learning Linux ABI and process startup
- Security research / exploit development
- OS and runtime development
- libc‑free sandboxed tools


<p align="right">
  <a href="https://creativecommons.org/publicdomain/zero/1.0/">
    <img src="https://licensebuttons.net/p/zero/1.0/88x31.png" alt="License: CC0 1.0">
  </a>
</p>

