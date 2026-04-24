/* This software is dedicated to the public domain under CC0 1.0 Universal. */
/* See LICENCE.md for full legal text. */
void println(const char *);
void _exit(int);

int main(void)
{
#if defined(__x86_64__)
    println("Hello from 64 bit minil\n");
#else
    println("Hello from 32 bit minil\n");
#endif
    _exit(0);
}
