
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

// D E F I N E S /////////////////////////////////////////////////////////////

// defines for the radar scanner

#define SCANNER_X               12      // position of scanner
#define SCANNER_Y               135
#define SCANNER_WIDTH           57      // size of scanner area
#define SCANNER_HEIGHT          42

// commands that can be sent to the scanner engine

#define SCANNER_CLEAR           0       // clear the scanner
#define SCANNER_LOGO            1       // draw the KRK logo
#define SCANNER_ERASE_BLIPS     2       // erase all the radar blips
#define SCANNER_DRAW_BLIPS      3       // draw all the radar blips

// defines for the general multi function tactical display

#define TACTICAL_X              249     // position of tactical
#define TACTICAL_Y              134
#define TACTICAL_WIDTH          57      // size of tactical
#define TACTICAL_HEIGHT         34

// different modes of operation for the tactical display

#define TACTICAL_MODE_STS       0       // ship status mode
#define TACTICAL_MODE_HULL      1       // hull damage report
#define TACTICAL_MODE_OFF       2       // tactical display off

// these are the different icon indexes for the tactical display

#define TACTICAL_ICON_BLANK     0       // a black square
#define TACTICAL_ICON_KRK       1       // the kill or be killed logo
#define TACTICAL_ICON_GAUGE     2       // the gauge set
#define TACTICAL_ICON_TALLON    3       // the tallon hull
#define TACTICAL_ICON_SLIDER    4       // the slider hull

#define TACTICAL_ICON_WIDTH     38      // size of sprite bitmaps for tactical
#define TACTICAL_ICON_HEIGHT    28

// commands the drawing engine of the tactical display

#define TACTICAL_CLEAR          0       // clear the tactical
#define TACTICAL_DRAW           1       // draw the tactical
#define TACTICAL_UPDATE         2       // refresh the tactical

// the main menu selection system defines

#define SELECT_BOX_SX           104     // starting position of selection boxes
#define SELECT_BOX_SY           58
#define SELECT_BOX_DY           19      // vertical change between boxes
#define SELECT_BOX_WIDTH        94      // dimensions of selection boxes
#define SELECT_BOX_HEIGHT       10

#define SELECT_LGT_SX           86      // starting position of little lights by box
#define SELECT_LGT_SY           61
#define SELECT_LGT_DY           19      // vertical change to next light
#define SELECT_LGT_WIDTH        4       // dimensions of light
#define SELECT_LGT_HEIGHT       4

#define MAX_SELECTION           3       // maximum number of selections

// defines for the background moutain scape

#define MOUNTAIN_HEIGHT         43      // height of mountain
#define MOUNTAIN_WIDTH          320     // total width of backdrop
#define MOUNTAIN_Y_POS          56      // starting y position to map down backdrop

// starting position of the intro startup sequence

#define START_MESS_X            2
#define START_MESS_Y            8

// position of alpha numeric input area

#define DISPLAY_X              0
#define DISPLAY_Y              0


// defines for the aliens and logic

#define ALIEN_DEAD              0      // alien is dead
#define ALIEN_DYING             1      // alien is dying
#define ALIEN_NEW_STATE         2      // alien wishes a new state
#define ALIEN_ATTACK            3      // alien is attacking player
#define ALIEN_RANDOM            4      // alien is moving in random directions
#define ALIEN_EVADE             5      // alien is evading player
#define ALIEN_STOP              6      // alien is at a full stop
#define ALIEN_TURN              7      // alien is making a turn

#define NUM_ALIENS              4      // total number of aliens in game

#define ALIEN_TALLON            0      // id for an alien with a tallon class ship
#define ALIEN_SLIDER            1      // is for an alien with a slider class ship

// dimesions of the game world

#define GAME_MAX_WORLD_X        7500    // x dimensions
#define GAME_MIN_WORLD_X        -7500

#define GAME_MAX_WORLD_Y        400     // y dimensions
#define GAME_MIN_WORLD_Y        -400

#define GAME_MAX_WORLD_Z        7500    // z dimensions
#define GAME_MIN_WORLD_Z        -7500

#define GUN_HEIGHT              -10    // height of weapon system from ground level

#define NUM_DYNAMIC              3     // total number of dynamic game objects

#define NUM_MISSILES             12    // maxium number of missiles in the game

// possible missle states

#define MISSILE_INACTIVE         0     // an inactive missile
#define MISSILE_ACTIVE           1     // an active and moving moving

#define MAX_PLAYER_MISSILES      6     // maximum number of missiles player
                                       // can fire at once

// used to track who fired a missile, helps for collision detection

#define NO_OWNER                 0     // no owner, can cause damage to anyone
#define PLAYER_OWNER             1     // the player fired it
#define ALIEN_OWNER              2     // an alien fired it
#define OTHER_OWNDER             3     // an unknown fired it

// these are used to index into the 3-D models

#define MISSILES_TEMPLATE        0     // the missile model
#define TALLONS_TEMPLATE         1     // the tallon model
#define SLIDERS_TEMPLATE         2     // the slider model


// these define the number of each object type

#define NUM_STATIONARY           6    // total number of stationary object types

#define NUM_OBSTACLES_1          32   // number of rocks
#define NUM_OBSTACLES_2          32   // number of crystals
#define NUM_BARRIERS             8    // number of lasrer barriers
#define NUM_TOWERS               4    // number of control towers
#define NUM_STATIONS             1    // number of power stations
#define NUM_TELEPODS             4    // number of telepods

// these are the model indices

#define OBSTACLES_1_TEMPLATE     0    // the rock model
#define OBSTACLES_2_TEMPLATE     1    // the crystal model
#define BARRIERS_TEMPLATE        2    // the laser barrier model
#define TOWERS_TEMPLATE          3    // the control tower model
#define STATIONS_TEMPLATE        4    // the power station model
#define TELEPODS_TEMPLATE        5    // the telepod model

// digital sound system

#define KRKMIS_VOC                  0  // the sound a missile makess
#define KRKEMIS_VOC                 1  // the sound an enemy missile makes
#define KRKTAC_VOC                  2  // tactical engaging
#define KRKSCN_VOC                  3  // scanner engaging
#define KRKHUD_VOC                  4  // hud engaging
#define KRKPOW_VOC                  5  // powering up
#define KRKKEY_VOC                  6  // a key was pressed
#define KRKEX1_VOC                  7  // the alien explosion
#define KRKEX2_VOC                  8  // the blast of an alien

#define NUM_SOUND_FX                9      // the number of sound fx loaded in

// color fx registers for a multitude of objects

#define SHIELDS_REG                 232    // the insrument panel shields color register
#define RADAR_REG                   233    // the insrument panel radar color register
#define COMM_REG                    234    // the insrument panel communications color register
#define HUD_REG                     235    // the insrument panel hud color register

#define STS_REG                     236    // the insrument panel ship status color register
#define HULL_REG                    237    // the insrument panel hull color register
#define OFF_REG                     238    // the insrument panel off color register

#define PLAYERS_WEAPON_FIRE_REG     239    // the little light that glows when the player fires

#define ENGINES_TALLON_REG          240    // the engine flicker for tallons
#define ENGINES_SLIDER_REG          241    // the engine flicker for sliders
#define BARRIERS_REG                242    // the laser barriers
#define SHIELDS_FLICKER_REG         243    // the shields (not implemented)

#define START_PANEL_REG             224    // the start register for the main menu fx
#define END_PANEL_REG               (224+7)// the end register for the main menu fx

#define SELECT_REG                  254    // the glowing currently selected main menu item

#define ALIEN_EXPL_BASE_REG         244    // the aliens that are killed all
                                           // get allocated a color register at this base

// defines for the grdient sky and ground

#define SKY_COLOR_1                 50     // reds
#define SKY_COLOR_2                 52
#define SKY_COLOR_3                 53

#define GND_COLOR_1                 216    // browns
#define GND_COLOR_2                 215
#define GND_COLOR_3                 214

// music system

#define NUM_INTRO_SEQUENCES    11 // the number of elements in intro music score
#define NUM_GAME_SEQUENCES     18 // the number of elements in game music score

// defines for briefing instructions

#define NUM_PAGES              8
#define NUM_LINES_PAGE         17
#define NUM_SHIP_SPECS         7


// state of the game itself

#define GAME_SETUP             0   // the game is in the setup mode
#define GAME_LINKING           1   // the communications link is being established
#define GAME_RUNNING           2   // the game is running
#define GAME_PAUSED            3   // the game is paused (not implemented)
#define GAME_OVER              4   // what do you think

// general object states

#define DEAD                   0   // these are general states for any
#define ALIVE                  1   // object
#define DYING                  2

// defines for the ship state machine

#define SHIP_STABLE            0   // the ship is stable
#define SHIP_HIT               1   // the ship has been hit
#define SHIP_TELEPORT          2   // the ship is teleporting (not implemented)

#define SHIP_FLAME_COLOR       96  // odd ball, the starting base color register
                                   // that is used to flicker the screen to simulate
                                   // a torpedo blast

// used to indicate which ship type the player is using

#define PLAYER_TALLON          0
#define PLAYER_SLIDER          1

// defines for setup selections

#define SETUP_CHALLENGE             0
#define SETUP_SELECT_MECH           1
#define SETUP_RULES                 2
#define SETUP_EXIT                  3

#define NUM_SETUP                   4   // number of setup choices

// size of the "tech" font used in intro

#define TECH_FONT_WIDTH             4   // width of high tech font
#define TECH_FONT_HEIGHT            7   // height of high tech font
#define NUM_TECH_FONT               64  // number of characters in tech font

// S T R U C T U R E S ///////////////////////////////////////////////////////

// this structure is used to replicate static similar objects based on the
// same model

typedef struct fixed_obj_typ
        {
        int state;        // state of object
        int rx,ry,rz;     // rotation rate of object
        float x,y,z;      // position of object

        } fixed_obj, *fixed_obj_ptr;

// this structure is used for all projectiles in the game, similar
// to the above structure except with added fields for animation, collision,
// etc.

typedef struct proj_obj_typ
        {

        int state;            // state of projectile
        int owner;            // owner of projectile
        int lifetime;         // lifetime of projectile

        vector_3d direction;  // direction and velocity of projectile

        float x,y,z;          // position of projectile

        } missile_obj, proj_obj, *missile_obj_ptr, *proj_obj_ptr;

// this data structure is used to hold and alien attacker

typedef struct alien_typ
        {
        int state;                // state of alien
        int counter_1;            // counters
        int counter_2;
        int threshold_1;          // thresholds for counters
        int threshold_2;
        int aux_1;                // auxialliary variables for whatever
        int aux_2;
        int color_reg;            // color register of alien during explosion
        RGB_color color;          // actual rgb color of explosion
        int type;                 // type of alien
        int angular_heading;      // curr angle about y axis, i.e. yaw

        int speed;                // speed of ship

        vector_3d direction;      // the current trajectory vector

        float x,y,z;              // position of alien

        } alien, *alien_ptr;

// P R O T O T Y P E S //////////////////////////////////////////////////////

void Select_Mech(void);

void Misc_Color_Init(void);

void Tallon_Color_FX(void);

void Slider_Color_FX(void);

void Barrier_Color_FX(void);

void Draw_Hud(void);

void Draw_Tactical(int command);

void Load_Tactical(void);

void Draw_Scanner(int command);

void Init_Missiles(void);

void Move_Missiles(void);

void Draw_Missiles(void);

int Start_Missile(int owner,
                  vector_3d_ptr pos,
                  vector_3d_ptr dir,
                  int speed,
                  int lifetime);

void Init_Aliens(void);

void Process_Aliens(void);

void Draw_Aliens(void);

void Draw_Background(int mountain_pos);

void Draw_Box(int x1,int y1,int x2, int y2,int color);

void Tech_Print(int x,int y,char *string,unsigned char far *destination);

void Font_Engine_1(int x,int y,
                   int font,int color,
                   char *string,unsigned char far *destination);

void Panel_FX(int reset);

void Intro_Planet(void);

void Closing_Screen(void);

void Intro_Waite(void);

