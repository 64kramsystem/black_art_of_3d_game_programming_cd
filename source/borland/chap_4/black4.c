
// BLACK4.C - Library module

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
#include "black4.h"

// G L O B A L S  ////////////////////////////////////////////////////////////

// default sprite width and height

unsigned int sprite_width         = SPRITE_WIDTH;

unsigned int sprite_height        = SPRITE_HEIGHT;

unsigned char far *double_buffer  = NULL;  // the double buffer

// the default dimensions of the double buffer

unsigned int double_buffer_height = SCREEN_HEIGHT;

// size of double buffer in WORDS

unsigned int double_buffer_size   = SCREEN_WIDTH*SCREEN_HEIGHT/2;

unsigned char far *page_0_buffer  = (unsigned char far *)0xA0000000L;

unsigned char far *page_1_buffer  = (unsigned char far *)0xA0008000L;

int mode_z_page                   = PAGE_0;

// F U N C T I O N S /////////////////////////////////////////////////////////

int PCX_Init(pcx_picture_ptr image)
{
// this function allocates the buffer that the image data will be loaded into
// when a PCX file is decompressed

if (!(image->buffer = (unsigned char far *)farmalloc(SCREEN_WIDTH * SCREEN_HEIGHT + 1)))
   {
   printf("\nPCX SYSTEM - Couldn't allocate PCX image buffer");
   return(0);
   } // end if

// success

return(1);

} // end PCX_Init

//////////////////////////////////////////////////////////////////////////////

int PCX_Load(char *filename, pcx_picture_ptr image,int load_palette)
{

// this function loads a PCX file into the image structure. The function
// has three main parts: 1. load the PCX header, 2. load the image data and
// decompress it and 3. load the palette data and update the VGA palette
// note: the palette will only be loaded if the load_palette flag is 1

FILE *fp;                // the file pointer used to open the PCX file

int num_bytes,           // number of bytes in current RLE run
    index;               // loop variable

long count;              // the total number of bytes decompressed

unsigned char data;      // the current pixel data

char far *temp_buffer;   // working buffer

// open the file, test if it exists

if ((fp = fopen(filename,"rb"))==NULL)
   {
   printf("\nPCX SYSTEM - Couldn't find file: %s",filename);
   return(0);

   } // end if couldn't find file

// load the header

temp_buffer = (char far *)image;

for (index=0; index<128; index++)
    {
    temp_buffer[index] = (char)getc(fp);
    } // end for index

// load the data and decompress into buffer, we need a total of 64,000 bytes

count=0;

// loop while 64,000 bytes haven't been decompressed

while(count<=SCREEN_WIDTH * SCREEN_HEIGHT)
	  {
     // get the first piece of data

	  data = (unsigned char)getc(fp);

     // is this a RLE run?

     if (data>=192 && data<=255)
        {
        // compute number of bytes in run

        num_bytes = data-192;

        // get the actual data for the run

        data  = (unsigned char)getc(fp);

        // replicate data in buffer num_bytes times

		  while(num_bytes-->0)
             {
             image->buffer[count++] = data;

             } // end while

        } // end if rle
     else
        {
        // actual data, just copy it into buffer at next location

        image->buffer[count++] = data;

		  } // end else not rle

     } // end while

// load color palette

// move to end of file then back up 768 bytes i.e. to begining of palette

fseek(fp,-768L,SEEK_END);

// load the PCX pallete into the VGA color registers

for (index=0; index<256; index++)
    {
    // get the red component

	 image->palette[index].red   = (unsigned char)(getc(fp) >> 2);

    // get the green component

    image->palette[index].green = (unsigned char)(getc(fp) >> 2);

    // get the blue component

    image->palette[index].blue  = (unsigned char)(getc(fp) >> 2);

    } // end for index

// time to close the file

fclose(fp);

// change the palette to newly loaded palette if commanded to do so

if (load_palette)
	{

   // for each palette register set to the new color values

   for (index=0; index<256; index++)
       {

       Write_Color_Reg(index,(RGB_color_ptr)&image->palette[index]);

       } // end for index

   } // end if load palette data into VGA

// success

return(1);

} // end PCX_Load

/////////////////////////////////////////////////////////////////////////////

void PCX_Delete(pcx_picture_ptr image)
{
// this function de-allocates the buffer region used for the pcx file load

farfree(image->buffer);

} // end PCX_Delete

//////////////////////////////////////////////////////////////////////////////

void PCX_Show_Buffer(pcx_picture_ptr image)
{
// just copy he pcx buffer into the video buffer

char far *data; // temp variable used for aliasing

// alias image buffer

data = image->buffer;

// use inline assembly for speed

_asm   {
	push ds               // save the data segment
	les di, video_buffer  // point es:di to video buffer
	lds si, data          // point ds:si to data area
	mov cx,320*200/2      // move 32000 words
	cld                   // set direction to foward
	rep movsw             // do the string operation
	pop ds                // restore the data segment
	}  // end inline

} // end PCX_Show_Buffer

///////////////////////////////////////////////////////////////////////////////

