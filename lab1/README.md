## UID: 305934503

## Pipe Up

This program replicates the pipe (|) operator in shells by executing a series of Unix commands in a pipeline, passing the output of each command as input to the next.

## Building

Build the program by running the following command in the directory with pipe.c and Makefile.
``` shell
make
```

## Running

Here are two example runs of the program. The expected output is shown using commands with the original pipe (|) operator, while this programs outputs are shown using commands with ./pipe.
``` shell
cs111@cs111 CS111/lab1 (main *%) » ls | cat | wc   
      6       6      51
cs111@cs111 CS111/lab1 (main *%) » ./pipe ls cat wc
      6       6      51
cs111@cs111 CS111/lab1 (main *%) » ls | sort
Makefile
pipe
pipe.c
pipe.o
README.md
test_lab1.py
cs111@cs111 CS111/lab1 (main *%) » ./pipe ls sort  
Makefile
pipe
pipe.c
pipe.o
README.md
test_lab1.py
```

## Cleaning up

Clean up the binary files by running the following command in the same directory where you ran "make".
``` shell
make clean
```