void Intro_KRK(void);

void Intro_Controls(void);

void Intro_Briefing(void);

void Reset_System(void);

void Music_Init(void);

void Music_Close(void);

void Digital_FX_Init(void);

void Digital_FX_Close(void);

int Digital_FX_Play(int the_effect, int priority);

int Parse_Commands(int argc, char **argv);

void Draw_Stationary_Objects(void);

void Set_3D_View(void);

void Load_3D_Objects(void);

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

sound digital_FX[NUM_SOUND_FX];

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

void Select_Mech(void)
{
// this function allows the user to select a ship via the keyboard
// the function draw the ship in wire frame, displays the specifications
// of the ship and then allows the user to select or escape

int done=0,               // event loop exit flag
    current_ship_type,    // current displayed ship
    index;                // looping variable

// set current ship to the actual one player is using

current_ship_type = players_ship_type;

// clear the screen

Fill_Screen(0);

// make sure user isn't hitting the enter key still

while(keys_active);

// enter into main event loop

while(!done)
     {
     // clear the double buffer

     Fill_Double_Buffer_32(0);

     // get user input

     if (keys_active)
        {

        // test for various inputs

        if (keyboard_state[MAKE_RIGHT])
           {
           // move to next ship

           // click the next button

           if (++current_ship_type==2)
              current_ship_type=0;

           // make a key sound

           Digital_FX_Play(KRKKEY_VOC,3);

           } // end if
        else
        if (keyboard_state[MAKE_LEFT])
           {
           // move to previous ship

           // click the previous button

           if (--current_ship_type<0)
              current_ship_type=1;

           // make a key sound

           Digital_FX_Play(KRKKEY_VOC,3);

           } // end if
        else
        if (keyboard_state[MAKE_ENTER])
           {
           // select the current ship

           // make players ship the selected ship

           players_ship_type = current_ship_type;

           // exit

           done = 1;

           // make a key sound

           Digital_FX_Play(KRKKEY_VOC,3);

           } // end if
        else
        if (keyboard_state[MAKE_ESC])
           {
           // exit this menu and go back to main menu

           // exit

           done = 1;

           // make a key sound

           Digital_FX_Play(KRKKEY_VOC,3);

           } // end if

        } // end if keys active

     // draw the object and it's stats

     if (current_ship_type==PLAYER_TALLON)
        {
        // change heading of tallon model

        if ((dynamic_obj[TALLONS_TEMPLATE].state+=5)>360)
            dynamic_obj[TALLONS_TEMPLATE].state-=360;

        // perform the rotation

        Rotate_Object(&dynamic_obj[TALLONS_TEMPLATE],0,5,0);

        // update object template with new heading

        // set world position to a reasonable spot

        dynamic_obj[TALLONS_TEMPLATE].world_pos.x = -50;
        dynamic_obj[TALLONS_TEMPLATE].world_pos.y = 0;
        dynamic_obj[TALLONS_TEMPLATE].world_pos.z = 400;

        // convert object local coordinates to world coordinates

        Local_To_World_Object(&dynamic_obj[TALLONS_TEMPLATE]);

        // draw the object in wireframe

        Draw_Object_Wire(&dynamic_obj[TALLONS_TEMPLATE],12);

        // draw the stats

        Font_Engine_1(0,130,0,12,"TALLON SPECIFICATIONS",video_buffer);

        // draw the specfication strings

        for (index=0; index<NUM_SHIP_SPECS; index++)
            Font_Engine_1(4,140+8*index,0,12,tallon_specs[index],video_buffer);

        } // end
     else
        {
        // else must be a slider

        // change heading of slider model

        if ((dynamic_obj[SLIDERS_TEMPLATE].state+=5)>360)
            dynamic_obj[SLIDERS_TEMPLATE].state-=360;

        // perform the rotation

        Rotate_Object(&dynamic_obj[SLIDERS_TEMPLATE],0,5,0);

        // set world position to a reasonable spot

        dynamic_obj[SLIDERS_TEMPLATE].world_pos.x = -50;
        dynamic_obj[SLIDERS_TEMPLATE].world_pos.y = 0;
        dynamic_obj[SLIDERS_TEMPLATE].world_pos.z = 400;

        // convert object local coordinates to world coordinates

        Local_To_World_Object(&dynamic_obj[SLIDERS_TEMPLATE]);

        // draw the object in wireframe

        Draw_Object_Wire(&dynamic_obj[SLIDERS_TEMPLATE],12);

        // draw the stats

        Font_Engine_1(0,130,0,12,"SLIDER SPECIFICATIONS",video_buffer);

        // draw the specfication strings

        for (index=0; index<NUM_SHIP_SPECS; index++)
            Font_Engine_1(4,140+8*index,0,12,slider_specs[index],video_buffer);

        } // end else

     // draw the buttons

     buttons_spr.x          = 260;
     buttons_spr.y          = 20;
     buttons_spr.curr_frame = 4;

     Sprite_Draw((sprite_ptr)&buttons_spr,double_buffer,0);

     buttons_spr.y          = 40;
     buttons_spr.curr_frame = 6;

     Sprite_Draw((sprite_ptr)&buttons_spr,double_buffer,0);

     buttons_spr.y          = 60;
     buttons_spr.curr_frame = 2;

     Sprite_Draw((sprite_ptr)&buttons_spr,double_buffer,0);

     buttons_spr.y          = 80;
     buttons_spr.curr_frame = 0;

     Sprite_Draw((sprite_ptr)&buttons_spr,double_buffer,0);

     // draw the headers

     Font_Engine_1(60,0,0,12,"KILL OR BE KILLED BATTLE MECH SELECTION",double_buffer);

     // display double buffer

     Display_Double_Buffer_32(double_buffer,0);

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

     } // end while

} // end Select_Mech

/////////////////////////////////////////////////////////////////////////////

void Misc_Color_Init(void)
{
// this function initializes various color registers for the game phase of
// KRK

Write_Color_Reg(SHIELDS_REG,                  (RGB_color_ptr)&dark_blue);
Write_Color_Reg(RADAR_REG,                    (RGB_color_ptr)&dark_blue);
Write_Color_Reg(COMM_REG,                     (RGB_color_ptr)&dark_blue);
Write_Color_Reg(HUD_REG,                      (RGB_color_ptr)&dark_blue);
Write_Color_Reg(STS_REG,                      (RGB_color_ptr)&dark_green);
Write_Color_Reg(HULL_REG,                     (RGB_color_ptr)&dark_green);
Write_Color_Reg(OFF_REG,                      (RGB_color_ptr)&bright_green);
Write_Color_Reg(PLAYERS_WEAPON_FIRE_REG,      (RGB_color_ptr)&black);
Write_Color_Reg(ENGINES_TALLON_REG,           (RGB_color_ptr)&black);
Write_Color_Reg(ENGINES_SLIDER_REG,           (RGB_color_ptr)&black);
Write_Color_Reg(BARRIERS_REG,                 (RGB_color_ptr)&black);
Write_Color_Reg(SHIELDS_FLICKER_REG,           (RGB_color_ptr)&black);

} // end Misc_Color_Init

////////////////////////////////////////////////////////////////////////////

void Tallon_Color_FX(void)
{
// this function flickers the tallons engines

static int engine_counter=0; // used to track time from call to call

// test if it's time to change color

if (++engine_counter==1)
   Write_Color_Reg(ENGINES_TALLON_REG,(RGB_color_ptr)&bright_blue);
else
if (engine_counter==2)
   Write_Color_Reg(ENGINES_TALLON_REG,(RGB_color_ptr)&medium_blue);
else
if (engine_counter==3)
   engine_counter=0;


} // end Tallon_Color_FX

/////////////////////////////////////////////////////////////////////////////

void Slider_Color_FX(void)
{
// this function flickers the sliders engines

static int engine_counter=0; // used to track time from call to call

// test if it's time to change color

if (++engine_counter==1)
   Write_Color_Reg(ENGINES_SLIDER_REG,(RGB_color_ptr)&bright_green);
else
if (engine_counter==2)
   Write_Color_Reg(ENGINES_SLIDER_REG,(RGB_color_ptr)&medium_green);
else
if (engine_counter==3)
   engine_counter=0;

} // end Slider_Color_FX

/////////////////////////////////////////////////////////////////////////////

void Barrier_Color_FX(void)
{
// this function flickers the game grid barriers beacon

static int beacon_counter=0; // used to track time from call to call

// test if it's time to change color

if (++beacon_counter==1)
   Write_Color_Reg(BARRIERS_REG,(RGB_color_ptr)&bright_green);
else
if (beacon_counter==10)
   Write_Color_Reg(BARRIERS_REG,(RGB_color_ptr)&dark_green);
else
if (beacon_counter==25)
   beacon_counter=0;

} // end Barrier_Color_FX

/////////////////////////////////////////////////////////////////////////////

void Draw_Hud(void)
{
// this function draws the heads up display

char buffer[64]; // local working buffer

// create the string with the x,y,z and heading in it

sprintf(buffer,"X(%5d) Y(%5d) Z(%5d) TRAJ(%4d) KILLS:(%3d)",(int)view_point.x,
                                                            (int)view_point.y,
                                                            (int)view_point.z,
                                                            (int)ship_yaw,
                                                            (int)ship_kills);
// print the string to the double buffer

Font_Engine_1(16,10,0,12,buffer,double_buffer);

} // end Draw_Hud

/////////////////////////////////////////////////////////////////////////////

