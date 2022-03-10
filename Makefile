# --- DEFINITIONS --- #

CC=gcc
CFLAGS= -Wall -g
.PHONY: analyzer clean

# --- EXECUTIONS --- #

test: analyzer bad good
	./$< bin/good
	./$< bin/bad


# --- COMPILATIONS --- #

analyzer: src/main.c bin/lib
	$(CC)  -o $@ $^

bin/lib: src/lib.c
	$(CC) $(CFLAGS) -c -o $@ $^


%: src/%.c
	$(CC) $(CFLAGS) -o bin/$@ $<
bad: src/bad.c
good: src/good.c


# --- CLEANNING --- #

clean:
	rm -f bin/*.o bin/bad bin/good bin/lib analyzer
