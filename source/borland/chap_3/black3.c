
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

#include "black3.h"   // the header file for this module

// G L O B A L S //////////////////////////////////////////////////////////////

unsigned char far *video_buffer = (unsigned char far *)0xA0000000L;
unsigned char far *rom_char_set = (unsigned char far *)0xF000FA6EL;

// F U N C T I O N S /////////////////////////////////////////////////////////

void Print_Char(int xc,int yc,char c,int color,int transparent)
{
// this function is used to print a character on the screen. It uses the
// internal 8x8 character set to do this. Note that each character is
// 8 bytes where each byte represents the 8 pixels that make up the row
// of pixels

int offset,               // offset into video memory
         x,               // loop variable
         y;               // loop variable

unsigned char far *work_char; // pointer to character being printed

unsigned char bit_mask;       // bitmask used to extract proper bit

// compute starting offset in rom character lookup table
// multiple the character by 8 and add the result to the starting address
// of the ROM character set

work_char = rom_char_set + c * ROM_CHAR_HEIGHT;

// compute offset of character in video buffer, use shifting to multiply

offset = (yc << 8) + (yc << 6) + xc;

// draw the character row by row

for (y=0; y<ROM_CHAR_HEIGHT; y++)
    {
    // reset bit mask

    bit_mask = 0x80;

	 // draw each pixel of this row

    for (x=0; x<ROM_CHAR_WIDTH; x++)
        {
        // test for transparent pixel i.e. 0, if not transparent then draw

        if ((*work_char & bit_mask))
             video_buffer[offset+x] = (unsigned char)color;

        else
        if (!transparent)               // takes care of transparency
            video_buffer[offset+x] = 0; // make black part opaque

        // shift bit mask

		  bit_mask = (bit_mask>>1);

        } // end for x

    // move to next line in video buffer and in rom character data area

    offset      += MODE13_WIDTH;
    work_char++;

    } // end for y

} // end Print_Char

//////////////////////////////////////////////////////////////////////////////

void Print_String(int x,int y,int color, char *string,int transparent)
{
// this function prints an entire string on the screen with fixed spacing
// between each character by calling the Print_Char() function

 int index,   // loop index
     length;  // length of string

// compute length of string

length = strlen(string);

// print the string a character at a time

for (index=0; index<length; index++)
	  Print_Char(x+(index<<3),y,string[index],color,transparent);

} // end Print_String

///////////////////////////////////////////////////////////////////////////////

void Write_Pixel(int x,int y,int color)
{

// plots the pixel in the desired color a little quicker using binary shifting
// to accomplish the multiplications

// use the fact that 320*y = 256*y + 64*y = y<<8 + y<<6

video_buffer[((y<<8) + (y<<6)) + x] = (unsigned char )color;

} // end Write_Pixel

///////////////////////////////////////////////////////////////////////////////

int Read_Pixel(int x,int y)
{
// this function read a pixel from the screen buffer


// use the fact that 320*y = 256*y + 64*y = y<<8 + y<<6

return((int)(video_buffer[((y<<8) + (y<<6)) + x]));

} // end Read_Pixel

//////////////////////////////////////////////////////////////////////////////

void Set_Graphics_Mode(int mode)
{

// use the video interrupt 10h and the C interrupt function to set
// the video mode

union REGS inregs,outregs;

inregs.h.ah = 0;                    // set video mode sub-function
inregs.h.al = (unsigned char)mode;  // video mode to change to

int86(0x10, &inregs, &outregs);

} // end Set_Graphics_Mode

/////////////////////////////////////////////////////////////////////////////

void Time_Delay(int clicks)
{
// this function uses the internal timer to wait a specified number of "clicks"
// the actual amount of real time is the number of clicks * (time per click)
// usually the time per click is set to 1/18th of a second or 55ms

long far *clock = (long far *)0x0000046CL; // address of timer

long start_time; // starting time

// get current time

start_time = *clock;

// when the current time minus the starting time >= the requested delay then
// the function can exit

while(labs(*clock - start_time) < (long)clicks){}

} // end Time_Delay

////////////////////////////////////////////////////////////////////////////////

void Line_H(int x1,int x2,int y,int color)
{
// draw a horizontal line using the memset function
// this function does not do clipping hence x1,x2 and y must all be within
// the bounds of the screen

int temp; // used for temporary storage during endpoint swap

// sort x1 and x2, so that x2 > x1

if (x1>x2)
   {
   temp = x1;
   x1   = x2;
   x2   = temp;
   } // end swap

// draw the row of pixels

_fmemset((char far *)(video_buffer + ((y<<8) + (y<<6)) + x1),
         (unsigned char)color,x2-x1+1);

} // end Line_H

