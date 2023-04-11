all: logbuf

logbuf: logbuf.o
	gcc logbuf.o -o logbuf -lpthread

logbuf.o: logbuf.c
	gcc -c logbuf.c

clean: 
	rm -rf *.o logbuf