# --------------------------------------------------
# minil Makefile
# --------------------------------------------------

CC  ?= gcc
CXX ?= g++

COMMON := -ffreestanding -nostdlib -no-pie -O2

CFLAGS   := $(COMMON)
CXXFLAGS := $(COMMON) -std=c++23 \
            -nostdinc++ \
            -fno-exceptions -fno-rtti -fno-threadsafe-statics

# Sources
ASM_SRC := minil.S
C_SRC   := exender.c
CPP_SRC := app.cpp

# Objects
ASM_OBJ := minil.o
C_OBJ   := exender.o
CPP_OBJ := app.o

TARGET   := app
TARGET32 := app32

# --------------------------------------------------
# Default build (x86-64)
# --------------------------------------------------

all: $(TARGET)

$(TARGET): $(ASM_OBJ) $(C_OBJ) $(CPP_OBJ)
    $(CXX) $(CXXFLAGS) $^ -o $@

# --------------------------------------------------
# 32-bit build (i386)
# --------------------------------------------------

i386: $(TARGET32)

$(TARGET32): $(ASM_SRC) $(C_SRC) $(CPP_SRC)
    $(CXX) -m32 $(CXXFLAGS) $^ -o $@

# --------------------------------------------------
# Compile rules
# --------------------------------------------------

minil.o: minil.S
    $(CC) $(CFLAGS) -c $< -o $@

exender.o: exender.c
    $(CC) $(CFLAGS) -c $< -o $@

app.o: app.cpp
    $(CXX) $(CXXFLAGS) -c $< -o $@

# --------------------------------------------------
# Helpers
# --------------------------------------------------

run: $(TARGET)
    ./$(TARGET)

clean:
    rm -f *.o $(TARGET) $(TARGET32)

.PHONY: all i386 run clean