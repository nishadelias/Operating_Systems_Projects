# You Spin Me Round Robin

This project simulates a round robin scheduling algorithm for processes with different arrival times and different burst times. For a given quantum size, the average waiting time and average response time are outputted.

## Building

Build the program by running the following command in the directory with rr.c and Makefile
```shell
make
```

## Running

Use the following command to run the program
```shell
./rr processes.txt 3
```

Here are the results for the above command
```shell
Average waiting time: 7.00
Average response time: 2.75
```

In the example above, the quantum size is 3. The first line of processes.txt contains the number of processes. Every subsequent line contains "process#, arrival-time, burst-time". 

## Cleaning up

Clean up the binary files by running the following command in the same directory where you ran "make". 
```shell
make clean
```
