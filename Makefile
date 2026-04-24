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

# Objects
ASM_OBJ := minil.o
C_OBJ   := exender.o
CPP_OBJ := app.o

TARGET := app

# --------------------------------------------------
# Build (x86-64)
# --------------------------------------------------

all: $(TARGET)

$(TARGET): $(ASM_OBJ) $(C_OBJ) $(CPP_OBJ)
    $(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

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
    rm -f *.o $(TARGET)

.PHONY: all run clean
