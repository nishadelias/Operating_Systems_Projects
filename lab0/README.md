# A Kernel Seedling
This module adds the file /proc/count, which contains the current number of running processes on the machine.

## Building
From the command line, run:
```shell
make
sudo insmod proc_count.ko
```
This builds the module, and loads it into the kernel.

## Running
```shell
cat /proc/count
```
This command prints out the current number of running processes on the machine.

## Cleaning Up
```shell
sudo rmmod proc_count
make clean
```
This removes the module from the kernel and cleans up the binaries.

## Testing
```python
python -m unittest
```
This command verifies that the module works correctly.

This module was tested on kernel version Linux 5.14.8-arch1-1.

```shell
uname -r -s -v
```
Rum this command to check the kernel release version.
