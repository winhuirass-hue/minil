# minil
'''bash
as minil.S -o minil.o
gcc -c app.c -ffreestanding -O2 -nostdlib -o app.o
ld -o app minil.o app.o -e _start
'''