void PCX_Copy_To_Buffer(pcx_picture_ptr image,unsigned char far *buffer)
{
// this function is used to copy the data in the PCX buffer to another buffer
// usually the double buffer

// use the word copy function, note: double_buffer_size is in WORDS

fwordcpy((void far *)buffer,(void far *)image->buffer,double_buffer_size);

} // end PCX_Copy_To_Buffer

//////////////////////////////////////////////////////////////////////////////

void PCX_Get_Sprite(pcx_picture_ptr image,
                     sprite_ptr sprite,
                     int sprite_frame,
                     int cell_x, int cell_y)

{
// this function is used to load the images for a sprite into the sprite
// frames array. It functions by using the size of the sprite and the
// position of the requested cell to compute the proper location in the
// pcx image buffer to extract the data from.

int x_off,  // position of sprite cell in PCX image buffer
    y_off,
    y,      // looping variable
    width,  // size of sprite
    height;

unsigned char far *sprite_data;

// extract width and height of sprite

width  = sprite->width;
height = sprite->height;

// first allocate the memory for the sprite in the sprite structure

sprite->frames[sprite_frame] = (unsigned char far *)farmalloc(width * height + 1);

// create an alias to the sprite frame for ease of access

sprite_data = sprite->frames[sprite_frame];

// now load the sprite data into the sprite frame array from the pcx picture

x_off = (width+1)  * cell_x + 1;
y_off = (height+1) * cell_y + 1;

// compute starting y address

y_off = y_off * 320; // 320 bytes per line

// scan the data row by row

for (y=0; y<height; y++,y_off+=320)
    {
    // copy the row of pixels

    _fmemcpy((void far *)&sprite_data[y*width],

             (void far *)&(image->buffer[y_off + x_off]),
             width);

    } // end for y

// increment number of frames

sprite->num_frames++;

// done!, let's bail!

} // end PCX_Get_Sprite

//////////////////////////////////////////////////////////////////////////////

void Sprite_Init(sprite_ptr sprite,int x,int y,int width,int height,
                                   int c1,int c2,int c3,
                                   int t1,int t2,int t3)
{
// this function initializes a sprite

int index;

sprite->x            = x;
sprite->y            = y;
sprite->width        = width;
sprite->height       = height;
sprite->visible      = 1;
sprite->counter_1    = c1;
sprite->counter_2    = c2;
sprite->counter_3    = c3;
sprite->threshold_1  = t1;
sprite->threshold_2  = t2;
sprite->threshold_3  = t3;
sprite->curr_frame   = 0;
sprite->state        = SPRITE_DEAD;
sprite->num_frames   = 0;
sprite->background   = (unsigned char far *)farmalloc(width * height+1);

// set all bitmap pointers to null

for (index=0; index<MAX_SPRITE_FRAMES; index++)
    sprite->frames[index] = NULL;

} // end Sprite_Init

//////////////////////////////////////////////////////////////////////////////

void Sprite_Delete(sprite_ptr sprite)
{
// this function deletes all the memory associated with a sprite

int index;

farfree(sprite->background);

// now de-allocate all the animation frames

for (index=0; index<MAX_SPRITE_FRAMES; index++)
    farfree(sprite->frames[index]);

} // end Sprite_Delete

////////////////////////////////////////////////////////////////////////////////

void Sprite_Under(sprite_ptr sprite, unsigned char far *buffer)
{

// this function scans the background under a sprite so that when the sprite
// is drawn the background isn't obliterated

unsigned char far *back_buffer; // background buffer for sprite

int y,                          // current line being scanned
    width,                      // size of sprite
    height;

// alias a pointer to sprite background for ease of access

back_buffer = sprite->background;

// alias width and height

width  = sprite->width;
height = sprite->height;

// compute offset of background in source buffer

buffer = buffer + (sprite->y << 8) + (sprite->y << 6) + sprite->x;

for (y=0; y<height; y++)
    {
    // copy the next row out off image buffer into sprite background buffer

    _fmemcpy((void far *)back_buffer,
             (void far *)buffer,
             width);

    // move to next line in source buffer and in sprite background buffer

    buffer      += SCREEN_WIDTH;
    back_buffer += width;

    } // end for y

} // end Sprite_Under

//////////////////////////////////////////////////////////////////////////////

void Sprite_Erase(sprite_ptr sprite,unsigned char far *buffer)
{
// replace the background that was behind the sprite
// this function replaces the background that was saved from where a sprite
// was going to be placed

unsigned char far *back_buffer; // background buffer for sprite

int y,                          // current line being scanned
	 width,                      // size of sprite
    height;

// alias a pointer to sprite background for ease of access

back_buffer = sprite->background;

// alias width and height

width  = sprite->width;
height = sprite->height;

// compute offset in destination buffer

buffer = buffer + (sprite->y << 8) + (sprite->y << 6) + sprite->x;

for (y=0; y<height; y++)
    {
    // copy the next from sprite background buffer to destination buffer

    _fmemcpy((void far *)buffer,
             (void far *)back_buffer,
             width);

	 // move to next line in destination buffer and in sprite background buffer

    buffer      += SCREEN_WIDTH;
    back_buffer += width;

    } // end for y

} // end Sprite_Erase

//////////////////////////////////////////////////////////////////////////////

