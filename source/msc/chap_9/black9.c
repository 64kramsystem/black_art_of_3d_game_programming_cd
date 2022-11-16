
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
#include "black5.h"
#include "black9.h"

// G L O B A L S /////////////////////////////////////////////////////////////

void (_interrupt _far *Old_Serial_ISR)();  // holds old com port interrupt handler

char serial_buffer[SERIAL_BUFF_SIZE];  // the receive buffer

int serial_end   = -1;                 // indexes into receive buffer
int serial_start = -1;
int serial_ch;
int char_ready=0;                   // current character and ready flag
int old_int_mask;                   // the old interrupt mask on the PIC
int open_port;                      // the currently open port
int serial_lock = 0;                // serial ISR semaphore so the buffer
                                    // isn't altered will it is being written
                                    // to by the ISR
// G L O B A L S ////////////////////////////////////////////////////////////

char *modem_strings[]={"OK",             // these are the standard Hayes
                       "CONNECT",        // response strings
                       "RING",
                       "NO CARRIER",
                       "ERROR",
                       "CONNECT 1200",
                       "NO DIALTONE",
                       "BUSY",
                       "NO ANSWER",
                       "CONNECT 0600",
                       "CONNECT 2400",

                       "CARRIER 2400",   // experimental response strings
                       "CONNECT 9600",
                       "CONNECT 4800"};

//////////////////////////////////////////////////////////////////////////////

void _interrupt _far Serial_ISR(void)
{
// this is the serial ISR that is installed. It is called whenever a character
// is received. The received character is then placed into the next position
// in the input ring buffer

_asm sti

serial_ch = _inp(open_port + SERIAL_RBF);

// wrap buffer index around

if (++serial_end > SERIAL_BUFF_SIZE-1)
    serial_end = 0;

// move character into buffer

serial_buffer[serial_end] = serial_ch;

++char_ready;

// restore PIC

_outp(PIC_ICR,0x20);

} // end Serial_ISR

//////////////////////////////////////////////////////////////////////////////

int Serial_Ready(void)
{

// this functions returns true if there are any characters waiting and 0 if
// the buffer is empty

return(char_ready);

} // end Serial_Ready

//////////////////////////////////////////////////////////////////////////////

int Serial_Read(void)
{

// this function reads a character from the circulating buffer and returns it
// to the caller

int ch;

// test if there is a character(s) ready in buffer

if (serial_end != serial_start)
   {

   // wrap buffer index if needed

   if (++serial_start > SERIAL_BUFF_SIZE-1)
       serial_start = 0;

   // get the character out of buffer

   ch = serial_buffer[serial_start];

   // one less character in buffer now

   if (char_ready > 0)
       --char_ready;

   // send data back to caller

   return(ch);

   } // end if a character is in buffer
else
   // buffer was empty return a NULL i.e. 0
   return(0);

} // end Serial_Read


//////////////////////////////////////////////////////////////////////////////

void Serial_Write(char ch)
{

// this function writes a character to the transmit buffer, but first it
// waits for the transmit buffer to be empty.  note: it is not interrupt
// driven and it turns of interrupts while it's working

// wait for transmit buffer to be empty

while(!(_inp(open_port + SERIAL_LSR) & 0x20)){}

// turn off interrupts for a bit

_asm cli

// send the character

_outp(open_port + SERIAL_THR, ch);

// turn interrupts back on

_asm sti

} // end Serial_Write

//////////////////////////////////////////////////////////////////////////////

void Serial_Print(char *string,int cr)
{
// this function is used to print a string to the serial port

int index,      // looping variable
    length;     // used for length of string

length = strlen(string);

// write each character

for (index=0; index<length; index++)
    Serial_Write(string[index]);

// send a carriage return if requested

if (cr)
    Serial_Write(13);

} // end Serial_Print

//////////////////////////////////////////////////////////////////////////////

