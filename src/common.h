// common.h -- Defines typedefs and some global functions.
// From JamesM's kernel development tutorials.

#ifndef COMMON_H
#define COMMON_H

#define PANIC(msg) panic(msg, __FILE__, __LINE__);
#define ASSERT(b) ((b) ? (void)0 : panic_assert(__FILE__, __LINE__, #b))

// Some nice typedefs, to standardise sizes across platforms.
// These typedefs are written for 32-bit X86.
typedef unsigned int   u32int;
typedef          int   s32int;
typedef unsigned short u16int;
typedef          short s16int;
typedef unsigned char  u8int;
typedef          char  s8int;

typedef u32int size_t;

// some C wrappers for sending data on ports
void outb(u16int port, u8int value);
u8int inb(u16int port);
u16int inw(u16int port);

// memory handlers
//void memset(void * buffer, int ch, size_t count);

/* TODO
memcpy
strlen
strcmp 
*/

extern void panic(char *message, char *file, u32int line);
extern void panic_assert(char *file, u32int line, char *desc);



#endif
