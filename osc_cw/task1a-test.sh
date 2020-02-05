#!/bin/bash
fn=task1a
gcc -std=c99 task1a.c coursework.c linkedlist.c -o $fn
./task1a > task1a.txt
cat task1a.txt
rm task1a.txt
rm task1a
