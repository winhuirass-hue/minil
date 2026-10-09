# Getting Started

Welcome to **minil**.

minil is a minimal Linux user-space runtime written in pure assembly and freestanding C.

Unlike conventional applications, minil runs without:

- glibc
- musl
- CRT objects (`crt1.o`, `crti.o`, `crtn.o`)
- libstdc++
- standard I/O libraries

Instead, the runtime communicates directly with the Linux kernel through system calls.

---

# Requirements

Supported architectures:

- x86_64
- i386
- AArch64
- RISC-V

Required tools:

- GCC, Clang, or Zig CC
- GNU Make
- Linux

---

# Building

Clone the repository:

```bash
git clone https://github.com/winhuirass-hue/minil.git
cd minil
```

```bash
gcc -nostdlib -fno-pie -fcf-protection=none -no-pie minil.S app.c -O2 -ffreestanding -o app
```

### 32‑bit (i386)

```bash
gcc -m32 -fno-pie -fcf-protection=none -nostdlib -no-pie minil.S app.c -O2 -ffreestanding -o app32
```





