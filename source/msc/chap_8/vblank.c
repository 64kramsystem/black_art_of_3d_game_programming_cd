
// VBLANK.C - demo of the vertical blank interrupt supported by a few VGA cards

// I N C L U D E S ///////////////////////////////////////////////////////////

#include <io.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <bios.h>
#include <fcntl.h>
#include <memory.h>
#include <malloc.h>
#include <math.h>
#include <string.h>

#include "black3.h"
#include "black8.h"

// D E F I N E S /////////////////////////////////////////////////////////////

#define VERTICAL_BLANK_INTERRUPT    0x0A   // index of vblank in vector table
#define CRT_VERTICAL_END            0x11   // register with vblank enable bits

// G L O B A L S  ////////////////////////////////////////////////////////////

void (_interrupt far *Old_Vertical)();

// F U N C T I O N S /////////////////////////////////////////////////////////

void _interrupt _far Vertical_Blank(void)
{
// this function is the vertical blank handler

static int count = 0; // used to count the vertical blanks
char buffer[64];      // used to print a string
unsigned char data;   // used to read data

_asm sti ; // re-enable interrupts

// reset latch on VGA

_outp(CRT_CONTROLLER,CRT_VERTICAL_END);

data = _inp(CRT_CONTROLLER+1);

data = RESET_BITS(data,0x10);

_outp(CRT_CONTROLLER+1,data);

// do whatever you want here, but make it quick!

sprintf(buffer,"Number of vertical blanks is %d",count++);
Print_String(0,0,10,buffer,1);

// end process vertical blank

// re-enable interrupts on PIC, send end of interrupt command EOI, 20h

_outp(PIC_ICR, PIC_EOI);

} // end Vertical_Blank

// M A I N ///////////////////////////////////////////////////////////////////

void main(void)
{
unsigned char data;

// set the graphics mode to mode 13h

Set_Graphics_Mode(GRAPHICS_MODE13);

// save old vertical blank interrupt

Old_Vertical = _dos_getvect(VERTICAL_BLANK_INTERRUPT);

// set new interrupt

_dos_setvect(VERTICAL_BLANK_INTERRUPT,Vertical_Blank);

// enable interrupt on VGA

_outp(CRT_CONTROLLER,CRT_VERTICAL_END);
data = (unsigned char)_inp(CRT_CONTROLLER+1);

data = RESET_BITS(data,0x20);   // for your VGA you may need to SET this bit
_outp(CRT_CONTROLLER+1,data);

data = RESET_BITS(data,0x10);
_outp(CRT_CONTROLLER+1,data);

// enable interrupt on PIC, i.e. enable IRQ 2

data = (unsigned char)_inp(PIC_IMR);
data = RESET_BITS(data,0x40);
_outp(PIC_IMR,data);

while(!kbhit());

// disable interrupts from VGA

_outp(CRT_CONTROLLER,CRT_VERTICAL_END);
data =(unsigned char)_inp(CRT_CONTROLLER+1);

data = SET_BITS(data,0x20);    // for your VGA you may need to reset this bit
_outp(CRT_CONTROLLER+1,data);

// disable vertical blank interrupt on PIC

data =(unsigned char)_inp(PIC_IMR);
data = SET_BITS(data,0x04);
_outp(PIC_IMR,data);

// restore old vector

_dos_setvect(VERTICAL_BLANK_INTERRUPT,Old_Vertical);

Set_Graphics_Mode(TEXT_MODE);

} // end main

