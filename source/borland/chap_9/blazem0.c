
// STARBLAZER - Version 1.0

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

// include all of our stuff

#include "black3.h"
#include "black4.h"
#include "black5.h"
#include "black6.h"
#include "black8.h"
#include "black9.h"

#include "blazem0.h"

// G L O B A L S  ////////////////////////////////////////////////////////////

pcx_picture image_pcx,  // general PCX image used to load background and imagery
            image_controls;  // this holds the controls screen


RGB_color color_1,color_2; // used for temporaries during color rotation

RGB_palette game_palette;  // this will hold the startup system palette
                           // so we can restore it after screen FX

sprite button_1,        // the setup buttons
       button_2,
       button_3,
       displays,        // the display bitmaps
       starburst,       // the startburt sprite
       gryfon_r,        // this holds a generic gryfon ship with remote colors
       raptor_r,        // this holds a generic raptor ship with remote colors
       gryfon_l,        // this holds a generic gryfon ship with local colors
       raptor_l,        // this holds a generic raptor ship with local colors
       players_ship,    // this is the players ship
       remotes_ship,    // this is the remotes ship
       wormhole,        // a stationary worm hole in the game
       fuel_cells[NUM_FUEL_CELLS], // the stationary fuel cells in the game

       heads_text,      // heads up display sprites
       heads_numbers,
       heads_gauge;


monster alien;                     // the alien saucer that comes out of the
                                   // wormhole

sprite explosions[NUM_EXPLOSIONS]; // the explosions in the game

nova novas[NUM_NOVAS];             // the super nova explosions!

int active_explosions=0;           // number of active explosions

asteroid asteroids[NUM_ASTEROIDS]; // the asteroid fields

star stars[NUM_STARS];             // the star fields

particle missiles[NUM_MISSILES];   // all the projectiles

bitmap tech_font[NUM_TECH_FONT];   // the tech font bitmaps

int game_state = GAME_SETUP;  // the overall state of the game

int comm_port         = 0,    // currently selected comm port
    players_ship_type = 0,    // currently selected local ship type
    remotes_ship_type = 1;    // the remotes ships type


int comm_port_to_address[2] = {COM_1,COM_2}; // converts the selected com port
                                            // to an address

int master            = 1,    // the the player dials up a player
    slave             = 0,    // then he is master else he is slave
    linked            = 0,    // state of the modem communications system
    winner = WINNER_NONE;     // the winner of the game

// the start up arrays used to differentiate the player and remote

// master is index 1, slave is index 0

int game_start_x[] = {1200,1300};
int game_start_y[] = {1200,1200};

// the variables for the player

int players_last_x      = 0,    // the last postiion of player
    players_last_y      = 0,
    players_x           = 0,    // current players position
    players_y           = 0,
    players_dx          = 0,       // players position deltas since last frame
    players_dy          = 0,
    players_xv          = 0,       // velocity of ship
    players_yv          = 0,
    players_engine      = 0,       // state of engines
    players_stability   = 8,       // how long it takes for gravity
    players_flame_count = 0,       // used for engine flicker
    players_flame_time  = 1,
    players_gravity     = 0,       // current gravitron count
    players_shields     = 0,       // state of the shields
    players_shield_time = 0,       // how long sheilds have been on
    players_cloak       = -1,      // state of the shields  -1 off 1 on
    players_heads       = -1,      // state of heads up display
    players_comm        = -1,      // state of comm link
    players_scanner     = -1,      // space scanner
    players_num_ships   = 3,       // number of players ship
    players_shield_strength = 22000, // the amount of energy players shields have
    players_energy          = 22000, // the amount of ship energy
    players_score           = 0,     // the score of the player
    players_active_missiles = 0,     // the number of missiles the player has fired
    players_state           = ALIVE, // satte of player
    players_death_count     = 0;     // how long death sequence will last


int debounce_hud        = 0,       // these are used to debounce the players
    debounce_scan       = 0,       // inputs one some keys
    debounce_cloak      = 0,
    debounce_thrust     = 0,
    debounce_fire       = 0,
    debounce_shields    = 0;

int refresh_heads = 0,              // used to track when hud needs refreshing
    under_players_blip,             // these hold the pixels under the scanner
    under_remotes_blip;             // blip image of the player and remote

// remote variables

int remotes_last_x      = 0,    // the last postiion of player
    remotes_last_y      = 0,
    remotes_x           = 0,    // current remotes position
    remotes_y           = 0,
    remotes_dx          = 0,       // remotes position deltas since last frame
    remotes_dy          = 0,
    remotes_xv          = 0,       // velocity of ship
    remotes_yv          = 0,
    remotes_engine      = 0,       // state of engines
    remotes_stability   = 8,       // how long it takes for gravity
    remotes_flame_count = 0,       // used for engine flicker
    remotes_flame_time  = 1,
    remotes_gravity     = 0,       // current gravitron count
    remotes_shields     = 0,       // state of the shields
    remotes_shield_time = 0,       // how long sheilds have been on
    remotes_cloak       = -1,      // state of the shields  -1 off 1 on
    remotes_heads       = -1,      // state of heads up display
    remotes_comm        = -1,      // comm link
    remotes_scanner     = -1,      // space scanner
    remotes_num_ships   = 3,       // number of remotes ship
    remotes_shield_strength = 22000, // the amount of energy remotes shields have
    remotes_energy          = 22000, // the amount of ship energy
    remotes_score           = 0,     // the remotes score
    remotes_active_missiles = 0,     // the number of missiles the remote has fired
    remotes_state           = ALIVE, // state of remote
    remotes_death_count     = 0;     // how long death sequence will last

// unit motion look up tables

int motion_dx[16] = {0,1,2,2,3,2,2,1,0,-1,-2,-2,-3,-2,-2,-1};
int motion_dy[16] = {-3,-2,-2,-1,0,1,2,2,3,2,2,1,0,-1,-2,-2};

// musical sequence information

int music_enabled   = 0,      // flags that enable music and sound FX
    digital_enabled = 0;

int digital_FX_priority = 10; // the priority tracker of the current effect

int intro_sequence[] = {0,2,3,4,3,4,2,3,4,3,3};
int intro_seq_index  = 0;

int game_sequence[] = {0+5,1+5,4+5,5+5,4+5,2+5,1+5,3+5,1+5,5+5,5+5,4+5,1+5,
                       2+5,3+5,2+5,4+5,4+5};

int game_seq_index  = 0;

music song;    // the music structure

// sound fx stuff

_sound digital_FX[NUM_SOUND_FX];

// these colors are used for the shields on the ships

RGB_color  primary_red   = {63,0,0},    // pure red
           primary_blue  = {0,0,63},    // pure blue
           primary_green = {0,63,0},    // pure green
           primary_black = {0,0,0},     // pure black
           primary_white = {63,63,63},  // pure white

           remotes_shield_color={0,0,0}, // the current color of the remotes shield
           players_shield_color={0,0,0}, // the current color of the players shields

           players_engine_color={0,0,0}, // the color or the players engine
			  remotes_engine_color={0,0,0}; // the color or the remotes engine

// the instruction pages

