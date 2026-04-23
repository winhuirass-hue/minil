extern "C" long write(int, const void*, unsigned long);

int main() {
struct A {
    A() {
        write(1, "ctor\n", 5);
    }
};

A global;
}