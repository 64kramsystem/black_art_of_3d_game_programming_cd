
// MIDIDEMO.C - MIDI Music Demo

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

// include our the sound library

#include "black3.h"
#include "black6.h"

// M A I N ///////////////////////////////////////////////////////////////////

void main(int argc, char **argv)
{

char filename[16];  // the .XMI extended midi filename

int done=0,    // main loop exit flag
    loaded=0,  // tracks if a file has been loaded
    sequence,  // sequence of XMI extended midi file to be played
    select;    // the users input

music song;    // the music structure

// main event loop

while(!done)
     {
     // print out menu

     printf("\n\nExtended MIDI Menu\n");
     printf("\n1. Load Extended MIDI file.");
     printf("\n2. Play Sequence.");
     printf("\n3. Stop Sequence.");
     printf("\n4. Resume Sequence.");
     printf("\n5. Unload Extended MIDI File from Memory.");
     printf("\n6. Print Status.");
     printf("\n7. Exit Program.");

     printf("\n\nSelect One ?");

     // get input

     scanf("%d",&select);

     // what does user want to do?

     switch(select)
           {
           case 1:  // Load Extended MIDI file
                {
                printf("\nEnter Filename of song ?");
                scanf("%s",filename);

                // if a song is already loaded then delete it

                if (loaded)
                   {
                   // stop music and unload sound

                   Music_Stop();
                   Music_Unload((music_ptr)&song);

                   } // end if loaded

                // load the new file

                if (Music_Load(filename,(music_ptr)&song))
                   {
                   printf("\nMusic file successfully loaded...");

                   // flag that a file is loaded

                   loaded = 1;

                   }
                else
                   {
                   // error

                   printf("\nSorry, the file %s couldn't be loaded!",filename);

                   } // end else

                } break;

           case 2: // Play Sequence
                {
                // make sure a midi file has been loaded

                if (loaded)
                   {
                   printf("\nWhich Sequence 0..n ?");
                   scanf("%d",&sequence);

                   // play the requested sequence

                   Music_Play((music_ptr)&song,sequence);

                   } // end if loaded
                else
                   printf("\nYou must first load an extended MIDI file.");

                } break;

           case 3: // Stop Sequence
                {
                // make sure a midi file has been loaded

                if (loaded)
                    Music_Stop();
                else
                   printf("\nYou must first load an extended MIDI file.");

                } break;

           case 4: // Resume Sequence
                {
                // make sure a midi file has been loaded

                if (loaded)
                    Music_Resume();
                else
                   printf("\nYou must first load an extended MIDI file.");

                } break;

           case 5: // Unload Extended MIDI File from Memory
                {
                // make sure a midi file has been loaded

                if (loaded)
                   {
                   Music_Stop();
                   Music_Unload((music_ptr)&song);
                   loaded=0;
                   }
                else
                   printf("\nYou must first load an extended MIDI file.");

                } break;

           case 6: // Print Status
                {

                printf("\nMIDIPAK Status = %d",Music_Status());

                } break;

           case 7: // Exit Program
                {
                // delete music and stop

                if (loaded)
                   {
                   Music_Stop();
                   Music_Unload((music_ptr)&song);
                   loaded=0;
                   }

                done=1;

                } break;

           default:
                  {
                  printf("\nInvalid Selection!");
                  } break;

           } // end switch

     } // end while

// unload file if there is one

if (loaded)
   Music_Unload((music_ptr)&song);

} // end main