char *instructions[]={

               "STARBLAZER MISSION BRIEFING,       ",
               "STAR DATE: 2595.001                ",
               "                                   ",
               "LONG RANGE SCANS HAVE DETECTED THE ",
               "PRESENCE OF AN UNKNOWN VESSEL IN   ",
               "THE TALLEON ASTEROID BELT. YOUR    ",
               "MISSION IS SIMPLE: SEEK OUT THE    ",
               "AGGRESSOR AND DESTROY IT.          ",
               "                                   ",
               "THE ASTEROID BELT IS FAIRLY SPARSE ",
               "AND POSES MINIMAL THREAT TO YOUR   ",
               "SHIP, BUT THE ASTEROIDS ARE DEADLY ",
               "IF THEY BREACH YOUR SHIPS HULL.    ",
               "                                   ",
               "                                   ",
               "                                   ",
               "               1                   ",
               "THE ASTEROID BELT EXISTS AT        ",
               "COORDINATES (0.0) TO (2500.2500),  ",
               "(USING THE STANDARD CENTRAL        ",
               "NAVIGATION MULTIVARIATE VECTOR     ",
               "POSITIONING SYSTEM). IF YOU TRY TO ",
               "GO BEYOND THESE BOUNDS YOUR SHIP   ",
               "WILL AUTOMATICALLY HYPERWARP TO    ",
               "THE OPPOSITE EDGE OF THE SECTOR.   ",
               "THUS,YOU CAN NOT LEAVE UNTIL YOU   ",
               "HAVE COMPLETED YOUR MISSION!       ",
               "                                   ",
               "THERE HAVE ALSO BEEN REPORTS OF A  ",
               "STABLE WORMHOLE IN THIS SECTOR. THE",
               "WORMHOLE SEEMS TO BE A SINGLE POLE ",
               "TRANSVERSE DISTURBANCE AND EMITS NO",
               "GRAVITRONS.                        ",
               "               2                   ",
               "BUT REPORTS DO DETIAL MATTER AND   ",
               "ENERGY EMMISIONS FROM THE WORMHOLE,",
               "SO STAY CLEAR OF IT! IT IS LOCATED ",
               "AT COORDINATES (1200.1200).        ",
               "                                   ",
               "FINALLY, A SMALL SHIP HAS BEEN     ",
               "DETECTED,HOWEVER,IT SEEMS TO HAVE  ",
               "NO HOSTILE INTENTIONS. MOREOVER,IT ",
               "APPEARS THAT IT HAS A SYMETRICAL   ",
               "SUB-SPACE FIELD SHIELDING SYSTEM   ",
               "WHICH OUR BEST SCIENTISTS STILL DO ",
               "NOT FULLY UNDERSTAND. NEEDLESS TO  ",
               "SAY, YOUR WEAPONS ARE USELESS      ",
               "AGAINST IT...                      ",
               "                                   ",
               "                                   ",
               "               3                   ",
               "SPECIFICATIONS                     ",
               "                                   ",
               "YOUR SHIP IS EQUIPPED WITH ALL OF  ",
               "THE LATEST FEATURES INCLUDING:     ",
               "HEADS UP DISPLAYS, SECTOR SCANNER, ",
               "TWIN TACHION DRIVE, AUTOFIRE       ",
               "PARTICLE CANNONS, PLASMA-FIELD-    ",
               "EFFECT SUB-SPACE SHIELDS,AND A     ",
               "CLOAKING DEVICE BUILT IN THE       ",
               "ANTARUS SYSTEM SPECIFICALLY FOR    ",
               "YOUR SHIP.                         ",
               "                                   ",
               "FINALLY, YOUR SHIP HAS THE NEW     ",
               "PROTOTYPE SHAPE SHIFTING PLASMA    ",
               "ENERGY HULL SYSTEM.                ",
               "                                   ",
               "               4                   ",
               "ENTERING INTO COMBAT               ",
               "                                   ",
               "TO PLAY THE GAME YOU CAN EITHER    ",
               "PLAY IN THE SOLO MODE FOR PRACTICE ",
               "OR MODEM-2-MODEM. TO PLAY SOLO     ",
               "MODE,SELECT THE SHIP YOU LIKE BY   ",
               "USING THE SELECT SHIP OPTION ON    ",
               "THE MAIN MENU. ONCE YOU HAVE       ",
               "SELECTED THE SHIP OF YOUR CHOICE,  ",
               "USE THE PLAY SOLO OPTION AND YOU   ",
               "WILL BE WARPED TO THE TALLEON      ",
               "ASTEROID BELT FOR A PRACTICE       ",
               "SESSION.                           ",
               "                                   ",
               "                                   ",
               "                                   ",
               "               5                   ",
               "TO PLAY MODEM-2-MODEM, YOU CAN     ",
               "EITHER DIAL UP A COMPETITOR OR WAIT",
               "FOR A COMPETITOR TO CALL. HOWEVER, ",
               "BEFORE YOU CAN DO THIS, YOU MUST   ",
               "SELECT WHICH COMMUNICATIONS PORT   ",
               "YOUR MODEM IS ON. THIS CAN BE DONE ",
               "WITH THE SELECT COMM PORT MENU     ",
               "OPTION.                            ",
               "                                   ",
               "ONCE YOU HAVE SELECTED THE COMM    ",
               "PORT THEN EITHER DIAL UP A         ",
               "COMPETITOR WITH THE MAKE CONNECTION",
               "MENU ITEM OR WAIT FOR A CALL VIA   ",
               "THE WAIT FOR CONNECTION MENU ITEM. ",
               "ONCE A CONNECTION HAS BEEN MADE YOU",
               "WILL BE WARPED INTO BATTLE.        ",
               "               6                   ",
               "CONTROLING THE SHIP                ",
               "                                   ",
               "NAVIGATION:                        ",
               "                                   ",
               "TO NAVIAGATE THE SHIP USE THE RIGHT",
               "AND LEFT ARROWS TO TURN. TO THRUST ",
               "FOWARD USE THE UP ARROW.           ",
               "                                   ",
               "SHIELDS:                           ",
               "                                   ",
               "TO ENGAGE YOUR SHIELDS PRESS THE   ",
               "<ALT> KEY. THEY WILL LAST FOR      ",
               "APPROXIMATELY 5 SECS BEFORE        ",
               "SHUTTING DOWN.                     ",
               "                                   ",
               "                                   ",
               "               7                   ",
               "CLOAKING DEVICE:                   ",
               "                                   ",
               "THE CLOAKING DEVICE CAN BE TOGGLED ",
               "BY PRESSING THE <C> KEY. CLOAKING  ",
               "EXHIBITS HIGH POWER CONSUMPTION AND",
               "YOUR SHIELDS ARE USELESS WHEN      ",
               "CLOAKED.                           ",
               "                                   ",
               "HEADS UP DISPLAY:                  ",
               "                                   ",
               "THE HEADS UP DISPLAY CAN BE TOGGLED",
               "WITH THE <H> KEY. IT DISPLAYS      ",
               "PERTAINENT TACTICAL INFORMATION IN ",
               "REAL-TIME.                         ",
               "                                   ",
               "                                   ",
               "               8                   ",
               "LONG RANGE SCANNERS:               ",
               "                                   ",
               "YOUR SHIP IS EQUIPPED WITH A LONG  ",
               "RANGE SECTOR SCAN THAT WILL DISPLAY",
               "ALL OBJECTS IN THE SECTOR. YOUR    ",
               "SHIP IS MARKED GREEN, THE ENEMY IS ",
               "MARKED RED. ALL OTHER OBJECTS ARE  ",
               "BLUE. ENGAGE AND DISENGAGE THE     ",
               "SCANNER BY PRESSING THE <S> KEY.   ",
               "                                   ",
               "WEAPONS:                           ",
               "                                   ",
               "TO FIRE YOUR CANNONS PRESS THE     ",
               "<SPACE> KEY. THEY SUPPORT THE ATF  ",
               "OPTION(ALL SHIPS BUILT AFTER       ",
               "2491.300 SUPPORT RAPID FIRE).      ",
               "               9                   ",
               "EXITING THE GAME:                  ",
               "                                   ",
               "THE GAME CAN BE EXITED BY PRESSING ",
               "<ESC> AT ANY TIME. IF THERE IS A   ",
               "CONNECTIONM IT WILL BE TERMINATED. ",
               "AFTER EXITING THE GAME, YOU WILL BE",
               "SENT BACK TO THE MAIN MENU SYSTEM. ",
               "                                   ",
               "WARNINGS:                          ",
               "                                   ",
               "YOUR SHIP CONSUMES A GREAT DEAL OF ",
               "POWER SINCE IT IS A FIGHTER. YOU   ",
               "CAN MINIMIZE THIS POWER CONSUMPTION",
               "BY USING YOUR WEAPONS,THRUSTER,AND ",
               "CLOAKING DEVICE SPARINGLY.         ",
               "                                   ",
               "               10                  ",
               "YOUR SHIELDS OPERATE BY A FUSION   ",
               "REACTION OF TRI-RUBIDIUM, THUS ONCE",
               "YOUR SHIELD STRENGTH IS DEPLETED,  ",
               "YOUR SHIELDS ARE INOPERABLE!       ",
               "                                   ",
               "THE ORIGINAL REPORTS OF THE SECTOR ",
               "NOTE LARGE ENERGY FLUX SOURCES     ",
               "FLOATING AROUND FROM TIME TO TIME. ",
               "IT MAY BE POSSIBLE TO EXTRACT THE  ",
               "ENERGY FROM THESE ENERGY PODS.     ",
               "                                   ",
               "                                   ",
               "                                   ",
               "                                   ",
               "                                   ",
               "                                   ",
               "               11                  ",
               "TO HELP YOUR SHIP FACILITATE THIS, ",
               "AN EXTERNAL ENERGION ACCEPTOR      ",
               "MATRIX HAS BEEN FITTED TO THE HULL ",
               "OF YOUR SHIP. YOU MAY BE ABLE TO   ",
               "ABSORB THE ENERGY OF THE PODS BY   ",
               "COMING IN CONTACT WITH THEM, BUT   ",
               "THIS IS THEORETICAL AND YOU DO SO  ",
               "AT YOUR OWN RISK.                  ",
               "                                   ",
               "GOOD HUNTING                       ",
               "MAY THE WIND BE AT YOUR BACK...    ",
               "                                   ",
               "                                   ",
               "                                   ",
               "                                   ",
               "                                   ",
               "          END BREIFING             "};

