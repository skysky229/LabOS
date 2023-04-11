all: dinPhil

dinPhil: dinPhil.o
	gcc dinPhil.o -o dinPhil -lpthread

dinPhil.o: dinPhil.c
	gcc -c dinPhil.c

clean:
	rm -rf *.o dinPhil