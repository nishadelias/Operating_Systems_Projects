# A Kernel Seedling
This module adds the file /proc/count, which contains the current number of running processes on the machine.

## Building
```shell
Run the command "make" from the command line to build the module.
Then run the command "sudo insmod proc_count.ko" to load the module into the kernel.
```

## Running
```shell
Run the command "cat /proc/count", which prints out the current number of running processes on the machine.
```
TODO: results?

## Cleaning Up
```shell
Run the command "sudo rmmod proc_count" in order to remove the module.
```

## Testing
```python
python -m unittest
```
TODO: results?

Report which kernel release version you tested your module on
(hint: use `uname`, check for options with `man uname`).
It should match release numbers as seen on https://www.kernel.org/.

```shell
uname -r -s -v
```
TODO: kernel ver?