//////////////////////////////////////////////////////////////////////////////

void Line_V(int y1,int y2,int x,int color)
{
// draw a vertical line, note that a memset function can no longer be
// used since the pixel addresses are no longer contiguous in memory
// note that the end points of the line must be on the screen

unsigned char far *start_offset; // starting memory offset of line

int index, // loop index
    temp;  // used for temporary storage during swap

// make sure y2 > y1

if (y1>y2)
   {
   temp = y1;
   y1   = y2;
   y2   = temp;
   } // end swap

// compute starting position

start_offset = video_buffer + ((y1<<8) + (y1<<6)) + x;

for (index=0; index<=y2-y1; index++)
    {
    // set the pixel

    *start_offset = (unsigned char)color;

    // move downward to next line

    start_offset+=320;

    } // end for index

} // end Line_V

//////////////////////////////////////////////////////////////////////////////

void Write_Color_Reg(int index, RGB_color_ptr color)
{

// this function is used to write a color register with the RGB value
// within "color"

// tell vga card which color register to update

outp(COLOR_REGISTER_WR, index);

// update the color register RGB triple, note the same port is used each time
// the hardware will make sure each of the components is stored in the
// proper location

outp(COLOR_DATA,color->red);
outp(COLOR_DATA,color->green);
outp(COLOR_DATA,color->blue);

} // end Write_Color_Reg

///////////////////////////////////////////////////////////////////////////////

RGB_color_ptr Read_Color_Reg(int index, RGB_color_ptr color)
{

// this function reads the RGB triple out of a palette register and places it
// into "color"

// tell vga card which register to read

outp(COLOR_REGISTER_RD, index);

// now extract the data

color->red   = (unsigned char)inp(COLOR_DATA);
color->green = (unsigned char)inp(COLOR_DATA);
color->blue  = (unsigned char)inp(COLOR_DATA);

// return a pointer to color so that the function can be used as an RVALUE

return(color);

} // end Read_Color_Reg

///////////////////////////////////////////////////////////////////////////////

void Read_Palette(int start_reg,int end_reg, RGB_palette_ptr the_palette)
{
// this function will read the palette registers in the range start_reg to
// end_reg and save them into the appropriate positions in colors

int index; // used for looping

RGB_color color;

// read all the registers

for (index=start_reg; index<=end_reg; index++)
    {
    // read the color register

    Read_Color_Reg(index,(RGB_color_ptr)&color);

    // save it in database

    the_palette->colors[index].red   = color.red;
    the_palette->colors[index].green = color.green;
    the_palette->colors[index].blue  = color.blue;

    } // end for index

// save the interval of registers that were saved

the_palette->start_reg = start_reg;
the_palette->end_reg   = end_reg;

} // end Read_Palette

///////////////////////////////////////////////////////////////////////////////

void Write_Palette(int start_reg,int end_reg, RGB_palette_ptr the_palette)
{
// this function will write to the color registers using the data in the
// sen palette structure

int index; // used for looping

// write all the registers

for (index=start_reg; index<=end_reg; index++)
    {
    // write the color registers using the data from the sent palette

    Write_Color_Reg(index,(RGB_color_ptr)&(the_palette->colors[index]));

    } // end for index

} // end Write_Palette

///////////////////////////////////////////////////////////////////////////////

void Draw_Rectangle(int x1,int y1,int x2,int y2,int color)
{
// this function will draw a rectangle from (x1,y1) - (x2,y2)
// it is assumed that each endpoint is within the screen boundaries
// and (x1,y1) is the upper left hand corner and (x2,y2) is the lower
// right hand corner

unsigned char far *start_offset;   // starting memory offset of first row

int width;   // width of rectangle

// compute starting offset of first row

start_offset = video_buffer + ((y1<<8) + (y1<<6)) + x1;

// compute width of rectangle

width  = 1 + x2 - x1; // the "1" to reflect the true width in pixels

// draw the rectangle row by row

while(y1++<=y2)
     {
     // draw the line

     _fmemset((char far *)start_offset,(unsigned char)color,width);

     // move the memory pointer to the next line

     start_offset+=320;

     } // end while

} // end Draw_Rectangle

////////////////////////////////////////////////////////////////////////////////

