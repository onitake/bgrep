.PHONY: all clean

CC=gcc
CFLAGS=-Wall -Os

all: bgrep

clean:
	rm -f bgrep

bgrep: bgrep.c
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^
