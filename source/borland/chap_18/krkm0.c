
// KRK.C - Version 1.0, Andre' LaMothe

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

#include "black18.h"    // the newest version of the 3-D library
#include "krkm0.h"

// G L O B A L S  ////////////////////////////////////////////////////////////

object static_obj[NUM_STATIONARY];      // there are six basic stationary object types

fixed_obj obstacles_1[NUM_OBSTACLES_1]; // general obstacles, rocks etc.
fixed_obj obstacles_2[NUM_OBSTACLES_2]; // general obstacles, rocks etc.

fixed_obj barriers[NUM_BARRIERS];       // boundary universe boundaries
fixed_obj towers[NUM_TOWERS];           // the control towers
fixed_obj stations[NUM_STATIONS];       // the power stations
fixed_obj telepods[NUM_TELEPODS];       // the teleporters


object dynamic_obj[NUM_DYNAMIC];        // this array holds the models
                                        // for the dynamic game objects

alien aliens[NUM_ALIENS];               // take a wild guess!

missile_obj missiles[NUM_MISSILES];     // holds the missiles

int active_player_missiles = 0;         // how many missiles has player activated

int total_active_missiles = 0;          // total active missiles

layer mountains;              // the background mountains

pcx_picture image_pcx,        // general PCX image used to load background and imagery
            image_controls;   // this holds the controls screen

sprite tactical_spr,          // holds the images for the tactical displays
       buttons_spr;           // holds the images for the control buttons

RGB_color color_1,color_2;    // used for temporaries during color rotation

RGB_palette game_palette;     // this will hold the startup system palette

bitmap tech_font[NUM_TECH_FONT];   // the tech font bitmaps

int game_state = GAME_SETUP;       // the overall state of the game

int scanner_state  = 0,            // state of scanner
    hud_state      = 0,
    tactical_state = TACTICAL_MODE_OFF;

int ship_pitch  = 0,   // current direction of ship
    ship_yaw    = 0,   // not used
    ship_roll   = 0,   // not used
    ship_speed  = 0,   // speed of ship
    ship_energy = 50,  // current energy of ship
    ship_damage = 0,   // current damage up to 50 points
    ship_message = 0,  // current message to ship state machine
    ship_timer   = 0,  // a little timer
    ship_kills   = 0,  // how many bad guys has player killed
    ship_deaths  = 0;  // how many times has player been killed

vector_3d unit_z          = {0,0,1,1},  // a unit vector in the Z direction
          ship_direction  = {0,0,1,1};  // the ships direction

int players_ship_type = PLAYER_TALLON;  // the player starts off with a tallon

// musical sequence information

int music_enabled   = 0,      // flags that enable music and sound FX
    digital_enabled = 0;

int digital_FX_priority = 10; // the priority tracker of the current effect

int intro_sequence[] = {0+12,1+12,2+12,3+12,1+12,2+12,3+12,2+12,3+12,1+12,
                        2+12,3+12,2+12,2+12};

int intro_seq_index  = 0; // starting intro index number of sequence to be played

int game_sequence[]  = {11,8,2,3,1,2,5,8,6,1,0,2,4,5,2,1,0,3,4,8};

int game_seq_index   = 0; // starting game index number of sequence to be played

music song;    // the music structure

// sound fx stuff

_sound digital_FX[NUM_SOUND_FX];

// basic colors

RGB_color  bright_red   = {63,0,0},    // bright red
           bright_blue  = {0,0,63},    // bright blue
           bright_green = {0,63,0},    // bright green

           medium_red   = {48,0,0},    // medium red
           medium_blue  = {0,0,48},    // medium blue
           medium_green = {0,48,0},    // medium green

           dark_red     = {32,0,0},    // dark red
           dark_blue    = {0,0,32},    // dark blue
           dark_green   = {0,32,0},    // dark green

                  black = {0,0,0},     // pure black
                  white = {63,63,63},  // pure white

           color_1,color_2,color_3;    // general color variables

// the instruction pages

