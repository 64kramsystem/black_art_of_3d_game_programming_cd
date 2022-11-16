
// DIGIDEMO.C - Digital sound demo

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

// D E F I N E S /////////////////////////////////////////////////////////////

// defines for the phrases

#define SOUND_WHAT       0
#define SOUND_WRONG      1
#define SOUND_CORRECT    2
#define SOUND_PLUS       3
#define SOUND_EQUAL      4

// G L O B A L S /////////////////////////////////////////////////////////////

sound ones[11];   // this will hold the digital samples for 1..9
sound teens[11];  // this will hold the digital samples for 11-19
sound tens[11];   // this will hold the digital samples for 10,20,30,40...100
sound phrases[6]; // this will hold the phrases

// F U N C T I O N S /////////////////////////////////////////////////////////

void Say_Number(int number)
{
// this function uses digitized samples to construct whole numbers
// note that the teens i.e. numbers from 11-19 have to be spoken as a
// special case and can't be concatenated as the numbers 20-100 can!

int ones_place,
    tens_place;

// compute place values, use simple logic, more complex logic can be
// derived that uses MODING and so forth, but better to see whats going
// on. However, the main point of this is to see the digital library
// in action, so focus on that aspect.

// test for 0..9

if (number<10)
   {
   Sound_Play((sound_ptr)&ones[number]);
   while(Sound_Status()==1);
   return;
   } // end if 0..9

// test for 11..19

if (number >= 11 && number <= 19)
   {
   Sound_Play((sound_ptr)&teens[number-11]);
   while(Sound_Status()==1);
   return;
   } // end if 11..19

// now break number down into tens and ones

tens_place = number / 10;
ones_place = number % 10;

// first say tens place

Sound_Play((sound_ptr)&tens[tens_place-1]);
while(Sound_Status()==1);

// now say ones place (if any)

if (ones_place)
   {
   Sound_Play((sound_ptr)&ones[ones_place]);
   while(Sound_Status()==1);
   } // end if

} // end Say_Number

// M A I N ///////////////////////////////////////////////////////////////////

void main(int argc, char **argv)
{

char filename[16]; // used to build up filename

int number,    // loop variables
    number_1,
    number_2,
    answer,
    done=0;    // exit flag

float num_problems=0,  // used to track performance of player
      num_correct=0;

// load in the samples for the ones

for (number=1; number<=9; number++)
    {
    // build the filename

    sprintf(filename,"N%d.VOC",number);
    printf("\nLoading file %s",filename);

    // load the sound

    Sound_Load(filename,(sound_ptr)&ones[number],1);

    } // end for ones

// load in the samples for the teens

for (number=11; number<=19; number++)
    {
    // build the filename

    sprintf(filename,"N%d.VOC",number);
    printf("\nLoading file %s",filename);

    // load the sound

    Sound_Load(filename,(sound_ptr)&teens[number-11],1);

    } // end for teens

// load in the samples for the tens

for (number=10; number<=100; number+=10)
    {
    // build the filename

    sprintf(filename,"N%d.VOC",number);
    printf("\nLoading file %s",filename);

    // load the sound

    Sound_Load(filename,(sound_ptr)&tens[-1+number/10],1);

    } // end for tens

// load the phrases

printf("\nLoading the phrases...");

Sound_Load("what.voc",   (sound_ptr)&phrases[SOUND_WHAT     ],1);
Sound_Load("wrong.voc",  (sound_ptr)&phrases[SOUND_WRONG    ],1);
Sound_Load("correct.voc",(sound_ptr)&phrases[SOUND_CORRECT  ],1);
Sound_Load("plus.voc",   (sound_ptr)&phrases[SOUND_PLUS     ],1);
Sound_Load("equal.voc",  (sound_ptr)&phrases[SOUND_EQUAL    ],1);

// main event loop, note this one is not real-time since it waits
// for user input!

printf("\n                       S P E A K    N    A D D  !!!");
printf("\n\n\nThis program will test your skills of addition while demonstrating");
printf("\nthe digital sound channel in action!");

printf("\n\nJust answer each addition problem. To exit type in 0.\n");
printf("\n\nPress any key to begin!!!\n\n");

getch();

// the main event loop

while(!done)
     {

     // select two random numbers to add, but make sure their sum is less
     // than or equal to 100

     do
       {

       number_1 = 1 + rand()%99;
       number_2 = 1 + rand()%99;

       } while (number_1 + number_2 > 100);

     // ask user question

     printf("\nWhat is ");

     Sound_Play((sound_ptr)&phrases[SOUND_WHAT]);
     while(Sound_Status()==1);

     printf("%d",number_1);
     Say_Number(number_1);
     printf(" + ");

     Sound_Play((sound_ptr)&phrases[SOUND_PLUS]);
     while(Sound_Status()==1);

     Time_Delay(15);

     printf("%d",number_2);
     Say_Number(number_2);
     printf(" = ?",number_2);

     Sound_Play((sound_ptr)&phrases[SOUND_EQUAL]);
     while(Sound_Status()==1);

     scanf("%d",&answer);

     // make sure user isn't exiting

     if (answer!=0)
        {
        num_problems++;

        // test if user is correct

        if (answer == (number_1 + number_2))
           {
           Sound_Play((sound_ptr)&phrases[SOUND_CORRECT]);
           while(Sound_Status()==1);
           num_correct++;
           }
        else
           {
           // oops wrong answer!

           Sound_Play((sound_ptr)&phrases[SOUND_WRONG]);
           while(Sound_Status()==1);

           Say_Number(number_1+number_2);
           Time_Delay(25);

           } // end else wrong

        } // end if
     else
        {
        done = 1;
        } // end else user is exiting

     } // end main event loop

// unload all the sounds

for (number=1; number<=9; number++)
    Sound_Unload((sound_ptr)&ones[number]);

for (number=11; number<=19; number++)
    Sound_Unload((sound_ptr)&teens[number]);

for (number=10; number<=100; number+=10)
    Sound_Unload((sound_ptr)&tens[-1+number/10]);

for (number=0; number<=4; number++)
    Sound_Unload((sound_ptr)&phrases[number]);

// tell user his/her statistics

printf("\nYou got %.0f percent of the problems correct.",100*num_correct/num_problems);

} // end main

