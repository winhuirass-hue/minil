# --------------------------------------------------
# minil Makefile
# --------------------------------------------------

CC  ?= gcc
CXX ?= g++

COMMON := -ffreestanding -nostdlib -no-pie -O2 \
          -ffunction-sections -fdata-sections

CFLAGS   := $(COMMON)
CXXFLAGS := $(COMMON) -std=c++23 \
            -nostdinc++ \
            -fno-exceptions -fno-rtti -fno-threadsafe-statics

LDFLAGS := -Wl,--gc-sections

# Sources
ASM_SRC := minil.S
C_SRC   := exender.c
CPP_SRC := app.cpp

# Objects (x86-64)
ASM_OBJ := minil.o
C_OBJ   := exender.o
CPP_OBJ := app.o

# Objects (i386)
ASM_OBJ32 := minil32.o
C_OBJ32   := exender32.o
CPP_OBJ32 := app32.o

TARGET   := app
TARGET32 := app32

# --------------------------------------------------
# Default build (x86-64)
# --------------------------------------------------

all: $(TARGET)

$(TARGET): $(ASM_OBJ) $(C_OBJ) $(CPP_OBJ)
    $(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

# --------------------------------------------------
# i386 build
# --------------------------------------------------

i386: $(TARGET32)

$(TARGET32): $(ASM_OBJ32) $(C_OBJ32) $(CPP_OBJ32)
    $(CXX) -m32 $(CXXFLAGS) $(LDFLAGS) $^ -o $@

# --------------------------------------------------
# Compile rules
# --------------------------------------------------

# x86-64
minil.o: minil.S
    $(CC) $(CFLAGS) -c $< -o $@

exender.o: exender.c
    $(CC) $(CFLAGS) -c $< -o $@

app.o: app.cpp
    $(CXX) $(CXXFLAGS) -c $< -o $@

# i386
minil32.o: minil.S
    $(CC) -m32 $(CFLAGS) -c $< -o $@

exender32.o: exender.c
    $(CC) -m32 $(CFLAGS) -c $< -o $@

app32.o: app.cpp
    $(CXX) -m32 $(CXXFLAGS) -c $< -o $@

# --------------------------------------------------
# Helpers
# --------------------------------------------------

run: $(TARGET)
    ./$(TARGET)

run32: $(TARGET32)
    ./$(TARGET32)

clean:
    rm -f *.o $(TARGET) $(TARGET32)

.PHONY: all i386 run run32 clean
``
