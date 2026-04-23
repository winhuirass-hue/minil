
/* Minimal C support layer for freestanding C++
 * Works with minil.S
 * No malloc, no heap, no libc
 * CC0 1.0
 */

extern void _exit(int);

/* --------------------------------------------------
 * abort()
 * Required by C and C++
 * -------------------------------------------------- */
void abort(void)
{
    _exit(127);
}

/* --------------------------------------------------
 * __cxa_pure_virtual
 * Required by C++ ABI (pure virtual call)
 * -------------------------------------------------- */
void __cxa_pure_virtual(void)
{
    abort();
}

