# --- DEFINITIONS --- #

CC=gcc
.PHONY: run clean

# --- EXECUTIONS --- #

run: bin/main bin/bad bin/good
	./$< bin/good
	#./$< bin/bad


# --- COMPILATIONS --- #

bin/main: src/main.c bin/lib
	$(CC) -Wall -g -o $@ $^

bin/lib: src/lib.c
	$(CC) -Wall -c -o $@ $^


bin/%: src/%.c
	$(CC) -Wall -g -o $@ $<
bin/bad: src/bad.c
bin/good: src/good.c


# --- CLEANNING --- #

clean:
	rm -f bin/*.o bin/main bin/bad bin/good bin/lib