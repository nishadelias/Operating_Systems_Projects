# Hey! I'm Filing Here

In this lab, I successfully implemented a 1 MiB ext2 file system with 2 directories, 1 regular file, and 1 symbolic link.

## Building

Build the program by running the following command in this directory (the directory with the Makefile).
```shell
make
```

This should build the executable 'ext2-create'.

## Running

run the executable to create cs111-base.img:
```shell
./ext2-create
```

dump the filesystem information to help debug:
```shell
dumpe2fs cs111-base.img
```

check that your filesystem is correct:
```shell
fsck.ext2 cs111-base.img
```

create a directory to mnt your filesystem to:
```shell
mkdir mnt
```

mount your filesystem, loop lets you use a file :
```shell
sudo mount -o loop cs111-base.img mnt
```

## Cleaning up

unmount the filesystem when you're done:
```shell
sudo umount mnt
```

delete the directory used for mounting when you're done:
```shell
rmdir mnt
```

Clean up the executable and any other files that were generated during the build process by running the following command in this directory (the same directory where you ran "make").
```shell
make clean
```
