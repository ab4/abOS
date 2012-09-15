#!/bin/bash

sudo /sbin/losetup /dev/loop0 floppy.img
sudo mount /dev/loop0 /mnt/myfloppy
sudo cp src/kernel /mnt/myfloppy/kernel
sudo cp initrd.img /mnt/myfloppy/initrd
sudo umount /dev/loop0
sudo /sbin/losetup -d /dev/loop0 
