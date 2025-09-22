# Makefile for Linux 2048 Game
CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c99
TARGET = 2048
SRC = linux_2048.c

# Default target
all: $(TARGET)

# Build the game
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Debug build
debug: CFLAGS += -g -DDEBUG
debug: $(TARGET)

# Release build with optimizations
release: CFLAGS += -O3 -march=native
release: $(TARGET)

# Install to system (requires sudo)
install: $(TARGET)
	sudo cp $(TARGET) /usr/local/bin/
	sudo chmod +x /usr/local/bin/$(TARGET)
	@echo "2048 game installed to /usr/local/bin/"

# Clean build files
clean:
	rm -f $(TARGET) *.o

# Run the game
run: $(TARGET)
	./$(TARGET)

# Check for memory leaks
valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET)

.PHONY: all debug release install clean run valgrind