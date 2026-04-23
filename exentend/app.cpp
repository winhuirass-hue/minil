extern "C" long write(int, const void*, unsigned long);

struct A {
    A() {
        write(1, "ctor\n", 5);
    }
};

A global;

int main() {
    write(1, "hello\n", 6);
    return 0;
}
