#!/bin/bash
fn=task4
gcc -std=c99 $fn.c coursework.c linkedlist.c -lpthread -o $fn
./$fn > $fn.1.txt
valgrind -v --log-file=$fn.2.mem.txt ./task4 > $fn.2.txt
valgrind -v --tool=helgrind --log-file=$fn.3.sync.txt ./task4 > $fn.3.txt