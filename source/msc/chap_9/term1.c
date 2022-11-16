
// TERM1.C - A simple serial communications program.

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

#include "black9.h"
#include "black3.h"

// G L O B A L S ////////////////////////////////////////////////////////////

// M A I N ////////////////////////////////////////////////////////////////////

void main(void)
{

char ch;          // input character

int done    = 0,  // global exit flag
    sel,          // user menu input
    com_port;     // the com port to open

printf("\nSerial Communications Program Version 1.0\n\n");

// ask the user for the com port

printf("\nWhich COM port is your modem attached to 1 or 2?");

scanf("%d",&com_port);

// depending on users selection open COM 1 or 2

if (com_port==1)
    Serial_Open(COM_1,SERIAL_BAUD_2400,SERIAL_PARITY_NONE |
                SERIAL_BITS_8 | SERIAL_STOP_1);
else
    Serial_Open(COM_2,SERIAL_BAUD_2400,SERIAL_PARITY_NONE |
                SERIAL_BITS_8 | SERIAL_STOP_1);

// main loop

// flush serial port

Serial_Flush();

printf("\n");

// enter event loop

while(!done)
     {
     // try and get a character from local machine

     if (kbhit())
        {
        // get the character from keyboard

        ch = getch();

        printf("%c",ch);

        // send the character to other machine

        Serial_Write(ch);

        // has user pressed ESC ? if so bail.

        if (ch==27)
           {
           Serial_Flush();
           done=1;
           } // end if esc

        // test for CR, if so add an line feed

        if (ch==13)
            printf("\n");

        } // end if kbhit

     // try and get a character from remote

     if (ch = Serial_Read())
        {

        // print the character to the screen

        printf("%c",ch);

        // if it's a carriage return add a line feed

        if (ch==13)
            printf("\n");

        // if a esc character is sent from remote then close down

        if (ch==27)
           {
           printf("\nRemote Machine Closing Connection.");
           Serial_Flush();
           done=1;

           } // end if remote close

        } // end if serial read

     } // end if not done

// close the connection and blaze

Serial_Close();

} // end main