int Serial_Open(int port_base, int baud, int configuration)
{

// this function will open up the serial port, set it's configuration, turn
// on all the little flags and bits to make interrupts happen and load the
// ISR

unsigned char data;

// save the port I/O address for other functions

open_port = port_base;

// first set the baud rate

// turn on divisor latch registers

_outp(port_base + SERIAL_LCR, SERIAL_DIV_LATCH_ON);

// send low and high bytes to divsor latches

_outp(port_base + SERIAL_DLL, baud);
_outp(port_base + SERIAL_DLH, 0);

// set the configuration for the port

_outp(port_base + SERIAL_LCR, configuration);

// enable the interrupts

data = _inp(port_base + SERIAL_MCR);
data = SET_BITS(data,SERIAL_GP02);
_outp(port_base + SERIAL_MCR, data);

_outp(port_base + SERIAL_IER, 1);

// hold off on enabling PIC until we have the ISR installed

if (port_base == COM_1)
   {
   Old_Serial_ISR = _dos_getvect(INT_SERIAL_PORT_0);
   _dos_setvect(INT_SERIAL_PORT_0, Serial_ISR);

   }
else
   {
   Old_Serial_ISR = _dos_getvect(INT_SERIAL_PORT_1);
   _dos_setvect(INT_SERIAL_PORT_1, Serial_ISR);
   }

// enable the recieve character interrupt on PIC for selected comm port

old_int_mask = _inp(PIC_IMR);

_outp(PIC_IMR, (port_base==COM_1) ? (old_int_mask & 0xEF) : (old_int_mask & 0xF7 ));

return(1);

} // Serial_Open

//////////////////////////////////////////////////////////////////////////////

int Serial_Close(void)
{

// this function closes the port which entails turning off interrupts and
// restoring the old interrupt vector

unsigned char data;

// disable the comm port interrupts

data = _inp(open_port + SERIAL_MCR);
data = RESET_BITS(data,SERIAL_GP02);

_outp(open_port + SERIAL_MCR, data);

_outp(open_port + SERIAL_IER, 0);
_outp(PIC_IMR, old_int_mask );

// replace old comm port isr

if (open_port == COM_1)
   {
   _dos_setvect(INT_SERIAL_PORT_0, Old_Serial_ISR);
   }
else
   {
   _dos_setvect(INT_SERIAL_PORT_1, Old_Serial_ISR);
   }

return(1);

} // end Serial_Close

//////////////////////////////////////////////////////////////////////////////

void Serial_Flush(void)
{
// this function flushes out the serial buffer

int index; // looping index

// read up to 32 characters

for (index=0; index<32; index++)
    {
    Serial_Read();
    Time_Delay(1);
    } // end for index

} // end Serial_Flush

//////////////////////////////////////////////////////////////////////////////

void Modem_Control(int command)
{
// this function is used to control specific aspects of the modem hardware

unsigned char data;

// which command is being issued?

switch(command)
      {
      case MODEM_DTR_ON:
           {
           // read modem control register

           data = _inp(open_port + SERIAL_MCR);
           data = SET_BITS(data,1);
           _outp(open_port + SERIAL_MCR,data);

           } break;

      case MODEM_DTR_OFF:
           {
           // read modem control register

           data = _inp(open_port + SERIAL_MCR);
           data = RESET_BITS(data,1);
           _outp(open_port + SERIAL_MCR,data);

           } break;

      default: break;

      } // end switch

// wait a sec for it to take effect

Time_Delay(DELAY_1_SECOND);

} // end Modem_Control

////////////////////////////////////////////////////////////////////////////////

int Initialize_Modem(char *extra_init)
{
// this function will initialize the modem and prepare it for use
// if your modem has some specific sequence that must be sent to reset
// it then send it in the extra string, otherwise set extra equal to NULL

int result;

// send reset command

Modem_Send_Command("ATZ");

result = Modem_Result(NULL,0);

if (result!=MODEM_OK)
    return(result);

// allow dtr line to be controlled

//Modem_Send_Command("AT&D2");

//result = Modem_Result(NULL,0);

//if (result!=MODEM_OK)
//    return(result);

// place modem in direct asynchronous mode

Modem_Send_Command("AT&Q0");

result = Modem_Result(NULL,0);

if (result!=MODEM_OK)
    return(result);

// send hardware specific modem command

if (extra_init && strlen(extra_init)>=2)
   {

   Modem_Send_Command(extra_init);

   result=Modem_Result(NULL,0);

   } // end if

return(result);

} // end Initialize_Modem

///////////////////////////////////////////////////////////////////////////////

void Modem_Send_Command(char *buffer)
{
// this function sends a command string to the modem

int index,    // looping variable
    length;   // length of command

// write the string out

length = strlen(buffer);

for (index=0; index<length; index++)
    Serial_Write(buffer[index]);

// printf("\nSending:%s",buffer);

// send a carriage return

Serial_Write(13);

Time_Delay(DELAY_1_SECOND);

} // end Modem_Send_Command