// these were in the main

int index,             // looping variable
    sel,               // used for input
    px_window,         // starting upper left hand corner of players view port
    py_window,
    result,            // result from comm system
    response,          // result from comm system
    players_key_state, // state of players input
    remotes_key_state, // state pf remotes input

    sent_right =0,     // diagnostic counters to track number of
    sent_left  =0,     // of input instructions both sent and received
    sent_up    =0,
    rec_right  =0,
    rec_left   =0,
    rec_up     =0;

unsigned char seed;    // a random number seed

char buffer[64],            // general buffer
     modem_ini_string[64],  // used for extra modem intialization string
     number[32],            // used to print strings
     ch;                    // used for keyboard input

// F U N C T I O N S ////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////

void Setup_Interface(void)
{

// this event loop is for the setup phase

if (keys_active>0)
   {

   // what is user trying to do

       if (keyboard_state[MAKE_UP])
              {
              // erase the button and move it up
              Sprite_Erase((sprite_ptr)&button_1,video_buffer);

              button_1.y-=12;

              // test if we need to wrap around bottom

              if (--button_1.counter_1 < 0)
                 {
                 button_1.counter_1 = 6;
                 button_1.y         = 63+6*12;
                 } // end if wrap around

              // scan and draw button
              Sprite_Under((sprite_ptr)&button_1,video_buffer);
              Sprite_Draw((sprite_ptr)&button_1,video_buffer,1);

              Digital_FX_Play(BLZKEY_VOC,2);

              Time_Delay(1);

              } // end if up

       else
       if (keyboard_state[MAKE_DOWN])
              {
              // erase the button and move it down
              Sprite_Erase((sprite_ptr)&button_1,video_buffer);

              button_1.y+=12;

              // test if we need to wrap around top

              if (++button_1.counter_1 > 6)
                 {
                 button_1.counter_1 = 0;
                 button_1.y         = 63;
                 } // end if wrap around

              // scan and draw button
              Sprite_Under((sprite_ptr)&button_1,video_buffer);
              Sprite_Draw((sprite_ptr)&button_1,video_buffer,1);

              Digital_FX_Play(BLZKEY_VOC,2);

              Time_Delay(1);

              } // end if down
        else
        if (keyboard_state[MAKE_ENTER] )
              {

              // illuminate button for a second

              // draw button down

              button_1.curr_frame = 1;
              Sprite_Draw((sprite_ptr)&button_1,video_buffer,1);

              Digital_FX_Play(BLZKEY_VOC,2);

              Time_Delay(5);

              // now draw button up

              button_1.curr_frame = 0;
              Sprite_Draw((sprite_ptr)&button_1,video_buffer,1);

              // test which item is being selected

              switch(button_1.counter_1)
                    {
                    case SETUP_PLAY_SOLO:
                         {

                         Clear_Display(0);

                         Font_Engine_1(DISPLAY_X+2,DISPLAY_Y+2,0,0,
                                       "ENTERING ARENA",
                                       video_buffer);

                         // move to running state

                         game_state = GAME_RUNNING;

                         Time_Delay(5);

                         } break;

                    case SETUP_MAKE_CONNECTION:
                         {

                         Clear_Display(0);

                         Font_Engine_1(DISPLAY_X+2,DISPLAY_Y+2,0,0,
                                       "ENTER NUMBER",
                                       video_buffer);




                         // remove the keyboard handler,
                         // makes user input easier for strings

                         Keyboard_Remove_Driver();

                         result = Get_Line(number);

                         // replace our handler

                         Keyboard_Install_Driver();

                         // get the number

                         if (!result)
                            {

                            Clear_Display(0);

                            Font_Engine_1(DISPLAY_X+2,DISPLAY_Y+2,0,0,
                                          "ABORTED...",
                                          video_buffer);

                            Digital_FX_Play(BLZABRT_VOC,1);

                            Time_Delay(5);

                            Clear_Display(0);

                            break;

                            } // end if aborted input

                         // make a connection

                         // open com port

                         Modem_Control(MODEM_DTR_ON);

                         Serial_Open(comm_port_to_address[comm_port] ,SERIAL_BAUD_2400,SERIAL_PARITY_NONE |
                                     SERIAL_BITS_8 | SERIAL_STOP_1);

                         Serial_Flush();

                         // initialize modem with standard initialization
                         // plus any added init string in blaze.mod

                         Initialize_Modem(modem_ini_string);

                         // let user know whats going on

                         Clear_Display(0);

                         Font_Engine_1(DISPLAY_X+2,DISPLAY_Y+2,0,0,
                                       "DIALING:",
                                       video_buffer);


                         Font_Engine_1(DISPLAY_X+2,DISPLAY_Y+2+8,0,0,
                                       number,
                                       video_buffer);

                         Digital_FX_Play(BLZDIAL_VOC,1);

                         result = Make_Connection(number);

                         if (result==MODEM_CONNECT_1200 ||
                             result==MODEM_CONNECT_2400 ||
                             result==MODEM_CONNECT      ||
                             result==MODEM_CARRIER_2400)
                            {
                            // set game to linked and local to
                            // master

                            linked=1;
                            master=1;
                            slave=0;

                            game_state = GAME_LINKING;

                            Clear_Display(0);

                            Font_Engine_1(DISPLAY_X+2,DISPLAY_Y+2,0,0,
                                          "CONNECTED!",
                                          video_buffer);

                            Time_Delay(5);

                            Clear_Display(0);

                            } //end if connection made
                         else
                         if (result==MODEM_USER_ABORT)
                            {
                            // close down serial connection

                            Hang_Up();
                            Serial_Flush();
                            Serial_Close();

                            // print that selection was aborted

                            Clear_Display(0);

                            Font_Engine_1(DISPLAY_X+2,DISPLAY_Y+2,0,0,
                                          "ABORTED...",
                                          video_buffer);

                            Digital_FX_Play(BLZABRT_VOC,1);

                            Time_Delay(5);

                            Clear_Display(0);

                            } // end if user aborted
                         else
                            {
                            // close down serial connection

                            Hang_Up();
                            Serial_Flush();
                            Serial_Close();

                            // print that selection was aborted

                            Clear_Display(0);

                            Font_Engine_1(DISPLAY_X+2,DISPLAY_Y+2,0,0,
                                          "COMM PROBLEM",
                                          video_buffer);

                            Time_Delay(5);

                            Clear_Display(0);

                            } // end if user aborted
                         } break;

                    case SETUP_WAIT_FOR_CONNECTION:
                         {

                         Clear_Display(0);

                         Font_Engine_1(DISPLAY_X+2,DISPLAY_Y+2,0,0,
                                       "ANSWER MODE",
                                       video_buffer);

                         Font_Engine_1(DISPLAY_X+2,DISPLAY_Y+2+8,0,0,
                                       "ENABLED...",
                                       video_buffer);

                         // open com port

                         Modem_Control(MODEM_DTR_ON);

                         Serial_Open(comm_port_to_address[comm_port] ,SERIAL_BAUD_2400,SERIAL_PARITY_NONE |
                                     SERIAL_BITS_8 | SERIAL_STOP_1);

                         Serial_Flush();

                         // wait for a call


                         // initialize modem with standard initialization
                         // plus any added init string in blaze.mod

                         Initialize_Modem(modem_ini_string);

                         result = Wait_For_Connection();

                         // is the connection message 2400?

                         if (result==MODEM_CONNECT_1200 ||
                             result==MODEM_CONNECT_2400 ||
                             result==MODEM_CONNECT      ||
                             result==MODEM_CARRIER_2400)
                            {
                            // set game state to linked and local
                            // to slave

                            linked=1;
                            master = 0;
                            slave = 1;

                            game_state = GAME_LINKING;

                            Clear_Display(0);

                            Font_Engine_1(DISPLAY_X+2,DISPLAY_Y+2,0,0,
                                          "CONNECTED!",
                                          video_buffer);

                            Time_Delay(5);

                            Clear_Display(0);

                            } // end if connection made
                         else
                         if (result==MODEM_USER_ABORT)
                            {
                            // user bailed so close down serial

                            Hang_Up();
                            Serial_Flush();
                            Serial_Close();

                            Clear_Display(0);

                            Font_Engine_1(DISPLAY_X+2,DISPLAY_Y+2,0,0,
                                          "ABORTED...",
                                          video_buffer);

                            // play sound fx

                            Digital_FX_Play(BLZABRT_VOC,1);

                            Time_Delay(5);

                            Clear_Display(0);

                            } // end if user aborted
                         else
                            {
                            // close down serial connection

                            Hang_Up();
                            Serial_Flush();
                            Serial_Close();

                            // print that selection was aborted

                            Clear_Display(0);

                            Font_Engine_1(DISPLAY_X+2,DISPLAY_Y+2,0,0,
                                          "COMM PROBLEM",
                                          video_buffer);

                            Time_Delay(5);

                            Clear_Display(0);

                            } // end if user aborted

                         } break;

                    case SETUP_SELECT_SHIP:
                         {

                         Clear_Display(0);

                         Font_Engine_1(DISPLAY_X+2,DISPLAY_Y+2,0,0,
                                       "GRYFON  RAPTOR",
                                       video_buffer);

                         // draw ships

                         displays.curr_frame = DISPLAY_IMG_SHIPS;

                         Sprite_Draw((sprite_ptr)&displays,video_buffer,1);

                         // let user select one

                         sel = Display_Select(players_ship_type);

                         // test if user changed ship

                         if (sel!=-1)
                            {
                            players_ship_type = sel;

                            // set up sprite

                            if (players_ship_type==GRYFON_SHIP)
                                {
                                Copy_Frames((sprite_ptr)&players_ship,(sprite_ptr)&gryfon_l);
                                }
                            else
                                {
                                Copy_Frames((sprite_ptr)&players_ship,(sprite_ptr)&raptor_l);

                                }
                            } // end if

                         } break;

                    case SETUP_SET_COMM_PORT:
                         {

                         Clear_Display(0);

                         Font_Engine_1(DISPLAY_X+2,DISPLAY_Y+2,0,0,
                                       "COMM 1  COMM 2",
                                       video_buffer);

                         // draw comm ports

                         displays.curr_frame = DISPLAY_IMG_PORTS;

                         Sprite_Draw((sprite_ptr)&displays,video_buffer,1);

                         // let user select one

                         sel = Display_Select(comm_port);

                         // test if user changed selection

                         if (sel!=-1)
                            {
                            // change comm port

                            comm_port=sel;

                            } // end if


                         } break;

                    case SETUP_BRIEFING:
                         {
                         // show user instructions

                         Digital_FX_Play(BLZMISS_VOC ,1);

                         Intro_Briefing();

                         // redisplay intro controls

                         Intro_Controls();

                         } break;

                    case SETUP_EXIT:
                         {
                         // set state to game over

                         game_state=GAME_OVER;

                         Clear_Display(0);

                         Font_Engine_1(DISPLAY_X+2,DISPLAY_Y+2,0,0,
                                       "EXITING SYSTEM",
                                       video_buffer);

                         } break;

                    default:break;

                    } // end switch

              } // end if enter

   } // end if keyboard hit

} // end Setup_Interface