void Sprite_Draw(sprite_ptr sprite, unsigned char far *buffer,int transparent)
{

// this function draws a sprite on the screen row by row very quickly
// note the use of shifting to implement multplication
// if the transparent flag is true then pixels wil be draw one by one
// else a memcpy will be used to draw each line

unsigned char far *sprite_data; // pointer to sprite data
unsigned char far *dest_buffer; // pointer to destination buffer

int x,y,                        // looping variables
    width,                      // width of sprite
	 height;                     // height of sprite

unsigned char pixel;            // the current pixel being processed

// alias a pointer to sprite for ease of access

sprite_data = sprite->frames[sprite->curr_frame];

// alias a variable to sprite size

width  = sprite->width;
height = sprite->height;

// compute number of bytes between adjacent video lines after a row of pixels
// has been drawn

// compute offset of sprite in destination buffer

dest_buffer = buffer + (sprite->y << 8) + (sprite->y << 6) + sprite->x;

// copy each line of the sprite data into destination buffer


if (transparent)
	{
	for (y=0; y<height; y++)
       {
       // copy the next row into the destination buffer

       for (x=0; x<width; x++)
           {

			  // test for transparent pixel i.e. 0, if not transparent then draw

           if ((pixel=sprite_data[x]))
                dest_buffer[x] = pixel;

           } // end for x

       // move to next line in desintation buffer and sprite image buffer

		 dest_buffer += SCREEN_WIDTH;
       sprite_data += width;

       } // end for y

   } // end if transparent
else
	{
   // draw sprite with transparency off

	for (y=0; y<height; y++)
       {
       // copy the next row into the destination buffer

       _fmemcpy((void far *)dest_buffer,(void far *)sprite_data,width);

		 // move to next line in desintation buffer and sprite image buffer

       dest_buffer += SCREEN_WIDTH;
       sprite_data += width;

       } // end for y

	} // end else

} // end Sprite_Draw


////////////////////////////////////////////////////////////////////////////////

void Sprite_Under_Clip(sprite_ptr sprite, unsigned char far *buffer)
{

// this function scans the background under a sprite, but only those
// portions that are visible

unsigned char far *back_buffer;   // pointer to sprite background buffer
unsigned char far *source_buffer; // pointer to source buffer

int x,y,                        // looping variables
	 sx,sy,                      // position of sprite
    width,                      // width of sprite
    bitmap_width  =0,           // width and height of sub-bitmap
    bitmap_height =0;

unsigned char pixel;            // the current pixel being processed

// alias a variable to sprite size

width         = sprite->width;
bitmap_width  = width;
bitmap_height = sprite->height;
sx            = sprite->x;
sy            = sprite->y;

// perform trivial rejection tests

if (sx >= (int)SCREEN_WIDTH || sy >= (int)double_buffer_height ||
   (sx+width) <= 0          || (sy+bitmap_height) <= 0)
   {
   // sprite is totally invisible therefore don't scan

   // set invisible flag in strucuture so that the draw sub-function
   // doesn't do anything

   sprite->visible = 0;

   return;

   } // end if invisible

// the sprite background region must be clipped before scanning
// therefore compute visible portion

// first compute upper left hand corner of clipped sprite background

if (sx<0)
   {

   bitmap_width += sx;
	sx            = 0;


   } // end off left edge
else
if (sx+width>=(int)SCREEN_WIDTH)
   {

   bitmap_width  = (int)SCREEN_WIDTH-sx;

   } // end off right edge

// now process y

if (sy<0)
   {

   bitmap_height += sy;
   sy             = 0;

	} // end off top edge
else
if (sy+bitmap_height>=(int)double_buffer_height)
	{

   bitmap_height  = (int)double_buffer_height - sy;

   } // end off lower edge

// this point we know were to start scanning the bitmap i.e.
// sx,sy
// and we know the size of the bitmap to be scanned i.e.
// width,height, so plug it all into the rest of function

// compute number of bytes between adjacent video lines after a row of pixels
// has been drawn

// compute offset of sprite background in source buffer

source_buffer = buffer + (sy << 8) + (sy << 6) + sx;

// alias a pointer to sprite background

back_buffer = sprite->background;

for (y=0; y<bitmap_height; y++)
    {
    // copy the next row into the destination buffer

    _fmemcpy((void far *)back_buffer,(void far *)source_buffer,bitmap_width);

    // move to next line in desintation buffer and sprite image buffer

    source_buffer += SCREEN_WIDTH;
    back_buffer   += width;  // note this width is the actual width of the
									  // entire bitmap NOT the visible portion
    } // end for y

// set variables in strucuture so that the erase sub-function can operate
// faster

sprite->x_clip      = sx;
sprite->y_clip      = sy;
sprite->width_clip  = bitmap_width;
sprite->height_clip = bitmap_height;
sprite->visible     = 1;

} // end Sprite_Under_Clip

//////////////////////////////////////////////////////////////////////////////

