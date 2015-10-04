# abOS
hobby real time operating system

at the moment it's equivalent to about 3/4 of an OS class project 
and no RT stuff has been implemented yet.  

To Run
------
"go.sh" calls
  "update_image.sh" which 
    mounts a floppy image, 
    compiles files in /src into the "kernel" exe, via Makefile in /src, 
    adds kernel to the floppy,
    adds initrd.img to the floppy (see note below)
    unmounts the floppy
  "run_bochs.sh" which
    mounts floppy
    runs bochs (configured via bochsrc.txt)
    unmounts floppy

Note: initrd.img is generated seperately by calling make_initrd in this directory