void Draw_Tactical(int command)
{
// this function is responsible for both draw the static tactical displays
// and updating the currently active tactical display

int index,    // looping variable
    color,    // holds a temp color
    length;   // used to hold length of indicator bars

// test command and see what caller wants done

switch(command)
      {

      case TACTICAL_CLEAR:
           {
           // totally clear the tactical display

           Draw_Rectangle(TACTICAL_X,
                          TACTICAL_Y,
                          TACTICAL_X+TACTICAL_WIDTH-1,
                          TACTICAL_Y+TACTICAL_HEIGHT-1,
                          0);


           } break;

      case TACTICAL_DRAW:
           {
           // based on tactical to be displayed render static portion
           // of it

           if (tactical_state==TACTICAL_MODE_STS)
              {
              tactical_spr.curr_frame = 2;

              // update button illuminations

              Write_Color_Reg(STS_REG, (RGB_color_ptr)&bright_green);
              Write_Color_Reg(HULL_REG,(RGB_color_ptr)&dark_green);
              Write_Color_Reg(OFF_REG, (RGB_color_ptr)&dark_green);

              } // end if

           else
           if (tactical_state==TACTICAL_MODE_HULL)
              {
              // which hull should be displayed

              if (players_ship_type==PLAYER_TALLON)
                 tactical_spr.curr_frame = 3;
              else
                 tactical_spr.curr_frame = 4;

              // update button illuminations

              Write_Color_Reg(STS_REG, (RGB_color_ptr)&dark_green);
              Write_Color_Reg(HULL_REG,(RGB_color_ptr)&bright_green);
              Write_Color_Reg(OFF_REG, (RGB_color_ptr)&dark_green);


              } // end if
           else
              {
              tactical_spr.curr_frame = 1;

              // update button illuminations

              Write_Color_Reg(STS_REG, (RGB_color_ptr)&dark_green);
              Write_Color_Reg(HULL_REG,(RGB_color_ptr)&dark_green);
              Write_Color_Reg(OFF_REG, (RGB_color_ptr)&bright_green);

              } // end else

           // now draw display

           tactical_spr.x = TACTICAL_X + 8;
           tactical_spr.y = TACTICAL_Y + 6;

           Sprite_Draw((sprite_ptr)&tactical_spr, video_buffer,0);

           } break;

      case TACTICAL_UPDATE:
           {
           // based on tactical state update display accordingly

           switch(tactical_state)
                 {

                 case TACTICAL_MODE_STS:
                      {

                      // draw velocity gauge

                      length = (ship_speed<<1)/5;

                      // test for negative values

                      if (length<0)
                         {
                         // invert value

                         length = -length;

                         // show negative velocity with red

                         color = 32;

                         } // end if
                      else
                         {
                         // show positive velocity with red

                         color = 144;

                         } // end else

                      // do a little out of bounds check

                      if (length>20)
                         length = 20;

                      // draw visible portion of digital indicator

                      if (length>0)
                         {

                         Line_H(TACTICAL_X+8+16,
                                TACTICAL_X+8+16+length,
                                TACTICAL_Y+6+5,
                                color);

                         Line_H(TACTICAL_X+8+16,
                                TACTICAL_X+8+16+length,
                                TACTICAL_Y+6+5+1,
                                color);
                         }

                      // undraw old line (if any)

                      if (++length<=20)
                         {
                         // erase old line

                         Line_H(TACTICAL_X+8+16+length,
                                TACTICAL_X+8+16+20,
                                TACTICAL_Y+6+5,
                                0);

                         Line_H(TACTICAL_X+8+16+length,
                                TACTICAL_X+8+16+20,
                                TACTICAL_Y+6+5+1,
                               0);

                         } // end if erase needed

                      // draw damage gauge

                      length = (ship_damage<<1)/5;

                      // check for negative values

                      if (length<0)
                         length = -length;

                      // draw visible portion of digital indicator

                      if (length>0)
                         {

                         Line_H(TACTICAL_X+8+16,
                                TACTICAL_X+8+16+length,
                                TACTICAL_Y+6+5+8,
                                96);

                         Line_H(TACTICAL_X+8+16,
                                TACTICAL_X+8+16+length,
                                TACTICAL_Y+6+5+8+1,
                                96);

                         } // end if

                      if (++length<=20)
                         {
                         // erase old line

                         Line_H(TACTICAL_X+8+16+length,
                                TACTICAL_X+8+16+20,
                                TACTICAL_Y+6+5+8,
                                0);

                         Line_H(TACTICAL_X+8+16+length,
                                TACTICAL_X+8+16+20,
                                TACTICAL_Y+6+5+8+1,
                               0);

                         } // end if erase needed

                      // draw energy gauge

                      length = (ship_energy<<1)/5;

                      // check for negative values

                      if (length<0)
                         length = -length;

                      // draw visible portion of digital indicator

                      if (length>0)
                         {

                         Line_H(TACTICAL_X+8+16,
                                TACTICAL_X+8+16+length,
                                TACTICAL_Y+6+5+16,
                                96);

                         Line_H(TACTICAL_X+8+16,
                                TACTICAL_X+8+16+length,
                                TACTICAL_Y+6+5+16+1,
                                96);
                         }

                      // undraw remainder of gauge

                      if (++length<=20)
                         {
                         // erase old line

                         Line_H(TACTICAL_X+8+16+length,
                                TACTICAL_X+8+16+20,
                                TACTICAL_Y+6+5+16,
                                0);

                         Line_H(TACTICAL_X+8+16+length,
                                TACTICAL_X+8+16+20,
                                TACTICAL_Y+6+5+16+1,
                               0);

                         } // end if erase needed

                      } break;

                 case TACTICAL_MODE_HULL:
                      {
                      // do nothing for now

                      } break;

                 case TACTICAL_MODE_OFF:
                      {
                      // do nothing for now

                      } break;

                 default:break;

                 } // end switch

           } break;

      default:break;

      } // end switch

} // end Draw_Tactical

/////////////////////////////////////////////////////////////////////////////

void Load_Tactical(void)
{
// this function loads various icons for the tactical displays

int index; // looping variable

// load the imagery for the icons for display

PCX_Init((pcx_picture_ptr)&image_pcx);
PCX_Load("krkdis.pcx", (pcx_picture_ptr)&image_pcx,1);

// intialize the tactical sprite

Sprite_Init((sprite_ptr)&tactical_spr,0,0,38,28,0,0,0,0,0,0);

// extract the bitmaps for heads up text

for (index=0; index<5; index++)
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,
                   (sprite_ptr)&tactical_spr,index,index,0);

// delete pcx file

PCX_Delete((pcx_picture_ptr)&image_pcx);

// load the imagery for the control buttons

PCX_Init((pcx_picture_ptr)&image_pcx);
PCX_Load("krkbutt.pcx", (pcx_picture_ptr)&image_pcx,1);

// intialize the tactical sprite

Sprite_Init((sprite_ptr)&buttons_spr,0,0,42,12,0,0,0,0,0,0);

// extract the bitmaps for heads up text

for (index=0; index<8; index++)
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,
                   (sprite_ptr)&buttons_spr,index,index%4,index/4);

// delete pcx file

PCX_Delete((pcx_picture_ptr)&image_pcx);

} // end Load_Tactical

//////////////////////////////////////////////////////////////////////////////

void Draw_Scanner(int command)
{

// this function draws the scanner, it has three modes, clear, erase blips,
// and draw blips

int index,  // looping variable
    xb,yb;  // temporary blip locations

// holds the scanner clips

static int blip_x[24],      // these arrays hold the blips from call to call
           blip_y[24],
           active_blips=0;  // totall number of active blips this frame

// what is the command

if (command==SCANNER_CLEAR)
   {
   // clear the scanner image

   Draw_Rectangle(SCANNER_X,
                  SCANNER_Y,
                  SCANNER_X+SCANNER_WIDTH-1,
                  SCANNER_Y+SCANNER_HEIGHT-1,
                  0);

   // reset number of blips

   active_blips=0;

   // exit

   return;

   } // end if clear

if (command==SCANNER_LOGO)
   {
   // clear the scanner surface off

   tactical_spr.curr_frame = 1;
   tactical_spr.x          = SCANNER_X + 8;
   tactical_spr.y          = SCANNER_Y + 6;

   Sprite_Draw((sprite_ptr)&tactical_spr, video_buffer,0);

   // reset number of blips

   active_blips=0;

   // exit

   return;

   } // end if

// now determine if scanner is being drawn or erased

if (command==SCANNER_ERASE_BLIPS)
   {
   // erase all the scanner blips

   // simply loop thru blips and erase them

   for (index=0; index<active_blips; index++)
       Write_Pixel(blip_x[index],blip_y[index],0);

   return;

   } // end if
else
   {
   // reset number of active blips

   active_blips = 0;

   // draw all the scanner blips

   // first barriers

   for (index=0; index<NUM_BARRIERS; index++)
       {
       // compute blip position

       xb = SCANNER_X+(8000+barriers[index].x)/282;
       yb = SCANNER_Y+(8000-barriers[index].z)/382;

       // draw the blip

       Write_Pixel(xb,yb,7);

       // save the blip

       blip_x[active_blips] = xb;
       blip_y[active_blips] = yb;

       // increment number of blips

       active_blips++;

       } // end for index

   // now telepods

   for (index=0; index<NUM_TELEPODS; index++)
       {
       // compute blip position

       xb = SCANNER_X+(8000+telepods[index].x)/282;
       yb = SCANNER_Y+(8000-telepods[index].z)/382;

       // draw the blip

       Write_Pixel(xb,yb,5);

       // save the blip

       blip_x[active_blips] = xb;
       blip_y[active_blips] = yb;

       // increment number of blips

       active_blips++;

       } // end for index

   // now power stations

   for (index=0; index<NUM_STATIONS; index++)
       {
       // compute blip position

       xb = SCANNER_X+(8000+stations[index].x)/282;
       yb = SCANNER_Y+(8000-stations[index].z)/382;

       // draw the blip

       Write_Pixel(xb,yb,13);

       // save the blip

       blip_x[active_blips] = xb;
       blip_y[active_blips] = yb;

       // increment number of blips

       active_blips++;

       } // end for index

   // now aliens

   for (index=0; index<NUM_ALIENS; index++)
       {

       if (aliens[index].state!=ALIEN_DEAD)
          {

          // compute blip position

          xb = SCANNER_X+(8000+aliens[index].x)/282;
          yb = SCANNER_Y+(8000-aliens[index].z)/382;

          // draw the blip

          Write_Pixel(xb,yb,12);

          // save the blip

          blip_x[active_blips] = xb;
          blip_y[active_blips] = yb;

          // increment number of blips

          active_blips++;

          } // end if

       } // end for index

   // finally the player

   // compute blip position

   xb = SCANNER_X+(8000+view_point.x)/282;
   yb = SCANNER_Y+(8000-view_point.z)/382;

   // draw the blip

   Write_Pixel(xb,yb,9);

   // save the blip

   blip_x[active_blips] = xb;
   blip_y[active_blips] = yb;

   // increment number of blips

   active_blips++;

   } // end else

} // end Draw_Scanner

//////////////////////////////////////////////////////////////////////////////

void Init_Missiles(void)
{
// this function resets the missile array and gets them ready for use

int index; // looping variable

// initialize all missiles to a known state

for (index=0; index<NUM_MISSILES; index++)
    {

    missiles[index].state       = MISSILE_INACTIVE;
    missiles[index].owner       = NO_OWNER;
    missiles[index].lifetime    = 0;

    missiles[index].direction.x = 0;
    missiles[index].direction.y = 0;
    missiles[index].direction.z = 0;

    missiles[index].x           = 0;
    missiles[index].y           = 0;
    missiles[index].z           = 0;

    } // end for index

// reset number of active missiles

active_player_missiles = 0;
total_active_missiles  = 0;

} // end Init_Missiles

/////////////////////////////////////////////////////////////////////////////

