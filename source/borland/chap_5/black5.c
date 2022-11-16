
// BLACK5.C - Library Module

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

#include "black5.h"

// G L O B A L S  ////////////////////////////////////////////////////////////

void (_interrupt _far *Old_Keyboard_ISR)();  // holds old keyboard interrupt handler

int raw_scan_code=0;  // the global keyboard scan code

// this holds the keyboard state table which tracks the state of every key
// on the keyboard

int keyboard_state[128];

// this tracks the number of keys that are currently pressed, helps
// with keyboard testing logic

int keys_active = 0;

// these values hold the maximum, minimum and neutral joystick values for
// both joysticks

unsigned int joystick_1_max_x,
             joystick_1_max_y,
             joystick_1_min_x,
             joystick_1_min_y,
             joystick_1_neutral_x,
             joystick_1_neutral_y,

             joystick_2_max_x,
             joystick_2_max_y,
             joystick_2_min_x,
             joystick_2_min_y,
             joystick_2_neutral_x,
             joystick_2_neutral_y;

// F U N C T I O N S /////////////////////////////////////////////////////////

unsigned char Get_Key(void)
{
// test if a key press has been buffered by system and if so, return the ASCII
// value of the key, otherwise return 0

if (_bios_keybrd(_KEYBRD_READY))
 return((unsigned char)_bios_keybrd(_KEYBRD_READ));
else return(0);

} // end Get_Key

//////////////////////////////////////////////////////////////////////////////

unsigned int Get_Shift_State(unsigned int mask)
{
// return the shift state of the keyboard logically ANDed with the sent mask

return(mask & _bios_keybrd(_KEYBRD_SHIFTSTATUS));

} // end Get_Shift_State

//////////////////////////////////////////////////////////////////////////////

unsigned char Get_Scan_Code(void)
{
// use BIOS functions to retrieve the scan code of the last pressed key
// if a key was pressed at all, otherwise return 0

_asm   {
	mov ah,01h      // function #1 which is "key ready"
	int 16h         // call the BIOS keyboard interrupt
	jz buffer_empty // if there was no key ready then exit
	mov ah,00h      // function #0: retrieve raw scan code
	int 16h         // call the BIOS keyboard interrupt
	mov al,ah       // result is placed by BIOS in ah, copy it to al
	xor ah,ah       // zero out ah
	jmp done        // jump to end so ax doesn't get reset

buffer_empty:
	  xor ax,ax     // a key was retrieved so write a 0 into ax to reflect this
done:

    } // end asm

// 8 or 16 bit data is returned in AX, hence no need to explicitly say return()

} // end Get_Scan_Code

//////////////////////////////////////////////////////////////////////////////

void _interrupt _far Keyboard_Driver()
{
// this function is used as the new keyboard driver. once it is installed
// it will continually update a keyboard state table that has an entry for
// every key on the keyboard, when a key is down the appropriate entry will be
// set to 1, when released the entry will be reset. any key can be querried by
// accessing the keyboard_state[] table with the make code of the key as the
// index

// need to use assembly for speed since this is an interrupt

_asm   {
	sti                    // re-enable interrupts
	in al, KEY_BUFFER      // get the key that was pressed from the keyboard
	xor ah,ah              // zero out upper 8 bits of AX
	mov raw_scan_code, ax  // store the key in global variable
	in al, KEY_CONTROL     // set the control register to reflect key was read
	or al, 82h             // set the proper bits to reset the keyboard flip flop
	out KEY_CONTROL,al     // send the new data back to the control register
	and al,7fh
	out KEY_CONTROL,al     // complete the reset
	mov al,20h             // reset command
	out PIC_PORT,al        // tell PIC to re-enable interrupts

	} // end inline assembly

// update the keyboard table

// test if the scan code was a make code or a break code

if (raw_scan_code <128)
   {
   // index into table and set this key to the "on" state if it already isn't
   // on

   if (keyboard_state[raw_scan_code]==KEY_UP)
      {
      // there is one more active key

      keys_active++;

      // update the state table

      keyboard_state[raw_scan_code] = KEY_DOWN;

      } // end if key wasn't already pressed

   } // end if a make code
else
   {
   // must be a break code, therefore turn the key "off"
   // note that 128 must be subtracted from the raw key to access the correct
   // element of the state table

   if (keyboard_state[raw_scan_code-128]==KEY_DOWN)
      {
      // there is one less active key

      keys_active--;

      // update the state table

		keyboard_state[raw_scan_code-128] = KEY_UP;

      } // end if key wasn't already pressed

   } // end else

} // end Keyboard_Driver

