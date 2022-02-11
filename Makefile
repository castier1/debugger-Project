CC=gcc
.PHONY: run clean

run: bin/main bin/bad
	./$^

bin/%: src/%.c
	gcc -Wall -o $@ $^

bin/main: src/main.c
bin/bad: src/bad.c

clean:
	rm -f bin/*.o bin/main bin/bad