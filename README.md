1. Project description:

(1) This is a Test program for MINI2440 board

(2) This program include boot code and some devices' driver, such as DM9000, Nand, Nor


2. Build

make 

make clean

Note: You should modify below line which in the Makefie in the top dir

LFLAGS := -Tgboot.lds -L /usr/local/arm/4.3.2/lib/gcc/arm-none-linux-gnueabi/4.3.2/armv4t -lgcc
