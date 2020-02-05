#!/bin/bash
fn=task5
gcc -std=c99 $fn.c coursework.c linkedlist.c -lpthread -o $fn
./$fn > $fn.1.txt
valgrind -v --log-file=$fn.2.mem.txt ./$fn > $fn.2.txt
valgrind -v --tool=helgrind --log-file=$fn.3.sync.txt ./$fn > $fn.3.txt