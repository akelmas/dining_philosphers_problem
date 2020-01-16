CC=gcc
CFLAGS=-I.
phsp: phsp.o 
	$(CC) -o phsp phsp.o
