
// BLACK6.C - Library Module

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


#include "black6.h"

// G L O B A L S  ////////////////////////////////////////////////////////////



// F U N C T I O N S /////////////////////////////////////////////////////////

int Sound_Load(char *filename, sound_ptr the_sound,int translate)
{
// this function will load a sound from disk into memory and pre-format
// it in preparation to be played

unsigned char far *temp_ptr;   // temporary pointer used to load sound
unsigned char far *sound_ptr;  // pointer to sound data

unsigned int segment,          // segment of sound data memory
             paragraphs,       // number of 16 byte paragraphs sound takes up
             bytes_read,       // used to track numbr of bytes read by DOS
             size_of_file,     // the total size of the VOC file in bytes
             header_length;    // the length of the header portion of VOC file

int sound_handle;              // DOS file handle


// open the sound file, use DOS file and memory allocation to make sure
// memory is on a 16 byte or paragraph boundary

if (_dos_open(filename, _O_RDONLY, &sound_handle)!=0)
   {
   printf("\nSound System - Couldn't open %s",filename);
   return(0);

   } // end if file not found

// compute number of paragraphs that sound file needs

size_of_file = _filelength(sound_handle);

paragraphs = 1 + (size_of_file)/16;

// allocate the memory on a paragraph boundary

_dos_allocmem(paragraphs,&segment);

// point data pointer to allocated data area

_FP_SEG(sound_ptr) = segment;
_FP_OFF(sound_ptr) = 0;

// alias pointer to memory storage area

temp_ptr = sound_ptr;

// read in blocks of 16k until file is loaded

do
 {
 // load next block

 _dos_read(sound_handle,temp_ptr, 0x4000, &bytes_read);

 // adjust pointer

 temp_ptr += bytes_read;

 } while(bytes_read==0x4000);

// close the file

_dos_close(sound_handle);

// make sure it's a voc file, test for "Creative"

if ((sound_ptr[0] != 'C') || (sound_ptr[1] != 'r'))
   {

   printf("\n%s is not a VOC file!",filename);

   // de-allocate the memory

   _dos_freemem(_FP_SEG(sound_ptr));

   // return failure

   return(0);

   } // end if voc file

   // compute start of sound data;

   header_length = (unsigned int)sound_ptr[20];

   // point buffer pointer to start of VOC file in memory

   the_sound->buffer       = sound_ptr;

   // set up the SNDSTRUC for DIGIPAK

   the_sound->SS.sound     = (unsigned char far*)(sound_ptr+header_length+4);

   the_sound->SS.sndlen    = (unsigned short)(size_of_file - header_length);

   the_sound->SS.IsPlaying = (short far *)&the_sound->status;

   the_sound->SS.frequency = (short)((long)(-1000000) /
                                ((int)sound_ptr[header_length+4]-256));

   // now format data for sound card if requested

   if (translate)
       Sound_Translate(the_sound);

// return success

return(1);

} // end Sound_Load

//////////////////////////////////////////////////////////////////////////////

void Sound_Unload(sound_ptr the_sound)
{

// this functions deletes the sound from memory

_dos_freemem(_FP_SEG(the_sound->buffer));

the_sound->buffer=NULL;

} // end Sound_Unload

//////////////////////////////////////////////////////////////////////////////

void Sound_Translate(sound_ptr the_sound)
{
// this function calls the DIGIPAK function massage audio to translate
// the raw audio data into the proper format for the sound card that
// the sound system is running on.

unsigned char far *buffer;

buffer = (unsigned char far*)&the_sound->SS;

_asm
   {
   push ds         ; save DS and SI on stack
   push si
   mov ax, 068Ah   ; function 3: MassageAudio
   lds si, buffer  ; move address of sound in DS:SI
   int 66h         ; call DIGIPAK
   pop si          ; restore DS and SI from stack
   pop ds

   } // end inline assembly

} // end Sound_Translate

///////////////////////////////////////////////////////////////////////////////

void Sound_Play(sound_ptr the_sound)
{
// ths function will the sound pointed to in the sent sound structure

unsigned char far *buffer;

buffer = (unsigned char far*)&the_sound->SS;

_asm
   {
   push ds         ; save DS and SI on stack
   push si
   mov ax, 068Bh   ; function 4: DigPlay2
   lds si, buffer  ; move address of sound in DS:SI
   int 66h         ; call DIGIPAK
   pop si          ; restore DS and SI from stack
   pop ds

   } // end inline assembly

} // end Sound_Play

//////////////////////////////////////////////////////////////////////////////

int Sound_Status(void)
{
// this function will return the status of DIGIPAK i.e. is a sound playing
// or not

_asm
   {
   mov ax, 0689h   ; function 2: SoundStatus
   int 66h         ; call DIGIPAK

   } // end inline assembly

// on exit AX will be used as the return value, if 1 then a sound is playing
// 0 if a sound is not playing

} // end Sound_Status

//////////////////////////////////////////////////////////////////////////////

