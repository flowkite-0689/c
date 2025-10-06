# C编译器设置
CC = gcc
CFLAGS = -Wall -Wextra -g -O0

# 目标文件
TARGET = 2048v2
SRC = day8hk/2048v2.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

debug: $(TARGET)
	gdb ./$(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean debug run