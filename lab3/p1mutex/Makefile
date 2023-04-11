all: shrdmem

shrdmem: shrdmem.o
	gcc shrdmem.o -o shrdmem -lpthread

shrdmem.o: shrdmem.c
	gcc -c shrdmem.c

clean:
	rm -rf *.o shrdmem