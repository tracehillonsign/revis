CC = gcc
CFLAGS = -Wall -Wextra -Werror -Iinclude
LDFLAGS = -lssl -lcrypto -lz

COMMON_SRCS = $(filter-out src/main.c src/debug.c, $(wildcard src/*.c))
COMMON_OBJS = $(patsubst src/%.c, build/%.o, $(COMMON_SRCS))

all: prepare build/revis

debug: prepare build/rdebug

build/revis: $(COMMON_OBJS) build/main.o
	$(CC) -o $@ $^ $(LDFLAGS)

build/rdebug: $(COMMON_OBJS) build/debug.o
	$(CC) -o $@ $^ $(LDFLAGS)

build/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f build/*.o build/revis build/rdebug

prepare:
	mkdir -p build

.PHONY: all clean prepare debug