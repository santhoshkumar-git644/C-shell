CC=gcc
CFLAGS=-std=c99 -D_POSIX_C_SOURCE=200809L -D_XOPEN_SOURCE=700 -Wall -Wextra -Werror -Wno-unused-parameter -fno-asm -Iinclude

all: shell.out

shell.out: src/main.c src/prompt.c src/ast.c src/parser.c src/exec.c
	$(CC) $(CFLAGS) src/main.c src/prompt.c src/ast.c src/parser.c src/exec.c -o shell.out

clean:
	rm -f shell.out