void Sound_Stop(void)
{
// this function will stop a currently playing sound

_asm
   {
   mov ax, 068Fh   ; function 8: StopSound
   int 66h         ; call DIGIPAK

   } // end inline assembly

} // end Sound_Stop

//////////////////////////////////////////////////////////////////////////////

int Music_Load(char *filename, music_ptr the_music)
{
// this function will load a xmidi file from disk into memory and register it

unsigned char far *temp_ptr;   // temporary pointer used to load music
unsigned char far *xmidi_ptr;   // pointer to xmidi data

unsigned int segment,          // segment of music data memory
             paragraphs,       // number of 16 byte paragraphs music takes up
             bytes_read;       // used to track numbr of bytes read by DOS

long         size_of_file;     // the total size of the xmidi file in bytes

int xmidi_handle;              // DOS file handle

// open the extended xmidi file, use DOS file and memory allocation to make sure
// memory is on a 16 byte or paragraph boundary

if (_dos_open(filename, _O_RDONLY, &xmidi_handle)!=0)
   {
   printf("\nMusic System - Couldn't open %s",filename);
   return(0);

   } // end if file not found

// compute number of paragraphs that sound file needs

size_of_file = _filelength(xmidi_handle);

paragraphs = 1 + (size_of_file)/16;

// allocate the memory on a paragraph boundary

_dos_allocmem(paragraphs,&segment);

// point data pointer to allocated data area

_FP_SEG(xmidi_ptr) = segment;
_FP_OFF(xmidi_ptr) = 0;

// alias pointer to memory storage area

temp_ptr = xmidi_ptr;

// read in blocks of 16k until file is loaded

do
 {
 // load next block

 _dos_read(xmidi_handle,temp_ptr, 0x4000, &bytes_read);

 // adjust pointer

 temp_ptr += bytes_read;

 } while(bytes_read==0x4000);

// close the file

_dos_close(xmidi_handle);

// set up the music structure

the_music->buffer = xmidi_ptr;
the_music->size   = size_of_file;
the_music->status = 0;

// now register the xmidi file with MIDIPAK

if ((the_music->register_info = Music_Register(the_music))==XMIDI_UNREGISTERED)
   {
   // delete the memory

   Music_Unload(the_music);

   // return an error

   return(0);

   } // end if couldn't register xmidi file

// else return success

return(1);

} // end Music_Load

/////////////////////////////////////////////////////////////////////////////

void Music_Unload(music_ptr the_music)
{

// this functions deletes the xmidi file data from memory

_dos_freemem(_FP_SEG(the_music->buffer));

the_music->buffer=NULL;

} // end Music_Unload

///////////////////////////////////////////////////////////////////////////////

int Music_Register(music_ptr the_music)
{

// this function registers the xmidi music with MIDIPAK, so that it can be
// played

unsigned int xmid_off,   // offset and segment of midi file
             xmid_seg,
             length_low, // length of midi file in bytes
             length_hi;

// extract segment and offset of music buffer

xmid_off = _FP_OFF((the_music->buffer));
xmid_seg = _FP_SEG((the_music->buffer));

// extract the low word and high word of xmidi file length

length_low = the_music->size;
length_hi  = (the_music->size) >> 16;

// call MIDIPAK

_asm
   {
   push si           ; save si and di
   push di

   mov ax,704h       ; function #5: RegisterXmidi

   mov bx,xmid_off   ; offset of xmidi data

   mov cx,xmid_seg   ; segment of xmidi data

   mov si,length_low ; low word of xmidi length
   mov di,length_hi  ; hi word of xmidi length

   int 66h           ; call MIDIPAK
   pop di            ; restore si and di
   pop si

   } // end inline assembly

// return value will be in AX

} // end Music_Register

///////////////////////////////////////////////////////////////////////////////

void Music_Stop(void)
{
// this function will stop the song currently playing

_asm
   {
   mov ax,705h    ; function #6: MidiStop
   int 66h        ; call MIDIPAK

   } // end inline assembly

} // end Music_Stop

///////////////////////////////////////////////////////////////////////////////

int Music_Play(music_ptr the_music,int sequence)
{
// this function plays a xmidi file from memory

_asm
   {
   mov ax,702h        ; function #3: PlaySequence
   mov bx, sequence   ; which sequence to play 0,1,2....
   int 66h            ; call MIDIPAK

   } // end inline assembly

// return value is in AX, 1 success, 0 sequence not available

} // end Music_Play

///////////////////////////////////////////////////////////////////////////////

void Music_Resume(void)
{
// this function resumes a previously stopped xmidi sequence

_asm
   {
   mov ax,70Bh        ; function #12: ResumePlaying
   int 66h            ; call MIDIPAK

   } // end inline assembly

} // end Music_Resume

///////////////////////////////////////////////////////////////////////////////

int Music_Status(void)
{
// this function returns the status of a playing sequence

_asm
   {
   mov ax,70Ch        ; function #13: SequenceStatus
   int 66h            ; call MIDIPAK

   } // end inline assembly

// return value is in AX

} // end Music_Status

///////////////////////////////////////////////////////////////////////////////

