/* app.c */
long write(int, const void*, unsigned long);
void _exit(int);

int main(void) {
    const char msg[] = "Hello from one-file minil\n";
    write(1, msg, sizeof(msg)-1);
    _exit(0);
}
