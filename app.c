long write(int, const void*, unsigned long);
void _exit(int);

int main(void)
{
#if defined(__x86_64__)
    const char msg[] = "Hello from 64 bit minil\n";
#else
    const char msg[] = "Hello from 32 bit minil\n";
#endif

    write(1, msg, sizeof(msg) - 1);
    _exit(0);
}
