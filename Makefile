# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -g -D_POSIX_C_SOURCE=200809L

# Executable name
TARGET = hangMan

# Source files
SRCS = hangMan.c file.c list.c hash.c game.c

# Object files (automatically generated from source files)
OBJS = $(SRCS:.c=.o)

# Header files
HEADERS = file.h list.h hash.h game.h

# Default target
all: $(TARGET)

# Create the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile source files to object files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Test targets
test-list: list.c
	$(CC) $(CFLAGS) -DTEST_LIST -o test_list list.c
	./test_list

test-file: file.c list.c
	$(CC) $(CFLAGS) -DTEST_FILE -o test_file file.c list.c
	./test_file length4.txt

test-hash: hash.c list.c file.c
	$(CC) $(CFLAGS) -DTEST_HASH -o test_hash hash.c list.c file.c
	./test_hash length4.txt

# Run with valgrind for memory leak detection
valgrind: $(TARGET)
	valgrind --tool=memcheck --leak-check=full --show-reachable=yes --track-origins=yes ./$(TARGET) length4.txt

# Clean build files
clean:
	rm -f $(TARGET) $(OBJS) test_list test_file test_hash

# Install (if needed for system-wide installation)
install: $(TARGET)
	cp $(TARGET) /usr/local/bin/

# Phony targets (not actual files)
.PHONY: all clean test-list test-file test-hash valgrind install