void Sprite_Erase_Clip(sprite_ptr sprite,unsigned char far *buffer)
{
// replace the background that was behind the sprite
// this function replaces the background that was saved from where a sprite
// was going to be placed

unsigned char far *back_buffer; // background buffer for sprite

int y,                          // current line being scanned
	 width,                      // size of sprite background buffer
    bitmap_height,              // size of clipped bitmap
    bitmap_width;

// make sure sprite was visible

if (!sprite->visible)
	return;

// alias a pointer to sprite background for ease of access

back_buffer = sprite->background;

// alias width and height

bitmap_width  = sprite->width_clip;
bitmap_height = sprite->height_clip;
width         = sprite->width;

// compute offset in destination buffer

buffer = buffer + (sprite->y_clip << 8) + (sprite->y_clip << 6)
                + sprite->x_clip;

for (y=0; y<bitmap_height; y++)
    {
    // copy the next row from sprite background buffer to destination buffer

    _fmemcpy((void far *)buffer,
             (void far *)back_buffer,
				 bitmap_width);

	 // move to next line in destination buffer and in sprite background buffer

    buffer      += SCREEN_WIDTH;
    back_buffer += width;

    } // end for y

} // end Sprite_Erase_Clip

//////////////////////////////////////////////////////////////////////////////

void Sprite_Draw_Clip(sprite_ptr sprite, unsigned char far *buffer,int transparent)
{

// this function draws a sprite on the screen row by row very quickly
// note the use of shifting to implement multplication
// if the transparent flag is true then pixels wil be draw one by one
// else a memcpy will be used to draw each line
// this function also performs clipping. It will test if the sprite
// is totally visible/invisible and will only draw the portions that are visible

unsigned char far *sprite_data; // pointer to sprite data
unsigned char far *dest_buffer; // pointer to destination buffer

int x,y,                        // looping variables
    sx,sy,                      // position of sprite
    width,                      // width of sprite
    bitmap_x      =0,           // starting upper left corner of sub-bitmap
	 bitmap_y      =0,           // to be drawn after clipping
    bitmap_width  =0,           // width and height of sub-bitmap
    bitmap_height =0;

unsigned char pixel;            // the current pixel being processed

// alias a variable to sprite size

width         = sprite->width;
bitmap_width  = width;
bitmap_height = sprite->height;
sx            = sprite->x;
sy            = sprite->y;

// perform trivial rejection tests

if (sx >= (int)SCREEN_WIDTH || sy >= (int)double_buffer_height ||
   (sx+width) <= 0          || (sy+bitmap_height) <= 0 || !sprite->visible)
   {
   // sprite is totally invisible therefore don't draw

   // set invisible flag in strucuture so that the erase sub-function
   // doesn't do anything

   sprite->visible = 0;

   return;

   } // end if invisible

// the sprite needs some clipping or no clipping at all, so compute
// visible portion of sprite rectangle

// first compute upper left hand corner of clipped sprite

if (sx<0)
	{

   bitmap_x      = -sx;
   sx            = 0;
   bitmap_width -= bitmap_x;

   } // end off left edge
else
if (sx+width>=(int)SCREEN_WIDTH)
	{

   bitmap_x      = 0;
   bitmap_width  = (int)SCREEN_WIDTH-sx;

   } // end off right edge

// now process y

if (sy<0)
   {

   bitmap_y       = -sy;
   sy             = 0;
	bitmap_height -= bitmap_y;

   } // end off top edge
else
if (sy+bitmap_height>=(int)double_buffer_height)
   {

   bitmap_y       = 0;
   bitmap_height  = (int)double_buffer_height - sy;

   } // end off lower edge

// this point we know were to start drawing the bitmap i.e.
// sx,sy
// and we know were in the data to extract the bitmap i.e.
// bitmap_x, bitmap_y,
// and finaly we know the size of the bitmap to be drawn i.e.
// width,height, so plug it all into the rest of function

// compute number of bytes between adjacent video lines after a row of pixels
// has been drawn

// compute offset of sprite in destination buffer

dest_buffer = buffer + (sy << 8) + (sy << 6) + sx;

// alias a pointer to sprite for ease of access and locate starting sub
// bitmap that will be drawn

sprite_data = sprite->frames[sprite->curr_frame] + (bitmap_y*width) + bitmap_x;

// copy each line of the sprite data into destination buffer

if (transparent)
   {
   for (y=0; y<bitmap_height; y++)
       {
       // copy the next row into the destination buffer

       for (x=0; x<bitmap_width; x++)
           {

           // test for transparent pixel i.e. 0, if not transparent then draw

           if ((pixel=sprite_data[x]))
                dest_buffer[x] = pixel;

           } // end for x

       // move to next line in desintation buffer and sprite image buffer

       dest_buffer += SCREEN_WIDTH;
       sprite_data += width;   // note this width is the actual width of the
                               // entire bitmap NOT the visible portion
       } // end for y

   } // end if transparent
else
   {
   // draw sprite with transparency off

	for (y=0; y<bitmap_height; y++)
       {
       // copy the next row into the destination buffer

       _fmemcpy((void far *)dest_buffer,(void far *)sprite_data,bitmap_width);

       // move to next line in desintation buffer and sprite image buffer

       dest_buffer += SCREEN_WIDTH;
       sprite_data += width;  // note this width is the actual width of the
                              // entire bitmap NOT the visible portion
       } // end for y

   } // end else

// set variables in strucuture so that the erase sub-function can operate
// faster

sprite->x_clip      = sx;
sprite->y_clip      = sy;
sprite->width_clip  = bitmap_width;
sprite->height_clip = bitmap_height;
sprite->visible     = 1;

} // end Sprite_Draw_Clip