void Fill_Screen(int color)
{
// this function will fill the entire screen with the sent color

// use the inline assembler for speed

_asm	{

	les di,video_buffer   // point es:di to video buffer

	mov al,BYTE PTR color // move the color into al and ah

	mov ah,al             // replicate color into ah

	mov cx,320*200/2      // number of words to fill(using word is faster than bytes)

	rep stosw             // move the color into the video buffer really fast!

	} // end inline asm

} // end Fill_Screen

//////////////////////////////////////////////////////////////////////////////

void Fill_Screen_Z(int color)
{
// this function will fill the mode Z video buffer with the sent color

// use the inline assembler for speed

_asm   {
	mov dx,SEQUENCER          // address the sequencer
	mov al,SEQ_PLANE_ENABLE   // select the plane enable register
	mov ah,0fh                // enable all four planes
	out dx,ax                 // do it baby!
	les di,video_buffer   // point es:di to video buffer
	mov al,BYTE PTR color // move the color into al and ah
	mov ah,al             // replicate color into ah
	mov cx,320*400/8      // number of words to fill(using word is faster than bytes)
	rep stosw             // move the color into the video buffer really fast!
	} // end inline asm

} // end Fill_Screen_Z

///////////////////////////////////////////////////////////////////////////////

void Write_Pixel_Z(int x,int y,int color)
{

// this function will write a pixel to screen in mode Z

// first select the proper color plane use inline for speed
// if we used C then there would be a function call and about 10-15 more
// instructions!

_asm   {
	mov dx,SEQUENCER          // address the sequencer
	mov al,SEQ_PLANE_ENABLE   // select the plane enable register
	mov cl,BYTE PTR x         // extract lower byte from x
	and cl,03h                // extract the plane number = x MOD 4
	mov ah,1                  // a "1" selects the plane in the plane enable
	shl ah,cl                 // shift the "1" bit proper number of times
	out dx,ax                 // do it baby!
	} // end asm

// write the pixel, offset = (y*320+x)/4

video_buffer[(y<<6)+(y<<4)+(x>>2)] = (unsigned char )color;

} // end Write_Pixel_Z

///////////////////////////////////////////////////////////////////////////////

void Set_Mode_Z(void)
{
// this function will set the video mode to 320x400x256

int data;  // used to store data

// set system to mode 13h and use it as a foundation to base 320x400 mode on

_asm   {
	mov ax,0013h  // ah=function number 00(set graphics mode), al=13h
	int 10h       // video interrupt 10h
   } // end asm

// make changes to the crt controller first

// set number of scanlines to 1

outp(CRT_CONTROLLER,CRT_MAX_SCANLINE);
data=inp(CRT_CONTROLLER+1);
outp(CRT_CONTROLLER+1,RESET_BITS(data,0x0f));

// use byte addressing instead of word

outp(CRT_CONTROLLER,CRT_ADDR_MODE);
data=inp(CRT_CONTROLLER+1);
outp(CRT_CONTROLLER+1,RESET_BITS(data,0x40));

// second register that needs to reflect byte addressing

outp(CRT_CONTROLLER,CRT_MODE_CONTROL);
data=inp(CRT_CONTROLLER+1);
outp(CRT_CONTROLLER+1,SET_BITS(data,0x40));

// make changes to graphics controller

// set addressing to not use odd/even memory writes

outp(GFX_CONTROLLER,GFX_WRITE_MODE);
data=inp(GFX_CONTROLLER+1);
outp(GFX_CONTROLLER+1,RESET_BITS(data,0x10));

// don't chain the memory maps together

outp(GFX_CONTROLLER,GFX_MISC);
data=inp(GFX_CONTROLLER+1);
outp(GFX_CONTROLLER+1,RESET_BITS(data,0x02));

// make changes to sequencer

// again we must select no chaining and no odd/even memory addressing

outp(SEQUENCER,SEQ_MEMORY_MODE);
data =inp(SEQUENCER+1);
data = RESET_BITS(data,0x08);
data = SET_BITS(data,0x04);
outp(SEQUENCER+1,data);

// now clear the screen

outp(SEQUENCER,SEQ_PLANE_ENABLE);
outp(SEQUENCER+1,0x0f);

// clear the screen, remember it is 320x400, but that is divided into four
// planes, hence we need only to clear 32k out since there will ne four planes
// each being cleared in parallel for a total of 4*32k or 128 = 320x400
// note: "k" in this example means 1000 not 1024

_asm   {

	les di,video_buffer   // point es:di to video buffer, same addre for mode Z
	xor ax,ax             // move a zero into al and ah
	mov cx,320*400/8      // number of words to fill(using word is faster than bytes)
	rep stosw             // move the color into the video buffer really fast!

	} // end inline asm

} // end Set_Mode_Z


