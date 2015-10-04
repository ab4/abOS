// main.c -- Defines the C-code kernel entry point, calls initialisation routines.
// Made for JamesM's tutorials

#include "monitor.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "paging.h"
#include "multiboot.h"
#include "fs.h"
#include "initrd.h"
#include "task.h"
#include "syscall.h"

extern u32int placement_address; // in kheap.c
extern fs_node_t *fs_root; // in fs.c

u32int initial_esp; // initial stack pointer

int main(struct multiboot *mboot_ptr, u32int initial_stack)
{
  initial_esp = initial_stack; 

  // initialise 
  init_descriptor_tables();  // Initialise all the ISRs and segmentation
  monitor_clear();           // Initialise the screen (by clearing it)

  // Initialise the PIT to 100Hz
  asm volatile("sti");
//eeiabat  init_timer(50);

  // Screen test
  //monitor_write("Hello, world!\n"); 

  // Find the location of our initial ramdisk.
  ASSERT(mboot_ptr->mods_count > 0);
  u32int initrd_location = *((u32int*)mboot_ptr->mods_addr);
  u32int initrd_end = *(u32int*)(mboot_ptr->mods_addr+4);
  // Don't trample our module with placement accesses, please!
  placement_address = initrd_end;

  // GDT/IDT test
  //asm volatile ("int $0x3");
  //asm volatile ("int $0x20"); 

  // IRQ/PIT test
  //asm volatile("sti");
  //init_timer(50); // Initialise timer to 50Hz 
  
  // Paging test
  //u32int *ptr = (u32int*)0xA0000000;
  //u32int do_page_fault = *ptr;
 
  // Heap test
  //u32int a = kmalloc(8);
  initialise_paging();     // Start paging.
/*
  u32int b = kmalloc(8);
  u32int c = kmalloc(8);
  monitor_write("a: ");
  monitor_write_hex(a);
  monitor_write(", b: ");
  monitor_write_hex(b);
  monitor_write("\nc: ");
  monitor_write_hex(c);

  kfree(c);
  kfree(b);
  u32int d = kmalloc(12);
  monitor_write(", d: ");
  monitor_write_hex(d);  
  */

  initialise_tasking();    // Start multitasking.

  // Initialise the initial ramdisk, and set it as the filesystem root.
  fs_root = (fs_node_t *) initialise_initrd(initrd_location);

  // usermode test
  initialise_syscalls();
  switch_to_user_mode();
  syscall_monitor_write("Hello, user world!\n");
//monitor_write("Hello, user world!\n");

#ifdef MULTI_TASK
  // Create a new process in a new address space which is a clone of this.
  int ret = fork(0x123);
  monitor_write("fork() returned ");
  monitor_write_hex(ret);
  monitor_write(", and getpid() returned ");
  monitor_write_hex(getpid());
  monitor_write("\n============================================================================\n");

  // The next section of code is not reentrant so make sure we aren't interrupted during.
  asm volatile("cli");
  // filesystem test: list the contents of /
  int i = 0;
  struct dirent *node = 0;

  while ( (node = readdir_fs(fs_root, i)) != 0)
  {
    monitor_write("Found file ");
    monitor_write(node->name);
    fs_node_t *fsnode = finddir_fs(fs_root, node->name);


    if ((fsnode->flags&0x7) == FS_DIRECTORY)
    {
      monitor_write("\n\t(directory)\n");
    }
    else
    {
      monitor_write("\n\t contents: \"");
      char buf[256];
      u32int sz = read_fs(fsnode, 0, 256, buf);
      int j;
      for (j = 0; j < sz; j++)
        monitor_put(buf[j]);
            
      monitor_write("\"\n");
    }

    i++;
  }
  monitor_write("\n");

  asm volatile("sti");

  switch_task();
#endif

  // Genesis test
  return 0xDEADBABA;
} 