//////////////////////////////////////////////////////////////////////////////

int Sprites_Collide(sprite_ptr sprite_1, sprite_ptr sprite_2)
{
// this function tests if two sprites have intersected by testing their
// bounding boxes for collision

return(0);

} // end Sprites_Collide

///////////////////////////////////////////////////////////////////////////////

void Display_Double_Buffer(unsigned char far *buffer,int y)
{
// this functions copies the double buffer into the video buffer at the
// starting y location

_asm
	{
	push ds                     // save DS on stack
	mov cx,double_buffer_size   // this is the size of buffer in WORDS
	les di,video_buffer         // es:di is destination of memory move

	mov ax,320                  // multiply y by 320 i.e. screen width
	mul y
	add di,ax                   // add result to es:di

	lds si,buffer               // ds:si is source of memory move
	rep movsw                   // move all the words
	pop ds                      // restore the data segment
	} // end asm

} // end Display_Double_Buffer

//////////////////////////////////////////////////////////////////////////////

int Create_Double_Buffer(int num_lines)
{

// allocate enough memory to hold the double buffer

if ((double_buffer = (unsigned char far *)farmalloc(SCREEN_WIDTH * (num_lines + 1)))==NULL)
   {
   printf("\nCouldn't allocate double buffer.");
   return(0);
   } // end if couldn't allocate

// set the height of the buffer and compute it's size

double_buffer_height = num_lines;

double_buffer_size = SCREEN_WIDTH * num_lines/2;

// fill the buffer with black

_fmemset(double_buffer, 0, SCREEN_WIDTH * num_lines);

// everything was ok

return(1);

} // end Init_Double_Buffer

///////////////////////////////////////////////////////////////////////////////

void Fill_Double_Buffer(int color)
{
// this function fills in the double buffer with the sent color a WORD at
// a time

_asm	{
	mov cx,double_buffer_size // this is the size of buffer in WORDS
	mov al, BYTE PTR color    // move the color into al
	mov ah,al                 // move the color in ah
	les di,double_buffer      // es:di points to the double buffer
	rep stosw                 // fill all the words
	} // end asm

} // end Fill_Double_Buffer

//////////////////////////////////////////////////////////////////////////////

void Delete_Double_Buffer(void)
{
// this function free's up the memory allocated by the double buffer
// make sure to use FAR version

if (double_buffer)
  farfree(double_buffer);

} // end Delete_Double_Buffer

///////////////////////////////////////////////////////////////////////////////

void Screen_Transition(int effect)
{
// this function can be called to perform a myraid of screen transitions
// to the video buffer, note I have left one for you to create!

int pal_reg;       //  used as loop counter
long index;        // used as loop counter
RGB_color color;   // temporary color

// test which screen effect is being selected

switch(effect)
      {

      case SCREEN_DARKNESS:
           {
           // fade to black

			  for (index=0; index<20; index++)
               {
               // loop thru all palette registers

               for (pal_reg=1; pal_reg<255; pal_reg++)
                   {
                   // get the next color to fade

                   Read_Color_Reg(pal_reg,(RGB_color_ptr)&color);

                   // test if this color regisyer is already black

                   if (color.red > 4) color.red-=3;
                   else
                      color.red = 0;

						 if (color.green > 4) color.green-=3;
                   else
                      color.green = 0;

                   if (color.blue  > 4) color.blue-=3;
                   else
                      color.blue = 0;

                   // set the color to a diminished intensity

                   Write_Color_Reg(pal_reg,(RGB_color_ptr)&color);

                   } // end for pal_reg

					// wait a bit

					Time_Delay(1);

               } // end for index

           } break;

      case SCREEN_WHITENESS:
			  {
           // fade to white

			  for (index=0; index<20; index++)
               {

               // loop thru all palette registers

					for (pal_reg=0; pal_reg<255; pal_reg++)
						 {
                   // get the color to fade

                   Read_Color_Reg(pal_reg,(RGB_color_ptr)&color);

                   color.red+=4;

						 if (color.red > 63)
                      color.red = 63;

                   color.green+=4;

                   if (color.green > 63)
                      color.green = 63;

						 color.blue+=4;

                   if (color.blue >63)
                      color.blue = 63;

                   // set the color to a brightend intensity

                   Write_Color_Reg(pal_reg,(RGB_color_ptr)&color);

                   } // end for pal_reg

               // wait a bit

               Time_Delay(1);

               } // end for index

			  } break;


      case SCREEN_WARP:
           {

           // this one you do!!!!

           } break;

      case SCREEN_SWIPE_X:
           {
           // do a screen wipe from right to left, left to right

           for (index=0; index<160; index+=2)
					{

               // use this as a 1/70th of second time delay

               Wait_For_Vertical_Retrace();

               // draw two vertical lines at opposite ends of the screen

					Line_V(0,199,319-index,0);
               Line_V(0,199,index,0);
               Line_V(0,199,319-(index+1),0);
               Line_V(0,199,index+1,0);

               } // end for index

           } break;

		case SCREEN_SWIPE_Y:
           {

           // do a screen wipe from top to bottom, bottom to top

           for (index=0; index<100; index+=2)
               {

               // use this as a 1/70th of second time delay

               Wait_For_Vertical_Retrace();

               // draw two horizontal lines at opposite ends of the screen

               Line_H(0,319,199-index,0);
					Line_H(0,319,index,0);
					Line_H(0,319,199-(index+1),0);
               Line_H(0,319,index+1,0);

               } // end for index

           } break;


      case SCREEN_DISOLVE:
           {
           // disolve the screen by plotting zillions of little black dots

           for (index=0; index<=300000; index++)
                Write_Pixel(rand()%320,rand()%200, 0);

			  } break;

      default:break;

      } // end switch

} // end Screen_Transition

