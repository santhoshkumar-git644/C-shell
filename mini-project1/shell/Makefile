CC=gcc
CFLAGS=-std=c99 -D_POSIX_C_SOURCE=200809L -D_XOPEN_SOURCE=700 -Wall -Wextra -Werror -Wno-unused-parameter -fno-asm -Iinclude

all: shell.out

shell.out: src/main.c src/prompt.c src/ast.c src/parser.c src/exec.c src/hop.c src/reveal.c src/log.c src/pipe.c src/redir.c src/jobs.c src/signals.c
	$(CC) $(CFLAGS) src/main.c src/prompt.c src/ast.c src/parser.c src/exec.c src/hop.c src/reveal.c src/log.c src/pipe.c src/redir.c src/jobs.c src/signals.c -o shell.out

clean:
	rm -f shell.out
