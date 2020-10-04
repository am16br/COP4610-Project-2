# Part 2

create proc fs timer

## Compilation and Running

### Compilation

```sh
sudo make
```

### Running

```sh
sudo insmod my_timer.ko
```

### Testing

```sh
cat /proc/timer
```

output example:

```
current time 1601837081.537226136
elapsed time 4.369879081
```

### Cleanup

```sh
sudo rmmod my_timer
```