//////////////////////////////////////////////////////////////////////////////////

void Wait_For_Vertical_Retrace(void)
{
// this function waits for the start of a vertical retrace, if a vertical
// retrace is in progress then it waits until the next one
// therefore the function can wait a maximum of 2/70 th's of a second
// before returning
// this function can be used to synchronize video updates to the vertical blank
// or as a high resolution time reference

while(inp(VGA_INPUT_STATUS_1) & VGA_VRETRACE_MASK)
	  {
	  // do nothing, vga is already in retrace
	  } // end while

// now wait for start of vertical retrace and exit

while(!(inp(VGA_INPUT_STATUS_1) & VGA_VRETRACE_MASK))
     {
     // do nothing, wait for start of retrace
     } // end while

// at this point a vertical retrace is occuring, so return back to caller

} // end Wait_For_Vertical_Retrace

///////////////////////////////////////////////////////////////////////////////

void fwordcpy(void far *destination, void far *source,int num_words)
{
// this function is similar to fmemcpy except that is moves data in words
// it is about 25% faster than memcpy which uses bytes

_asm	{
	push ds              // need to save segment registers i.e. ds
	les di,destination   // point es:di to destination of memory move
	lds si,source        // point ds:si to source of memory move
	mov cx,num_words     // move into cx the number of words to be moved
	rep movsw            // let the processor do the memory move
	pop ds               // restore the ds segment register

   } // end inline asm

} // end fwordcpy

//////////////////////////////////////////////////////////////////////////////

void Bitmap_Put(bitmap_ptr image, unsigned char far *destination,int transparent)
{
// this fucntion will draw a bitmap on the destination buffer which could
// be a double buffer or the video buffer

int x,y,                        // looping variables
    width,height;               // size of bitmap

unsigned char far *bitmap_data; // pointer to bitmap buffer
unsigned char far *dest_buffer; // pointer to destination buffer

unsigned char pixel;            // current pixel value being processed

// compute offset of bitmap in destination buffer. note: all video or double
// buffers must be 320 bytes wide!

dest_buffer = destination + (image->y << 8) + (image->y << 6) + image->x;

// create aliases to variables so the strucuture doesn't need to be
// dereferenced continually

height      = image->height;
width       = image->width;
bitmap_data = image->buffer;

// test if transparency is on or off

if (transparent)
   {
   // use version that will draw a transparent bitmap(slighlty slower)

   // draw each line of the bitmap

   for (y=0; y<height; y++)
       {
       // copy the next row into the destination buffer

       for (x=0; x<width; x++)
           {

			  // test for transparent pixel i.e. 0, if not transparent then draw

           if ((pixel=bitmap_data[x]))
                dest_buffer[x] = pixel;

           } // end for x

       // move to next line in double buffer and in bitmap buffer

       dest_buffer += SCREEN_WIDTH;
       bitmap_data += width;

       } // end for y

   } // end if transparent
else
	{

   // draw each line of the bitmap, note how each pixel doesn't need to be
   // tested for transparency hence a memcpy can be used (very fast!)

   for (y=0; y<height; y++)
       {
       // copy the next row into the destination buffer using memcpy for speed

       _fmemcpy((void far *)dest_buffer,
                (void far *)bitmap_data,width);

       // move to next line in destination buffer and in bitmap buffer

       dest_buffer += SCREEN_WIDTH;
       bitmap_data += width;

		 } // end for y

   } // end else non-transparent version

} // end Bitmap_Put

//////////////////////////////////////////////////////////////////////////////

void Bitmap_Get(bitmap_ptr image, unsigned char far *source)
{
// this function will scan a bitmap fomr the source buffer
// could be a double buffer, video buffer or any other buffer with a
// logical row width of 320 bytes

unsigned int source_off,       // offsets into destination and source buffers
				 bitmap_off;

int y,                         // looping variable
    width,height;              // size of bitmap

unsigned char far *bitmap_data; // pointer to bitmap buffer


// compute offset of bitmap in source buffer. note: all video or double
// buffers must be 320 bytes wide!

source_off   = (image->y << 8) + (image->y << 6) + image->x;

bitmap_off = 0;

// create aliases to variables so the strucuture doesn't need to be
// dereferenced continually

height      = image->height;
width       = image->width;
bitmap_data = image->buffer;

// draw each line of the bitmap, note how each pixel doesn't need to be
// tested for transparency hence a memcpy can be used (very fast!)

for (y=0; y<height; y++)
    {
    // copy the next row into the bitmap buffer using memcpy for speed

    _fmemcpy((void far *)&bitmap_data[bitmap_off],
             (void far *)&source[source_off],width);

	 // move to next line in source buffer and in bitmap buffer

    source_off += SCREEN_WIDTH;
    bitmap_off += width;

    } // end for y

} // end Bitmap_Get

