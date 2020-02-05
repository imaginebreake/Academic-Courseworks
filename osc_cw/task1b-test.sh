#!/bin/bash
fn=task1b
gcc -std=c99 task1b.c coursework.c linkedlist.c -o $fn
./task1b > task1b.txt
cat task1b.txt
rm task1b
rm task1b.txt
