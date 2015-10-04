#ifndef PAGING_H
#define PAGING_H

#include "common.h"
#include "isr.h"

// Macros used with the bitset
#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

extern u32int placement_address; // defined in kheap.c

typedef struct page
{
   u32int present    : 1;   // Page present in memory
   u32int rw         : 1;   // Read-only if clear, readwrite if set
   u32int user       : 1;   // Supervisor level only if clear
   u32int accessed   : 1;   // Has the page been accessed since last refresh?
   u32int dirty      : 1;   // Has the page been written to since last refresh?
   u32int unused     : 7;   // Amalgamation of unused and reserved bits
   u32int frame      : 20;  // Frame address (shifted right 12 bits)
} page_t;

typedef struct page_table
{
   page_t pages[1024];
} page_table_t;

typedef struct page_directory
{
   page_table_t *tables[1024]; //pointers to pagetables

   // physical location of tables's pointers, for loading into CR3 register 
   u32int tablesPhysical[1024]; 

   //   The physical address of tablesPhysical. This comes into play
   //   when we get our kernel heap allocated and the directory
   //   may be in a different location in virtual memory.
   u32int physicalAddr;
} page_directory_t;

/* frame functions */
void alloc_frame(page_t *page, int is_kernel, int is_writeable);
void free_frame(page_t *page);

/*  Paging functions  */
void initialise_paging();
void switch_page_directory(page_directory_t *new);
//  If make == 1 and the page-table in which this page should reside isn't there, create it!
page_t *get_page(u32int address, int make, page_directory_t *dir);
void page_fault(registers_t regs); // Handler for page faults.

page_directory_t *clone_directory(page_directory_t *src);

#endif