//////////////////////////////////////////////////////////////////////////////

int Bitmap_Allocate(bitmap_ptr image, int width, int height)
{
// this function can be used to allocate the memory needed for a bitmap

if ((image->buffer = (unsigned char far *)farmalloc(width*height+1))==NULL)
	return(0);
else
   return(1);

} // end Bitmap_Allocate

//////////////////////////////////////////////////////////////////////////////

void Bitmap_Delete(bitmap_ptr the_bitmap)
{
// this function deletes the memory used by a bitmap

if (the_bitmap->buffer)
   farfree(the_bitmap->buffer);

} // end Bitmap_Delete

//////////////////////////////////////////////////////////////////////////////

void Layer_Draw(layer_ptr source_layer, int source_x, int source_y,
                unsigned char far *dest_buffer,int dest_y,int dest_height,
                int transparent)
{
// this function will map down a section of the layer onto the destination
// buffer at the desired location, note the width of the destination buffer
// is always assumed to be 320 bytes width. Also, the function will always
// wrap around the layer

int x,y,                        // looping variables
    layer_width,                // the width of the layer
    right_width,                // the width of the right and left half of
    left_width;                 // the layer to be drawn

unsigned char far *layer_buffer_l; // pointers to the left and right halves
unsigned char far *dest_buffer_l;  // of the layer buffer and destination
unsigned char far *layer_buffer_r; // buffer
unsigned char far *dest_buffer_r;

unsigned char pixel;            // current pixel value being processed

layer_width = source_layer->width;

dest_buffer_l  = dest_buffer + (dest_y << 8) + (dest_y << 6);

layer_buffer_l = source_layer->buffer + layer_width*source_y + source_x;

// test if wraping is needed

if ( ( (layer_width-source_x)-(int)SCREEN_WIDTH ) >= 0)
	{
   // there's enough data in layer to draw a complete line, no wraping needed

   left_width  = SCREEN_WIDTH;

   right_width = 0; // no wraping flag

	}
else
   {
   // wrapping needed

   left_width  = layer_width - source_x;

   right_width = SCREEN_WIDTH - left_width;

	dest_buffer_r  = dest_buffer_l + left_width;

   layer_buffer_r = layer_buffer_l - source_x; // move to far left end of layer

   } // end else need to wrap

// test if transparency is on or off

if (transparent)
   {
   // use version that will draw a transparent bitmap(slighlty slower)
   // first draw left half then right half

   // draw each line of the bitmap

	for (y=0; y<dest_height; y++)
		 {
       // copy the next row into the destination buffer

       for (x=0; x<left_width; x++)
           {

           // test for transparent pixel i.e. 0, if not transparent then draw

           if ((pixel=layer_buffer_l[x]))
                dest_buffer_l[x] = pixel;

           } // end for x

       // move to next line in destination buffer and in layer buffer

		 dest_buffer_l  += SCREEN_WIDTH;
		 layer_buffer_l += layer_width;

       } // end for y

   // now right half

   // draw each line of the bitmap

   if (right_width)
      {
      for (y=0; y<dest_height; y++)
          {
          // copy the next row into the destination buffer

          for (x=0; x<right_width; x++)
				  {

              // test for transparent pixel i.e. 0, if not transparent then draw

              if ((pixel=layer_buffer_r[x]))
                   dest_buffer_r[x] = pixel;

              } // end for x

          // move to next line in destination buffer and in layer buffer

          dest_buffer_r  += SCREEN_WIDTH;
          layer_buffer_r += layer_width;

          } // end for y

		} // end if right side needs to be drawn

   } // end if transparent
else
   {

   // draw each line of the bitmap, note how each pixel doesn't need to be
   // tested for transparency hence a memcpy can be used (very fast!)

   for (y=0; y<dest_height; y++)
       {
       // copy the next row into the destination buffer using memcpy for speed

       _fmemcpy((void far *)dest_buffer_l,
                (void far *)layer_buffer_l,left_width);

		 // move to next line in double buffer and in bitmap buffer

       dest_buffer_l  += SCREEN_WIDTH;
       layer_buffer_l += layer_width;

       } // end for y

   // now right half if needed

   if (right_width)
      {

      for (y=0; y<dest_height; y++)
          {
          // copy the next row into the destination buffer using memcpy for speed

			 _fmemcpy((void far *)dest_buffer_r,
						 (void far *)layer_buffer_r,right_width);

          // move to next line in double buffer and in bitmap buffer

          dest_buffer_r  += SCREEN_WIDTH;
          layer_buffer_r += layer_width;

			 } // end for y

      } // end if right half

   } // end else non-transparent version

} // end Layer_Draw

//////////////////////////////////////////////////////////////////////////////

