make clean
make all
time ./aggsum_multi-thread 100000000 10 3
time ./aggsum_single-thread 100000000 3 
