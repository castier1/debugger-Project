# --- DEFINITIONS --- #

CC=gcc
CFLAGS= -g
TARGETS= bin/signal bin/meta bin/binary bin/process bin/syscall bin/breakpoint

.PHONY: analyzer clean

$(shell mkdir -p bin)

# --- EXAMPLES --- #

%: src/%.c
	$(CC) $(CFLAGS) -o bin/$@ $<

bad: src/bad.c analyzer
good: src/good.c analyzer


# --- COMPILATIONS --- #

analyzer: src/main.c lib
	$(CC) $(CFLAGS) -o $@ $< bin/lib $(TARGETS)

lib: src/lib.c signal meta binary process syscall breakpoint
	$(CC) $(CFLAGS) -c -o bin/$@ $< $(TARGETS)

signal: src/signal.c
	$(CC) $(CFLAGS) -c $< -o bin/$@

meta: src/metadata.c
	$(CC) $(CFLAGS) -c $< -o bin/$@

breakpoint: src/breakpoint.c binary process
	$(CC) $(CFLAGS) -c $< -o bin/$@ bin/binary bin/process

binary: src/binary.c
	$(CC) $(CFLAGS) -c $< -o bin/$@

process: src/process.c
	$(CC) $(CFLAGS) -c $< -o bin/$@

syscall: src/syscall.c
	$(CC) $(CFLAGS) -c $< -o bin/$@


# --- CLEANNING --- #

clean:
	rm -rf bin analyzer
