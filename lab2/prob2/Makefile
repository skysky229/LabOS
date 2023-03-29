all: sum_multi-thread sum_serial

sum_multi-thread: sum_multi-thread.o
	gcc sum_multi-thread.o -o sum_multi-thread -lpthread

sum_multi-thread.o: sum_multi-thread.c
	gcc -c sum_multi-thread.c

sum_serial: sum_serial.o
	gcc sum_serial.o -o sum_serial -lpthread

sum_serial.o: sum_serial.c
	gcc -c sum_serial.c

clean: 
	rm -f *.o sum_multi-thread sum_serial