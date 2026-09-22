# Home

> minimal runtime, maximal control

minil is a minimal Linux user-space runtime written in assembly and
freestanding C.

It provides:

- Custom `_start`
- Direct Linux syscalls
- Manual `argc`, `argv`, `envp` handling
- `.init_array` constructor support
- Freestanding C/C++ support
- Multi-architecture implementation

---

## Supported Architectures

- x86_64
- i386
- AArch64
- RISC-V

---

## Philosophy

> no hidden runtime, no implicit initialization,
> no abstraction over syscalls.

---

## Runtime Flow

```text
Linux Kernel
      │
      ▼
    _start
      │
      ▼
argc / argv / envp
      │
      ▼
   environ
      │
      ▼
 .init_array
      │
      ▼
    main()
      │
      ▼
    _exit()
```
