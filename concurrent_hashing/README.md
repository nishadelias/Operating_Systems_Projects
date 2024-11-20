# Hash Hash Hash
In this project, we implement a hash table in which we can safely add values to the hash table concurrently (in a multithreaded environment). We add mutexes to a base (serial) implementation in order to guarantee thread safety.

## Building
Build the program by running the following command in this directory (the directory with the Makefile).
```shell
make
```
This should build the executable 'hash-table-tester'.

## Running
Run the program with the following command.
```shell
./hash-table-tester -t n -s m
```
n is the number of threads to use (default is 4) and m is the number of hash table entries to add per thread (default is 25,000)

Below is an example with only the Hash Table Base shown (serial implementation). The time is in microseconds.
```shell
$ ./hash-table-tester -t 8 -s 50000
Generation: 72,388 usec
Hash table base: 923,818 usec
  - 0 missing
```

## First Implementation
In `hash-table_v1.c`, I added a mutex to the 'hash_table_v1' struct to protect the entire hash table. 

I intialized this mutex in the 'hash_table_v1_create' function with pthread_mutex_init. 

In the 'hash_table_v1_add_entry', the critical section is adding values to the hash table. I locked the mutex before the critical section and unlocked it after the critical section. I also checked the success of locking and unlocking the mutex and handled any errors. By locking the entire hash table with a single mutex before adding values, we can guarantee correctness because two threads cannot add values to the hash table at the same time. 

Lastly, I destroyed the mutex in the 'hash_table_v1_destroy' function using pthread_mutex_destroy.

### Performance
```shell
$ ./hash-table-tester -t 8 -s 50000
Generation: 72,388 usec
Hash table base: 923,818 usec
  - 0 missing
Hash table v1: 1,436,114 usec
  - 0 missing
```
Version 1 is a slower than the base version because there is added overhead from creating the mutex, locking, and unlocking the critical section. In addition, there is no parallelism in adding entries to the hash table because we lock the entire hash table every time we need to add an entry.

## Second Implementation
In `hash-table_v2.c`, I added a mutex to each 'hash_table_entry' struct so that each bucket in the hash table has its own lock. This allows multiple threads to work on different parts of the hash table concurrently without causing contention. 

I initialized these mutexes in the 'hash_table_v2_create' function with pthread_mutex_init. 

While the critical section is still adding entries to the hash table, we now have locks for each entry rather than one lock for the whole hash table. In the 'hash_table_v2_add_entry' function, I locked the mutex for the specific entry (bucket) before the critical section and unlocked it after the critical section. I checked the success of locking and unlocking the mutex and handled any errors. 

Lastly, I destroyed the mutexes in the 'hash_table_v2_destroy' function using pthread_mutex_destroy.

### Performance
```shell
$ ./hash-table-tester -t 8 -s 50000
Generation: 72,388 usec
Hash table base: 923,818 usec
  - 0 missing
Hash table v1: 1,436,114 usec
  - 0 missing
Hash table v2: 296,911 usec
  - 0 missing
```
The speedup time of Version 2 compared to the base table is approximately the number of threads we are using. We exploit as much parallelism as possible in adding entries to the hash table. The only time entries are not added in parallel is when two or more threads try to add values to the same entry. There is also added overhead from creating the mutexes, locking, and unlocking the critical section.

## Cleaning up
Clean up the executable and any other files that were generated during the build process by running the following command in this directory (the same directory where you ran "make").
```shell
make clean
```
