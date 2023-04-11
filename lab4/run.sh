#!/bin/bash
make clean
make all
echo "Input file:"
cat input.txt
echo "Results"
cat input.txt | ./sched
