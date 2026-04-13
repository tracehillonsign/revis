CC = gcc

CFLAGS = -Wall -Iinclude

TARGET = build/revis

SRCS = $(wildcard src/*.c)

OBJS = $(patsubst src/%.c, build/%.o, $(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^

build/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

prepare:
	mkdir -p build

.PHONY: all clean prepare