///////////////////////////////////////////////////////////////////////////////

void Keyboard_Install_Driver(void)
{
// this function installs the new keyboard driver

int index; // used as loop variable

// clear out the keyboard state table

for (index=0; index<128; index++)
    keyboard_state[index]=0;

// save the old keyboard driver

Old_Keyboard_ISR = _dos_getvect(KEYBOARD_INTERRUPT);

// install the new keyboard driver

_dos_setvect(KEYBOARD_INTERRUPT, Keyboard_Driver);

} // end Keyboard_Install_Driver

///////////////////////////////////////////////////////////////////////////////

void Keyboard_Remove_Driver(void)
{

// this functions restores the old keyboard driver (DOS version) with the
// previously saved vector

_dos_setvect(KEYBOARD_INTERRUPT, Old_Keyboard_ISR);

} // end Keyboard_Remove_Driver

//////////////////////////////////////////////////////////////////////////////

unsigned char Joystick_Buttons(unsigned char button)
{

// this function reads the state of the joystick buttons by retrieving the
// appropriate bit in the joystick port

outp(JOYPORT,0); // clear the joystick port and request a sample

// invert buttons then mask with request so that a button that is pressed
// returns a "1" instead of a "0"

return( (unsigned char)(~inp(JOYPORT) & button));

} // end Joystick_Buttons

//////////////////////////////////////////////////////////////////////////////

unsigned int Joystick(unsigned char stick)
{
// this function will read a joystick by starting the timing circuits connected
// to each joystick pot, when the timing circuit has charged the joystick
// bit will revert to 0, the time this process takes is proportional to
// the joystick position and is returned as the result

 _asm	 {
	 cli                    // disable interupts for timing purposes

	 mov ah, byte ptr stick // select joystick to read with bitmask
	 xor al,al              // zero out al
	 xor cx,cx              // clear cx i.e. set it to 0
	 mov dx,JOYPORT         // point dx to the joystick port
	 out dx,al              // start the 555 timers charging
charged:
	 in al,dx               // read the joystick port and test of the bit
	 test al,ah             // has reverted back to 0
	 loopne charged         // if the joystick circuit isn't charged then
									// decrement cx and loop


	 xor ax,ax              // zero out ax
	 sub ax,cx              // subtract cx from ax to get a number that increases
									// as the joystick position in moved away from neutral

	 sti                    // re-enable interrupts

	 } // end asm

// ax has the 16 result, so no need for an explicit return

} // end Joystick

//////////////////////////////////////////////////////////////////////////////

unsigned int Joystick_Bios(unsigned char stick)
{
// read the joystick using bios interrupt 15h with the joystick function 84h

union REGS inregs, outregs; // used to hold CPU registers

inregs.h.ah = 0x84; // joystick function 84h
inregs.x.dx = 0x01; // read joysticks subfunction 1h

// call the BIOS joystick interrupt

int86(0x15,&inregs, &outregs);

// return requested joystick

switch(stick)
      {
      case JOYSTICK_1_X:  // ax has joystick 1's X axis
           {
           return(outregs.x.ax);
           } break;

      case JOYSTICK_1_Y:  // bx has joystick 1's Y axis
           {
           return(outregs.x.bx);
           } break;

      case JOYSTICK_2_X:  // cx has joystick 2's X axis
           {
           return(outregs.x.cx);
           } break;

		case JOYSTICK_2_Y:  // dx has joystick 2's Y axis
           {
           return(outregs.x.dx);
           } break;

      default:break;

      } // end switch stick

} // end Joystick_Bios

//////////////////////////////////////////////////////////////////////////////

