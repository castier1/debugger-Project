CC=gcc
.PHONY: run clean

run: main bad
	./$^

%:%.c
	gcc -Wall -o $@ $^

main: main.c
bad: bad.c

clean:
	rm *.o main bad