
// BLACK8.H - Header file for BLACK8.C

// D E F I N E S //////////////////////////////////////////////////////////////

// the 8259A programmable interrupt controller

#define PIC_ICR  0x20 // the interrupt control register
#define PIC_IMR  0x21 // the interrupt mask register
#define PIC_EOI  0x20 // end of interrupt command

// the 8253 timer chip

#define TIMER_CONTROL  0x43  // the 8253's control register
#define TIMER_SET_BITS 0x3C  // the bit pattern that will place the timer into
                             // binary counting with counter load sequence of
                             // low byte then high byte

#define TIMER_COUNTER_0       0x40  // counter 0
#define TIMER_COUNTER_1       0x41  // counter 1
#define TIMER_COUNTER_2       0x42  // counter 2

#define TIMER_120HZ    0x26D7 // 120 hz
#define TIMER_100HZ    0x2E9B // 100 hz
#define TIMER_60HZ     0x4DAE // 60 hz
#define TIMER_50HZ     0x5D37 // 50 hz
#define TIMER_40HZ     0x7486 // 40 hz
#define TIMER_30HZ     0x965C // 30 hz
#define TIMER_20HZ     0xE90B // 20 hz
#define TIMER_18HZ     0xFFFF // 18.2 hz (slowest possible)

#define TIME_KEEPER_INT     0x1C    // the time keeper interrupt

// M A C R O S ///////////////////////////////////////////////////////////////

#define LOW_BYTE(n) (n & 0x00FF)       // extracts the low-byte of a word
#define HI_BYTE(n)  ((n>>8) & 0x00FF)  // extracts the hi-byte of a word

// E X T E R N A L S//////////////////////////////////////////////////////////

extern long starting_time, // these are used to compute the length of some event
            ending_time;

// P R O T O T Y P E S ///////////////////////////////////////////////////////

long Timer_Query(void);

void Timer_Program(int timer,unsigned int rate);