void Joystick_Calibrate(int stick,int method)
{

// this function is used to calibrate the a joystick. the function will
// query the user to move the joystick in circlular motion and then release
// the stick back to the neutral position and press the fire button. using
// this information the function will compute the max,min and neutral
// values for both the X and Y axis of the joystick. these values will
// then be stored in global variables so they can be used by other
// functions, note the function can use either the BIOS joystick call
// or the low level one we made

unsigned int x_value, // used to read values of X and Y axis in real-time
             y_value;

// which stick does caller want to calibrate?

if (stick==JOYSTICK_1)
   {
   printf("\nCalibrating Joystick #1: Move the joystick in a circle, then");
   printf("\nplace the stick into it's neutral position and press fire.");

   // set calibrations values to extremes

   joystick_1_max_x = 0;
   joystick_1_max_y = 0;
   joystick_1_min_x = 32000;
	joystick_1_min_y = 32000;

   // process X and Y values in real time

   while(!Joystick_Buttons(JOYSTICK_BUTTON_1_1 | JOYSTICK_BUTTON_1_2))
        {
        // get the new values and try to update calibration

        // test if user wants to use bios or low level

        if (method==USE_BIOS)
           {
           x_value = Joystick_Bios(JOYSTICK_1_X);
           y_value = Joystick_Bios(JOYSTICK_1_Y);
           }
        else
           {
           x_value = Joystick(JOYSTICK_1_X);
           y_value = Joystick(JOYSTICK_1_Y);
           }
		  // update globals with new extremes

        // process X - axis

        if (x_value >= joystick_1_max_x)
            joystick_1_max_x = x_value;

        if (x_value <= joystick_1_min_x)
            joystick_1_min_x = x_value;

        // process Y - axis

        if (y_value >= joystick_1_max_y)
            joystick_1_max_y = y_value;

        if (y_value <= joystick_1_min_y)
            joystick_1_min_y = y_value;

        } // end while

		  // stick is now in neutral position so record the values here also

        joystick_1_neutral_x = x_value;
        joystick_1_neutral_y = y_value;

   // notify user process is done

   printf("\nJoystick #1 Calibrated. Press the fire button to exit.");

   while(Joystick_Buttons(JOYSTICK_BUTTON_1_1 | JOYSTICK_BUTTON_1_2));
   while(!Joystick_Buttons(JOYSTICK_BUTTON_1_1 | JOYSTICK_BUTTON_1_2));

   } // end calibrate joystick #1

else
if (stick==JOYSTICK_2)
   {
   printf("\nCalibrating Joystick #1: Move the joystick in a circle, then");
   printf("\nplace the stick into it's neutral position and press fire.");

	// set calibrations values to extremes

   joystick_2_max_x = 0;
   joystick_2_max_y = 0;
   joystick_2_min_x = 32000;
   joystick_2_min_y = 32000;

   // process X and Y axis values in real time

   while(!Joystick_Buttons(JOYSTICK_BUTTON_2_1 | JOYSTICK_BUTTON_2_2))
        {
        // get the new values and try to update calibration

        // test if user wants to use bios or low level

        if (method==USE_BIOS)
           {
           x_value = Joystick_Bios(JOYSTICK_1_X);
           y_value = Joystick_Bios(JOYSTICK_1_Y);
           }
		  else
           {
           x_value = Joystick(JOYSTICK_1_X);
           y_value = Joystick(JOYSTICK_1_Y);
           }

        // update globals with new extremes

        // process X - axis

        if (x_value >= joystick_2_max_x)
            joystick_2_max_x = x_value;
        else
        if (x_value <= joystick_2_min_x)
            joystick_2_min_x = x_value;

        // process Y - axis

        if (y_value >= joystick_2_max_y)
            joystick_2_max_y = y_value;
		  else
        if (y_value <= joystick_2_min_y)
            joystick_2_min_y = y_value;

        } // end while

        // stick is now in neutral position so record the values here also

        joystick_2_neutral_x = x_value;
        joystick_2_neutral_y = y_value;

   // notify user process is done

   printf("\nJoystick #2 Calibrated. Press the fire button to exit.");

   while(Joystick_Buttons(JOYSTICK_BUTTON_1_1 | JOYSTICK_BUTTON_1_2));
   while(!Joystick_Buttons(JOYSTICK_BUTTON_1_1 | JOYSTICK_BUTTON_1_2));

   } // end calibrate joystick #2

} // end Joystick_Calibrate

