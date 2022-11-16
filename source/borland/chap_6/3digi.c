
// 3DIGI.C - A simple 3-D sound demo using the parallel ports and D/A
// convertors

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
#include "black6.h"

// D E F I N E S /////////////////////////////////////////////////////////////

// the parallel port offsets

#define PAR_1_PORT     0
#define PAR_2_PORT     1
#define PAR_3_PORT     2
#define PAR_4_PORT     3

// G L O B A L S ////////////////////////////////////////////////////////////

// pointer to base parallel port

unsigned int far *par_port = (unsigned int far*)0x00000408L;

// the parallel ports to be used for right and left

int right_3D = PAR_1_PORT,
	 left_3D  = PAR_2_PORT;

// the amount of power going to each channel (i.e. the volume)

float power_3D = .5; // each channel is at 50%, this will range 0..1

// F U N C T I O N S ////////////////////////////////////////////////////////

void Par_Write(int port,unsigned char data)
{

outp(*(par_port+port),data);

} // end Par_Write

///////////////////////////////////////////////////////////////////////////////

void Sound_Play_3D(_sound_ptr the_sound,float power_right, float power_left,int speed)
{

// this function will play a digitized sound through each of the parallel
// ports. Each channels output is controlled by the input powers. Note that
// a local replica of the sound is made so that the attenuated version
// can be computed in real-time without destroying the original

unsigned int freq,      // used to slow or speed up playback rate
				 index,     // looping variable
				 size;      // size of sound data

unsigned char far *right_buffer; // working buffers
unsigned char far *left_buffer;

// create new sound based on right and left power

size   = the_sound->SS.sndlen;

right_buffer = (unsigned char far *)farmalloc(size);
left_buffer  = (unsigned char far *)farmalloc(size);

// create the new sounds, one for each channel

for (index=0; index<size; index++)
    {

    // compute proper value based on power level

    right_buffer[index] =
             (unsigned char)((float)the_sound->SS.sound[index] * power_right);

	 left_buffer[index] =
				 (unsigned char)((float)the_sound->SS.sound[index] * power_left);

    } // end for index

	  // play the sound

	  for (index=0; index<size; index++)
			{

			// delay a bit to slow frequency down

			for (freq=0; freq<speed; freq++)
				 {
				 // write left channel

				 Par_Write(left_3D,left_buffer[index]);

				 // write right channel

				 Par_Write(right_3D,right_buffer[index]);

				 } // end time delay frequency control

			} // end for index

// release the memory

farfree((void far *)right_buffer);
farfree((void far *)left_buffer);

} // end Sound_Play_3D

// M A I N ///////////////////////////////////////////////////////////////////

void main(int argc, char **argv)
{

unsigned int index,    // looping variable
				 done=0,   // exit flag
				 delay=5;

_sound effect_r,effect_l;  // the sound effects

float power_delta = .1;

// load the sound test sound effect into memory without translation

if (!Sound_Load("3dleft.voc",(_sound_ptr)&effect_l,0))
	{
	printf("\nCouldn't load test sound 3DLEFT.VOC");
   return;

   } // end if

if (!Sound_Load("3dright.voc",(_sound_ptr)&effect_r,0))
   {
   printf("\nCouldn't load test sound 3DRIGHT.VOC");
   return;

   } // end if


// display menu

printf("\n3D DIGITAL SOUND DEMO\n");
printf("\nUse the <S> and <F> keys to slow down and speed up the sound.");
printf("\nPress <Q> to exit.\n");

// enter event loop

while(!done)
	  {

	  // test which size sound source is on

     if (power_3D > .5)
        Sound_Play_3D((_sound_ptr)&effect_r,power_3D, (1-power_3D),delay);
     else
        Sound_Play_3D((_sound_ptr)&effect_l,power_3D, (1-power_3D),delay);

     // pan sound from right to left left to right
     // make you dizzy!

	  power_3D+=power_delta;

     if (power_3D > 1 || power_3D < 0)
        {
		  power_delta=-power_delta;

        power_3D+=power_delta;

        } // end of reverse pan

     // test if user is hitting keyboard

     if (kbhit())
        {
        // what does user want to do

        switch(getch())
              {

              case 'f': // speed up the sound
                   {
                   if (--delay < 1)
                      delay=1;

                   } break;

              case 's': // slow down the sound
						 {
                   ++delay;

                   } break;

              case 'q': // exit
                   {
                   done=1;
                   } break;

              } // end switch

        } // end if kbhit

     } // end while

// unload the sounds

Sound_Unload((_sound_ptr)&effect_l);
Sound_Unload((_sound_ptr)&effect_r);

} // end main