char *instructions[]={"KILL OR BE KILLED                  ",
                      "                                   ",
                      "INTERGALACTIC BATTLE FEDERATION    ",
                      "THESE RULES APPLY TO ALL ENTRIES   ",
                      "                                   ",
                      "1. YOU MAY FREELY DESTROY ALL AND  ",
                      "   ANY ENEMY THAT YOU MAY MEET UP  ",
                      "   WITH.                           ",
                      "                                   ",
                      "2. ANY PARTICIPANT EXHIBITING MERCY",
                      "   IN ANY FORM WILL BE SUMMARILY   ",
                      "   TERMINATED. YOU MUST KILL!      ",
                      "                                   ",
                      "3. LEAVING THE GAME GRID WHILE     ",
                      "   THERE ARE ANY OPPONENTS ALIVE IS",
                      "   FORBIDDEN.                      ",
                      "                1                  ",

                      "PLAYING THE GAME                   ",
                      "                                   ",
                      "AT THE MAIN MENU SELECT THE BATTLE ",
                      "MECH THAT YOU WISH TO PLAY WITH    ",
                      "BY USING THE (SELECT MECH) OPTION  ",
                      "ITEM. YOU WILL THEN BE SENT TO     ",
                      "THE HOLOGRAPHIC SPECIFICATION AREA.",
                      "                                   ",
                      "USE THE RIGHT AND LEFT ARROW KEYS  ",
                      "TO VIEW DIFFERENT MECHS. WHEN YOU  ",
                      "ARE SATIFIED THEN PRESS (ENTER).   ",
                      "                                   ",
                      "TO EXIT WITHOUT MAKING A SELECTION ",
                      "PRESS (ESC).                       ",
                      "                                   ",
                      "                                   ",
                      "                2                  ",

                      "AFTER YOU HAVE SELECTED A MECH THEN",
                      "SELECT THE (CHALLENGE) ITEM OF THE ",
                      "MAIN MENU. YOU WILL ENTER INTO THE ",
                      "GAME AREA AT ITS EXACT CENTER.     ",
                      "                                   ",
                      "THE GAME AREA CONSISTS OF OBSTACLES",
                      "SUCH AS ROCKS AND CRYSTAL GROWTHS. ",
                      "                                   ",
                      "AROUND THE PERIMETER OF THE GAME   ",
                      "GRID IS AN LASER BARRIER SYSTEM    ",
                      "POWERED BY LARGE BLACK MONOLITHS   ",
                      "WITH GREEN BEACONS. STAY AWAY FROM ",
                      "THESE IF POSSIBLE. AS NOTED ABOVE  ",
                      "YOU WILL BE POSITIONED AT THE EXACT",
                      "CENTER OF THE GAME GRID. IN FACT,  ",
                      "THE POSITION YOU ARE STARTED AT    ",
                      "                3                  ",

                      "IS THE LOCATION OF THE MAIN POWER  ",
                      "SOURCE FOR THE GAME GRID. IT MAY BE",
                      "POSSIBLE TO RE-CHARGE BY DRIVING   ",
                      "OVER THIS ROTATING ENERGY SOURCE.  ",
                      "                                   ",
                      "HOWEVER, THERE ARE FOUR COM TOWERS ",
                      "OVERLOOKING THIS CENTRAL POWER     ",
                      "STATION AND ENTRIES TRYING TO STEEL",
                      "POWER MAY BE DEALT WITH...         ",
                      "                                   ",
                      "FINALLY TO MAKE THE GAME MORE      ",
                      "INTERESTING THERE ARE FOUR TELEPODS",
                      "LOCATED AT THE FOUR CORNERS OF THE ",
                      "GAME GRID. TO TELEPORT TO ANOTHER  ",
                      "POD, SIMPLY NAVIGATE UNDER ONE OF  ",
                      "THE PODS.                          ",
                      "                4                  ",

                      "CONTROLING THE SHIP                ",
                      "                                   ",
                      "BOTH SHIPS HAVE THE SAME CONTROLS  ",
                      "THEREFORE THE FOLLOWING EXPLAINA-  ",
                      "TION IS VALID FOR BOTH TALLON AND  ",
                      "SLIDER PILOTS.                     ",
                      "                                   ",
                      "INFORMATION DISPLAYS               ",
                      "                                   ",
                      "THE SHIPS ARE FITTED WITH THE      ",
                      "FOLLOWING DISPLAYS.                ",
                      "                                   ",
                      "1. SCANNERS                        ",
                      "2. HEADS UP DISPLAY (HUD)          ",
                      "3. MULTI-FUNCTION DISPLAY (MFD)    ",
                      "                                   ",
                      "                5                  ",

                      "ENGAGING THE DISPLAYS              ",
                      "                                   ",
                      " SCANNER....S TOGGLES              ",
                      " HUD........H TOGGLES              ",
                      " MFD........T TO SELECT VARIOUS    ",
                      "              DATA OUTPUTS.        ",
                      "                                   ",
                      "THE SCANNER IS LOCATED ON THE LEFT ",
                      "MAIN VIEW SCREEN AND THE MFD IS    ",
                      "LOCATED ON THE RIGHT MAIN VIEWING  ",
                      "SCREEN.                            ",
                      "                                   ",
                      "THE SCANNER REPRESENTS YOU AS A    ",
                      "BLUE BLIP AND YOU ENEMIES AS RED.  ",
                      "OTHER IMPORTANT OBJECTS ARE ALSO   ",
                      "SCANNED AND DISPLAYED.             ",
                      "                6                  ",

                      "THE MFD IS CURRENTLY FIT TO DISPLAY",
                      "TWO MAIN DATA SETS. THE FIRST IS   ",
                      "THE SHIPS SPEED, DAMAGE AND ENERGY ",
                      "IN LINEAR GRAPHS AND THE SECOND IS ",
                      "AN OUTSIDE HULL DAMAGE SYSTEM.     ",
                      "                                   ",
                      "THE HUD IS A STANDARD POSITION,    ",
                      "TRAJECTORY AND SPEED INDICATOR.    ",
                      "                                   ",
                      "MOTION AND WEAPONS CONTROL         ",
                      "                                   ",
                      "TO ROTATE THE SHIP RIGHT AND LEFT  ",
                      "USE THE RIGHT AND LEFT ARROW KEYS. ",
                      "                                   ",
                      "TO INCREASE FORWARD THRUST USE THE ",
                      "UP ARROW AND TO DECREASE THRUST USE",
                      "                7                  ",


                      "THE DOWN ARROW.                    ",
                      "                                   ",
                      "TO FIRE THE PULSE CANNON PRESS THE ",
                      "SPACE BAR.                         ",
                      "                                   ",
                      "TO EXIT THE GAME PRESS ESCAPE.     ",
                      "                                   ",
                      "HAVE FUN AND REMEMBER:             ",
                      "                                   ",
                      "KILL OR BE KILLED!!!               ",
                      "                                   ",
                      "                                   ",
                      "                                   ",
                      "                                   ",
                      "                                   ",
                      "                                   ",
                      "           END OF FILE             ",};