void Move_Missiles(void)
{
// this function moves all the missiles

int index,            // looping variables
    index_2,
    radius_tallon,    // used to hold the radius of the ships
    radius_slider,
    radius_player,
    dx,               // used during distance calculations
    dz,
    dist,
    min;

// pre-compute radi of alien ship types

radius_tallon = (0.75*dynamic_obj[TALLONS_TEMPLATE].radius);
radius_slider = (0.75*dynamic_obj[SLIDERS_TEMPLATE].radius);

// compute radius of players ship

if (players_ship_type==PLAYER_TALLON)
   radius_player = dynamic_obj[TALLONS_TEMPLATE].radius;
else
   radius_player = dynamic_obj[SLIDERS_TEMPLATE].radius;

// move all the missiles and test for collisions

for (index=0; index<NUM_MISSILES; index++)
    {
    // for each missile that is active, move it and test it against bounds
    // and lifetime

    if (missiles[index].state==MISSILE_ACTIVE)
       {
       // process missile

       // first move missile

       missiles[index].x+=missiles[index].direction.x;
       missiles[index].y+=missiles[index].direction.y;
       missiles[index].z+=missiles[index].direction.z;

       // test for collisions with aliens if player fired this missile

       if (missiles[index].owner==PLAYER_OWNER)
          {
          for (index_2=0; index_2<NUM_ALIENS; index_2++)
              {

              // test if alien is not dead or dying

              if (aliens[index_2].state!=ALIEN_DEAD &&
                  aliens[index_2].state!=ALIEN_DYING)
                 {

                 // test if missiles center is within bounding radius
                 // of alien ship

                 // compute distance based on taylor expansion about 12% error
                 // max

                 // first find |dx| and |dz|

                 if ( (dx = ((int)aliens[index_2].x - (int)missiles[index].x) ) < 0)
                    dx=-dx;

                 if ( (dz = ((int)aliens[index_2].z - (int)missiles[index].z)) < 0)
                    dz=-dz;

                 // compute minimum delta

                 if (dx<=dz)
                    min=dx;
                 else
                    min=dz;

                 // compute distance

                 dist = dx + dz - (min >> 1);

                 // test distance against average radius of alien ship

                 if (aliens[index_2].type==ALIEN_TALLON)
                    {
                    if (dist<=radius_tallon)
                       {
                       // kill missile

                       missiles[index].lifetime=-1;

                       // send message to aliens that this one is dying

                       aliens[index_2].state       = ALIEN_DYING;
                       aliens[index_2].counter_1   = 0;
                       aliens[index_2].threshold_1 = 20;

                       aliens[index_2].speed         = 0;

                       aliens[index_2].color.red   = 0;
                       aliens[index_2].color.green = 0;
                       aliens[index_2].color.blue  = 0;

                       // lets hear him fry

                       Digital_FX_Play(KRKEX1_VOC,0);

                       // add one more notch to my 6 shooter!

                       ship_kills++;

                       // move to next missile

                       continue;

                       } // end if

                    } // end if tallon
                 else
                 if (aliens[index_2].type==ALIEN_SLIDER)
                    {
                    if (dist<=radius_slider)
                       {
                       // kill missile

                       missiles[index].lifetime=-1;

                       // send message to aliens that this one is dying

                       aliens[index_2].state       = ALIEN_DYING;
                       aliens[index_2].counter_1   = 0;
                       aliens[index_2].threshold_1 = 20;

                       aliens[index_2].speed       = 0;

                       aliens[index_2].color.red   = 0;
                       aliens[index_2].color.green = 0;
                       aliens[index_2].color.blue  = 0;

                       // lets hear him fry

                       Digital_FX_Play(KRKEX1_VOC,0);

                       // add one more notch to my 6 shooter!

                       ship_kills++;

                       // move to next missile

                       continue;

                       } // end if

                    } // end if slider

                 } // end if alien not dead

              } // end for

          } // end if player fired missile
       else
          {
          // this missile must be an aliens, so test it against ship

          if ( (dx = ((int)view_point.x - (int)missiles[index].x) ) < 0)
             dx=-dx;

          if ( (dz = ((int)view_point.z - (int)missiles[index].z)) < 0)
             dz=-dz;

          // compute minimum delta

          if (dx<=dz)
             min=dx;
          else
             min=dz;

          // compute distance

          dist = dx + dz - (min >> 1);

          // test for collision

          if (dist<=radius_player)
             {
             // send message to player

             ship_message = SHIP_HIT;
             ship_timer   = 25;

             // add some sound

             Digital_FX_Play(KRKEX2_VOC,0);

             // add some damage

             if (++ship_damage>50)
                ship_damage=50;

             // kill missile

             missiles[index].lifetime=-1;

             } // end if collision

          } // end else alien

       // test for out of bounds

       if (missiles[index].x > GAME_MAX_WORLD_X+500 ||
           missiles[index].x < GAME_MIN_WORLD_X-500 ||

           missiles[index].z > GAME_MAX_WORLD_Z+500 ||
           missiles[index].z < GAME_MIN_WORLD_Z-500 )
           {

           // de-activate missile

           missiles[index].state = MISSILE_INACTIVE;

           if (missiles[index].owner==PLAYER_OWNER)
              active_player_missiles--;

           // decrement total always

           total_active_missiles--;

           } // end if
       else
       if ((--missiles[index].lifetime)<0)
           {
           // missile has died out (ran out of energy)

           missiles[index].state = MISSILE_INACTIVE;

           if (missiles[index].owner==PLAYER_OWNER)
              active_player_missiles--;

           // decrement total always

           total_active_missiles--;

           } // end if

       } // end if missile alive

    } // end for index

// only rotate if there are active missiles

if (total_active_missiles)
   Rotate_Object(&dynamic_obj[MISSILES_TEMPLATE],0,0,30);

} // end Move_Missiles

/////////////////////////////////////////////////////////////////////////////

void Draw_Missiles(void)
{
// this function draws all the missiles (in 3d)

int index; // looping variable

for (index=0; index<NUM_MISSILES; index++)
    {
    // test if missile is active before starting 3-D processing

    if (missiles[index].state==MISSILE_ACTIVE)
       {

       // test if object is visible

       // now before we continue to process object, we must
       // move it to the proper world position

       dynamic_obj[MISSILES_TEMPLATE].world_pos.x = missiles[index].x;
       dynamic_obj[MISSILES_TEMPLATE].world_pos.y = missiles[index].y;
       dynamic_obj[MISSILES_TEMPLATE].world_pos.z = missiles[index].z;

       if (!Remove_Object(&dynamic_obj[MISSILES_TEMPLATE],OBJECT_CULL_XYZ_MODE))
          {
          // convert object local coordinates to world coordinate

          Local_To_World_Object(&dynamic_obj[MISSILES_TEMPLATE]);

          // remove the backfaces and shade object

          Remove_Backfaces_And_Shade(&dynamic_obj[MISSILES_TEMPLATE],-1);

          // convert world coordinates to camera coordinate

          World_To_Camera_Object(&dynamic_obj[MISSILES_TEMPLATE]);

          // clip the objects polygons against viewing volume

          Clip_Object_3D(&dynamic_obj[MISSILES_TEMPLATE],CLIP_Z_MODE);

          // generate the final polygon list

          Generate_Poly_List(&dynamic_obj[MISSILES_TEMPLATE],ADD_TO_POLY_LIST);

          } // end if object is outside viewing volume

       } // end if missile active

    } // end for index

} // end Draw_Missiles

//////////////////////////////////////////////////////////////////////////////

int Start_Missile(int owner,
                  vector_3d_ptr pos,
                  vector_3d_ptr dir,
                  int speed,
                  int lifetime)
{
// this function starts a missile up by hunting for a unused missile, initializing
// it and then starting it with the proper parameters

int index; // looping variable

// first test if this is a missile fired by player, if so, test if player
// is getting greedy

if (owner==PLAYER_OWNER && active_player_missiles>=MAX_PLAYER_MISSILES)
   return(0);

// hunt for an inactive missile

for (index=0; index<NUM_MISSILES; index++)
    {
    // is this missile free?

    if (missiles[index].state==MISSILE_INACTIVE)
       {

       // set this missile up

       missiles[index].state       = MISSILE_ACTIVE;
       missiles[index].owner       = owner;
       missiles[index].lifetime    = lifetime;

       missiles[index].direction.x = speed*dir->x;
       missiles[index].direction.y = speed*dir->y;
       missiles[index].direction.z = speed*dir->z;

       // start missile at center of viewport plus one step out

       missiles[index].x           =            pos->x+dir->x;
       missiles[index].y           = GUN_HEIGHT+pos->y+dir->y;
       missiles[index].z           =            pos->z+dir->z;

       // test if player fired the missile and update active missiles

       if (owner==PLAYER_OWNER)
          {
          // there's now one more missile
          active_player_missiles++;

          // play sound fx

          Digital_FX_Play(KRKMIS_VOC,2);

          }
       else
          {
          // must be alien

          // play sound fx

          Digital_FX_Play(KRKEMIS_VOC,3);

          } // end else

       // increment total active missiles

       total_active_missiles++;

       // exit loop baby!

       return(1);

       } // end if

    } // end for index

// couldn't satisfy request, let caller know

return(0);

} // end Start_Missile

//////////////////////////////////////////////////////////////////////////////

void Init_Aliens(void)
{
// this function initializes the alien structures

int index; // looping variable

for (index=0; index<NUM_ALIENS; index++)
    {

    aliens[index].state           = ALIEN_NEW_STATE;
    aliens[index].counter_1       = 0;
    aliens[index].counter_2       = 0;
    aliens[index].threshold_1     = 0;
    aliens[index].threshold_2     = 0;
    aliens[index].aux_1           = 0;
    aliens[index].aux_2           = 0;
    aliens[index].color_reg       = ALIEN_EXPL_BASE_REG+index;


    aliens[index].color.red       = 0;
    aliens[index].color.green     = 0;
    aliens[index].color.blue      = 0;

    aliens[index].type            = index%2;
    aliens[index].speed           = 0;

    aliens[index].angular_heading = 0;

    aliens[index].direction.x     = 0;
    aliens[index].direction.y     = 0;
    aliens[index].direction.z     = 0;

    aliens[index].x               = -2000 + rand()%4000;
    aliens[index].y               = 0;
    aliens[index].z               = -2000 + rand()%4000;

    } // end for index

} // end Init_Aliens

//////////////////////////////////////////////////////////////////////////////

