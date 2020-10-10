# Part 3

## Kernel Compilation and Syscalls

### Applying the Patches

First download the correct kernel.

```bash
# go to $HOME (or wherever you want)
cd

# download the kernel; this may take a few minutes to half an hour
# depending on network connection
wget https://www.kernel.org/pub/linux/kernel/v4.x/linux-4.19.98.tar.xz

# then, extract it to /usr/src/
tar -xvf linux-4.19.98.tar.xz -C /usr/src
```

You may want to make a backup of the kernel and create a link in $HOME

```bash
# copy /usr/src/linux-4.19.98/ to /usr/src/test_kernel
sudo cp -r /usr/src/linux-4.19.98/ /usr/src/test_kernel

# link test_kernel so it's in $HOME
ln -s /usr/src/test_kernel .
```

Now we need to apply the patches

```bash
cd test_kernel

# apply patches
sudo patch -p1 < ~/kernel.diff
```

### Build Tools, Compilation, Installation

Install build tools. Do this before running any `make`.

```bash
sudo apt-get install build-essential libncurses-dev bison flex libssl-dev libelf-dev
```

Now do this once before running any of the other makes

```bash
# run this, move to <Save>, press Return; don't change anything (I guess)
sudo menuconfig

# build kernel (-j flag speeds up a lot for n cores);
# this takes a very LONG time
sudo make -j $(nproc)

# install kernel and boot files
sudo make modules_install install
```

Reboot (pray it works). Confirm that `uname -r` reports `4.19.98`.

### Patch Creation

Creating the patch is straight forward.

```bash
# diff -Naur OLD_DIR NEW_DIR; redirect to wherever you want
sudo diff -Naur /usr/src/linux-4.19.98/ test_kernel/ > kernel.diff
```

Apply it like this:

```bash
# go into the directory
cd test_kernel

# apply the patch; be sure to specify the correct diff file
patch -p1 < ~/kernel.diff
```