// specification for the tallon

char *tallon_specs[] = {"MASS:      2567 KS        ",
                        "LENGTH:    25.3 M         ",
                        "HEAT DIS:  2.3 E.S        ",
                        "WEAPONS:   POSITRON PLASMA",
                        "SHIELDS:   STANDARD E-MAG ",
                        "MAX SPD:   50 M.S         ",
                        "ANG VEL:   72 D.S         ",
                        "PROP UNIT: FUSION PULSE   "};

// specifications for the slider

char *slider_specs[] = {"MASS:      2245 KS        ",
                        "LENGTH:    22.3 M         ",
                        "HEAT DIS:  3.4 E.S        ",
                        "WEAPONS:   POSITRON PLASMA",
                        "SHIELDS:   STANDARD E-MAG ",
                        "MAX SPD:   45 M.S         ",
                        "ANG VEL:   80 D.S         ",
                        "PROP UNIT: FUSION PULSE   "};

// F U N C T I O N S ////////////////////////////////////////////////////////

// M A I N //////////////////////////////////////////////////////////////////

void main(int argc, char **argv)
{

// the main controls most of the platyer and remote logic, normally we would
// probably move most of the code into functions, but for instructional purposes
// this is easier to follow, believe me there are already enough function calls
// to make your head spin!

int index,               // looping variable
    sel,                 // used for input
    players_key_state,   // state of players input
    current_sel=0,       // currently highlighted interface selection
    weapons_lit_count=0; // times how long the light flash from a missile
                         // should be displayed when player fires

unsigned char seed;      // a random number seed

char buffer[64],         // general buffer
     number[32],         // used to print strings
     ch;                 // used for keyboard input



//INITIALIZATION SECTION///////////////////////////////////////////////////////

// set up viewing system

Set_3D_View();

// build all look up tables

Build_Look_Up_Tables();

// load in all 3d models

Load_3D_Objects();

// parse the command line and set up configuration

Parse_Commands(argc,argv);

// set the graphics mode to mode 13h

Set_Graphics_Mode(GRAPHICS_MODE13);

// start up music system

if (music_enabled)
   {
   Music_Init();
   Music_Play((music_ptr)&song,16);
   } // end if music enabled

//INTRODUCTION SECTION/////////////////////////////////////////////////////////

// put up Waite header

Intro_Waite();

// put up my header

Intro_KRK();

// seed the random number generator with time

srand((unsigned int)Timer_Query());

Tech_Print(START_MESS_X,START_MESS_Y," KILL OR BE KILLED 1.0 STARTING UP...",video_buffer);
Time_Delay(5);

// initialize font engine

Font_Engine_1(0,0,0,0,NULL,NULL);
Tech_Print(START_MESS_X,START_MESS_Y+16," LANGUAGE TRANSLATION ENABLED",video_buffer);

// create the double buffer

Create_Double_Buffer(129);
Tech_Print(START_MESS_X,START_MESS_Y+26," DOUBLE BUFFER CREATED",video_buffer);

// install the keyboard driver

Keyboard_Install_Driver();
Tech_Print(START_MESS_X,START_MESS_Y+36," NEURAL INTERFACE ACTIVATED",video_buffer);

// load and create background layer

Tech_Print(START_MESS_X,START_MESS_Y+46," BACKGROUND ENABLED",video_buffer);

PCX_Init((pcx_picture_ptr)&image_pcx);
PCX_Load("krkbak.pcx",(pcx_picture_ptr)&image_pcx,1);
PCX_Copy_To_Buffer((pcx_picture_ptr)&image_pcx,double_buffer);
PCX_Delete((pcx_picture_ptr)&image_pcx);

Layer_Create((layer_ptr)&mountains,MOUNTAIN_WIDTH,MOUNTAIN_HEIGHT);
Layer_Build((layer_ptr)&mountains,0,0,
            double_buffer,
            0,36,SCREEN_WIDTH,43);


Load_Tactical();
Tech_Print(START_MESS_X,START_MESS_Y+56," TACTICAL ONLINE",video_buffer);


// memory problem

if (digital_enabled)
   {
   Sound_Load("KRKPOW.VOC" ,  (_sound_ptr)&digital_FX[KRKPOW_VOC ],1);
   Digital_FX_Play(KRKPOW_VOC,3);
   Sound_Unload((_sound_ptr)&digital_FX[KRKPOW_VOC]);
   } // end if

// end memory problem

// all systems powered

Tech_Print(START_MESS_X,START_MESS_Y+108," ALL SYSTEMS POWERED AND AVAILABLE",video_buffer);

for (index=0; index<3; index++)
    {
    // draw the message and the erase the message
    Font_Engine_1(START_MESS_X,START_MESS_Y+108,0,0," ALL SYSTEMS POWERED AND AVAILABLE",video_buffer);
    Time_Delay(8);

    Font_Engine_1(START_MESS_X,START_MESS_Y+108,0,0,"                                  ",video_buffer);
    Time_Delay(8);

    } // end for

// start up digital FX system

if (digital_enabled)
   {
   Digital_FX_Init();
   } // end if digital sound enabled


// do intro piece

Intro_Planet();

// save the system pallete here because we are going to really thrash it!!!

Read_Palette(0,255,(RGB_palette_ptr)&game_palette);


//MAIN EVENT LOOP//////////////////////////////////////////////////////////////


// main event loop ///////////////////////////////////////////////////////////

while(game_state!=GAME_OVER)
     {
     // test the overall game state


//SETUP SECTION////////////////////////////////////////////////////////////////

     if (game_state==GAME_SETUP)
        {

        // user in in the setup state

        Intro_Controls();

        Draw_Box(SELECT_BOX_SX,
                 SELECT_BOX_SY+current_sel*SELECT_BOX_DY,
                 SELECT_BOX_SX+SELECT_BOX_WIDTH,
                 SELECT_BOX_SY+SELECT_BOX_HEIGHT+current_sel*SELECT_BOX_DY,
                 254);


        Draw_Rectangle(SELECT_LGT_SX,
                      SELECT_LGT_SY+current_sel*SELECT_LGT_DY,
                      SELECT_LGT_SX+SELECT_LGT_WIDTH,
                      SELECT_LGT_SY+SELECT_LGT_HEIGHT+current_sel*SELECT_LGT_DY,
                      254);

        // restore pallete

        Write_Palette(0,255,(RGB_palette_ptr)&game_palette);

        // enter setup event loop

        Panel_FX(1);

        while(game_state==GAME_SETUP)
             {
             // this event loop is for the setup phase

             // test for up or down

             if (keyboard_state[MAKE_UP])
                {

                // erase current cursor position

                Draw_Box(SELECT_BOX_SX,
                         SELECT_BOX_SY+current_sel*SELECT_BOX_DY,
                         SELECT_BOX_SX+SELECT_BOX_WIDTH,
                         SELECT_BOX_SY+SELECT_BOX_HEIGHT+current_sel*SELECT_BOX_DY,
                         0);

                Draw_Rectangle(SELECT_LGT_SX,
                               SELECT_LGT_SY+current_sel*SELECT_LGT_DY,
                               SELECT_LGT_SX+SELECT_LGT_WIDTH,
                               SELECT_LGT_SY+SELECT_LGT_HEIGHT+current_sel*SELECT_LGT_DY,
                               0);

                // move up one and draw cursor and box

                if (--current_sel<0)
                   current_sel = MAX_SELECTION;

                // draw the new selected selection

                Draw_Box(SELECT_BOX_SX,
                         SELECT_BOX_SY+current_sel*SELECT_BOX_DY,
                         SELECT_BOX_SX+SELECT_BOX_WIDTH,
                         SELECT_BOX_SY+SELECT_BOX_HEIGHT+current_sel*SELECT_BOX_DY,
                         254);


                Draw_Rectangle(SELECT_LGT_SX,
                              SELECT_LGT_SY+current_sel*SELECT_LGT_DY,
                              SELECT_LGT_SX+SELECT_LGT_WIDTH,
                              SELECT_LGT_SY+SELECT_LGT_HEIGHT+current_sel*SELECT_LGT_DY,
                              254);


                Digital_FX_Play(KRKKEY_VOC,3);

                Time_Delay(1);

                } // end if

             if (keyboard_state[MAKE_DOWN])
                {

                // erase current cursor position

                Draw_Box(SELECT_BOX_SX,
                         SELECT_BOX_SY+current_sel*SELECT_BOX_DY,
                         SELECT_BOX_SX+SELECT_BOX_WIDTH,
                         SELECT_BOX_SY+SELECT_BOX_HEIGHT+current_sel*SELECT_BOX_DY,
                         0);


                Draw_Rectangle(SELECT_LGT_SX,
                              SELECT_LGT_SY+current_sel*SELECT_LGT_DY,
                              SELECT_LGT_SX+SELECT_LGT_WIDTH,
                              SELECT_LGT_SY+SELECT_LGT_HEIGHT+current_sel*SELECT_LGT_DY,
                              0);


                // move up one and draw cursor and box

                if (++current_sel>MAX_SELECTION)
                   current_sel = 0;

                // draw the new selected selection

                Draw_Box(SELECT_BOX_SX,
                         SELECT_BOX_SY+current_sel*SELECT_BOX_DY,
                         SELECT_BOX_SX+SELECT_BOX_WIDTH,
                         SELECT_BOX_SY+SELECT_BOX_HEIGHT+current_sel*SELECT_BOX_DY,
                         SELECT_REG);


                Draw_Rectangle(SELECT_LGT_SX,
                              SELECT_LGT_SY+current_sel*SELECT_LGT_DY,
                              SELECT_LGT_SX+SELECT_LGT_WIDTH,
                              SELECT_LGT_SY+SELECT_LGT_HEIGHT+current_sel*SELECT_LGT_DY,
                              SELECT_REG);

                Digital_FX_Play(KRKKEY_VOC,3);

                Time_Delay(1);

                } // end if

             // test for a selection via enter key

             if (keyboard_state[MAKE_ENTER])
                {

                Digital_FX_Play(KRKKEY_VOC,3);

                // what is the selection?

                switch(current_sel)
                      {

                      case 0:
                           {

                           game_state = GAME_RUNNING;

                           } break;

                      case 1:
                           {
                           // call the mech selection function

                           Select_Mech();

                           // re-draw main menu

                           Intro_Controls();

                           } break;


                      case 2:
                           {
                           // show user the rules and instructions

                           Intro_Briefing();

                           // re-draw main menu

                           Intro_Controls();

                           } break;

                      case 3:
                           {

                           game_state = GAME_OVER;

                           } break;

                      default: break;

                      } //  end switch

                Draw_Box(SELECT_BOX_SX,
                         SELECT_BOX_SY+current_sel*SELECT_BOX_DY,
                         SELECT_BOX_SX+SELECT_BOX_WIDTH,
                         SELECT_BOX_SY+SELECT_BOX_HEIGHT+current_sel*SELECT_BOX_DY,
                         254);


                Draw_Rectangle(SELECT_LGT_SX,
                              SELECT_LGT_SY+current_sel*SELECT_LGT_DY,
                              SELECT_LGT_SX+SELECT_LGT_WIDTH,
                              SELECT_LGT_SY+SELECT_LGT_HEIGHT+current_sel*SELECT_LGT_DY,
                              254);


                } // end if

             // slow things down a bit

             Time_Delay(1);

             // check on music

             if (music_enabled)
                {
                // test if piece is complete or has been stopped

                if (Music_Status()==2 || Music_Status()==0)
                   {
                   // advance to next sequence

                   if (++intro_seq_index==14)
                      intro_seq_index=0;

                   Music_Play((music_ptr)&song,intro_sequence[intro_seq_index]);

                   } // end if

                } // end if music enabled

             // do special fx

             Panel_FX(0);

             } // end setup event loop

        } // end if in control state

//GAME RUNNING SECTION/////////////////////////////////////////////////////////

     else
     if (game_state==GAME_RUNNING)
        {

        // restore pallete

        Write_Palette(0,255,(RGB_palette_ptr)&game_palette);

        // reset system variables

        Reset_System();

        // restart everything

        Init_Missiles();

        Init_Aliens();

        // start music

        if (music_enabled)
           {
           Music_Stop();

           // start from begining sequence

           game_seq_index=0;

           Music_Play((music_ptr)&song,game_sequence[game_seq_index]);

           } // end if music enabled

        // clear double buffer

        Fill_Double_Buffer_32(0);

        // load in instrument area

        PCX_Init((pcx_picture_ptr)&image_pcx);
        PCX_Load("krkcp.pcx", (pcx_picture_ptr)&image_pcx,1);
        PCX_Show_Buffer((pcx_picture_ptr)&image_pcx);
        PCX_Delete((pcx_picture_ptr)&image_pcx);

        // set up displays

        Draw_Scanner(SCANNER_CLEAR);
        Draw_Scanner(SCANNER_LOGO);

        Draw_Tactical(TACTICAL_CLEAR);
        Draw_Tactical(TACTICAL_DRAW);

        // reset color animation registers

        Misc_Color_Init();

        // enter into the main game loop

        while(game_state==GAME_RUNNING)
             {
             // compute starting time of this frame

             starting_time = Timer_Query();

             // keyboard input section

             // change ship velocity

             if (keyboard_state[MAKE_UP])
                {
                // speed up

                if ((ship_speed+=5)>55)
                    ship_speed=55;

                } // end if speed increase

             if (keyboard_state[MAKE_DOWN])
                {
                // slow down

                if ((ship_speed-=5)<-55)
                    ship_speed=-55;

                } // end if speed decrease

             // test for turns

             if (keyboard_state[MAKE_RIGHT])
                {
                // rotate ship to right

                if ((ship_yaw+=4)>=360)
                   ship_yaw-=360;

                } // end if

             if (keyboard_state[MAKE_LEFT])
                {

                // rotate ship to left

                if ((ship_yaw-=4)<0)
                   ship_yaw+=360;

                } // end if

             // test for weapons fire

             if (keyboard_state[MAKE_SPACE])
                {
                // fire a missile

                Start_Missile(PLAYER_OWNER,
                              &view_point,
                              &ship_direction,
                              ship_speed+30,
                              100);

                // illuminate instrument panel to denote a shot has
                // been fired

                Write_Color_Reg(PLAYERS_WEAPON_FIRE_REG,(RGB_color_ptr)&bright_red);

                // set timer to turn off illumination

                weapons_lit_count = 2;

                } // end if

              // instrumentation checks

              // left hand scanner

              if (keyboard_state[MAKE_S])
                 {
                 if (scanner_state==1)
                    {
                    // turn the scanner off

                    scanner_state=0;

                    // replace krk logo

                    Draw_Scanner(SCANNER_CLEAR);

                    Draw_Scanner(SCANNER_LOGO);

                    }
                 else
                    {
                    // turn the scanner on

                    scanner_state=1;

                    // clear the scanner area for radar inage

                    Draw_Scanner(SCANNER_CLEAR);

                    Digital_FX_Play(KRKSCN_VOC,1);


                    } // end else

                 } // end

             // right half tactical display

             if (keyboard_state[MAKE_T])
                {

                // test if tactical was off

                if (tactical_state==TACTICAL_MODE_OFF)
                   {
                   // playe sound

                   Digital_FX_Play(KRKTAC_VOC,1);


                   } // end if

                // toggle to next state of tactical display

                // clear tactical display before next state

                Draw_Tactical(TACTICAL_CLEAR);

                // move to next state

                if (++tactical_state>TACTICAL_MODE_OFF)
                   tactical_state = TACTICAL_MODE_STS;

                // based on new tactical state draw proper display

                Draw_Tactical(TACTICAL_DRAW);

                } // end if

             // heads up display

             if (keyboard_state[MAKE_H])
                {
                // toggle hud

                if (hud_state==1)
                   {
                   hud_state=0;

                   // set indicator to proper illumination

                   Write_Color_Reg(HUD_REG,(RGB_color_ptr)&dark_blue);

                   } // end if
                else
                   {
                   hud_state = 1;

                   // set indicator to proper illumination

                   Write_Color_Reg(HUD_REG,(RGB_color_ptr)&bright_blue);

                   Digital_FX_Play(KRKHUD_VOC,1);

                   } // end else

                } // end if

             // test for exit

             if (keyboard_state[MAKE_ESC])
                game_state=GAME_SETUP;


             // motion and control section

             // create a trajectory vector aligned with view direction

             ship_direction.x = sin_look[ship_yaw];
             ship_direction.y = 0;
             ship_direction.z = cos_look[ship_yaw];

             // move viewpoint based on ship trajectory

             view_point.x+=ship_direction.x*ship_speed;
             view_point.z+=ship_direction.z*ship_speed;

             // move objects here

             Move_Missiles();

             // move and perform AI for aliens

             Process_Aliens();

             // test ship against universe boundaries

             if (view_point.x>GAME_MAX_WORLD_X)
                 view_point.x=GAME_MAX_WORLD_X;
             else
             if (view_point.x<GAME_MIN_WORLD_X)
                 view_point.x=GAME_MIN_WORLD_X;

             if (view_point.z>GAME_MAX_WORLD_Z)
                 view_point.z=GAME_MAX_WORLD_Z;
             else
             if (view_point.z<GAME_MIN_WORLD_Z)
                 view_point.z=GAME_MIN_WORLD_Z;

             // add in vibrational noise due to terrain

             if (ship_speed)
                 view_point.y = 40+rand()%(1+abs(ship_speed)/8);
             else
                 view_point.y = 40;

             // test for ship hit message

             if (ship_message == SHIP_HIT)
                {
                // do screen shake

                view_point.y = 40+5*(rand()%10);

                // perform color fx

                // test if shake complete

                if (--ship_timer < 0)
                   {
                   // reset ships state

                   ship_message = SHIP_STABLE;

                   view_point.y = 40;

                   // reset colors

                   } // end if screen shake complete

                } // end if


             // transform stationary objects here

             Rotate_Object(&static_obj[STATIONS_TEMPLATE],0,10,0);

             Rotate_Object(&static_obj[TELEPODS_TEMPLATE],0,15,0);

             // set view angles based on trajectory of ship

             view_angle.ang_y = ship_yaw;

             // now that user has possible moved viewpoint, create the global
             // world to camera transformation matrix

             Create_World_To_Camera();

             // reset the polygon list

             Generate_Poly_List(NULL,RESET_POLY_LIST);

             // perform general 3-D pipeline for all 3-D objects

             // first draw stationary objects

             Draw_Stationary_Objects();

             // draw dynamic objects

             Draw_Aliens();

             // draw the missiles

             Draw_Missiles();

             // draw background

             Draw_Background((int)(ship_yaw*.885));

             // sort the polygons

             Sort_Poly_List();

             // draw the polygon list

             Draw_Poly_List();

             // draw the instruments here

             if (scanner_state==1)
                {
                // erase old blips

                Draw_Scanner(SCANNER_ERASE_BLIPS);

                // refresh scanner image

                Draw_Scanner(SCANNER_DRAW_BLIPS);

                } // end if

             // update tactical display

             if (tactical_state!=TACTICAL_MODE_OFF)
                 Draw_Tactical(TACTICAL_UPDATE);

             if (hud_state==1)
                Draw_Hud();

             // do special color fx

             // flicker the engines of the aliens

             Tallon_Color_FX();

             Slider_Color_FX();

             // strobe the perimeter barriers

             Barrier_Color_FX();

             // take care of weapons flash

             if (weapons_lit_count>0)
                {
                // test if it's time to to off flash

                if (--weapons_lit_count==0)
                   Write_Color_Reg(PLAYERS_WEAPON_FIRE_REG,(RGB_color_ptr)&black);

                } // end if

             // test if screen should be colored to simulate fire blast

             if (ship_message==SHIP_HIT)
                {
                // test for time intervals

                if (ship_timer>5 && (rand()%3)==1)
                   Fill_Double_Buffer_32(SHIP_FLAME_COLOR+rand()%16);

                } // end if ship hit

             // display double buffer

             Display_Double_Buffer_32(double_buffer,0);

             // lock onto 18 frames per second max

             while((Timer_Query()-starting_time)<1);

             // check on music

             if (music_enabled)
                {
                if (Music_Status()==2 || Music_Status()==0)
                   {

                   // advance to next sequence

                   if (++game_seq_index==21)
                      game_seq_index=0;

                   Music_Play((music_ptr)&song,game_sequence[game_seq_index]);

                    } // end if

                 } // end if music enabled

             } // end game loop

        // test if there is a winner or user just decided to exit

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

//GAME OVER SECTION////////////////////////////////////////////////////////////

// exit in a very cool way

Screen_Transition(SCREEN_SWIPE_X);

// free up all resources

Delete_Double_Buffer();

// close down FX

Digital_FX_Close();

// show the credits

Closing_Screen();

// close down music

Music_Close();

// remove the keyboard handler

Keyboard_Remove_Driver();

Set_Graphics_Mode(TEXT_MODE);

// see ya!

printf("\nKILL OR BE KILLED:Normal Shutdown.\n");

} // end main

