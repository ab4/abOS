#ifndef FRAME_H
#define FRAME_H


// Macros used in the bitset algorithms.
#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

#include "common.h"

extern u32int placement_address; // defined in kheap.c

void alloc_frame(page_t *page, int is_kernel, int is_writeable);
void free_frame(page_t *page);


#endif
