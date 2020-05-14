#! /bin/bash

echo "compling..."
rm -rf *.o
gcc -c *.c
gcc -o output *.o
rm -rf *.o
echo "done"
./output
rm -rf output