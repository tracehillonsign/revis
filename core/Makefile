CC = gcc

CFLAGS = -Wall -Wextra -Werror -Iinclude

LDFLAGS = -lssl -lcrypto -lz

TARGET = build/revis

SRCS = $(wildcard src/*.c)

OBJS = $(patsubst src/%.c, build/%.o, $(SRCS))

all: prepare $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

build/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

prepare:
	mkdir -p build

.PHONY: all clean prepare