CC=gcc
SRC = src/yaqp.c lib/PRS-decomp.c
CFLAGS = -Wextra -Wall -Wundef -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow=5 -Wwrite-strings -Waggregate-return -Wcast-qual -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code -march=native -std=c99 -Ilib/include

main: $(SRC)
	$(CC) -o bin/yaqp $(SRC) $(CFLAGS)
