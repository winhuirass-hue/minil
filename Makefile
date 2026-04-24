# --------------------------------------------------
# minil Makefile
# --------------------------------------------------

# Toolchain
CXX      ?= g++
CFLAGS   := -std=c++23 \
            -ffreestanding \
            -nostdlib -nostdinc++ \
            -fno-exceptions -fno-rtti -fno-threadsafe-statics \
            -no-pie \
            -O2

# Sources
ASM_SRC  := minil.S
C_SRC    := exender.c
CPP_SRC  := app.cpp

# Targets
TARGET   := app
TARGET32 := app32

# --------------------------------------------------
# Default target (x86-64)
# --------------------------------------------------

all: $(TARGET)

$(TARGET): $(ASM_SRC) $(C_SRC) $(CPP_SRC)
    $(CXX) $(CFLAGS) $^ -o $@

# --------------------------------------------------
# 32-bit build (i386)
# --------------------------------------------------

i386: $(TARGET32)

$(TARGET32): $(ASM_SRC) $(C_SRC) $(CPP_SRC)
    $(CXX) -m32 $(CFLAGS) $^ -o $@

# --------------------------------------------------
# Run helpers
# --------------------------------------------------

run: $(TARGET)
    ./$(TARGET)

run32: $(TARGET32)
    ./$(TARGET32)

# --------------------------------------------------
# Clean
# --------------------------------------------------

clean:
    rm -f $(TARGET) $(TARGET32)

.PHONY: all i386 run run32 clean