//////////////////////////////////////////////////////////////////////////////

int Serial_Read_Wait(void)
{
// this function waits for a character to be ready and then returns it

while(!Serial_Ready());

// return the character

return(Serial_Read());

} // end Serial_Read_Wait

///////////////////////////////////////////////////////////////////////////////

int Modem_Result(char *output,int exit_enable)
{
// this function is a bit messy (as are all parsing functions), it is used
// to rtrieve a response from the mode, however, it will disregard any
// echoed commands, also, the last parameter exit_enable is used as a flag
// to allow the keyboard to force an exit, this is useful in a situation
// such as waiting for an answer or waiting to be called, in any case, this
// give the function an exit avenue, the exit is enabled by pressing a key
// on the keyboard, however, the keyboard handler must be installed for
// this to work!

int index=0,        // looping variable
    start=0;

char ch,          // woring character
     buffer[64];  // working buffer

// hunt for start of response

while(1)
     {
     // is a character ready?

     if (Serial_Ready())
        {
        if (Serial_Read()==10)
           break;

        } // end if a character is ready

     // test if user is trying to abort

     if (exit_enable && (kbhit() || keys_active))
        return(MODEM_USER_ABORT);

     } // end while

// read the response

while(1)
     {
     // is a character ready?

     if (Serial_Ready())
        {
        ch = Serial_Read();

        if (ch==10)
           break;

        buffer[index++] = ch;

        } // end if a character is ready

     // test if user is trying to abort

     if (exit_enable && (kbhit() || keys_active))
        return(MODEM_USER_ABORT);

     } // end while

// terminate response

buffer[index-1] = 0;

// printf("\nReceived:%s",buffer);

// copy the result into the output string

if (output!=NULL)
   strcpy(output,buffer);

// compute which response has been given

for (index=0; index<NUM_MODEM_RESPONSES; index++)
    {
    // test the response to next response string

    if (strcmp(modem_strings[index],buffer)==0)
       return(index);

    } // end for index

// there must be some kind of error

return(MODEM_ERROR);

} // end Modem_Result

///////////////////////////////////////////////////////////////////////////////

int Make_Connection(char *number)
{
// this function calls up the sent phone number and returns true if the
// connection was made or false if it wasn't

int result;
char command[64];

// flush serial buffers

Serial_Flush();

// enable the DTR line

Modem_Control(MODEM_DTR_ON);

Time_Delay(DELAY_1_SECOND);

// dial number

sprintf(command,"ATDT%s",number);

// make the call

Modem_Send_Command(command);

result = Modem_Result(NULL,1);

// test the result

if (result==MODEM_CONNECT      ||
    result==MODEM_CONNECT_1200 ||
    result==MODEM_CONNECT_2400)
    {
    // a successful connection has been made

    return(result);

    } // end if connection made
else
   {
   // there must be a problem, hang up the phone

   Hang_Up();

   // return the error

   return(result);

   } // end else

} // end Make_Connection

////////////////////////////////////////////////////////////////////////////////

int Wait_For_Connection(void)
{
// this function will wait for a connection to be made and return true
// when this occurs. the function will return false if a connection isn't
// made or in a specific amount of time or if a key is pressed

int result;

char command[64];

// flush serial buffers

Serial_Flush();

// make sure modem is hung up

Hang_Up();

Modem_Control(MODEM_DTR_ON);

// wait for phone to ring...

result =  Modem_Result(NULL,1);

// was that a ring?

if (result == MODEM_RING)
   {

   // tell modem to answer

   Modem_Send_Command("ATA");

   result = Modem_Result(NULL,1);

   // test the result

   if (result==MODEM_CONNECT ||
       result==MODEM_CONNECT_1200 ||
       result==MODEM_CONNECT_2400)
       {
       // a successful connection has been made

       return(result);

       } // end if connection made
   else
      {
      // there must be a problem, hang up the phone

      Hang_Up();

      // return the error

      return(result);

      } // end else

   } // end if ringing

} // end Wait_For_Connection

///////////////////////////////////////////////////////////////////////////////

int Hang_Up(void)
{
// this function hangs up the phone and places the modem back into it's command
// state

int result;

// drop dtr line

Modem_Control(MODEM_DTR_OFF);

return(MODEM_OK);

} // end Hang_Up