void Process_Aliens(void)
{
// this function performs AI for the aliens and transforms them

int index,      // looping variable
    which_pod;  // used to select which telepod to start a dead alien at

float head_x,   // used to compute final heading of alien
      head_z,
      target_x, // used to compute desired target heading for alien
      target_z,
      normal_y, // the y component of a normal vector
      distance; // distance between alien and player


vector_3d alien_pos,    // temp to hold alien position
          alien_dir;    // temp to hold alien direction

// this holds the aliens personality probability table.
// here are the meanings of the values and the current distribution

// 0 attack, 30%
// 1 random, 20%
// 2 evade,  10%
// 3 stop,   10%
// 4 turn,   20%

static int alien_personality[10]={0,1,4,0,1,4,3,0,2,3};

// first process live aliens

for (index=0; index<NUM_ALIENS; index++)
    {
    // is this alien alive?

    if (aliens[index].state!=ALIEN_DEAD)
       {

       // based on state of alien, do the right thing

       switch(aliens[index].state)
             {
             case ALIEN_DEAD:
                  {
                  // process dead state

                  aliens[index].state = ALIEN_NEW_STATE;

                  } break;

             case ALIEN_DYING:
                  {
                  // continue dying

                  // increase intensity of color

                  if ((aliens[index].color.green+=4)>63)
                      aliens[index].color.green = 63;

                  // set color register

                  Write_Color_Reg(aliens[index].color_reg,(RGB_color_ptr)&aliens[index].color);

                  // is death sequence complete?

                  if (++aliens[index].counter_1 > aliens[index].threshold_1)
                     {
                     // tell state machine to select a new state

                     aliens[index].state = ALIEN_DEAD;

                     Write_Color_Reg(aliens[index].color_reg,(RGB_color_ptr)&black);

                     } // end if

                  } break;

             case ALIEN_NEW_STATE:
                  {

                  // a new state has been requested, select a new state based
                  // on probability and personality

                  switch(alien_personality[rand()%10])
                        {
                        case 0: // attack

                             {
                             // the alien is going to attack the player

                             // how long for total state

                             aliens[index].counter_1   = 0;
                             aliens[index].threshold_1 = 50 + rand()%150;

                             // set time between heading adjustements

                             aliens[index].counter_2   = 0;
                             aliens[index].threshold_2 = 3 + rand()%5;

                             // set speed

                             aliens[index].speed = 45 + rand()%5;

                             // set new state

                             aliens[index].state = ALIEN_ATTACK;

                             } break;

                        case 1:  // random
                             {

                             // how long for total state

                             aliens[index].counter_1   = 0;
                             aliens[index].threshold_1 = 50 + rand()%150;

                             // how often for random direction changes

                             aliens[index].counter_2   = 0;
                             aliens[index].threshold_2 = 10 + rand()%50;

                             // set speed

                             aliens[index].speed = 40 + rand()%25;

                             // and set new state

                             aliens[index].state = ALIEN_RANDOM;

                             } break;

                        case 2:  // evade
                             {

                             // how long for total state

                             aliens[index].counter_1   = 0;
                             aliens[index].threshold_1 = 50 + rand()%150;

                             // set time between heading adjustements

                             aliens[index].counter_2   = 0;
                             aliens[index].threshold_2 = 3 + rand()%5;

                             // set speed

                             aliens[index].speed = 50 + rand()%5;

                             // set new state

                             aliens[index].state = ALIEN_EVADE;

                             } break;

                        case 3: // stop
                             {
                             // set number of frames for alien to stop

                             aliens[index].counter_1   = 0;
                             aliens[index].threshold_1 = 5 + rand()%10;

                             // set speed to 0

                             aliens[index].speed = 0;

                             // and set new state

                             aliens[index].state = ALIEN_STOP;

                             } break;


                        case 4:  // turn
                             {

                             // set amount of time for turn

                             aliens[index].counter_1   = 0;
                             aliens[index].threshold_1 = 20 + rand()%20;

                             // set angular turning rate

                             aliens[index].aux_1 = -5+rand()%11;

                             // set speed for turn

                             aliens[index].speed = 35+rand()%30;

                             // and set new state

                             aliens[index].state = ALIEN_TURN;

                             } break;

                        default: break;

                        } // end switch

                  } break;

             case ALIEN_ATTACK:
                  {
                  // continue tracking player

                  // test if it's time to adjust heading to track player

                  if (++aliens[index].counter_2 > aliens[index].threshold_2)
                     {

                     // adjust heading toward player, use a heuristic approach
                     // that simply tries to keep turing the alien toward
                     // the player, later maybe we could add a bit of
                     // trajectory lookahead, so the alien could intercept
                     // the player???


                     // to determine which way the alien needs to turn we
                     // can use the following trick: based on the current
                     // trajectory of the alien and the vector from the
                     // alien to the player, we can compute a normal vector

                     // compute heading vector (happens to be a unit vector)

                     head_x = sin_look[aliens[index].angular_heading];
                     head_z = cos_look[aliens[index].angular_heading];

                     // compute target trajectory vector, players position
                     // minus aliens position

                     target_x = view_point.x - aliens[index].x;
                     target_z = view_point.z - aliens[index].z;

                     // now compute y component of normal

                     normal_y = (head_z*target_x - head_x*target_z);

                     // based on the sign of the result we can determine if
                     // we should turn the alien right or left, but be careful
                     // we are in a LEFT HANDED system!

                     if (normal_y>=0)
                        aliens[index].angular_heading+=(10+rand()%10);
                     else
                        aliens[index].angular_heading-=(10+rand()%10);

                     // check angle for overflow/underflow

                     if (aliens[index].angular_heading >=360)
                        aliens[index].angular_heading-=360;
                     else
                     if (aliens[index].angular_heading < 0)
                        aliens[index].angular_heading+=360;

                     // reset counter

                     aliens[index].counter_2 = 0;

                     } // end if

                  // test if attacking sequence is complete

                  if (++aliens[index].counter_1 > aliens[index].threshold_1)
                     {
                     // tell state machine to select a new state

                     aliens[index].state = ALIEN_NEW_STATE;

                     } // end if

                  // try and fire a missile

                  distance = fabs(view_point.x - aliens[index].x) +
                             fabs(view_point.z - aliens[index].z);

                  if ((rand()%15)==1 && distance<3500)
                     {
                     // create local vectors

                     // first position

                     alien_pos.x = aliens[index].x;
                     alien_pos.y = 45; // alien y centerline
                     alien_pos.z = aliens[index].z;

                     // now direction

                     alien_dir.x = sin_look[aliens[index].angular_heading];
                     alien_dir.y = 0;
                     alien_dir.z = cos_look[aliens[index].angular_heading];

                     // start the missle

                     Start_Missile(ALIEN_OWNER,
                                   &alien_pos,
                                   &alien_dir,
                                   aliens[index].speed+25,
                                   75);

                     } // end if fire a missile

                  } break;

             case ALIEN_RANDOM:
                  {
                  // continue moving in randomly selected direction

                  // test if it's time to select a new direction

                  if (++aliens[index].counter_2 > aliens[index].threshold_2)
                     {

                     // select a new direction  +- 30 degrees

                     aliens[index].angular_heading+=(-30 + 10*rand()%7);

                     // check angle for overflow/underflow

                     if (aliens[index].angular_heading >=360)
                        aliens[index].angular_heading-=360;
                     else
                     if (aliens[index].angular_heading < 0)
                        aliens[index].angular_heading+=360;

                     // reset counter

                     aliens[index].counter_2 = 0;

                     } // end if

                  // test if entire random sequence is complete

                  if (++aliens[index].counter_1 > aliens[index].threshold_1)
                     {
                     // tell state machine to select a new state

                     aliens[index].state = ALIEN_NEW_STATE;

                     } // end if

                  } break;

             case ALIEN_EVADE:
                  {
                  // continue evading player

                  // test if it's time to adjust heading to evade player

                  if (++aliens[index].counter_2 > aliens[index].threshold_2)
                     {

                     // adjust heading away from player, use a heuristic approach
                     // that simply tries to keep turing the alien away from
                     // the player

                     // to determine which way the alien needs to turn we
                     // can use the following trick: based on the current
                     // trajectory of the alien and the vector from the
                     // alien to the player, we can compute a normal vector

                     // compute heading vector (happens to be a unit vector)

                     head_x = sin_look[aliens[index].angular_heading];
                     head_z = cos_look[aliens[index].angular_heading];

                     // compute target trajectory vector, players position
                     // minus aliens position

                     target_x = view_point.x - aliens[index].x;
                     target_z = view_point.z - aliens[index].z;

                     // now compute y component of normal

                     normal_y = (head_z*target_x - head_x*target_z);

                     // based on the sign of the result we can determine if
                     // we should turn the alien right or left, but be careful
                     // we are in a LEFT HANDED system!

                     if (normal_y>=0)
                        aliens[index].angular_heading-=(10+rand()%10);
                     else
                        aliens[index].angular_heading+=(10+rand()%10);

                     // check angle for overflow/underflow

                     if (aliens[index].angular_heading >=360)
                        aliens[index].angular_heading-=360;
                     else
                     if (aliens[index].angular_heading < 0)
                        aliens[index].angular_heading+=360;

                     // reset counter

                     aliens[index].counter_2 = 0;

                     } // end if

                  // test if attacking sequence is complete

                  if (++aliens[index].counter_1 > aliens[index].threshold_1)
                     {
                     // tell state machine to select a new state

                     aliens[index].state = ALIEN_NEW_STATE;

                     } // end if

                  } break;

             case ALIEN_STOP:
                  {
                  // sit still and rotate

                  // test if stopping sequence is complete

                  if (++aliens[index].counter_1 > aliens[index].threshold_1)
                     {
                     // tell state machine to select a new state

                     aliens[index].state = ALIEN_NEW_STATE;

                     } // end if

                  } break;

             case ALIEN_TURN:
                  {
                  // continue turn

                  aliens[index].angular_heading+=aliens[index].aux_1;

                  // check angle for overflow/underflow

                  if (aliens[index].angular_heading >=360)
                     aliens[index].angular_heading-=360;
                  else
                  if (aliens[index].angular_heading < 0)
                     aliens[index].angular_heading+=360;

                  // test if turning sequence is complete

                  if (++aliens[index].counter_1 > aliens[index].threshold_1)
                     {
                     // tell state machine to select a new state

                     aliens[index].state = ALIEN_NEW_STATE;

                     } // end if

                  } break;

             default: break;

             } // end switch

       // now move the alien based on direction

       aliens[index].x +=  (aliens[index].speed*sin_look[aliens[index].angular_heading]);
       aliens[index].z +=  (aliens[index].speed*cos_look[aliens[index].angular_heading]);

       // perform bounds checking, if an alien hits an edge warp to other side

       if (aliens[index].x>(GAME_MAX_WORLD_X+750))
           aliens[index].x=(GAME_MIN_WORLD_X-500);
       else
       if (aliens[index].x<(GAME_MIN_WORLD_X-750))
           aliens[index].x=(GAME_MAX_WORLD_X+500);

       if (aliens[index].z>(GAME_MAX_WORLD_Z+750))
           aliens[index].z=(GAME_MIN_WORLD_Z-500);
       else
       if (aliens[index].z<(GAME_MIN_WORLD_Z-750))
           aliens[index].z=(GAME_MAX_WORLD_Z+500);

       } // end if alive

    } // end for index

// try and turn on dead guys at teleporters 10% chance

if ((rand()%10)==1)
   for (index=0; index<NUM_ALIENS; index++)
       {
       // test if this one is dead

       if (aliens[index].state==ALIEN_DEAD)
          {

          // set state to new state

          aliens[index].state = ALIEN_NEW_STATE;

          // select telepod position to start at

          which_pod = rand()%NUM_TELEPODS;

          aliens[index].x = telepods[which_pod].x;
          aliens[index].z = telepods[which_pod].z;

          // that's enough for now!

          break;

          } // end if

       } // end for index

} // end Process_Aliens

//////////////////////////////////////////////////////////////////////////////

void Draw_Aliens(void)
{
// this function simply draws the aliens

int index,       // looping variable
    diff_angle;  // used to track anglular difference between virtual object
                 // and real object

// draw all the aliens (ya all four of them!)

for (index=0; index<NUM_ALIENS; index++)
    {
    // test if missile is alive before starting 3-D processing

    if (aliens[index].state!=ALIEN_DEAD)
       {

       // which kind of alien are we dealing with?

       if (aliens[index].type == ALIEN_TALLON)
          {

          dynamic_obj[TALLONS_TEMPLATE].world_pos.x = aliens[index].x;
          dynamic_obj[TALLONS_TEMPLATE].world_pos.y = aliens[index].y;
          dynamic_obj[TALLONS_TEMPLATE].world_pos.z = aliens[index].z;

          if (!Remove_Object(&dynamic_obj[TALLONS_TEMPLATE],OBJECT_CULL_XYZ_MODE))
             {

             // rotate tallon model to proper direction for this copy of it

             // look in state field of model to determine current angle and then
             // compare it to the desired angle, compute the difference and
             // use the result as the rotation angle to rotate the model
             // into the proper orientation for this copy of it

             diff_angle = aliens[index].angular_heading -
                          dynamic_obj[TALLONS_TEMPLATE].state;

             // fix the sign of the angle

             if (diff_angle<0)
                 diff_angle+=360;

             // perform the rotation

             Rotate_Object(&dynamic_obj[TALLONS_TEMPLATE],0,diff_angle,0);

             // update object template with new heading

             dynamic_obj[TALLONS_TEMPLATE].state = aliens[index].angular_heading;

             // convert object local coordinates to world coordinate

             Local_To_World_Object(&dynamic_obj[TALLONS_TEMPLATE]);

             // remove the backfaces and shade object

             if (aliens[index].state==ALIEN_DYING)
                 Remove_Backfaces_And_Shade(&dynamic_obj[TALLONS_TEMPLATE],aliens[index].color_reg);
             else
                 Remove_Backfaces_And_Shade(&dynamic_obj[TALLONS_TEMPLATE],-1);

             // convert world coordinates to camera coordinate

             World_To_Camera_Object(&dynamic_obj[TALLONS_TEMPLATE]);

             // clip the objects polygons against viewing volume

             Clip_Object_3D(&dynamic_obj[TALLONS_TEMPLATE],CLIP_Z_MODE);

             // generate the final polygon list

             Generate_Poly_List(&dynamic_obj[TALLONS_TEMPLATE],ADD_TO_POLY_LIST);

             } // end if object is outside viewing volume

          } // end if tallon
       else
          {

          dynamic_obj[SLIDERS_TEMPLATE].world_pos.x = aliens[index].x;
          dynamic_obj[SLIDERS_TEMPLATE].world_pos.y = aliens[index].y;
          dynamic_obj[SLIDERS_TEMPLATE].world_pos.z = aliens[index].z;

          if (!Remove_Object(&dynamic_obj[SLIDERS_TEMPLATE],OBJECT_CULL_XYZ_MODE))
             {

             // rotate slider model to proper direction for this copy of it

             // look in state field of model to determine current angle and then
             // compare it to the desired angle, compute the difference and
             // use the result as the rotation angle to rotate the model
             // into the proper orientation for this copy of it

             diff_angle = aliens[index].angular_heading -
                          dynamic_obj[SLIDERS_TEMPLATE].state;

             // fix the sign of the angle

             if (diff_angle<0)
                 diff_angle+=360;

             // perform the rotation

             Rotate_Object(&dynamic_obj[SLIDERS_TEMPLATE],0,diff_angle,0);

             // update object template with new heading

             dynamic_obj[SLIDERS_TEMPLATE].state = aliens[index].angular_heading;

             // convert object local coordinates to world coordinate

             Local_To_World_Object(&dynamic_obj[SLIDERS_TEMPLATE]);

             // remove the backfaces and shade object

             if (aliens[index].state==ALIEN_DYING)
                 Remove_Backfaces_And_Shade(&dynamic_obj[SLIDERS_TEMPLATE],aliens[index].color_reg);
             else
                 Remove_Backfaces_And_Shade(&dynamic_obj[SLIDERS_TEMPLATE],-1);

             // convert world coordinates to camera coordinate

             World_To_Camera_Object(&dynamic_obj[SLIDERS_TEMPLATE]);

             // clip the objects polygons against viewing volume

             Clip_Object_3D(&dynamic_obj[SLIDERS_TEMPLATE],CLIP_Z_MODE);

             // generate the final polygon list

             Generate_Poly_List(&dynamic_obj[SLIDERS_TEMPLATE],ADD_TO_POLY_LIST);

             } // end if object is outside viewing volume

          } // end else slider

       } // end if alien alive

    } // end for index

} // end Draw_Aliens