// M A I N //////////////////////////////////////////////////////////////////

void main(int argc, char **argv)
{

// the main controls most of the platyer and remote logic, normally we would
// probably move most of the code into functions, but for instructional purposes
// this is easier to follow, believe me there are already enough function calls
// to make your head spin!

// parse the command line and set up configuration

Parse_Commands(argc,argv);

// get the modem initialization string

Get_Modem_String(modem_ini_string);

printf("\nExtra modem initialization string = %s",modem_ini_string);

Time_Delay(25);

// start up game

Start_Up();

// main event loop ///////////////////////////////////////////////////////////

while(game_state!=GAME_OVER)
     {
     // test the overall game state

     if (game_state==GAME_SETUP)
        {
        // if machines were linked then break connection

        if (linked)
           {
           // close down serial connection

           Hang_Up();
           Serial_Flush();
           Serial_Close();

           // reset linked flag

           linked = 0;

           } // end if

        // user in in the setup state

        Intro_Controls();

        // restore pallete

        Write_Palette(0,255,(RGB_palette_ptr)&game_palette);

        // enter setup event loop

        while(game_state==GAME_SETUP)
             {

             // perform setup input querying and preocessing

             Setup_Interface();

             // perform special effects to control panel

             Panel_FX();

             // slow things down a bit

             Time_Delay(1);

             // check on music

             if (music_enabled)
                {
                // test if piece is complete or has been stopped

                if (Music_Status()==2 || Music_Status()==0)
                   {
                   // advance to next sequence

                   if (++intro_seq_index==11)
                      intro_seq_index=0;

                   Music_Play((music_ptr)&song,intro_sequence[intro_seq_index]);

                   } // end if

                } // end if music enabled

             } // end setup event loop

        } // end if in control state

///////////////////////////////////////////////////////////////////////////////
     else
     if (game_state==GAME_LINKING)
        {

        // wait a second to machine sure otehr machine is linked

        Time_Delay(DELAY_2_SECOND);

        // test if this machine is master or slave and negotiate
        // connection

        if (master)
           {

           // send master signal

           Serial_Write('M');

           // send ship type

           Serial_Write('0'+players_ship_type);

           // this is a crucial function call, both machines must use the same
           // random sequence, this is accomplised by the master sending a seed
           // to the remote to be used as the seed for the random number
           // generator

           // send random seed

           seed = (unsigned char)Timer_Query();

           Serial_Write(seed);

           // seed local random number generator

           srand(seed);


           // wait for acknowledge

           response = Serial_Read_Wait();

           if (response=='S')
              {

              // get remotes ship type

              response = Serial_Read_Wait();

              remotes_ship_type = response - '0';

              // move game state into running mode

              game_state=GAME_RUNNING;

              } // end if linked

           } // end if master
        else
        if (slave)
           {
           // send master signal

           Serial_Write('S');

           // send ship type

           Serial_Write('0'+players_ship_type);

           // wait for a master signal

           // wait for acknowledge

           response = Serial_Read_Wait();

           if (response=='M')
              {
              // get remotes ship type

              response = Serial_Read_Wait();

              remotes_ship_type = response - '0';

              // the next byte is the random number generator seed

              seed = (unsigned char)Serial_Read_Wait();

              srand(seed);


              // move game state into running mode

              game_state=GAME_RUNNING;

              } // end if linked

           } // end if slave

        } // end if game linking

////////////////////////////////////////////////////////////////////////////////

     else
     if (game_state==GAME_RUNNING)
        {

        // restore pallete

        Write_Palette(0,255,(RGB_palette_ptr)&game_palette);

        // turn shields off

        Shield_Control(THE_PLAYER,0);
        Shield_Control(THE_REMOTE,0);

        // reset system variables

        Reset_System();

        // select engine colors

        if (players_ship_type==GRYFON_SHIP)
           players_engine_color = primary_white;
        else
           players_engine_color = primary_green;

        if (remotes_ship_type==GRYFON_SHIP)
           remotes_engine_color = primary_white;
        else
           remotes_engine_color = primary_green;

        // set up remote ships ship type

        if (remotes_ship_type==GRYFON_SHIP)
            Copy_Frames((sprite_ptr)&remotes_ship,(sprite_ptr)&gryfon_r);
        else
            Copy_Frames((sprite_ptr)&remotes_ship,(sprite_ptr)&raptor_r);

        // restart everything

        Init_Wormhole();
        Init_Stars();
        Init_Missiles();
        Init_Asteroids(16,6,4);
        Init_Explosions();
        Init_Novas();
        Init_Alien();
        Init_Heads();
        Init_Fuel_Cells();

        // start music

        if (music_enabled)
           {
           Music_Stop();

           game_seq_index=0;

           Music_Play((music_ptr)&song,game_sequence[game_seq_index]);

           } // end if music enabled

        Digital_FX_Play(BLZENTR_VOC,1);

        // clear double buffer

        Fill_Double_Buffer(0);

        // scan under all objects

        Under_Wormhole();
        Under_Asteroids();
        Under_Stars();
        Under_Fuel_Cells();

// BEGIN PLAYER ///////////////////////////////////////////////////////////////

        Sprite_Under((sprite_ptr)&players_ship,double_buffer);

// END PLAYER ///////////////////////////////////////////////////////////////

// BEGIN REMOTE ///////////////////////////////////////////////////////////////

        px_window = players_x - 160+11;
        py_window = players_y - 100+9;

        remotes_ship.x = remotes_x - px_window;
        remotes_ship.y = remotes_y - py_window;

        Sprite_Under_Clip((sprite_ptr)&remotes_ship,double_buffer);

// END REMOTE ///////////////////////////////////////////////////////////////

        // enter into the main game loop

        while(game_state==GAME_RUNNING)
             {

             // compute starting time of this frame

             starting_time = Timer_Query();

             // reset all vars

             refresh_heads=0;

             // flag engines off

             players_engine = 0;
             remotes_engine = 0;

             // erase all objects

             Erase_Fuel_Cells();
             Erase_Alien();
             Erase_Wormhole();
             Erase_Stars();
             Erase_Asteroids();
             Erase_Missiles();
             Erase_Explosions();
             Erase_Novas();

             // erase scanner data

             if (players_scanner==1)
                Erase_Blips();

// BEGIN PLAYER//////////////////////////////////////////////////////////////////


             Sprite_Erase((sprite_ptr)&players_ship,double_buffer);


// END PLAYER///////////////////////////////////////////////////////////////////

// BEGIN REMOTE//////////////////////////////////////////////////////////////////


             Sprite_Erase_Clip((sprite_ptr)&remotes_ship,double_buffer);


// END REMOTE///////////////////////////////////////////////////////////////////


// BEGIN PLAYER//////////////////////////////////////////////////////////////////

             // move player

             players_last_x = players_x;
             players_last_y = players_y;

             // test if a key is depressed

             players_key_state = 0;

             if (players_state==ALIVE)
             {
             if (keys_active > 0)
                {
                // which key ?

                if (keyboard_state[MAKE_LEFT] )
                     {
                     // rotate left

                     if (--players_ship.curr_frame < 0)
                        players_ship.curr_frame = 15;

                     // add this action to key state

                     players_key_state += REMOTE_LEFT;

#if DEBUG
                     sent_left++;
#endif

                     } // end if left
                else
                if (keyboard_state[MAKE_RIGHT])
                     {
                     // rotate right

                     if (++players_ship.curr_frame > 15)
                        players_ship.curr_frame = 0;

                     // add this action to key state

                     players_key_state += REMOTE_RIGHT;

#if DEBUG
                     sent_right++;
#endif


                     } // end if right

                if (keyboard_state[MAKE_UP] && players_energy>0)
                     {
                     // thrust foward

                     players_xv+=motion_dx[players_ship.curr_frame];
                     players_yv+=motion_dy[players_ship.curr_frame];

                     // bound maximum velocity

                     if (players_xv > 8)
                        players_xv=8;
                     else
                     if (players_xv <-8)
                        players_xv=-8;

                     if (players_yv > 8)
                        players_yv=8;
                     else
                     if (players_yv <-8)
                        players_yv=-8;

                     // flag engines on

                     players_engine = 1;

                     players_energy--;

                     // add this action to key state

                     players_key_state += REMOTE_THRUST;

#if DEBUG
                     sent_up++;
#endif

                     } // end if up

                if (keyboard_state[MAKE_SPACE] && !debounce_fire &&
                    players_active_missiles < 5 && players_energy>0)
                     {
                     // fire weapons

                     Start_Missile(players_x+SHIP_WIDTH/2,players_y+SHIP_HEIGHT/2,
                                   players_xv+2*motion_dx[players_ship.curr_frame],
                                   players_yv+2*motion_dy[players_ship.curr_frame],
                                   10,PLAYER_MISSILE);

                     // update energy

                     players_energy-=5;

                     // now there is one more active missile

                     players_active_missiles++;

                     // add this action to key state

                     players_key_state += REMOTE_FIRE;

                     // set fire button debounce

                     debounce_fire = 1;

                     } // end if weapons

                 // instrumentation

                 if (keyboard_state[MAKE_ALT] && !debounce_shields &&
                     players_cloak==-1 && players_shield_strength>0)
                     {
                     // turn the shields on

                     Shield_Control(THE_PLAYER,1);

                     Digital_FX_Play(BLZSHLD_VOC,1);

                     // start timer

                     players_shield_time = 100;

                     // add this action to key state

                     players_key_state += REMOTE_SHIELDS;

                     // request debounce

                     debounce_shields = 1;

                     } // end if shields
                 else
                 if (keyboard_state[MAKE_C] && !debounce_cloak && players_energy>0)
                      {
                      // toggle the cloaking device

                      players_cloak = -players_cloak;

                      Digital_FX_Play(BLZCLK_VOC ,1);

                      // refresh heads up if it's on

                      refresh_heads=1;

                      // request debounce

                      debounce_cloak = 1;

                      // add this action to key state

                      players_key_state += REMOTE_CLOAK;

                      } // emd if cloak
                else
                if (keyboard_state[MAKE_H] && !debounce_hud)
                     {
                     // toggle the display status

                     players_heads=-players_heads;

                     // test if it's on or off

                     if (players_heads==1)
                        {
                        Draw_Heads();
                        Digital_FX_Play(BLZTAC_VOC ,1);
                        }
                     else
                        Erase_Heads();


                     // request debounce

                     debounce_hud = 1;

                     } // end if heads
                else
                if (keyboard_state[MAKE_S] && !debounce_scan)
                     {
                     // toggle the scanner

                     players_scanner=-players_scanner;

                     // test if it's on or off

                     if (players_scanner==1)
                        {
                        Draw_Scanner();
                        Digital_FX_Play(BLZSCN_VOC ,1);
                        }
                     else
                        Erase_Scanner();

                     // refresh heads up if it's on

                     refresh_heads=1;

                     // request debounce

                     debounce_scan = 1;

                     } // end if scanner
                else
                if (keyboard_state[MAKE_ESC])
                     {
                     game_state = GAME_SETUP;

                     // add this action to key state

                     players_key_state += REMOTE_ESC;

                     } // end if escape

                } // end if kbhit

             } // end if player alive

               // debounce section

               if (!keyboard_state[MAKE_C])
                   debounce_cloak=0;

               if (!keyboard_state[MAKE_H])
                  debounce_hud=0;

               if (!keyboard_state[MAKE_S])
                  debounce_scan=0;

               if (!keyboard_state[MAKE_SPACE])
                  debounce_fire=0;

               if (!keyboard_state[MAKE_ALT])
                  debounce_shields=0;

             // send local players data to remote machine

             if (linked)
                {

                Serial_Write(players_key_state);

                } // end if linked

             // update energy loss due to normal operation

             if (players_cloak==1)
                players_energy--;

             if (players_energy>0)
                {
                players_energy--;

                // enable refresh

                refresh_heads = 1;

                } // end energy update
             else
                players_energy=0;


             // test if shields are on

             if (players_shields==1)
                {
                players_shield_strength-=8;

                if (players_shield_strength<0)
                    players_shield_strength = 0;

                // enable refresh

                refresh_heads = 1;

                } // end if shields are on

             // test if hud needs refreshing

             if (players_heads==1 && refresh_heads)
                {

                Draw_Heads();

                // reset refresh flag
                refresh_heads = 0;

                } // end if hud needs a refresh

             // translate player and apply friction if engines aren't on

             players_x+=players_xv;
             players_y+=players_yv;


             // test for gravity

             if (!players_engine)
                {
                if (++players_gravity >=players_stability)
                   {
                   // reset gravity count

                   players_gravity = 0;

                   // apply friction (in space!!!). hey it's just a game

                   if (players_xv>0)
                      players_xv--;
                   else
                   if (players_xv<0)
                      players_xv++;

                   if (players_yv>0)
                      players_yv--;
                   else
                   if (players_yv<0)
                      players_yv++;

                   } // end if time to apply friction

                } // end if engines are off


             // show engines flicker

             if (players_engine)
                {
                if (++players_flame_count > players_flame_time)
                   {
                   // turn engines on

                   Write_Color_Reg(PLAYERS_ENGINE_REG,
                                   (RGB_color_ptr)&players_engine_color);

                   // reset counter

                   players_flame_count = 0;

                   } // end if time to toggle engines
                else
                   {
                   // turn engines off

                   Write_Color_Reg(PLAYERS_ENGINE_REG,
                                   (RGB_color_ptr)&primary_black);

                   } // end else

                } // end if engines are on


             // test if shields should turn off

             if (players_shield_time>0)
                {
                // try and turn off shields

                if (--players_shield_time<=0)
                   Shield_Control(THE_PLAYER,0);
                else
                   {
                   // which shield colors?

                   if (players_ship_type==GRYFON_SHIP)
                      {
                      if ((players_shield_color.blue+=8) >=64)
                         players_shield_color.blue = 24;

                      Write_Color_Reg(PLAYERS_SHIELD_REG,(RGB_color_ptr)&players_shield_color);
                      }
                   else
                      {
                      // must be a raptor

                      if ((players_shield_color.red+=8) >=64)
                         players_shield_color.red = 24;

                      Write_Color_Reg(PLAYERS_SHIELD_REG,(RGB_color_ptr)&players_shield_color);

                      } // end else

                   } // end else modulate shields

                } // end if





             // compute players delta

             players_dx = players_last_x - players_x;
             players_dy = players_last_y - players_y;

             // do player boundary detection
             // we are going to wrap universe around, but maybe placing
             // a barrier at edges would be better?

             if (players_x > UNIVERSE_WIDTH)
                players_x = players_x - UNIVERSE_WIDTH;
             else
             if (players_x < 0)
                players_x = players_x + UNIVERSE_WIDTH;

             if (players_y > UNIVERSE_HEIGHT)
                players_y = players_y - UNIVERSE_HEIGHT;
             else
             if (players_y < 0)
                players_y = players_y + UNIVERSE_HEIGHT;


//END PLAYER///////////////////////////////////////////////////////////////////




// BEGIN REMOTE//////////////////////////////////////////////////////////////////


             // only process remote if machines are linked

             if (linked)
             {

             // move remote

             remotes_last_x = remotes_x;
             remotes_last_y = remotes_y;

             // reset remotes input

             remotes_key_state = 0;

             // get input from remote machine

             remotes_key_state = Serial_Read_Wait();

             // test if a key is depressed


             if (remotes_state==ALIVE)
             {

             if (remotes_key_state > 0)
                {
                // which key ?

                if ((remotes_key_state & REMOTE_LEFT))
                     {
                     // rotate left

                     if (--remotes_ship.curr_frame < 0)
                        remotes_ship.curr_frame = 15;
#if DEBUG

                     rec_left++;
#endif

                     } // end if left
                else
                if ((remotes_key_state & REMOTE_RIGHT))
                     {
                     // rotate right

                     if (++remotes_ship.curr_frame > 15)
                        remotes_ship.curr_frame = 0;

#if DEBUG
                     rec_right++;
#endif

                     } // end if right

                if ((remotes_key_state & REMOTE_THRUST) && remotes_energy>0)
                     {
                     // thrust foward

                     remotes_xv+=motion_dx[remotes_ship.curr_frame];
                     remotes_yv+=motion_dy[remotes_ship.curr_frame];

                     // bound maximum velocity

                     if (remotes_xv > 8)
                        remotes_xv=8;
                     else
                     if (remotes_xv <-8)
                        remotes_xv=-8;

                     if (remotes_yv > 8)
                        remotes_yv=8;
                     else
                     if (remotes_yv <-8)
                        remotes_yv=-8;

                     // flag engines on

                     remotes_engine = 1;
                     remotes_energy--;

#if DEBUG
                     rec_up++;
#endif


                     } // end if up

                if ((remotes_key_state & REMOTE_FIRE) && remotes_active_missiles < 5 &&
                    remotes_energy>0)
                     {
                     // fire weapons

                     Start_Missile(remotes_x+SHIP_WIDTH/2,remotes_y+SHIP_HEIGHT/2,
                                   remotes_xv+2*motion_dx[remotes_ship.curr_frame],
                                   remotes_yv+2*motion_dy[remotes_ship.curr_frame],
                                   10,REMOTE_MISSILE);

                     // decrease energy

                     remotes_energy-=5;

                     // now there is one more active missile

                     remotes_active_missiles++;

                     } // end if weapons

                 // instrumentation

                 if ((remotes_key_state & REMOTE_SHIELDS) &&
                    remotes_cloak==-1 && remotes_shield_strength>0)
                     {
                     // turn the shields on

                     Shield_Control(THE_REMOTE,1);

                     // start timer

                     remotes_shield_time = 100;

                     } // end if shields
                 else
                 if ((remotes_key_state & REMOTE_CLOAK) && remotes_energy>0)
                      {
                      // toggle the cloaking device

                      remotes_cloak = -remotes_cloak;

                      } // emd if cloak
                else
                if ((remotes_key_state & REMOTE_ESC))
                     {

                     game_state = GAME_SETUP;

                     } // end if escape

                } // end if kbhit

             // update energy loss due to normal operation

             if (remotes_cloak==1)
                remotes_energy--;

             if (remotes_energy>0)
                {
                remotes_energy--;

                } // end energy update
             else
                remotes_energy=0;

             // test if shields are on

             if (remotes_shields==1)
                {
                remotes_shield_strength-=8;

                if (remotes_shield_strength<0)
                    remotes_shield_strength = 0;

                } // end if shields are on

             } // end if remote alive

             // translate remote and apply friction if engines aren't on

             remotes_x+=remotes_xv;
             remotes_y+=remotes_yv;

             // test for gravity

             if (!remotes_engine)
                {
                if (++remotes_gravity >=remotes_stability)
                   {
                   // reset gravity count

                   remotes_gravity = 0;

                   // apply friction (in space!!!). hey it's just a game

                   if (remotes_xv>0)
                      remotes_xv--;
                   else
                   if (remotes_xv<0)
                      remotes_xv++;

                   if (remotes_yv>0)
                      remotes_yv--;
                   else
                   if (remotes_yv<0)
                      remotes_yv++;

                   } // end if time to apply friction

                } // end if engines are off

             // show engines flicker

             if (remotes_engine)
                {
                if (++remotes_flame_count > remotes_flame_time)
                   {
                   // turn engines on

                   Write_Color_Reg(REMOTES_ENGINE_REG,
                                   (RGB_color_ptr)&remotes_engine_color);

                   // reset counter

                   remotes_flame_count = 0;

                   } // end if time to toggle engines
                else
                   {
                   // turn engines off

                   Write_Color_Reg(REMOTES_ENGINE_REG,
                                   (RGB_color_ptr)&primary_black);

                   } // end else

                } // end if engines are on


             // test if shields should turn off

             if (remotes_shield_time>0)
                {
                // try and turn off shields

                if (--remotes_shield_time<=0)
                   Shield_Control(THE_REMOTE,0);
                else
                   {
                   // which shield colors?

                   if (remotes_ship_type==GRYFON_SHIP)
                      {
                      if ((remotes_shield_color.blue+=8) >=64)
                         remotes_shield_color.blue = 24;

                      Write_Color_Reg(REMOTES_SHIELD_REG,(RGB_color_ptr)&remotes_shield_color);
                      }
                   else
                      {
                      // must be a raptor

                      if ((remotes_shield_color.red+=8) >=64)
                         remotes_shield_color.red = 24;

                      Write_Color_Reg(REMOTES_SHIELD_REG,(RGB_color_ptr)&remotes_shield_color);

                      } // end else

                   } // end else modulate shields

                } // end if

             // compute remotes delta

             remotes_dx = remotes_last_x - remotes_x;
             remotes_dy = remotes_last_y - remotes_y;

             // do remotes boundary detection
             // we are going to wrap universe around, but maybe placing
             // a barrier at edges would be better?

             if (remotes_x > UNIVERSE_WIDTH)
                remotes_x = remotes_x - UNIVERSE_WIDTH;
             else
             if (remotes_x < 0)
                remotes_x = remotes_x + UNIVERSE_WIDTH;

             if (remotes_y > UNIVERSE_HEIGHT)
                remotes_y = remotes_y - UNIVERSE_HEIGHT;
             else
             if (remotes_y < 0)
                remotes_y = remotes_y + UNIVERSE_HEIGHT;

             } // end if linked

//END REMOTE///////////////////////////////////////////////////////////////////

             // call AI logic for alien

             Alien_Control();

             // move all objects

             Move_Stars();
             Move_Alien();
             Move_Asteroids();
             Move_Missiles();
             Animate_Explosions();
             Move_Novas();
             Animate_Wormhole();
             Animate_Fuel_Cells();

             // perform death sequence logic for player

             if (players_state==DYING)
                {

                // decrement death counter

                if (--players_death_count<=0)
                   {
                   // reset player to starting position

                   Reset_Player();

                   // test if this is it...

                   if (--players_num_ships==0)
                      {
                      // return to setup

                      game_state = GAME_SETUP;
                      winner = WINNER_REMOTE;

                      } // end if it's all over

                      // refresh hud in any case

                      if (players_heads==1)
                         Draw_Heads();

                   } // end if death done

                } // end if player dying


             // perform death sequence logic for remote

             if (linked && remotes_state==DYING)
                {

                // decrement death counter

                if (--remotes_death_count<=0)
                   {
                   // reset remote to starting position

                   Reset_Remote();

                   // test if this is it...

                   if (--remotes_num_ships==0)
                      {

                      game_state = GAME_SETUP;
                      winner = WINNER_PLAYER;

                      } // end if it's all over

                   } // end if death done

                } // end if remote dying

             // scan under all objects

             Under_Fuel_Cells();
             Under_Stars();
             Under_Asteroids();

             Under_Missiles();
             Under_Explosions();
             Under_Novas();
             Under_Wormhole();
             Under_Alien();

             // render scanner with new data

             if (players_scanner==1)
                Under_Blips();

// BEGIN PLAYER ////////////////////////////////////////////////////////////////

             Sprite_Under((sprite_ptr)&players_ship,double_buffer);

// END PLAYER //////////////////////////////////////////////////////////////////


// BEGIN REMOTE ////////////////////////////////////////////////////////////////

             // translate remote to player

             px_window = players_x - 160+11;
             py_window = players_y - 100+9;

             remotes_ship.x = remotes_x - px_window;
             remotes_ship.y = remotes_y - py_window;

             Sprite_Under_Clip((sprite_ptr)&remotes_ship,double_buffer);

// END REMOTE //////////////////////////////////////////////////////////////////

             // draw all objects

             Draw_Wormhole();
             Draw_Fuel_Cells();
             Draw_Stars();
             Draw_Asteroids();

//BEGIN PLAYER//////////////////////////////////////////////////////////////////

             // display proper image of ship depending on state of engines
             // and cloak


             // but first is there a ship?

             if (players_state==ALIVE)
             {
             // test if ship is totally cloaked
             if (players_cloak==-1)
                {
                // ship isn't cloaked

                if (players_engine)
                   {
                   // index into frames with thrust

                   players_ship.curr_frame+=16;

                   // draw the ship with thrust showing

                   Sprite_Draw((sprite_ptr)&players_ship,double_buffer,1);

                   // restore the original frame

                   players_ship.curr_frame-=16;

                   } // end if engines are on
                else
                   {
                   // no engines

                   Sprite_Draw((sprite_ptr)&players_ship,double_buffer,1);

                   } // end else no engine flicker

                } // end if not cloaked
             else
                {
                // player is cloaked

                // test if player is trying to engage shields

                if (players_shields)
                   {
                   // force shields off

                   Shield_Control(THE_PLAYER,0);
                   players_shield_time=0;

                   } // end if trying to engage sheilds

                } // end else ship cloaked

             } // end if player alive

//END PLAYER//////////////////////////////////////////////////////////////////


//BEGIN REMOTE//////////////////////////////////////////////////////////////////

             // display proper image of ship depending on state of engines
             // and cloak

             // but first is there is a ship?

             if (remotes_state==ALIVE)
             {
             // test if ship is totally cloaked
             if (remotes_cloak==-1)
                {
                // ship isn't cloaked

                if (remotes_engine)
                   {
                   // index into frames with thrust

                   remotes_ship.curr_frame+=16;

                   // draw the ship with thrust showing

                   Sprite_Draw_Clip((sprite_ptr)&remotes_ship,double_buffer,1);

                   // restore the original frame

                   remotes_ship.curr_frame-=16;

                   } // end if engines are on
                else
                   {
                   // no engines

                   Sprite_Draw_Clip((sprite_ptr)&remotes_ship,double_buffer,1);

                   } // end else no engine flicker

                } // end if not cloaked
             else
                {
                // player is cloaked

                // test if player is trying to engage shields

                if (remotes_shields)
                   {
                   // force shields off

                   Shield_Control(THE_REMOTE,0);
                   remotes_shield_time=0;

                   } // end if trying to engage sheilds

                } // end else ship cloaked

             } // end if remote is alive


//END REMOTE//////////////////////////////////////////////////////////////////

             // draw remaining priority objects

             Draw_Missiles();
             Draw_Alien();
             Draw_Explosions();
             Draw_Novas();

             // render scanner with new data

             if (players_scanner==1)
                Draw_Blips();

             // display coordinates and score

             sprintf(buffer,"COORDINATES:[%d,%d]    ",players_x,players_y);
             Font_Engine_1(0,0,0,0,buffer,double_buffer);

             sprintf(buffer,"SCORE: %d",players_score);
             Font_Engine_1(250,0,0,0,buffer,double_buffer);

#if DEBUG
             // diagnostic stuff

             sprintf(buffer,"REMOTE:[%d,%d]    ",remotes_x,remotes_y);
             Font_Engine_1(0,10,0,0,buffer,double_buffer);

             sprintf(buffer,"SENT: R=%d L=%d U=%d ",sent_right,sent_left,sent_up);
             Font_Engine_1(0,20,0,0,buffer,double_buffer);

             sprintf(buffer,"REC: R=%d L=%d U=%d ",rec_right,rec_left,rec_up);
             Font_Engine_1(0,30,0,0,buffer,double_buffer);

             // end diagnostic stuff

#endif

             // display double buffer

             Display_Double_Buffer(double_buffer,0);

             // lock onto 18 frames per second max

             while((Timer_Query()-starting_time)<1);

             // check on music

             if (music_enabled)
             {
             if (Music_Status()==2 || Music_Status()==0)
                {

                // advance to next sequence

                if (++game_seq_index==18)
                   game_seq_index=0;

                Music_Play((music_ptr)&song,game_sequence[game_seq_index]);

                } // end if

             } // end if music enabled

             } // end game loop

        // test if there is a winner or user just decided to exit

        if (winner==WINNER_REMOTE)
           {
           // tell player remote is winner

           Tech_Print(128,80,"CEASE COMBAT!",video_buffer);

           if (music_enabled)
              {
              Music_Stop();
              Music_Play((music_ptr)&song,11);
              }
           Time_Delay(25);
           Digital_FX_Play(BLZLOS_VOC,2);
           Tech_Print(120,100,"YOU ARE DEFEATED",video_buffer);
           Time_Delay(50);

           } // end if remote won
        else
        if (winner==WINNER_PLAYER)
           {
           // tell player he is winner

           Tech_Print(128,80,"CEASE COMBAT!",video_buffer);
           if (music_enabled)
              {
              Music_Stop();
              Music_Play((music_ptr)&song,11);
              }
           Time_Delay(25);
           Digital_FX_Play(BLZWIN_VOC,2);
           Tech_Print(115,100,"YOU ARE VICTORIOUS",video_buffer);
           Time_Delay(50);

           } // end if player won

        // do all exit clean up here to move back to the setup state

        Screen_Transition(SCREEN_DARKNESS);

        // restart intro music

        if (music_enabled)
           {
           // stop game music, start intro music again

           intro_seq_index=0;
           Music_Stop();
           Music_Play((music_ptr)&song,intro_sequence[intro_seq_index]);

           } // end if music enabled

        } // end if running

     } // end main system event loop

///////////////////////////////////////////////////////////////////////////////

// exit in a very cool way

Screen_Transition(SCREEN_DARKNESS);

// free up all resources

Delete_Double_Buffer();

// remove the keyboard handler

Keyboard_Remove_Driver();

// close down FX

Digital_FX_Close();

// show the credits

Closing_Screen();

// close down music

Music_Close();

Set_Graphics_Mode(TEXT_MODE);

// see ya!

printf("\nSTARBLAZER Shutdown Normal.\n");

} // end main


