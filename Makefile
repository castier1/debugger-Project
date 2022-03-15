# --- DEFINITIONS --- #

CC=gcc
CFLAGS= -g
.PHONY: analyzer clean

$(shell mkdir -p bin)

# --- EXECUTIONS --- #

run: bad good
	./analyzer bin/good
	./analyzer bin/bad


# --- EXAMPLES --- #

%: src/%.c analyzer
	$(CC) $(CFLAGS) -o bin/$@ $<

bad: src/bad.c
good: src/good.c


# --- COMPILATIONS --- #

analyzer: src/main.c bin/lib
	$(CC) $(CFLAGS) -o $@ $^

bin/lib: src/lib.c
	$(CC) $(CFLAGS) -c -o $@ $^


# --- CLEANNING --- #

clean:
	rm -rf bin analyzer