/////////////////////////////////////////////////////////////////////////////

void Draw_Background(int mountain_pos)
{
// this function draw the gradient sky and ground for the 3-D view

long color; // used to build up a 4 byte color

// this function draws the background and mountains for the foreground
// 3-D image

// the sky has three layers

// draw layer 1

color = SKY_COLOR_1;

color = color | (color << 8);
color = color | (color << 16);

fquadset(double_buffer, color, 4*320/4);

// draw layer 2

color = SKY_COLOR_2;

color = color | (color << 8);
color = color | (color << 16);

fquadset(double_buffer+320*4, color, 8*320/4);

// draw layer 3

color = SKY_COLOR_3;

color = color | (color << 8);
color = color | (color << 16);

fquadset(double_buffer+320*12, color, 44*320/4);

// now draw the scrolling mountainscape

Layer_Draw((layer_ptr)&mountains,mountain_pos,0,
           double_buffer,MOUNTAIN_Y_POS,MOUNTAIN_HEIGHT,0);

// now draw the ground

// layer 1

color = GND_COLOR_1;

color = color | (color << 8);
color = color | (color << 16);

fquadset(double_buffer+320*99, color, 3*320/4);

// layer 2

color = GND_COLOR_2;

color = color | (color << 8);
color = color | (color << 16);

fquadset(double_buffer+320*102, color, 6*320/4);

color = GND_COLOR_3;

color = color | (color << 8);
color = color | (color << 16);

fquadset(double_buffer+320*108, color, 21*320/4);

} // end Draw_Background

/////////////////////////////////////////////////////////////////////////////

void Draw_Box(int x1,int y1,int x2, int y2,int color)
{
// this function draws a hollow rectangle

Line_H(x1,x2,y1,color);
Line_H(x1,x2,y2,color);
Line_V(y1,y2,x1,color);
Line_V(y1,y2,x2,color);

} // end Draw_Box

///////////////////////////////////////////////////////////////////////////////

void Tech_Print(int x,int y,char *string,unsigned char far *destination)
{
// this function is used to print text out like a teletypwriter,it looks
// cool, trust me!

int length,  // length of input string
    index,   // looping variable
    counter; // used to time process

char buffer[3];  // a little string used to call font engine with

// compute length of input string

length = strlen(string);

// print the string out a character at a time

for (index=0; index<length; index++)
    {
    // the first character is the actual printable character

    buffer[0] = string[index];

    // null terminate

    buffer[1] = 0;

    // print the string

    Font_Engine_1(x,y,0,0,buffer,destination);

    // move to next position

    x+=(TECH_FONT_WIDTH+1);

    // wait a bit  1/70th of a second

    Wait_For_Vertical_Retrace();

    // clear the cursor

    } // end for

// done!

} // end Tech_Print

//////////////////////////////////////////////////////////////////////////////

void Font_Engine_1(int x,int y,
                   int font,int color,
                   char *string,unsigned char far *destination)
{
// this function prints a string out using one of the graphics fonts that
// we have drawn, note this first version doesn't use the font field, but
// we'll throw it in to keep the interface open for a future version

static int font_loaded=0;   // this is used to track the first time the
                            // function is loaded

int index,    // loop index
    c_index,  // character index
    length;   // used to compute lengths of strings

// test if this is the first time this function is called, if so load the
// font

if (!font_loaded)
   {
   // load the 4x7 tech font

   PCX_Init((pcx_picture_ptr)&image_pcx);
   PCX_Load("krkfnt.pcx", (pcx_picture_ptr)&image_pcx,1);

   // allocate memory for each bitmap and load character

   for (index=0; index<NUM_TECH_FONT; index++)
       {
       // allocate memory for charcter

       Bitmap_Allocate((bitmap_ptr)&tech_font[index],
                       TECH_FONT_WIDTH,TECH_FONT_HEIGHT);

       // set size of character

       tech_font[index].width = TECH_FONT_WIDTH;
       tech_font[index].height= TECH_FONT_HEIGHT;

       // extract bitmap from PCX buffer

       tech_font[index].x = 1 + (index % 16) * (TECH_FONT_WIDTH+1);
       tech_font[index].y = 1 + (index / 16) * (TECH_FONT_HEIGHT+1);

       Bitmap_Get((bitmap_ptr)&tech_font[index],
                  (unsigned char far *)image_pcx.buffer);

       } // end for index
   // font is loaded, delete pcx file and set flag

   PCX_Delete((pcx_picture_ptr)&image_pcx);

   font_loaded=1;

   } // end if first time
else
   {
   // print the sent string

   // pre-compute length of string

   length=strlen(string);

   // print the string character by character

   for (index=0; index<length; index++)
       {
       // extract the character index from the space character

       c_index = string[index] - ' ';

       // set bitmap position

       tech_font[c_index].y = y;
       tech_font[c_index].x = x;

       // display bitmap

       Bitmap_Put((bitmap_ptr)&tech_font[c_index],
                  (unsigned char far*)destination,0);

       // move to next character position

       x+=(TECH_FONT_WIDTH+1);

       } // end for index

   } // end else print string

} // end Font_Engine_1

/////////////////////////////////////////////////////////////////////////////

void Panel_FX(int reset)
{
// this function performs all of the special effects for the control panel

int index; // looping variable

static int panel_counter = 0;  // used to time the color rotation of the panel

static int entered=0; // flags if function has been entered yet

// test if this is the first time in function

if (!entered || reset)
   {
   // set entrance flag

   entered = 1;

   // set up color registers

   for (index=START_PANEL_REG; index<=END_PANEL_REG; index++)
       {
       // generate the color

       color_1.red   = 0;
       color_1.green = 0;
       color_1.blue  = 0;

       // write the data

       Write_Color_Reg(index, (RGB_color_ptr)&color_1);

       } // end for

   color_1.red   = 63;

   Write_Color_Reg(START_PANEL_REG,(RGB_color_ptr)&color_1);

   Write_Color_Reg(START_PANEL_REG+3,(RGB_color_ptr)&color_1);

   // set up selection indicator color

   color_1.red   = 30;
   color_1.green = 0;
   color_1.blue  = 0;

   Write_Color_Reg(SELECT_REG, (RGB_color_ptr)&color_1);

   } // end if entered

// is it time to update colors?

if (++panel_counter>2)
   {
   // reset counter

   panel_counter=0;

   // do animation to colors

   Read_Color_Reg(END_PANEL_REG, (RGB_color_ptr)&color_1);

   for (index=END_PANEL_REG; index>START_PANEL_REG; index--)
       {
       // read the (i-1)th register

       Read_Color_Reg(index-1, (RGB_color_ptr)&color_2);

       // assign it to the ith

       Write_Color_Reg(index, (RGB_color_ptr)&color_2);

       } // end rotate loop

    // place the value of the first color register into the last to
    // complete the rotation

    Write_Color_Reg(START_PANEL_REG, (RGB_color_ptr)&color_1);

   } // end if time

// update selection color

Read_Color_Reg(SELECT_REG, (RGB_color_ptr)&color_1);

if ((color_1.red+=5) > 63)
   color_1.red = 25;

Write_Color_Reg(SELECT_REG, (RGB_color_ptr)&color_1);

} // end Panel_FX

/////////////////////////////////////////////////////////////////////////////

void Intro_Planet(void)
{
// this function does the introdcution to centari alpha 3

int index; // looping variable

// data output fields

static char *template[]={"PLANET:   ",
                         "TYPE:     ",
                         "MASS:     ",
                         "TEMP:     ",
                         "PERIOD:   ",
                         "LIFEFORMS:",
                         "STATUS:   ",};

// the data for each field

static char *data[] = {" CENTARI ALPHA 3",
                       " F-CLASS, AMMONIUM ATMOSPHERE",
                       " 20.6 KELA",
                       " 150.2 DEG",
                       " 3.2 TELGANS",
                       " SILICA BASED, PHOSPOROUS METABOLIZERS",
                       " BATTLE AREA - RESTRICTED" };

// load in the KRK title screen

PCX_Init((pcx_picture_ptr)&image_pcx);
PCX_Load("krkredp.pcx",(pcx_picture_ptr)&image_pcx,1);

// show the PCX buffer

PCX_Show_Buffer((pcx_picture_ptr)&image_pcx);

// done with data so delete it

PCX_Delete((pcx_picture_ptr)&image_pcx);

// do special effects

Time_Delay(50);

// draw out statistics

for (index=0; index<7; index++)
    {

    // draw header field

    Font_Engine_1(START_MESS_X,START_MESS_Y+index*10,0,0,
                  template[index],
                  video_buffer);


    // draw information for field

    Tech_Print(START_MESS_X+80,START_MESS_Y+index*10,data[index],video_buffer);

    Time_Delay(20);

    } // end for index

// wait for a sec

Time_Delay(50);

} // end Intro_Planet

//////////////////////////////////////////////////////////////////////////////

void Closing_Screen(void)
{
// this function prints the credits

int index;  // looping variable

static char *extra_credits[] = {"EXTRA CREDITS",
                                "             ",
                                "MUSICAL MASTERY BY",
                                "DEAN HUDSON OF",
                                "ECLIPSE PRODUCTIONS",
                                "                   ",
                                "MIDPAK INSTRUMENTATION CONSULTING BY",
                                "ROB WALLACE OF",
                                "WALLACE MUSIC & SOUND",
                                "                     ",
                                "TITLE SCREEN BY",
                                "RICHARD BENSON"};

// blank the screen

Fill_Screen(0);

// restore pallete

Write_Palette(0,255,(RGB_palette_ptr)&game_palette);

if (music_enabled)
   {
   Music_Stop();
   Music_Play((music_ptr)&song,11);
   } // end if

// print out the credits

for (index=0; index<=11; index++)
    {

    Tech_Print(160-(TECH_FONT_WIDTH+1)*(strlen(extra_credits[index])/2),
               8+index*(TECH_FONT_HEIGHT+4),
               extra_credits[index],video_buffer);

    if (keys_active)
       return;

    Time_Delay(10);

    } // end for index

Time_Delay(50);

// scroll them away

for (index=0; index<135; index++)
    {
    fquadcpy(video_buffer,video_buffer+320,16000-80);

    // test for exit

    if (keys_active)
       return;

    } // end for index

} // end Closing_Screen

//////////////////////////////////////////////////////////////////////////////

void Intro_Waite(void)
{
// load in the waite group title screen

PCX_Init((pcx_picture_ptr)&image_pcx);
PCX_Load("waite.pcx",(pcx_picture_ptr)&image_pcx,1);

// done with data so delete it

PCX_Delete((pcx_picture_ptr)&image_pcx);

// show the PCX buffer

PCX_Show_Buffer((pcx_picture_ptr)&image_pcx);

// do special effects

// wait for a sec

Time_Delay(40);

Screen_Transition(SCREEN_WHITENESS);

// blank the screen

Fill_Screen(0);

} // end Intro_Waite

//////////////////////////////////////////////////////////////////////////////

void Intro_KRK(void)
{
// load in the main title screen group title screen

PCX_Init((pcx_picture_ptr)&image_pcx);
PCX_Load("krkfirst.pcx",(pcx_picture_ptr)&image_pcx,1);

// done with data so delete it

PCX_Delete((pcx_picture_ptr)&image_pcx);

// show the PCX buffer

PCX_Show_Buffer((pcx_picture_ptr)&image_pcx);

// do special effects

// wait for a sec

Time_Delay(50);

Screen_Transition(SCREEN_DARKNESS);

// blank the screen

Fill_Screen(0);

} // end Intro_KRK

//////////////////////////////////////////////////////////////////////////////

