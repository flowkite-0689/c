# Makefile for Bank System
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude
TARGET = bank
SOURCES = src/main.c src/account.c src/auth.c src/ui.c
OBJECTS = $(SOURCES:.c=.o)

# 默认目标
all: $(TARGET)

# 链接生成可执行文件
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

# 编译源文件
src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# 修复文件名错误
fix_filename:
	mv src/account,c src/account.c

# 清理生成的文件
clean:
	del /Q $(OBJECTS) $(TARGET).exe 2>nul || echo "清理完成"

# 重新编译
rebuild: clean all

# 运行程序
run: $(TARGET)
	.\$(TARGET)

.PHONY: all clean rebuild run fix_filename
