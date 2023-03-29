all: aggsum_multi-thread aggsum_single-thread

aggsum_multi-thread: aggsum_multi-thread.o
	gcc aggsum_multi-thread.o -o aggsum_multi-thread -lpthread
	
aggsum_single-thread: aggsum_single-thread.o
		gcc aggsum_single-thread.o -o aggsum_single-thread -lpthread

aggsum_multi-thread.o: aggsum_multi-thread.c
	gcc -c aggsum_multi-thread.c
	
aggsum_single-thread.o: aggsum_single-thread.c
	gcc -c aggsum_single-thread.c

clean: 
	rm -f *.o aggsum_multi-thread aggsum_single-thread
