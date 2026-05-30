CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude -g
CLANG_FORMAT ?= clang-format
FORMAT_SRC = src/elomaxz.c include/elomaxz.h examples/counter/main.c

SRC = src/elomaxz.c
OBJ = $(SRC:.c=.o)
LIB = libelomaxz.a

all: $(LIB) bin/counter

$(LIB): $(OBJ)
	ar rcs $@ $^

src/%.o: src/%.c include/elomaxz.h
	$(CC) $(CFLAGS) -c $< -o $@

bin/counter: examples/counter/main.c $(LIB)
	mkdir -p bin
	$(CC) $(CFLAGS) -o $@ examples/counter/main.c $(LIB)

clean:
	rm -f $(OBJ) $(LIB) bin/counter examples/counter/*.o
	rm -rf bin/

run: bin/counter
	./bin/counter

format:
	$(CLANG_FORMAT) -i $(FORMAT_SRC)

format-check:
	$(CLANG_FORMAT) --dry-run --Werror $(FORMAT_SRC)

.PHONY: all clean run format format-check