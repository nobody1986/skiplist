cc=gcc
objs = skiplist.o main.o
cflags=-g -std=c11 
libs=-lm

skip : $(objs)
	gcc $(cflags) -o skip $(objs) $(libs)

main.o : main.c  skiplist.h
	 gcc $(cflags)  -c main.c

skiplist.o : skiplist.c skiplist.h
	gcc $(cflags)  -c skiplist.c $(libs)

clean :
	rm skip  $(objs)