void Intro_Controls(void)
{
// this function displays the controls screen

// load in the starblazer controls screen

PCX_Init((pcx_picture_ptr)&image_controls);
PCX_Load("krkredpc.pcx",(pcx_picture_ptr)&image_controls,1);

// copy controls data to video buffer

PCX_Show_Buffer((pcx_picture_ptr)&image_controls);

// delete pcx file

PCX_Delete((pcx_picture_ptr)&image_controls);

} // end Intro_Controls

////////////////////////////////////////////////////////////////////////////

void Intro_Briefing(void)
{
// this function displays the briefing screen

int done=0, // exit flag
    page=0, // current page user is reading
    index;  // looping variable

// load in the starblazer controls screen

PCX_Init((pcx_picture_ptr)&image_controls);
PCX_Load("krkins.pcx",(pcx_picture_ptr)&image_controls,0);

// copy controls data to video buffer

PCX_Show_Buffer((pcx_picture_ptr)&image_controls);

// delete pcx file

PCX_Delete((pcx_picture_ptr)&image_controls);

// display the first page

for (index=0; index<NUM_LINES_PAGE; index++)
    Font_Engine_1(78,24+index*8,0,0,instructions[index+page*17],video_buffer);

// enter main event loop

while(!done)
     {
     // has the user pressed a key

     if (keys_active>0)
        {

        if (keyboard_state[MAKE_UP])
           {
           // page up

           if (--page<0)
              page = 0;

           // press button

           Digital_FX_Play(KRKKEY_VOC,3);

           Time_Delay(2);

           } // end if up

        if (keyboard_state[MAKE_DOWN])
           {
           // page down

           if (++page>=NUM_PAGES)
              page = NUM_PAGES-1;

           // press button

           Digital_FX_Play(KRKKEY_VOC,3);

           Time_Delay(2);

           } // end if down

        if (keyboard_state[MAKE_ESC])
           {

           Digital_FX_Play(KRKKEY_VOC,3);

           done=1;

           } // end if esc

        // refresh display

        for (index=0; index<NUM_LINES_PAGE; index++)
            Font_Engine_1(78,24+index*8,0,0,instructions[index+page*17],video_buffer);

        } // end if a key has been pressed

     // wait a sec

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

     } // end main while

} // end Intro_Briefing

//////////////////////////////////////////////////////////////////////////////

void Reset_System(void)
{
// this function resets everything so the game can be ran again
// I hope I didn't leave anything out?

scanner_state  = 0;
hud_state      = 0;
tactical_state = TACTICAL_MODE_OFF;

ship_pitch     = 0;
ship_yaw       = 0;
ship_roll      = 0;
ship_speed     = 0;
ship_energy    = 50;
ship_damage    = 0;
ship_message   = SHIP_STABLE;
ship_timer     = 0;
ship_kills     = 0;
ship_deaths    = 0;

// reset back to power station

view_point.x = 0;
view_point.z = 0;

} // end Reset_System

/////////////////////////////////////////////////////////////////////////////

void Music_Init(void)
{
// this function loads the music and resets all the indexes

static int loaded=0;

// has the music already been loaded

if (!music_enabled)
   return;

if (!loaded)
   {
   Music_Load("krkmus.xmi",(music_ptr)&song);
   loaded=1;
   } // end if not loaded

// reset sequence counters

game_seq_index=0;
intro_seq_index=0;

} // end Music_Init

/////////////////////////////////////////////////////////////////////////////

void Music_Close(void)
{
// this function unloads the music files

if (!music_enabled)
   return;

// turn off music and unload song

Music_Stop();
Music_Unload((music_ptr)&song);

} // end Music_Close

//////////////////////////////////////////////////////////////////////////////

void Digital_FX_Init(void)
{
// this function initializes the digital sound fx system

static int loaded=0;

if (!digital_enabled)
   return;

// have the sound fx been loaded?

if (!loaded)
   {
   // load int sounds

   Sound_Load("KRKMIS.VOC" ,  (sound_ptr)&digital_FX[KRKMIS_VOC ] ,1);
   Sound_Load("KRKEMIS2.VOC",  (sound_ptr)&digital_FX[KRKEMIS_VOC],1);
   Sound_Load("KRKTAC.VOC" ,  (sound_ptr)&digital_FX[KRKTAC_VOC ],1);
   Sound_Load("KRKSCN.VOC" ,  (sound_ptr)&digital_FX[KRKSCN_VOC ],1);
   Sound_Load("KRKHUD.VOC" ,  (sound_ptr)&digital_FX[KRKHUD_VOC ],1);

   Sound_Load("KRKKEY.VOC" ,  (sound_ptr)&digital_FX[KRKKEY_VOC ],1);
   Sound_Load("KRKEX1.VOC" ,  (sound_ptr)&digital_FX[KRKEX1_VOC ],1);
   Sound_Load("KRKEX2.VOC" ,  (sound_ptr)&digital_FX[KRKEX2_VOC ],1);

   // set loaded flag

   loaded=1;

   } // end if sound effects aren't loaded

} // end Digital_FX_Init

///////////////////////////////////////////////////////////////////////////////

void Digital_FX_Close(void)
{
// this function unloads all the digital FX

if (!digital_enabled)
   return;

// unload all the sound fx from memory

Sound_Unload((sound_ptr)&digital_FX[KRKMIS_VOC]);
Sound_Unload((sound_ptr)&digital_FX[KRKEMIS_VOC]);
Sound_Unload((sound_ptr)&digital_FX[KRKTAC_VOC]);
Sound_Unload((sound_ptr)&digital_FX[KRKSCN_VOC]);
Sound_Unload((sound_ptr)&digital_FX[KRKHUD_VOC]);

Sound_Unload((sound_ptr)&digital_FX[KRKPOW_VOC]);
Sound_Unload((sound_ptr)&digital_FX[KRKKEY_VOC]);
Sound_Unload((sound_ptr)&digital_FX[KRKEX1_VOC]);
Sound_Unload((sound_ptr)&digital_FX[KRKEX2_VOC]);

} // end Digital_FX_Close

/////////////////////////////////////////////////////////////////////////////

int Digital_FX_Play(int the_effect, int priority)
{
// this function is used to play a digital effect using a pre-emptive priority
// scheme. The algorithm works like this: if a sound is playing then its
// priority is compared to the sound that is being requested to be played
// if the new sound has higher priority (a smaller number) then the currenlty
// playing sound is pre-empted for the new sound and the global FX priority
// is set to the new sound. If there is no sound playing then the new sound
// is simple played and the global priority is set

// is the digital fx system on-line?

if (!digital_enabled)
   return(0);

// is there a sound playing?

if (!Sound_Status() || (priority <= digital_FX_priority))
   {
   // start new sound

   Sound_Stop();

   Sound_Play((sound_ptr)&digital_FX[the_effect]);

   // set the priority

   digital_FX_priority = priority;

   return(1);

   } // end if
else // the current sound is of higher priority
   return(0);

} // end Digital_FX_Play

/////////////////////////////////////////////////////////////////////////////

int Parse_Commands(int argc, char **argv)
{
// this function is used to parse the commands line parameters that are to be
// used as switched to enable different modes of operation

int index;  // looping variable

for (index=1; index<argc; index++)
    {
    // get the first character from the string

    switch(argv[index][0])
          {

          case 's': // enable sound effects
          case 'S':
               {
               digital_enabled=1;
               } break;

          case 'm': // enable nusic
          case 'M':
               {
               music_enabled=1;
               } break;

          // more commands would go here...

          default:break;

          } // end switch

    } // end for index

} // end Parse_Commands

////////////////////////////////////////////////////////////////////////////

void Draw_Stationary_Objects(void)
{
// this function draws all the stationary non active objects

int index; // looping index

// phase 0: obstacle type one

for (index=0; index<NUM_OBSTACLES_1; index++)
    {
    // test if object is visible

    // now before we continue to process object, we must
    // move it to the proper world position

    static_obj[OBSTACLES_1_TEMPLATE].world_pos.x = obstacles_1[index].x;
    static_obj[OBSTACLES_1_TEMPLATE].world_pos.y = obstacles_1[index].y;
    static_obj[OBSTACLES_1_TEMPLATE].world_pos.z = obstacles_1[index].z;

    if (!Remove_Object(&static_obj[OBSTACLES_1_TEMPLATE],OBJECT_CULL_XYZ_MODE))
       {

       // convert object local coordinates to world coordinate

       Local_To_World_Object(&static_obj[OBSTACLES_1_TEMPLATE]);

       // remove the backfaces and shade object

       Remove_Backfaces_And_Shade(&static_obj[OBSTACLES_1_TEMPLATE],-1);

       // convert world coordinates to camera coordinate

       World_To_Camera_Object(&static_obj[OBSTACLES_1_TEMPLATE]);

       // clip the objects polygons against viewing volume

       Clip_Object_3D(&static_obj[OBSTACLES_1_TEMPLATE],CLIP_Z_MODE);

       // generate the final polygon list

       Generate_Poly_List(&static_obj[OBSTACLES_1_TEMPLATE],ADD_TO_POLY_LIST);
       }

    }  // end for index

// phase 1: obstacle type two

for (index=0; index<NUM_OBSTACLES_2; index++)
    {
    // test if object is visible

    // now before we continue to process object, we must
    // move it to the proper world position

    static_obj[OBSTACLES_2_TEMPLATE].world_pos.x = obstacles_2[index].x;
    static_obj[OBSTACLES_2_TEMPLATE].world_pos.y = obstacles_2[index].y;
    static_obj[OBSTACLES_2_TEMPLATE].world_pos.z = obstacles_2[index].z;

    if (!Remove_Object(&static_obj[OBSTACLES_2_TEMPLATE],OBJECT_CULL_XYZ_MODE))
       {

       // convert object local coordinates to world coordinate

       Local_To_World_Object(&static_obj[OBSTACLES_2_TEMPLATE]);

       // remove the backfaces and shade object

       Remove_Backfaces_And_Shade(&static_obj[OBSTACLES_2_TEMPLATE],-1);

       // convert world coordinates to camera coordinate

       World_To_Camera_Object(&static_obj[OBSTACLES_2_TEMPLATE]);

       // clip the objects polygons against viewing volume

       Clip_Object_3D(&static_obj[OBSTACLES_2_TEMPLATE],CLIP_Z_MODE);

       // generate the final polygon list

       Generate_Poly_List(&static_obj[OBSTACLES_2_TEMPLATE],ADD_TO_POLY_LIST);
       }

    }  // end for index

// phase 2: the towers

for (index=0; index<NUM_TOWERS; index++)
    {
    // test if object is visible

    // now before we continue to process object, we must
    // move it to the proper world position

    static_obj[TOWERS_TEMPLATE].world_pos.x = towers[index].x;
    static_obj[TOWERS_TEMPLATE].world_pos.y = towers[index].y;
    static_obj[TOWERS_TEMPLATE].world_pos.z = towers[index].z;

    if (!Remove_Object(&static_obj[TOWERS_TEMPLATE],OBJECT_CULL_XYZ_MODE))
       {

       // convert object local coordinates to world coordinate

       Local_To_World_Object(&static_obj[TOWERS_TEMPLATE]);

       // remove the backfaces and shade object

       Remove_Backfaces_And_Shade(&static_obj[TOWERS_TEMPLATE],-1);

       // convert world coordinates to camera coordinate

       World_To_Camera_Object(&static_obj[TOWERS_TEMPLATE]);

       // clip the objects polygons against viewing volume

       Clip_Object_3D(&static_obj[TOWERS_TEMPLATE],CLIP_Z_MODE);

       // generate the final polygon list

       Generate_Poly_List(&static_obj[TOWERS_TEMPLATE],ADD_TO_POLY_LIST);
       }

    }  // end for index


// phase 3: the barriers

for (index=0; index<NUM_BARRIERS; index++)
    {
    // test if object is visible

    // now before we continue to process object, we must
    // move it to the proper world position

    static_obj[BARRIERS_TEMPLATE].world_pos.x = barriers[index].x;
    static_obj[BARRIERS_TEMPLATE].world_pos.y = barriers[index].y;
    static_obj[BARRIERS_TEMPLATE].world_pos.z = barriers[index].z;

    if (!Remove_Object(&static_obj[BARRIERS_TEMPLATE],OBJECT_CULL_XYZ_MODE))
       {

       // convert object local coordinates to world coordinate

       Local_To_World_Object(&static_obj[BARRIERS_TEMPLATE]);

       // remove the backfaces and shade object

       Remove_Backfaces_And_Shade(&static_obj[BARRIERS_TEMPLATE],-1);

       // convert world coordinates to camera coordinate

       World_To_Camera_Object(&static_obj[BARRIERS_TEMPLATE]);

       // clip the objects polygons against viewing volume

       Clip_Object_3D(&static_obj[BARRIERS_TEMPLATE],CLIP_Z_MODE);

       // generate the final polygon list

       Generate_Poly_List(&static_obj[BARRIERS_TEMPLATE],ADD_TO_POLY_LIST);
       }

    }  // end for index



// phase 4: the main power station

for (index=0; index<NUM_STATIONS; index++)
    {
    // test if object is visible

    // now before we continue to process object, we must
    // move it to the proper world position

    static_obj[STATIONS_TEMPLATE].world_pos.x = stations[index].x;
    static_obj[STATIONS_TEMPLATE].world_pos.y = stations[index].y;
    static_obj[STATIONS_TEMPLATE].world_pos.z = stations[index].z;

    if (!Remove_Object(&static_obj[STATIONS_TEMPLATE],OBJECT_CULL_XYZ_MODE))
       {
       // convert object local coordinates to world coordinate

       Local_To_World_Object(&static_obj[STATIONS_TEMPLATE]);

       // remove the backfaces and shade object

       Remove_Backfaces_And_Shade(&static_obj[STATIONS_TEMPLATE],-1);

       // convert world coordinates to camera coordinate

       World_To_Camera_Object(&static_obj[STATIONS_TEMPLATE]);

       // clip the objects polygons against viewing volume

       Clip_Object_3D(&static_obj[STATIONS_TEMPLATE],CLIP_Z_MODE);

       // generate the final polygon list

       Generate_Poly_List(&static_obj[STATIONS_TEMPLATE],ADD_TO_POLY_LIST);
       }

    }  // end for index

// phase 5: the telepods

for (index=0; index<NUM_TELEPODS; index++)
    {
    // test if object is visible

    // now before we continue to process object, we must
    // move it to the proper world position

    static_obj[TELEPODS_TEMPLATE].world_pos.x = telepods[index].x;
    static_obj[TELEPODS_TEMPLATE].world_pos.y = telepods[index].y;
    static_obj[TELEPODS_TEMPLATE].world_pos.z = telepods[index].z;

    if (!Remove_Object(&static_obj[TELEPODS_TEMPLATE],OBJECT_CULL_XYZ_MODE))
       {
       // convert object local coordinates to world coordinate

       Local_To_World_Object(&static_obj[TELEPODS_TEMPLATE]);

       // remove the backfaces and shade object

       Remove_Backfaces_And_Shade(&static_obj[TELEPODS_TEMPLATE],-1);

       // convert world coordinates to camera coordinate

       World_To_Camera_Object(&static_obj[TELEPODS_TEMPLATE]);

       // clip the objects polygons against viewing volume

       Clip_Object_3D(&static_obj[TELEPODS_TEMPLATE],CLIP_Z_MODE);

       // generate the final polygon list

       Generate_Poly_List(&static_obj[TELEPODS_TEMPLATE],ADD_TO_POLY_LIST);
       }

    }  // end for index

} // end Draw_Stationary_Objects

