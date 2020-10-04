# Part 1

make an executable that calls 7 more system calls than an empty C file

## Compilation and Testing

Make sure that `empty.c` and `part1.c` are created.

```sh
$ cd Part1
$ make
$ ./test.sh
Make and straces succeeded; Results:
Perfect; there are 7 additional lines
```

This will inform you about any errors in the make file and if the correct number
of additional system calls is called by the second program with nice colors.
