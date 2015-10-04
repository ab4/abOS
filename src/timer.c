// timer.c -- Initialises the PIT, and handles clock updates.
// Written for JamesM's kernel development tutorials.

#include "timer.h"
#include "isr.h"
#include "monitor.h"

u32int tick = 0;

static void timer_callback(registers_t regs)
{
   tick++;
   switch_task();
//monitor_write("switched task\n ");
//   monitor_write("Tick: ");
//   monitor_write_dec(tick);
//   monitor_write("\n");
}

void init_timer(u32int frequency)
{
   // Firstly, register our timer callback.
   register_interrupt_handler(IRQ0, &timer_callback);

   // The value we send to the PIT is the value to divide it's input clock
   // (1193180 Hz) by, to get our required frequency. Important to note is
   // that the divisor must be small enough to fit into 16-bits.
   u32int divisor = 1193180 / frequency;

   /*
    Send the command byte.
    * Bit 0: (BCP) Binary Counter
          o 0: Binary
          o 1: Binary Coded Decimal (BCD)
    * Bit 1-3: (M0, M1, M2) Operating Mode. See above sections for a description of each.
          o 000: Mode 0: Interrupt or Terminal Count
          o 001: Mode 1: Programmable one-shot
          o 010: Mode 2: Rate Generator
          o 011: Mode 3: Square Wave Generator
          o 100: Mode 4: Software Triggered Strobe
          o 101: Mode 5: Hardware Triggered Strobe
          o 110: Undefined; Don't use
          o 111: Undefined; Don't use
    * Bits 4-5: (RL0, RL1) Read/Load Mode. We are going to read or send data to a counter register
          o 00: Counter value is latched into an internal control register at the time of the I/O write operation.
          o 01: Read or Load Least Significant Byte (LSB) only
          o 10: Read or Load Most Significant Byte (MSB) only
          o 11: Read or Load LSB first then MSB
    * Bits 6-7: (SC0-SC1) Select Counter. See above sections for a description of each.
          o 00: Counter 0
          o 01: Counter 1
          o 10: Counter 2
          o 11: Illegal value
   */
   outb(0x43, 0x36); 

   // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
   u8int l = (u8int)(divisor & 0xFF);
   u8int h = (u8int)( (divisor>>8) & 0xFF );

   // Send the frequency divisor; bit4/5 set in control port -> lower first
   outb(0x40, l);
   outb(0x40, h);
} 
