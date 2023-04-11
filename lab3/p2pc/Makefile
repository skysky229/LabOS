all: pc

pc: pc.o
	gcc pc.o -o pc -lpthread

pc.o: pc.c
	gcc -c pc.c 

clean:
	rm -rf *.o pc