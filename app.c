/* This software is dedicated to the public domain under CC0 1.0 Universal. */
/* See LICENCE.md for full legal text. */
void println(const char *);
void _exit(int);

int main(void)
{
#if defined(__x86_64__)
    println("Hello from 64 bit minil");
#else
    println("Hello from 32 bit minil");
#endif
    _exit(0);
}
