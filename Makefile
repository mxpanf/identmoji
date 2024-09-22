# Makefile for building the identmoji application

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -O2
LIBS = -lssl -lcrypto

# Target executable
TARGET = identmoji

# Source file
SRC = identmoji.c

# Default installation directory
PREFIX = /usr/local

# Build target
all: $(TARGET)

# Rule for compiling the program
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LIBS)

# Install the binary to the system's PATH
install: $(TARGET)
	install -m 0755 $(TARGET) $(PREFIX)/bin

# Clean target to remove binary
clean:
	rm -f $(TARGET)

.PHONY: all clean install
