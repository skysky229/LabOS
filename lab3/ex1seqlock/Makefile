all: seqlock

seqlock: seqlock.o
	gcc seqlock.o -o seqlock -lpthread

seqlock.o: seqlock.c seqlock.h
	gcc -c seqlock.c

clean:
	rm -rf *.o seqlock