void Layer_Build(layer_ptr dest_layer,int dest_x, int dest_y,
                 unsigned char far *source_buffer,int source_x,int source_y,
                 int width,int height)
{
// this function is used to build up the layer out of smaller pieces
// this allows a layer to be very long, tall etc. also the source data buffer
// must be constructed such that there are 320 bytes per row

int y,                       // looping variable
    layer_width;             // the width of the layer

unsigned char far *source_data;   // pointer to start of source bitmap image
unsigned char far *layer_buffer;  // pointer to layer buffer

// extract width of layer

layer_width  = dest_layer->width;

// compute starting location in layer buffer

layer_buffer = dest_layer->buffer + layer_width*dest_y + dest_x; ;

// compute starting location in source image buffer

source_data = source_buffer + (source_y << 8) + (source_y << 6) + source_x;

// scan each line of source image into layer buffer

for (y=0; y<height; y++)
    {
	 // copy the next row into the layer buffer using memcpy for speed

    _fmemcpy((void far *)layer_buffer,
             (void far *)source_data,width);

    // move to next line in source buffer and in layer buffer

    source_data  += SCREEN_WIDTH;
	 layer_buffer += layer_width;

    } // end for y

} // end Layer_Build

//////////////////////////////////////////////////////////////////////////////

int Layer_Create(layer_ptr dest_layer, int width, int height)
{
// this function can be used to allocate the memory needed for a layer
// the width must be divisible by two.

if ((dest_layer->buffer = (unsigned char far *)farmalloc(width*height+2))==NULL)
   return(0);
else
	{
   // save the dimensions of layer

   dest_layer->width  = width;
   dest_layer->height = height;

   return(1);

	} // end else

} // end Layer_Create

//////////////////////////////////////////////////////////////////////////////

void Layer_Delete(layer_ptr the_layer)
{
// this function deletes the memory used by a layer

if (the_layer->buffer)
	farfree(the_layer->buffer);

} // end Layer_Delete

//////////////////////////////////////////////////////////////////////////////

void Print_Char_DB(int xc,int yc,char c,int color,int transparent)
{
// this function is used to print a character on the double buffer. It uses the
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

// compute offset of character in double buffer, use shifting to multiply

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
             double_buffer[offset+x] = (unsigned char)color;

        else
        if (!transparent)               // takes care of transparency
            double_buffer[offset+x] = 0; // make black part opaque

        // shift bit mask

		  bit_mask = (bit_mask>>1);

        } // end for x

    // move to next line in double buffer and in rom character data area

    offset      += MODE13_WIDTH;
	 work_char++;

    } // end for y

} // end Print_Char_DB

//////////////////////////////////////////////////////////////////////////////

void Print_String_DB(int x,int y,int color, char *string,int transparent)
{
// this function prints an entire string into the double buffer with fixed
// spacing between each character by calling the Print_Char() function

 int index,   // loop index
     length;  // length of string

// compute length of string

length = strlen(string);

// print the string a character at a time

for (index=0; index<length; index++)
     Print_Char_DB(x+(index<<3),y,string[index],color,transparent);

} // end Print_String_DB

///////////////////////////////////////////////////////////////////////////////

void Write_Pixel_DB(int x,int y,int color)
{

// plots the pixel in the desired color to the double buffer
// to accomplish the multiplications

// use the fact that 320*y = 256*y + 64*y = y<<8 + y<<6

double_buffer[((y<<8) + (y<<6)) + x] = (unsigned char )color;

} // end Write_Pixel_DB

///////////////////////////////////////////////////////////////////////////////

int Read_Pixel_DB(int x,int y)
{
// this function read a pixel from the double buffer

// use the fact that 320*y = 256*y + 64*y = y<<8 + y<<6

return((int)(double_buffer[((y<<8) + (y<<6)) + x]));

} // end Read_Pixel_DB

//////////////////////////////////////////////////////////////////////////////

void Set_Visual_Page_Mode_Z(int page)
{
// this function sets the visual page that will be displayed by the VGA

if (page==PAGE_0)
   {
   // re-program the start address registers in the CRT controller
   // to point at page 0 @ 0xA000:0000

	// first low byte of address

   outp(CRT_CONTROLLER,CRT_ADDR_LOW);
   outp(CRT_CONTROLLER+1,0x00);

   // now high byte

   outp(CRT_CONTROLLER,CRT_ADDR_HI);
	outp(CRT_CONTROLLER+1,0x00);

   } // end if page 0
else
if (page==PAGE_1)
   {
   // re-program the start address registers in the CRT controller
   // to point at page 1 @ 0xA000:8000

   // first low byte of address

   outp(CRT_CONTROLLER,CRT_ADDR_LOW);
   outp(CRT_CONTROLLER+1,0x00);

   // now high byte

	outp(CRT_CONTROLLER,CRT_ADDR_HI);
	outp(CRT_CONTROLLER+1,0x80);

   } // end else page 1

// note: we could use WORD out's, but this is clearer, feel free to change them

} // end Set_Visual_Page_Mode_Z

///////////////////////////////////////////////////////////////////////////////

void Set_Working_Page_Mode_Z(int page)
{
// this function sets the page that all mode Z functions will update when
// called

if (page==PAGE_0)
	video_buffer = page_0_buffer;
else
   video_buffer = page_1_buffer;

} // end Set_Working_Page_Mode_Z


