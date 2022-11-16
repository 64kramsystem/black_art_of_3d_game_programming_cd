
// TERM2.C - A menu driven modem terminal program.

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
#include "black5.h"

// G L O B A L S ////////////////////////////////////////////////////////////

// M A I N ////////////////////////////////////////////////////////////////////

void main(void)
{

char ch,          // input character
     number[64];  // the phone number that is dialed

int done    = 0,  // global exit flag
    sel,          // user menu input
    linked  = 0,  // has a connection been made
    result,       // result of modem commands
    com_port;     // the com port to open

printf("\nModem Terminal Communications Program Version 2.0\n\n");

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

// flush modem and serial port

Serial_Flush();

Initialize_Modem(NULL);

printf("\n");

// enter event loop

while(!done)
     {

     // if the a connection hasn't been made then enter into menu

     if (!linked)
        {
        printf("\nMain Menu\n");
        printf("\n1 - Make a Call");
        printf("\n2 - Wait for Call");
        printf("\n3 - Hang Up");
        printf("\n4 - Exit");
        printf("\n\nSelect ?");

        // get input from user

        scanf("%d",&sel);

        // what is user trying to do

        switch(sel)
              {

              case 1:  // make a call
                   {
                   printf("\nNumber to dial?");
                   scanf("%s",number);

                   // try and make the connection

                   result = Make_Connection(number);

                   // was the response code a connect message

                   if (result==MODEM_CONNECT_1200 || result==MODEM_CONNECT_2400 ||
                      result==MODEM_CONNECT || result==MODEM_CARRIER_2400)
                      {
                      linked=1;
                      printf("\nEntering Terminal Mode...\n");
                      }
                   else
                   if (result==MODEM_USER_ABORT)
                      {
                      printf("\nUser Aborted!\n");
                      Hang_Up();
                      Serial_Flush();
                      } // end else

                   } break;

              case 2:  // wait for call
                   {
                   printf("\nWaiting...");

                   // wait for a call

                   result = Wait_For_Connection();

                   // was the response code a connection

                   if (result==MODEM_CONNECT_1200 || result==MODEM_CONNECT_2400 ||
                      result==MODEM_CONNECT || result==MODEM_CARRIER_2400)
                      {
                      linked=1;
                      printf("\nEntering Terminal Mode...\n");
                      }
                   else
                   if (result==MODEM_USER_ABORT)
                      {
                      printf("\nUser Aborted!\n");
                      Hang_Up();
                      Serial_Flush();
                      } // end else

                   } break;

              case 3: // hang up
                   {
                   // drop DTR and flush serial buffer

                   Hang_Up();
                   Serial_Flush();

                   } break;

              case 4: // exit
                   {
                   // set global exit flag
                   done=1;

                   } break;

              default:break;

              } // end switch

        }  // end if not linked


        // once machines are linked, allow bi-directional communication

        while(linked==1)
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
                   linked=0;
                   Hang_Up();
                   Serial_Flush();
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
                   linked=0;
                   Hang_Up();
                   Serial_Flush();

                   } // end if remote close

                } // end if serial read

             } // end while linked

     } // end if not done

// close the connection and blaze

// break connection just in case

Hang_Up();

Serial_Close();

} // end main

