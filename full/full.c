/* This software is dedicated to the public domain under CC0 1.0 Universal. */
/* See LICENCE.md for full legal text. */

#ifdef __cplusplus
extern "C" {
#endif

#define PROT_NONE   0x0
#define PROT_READ   0x1
#define PROT_WRITE  0x2
#define PROT_EXEC   0x4

#define MAP_SHARED      0x01
#define MAP_PRIVATE     0x02
#define MAP_FIXED       0x10
#define MAP_ANONYMOUS   0x20

size_t strlen(const char*);
int strncmp(const char*, const char*, size_t);

#if __STDC_VERSION__ >= 202311L

#define ckd_add(r,a,b) __builtin_add_overflow((a),(b),(r))
#define ckd_sub(r,a,b) __builtin_sub_overflow((a),(b),(r))
#define ckd_mul(r,a,b) __builtin_mul_overflow((a),(b),(r))

#endif

#define ALIGNOF(t) __alignof__(t)
#define OFFSETOF(t,m) __builtin_offsetof(t,m)

#define LIKELY(x)   __builtin_expect(!!(x), 1)
#define UNLIKELY(x) __builtin_expect(!!(x), 0)

#define TRAP()      __builtin_trap()

typedef __INTPTR_TYPE__ intptr_t;

typedef __SIZE_TYPE__ size_t;
typedef unsigned char u8;
typedef unsigned int  u32;

typedef signed char        i8;
typedef int                i32;

typedef unsigned short sa_family_t;
typedef unsigned int   socklen_t;

struct sockaddr {
    sa_family_t sa_family;
    char sa_data[14];
};

struct sockaddr_un {
    sa_family_t sun_family;
    char sun_path[108];
};

#define AF_UNIX      1
#define AF_INET      2

#define SOCK_STREAM  1
#define SOCK_DGRAM   2

extern void _exit(int) __attribute__((noreturn));
extern char** environ;

/* --------------------------------------------------
 * abort()
 * -------------------------------------------------- */
__attribute__((noreturn))
void abort(void)
{
    _exit(127);
}

void __cxa_pure_virtual(void)
{
    abort();
}

/* --------------------------------------------------
 * Raw Linux syscall helpers
 * -------------------------------------------------- */

#if defined(__x86_64__)

static long sys_call1(long n, long a)
{
    long ret;

    __asm__ volatile (
        "syscall"
        : "=a"(ret)
        : "a"(n), "D"(a)
        : "rcx", "r11", "memory"
    );

    return ret;
}

static long sys_call2(long n, long a, long b)
{
    long ret;

    __asm__ volatile (
        "syscall"
        : "=a"(ret)
        : "a"(n), "D"(a), "S"(b)
        : "rcx", "r11", "memory"
    );

    return ret;
}

static long sys_call3(long n, long a, long b, long c)
{
    long ret;

    __asm__ volatile (
        "syscall"
        : "=a"(ret)
        : "a"(n), "D"(a), "S"(b), "d"(c)
        : "rcx", "r11", "memory"
    );

    return ret;
}

static long sys_call6(long n,
                      long a,
                      long b,
                      long c,
                      long d,
                      long e,
                      long f)
{
    long ret;

    register long r10 __asm__("r10") = d;
    register long r8  __asm__("r8")  = e;
    register long r9  __asm__("r9")  = f;

    __asm__ volatile (
        "syscall"
        : "=a"(ret)
        : "a"(n),
          "D"(a),
          "S"(b),
          "d"(c),
          "r"(r10),
          "r"(r8),
          "r"(r9)
        : "rcx", "r11", "memory"
    );

    return ret;
}

#else

static long sys_call1(long n, long a)
{
    long ret;

    __asm__ volatile (
        "int $0x80"
        : "=a"(ret)
        : "a"(n), "b"(a)
        : "memory"
    );

    return ret;
}

static long sys_call2(long n, long a, long b)
{
    long ret;

    __asm__ volatile (
        "int $0x80"
        : "=a"(ret)
        : "a"(n), "b"(a), "c"(b)
        : "memory"
    );

    return ret;
}

static long sys_call3(long n, long a, long b, long c)
{
    long ret;

    __asm__ volatile (
        "int $0x80"
        : "=a"(ret)
        : "a"(n), "b"(a), "c"(b), "d"(c)
        : "memory"
    );

    return ret;
}

static long sys_call6(long n,
                      long a,
                      long b,
                      long c,
                      long d,
                      long e,
                      long f)
{
    long ret;

    __asm__ volatile (
        "pushl %%ebp\n\t"
        "movl %[arg6], %%ebp\n\t"
        "int $0x80\n\t"
        "popl %%ebp\n\t"
        : "=a"(ret)
        : "0"(n),
          "b"(a),
          "c"(b),
          "d"(c),
          "S"(d),
          "D"(e),
          [arg6] "r"(f)
        : "memory"
    );

    return ret;
}

#endif


/* --------------------------------------------------
 * close()
 * -------------------------------------------------- */

int close(int fd)
{
#if defined(__x86_64__)
    return (int)sys_call1(3, fd);       /* SYS_close = 3 */
#else
    return (int)sys_call1(6, fd);       /* SYS_close = 6 */
#endif
}

/* --------------------------------------------------
 * socket()
 * -------------------------------------------------- */

int socket(int domain, int type, int protocol)
{
#if defined(__x86_64__)
    return (int)sys_call3(41, domain, type, protocol);   /* SYS_socket */
#else
    unsigned long args[3];

    args[0] = (unsigned long)domain;
    args[1] = (unsigned long)type;
    args[2] = (unsigned long)protocol;

    return (int)sys_call2(102, 1, (long)args);           /* socketcall(SYS_SOCKET) */
#endif
}

/* --------------------------------------------------
 * connect()
 * -------------------------------------------------- */

int connect(int fd, const struct sockaddr* addr, socklen_t len)
{
#if defined(__x86_64__)
    return (int)sys_call3(42, fd, (long)addr, len);      /* SYS_connect */
#else
    unsigned long args[3];

    args[0] = (unsigned long)fd;
    args[1] = (unsigned long)addr;
    args[2] = (unsigned long)len;

    return (int)sys_call2(102, 3, (long)args);           /* socketcall(SYS_CONNECT) */
#endif
}

/* --------------------------------------------------
 * mmap()
 * Raw Linux syscall.
 *
 * Returns:
 *   success: mapped address
 *   failure: negative errno casted to void*
 *
 * If you want libc-like behavior, wrap negative values
 * into MAP_FAILED later.
 * -------------------------------------------------- */

void* mmap(void* addr,
           size_t len,
           int prot,
           int flags,
           int fd,
           long off)
{
#if defined(__x86_64__)

    long ret;

    ret = sys_call6(
        9,              /* SYS_mmap */
        (long)addr,
        (long)len,
        (long)prot,
        (long)flags,
        (long)fd,
        (long)off
    );

    return (void*)(intptr_t)ret;

#else

    struct mmap_args {
        unsigned long addr;
        unsigned long len;
        unsigned long prot;
        unsigned long flags;
        unsigned long fd;
        unsigned long offset;
    } args;

    long ret;

    args.addr   = (unsigned long)addr;
    args.len    = (unsigned long)len;
    args.prot   = (unsigned long)prot;
    args.flags  = (unsigned long)flags;
    args.fd     = (unsigned long)fd;
    args.offset = (unsigned long)off;

    ret = sys_call1(90, (long)&args);   /* SYS_mmap = 90 */

   return (void*)(intptr_t)ret;
    
#endif
}

/* --------------------------------------------------
 * munmap()
 * -------------------------------------------------- */

int munmap(void* addr, size_t len)
{
#if defined(__x86_64__)
    return (int)sys_call2(11, (long)addr, (long)len);  /* SYS_munmap = 11 */
#else
    return (int)sys_call2(91, (long)addr, (long)len);  /* SYS_munmap = 91 */
#endif
}

/* --------------------------------------------------
 * Memory helpers
 * -------------------------------------------------- */

void* memset(void* dst, int val, size_t n)
{
    u8* p = (u8*)dst;
    u8 v = (u8)val;

    while (((uintptr_t)p & (sizeof(size_t) - 1)) && n) {
        *p++ = v;
        --n;
    }

    size_t word = 0;

    for (size_t i = 0; i < sizeof(size_t); ++i)
        word = (word << 8) | v;

    size_t* w = (size_t*)p;

    while (n >= sizeof(size_t)) {
        *w++ = word;
        n -= sizeof(size_t);
    }

    p = (u8*)w;

    while (n--)
        *p++ = v;

    return dst;
}

void* memcpy(void* dst, const void* src, size_t n)
{
    u8* d = (u8*)dst;
    const u8* s = (const u8*)src;

    while (((uintptr_t)d & (sizeof(size_t) - 1)) &&
           ((uintptr_t)s & (sizeof(size_t) - 1)) &&
           n)
    {
        *d++ = *s++;
        --n;
    }

    size_t* dw = (size_t*)d;
    const size_t* sw = (const size_t*)s;

    while (n >= sizeof(size_t)) {
        *dw++ = *sw++;
        n -= sizeof(size_t);
    }

    d = (u8*)dw;
    s = (const u8*)sw;

    while (n--)
        *d++ = *s++;

    return dst;
}

void* memmove(void* dst, const void* src, size_t n)
{
    u8* d = (u8*)dst;
    const u8* s = (const u8*)src;

    if (d == s || n == 0)
        return dst;

    if (d < s) {
        return memcpy(dst, src, n);
    }

    d += n;
    s += n;

    while (n--)
        *--d = *--s;

    return dst;
}

/* --------------------------------------------------
 * getenv()
 * -------------------------------------------------- */

char* getenv(const char* name)
{
    size_t name_len;

    if (!name || !*name)
        return 0;

    name_len = strlen(name);

    if (!environ)
        return 0;

    for (char** e = environ; *e; ++e) {
        if (strncmp(*e, name, name_len) == 0 &&
            (*e)[name_len] == '=') {
            return *e + name_len + 1;
        }
    }

    return 0;
}

/* --------------------------------------------------
 * Mini heap allocator
 * -------------------------------------------------- */

#define HEAP_BLOCK_SIZE (1024UL * 1024UL) 
#define HEAP_COUNT      4
#define HEAP_SIZE       (HEAP_BLOCK_SIZE * HEAP_COUNT)
#define ALIGNMENT   16UL
#define MAGIC_USED  ((size_t)0xC0FFEEUL)
#define MAGIC_FREE ((size_t)0xDEADBEEFUL)

#define ALIGN_UP(x) (((x) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))

typedef struct HeapBlock {
    size_t size;
    int free;
    int is_mmap;
    size_t magic;
    struct HeapBlock* next;
} HeapBlock;

#define HEADER_SIZE ALIGN_UP(sizeof(HeapBlock))

static u8 heap[HEAP_SIZE] __attribute__((aligned(16)));
static size_t heap_off = 0;
static HeapBlock* heap_head = 0;

static size_t heap_align_size(size_t n)
{
    size_t aligned;

    if (n == 0)
        n = 1;

    if (n > ((size_t)-1) - (ALIGNMENT - 1))
        abort();

    aligned = ALIGN_UP(n);

    return aligned;
}

static void heap_split_block(HeapBlock* block, size_t wanted)
{
    size_t remaining;
    HeapBlock* next;

    if (!block)
        return;

    if (block->size <= wanted)
        return;

    remaining = block->size - wanted;

    if (remaining < HEADER_SIZE + ALIGNMENT)
        return;

    next = (HeapBlock*)((u8*)block + HEADER_SIZE + wanted);

    next->size = remaining - HEADER_SIZE;
    next->free = 1;
    next->magic = MAGIC_USED;
    next->next = block->next;

    block->size = wanted;
    block->next = next;
}

static void heap_coalesce(void)
{
    HeapBlock* cur = heap_head;

    while (cur && cur->next) {
        u8* cur_end = (u8*)cur + HEADER_SIZE + cur->size;

        if (cur->free &&
            cur->next->free &&
            cur_end == (u8*)cur->next) {

            cur->size += HEADER_SIZE + cur->next->size;
            cur->next = cur->next->next;
            continue;
        }

        cur = cur->next;
    }
}

static HeapBlock* heap_find_free(size_t n)
{
    HeapBlock* cur = heap_head;

    while (cur) {
        if (cur->free && cur->size >= n)
            return cur;

        cur = cur->next;
    }

    return 0;
}

static HeapBlock* heap_new_block(size_t n)
{
    HeapBlock* block;
    size_t need;

    if (n > ((size_t)-1) - HEADER_SIZE)
        abort();

    need = HEADER_SIZE + n;

    if (need > HEAP_SIZE - heap_off)
        abort();

    block = (HeapBlock*)((u8*)heap + heap_off);

    block->size = n;
    block->free = 0;
    block->magic = MAGIC_USED;
    block->next = 0;

    if (!heap_head) {
        heap_head = block;
    } else {
        HeapBlock* cur = heap_head;

        while (cur->next)
            cur = cur->next;

        cur->next = block;
    }

    heap_off += need;

    return block;
}

static HeapBlock* heap_block_from_ptr(void* p)
{
    HeapBlock* block;

    if (!p)
        return 0;

    block = (HeapBlock*)((u8*)p - HEADER_SIZE);

    if (block->magic != MAGIC_USED)
        abort();

    return block;
}

/* --------------------------------------------------
 * malloc()
 * -------------------------------------------------- */


void* malloc(size_t n)
{
    HeapBlock* block;
    size_t wanted;

    wanted = heap_align_size(n);

    block = heap_find_free(wanted);

    if (block) {
        block->free = 0;
        block->magic = MAGIC_USED;
        heap_split_block(block, wanted);
        return (u8*)block + HEADER_SIZE;
    }

    block = heap_new_block(wanted);

    if (!block) {
        void* p = mmap(
            0,
            wanted,
            PROT_READ | PROT_WRITE,
            MAP_PRIVATE | MAP_ANONYMOUS,
            -1,
            0
        );
        
    if ((intptr_t)p < 0)
        return 0;

        return p;
    }


    return (u8*)block + HEADER_SIZE;
}


/* --------------------------------------------------
 * free()
 * -------------------------------------------------- */

void free(void* p)
{
    HeapBlock* block;

    if (!p)
        return;

    block = heap_block_from_ptr(p);

    if (block->magic != MAGIC_USED)
        abort();

    if (block->free)
        abort();

    block->magic = MAGIC_FREE;

    if (block->is_mmap) {
        munmap(block, block->size + HEADER_SIZE);
        return;
    }

    block->free = 1;

    heap_coalesce();
}

/* --------------------------------------------------
 * calloc()
 * -------------------------------------------------- */

void* calloc(size_t n, size_t s)
{
    size_t total;
    void* p;

#if __STDC_VERSION__ >= 202311L
    if (ckd_mul(&total, n, s))
        return 0;
#else
    if (n != 0 && s > ((size_t)-1) / n)
        return 0;

    total = n * s;
#endif

    p = malloc(total);

    if (!p)
        return 0;

    memset(p, 0, total);

    return p;
}

/* --------------------------------------------------
 * realloc()
 * -------------------------------------------------- */

void* realloc(void* p, size_t n)
{
    HeapBlock* block;
    size_t wanted;
    void* new_ptr;
    size_t copy_size;

    if (!p)
        return malloc(n);

    if (n == 0) {
        free(p);
        return 0;
    }

    block = heap_block_from_ptr(p);

    wanted = heap_align_size(n);

    if (block->size >= wanted) {
        heap_split_block(block, wanted);
        return p;
    }

    if (block->next && block->next->free) {
        u8* block_end =
            (u8*)block + HEADER_SIZE + block->size;

        if (block_end == (u8*)block->next) {
            size_t combined =
                block->size +
                HEADER_SIZE +
                block->next->size;

            if (combined >= wanted) {
                block->size = combined;
                block->next = block->next->next;

                heap_split_block(block, wanted);

                return p;
            }
        }
    }

    new_ptr = malloc(n);

    if (!new_ptr)
        return 0;

    copy_size = block->size;

    if (copy_size > n)
        copy_size = n;

    memcpy(new_ptr, p, copy_size);

    free(p);

    return new_ptr;
}

/* --------------------------------------------------
 * strlen
 * -------------------------------------------------- */
size_t strlen(const char* s)
{
    const char* p = s;

    while (*p)
        ++p;

    return (size_t)(p - s);
}

/* --------------------------------------------------
 * strcmp
 * -------------------------------------------------- */
int strcmp(const char* a, const char* b)
{
    while (*a && (*a == *b)) {
        ++a;
        ++b;
    }

    return (int)((unsigned char)*a - (unsigned char)*b);
}

/* --------------------------------------------------
 * strncmp
 * -------------------------------------------------- */
int strncmp(const char* a, const char* b, size_t n)
{
    while (n && *a && (*a == *b)) {
        ++a;
        ++b;
        --n;
    }

    if (n == 0)
        return 0;

    return (int)((unsigned char)*a - (unsigned char)*b);
}

/* --------------------------------------------------
 * memcmp
 * -------------------------------------------------- */
int memcmp(const void* a, const void* b, size_t n)
{
    const u8* x = (const u8*)a;
    const u8* y = (const u8*)b;

    for (size_t i = 0; i < n; ++i) {
        if (x[i] != y[i])
            return (int)x[i] - (int)y[i];
    }

    return 0;
}

/* --------------------------------------------------
 * puts
 * Puts text to println 
 * Requires write(int fd, const void* buf, size_t len).
 * -------------------------------------------------- */

extern void println(const char* s);

int puts(const char* s)
{
    if (!s)
        s = "(null)";

    println(s);
    return 0;
}

/* --------------------------------------------------
 * strdup
 * Allocates and duplicates a C string.
 * Requires malloc() and memcpy().
 * -------------------------------------------------- */
char* strdup(const char* s)
{
    size_t n;
    char* out;

    n = strlen(s);

    if (n == (size_t)-1)
        abort();

    n = n + 1;

    out = (char*)malloc(n);

    if (!out)
        abort();

    memcpy(out, s, n);

    return out;
}

#ifdef __cplusplus
}
#endif

/* --------------------------------------------------
 * C++ new/delete
 * -------------------------------------------------- */

#ifdef __cplusplus

/* Always needed (basic) */
void* operator new(size_t n) { return malloc(n); }
void* operator new[](size_t n) { return malloc(n); }

void operator delete(void* p) noexcept { free(p); }
void operator delete[](void* p) noexcept { free(p); }

/* Detector and addition of sized delete for C++14 and above */
#if __cplusplus >= 201402L

void operator delete(void* p, size_t) noexcept
{
    free(p);
}

void operator delete[](void* p, size_t) noexcept
{
    free(p);
}

#endif // __cplusplus >= 201402L

#endif // __cplusplus
