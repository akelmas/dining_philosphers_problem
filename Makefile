CC=gcc
CFLAGS=-I.

phspmake: src/phsp.c 
	$(CC) src/phsp.c -pthread -lm -o build/test

