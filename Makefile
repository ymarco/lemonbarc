CFLAGS=-g
CC=gcc

objects=main.o lemonbarc.o

lemonbarc: main.o lemonbarc.o
	$(CC) $(CFLAGS) $(objects) -o lemonbarc

main.o: lemonbarc.h

lemonbarc.o: lemonbarc.h