//////////////////////////////////////////////////////////////////////////////

int Joystick_Available(int stick_num)
{
// test if the joystick is plugged in that the user is requesting tested
// note the use of the BIOS joystick function, it is very reliable

if (stick_num == JOYSTICK_1)
   {
   // test if joystick 1 is plugged in by testing the port values
   // they will be 0,0 if there is no stick

   return(Joystick_Bios(JOYSTICK_1_X)+Joystick_Bios(JOYSTICK_1_Y));

   } // end if joystick 1
else
   {
   // test if joystick 2 is plugged in by testing the port values
	// they will be 0,0 if there is no stick

   return(Joystick_Bios(JOYSTICK_2_X)+Joystick_Bios(JOYSTICK_2_Y));

   } // end else joystick 2

} // end Joystick_Available

///////////////////////////////////////////////////////////////////////////////

int Mouse_Control(int command, int *x, int *y,int *buttons)
{
union REGS inregs,  // CPU register unions to be used by interrupts
            outregs;

// what is caller asking function to do?

switch(command)
      {

		case MOUSE_RESET: // this resets the mouse
           {

           // mouse subfunction 0: reset

           inregs.x.ax = 0x00;

           // call the mouse interrupt

           int86(MOUSE_INTERRUPT, &inregs, &outregs);

           // return number of buttons on this mouse

           *buttons = outregs.x.bx;

           // return success/failure of function

           return(outregs.x.ax);

           } break;

      case MOUSE_SHOW: // this shows the mouse
           {
           // this function increments the internal mouse visibility counter.
           // when it is equal to 0 then the mouse will be displayed.

           // mouse subfunction 1: increment show flag

           inregs.x.ax = 0x01;

           // call the mouse interrupt

           int86(MOUSE_INTERRUPT, &inregs, &outregs);

           // return success always

           return(1);

           } break;

		case MOUSE_HIDE:  // this hides the mouse
           {
           // this function decrements the internal mouse visibility counter.
           // when it is equal to -1 then the mouse will be hidden.

           // mouse subfunction 2: decrement show flag

           inregs.x.ax = 0x02;

           // call the interrupt

           int86(MOUSE_INTERRUPT, &inregs, &outregs);

           // return success

           return(1);

           } break;

      case MOUSE_POSITION_BUTTONS: // this gets both the position and
											  // state of buttons
           {
           // this functions computes the absolute position of the mouse
           // and the state of the mouse buttons

           // mouse subfunction 3: get position and buttons

           inregs.x.ax = 0x03;

           // call the mouse interrupt

           int86(MOUSE_INTERRUPT, &inregs, &outregs);

           // extract the info and send back to caller via pointers

           *x       = outregs.x.cx;
           *y       = outregs.x.dx;
           *buttons = outregs.x.bx;

           // return success always

           return(1);

           } break;

      case MOUSE_MOTION_REL:  // this gets the relative motion of mouse
           {

           // this functions gets the relative mouse motions from the last
           // call, these values will range from -32768 to +32767 and
           // be in mickeys which are 1/200 of inch or 1/400 of inch
           // depending on the resolution of your mouse

           // subfunction 11: get relative motion

           inregs.x.ax = 0x0B;

           // call the interrupt

           int86(MOUSE_INTERRUPT, &inregs, &outregs);

           // extract the info and send back to caller via pointers

           *x       = outregs.x.cx;
           *y       = outregs.x.dx;

           // return success

           return(1);

           } break;

      case MOUSE_SET_SENSITIVITY:
           {
           // subfunction 26: set sensitivity

           inregs.x.ax = 0x1A;

           // place the desired sensitivity and double speed values in place

			  inregs.x.bx = *x;
           inregs.x.cx = *y;
           inregs.x.dx = *buttons;

           // call the interrupt

			  int86(MOUSE_INTERRUPT, &inregs, &outregs);

           // always return success

           return(1);

           } break;

      default:break;

      } // end switch

} // end Mouse_Control