///////////////////////////////////////////////////////////////////////////////

void Set_3D_View(void)
{
// this function sets up the 3d viewing system for the game

// set 2-D clipping region to take into consideration the instrument panels

poly_clip_min_y  = 0;
poly_clip_max_y  = 128;

// set up viewing and 3D clipping parameters

clip_near_z      = 125,
clip_far_z       = 6000,
viewing_distance = 250;

// turn the damn light up a bit!

ambient_light    = 8;

light_source.x   = 0.918926;
light_source.y   = 0.248436;
light_source.z   = -0.306359;

view_point.x = 0;
view_point.y = 40;
view_point.z = 0;

} // end Set_3D_View

////////////////////////////////////////////////////////////////////////////////

void Load_3D_Objects(void)
{
// this function loads the 3-D models

int index; // looping variable

// load in dynamic game objects

// load in missile template

PLG_Load_Object(&dynamic_obj[MISSILES_TEMPLATE],"missile.plg",2);

// first fix template object at (0,0,0)

dynamic_obj[MISSILES_TEMPLATE].world_pos.x = 0;
dynamic_obj[MISSILES_TEMPLATE].world_pos.y = 0;
dynamic_obj[MISSILES_TEMPLATE].world_pos.z = 0;


// load in tallon alien template

PLG_Load_Object(&dynamic_obj[TALLONS_TEMPLATE],"tallon.plg",1);

// first fix template object at (0,0,0)

dynamic_obj[TALLONS_TEMPLATE].world_pos.x = 0;
dynamic_obj[TALLONS_TEMPLATE].world_pos.y = 0;
dynamic_obj[TALLONS_TEMPLATE].world_pos.z = 0;

// the "state" field is going to track the current angle of the object

dynamic_obj[TALLONS_TEMPLATE].state = 0; // pointing directly down the positive
                                         // Z-axis, this is the neutral position

// load in slider alien template

PLG_Load_Object(&dynamic_obj[SLIDERS_TEMPLATE],"slider.plg",1);

// first fix template object at (0,0,0)

dynamic_obj[SLIDERS_TEMPLATE].world_pos.x = 0;
dynamic_obj[SLIDERS_TEMPLATE].world_pos.y = 0;
dynamic_obj[SLIDERS_TEMPLATE].world_pos.z = 0;

// the "state" field is going to track the current angle of the object

dynamic_obj[SLIDERS_TEMPLATE].state = 0; // pointing directly down the positive
                                         // Z-axis, this is the neutral position


// load in static game objects, background, obstacles, etc.

// load in obstacle one template

PLG_Load_Object(&static_obj[OBSTACLES_1_TEMPLATE],"pylons.plg",1);

// first fix template object at (0,0,0)

static_obj[OBSTACLES_1_TEMPLATE].world_pos.x = 0;
static_obj[OBSTACLES_1_TEMPLATE].world_pos.y = 0;
static_obj[OBSTACLES_1_TEMPLATE].world_pos.z = 0;

// now position all obstacle copies

for (index=0; index<NUM_OBSTACLES_1; index++)
    {

    obstacles_1[index].state = 1;
    obstacles_1[index].rx    = 0;
    obstacles_1[index].ry    = 0;
    obstacles_1[index].rz    = 0;
    obstacles_1[index].x     = -8000 + rand()%16000;
    obstacles_1[index].y     = 0;
    obstacles_1[index].z     = -8000 + rand()%16000;

    } // end for index


// load in obstacle two template

PLG_Load_Object(&static_obj[OBSTACLES_2_TEMPLATE],"rock.plg",1);

// first fix template object at (0,0,0)

static_obj[OBSTACLES_2_TEMPLATE].world_pos.x = 0;
static_obj[OBSTACLES_2_TEMPLATE].world_pos.y = 0;
static_obj[OBSTACLES_2_TEMPLATE].world_pos.z = 0;

// now position all obstacle copies

for (index=0; index<NUM_OBSTACLES_2; index++)
    {

    obstacles_2[index].state = 1;
    obstacles_2[index].rx    = 0;
    obstacles_2[index].ry    = 0;
    obstacles_2[index].rz    = 0;
    obstacles_2[index].x     = -8000 + rand()%16000;
    obstacles_2[index].y     = 0;
    obstacles_2[index].z     = -8000 + rand()%16000;

    } // end for index

// load in tower template

PLG_Load_Object(&static_obj[TOWERS_TEMPLATE],"tower.plg",2);

// first fix template object at (0,0,0)

static_obj[TOWERS_TEMPLATE].world_pos.x = 0;
static_obj[TOWERS_TEMPLATE].world_pos.y = 0;
static_obj[TOWERS_TEMPLATE].world_pos.z = 0;

// now position all tower copies

for (index=0; index<NUM_TOWERS; index++)
    {

    towers[index].state = 1;
    towers[index].rx    = 0;
    towers[index].ry    = 0;
    towers[index].rz    = 0;
    towers[index].x     = 0;
    towers[index].y     = 0;
    towers[index].z     = 0;

    } // end for index

// position the towers

towers[0].x = -1500;
towers[0].z = 1500;

towers[1].x = -1500;
towers[1].z = -1500;

towers[2].x = 1500;
towers[2].z = -1500;

towers[3].x = 1500;
towers[3].z = 1500;



// load in telepod template

PLG_Load_Object(&static_obj[TELEPODS_TEMPLATE],"tele.plg",2);

// first fix template object at (0,0,0)

static_obj[TELEPODS_TEMPLATE].world_pos.x = 0;
static_obj[TELEPODS_TEMPLATE].world_pos.y = 0;
static_obj[TELEPODS_TEMPLATE].world_pos.z = 0;

// now position all tower copies

for (index=0; index<NUM_TELEPODS; index++)
    {

    telepods[index].state = 1;
    telepods[index].rx    = 0;
    telepods[index].ry    = 0;
    telepods[index].rz    = 0;
    telepods[index].x     = 0;
    telepods[index].y     = 0;
    telepods[index].z     = 0;

    } // end for index

// position the telepods

telepods[0].x = -6000;
telepods[0].z = 6000;

telepods[1].x = -6000;
telepods[1].z = -6000;

telepods[2].x = 6000;
telepods[2].z = -6000;

telepods[3].x = 6000;
telepods[3].z = 6000;


// load in universe boundary template

PLG_Load_Object(&static_obj[BARRIERS_TEMPLATE],"barrier.plg",2);

// first fix template object at (0,0,0)

static_obj[BARRIERS_TEMPLATE].world_pos.x = 0;
static_obj[BARRIERS_TEMPLATE].world_pos.y = 0;
static_obj[BARRIERS_TEMPLATE].world_pos.z = 0;

// now position all barrier copies

for (index=0; index<NUM_BARRIERS; index++)
    {

    barriers[index].state = 1;
    barriers[index].rx    = 0;
    barriers[index].ry    = 15;
    barriers[index].rz    = 0;
    barriers[index].x     = 0;
    barriers[index].y     = 0;
    barriers[index].z     = 0;

    } // end for index

// position the barriers

barriers[0].x = -8000;
barriers[0].z = 8000;

barriers[1].x = -8000;
barriers[1].z = 0;

barriers[2].x = -8000;
barriers[2].z = -8000;

barriers[3].x = 0;
barriers[3].z = -8000;

barriers[4].x = 8000;
barriers[4].z = -8000;

barriers[5].x = 8000;
barriers[5].z = 0;

barriers[6].x = 8000;
barriers[6].z = 8000;

barriers[7].x = 0;
barriers[7].z = 8000;

// load in power station template

PLG_Load_Object(&static_obj[STATIONS_TEMPLATE],"station.plg",2);

// first fix template object at (0,0,0)

static_obj[STATIONS_TEMPLATE].world_pos.x = 0;
static_obj[STATIONS_TEMPLATE].world_pos.y = 0;
static_obj[STATIONS_TEMPLATE].world_pos.z = 0;

// now position all power stations

for (index=0; index<NUM_STATIONS; index++)
    {

    stations[index].state = 1;
    stations[index].rx    = 0;
    stations[index].ry    = 0;
    stations[index].rz    = 0;
    stations[index].x     = 0;
    stations[index].y     = 0;
    stations[index].z     = 0;

    } // end for index

} // end Load_3D_Models




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
   Sound_Load("KRKPOW.VOC" ,  (sound_ptr)&digital_FX[KRKPOW_VOC ],1);
   Digital_FX_Play(KRKPOW_VOC,3);
   Sound_Unload((sound_ptr)&digital_FX[KRKPOW_VOC]);
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