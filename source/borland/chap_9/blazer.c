
// STARBLAZER.C - Version 1.0

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

// D E F I N E S /////////////////////////////////////////////////////////////

// digital sound system

#define BLZCLK_VOC      0       // engaging the cloaking device
#define BLZEXP1_VOC     1       // standard explosion
#define BLZEXP2_VOC     2       // the super nova
#define BLZLAS_VOC      3       // laser torpedo sound
#define BLZNRG_VOC      4       // energy up sound
#define BLZSHLD_VOC     5       // shields engaging
#define BLZTAC_VOC      6       // tactical heads up
#define BLZSCN_VOC      7       // scanners turning on
#define BLZMISS_VOC     8       // mission debriefing

#define BLZBIOS_VOC     9       // bios is ok
#define BLZENTR_VOC     10      // entering game arena
#define BLZABRT_VOC     11      // function aborted
#define BLZSEL_VOC      12      // a selection was made
#define BLZKEY_VOC      13      // a key has been pressed
#define BLZDIAL_VOC     14      // the computer is dialing

#define BLZLOS_VOC      15      // the loser sound
#define BLZWIN_VOC      16      // the loser sound


#define NUM_SOUND_FX    17     // the number of sound fx loaded in

// music system

#define NUM_INTRO_SEQUENCES  11 // the number of elements in intro music score
#define NUM_GAME_SEQUENCES   18 // the number of elements in game music score

// modem defines

// defines for briefing instructions

#define NUM_PAGES          12
#define NUM_LINES_PAGE     17

// defines for the super nova explosion

#define NUM_CINDERS        20     // maximum number of cinders per explosion

#define CINDER_START_COLOR 48     // starting cinder color
#define CINDER_END_COLOR   48+15  // ending cinder color

#define NUM_NOVAS          3      // maximum number of super novas in game

#define NOVA_ACTIVE        1      // states of the novas
#define NOVA_INACTIVE      0

// heads up display defines

#define SCANNER_X             128  // position of scanner on screen
#define SCANNER_Y             166

#define MAX_BLIPS             50   // maximum number of scanner blips

#define BLIP_ASTEROID         9    // colors dor scanner blips
#define BLIP_PLAYER           10
#define BLIP_REMOTE           12
#define BLIP_ALIEN            13

#define RIGHT_HEADS_TEXT_X    240  // position of rightmost text for headsup
#define RIGHT_HEADS_TEXT_Y    120
											  // position of leftmost text for headsup
#define LEFT_HEADS_TEXT_X     0
#define LEFT_HEADS_TEXT_Y     120

#define HEADS_CLOAK         1    // bitmap indices for various headsup elements
#define HEADS_SCANNER       2
#define HEADS_COMM          3
#define HEADS_SHIPS         4
#define HEADS_ENERGY        5
#define HEADS_SHIELDS

#define HEADS_1             1
#define HEADS_2             2
#define HEADS_3             3

#define HEADS_GAUGE_1       1

// fuel cell defines

#define NUM_FUEL_CELLS      4   // number of fuel cells in game
#define NUM_FUEL_FRAMES     3   // number of animation frames for fuel cells

#define FUEL_CELL_ACTIVE    1   // states of a fuel cell
#define FUEL_CELL_ABSORBED  0

// alien defines
										  // number of alien animation frames
#define NUM_ALIEN_FRAMES    4
#define ALIEN_ODDS          50  // probabiliy of alien comming out

#define ALIEN_INACTIVE      0  // states of the alien
#define ALIEN_RANDOM        1  // the three modes of intelligence
#define ALIEN_CHASE_PLAYER  2
#define ALIEN_CHASE_REMOTE  3

// worm hole defines

#define NUM_WORMHOLE_FRAMES 8     // number of wormhole animation frames

#define WORMHOLE_X          1250  // position of wormhole
#define WORMHOLE_Y          1250

// starting position of the intro startup sequence

#define START_MESS_X        2
#define START_MESS_Y        8

// general explosions

#define NUM_EXPLOSIONS        4  // number of explosions that can run at once
#define NUM_EXPLOSION_FRAMES  6  // number frames in explosion

#define EXPLOSION_INACTIVE    0  // state of an inactive explosion
#define EXPLOSION_ACTIVE      1  // state of a live explosion

// missile defines

#define GENERIC_MISSILE    0

#define PLAYER_MISSILE     1   // this missile was fired by the local player
#define REMOTE_MISSILE     2   // this missile was fired by remote player

#define MISS_ACTIVE        1   // flags that a missile is alive
#define MISS_INACTIVE      0   // flags that a missile is dead
#define NUM_MISSILES       12  // number of total missiles allowed in world

// state of the game itself

#define GAME_SETUP                  0   // the game is in the setup mode
#define GAME_LINKING                1   // the communications link is being established
#define GAME_RUNNING                2   // the game is running
#define GAME_PAUSED                 3   // the game is paused (not implemented)
#define GAME_OVER                   4   // what do you think

// general object states

#define DEAD                        0   // these are general states for any
#define ALIVE                       1   // object
#define DYING                       2

// defines for setup selections

#define SETUP_PLAY_SOLO             0
#define SETUP_MAKE_CONNECTION       1
#define SETUP_WAIT_FOR_CONNECTION   2
#define SETUP_SELECT_SHIP           3
#define SETUP_SET_COMM_PORT         4
#define SETUP_BRIEFING              5
#define SETUP_EXIT                  6

#define NUM_SETUP                   7   // number of setup choices

// the coordinates of the setup display window

#define DISPLAY_X                   243 // position of the small setup display
#define DISPLAY_Y                   68  // box
#define DISPLAY_WIDTH               73  // size of window
#define DISPLAY_HEIGHT              28

#define DISPLAY_IMG_SHIPS           0   // commands to the display function
#define DISPLAY_IMG_PORTS           1   // indicating icons to display

// size of the "tech" font used in intro

#define TECH_FONT_WIDTH         4   // width of high tech font
#define TECH_FONT_HEIGHT        7   // height of high tech font
#define NUM_TECH_FONT           64  // number of characters in tech font

// asteroid defines

#define ASTEROID_SMALL          0   // different asteroid sizes
#define ASTEROID_MEDIUM         1
#define ASTEROID_LARGE          2

#define ASTEROID_INACTIVE       0   // asteroid states
#define ASTEROID_ACTIVE         1
#define ASTEROID_INOP           2

#define NUM_LARGE_ASTEROIDS     6   // max number of large asteroids in the game
#define NUM_MEDIUM_ASTEROIDS    10  // max number of medium asteroids in game
#define NUM_SMALL_ASTEROIDS     24  // max number of small asteroids in game

#define START_LARGE_ASTEROIDS   0   // starting index of large asteroids
#define START_MEDIUM_ASTEROIDS  6   // starting index of medium asteroids
#define START_SMALL_ASTEROIDS   16  // starting index of small asteroids

#define END_LARGE_ASTEROIDS     (0+6-1)    // ending indices
#define END_MEDIUM_ASTEROIDS    (6+10-1)
#define END_SMALL_ASTEROIDS     (16+24-1)

#define ASTEROID_LARGE_WIDTH    34  // size of large asteroids
#define ASTEROID_LARGE_HEIGHT   28

#define ASTEROID_MEDIUM_WIDTH   16  // size of medium asteroids
#define ASTEROID_MEDIUM_HEIGHT  14

#define ASTEROID_SMALL_WIDTH    8   // size of small asteroids
#define ASTEROID_SMALL_HEIGHT   6

#define NUM_ASTEROID_FRAMES     8   // number of asteroid animation frames

#define NUM_ASTEROIDS  (6+10+24)    // total number of asteroids in universe

// defines for the star field

#define NUM_STARS                50   // number of stars in universe

#define STAR_PLANE_0             0    // far plane
#define STAR_PLANE_1             1    // near plane
#define STAR_PLANE_2             2

#define STAR_COLOR_0             8    // color of farthest star plane
#define STAR_COLOR_1             7
#define STAR_COLOR_2             15   // color of nearest star plane

// dimensions of the universe

#define UNIVERSE_WIDTH           2500
#define UNIVERSE_HEIGHT          2500
#define UNIVERSE_BORDER          160   // the thickness of the border around
                                       // the universe
// defines for ships

#define GRYFON_SHIP             0
#define RAPTOR_SHIP             1

#define SHIP_WIDTH              22     // size of both ships
#define SHIP_HEIGHT             18


// shield defines

#define PLAYERS_SHIELD_REG      240   // color registers of players shield
#define REMOTES_SHIELD_REG      241   // color and remotes shield color

// engine defines

#define PLAYERS_ENGINE_REG      242  // color registers for engine flicker
#define REMOTES_ENGINE_REG      243

// introduction panel colors

#define END_PANEL_REG           41   // the color register range for the
#define START_PANEL_REG         32   // scrolling red lights on setup screen

// general identifiers for the player and remote

#define THE_PLAYER             0
#define THE_REMOTE             1

#define WINNER_NONE            0
#define WINNER_PLAYER          1
#define WINNER_REMOTE          2


// remote keyboard events

#define REMOTE_RIGHT           1   // each one of these bits is set or reset
#define REMOTE_LEFT            2   // in a single byte and sent to the remote
#define REMOTE_THRUST          4   // to indicate what the local player did
#define REMOTE_SHIELDS         8
#define REMOTE_CLOAK           16
#define REMOTE_FIRE            32
#define REMOTE_ESC             64

// S T R U C T U R E S ///////////////////////////////////////////////////////

// this is the structure for the asteroids

typedef struct asteroid_typ
        {
        int xv;                 // x velocity of asteroid
        int yv;                 // y velocity of asteroid
        int x,y;                // universe coordinates

        int type;               // type of asteroid, big, medium, small
        sprite rock;            // the asteroid sprite

        } asteroid, *asteroid_ptr;

// this is the structure for the stars

typedef struct star_typ
        {
        int x,y;                   // position of star
        int color;                 // color of star
        int plane;                 // plane that star is in
        int back_color;            // the color of the pixel under the star

        } star, *star_ptr;

// this is the structure used for weapons and explosion particles

typedef struct particle_type
        {
        int x,y;                    // universe position
        int sx,sy;                  // screen coordinates
        int xv,yv;                  // velocity
        int type;                   // type of particle
        int state;                  // state of particle
        int counter;                // general counter
        int threshold;              // threshold for counter
        int color;                  // color of particle
        int back_color;             // the color of the pixel under the particle
        int visible;                // helps speed clipping
        int lifetime;               // the liftime of the particle in frames

        } particle, *particle_ptr;

// this is the structure for a super nova explosion

typedef struct nova_typ
        {
        int state;                      // state of super nova explosion
        int x,y;                        // position of super nova explosion
        int lifetime;                   // the lifetime of the super nova

        particle cinders[NUM_CINDERS]; // particles of the explosion

        } nova, *nova_ptr;

// this is the structure for all simple creatures

typedef struct monster_typ
        {

        int x,y;        // position in universe coordinates
        int xv,yv;      // velocity of monster
        int state;      // state fo monster
        int type;       // type of monster

        sprite body;    // the body sprite for monster

        } monster, *monster_ptr;

// this is a scanner object

typedef struct blip_typ
        {
        int x,y;    // position of blip
        int color;  // color of blip i.e. type of scanner object

        } blip, *blip_ptr;

// P R O T O T Y P E S //////////////////////////////////////////////////////

void Init_Stars(void);

void Move_Stars(void);

void Draw_Stars(void);

void Erase_Stars(void);

void Under_Stars(void);

void Init_Asteroids(int small, int medium, int large);

void Start_Asteroid(int x,int y,int type);

void Erase_Asteroids(void);

void Draw_Asteroids(void);

void Under_Asteroids(void);

void Move_Asteroids(void);

void Font_Engine_1(int x,int y,
                   int font,int color,
                   char *string,unsigned char far *destination);

void Clear_Display(int color);

void Intro_Title(void);

void Intro_Controls(void);

void Load_Icons(void);

void Load_Ships(void);

void Do_Starburst(void);

int Display_Select(int current);

void Copy_Frames(sprite_ptr dest, sprite_ptr source);

void Shield_Control(int ship,int on);

void Reset_System(void);

void Start_Players_Death(void);

void Reset_Player(void);

void Reset_Remote(void);

void Start_Remotes_Death(void);

void Panel_FX(void);

void Erase_Missiles(void);

void Under_Missiles(void);

void Draw_Missiles(void);

void Init_Missiles(void);

void Move_Missiles(void);

int Start_Missile(int x,int y,int xv,int yv, int color,int type);

void Start_Explosion(int x,int y,int speed);

void Under_Explosions(void);

void Erase_Explosions(void);

void Draw_Explosions(void);

void Animate_Explosions(void);

void Init_Explosions(void);

void Tech_Print(int x,int y,char *string,unsigned char far *destination);

void Load_Wormhole(void);

void Init_Wormhole(void);

void Under_Wormhole(void);

void Erase_Wormhole(void);

void Draw_Wormhole(void);

void Animate_Wormhole(void);

void Load_Alien(void);

void Init_Alien(void);

void Alien_Control(void);

void Under_Alien(void);

void Erase_Alien(void);

void Draw_alien(void);

void Move_Alien(void);

void Load_Fuel_Cells(void);

void Init_Fuel_Cells(void);

void Under_Fuel_Cells(void);

void Erase_Fuel_Cells(void);

void Draw_Fuel_Cells(void);

void Animate_Fuel_Cells(void);

void Init_Scanner(void);

void Erase_Scanner(void);

void Draw_Scanner(void);

void Init_Novas(void);

void Erase_Novas(void);

void Under_Novas(void);

void Draw_Novas(void);

void Move_Novas(void);

void Start_Nova(int x,int y);

void Music_Init(void);

void Music_Close(void);

void Digital_FX_Init(void);

void Digital_FX_Close(void);

int Digital_FX_Play(int the_effect, int priority);

void Draw_Blips(void);

void Under_Blips(void);

void Erase_Blips(void);

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

// F U N C T I O N S ////////////////////////////////////////////////////////

int Get_Line(char *buffer)
{
// this function implements a crude line editor, it's used to input strings
// from keyboard

int index=0,
    ch;

// get the input string

while(1)
     {

     // has user hiy a key?

     if (kbhit())
        {
        // make a sound

        Digital_FX_Play(BLZKEY_VOC,2);

        // get the key

        ch = getch();

        // test for a numeric character

        if (ch>='0' && ch<='9')
           {

           buffer[index]   = ch;
           buffer[index+1] = 0;

           Font_Engine_1(DISPLAY_X+2,DISPLAY_Y+2+8,0,0,
                         buffer,
                         video_buffer);

           // test if end of line reached

           if (++index==12)
              index=11;

           } // end if numeric
        else
        if (ch==13)  // test for enter
           {
           // user is done, so exit

           // null terminate

           buffer[index] = 0;

           return(1);

           } // end if carriage return
        else
        if (ch==8 || ch==127) // back space or delete
           {
           if (--index <0)
              index=0;

           buffer[index]  =' ';
           buffer[index+1]=0;

           Font_Engine_1(DISPLAY_X+2,DISPLAY_Y+2+8,0,0,
                         buffer,
                         video_buffer);

           buffer[index]=0;

           // erase the character

           } // end if backspace
        else
        if (ch==27)
           return(0);

        } // end if kbhit

     } // end while

} // end Get_Line

//////////////////////////////////////////////////////////////////////////////

void Init_Novas(void)
{
// this function initializes all the super novas

int index_n, // looping variables
    index_c;

// process each nova

for (index_n=0; index_n<NUM_NOVAS; index_n++)
    {

    // set all novas to inactive

    novas[index_n].state    = NOVA_INACTIVE;
    novas[index_n].lifetime = 0;

    // intialize each cinder

    for (index_c=0; index_c<NUM_CINDERS; index_c++)
        {
        // clear out all fields

        novas[index_n].cinders[index_c].x = 0;
        novas[index_n].cinders[index_c].y = 0;

        novas[index_n].cinders[index_c].xv = 0;
        novas[index_n].cinders[index_c].yv = 0;

        novas[index_n].cinders[index_c].sx = 0;
        novas[index_n].cinders[index_c].sy = 0;

        novas[index_n].cinders[index_c].color = 0;
        novas[index_n].cinders[index_c].back_color = 0;

        // set timing fields

        novas[index_n].cinders[index_c].lifetime = 0;

        novas[index_n].cinders[index_c].counter   = 0;
        novas[index_n].cinders[index_c].threshold = 0;

        } // end for index

    } // end for index_n

} // end Init_Novas

/////////////////////////////////////////////////////////////////////////////

void Erase_Novas(void)
{
// this function replaces the what was under the novas

int index_n, // looping variables
    index_c;

particle_ptr bits;  // used as a high speed alias into cinders array

// process each nova

for (index_n=0; index_n<NUM_NOVAS; index_n++)
    {

    // is this nova active ?

    if (novas[index_n].state==NOVA_ACTIVE)
       {

       // alias a pointer to cinder array for speed

       bits = (particle_ptr)novas[index_n].cinders;

       // process each cinder

       for (index_c=0; index_c<NUM_CINDERS; index_c++)
           {

           // is this cinder visible?

           if (bits[index_c].visible)
              Write_Pixel_DB(bits[index_c].sx,bits[index_c].sy,
                             bits[index_c].back_color);

           } // end for index_c

       } // end if nova active

    } // end for index_n

} // end Erase_Novas

/////////////////////////////////////////////////////////////////////////////

void Under_Novas(void)
{

// this function scans whats under a nova

int index_n, // looping variables
    index_c,
    px_window,   // the starting postion of the players window
    py_window,
    cx,cy;       // local aliases for speed

particle_ptr bits;  // used as a high speed alias into cinders array

// compute starting position of players window so screen mapping can be done

px_window = players_x - 160+11;
py_window = players_y - 100+9;

// process each nova

for (index_n=0; index_n<NUM_NOVAS; index_n++)
    {

    // is this nova active ?

    if (novas[index_n].state==NOVA_ACTIVE)
       {

       // alias a pointer to cinder array for speed

       bits = (particle_ptr)novas[index_n].cinders;

       // process each cinder

       for (index_c=0; index_c<NUM_CINDERS; index_c++)
           {

           cx=bits[index_c].sx = (bits[index_c].x - px_window);
           cy=bits[index_c].sy = (bits[index_c].y - py_window);

           // test if cinder is visible on screen?

           if (cx>=320 || cx <0 || cy>=200  || cy<0)
              {

              // this cindere is invisible and has been clipped

              bits[index_c].visible = 0;

              // process next cinder

              continue;

              } // end visibility test

           // scan under cinder

           bits[index_c].back_color = Read_Pixel_DB(cx,cy);

           // set visibility flag

           bits[index_c].visible = 1;

           } // end for index_c

       } // end if nova active

    } // end for index_n

} // end Under_Novas

/////////////////////////////////////////////////////////////////////////////

void Draw_Novas(void)
{

// this function draws the novas

int index_n, // looping variables
    index_c;

particle_ptr bits;  // used as a high speed alias into cinders array

// process each nova

for (index_n=0; index_n<NUM_NOVAS; index_n++)
    {

    // is this nova active ?

    if (novas[index_n].state==NOVA_ACTIVE)
       {

       // alias a pointer to cinder array for speed

       bits = (particle_ptr)novas[index_n].cinders;

       // process each cinder

       for (index_c=0; index_c<NUM_CINDERS; index_c++)
           {

           // is this cinder visible?

           if (bits[index_c].visible)
              Write_Pixel_DB(bits[index_c].sx,bits[index_c].sy,
                             bits[index_c].color);

           } // end for index_c

       } // end if nova active

    } // end for index_n

} // end Draw_Novas

/////////////////////////////////////////////////////////////////////////////

void Move_Novas(void)
{

// this function draws the novas

int index_n, // looping variables
    index_c;

particle_ptr bits;  // used as a high speed alias into cinders array

// process each nova

for (index_n=0; index_n<NUM_NOVAS; index_n++)
    {

    // is this nova active ?

    if (novas[index_n].state==NOVA_ACTIVE)
       {

       // alias a pointer to cinder array for speed

       bits = (particle_ptr)novas[index_n].cinders;

       // process each cinder

       for (index_c=0; index_c<NUM_CINDERS; index_c++)
           {
           // move the cinder

           bits[index_c].x+=bits[index_c].xv;
           bits[index_c].y+=bits[index_c].yv;

           // animate the cinder color

           if (++bits[index_c].counter > bits[index_c].threshold)
              {
              // decrement color

              if (++bits[index_c].color >= CINDER_END_COLOR)
                 bits[index_c].color = CINDER_END_COLOR;

              // reset counter

              bits[index_c].counter = 0;

              } // end if color change

           } // end for index_c

       // age the nova

       if (--novas[index_n].lifetime<=0)
          novas[index_n].state=NOVA_INACTIVE;

       } // end if nova active

    } // end for index_n

} // end Move_Novas

///////////////////////////////////////////////////////////////////////////////

void Start_Nova(int x,int y)
{
// this function starts a super nova explosion

int index_c, // loopnig variables
    index_n;


// hunt for an inactive nove

for (index_n=0; index_n<NUM_NOVAS; index_n++)
    {

    // test for inactive nova

    if (novas[index_n].state==NOVA_INACTIVE)
       {

       // activate nova

       novas[index_n].state = NOVA_ACTIVE;

       // set lifetime in frames

       novas[index_n].lifetime = 50 + rand()%20;

       // intialize each cinder or restart a single cinder

       for (index_c=0; index_c<NUM_CINDERS; index_c++)
           {
           // fill in position, velocity and color of cinder

           novas[index_n].cinders[index_c].x   = x;
           novas[index_n].cinders[index_c].y   = y;
           novas[index_n].cinders[index_c].sx  = 0;
           novas[index_n].cinders[index_c].sy  = 0;
           novas[index_n].cinders[index_c].xv  = -8 + rand()%16;
           novas[index_n].cinders[index_c].yv  = -8 + rand()%16;

           novas[index_n].cinders[index_c].color      = CINDER_START_COLOR;
           novas[index_n].cinders[index_c].back_color = 0;

           // set timing fields

           novas[index_n].cinders[index_c].counter = 0;
           novas[index_n].cinders[index_c].threshold = 2 + rand()%6;

           } // end for index_c

           // make sound

           Digital_FX_Play(BLZEXP2_VOC,0);

       // break out of out for

       break;

       } // end if nova found

    } // end for index_n

} // end Start_Nova

//////////////////////////////////////////////////////////////////////////////

void Line_H_2(int x1,int x2,int y,int color,unsigned char far *dest)
{
// draw a horizontal line to the destination buffer

_fmemset((char far *)(dest + ((y<<8) + (y<<6)) + x1),
         (unsigned char)color,x2-x1+1);

} // end Line_H_2

//////////////////////////////////////////////////////////////////////////////

void Line_V_2(int y1,int y2,int x,int color, unsigned char far *dest)
{
// draw a vertical line to destination buffer

unsigned char far *start_offset; // starting memory offset of line

int index; // loop index

// compute starting position

start_offset = dest + ((y1<<8) + (y1<<6)) + x;

for (index=0; index<=y2-y1; index++)
    {
    // set the pixel

    *start_offset = (unsigned char)color;

    // move downward to next line

    start_offset+=320;

    } // end for index

} // end Line_V_2

//////////////////////////////////////////////////////////////////////////////

void Init_Stars(void)
{
// this function initializes all the stars in the star field

int index; // looping variable

for (index=0; index<NUM_STARS; index++)
    {

    // select plane that star will be in

    switch(rand()%3)
          {
          case STAR_PLANE_0:
               {
               stars[index].color = STAR_COLOR_0;
               stars[index].plane = STAR_PLANE_0;
               } break;

          case STAR_PLANE_1:
               {
               stars[index].color = STAR_COLOR_1;
               stars[index].plane = STAR_PLANE_1;
               } break;

          case STAR_PLANE_2:
               {
               stars[index].color = STAR_COLOR_2;
               stars[index].plane = STAR_PLANE_2;
               } break;

          default:break;

          } // end switch

     // set fields that aren't plane specific

     stars[index].x = rand()%320;  // change this latter to reflect clipping
     stars[index].y = rand()%200;  // region
     stars[index].back_color = 0;

    } // end for index

} // end Init_Stars

/////////////////////////////////////////////////////////////////////////////

void Move_Stars(void)
{
// this function moves the star field, note that the star field is always
// ins screen coordinates, otherwise, we would need thousands of stars to
// fill up the universe instead of 50!

int index,   // looping variable
    star_x,  // used as fast aliases to star position
    star_y,
    plane_0_dx,
    plane_0_dy,
    plane_1_dx,
    plane_1_dy,
    plane_2_dx,
    plane_2_dy;

// pre-compute plane translations

plane_0_dx = players_dx >> 2;
plane_0_dy = players_dy >> 2;

plane_1_dx = players_dx >> 1;
plane_1_dy = players_dy >> 1;

plane_2_dx = players_dx;
plane_2_dy = players_dy;

// move all the stars based on the motion of the player

for (index=0; index<NUM_STARS; index++)
    {
    // locally cache star position to speed up calculations

    star_x = stars[index].x;
    star_y = stars[index].y;

    // test which star field star is in so it is translated with correct
    // perspective

    switch(stars[index].plane)
          {
          case STAR_PLANE_0:
               {
               // move the star based on differntial motion of player
               // far plane is divided by 4

               star_x+=plane_0_dx;
               star_y+=plane_0_dy;

               } break;

          case STAR_PLANE_1:
               {
               // move the star based on differntial motion of player
               // middle plane is divided by 2

               star_x+=plane_1_dx;
               star_y+=plane_1_dy;

               } break;

          case STAR_PLANE_2:
               {
               // move the star based on differntial motion of player
               // near plane is divided by 1

               star_x+=plane_2_dx;
               star_y+=plane_2_dy;

               } break;

          } // end switch plane

    // test if a star has flown off an edge

    if (star_x >= 320)
        star_x = star_x-320;
    else
    if (star_x < 0)
        star_x = 320 + star_x;

    if (star_y >= 200)
        star_y = star_y-200;
    else
    if (star_y < 0)
        star_y = 200+star_y;

    // reset stars position in structure

    stars[index].x = star_x;
    stars[index].y = star_y;

    } // end for index

} // end Move_Stars

/////////////////////////////////////////////////////////////////////////////

void Draw_Stars(void)
{
// this function draws all the stars

int index; // looping variable

for (index=0; index<NUM_STARS; index++)
    Write_Pixel_DB(stars[index].x,stars[index].y,stars[index].color);

} // end Draw_Stars

/////////////////////////////////////////////////////////////////////////////

void Erase_Stars(void)
{
// this function erases all the stars

int index; // looping variable

for (index=0; index<NUM_STARS; index++)
    Write_Pixel_DB(stars[index].x,stars[index].y,stars[index].back_color);

} // end Erase_Stars

/////////////////////////////////////////////////////////////////////////////

void Under_Stars(void)
{
// this function scans under each star

int index; // looping variable

for (index=0; index<NUM_STARS; index++)
    stars[index].back_color = Read_Pixel_DB(stars[index].x,stars[index].y);

} // end Under_Stars

/////////////////////////////////////////////////////////////////////////////

void Init_Asteroids(int small, int medium, int large)
{
// this function loads in the imagery for the asteroids, allocates all the
// memory for them, sets up all the fields and starts them at random positions

int index,  // looping variables
    frame;

static int first_time=1;  // used to track first time this function is called

// intialize all the large asteroid sprites

for (index=START_LARGE_ASTEROIDS; index<=END_LARGE_ASTEROIDS; index++)
    {

    if (first_time)
    Sprite_Init((sprite_ptr)&asteroids[index].rock,
                0,0,
                ASTEROID_LARGE_WIDTH,ASTEROID_LARGE_HEIGHT,
                0,0,0,0,0,0);

    // set position,velocity and type fields

    asteroids[index].xv         = -8 + rand()%16;
    asteroids[index].yv         = -8 + rand()%16;
    asteroids[index].x          = rand()%UNIVERSE_WIDTH;
    asteroids[index].y          = rand()%UNIVERSE_HEIGHT,

    asteroids[index].type       = ASTEROID_LARGE;
    asteroids[index].rock.state = ASTEROID_INACTIVE;

    // use this to control rotation rate

    asteroids[index].rock.threshold_1 = 1 + rand()%3;

    } // end for index

// intialize all the medium asteroid sprites

for (index=START_MEDIUM_ASTEROIDS; index<=END_MEDIUM_ASTEROIDS; index++)
    {

    if (first_time)
    Sprite_Init((sprite_ptr)&asteroids[index].rock,
                0,0,
                ASTEROID_MEDIUM_WIDTH,ASTEROID_MEDIUM_HEIGHT,
                0,0,0,0,0,0);

    // set velocity and type fields

    asteroids[index].xv         = -6 + rand()%12;
    asteroids[index].yv         = -6 + rand()%12;
    asteroids[index].x          = rand()%UNIVERSE_WIDTH;
    asteroids[index].y          = rand()%UNIVERSE_HEIGHT,
    asteroids[index].type       = ASTEROID_MEDIUM;
    asteroids[index].rock.state = ASTEROID_INACTIVE;

    // use this to control rotation rate

    asteroids[index].rock.threshold_1 = 1 + rand()%3;

    } // end for index

// intialize all the small asteroid sprites

for (index=START_SMALL_ASTEROIDS; index<=END_SMALL_ASTEROIDS; index++)
    {

    if (first_time)
    Sprite_Init((sprite_ptr)&asteroids[index].rock,
                0,0,
                ASTEROID_SMALL_WIDTH,ASTEROID_SMALL_HEIGHT,
                0,0,0,0,0,0);

    // set velocity and type fields

    asteroids[index].xv         = -4 + rand()%8;
    asteroids[index].yv         = -4 + rand()%8;
    asteroids[index].x          = rand()%UNIVERSE_WIDTH;
    asteroids[index].y          = rand()%UNIVERSE_HEIGHT,
    asteroids[index].type       = ASTEROID_SMALL;
    asteroids[index].rock.state = ASTEROID_INACTIVE;

    // use this to control rotation rate

    asteroids[index].rock.threshold_1 = 1 + rand()%3;

    } // end for index

// now load the imagery for the large asteroids

if (first_time)
{

PCX_Init((pcx_picture_ptr)&image_pcx);
PCX_Load("blazelas.pcx", (pcx_picture_ptr)&image_pcx,1);

// extract the bitmaps for the asteroid, there are 8 animation cells

for (index=0; index<NUM_ASTEROID_FRAMES; index++)
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,
                   (sprite_ptr)&asteroids[START_LARGE_ASTEROIDS].rock,
                   index,index,0);

// now alias pointers of remaining asteroids sprites to same data that
// was allocated for the first large asteroids, this saves a lot of memory!

for (index=START_LARGE_ASTEROIDS+1; index<=END_LARGE_ASTEROIDS; index++)
    {
    // alias all the frame image pointer of this asteroid to the frames
    // of the first asteroid, no need to replicate this data in memory!

    for (frame=0; frame<NUM_ASTEROID_FRAMES; frame++)
        {
        // the image frames are within the sprite which is called rock which
        // is within the asteroid structure

        asteroids[index].rock.frames[frame] =
                asteroids[START_LARGE_ASTEROIDS].rock.frames[frame];

        } // end for frame

    // set number of frames field

    asteroids[index].rock.num_frames =
            asteroids[START_LARGE_ASTEROIDS].rock.num_frames;

    } // end for index

// delete the pcx file

PCX_Delete((pcx_picture_ptr)&image_pcx);


// now load the imagery for the medium asteroids

PCX_Init((pcx_picture_ptr)&image_pcx);
PCX_Load("blazemas.pcx", (pcx_picture_ptr)&image_pcx,1);

// extract the bitmaps for the asteroid, there are 8 animation cells

for (index=0; index<NUM_ASTEROID_FRAMES; index++)
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,
                   (sprite_ptr)&asteroids[START_MEDIUM_ASTEROIDS].rock,
                   index,index,0);

// now alias pointers of remaining asteroids sprites to same data that
// was allocated for the first large asteroids, this saves a lot of memory!

for (index=START_MEDIUM_ASTEROIDS+1; index<=END_MEDIUM_ASTEROIDS; index++)
    {
    // alias all the frame image pointer of this asteroid to the frames
    // of the first asteroid, no need to replicate this data in memory!

    for (frame=0; frame<NUM_ASTEROID_FRAMES; frame++)
        {
        // the image frames are within the sprite which is called rock which
        // is within the asteroid structure

        asteroids[index].rock.frames[frame] =
                       asteroids[START_MEDIUM_ASTEROIDS].rock.frames[frame];

        } // end for frame

    // set number of frames field

    asteroids[index].rock.num_frames =
            asteroids[START_MEDIUM_ASTEROIDS].rock.num_frames;

    } // end for index

// delete the pcx file

PCX_Delete((pcx_picture_ptr)&image_pcx);

// finally load the imagery for the small asteroids

PCX_Init((pcx_picture_ptr)&image_pcx);
PCX_Load("blazesas.pcx", (pcx_picture_ptr)&image_pcx,1);

// extract the bitmaps for the asteroid, there are 8 animation cells

for (index=0; index<NUM_ASTEROID_FRAMES; index++)
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,
                   (sprite_ptr)&asteroids[START_SMALL_ASTEROIDS].rock,
                   index,index,0);

// now alias pointers of remaining asteroids sprites to same data that
// was allocated for the first large asteroids, this saves a lot of memory!

for (index=START_SMALL_ASTEROIDS+1; index<=END_SMALL_ASTEROIDS; index++)
    {
    // alias all the frame image pointer of this asteroid to the frames
    // of the first asteroid, no need to replicate this data in memory!

    for (frame=0; frame<NUM_ASTEROID_FRAMES; frame++)
        {
        // the image frames are within the sprite which is called rock which
        // is within the asteroid structure

        asteroids[index].rock.frames[frame] =
                       asteroids[START_SMALL_ASTEROIDS].rock.frames[frame];

        } // end for frame

    // set number of frames field

    asteroids[index].rock.num_frames =
            asteroids[START_SMALL_ASTEROIDS].rock.num_frames;

    } // end for index

// delete the pcx file

PCX_Delete((pcx_picture_ptr)&image_pcx);

} // end if first time



// now start up the requested number of asteroids

// first the large

for (index=0; index<large; index++)
    {
    // look for inactive asteroids to start up

    if (asteroids[index+START_LARGE_ASTEROIDS].rock.state == ASTEROID_INACTIVE)
        asteroids[index+START_LARGE_ASTEROIDS].rock.state = ASTEROID_ACTIVE;

    } // end for index

// now the medium

for (index=0; index<medium; index++)
    {
    // look for inactive asteroids to start up

    if (asteroids[index+START_MEDIUM_ASTEROIDS].rock.state == ASTEROID_INACTIVE)
        asteroids[index+START_MEDIUM_ASTEROIDS].rock.state = ASTEROID_ACTIVE;

    } // end for index

// finally the small

for (index=0; index<small; index++)
    {
    // look for inactive asteroids to start up

    if (asteroids[index+START_SMALL_ASTEROIDS].rock.state == ASTEROID_INACTIVE)
        asteroids[index+START_SMALL_ASTEROIDS].rock.state = ASTEROID_ACTIVE;

    } // end for index

// test if this was the first tmie the function was called

if (first_time)
    first_time=0;

// what a pain!

} // end Init_Asteroids

/////////////////////////////////////////////////////////////////////////////

void Start_Asteroid(int x,int y,int type)
{
// this function is used to start up an asteroid at the sent position
// later possible implement velocity?

int index;  // looping variable

// which kind of asteroid is being requested?

switch(type)
      {
      case ASTEROID_LARGE:
           {
           // scan for inactive asteroid

           for (index=START_LARGE_ASTEROIDS; index<=END_LARGE_ASTEROIDS; index++)
               {
               // is this asteroid being used?

               if (asteroids[index].rock.state==ASTEROID_INACTIVE)
                  {
                  // set fields of asteroid and return
                  asteroids[index].xv         = -8 + rand()%16;
                  asteroids[index].yv         = -8 + rand()%16;
                  asteroids[index].x          = x;
                  asteroids[index].y          = y,
                  asteroids[index].rock.state = ASTEROID_ACTIVE;
                  return;

                  } // end if found an asteroid

               } // end for index

           } break;

      case ASTEROID_MEDIUM:
           {
           // scan for inactive asteroid

           for (index=START_MEDIUM_ASTEROIDS; index<=END_MEDIUM_ASTEROIDS; index++)
               {
               // is this asteroid being used?

               if (asteroids[index].rock.state==ASTEROID_INACTIVE)
                  {
                  // set fields of asteroid and return
                  asteroids[index].xv         = -6 + rand()%12;
                  asteroids[index].yv         = -6 + rand()%12;
                  asteroids[index].x          = x;
                  asteroids[index].y          = y,
                  asteroids[index].rock.state = ASTEROID_ACTIVE;
                  return;

                  } // end if found an asteroid

               } // end for index

           } break;

      case ASTEROID_SMALL:
           {
           // scan for inactive asteroid

           for (index=START_SMALL_ASTEROIDS; index<=END_SMALL_ASTEROIDS; index++)
               {
               // is this asteroid being used?

               if (asteroids[index].rock.state==ASTEROID_INACTIVE)
                  {
                  // set fields of asteroid and return

                  asteroids[index].xv         = -4 + rand()%8;
                  asteroids[index].yv         = -4 + rand()%8;
                  asteroids[index].x          = x;
                  asteroids[index].y          = y,
                  asteroids[index].rock.state = ASTEROID_ACTIVE;
                  return;

                  } // end if found an asteroid

               } // end for index

           } break;

      } // end swtich

} // end Start_Asteroid

//////////////////////////////////////////////////////////////////////////////

void Erase_Asteroids(void)
{
// this function traverses the asteroid list and erases all asteroids that
// are active

int index;

// erase all asteroids that are within screen window

for (index=0; index<NUM_ASTEROIDS; index++)
    {
    // test if this asteroids is active

    if (asteroids[index].rock.state==ASTEROID_ACTIVE)
       Sprite_Erase_Clip((sprite_ptr)&asteroids[index].rock,double_buffer);

    } // end for index

} // Erase_Asteroids

//////////////////////////////////////////////////////////////////////////////

void Draw_Asteroids(void)
{
// this function traverses the asteroid list and draws all asteroids that
// are active

int index;

for (index=0; index<NUM_ASTEROIDS; index++)
    {
    // test if this asteroids is active

    if (asteroids[index].rock.state==ASTEROID_ACTIVE)
        Sprite_Draw_Clip((sprite_ptr)&asteroids[index].rock,double_buffer,1);

    } // end for index

} // Draw_Asteroids

//////////////////////////////////////////////////////////////////////////////

void Under_Asteroids(void)
{

// this function traverses the asteroid list and scans under all asteroids that
// are active, note that this function is the only one that computes the screen
// coordinates of the asteroid sprites, placing the computation in the other
// functions would be redundant, hence the sprite coordinates at this function
// are undefined, and after this function they have been remapped to the video
// screen relative to the players position


int index,
    px_window,   // the starting postion of the players window
    py_window;

// compute starting position of players window so screen mapping can be done

px_window = players_x - 160+11;
py_window = players_y - 100+9;

// now scan under all asteroids

for (index=0; index<NUM_ASTEROIDS; index++)
    {
    // test if this asteroids is active

    if (asteroids[index].rock.state==ASTEROID_ACTIVE)
       {

       // postion asteroid correctly on view screen, note this is very similar
       // to what we will do in 3-D when we translate all the objects in the
       // universe to the viewer position

       asteroids[index].rock.x = (asteroids[index].x-px_window);
       asteroids[index].rock.y = (asteroids[index].y-py_window);

       Sprite_Under_Clip((sprite_ptr)&asteroids[index].rock,double_buffer);

       } // end if active

    } // end for index

} // Under_Asteroids

//////////////////////////////////////////////////////////////////////////////

void Move_Asteroids(void)
{

// this function traverses the asteroid list and moves and test for collsions
// note that the sprite positions of the asteroids are not touched only the
// universe or "world" coordinates are

int index,         // looping variable
    ast_x,ast_y;   // used for aliasing

// process each asteroid

for (index=0; index<NUM_ASTEROIDS; index++)
    {
    // test if this asteroids is active

    if (asteroids[index].rock.state==ASTEROID_ACTIVE)
       {
       // move the asteroid

       ast_x=asteroids[index].x;
       ast_y=asteroids[index].y;

       ast_x+=asteroids[index].xv;
       ast_y+=asteroids[index].yv;

       // test if asteroid is off screen bounadary

       if (ast_x>UNIVERSE_WIDTH+UNIVERSE_BORDER)
           ast_x = -UNIVERSE_BORDER;
       else
       if (ast_x<-UNIVERSE_BORDER)
           ast_x = UNIVERSE_WIDTH+UNIVERSE_BORDER;

       if (ast_y>UNIVERSE_HEIGHT+UNIVERSE_BORDER)
           ast_y = -UNIVERSE_BORDER;
       else
       if (ast_y<-UNIVERSE_BORDER)
           ast_y = UNIVERSE_HEIGHT+UNIVERSE_BORDER;

       // restore variables in structure

       asteroids[index].x = ast_x;
       asteroids[index].y = ast_y;

       // animate asteroid

       if (++asteroids[index].rock.counter_1>=asteroids[index].rock.threshold_1)
          {
          // reset counter

          asteroids[index].rock.counter_1 = 0;

          if (++asteroids[index].rock.curr_frame >= NUM_ASTEROID_FRAMES)
              asteroids[index].rock.curr_frame = 0;

          } // end if time to animate

       // test if asteroid has hit player

       // test for collision

       if (players_x+SHIP_WIDTH/2  >= ast_x                             &&
           players_y+SHIP_HEIGHT/2 >= ast_y                             &&
           players_x+SHIP_WIDTH/2  <= ast_x+asteroids[index].rock.width &&
           players_y+SHIP_HEIGHT/2 <= ast_y+asteroids[index].rock.height)
          {
          // kill the asteroid and the missile

          asteroids[index].rock.state = ASTEROID_INACTIVE;

          // what kind of asteroid did we have?

          switch(asteroids[index].type)
                {

                case ASTEROID_LARGE:
                     {

                     // start one medium and one small asteroid
                     //  (if possible)

                     Start_Asteroid(asteroids[index].x,
                                    asteroids[index].y,
                                    ASTEROID_MEDIUM);

                     Start_Asteroid(asteroids[index].x+ASTEROID_LARGE_WIDTH/2,
                                    asteroids[index].y+ASTEROID_LARGE_HEIGHT/2,
                                    ASTEROID_SMALL);

                     } break;

                case ASTEROID_MEDIUM:
                     {

                     // start two small asteroids (if possible)

                     Start_Asteroid(asteroids[index].x,
                                    asteroids[index].y,
                                    ASTEROID_SMALL);

                     Start_Asteroid(asteroids[index].x+ASTEROID_MEDIUM_WIDTH/2,
                                    asteroids[index].y+ASTEROID_MEDIUM_HEIGHT/2,
                                    ASTEROID_SMALL);

                     } break;

                case ASTEROID_SMALL:
                     {

                     // start a randomly positioned asteroid of any size
                     // at the worm hole

                     Start_Asteroid(WORMHOLE_X,WORMHOLE_Y,rand()%3);

                     } break;

                default:break;

                } // end switch

          // were shields up?

          if (players_shields)
             {
             // start explosion
             Start_Explosion(players_x,players_y,2);
             } // end if shields up
          else
             { // say bye bye!

             Start_Explosion(players_x,players_y,2);
             Start_Nova(players_x+SHIP_WIDTH/2,players_y+SHIP_HEIGHT/2);

             // start players death sequence

             Start_Players_Death();

             } // end if else

          } // end if player has been hit

       // test if asteroid has hit remote

       // test for collision

       if (linked)
       {
       if (remotes_x+SHIP_WIDTH/2  >= ast_x                             &&
           remotes_y+SHIP_HEIGHT/2 >= ast_y                             &&
           remotes_x+SHIP_WIDTH/2  <= ast_x+asteroids[index].rock.width &&
           remotes_y+SHIP_HEIGHT/2 <= ast_y+asteroids[index].rock.height)
          {
          // kill the asteroid and the missile

          asteroids[index].rock.state = ASTEROID_INACTIVE;

          // what kind of asteroid did we have?

          switch(asteroids[index].type)
                {

                case ASTEROID_LARGE:
                     {
                     // start one medium and one small asteroid
                     //  (if possible)

                     Start_Asteroid(asteroids[index].x,
                                    asteroids[index].y,
                                    ASTEROID_MEDIUM);

                     Start_Asteroid(asteroids[index].x+ASTEROID_LARGE_WIDTH/2,
                                    asteroids[index].y+ASTEROID_LARGE_HEIGHT/2,
                                    ASTEROID_SMALL);

                     } break;

                case ASTEROID_MEDIUM:
                     {
                     // start two small asteroids (if possible)

                     Start_Asteroid(asteroids[index].x,
                                    asteroids[index].y,
                                    ASTEROID_SMALL);

                     Start_Asteroid(asteroids[index].x+ASTEROID_MEDIUM_WIDTH/2,
                                    asteroids[index].y+ASTEROID_MEDIUM_HEIGHT/2,
                                    ASTEROID_SMALL);

                     } break;

                case ASTEROID_SMALL:
                     {

                     // start a randomly positioned asteroid of any size
                     // at the worm hole

                     Start_Asteroid(WORMHOLE_X,WORMHOLE_Y,rand()%3);

                     } break;

                default:break;

                } // end switch

          // were shields up?

          if (remotes_shields)
             {
             // start explosion
             Start_Explosion(remotes_x,remotes_y,2);
             } // end if shields up
          else
             { // say bye bye!

             Start_Explosion(remotes_x,remotes_y,2);
             Start_Nova(remotes_x+SHIP_WIDTH/2,remotes_y+SHIP_HEIGHT/2);

             // start remotes death sequence

             Start_Remotes_Death();

             } // end if else

          } // end if remote has been hit

       } // end if linked

       } // end if asteroids is active

    } // end for index

} // Move_Asteroids

//////////////////////////////////////////////////////////////////////////////

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

    // this is a little cursor kind of thing

    buffer[1] = '<';

    // null terminate

    buffer[2] = 0;

    // print the string

    Font_Engine_1(x,y,0,0,buffer,destination);

    // move to next position

    x+=(TECH_FONT_WIDTH+1);

    // wait a bit  1/70th of a second

    Wait_For_Vertical_Retrace();

    // clear the cursor

    } // end for

    // clear the cursor

    buffer[0] = ' ';
    buffer[1] = ' ';
    buffer[2] = 0;

    Font_Engine_1(x,y,0,0,buffer,destination);

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
   PCX_Load("blazefnt.pcx", (pcx_picture_ptr)&image_pcx,1);

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

void Clear_Display(int color)
{
// this function fills the setup display screen with a color

int y;

// clear display with horizontal lines

for (y=DISPLAY_Y; y<DISPLAY_Y + DISPLAY_HEIGHT; y++)
     Line_H(DISPLAY_X,DISPLAY_X+DISPLAY_WIDTH-1,y,color);

} // end Clear_Display

/////////////////////////////////////////////////////////////////////////////

void Intro_Title(void)
{

// load in the starblazer title screen

PCX_Init((pcx_picture_ptr)&image_pcx);
PCX_Load("blazeint.pcx",(pcx_picture_ptr)&image_pcx,1);

// done with data so delete it

PCX_Delete((pcx_picture_ptr)&image_pcx);

// show the PCX buffer

PCX_Show_Buffer((pcx_picture_ptr)&image_pcx);

// do special effects

// wait for a sec

Time_Delay(10);

Do_Starburst();

Time_Delay(50);

} // end Intro_Title

//////////////////////////////////////////////////////////////////////////////

void Closing_Screen(void)
{
// this function prints the credits

// blank the screen

Fill_Screen(0);

// restore pallete

Write_Palette(0,255,(RGB_palette_ptr)&game_palette);

if (music_enabled)
   {
   Music_Stop();
   Music_Play((music_ptr)&song,10);
   }

// draw the credits

Tech_Print(10,50,"MUSICAL MASTERY BY",video_buffer);
Time_Delay(20);
Tech_Print(20,60,"DEAN HUDSON OF",video_buffer);
Time_Delay(20);
Tech_Print(30,70,"ECLIPSE PRODUCTIONS",video_buffer);
Time_Delay(20);

Tech_Print(10,100,"MIDPAK INSTRUMENTATION CONSULTING BY",video_buffer);
Time_Delay(20);
Tech_Print(20,110,"ROB WALLACE OF",video_buffer);
Time_Delay(20);
Tech_Print(30,120,"WALLACE MUSIC & SOUND",video_buffer);

// wait a sec

Time_Delay(125);

// fade away

Screen_Transition(SCREEN_DARKNESS);

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

void Intro_Controls(void)
{
// this function displays the controls screen

// load in the starblazer controls screen

PCX_Init((pcx_picture_ptr)&image_controls);
PCX_Load("blazecon.pcx",(pcx_picture_ptr)&image_controls,1);

// copy controls data to video buffer

PCX_Show_Buffer((pcx_picture_ptr)&image_controls);

// scan under button sprite and draw

Sprite_Under((sprite_ptr)&button_1,video_buffer);
Sprite_Draw((sprite_ptr)&button_1,video_buffer,1);

// delete pcx file

PCX_Delete((pcx_picture_ptr)&image_controls);

} // end Intro_Controls

////////////////////////////////////////////////////////////////////////////

void Intro_Briefing(void)
{
// this function displays the controls screen

int done=0, // exit flag
    page=0, // current page user is reading
    index;  // looping variable

// load in the starblazer controls screen

PCX_Init((pcx_picture_ptr)&image_controls);
PCX_Load("blazeins.pcx",(pcx_picture_ptr)&image_controls,1);

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

           button_3.x = 185;
           button_3.y = 170;
           button_3.curr_frame = 3;

           Sprite_Draw((sprite_ptr)&button_3,video_buffer,1);

           Digital_FX_Play(BLZKEY_VOC,2);

           Time_Delay(2);

           button_3.curr_frame = 2;

           Sprite_Draw((sprite_ptr)&button_3,video_buffer,1);

           } // end if up

        if (keyboard_state[MAKE_DOWN])
           {
           // page down

           if (++page>=NUM_PAGES)
              page = NUM_PAGES-1;


           // press button

           button_3.x = 106;
           button_3.y = 170;
           button_3.curr_frame = 1;

           Sprite_Draw((sprite_ptr)&button_3,video_buffer,1);

           Digital_FX_Play(BLZKEY_VOC,2);

           Time_Delay(2);

           button_3.curr_frame = 0;

           Sprite_Draw((sprite_ptr)&button_3,video_buffer,1);

           } // end if down

        if (keyboard_state[MAKE_ESC])
           {
           done=1;
           } // end if esc

        // refresh display

        for (index=0; index<NUM_LINES_PAGE; index++)
            Font_Engine_1(78,24+index*8,0,0,instructions[index+page*17],video_buffer);

        } // end if a key has been pressed

     // do the scrolling lite thing

     Panel_FX();

     // wait a sec

     Time_Delay(1);

     } // end main while

} // end Intro_Briefing

////////////////////////////////////////////////////////////////////////////

void Load_Explosions(void)
{
// this function loads the bitmap explosions

int index,  // looping variable
    frames; // looping variable

// load the imagery for the explosions

PCX_Init((pcx_picture_ptr)&image_pcx);
PCX_Load("blazeexp.pcx", (pcx_picture_ptr)&image_pcx,1);

// load each explosion in

for (index=0; index<NUM_EXPLOSIONS; index++)
    {
    // initialize each sprite

    Sprite_Init((sprite_ptr)&explosions[index],0,0,28,22,0,0,0,0,0,0);

    // extract the animation frames

    for (frames=0; frames<NUM_EXPLOSION_FRAMES; frames++)
        PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&explosions[index],frames,frames,0);

    } // end index

// delete the pcx file

PCX_Delete((pcx_picture_ptr)&image_pcx);

} // end Load_Explosions

////////////////////////////////////////////////////////////////////////////

void Load_Icons(void)
{
// this function loads various icons for the game

int index; // looping variable

// load the imagery for the control buttons on the setup screen

PCX_Init((pcx_picture_ptr)&image_pcx);
PCX_Load("blazebt1.pcx", (pcx_picture_ptr)&image_pcx,1);

// intialize the button sprite

Sprite_Init((sprite_ptr)&button_1,118-10,63,10,8,0,0,0,0,0,0);

button_1.counter_1 = 0;  // button is on the 0th element in the list

// extract the bitmaps for the button, there are 4 animation cells

for (index=0; index<4; index++)
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&button_1,index,index,0);

// load in display selection buttons

// intialize the button sprite

Sprite_Init((sprite_ptr)&button_2,0,DISPLAY_Y+DISPLAY_HEIGHT-6,10,8,0,0,0,0,0,0);

// extract the bitmaps for the button, there are 2 animation cells

for (index=0; index<2; index++)
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&button_2,index,index,1);

// done with this PCX file so delete memory associated with it

PCX_Delete((pcx_picture_ptr)&image_pcx);


// load the imagery for the briefing control buttons

PCX_Init((pcx_picture_ptr)&image_pcx);
PCX_Load("blazebt3.pcx", (pcx_picture_ptr)&image_pcx,1);

// intialize the button sprite

Sprite_Init((sprite_ptr)&button_3,0,0,42,12,0,0,0,0,0,0);

// extract the bitmaps for the button, there are 4 animation cells

for (index=0; index<4; index++)
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&button_3,index,index,0);

// done with this PCX file so delete memory associated with it

PCX_Delete((pcx_picture_ptr)&image_pcx);

// load the imagery for the display bitmaps

PCX_Init((pcx_picture_ptr)&image_pcx);
PCX_Load("blazedis.pcx", (pcx_picture_ptr)&image_pcx,1);

// intialize the display sprite

Sprite_Init((sprite_ptr)&displays,DISPLAY_X,DISPLAY_Y+6,72,20,0,0,0,0,0,0);

// extract the bitmaps for the display bitmaps, there are 2 images

for (index=0; index<2; index++)
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&displays,index,index,0);

// done with this PCX file so delete memory associated with it

PCX_Delete((pcx_picture_ptr)&image_pcx);

} // end Load_Icons

////////////////////////////////////////////////////////////////////////////

void Load_Ships(void)
{
int index; // looping variable

// load the imagery for the local ships

PCX_Init((pcx_picture_ptr)&image_pcx);
PCX_Load("blazeshl.pcx", (pcx_picture_ptr)&image_pcx,1);

// load in the imagery for the local gryfon and raptor

Sprite_Init((sprite_ptr)&gryfon_l,0,0,22,18,0,0,0,0,0,0);
Sprite_Init((sprite_ptr)&raptor_l,0,0,22,18,0,0,0,0,0,0);

// there are 32 animation cells per ship

for (index=0; index<16; index++)
    {
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&gryfon_l,index,index%12,index/12);
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&raptor_l,index,index%12,2+index/12);

    // these frames are with engines on

    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&gryfon_l,index+16,index%12,4+index/12);
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&raptor_l,index+16,index%12,4+2+index/12);

    } // end for index

PCX_Delete((pcx_picture_ptr)&image_pcx);

// now the remote gryfon and raptor

PCX_Init((pcx_picture_ptr)&image_pcx);
PCX_Load("blazeshr.pcx", (pcx_picture_ptr)&image_pcx,1);

Sprite_Init((sprite_ptr)&gryfon_r,0,0,22,18,0,0,0,0,0,0);
Sprite_Init((sprite_ptr)&raptor_r,0,0,22,18,0,0,0,0,0,0);


// there are 32 animation cells per ship

for (index=0; index<16; index++)
    {
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&gryfon_r,index,index%12,index/12);
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&raptor_r,index,index%12,2+index/12);

    // these frames are with engines on

    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&gryfon_r,index+16,index%12,4+index/12);
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&raptor_r,index+16,index%12,4+2+index/12);

    } // end for index

// initialize the player and remote sprites

Sprite_Init((sprite_ptr)&players_ship,160-11,100-9,22,18,0,0,0,0,0,0);
Sprite_Init((sprite_ptr)&remotes_ship,0,0,22,18,0,0,0,0,0,0);

// intialize the starburst

Sprite_Init((sprite_ptr)&starburst,0,0,22,18,0,0,0,0,0,0);

// extract the bitmaps for the startburst, there are 6 animation cells

for (index=0; index<6; index++)
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&starburst,index,index,8);

PCX_Delete((pcx_picture_ptr)&image_pcx);

} // end Load_Ships

////////////////////////////////////////////////////////////////////////////

void Do_Starburst(void)
{
int x,y,number,index;

// select a random number of startb bursts

number = 2+rand()%3;

for (index=0; index<number; index++)
    {

    // select position for starburst

    starburst.x=160+(rand()%140);
    starburst.y=80+rand()%20;

    Sprite_Under((sprite_ptr)&starburst,video_buffer);

    // do starburst

    for (starburst.curr_frame=0; starburst.curr_frame<6; starburst.curr_frame++)
        {
        Sprite_Erase((sprite_ptr)&starburst,video_buffer);
        Sprite_Under((sprite_ptr)&starburst,video_buffer);
        Sprite_Draw((sprite_ptr)&starburst,video_buffer,1);

        Time_Delay(1+rand()%2);

        } // end for starburst frame

        // erase the starburst

        Sprite_Erase((sprite_ptr)&starburst,video_buffer);

    } // end index

} // end Do_Starburst

////////////////////////////////////////////////////////////////////////////

int Display_Select(int current)
{
// this function is used to select between two choices in the display window

// compute starting position of selection icon based on default selection

button_2.x = DISPLAY_X + 14 + current*40;

// scan under selection icon

Sprite_Under((sprite_ptr)&button_2,video_buffer);
Sprite_Draw((sprite_ptr)&button_2,video_buffer,1);

// until user exits process event loop

while(1)
     {
     // get input

     if (keys_active>0)
        {

        // what is user trying to do?

        if (keyboard_state[MAKE_RIGHT])
           {
           if (++current>1)
              current = 0;

           Digital_FX_Play(BLZKEY_VOC,2);

           Time_Delay(1);

           } // if if right


        else
        if (keyboard_state[MAKE_LEFT])
           {
           if (--current<0)
              current = 1;

           Digital_FX_Play(BLZKEY_VOC,2);

           Time_Delay(1);

           } // end if left

        else
        if (keyboard_state[MAKE_ESC])
           {
           // print that selection was aborted

           Clear_Display(0);

           Font_Engine_1(DISPLAY_X+2,DISPLAY_Y+2,0,0,
                         "ABORTED...",
                         video_buffer);

           Digital_FX_Play(BLZABRT_VOC,1);

           Time_Delay(5);

           Clear_Display(0);

           // return selection aborted

           return(-1);

           } // end if esc

        else
        if (keyboard_state[MAKE_ENTER])
           {
           // carriage return, making selection
           // illuminate button for a second

           // draw button down

           button_2.curr_frame = 1;
           Sprite_Draw((sprite_ptr)&button_2,video_buffer,1);

           Digital_FX_Play(BLZKEY_VOC,2);

           Time_Delay(5);

           // now draw button up

           button_2.curr_frame = 0;
           Sprite_Draw((sprite_ptr)&button_2,video_buffer,1);

           Clear_Display(0);

           Font_Engine_1(DISPLAY_X+2,DISPLAY_Y+2,0,0,
                         "SELECTION",
                         video_buffer);

           Font_Engine_1(DISPLAY_X+2,DISPLAY_Y+2+8,0,0,
                         "RECORDED",
                         video_buffer);

           Digital_FX_Play(BLZSEL_VOC,1);

           Time_Delay(5);

           Clear_Display(0);

           // return the selection

           return(current);

           } // end if enter

        // erase selection icon

        Sprite_Erase((sprite_ptr)&button_2,video_buffer);

        // compute x position

        button_2.x = DISPLAY_X + 14 + current*40;

        // scan under and draw selection icon

        Sprite_Under((sprite_ptr)&button_2,video_buffer);
        Sprite_Draw((sprite_ptr)&button_2,video_buffer,1);

        } // end if kbhit

     // perform special effects

     Panel_FX();

     // wait a bit

     Time_Delay(1);

     } // end while

} // Display_Select

///////////////////////////////////////////////////////////////////////////////

void Copy_Frames(sprite_ptr dest, sprite_ptr source)
{
// this function is used to copy the image frames from one sprite to another

int index; // looping variable

for (index=0; index<source->num_frames; index++)
    {
    // assign next frame

    dest->frames[index] = source->frames[index];

    } // end for index

// set up dest fields

dest->num_frames = source->num_frames;
dest->curr_frame = 0;

} // end Copy_Frames

/////////////////////////////////////////////////////////////////////////////

void Shield_Control(int ship,int on)
{
// this function is used to activate or de-activate the shields of the player
// or remote ship

if (ship==THE_PLAYER)
   {

   // which ship does player have?

   if (players_ship_type==GRYFON_SHIP)
      {
      // activating or de-activating shields?

      if (on)
         players_shield_color = primary_blue;
      else
         players_shield_color = primary_black;

      } // end if gryfon
   else
      {
      // activating or de-activating shields?

      if (on)
         players_shield_color = primary_red;
      else
         players_shield_color = primary_black;

       } // else raptor

   // set the color

   Write_Color_Reg(PLAYERS_SHIELD_REG,(RGB_color_ptr)&players_shield_color);

   // record shield change

   players_shields = on;

   } // end if players ship
else
   {
   // must be remote ship

   // which ship does remote have?

   if (remotes_ship_type==GRYFON_SHIP)
      {
      // activating or de-activating shields?

      if (on)
         remotes_shield_color = primary_blue;
      else
         remotes_shield_color = primary_black;
      } // end if gryfon
   else
      {
      // activating or de-activating shields?

      if (on)
         remotes_shield_color = primary_red;
      else
         remotes_shield_color = primary_black;
       } // else raptor

   // set the color

   Write_Color_Reg(REMOTES_SHIELD_REG,(RGB_color_ptr)&remotes_shield_color);

   // record shield change

   remotes_shields = on;

   } // end must be remote

} // end Shield_Control

///////////////////////////////////////////////////////////////////////////////

void Erase_Missiles(void)
{
// this function erases all the missiles

int index; // lopping variable

for (index=0; index<NUM_MISSILES; index++)
    {
    // is this missile active and visible

    if (missiles[index].state == MISS_ACTIVE && missiles[index].visible)
       {
       Write_Pixel_DB(missiles[index].sx,missiles[index].sy,missiles[index].back_color);
       } // end if alive

    } // end for index

} // end Erase_Missiles

/////////////////////////////////////////////////////////////////////////////

void Under_Missiles(void)
{
// this function scans the background under the missiles

int index,       // looping varible
    px_window,   // the starting postion of the players window
    py_window,
    mx,my;       // local aliases for speed

// compute starting position of players window so screen mapping can be done

px_window = players_x - 160 + 11;
py_window = players_y - 100 + 9;

for (index=0; index<NUM_MISSILES; index++)
    {
    // is this missile active

    if (missiles[index].state == MISS_ACTIVE)
       {
       // remap to screen coordinates

       mx=missiles[index].sx = (missiles[index].x - px_window);
       my=missiles[index].sy = (missiles[index].y - py_window);

       // test if missile is visible on screen?

       if (mx>=320 || mx <0 || my>=200  || my<0)
          {

          // this missile is invisible and has been clipped

          missiles[index].visible = 0;

          // process next missile

          continue;

          } // end visibility test

       // scan under missile

       missiles[index].back_color =
                      Read_Pixel_DB(mx,my);

       // set visibility flag

       missiles[index].visible = 1;

       } // end if alive

    } // end for index

} // end Under_Missiles

/////////////////////////////////////////////////////////////////////////////

void Draw_Missiles(void)
{
// this function draws all the missiles
int index; // looping variable

for (index=0; index<NUM_MISSILES; index++)
    {
    // is this missile active and visible

    if (missiles[index].state == MISS_ACTIVE && missiles[index].visible)
       {

       Write_Pixel_DB(missiles[index].sx,missiles[index].sy,missiles[index].color);

       } // end if alive

    } // end for index

} // end Draw_Missiles

/////////////////////////////////////////////////////////////////////////////

void Init_Missiles(void)
{
// this function resets and intializes all missiles
int index; // looping variable

for (index=0; index<NUM_MISSILES; index++)
    missiles[index].state = MISS_INACTIVE;

} // Init_Missiles

////////////////////////////////////////////////////////////////////////////

void Move_Missiles(void)
{
// this function move all the missiles and performs collision detection

int index,              // looping variables
    a_index,
    miss_x,miss_y;      // speed aliases

// process each missile

for (index=0; index<NUM_MISSILES; index++)
    {

    // is missile active

    if (missiles[index].state == MISS_ACTIVE)
       {
       // move the missile

       miss_x = (missiles[index].x += missiles[index].xv);
       miss_y = (missiles[index].y += missiles[index].yv);

       // test if a missile has hit an asteroid

       for (a_index=0; a_index<NUM_ASTEROIDS; a_index++)
           {
           // test if asteroid is active

           if (asteroids[a_index].rock.state==ASTEROID_ACTIVE)
              {
              // test for collision

              if (miss_x >= asteroids[a_index].x &&
                  miss_y >= asteroids[a_index].y &&
                  miss_x <= asteroids[a_index].x + asteroids[a_index].rock.width &&
                  miss_y <= asteroids[a_index].y + asteroids[a_index].rock.height)
                  {
                  // kill the asteroid and the missile

                  asteroids[a_index].rock.state = ASTEROID_INACTIVE;

                  missiles[index].state = MISS_INACTIVE;

                  // what kind of asteroid did we have?

                  switch(asteroids[a_index].type)
                        {

                        case ASTEROID_LARGE:
                             {
                             // start an explosion at proper place

                             Start_Explosion(asteroids[a_index].x,
                                             asteroids[a_index].y,2);

                             // start one medium and one small asteroid
                             //  (if possible)

                             Start_Asteroid(asteroids[a_index].x,
                                            asteroids[a_index].y,
                                            ASTEROID_MEDIUM);

                             Start_Asteroid(asteroids[a_index].x+ASTEROID_LARGE_WIDTH/2,
                                            asteroids[a_index].y+ASTEROID_LARGE_HEIGHT/2,
                                            ASTEROID_SMALL);

                             if (missiles[index].type==PLAYER_MISSILE)
                                players_score+=100;
                             else
                                remotes_score+=100;


                             } break;

                        case ASTEROID_MEDIUM:
                             {
                             // start an explosion at proper place

                             Start_Explosion(asteroids[a_index].x-2,
                                             asteroids[a_index].y-2,2);


                             // start two small asteroids (if possible)

                             Start_Asteroid(asteroids[a_index].x,
                                            asteroids[a_index].y,
                                            ASTEROID_SMALL);

                             Start_Asteroid(asteroids[a_index].x+ASTEROID_MEDIUM_WIDTH/2,
                                            asteroids[a_index].y+ASTEROID_MEDIUM_HEIGHT/2,
                                            ASTEROID_SMALL);

                             if (missiles[index].type==PLAYER_MISSILE)
                                players_score+=50;
                             else
                                remotes_score+=50;

                             } break;

                        case ASTEROID_SMALL:
                             {
                             // start an explosion at proper place

                             Start_Explosion(asteroids[a_index].x-8,
                                             asteroids[a_index].y-8,2);

                             // start a randomly positioned asteroid of any size
                             // at the worm hole

                             Start_Asteroid(WORMHOLE_X,WORMHOLE_Y,rand()%3);

                             if (missiles[index].type==PLAYER_MISSILE)
                                players_score+=25;
                             else
                                remotes_score+=25;

                             } break;

                        default:break;

                        } // end switch

                  // break inner loop

                  break;

                  } // end if collision

              } // end if active

           } // end for a_index

       // test if missiles hit local player

       if (linked && players_state==ALIVE && missiles[index].type==REMOTE_MISSILE)
       {
       if (miss_x>players_x && miss_x < players_x+SHIP_WIDTH &&
           miss_y>players_y && miss_y < players_y+SHIP_HEIGHT)
           {

           // de-activate missile

           missiles[index].state = MISS_INACTIVE;

           // were shields up?

           if (players_shields)
              {
              // start explosion
              Start_Explosion(players_x,players_y,2);
              } // end if shields up
           else
              { // say bye bye!

              Start_Explosion(players_x,players_y,2);
              Start_Nova(players_x+SHIP_WIDTH/2,players_y+SHIP_HEIGHT/2);

              // start local player death sequence

              Start_Players_Death();

              } // end if else

           } // end if local player hit

       } // end if remote fired this missile

       // test if missile has hit remote player

       if (linked && remotes_state==ALIVE && missiles[index].type==PLAYER_MISSILE)
          {
          if (miss_x>remotes_x && miss_x < remotes_x+SHIP_WIDTH &&
              miss_y>remotes_y && miss_y < remotes_y+SHIP_HEIGHT)
              {

              // de-activate missile

              missiles[index].state = MISS_INACTIVE;

              // were shields up?

              if (remotes_shields)
                 {
                 // start explosion
                 Start_Explosion(remotes_x,remotes_y,2);
                 } // end if shields up
              else
                 { // say bye bye!

                 Start_Explosion(remotes_x,remotes_y,2);
                 Start_Nova(remotes_x+SHIP_WIDTH/2,remotes_y+SHIP_HEIGHT/2);

                 // start remote death sequence

                 Start_Remotes_Death();

                 } // end if else

              } // end if remote player hit

          } // end if remote is linked

       // test if it's hit the edge of the screen or a wall

       if ( (miss_x >= UNIVERSE_WIDTH+UNIVERSE_BORDER)  ||
            (miss_x < -UNIVERSE_BORDER)                 ||
            (miss_y >  UNIVERSE_HEIGHT+UNIVERSE_BORDER) ||
            (miss_y < -UNIVERSE_BORDER)                 ||
            (--missiles[index].lifetime < 0))
          {
          // de-activate the missile

          missiles[index].state = MISS_INACTIVE;

          } // end if off edge of screen

       // test if this missiles has been terminated in some way, if so update
       // the active missile variable for the player or remote

       if (missiles[index].state == MISS_INACTIVE)
          {
          // update number of active missiles

          if (missiles[index].type == PLAYER_MISSILE)
             players_active_missiles--;
          else
          if (missiles[index].type == REMOTE_MISSILE)
             remotes_active_missiles--;

          } // end if this missiles was terminated on this cycle

       } // end if alive

    } // end for index

} // end Move_Missiles

/////////////////////////////////////////////////////////////////////////////

int Start_Missile(int x,int y,int xv,int yv, int color,int type)
{
// this function start a photon missile with the given position and speed

int index; // looping variable

// scan for an inactive

for (index=0; index<NUM_MISSILES; index++)
    {
    // is this missile free?

    if (missiles[index].state == MISS_INACTIVE)
       {
       // set up fields

       missiles[index].state       = MISS_ACTIVE;
       missiles[index].x           = x;
       missiles[index].y           = y;
       missiles[index].sx          = 0;
       missiles[index].sy          = 0;
       missiles[index].counter     = 0;
       missiles[index].threshold   = 0;
       missiles[index].xv          = xv;
       missiles[index].yv          = yv;
       missiles[index].color       = color;
       missiles[index].back_color  = 0;
       missiles[index].type        = type;
       missiles[index].visible     = 0;
       missiles[index].lifetime    = 30 + rand()%10;

       // make some sound

       Digital_FX_Play(BLZLAS_VOC,2);

       // return success

       return(1);

       } // end if found a good one

    } // end for index

// must not have found one

return(0);

} // end Start_Missile

/////////////////////////////////////////////////////////////////////////////

void Start_Players_Death(void)
{
// this function starts the players death sequence

players_xv                  = 0;
players_yv                  = 0;
players_engine              = 0;
players_flame_count         = 0;
players_gravity             = 0;
players_shields             = 0;
players_shield_time         = 0;
players_cloak               = -1;
players_state               = DYING;
players_death_count         = 48;


debounce_hud                = 0;
debounce_scan               = 0;
debounce_cloak              = 0;
debounce_thrust             = 0;
debounce_fire               = 0;
debounce_shields            = 0;

} // end Start_Players_Death

//////////////////////////////////////////////////////////////////////////////

void Reset_Player(void)
{
// this function resets the player to his starting position

players_last_x              = game_start_x[master];
players_last_y              = game_start_y[master];
players_x                   = game_start_x[master];
players_y                   = game_start_y[master];
players_state               = ALIVE;

} // end Reset_Player

//////////////////////////////////////////////////////////////////////////////

void Reset_Remote(void)
{
// this function resets the remote to his starting position

remotes_last_x              = game_start_x[slave];
remotes_last_y              = game_start_y[slave];
remotes_x                   = game_start_x[slave];
remotes_y                   = game_start_y[slave];
remotes_state               = ALIVE;

} // end Reset_Remote

//////////////////////////////////////////////////////////////////////////////

void Start_Remotes_Death(void)
{
// this function starts the remotes death sequence

remotes_xv                  = 0;
remotes_yv                  = 0;
remotes_engine              = 0;
remotes_flame_count         = 0;
remotes_gravity             = 0;
remotes_shields             = 0;
remotes_shield_time         = 0;
remotes_cloak               = -1;
remotes_state               = DYING;
remotes_death_count         = 48;

} // end Start_Remotes_Death

//////////////////////////////////////////////////////////////////////////////

void Reset_System(void)
{
// this function resets everything so the game can be ran again
// I hope I didn't leave anything out?

// player variables

winner = WINNER_NONE;      // the winner of the game

players_last_x              = game_start_x[master];
players_last_y              = game_start_y[master];
players_x                   = game_start_x[master];
players_y                   = game_start_y[master];
players_dx                  = 0;
players_dy                  = 0;
players_xv                  = 0;
players_yv                  = 0;
players_engine              = 0;
players_stability           = 8;
players_flame_count         = 0;
players_flame_time          = 1;
players_ship.curr_frame     = 0;
players_gravity             = 0;
players_shields             = 0;
players_shield_time         = 0;
players_cloak               = -1;
players_heads               = -1;
players_comm                = -1;
players_scanner             = -1;
players_num_ships           = 3;
players_shield_strength     = 22000;
players_energy              = 22000;
players_score               = 0;
players_active_missiles     = 0;
players_state               = ALIVE;
players_death_count         = 0;

debounce_hud                = 0;
debounce_scan               = 0;
debounce_cloak              = 0;
debounce_thrust             = 0;
debounce_fire               = 0;
debounce_shields            = 0;

refresh_heads = 0;

// remote variables

remotes_last_x          = game_start_x[slave];
remotes_last_y          = game_start_y[slave];
remotes_x               = game_start_x[slave];
remotes_y               = game_start_y[slave];
remotes_dx              = 0;
remotes_dy              = 0;
remotes_xv              = 0;
remotes_yv              = 0;
remotes_engine          = 0;
remotes_stability       = 8;
remotes_flame_count     = 0;
remotes_flame_time      = 1;
remotes_gravity         = 0;
remotes_shields         = 0;
remotes_shield_time     = 0;
remotes_cloak           = -1;
remotes_heads           = -1;
remotes_comm            = -1;
remotes_scanner         = -1;
remotes_num_ships       = 3;
remotes_shield_strength = 22000;
remotes_energy          = 22000;
remotes_score           = 0;
remotes_active_missiles = 0;
remotes_state           = ALIVE;
remotes_death_count    = 0;

} // end Reset_System

///////////////////////////////////////////////////////////////////////////////

void Panel_FX(void)
{
// this function performs all of the special effects for the control panel

int index; // lopping variable

static int panel_counter = 0;  // used to time the color rotation of the panel

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

} // end Panel_FX

////////////////////////////////////////////////////////////////////////////////

void Start_Explosion(int x,int y,int speed)
{
// this function stars a generic explosion

int index;  // looping variable

// scan for a useable explosion

for (index=0; index<NUM_EXPLOSIONS; index++)
    {

    if (explosions[index].state == EXPLOSION_INACTIVE)
       {

       // set up fields

       explosions[index].state       = EXPLOSION_ACTIVE;
       explosions[index].x           = 0; // screen coordinates
       explosions[index].y           = 0;
       explosions[index].counter_2   = x; // the counters will be used as universe
       explosions[index].counter_3   = y; // coordinates


       explosions[index].curr_frame  = 0;
       explosions[index].threshold_1 = speed;
       explosions[index].counter_1   = 0;

       // make some sound

       Digital_FX_Play(BLZEXP1_VOC,1);

       break; // exit loop

       } // end if found a good one

    } // end for index

} // end Start_Explosion

/////////////////////////////////////////////////////////////////////////////

void Under_Explosions(void)
{

// this function scans under the explosions


int index,     // looping index
    px_window, // the starting postion of the players window
    py_window;

// compute starting position of players window so screen mapping can be done

px_window = players_x - 160+11;
py_window = players_y - 100+9;

// scan for a running explosions

for (index=0; index<NUM_EXPLOSIONS; index++)
    {

    if (explosions[index].state == EXPLOSION_ACTIVE)
       {

       // postion explosion correctly on view screen, note this is very similar
       // to what we will do in 3-D when we translate all the objects in the
       // universe to the viewer position, note counter_2 and counter_3
       // in the sprite structure are used as universe or world x,y

       explosions[index].x = (explosions[index].counter_2-px_window);
       explosions[index].y = (explosions[index].counter_3-py_window);

       Sprite_Under_Clip((sprite_ptr)&explosions[index],double_buffer);

       } // end if found a good one

    } // end for index

} // end Under_Explosions

/////////////////////////////////////////////////////////////////////////////

void Erase_Explosions(void)
{
// this function erases all the current explosions

int index;

// scan for a useable explosion

for (index=0; index<NUM_EXPLOSIONS; index++)
    {

    if (explosions[index].state == EXPLOSION_ACTIVE)
       {
       Sprite_Erase_Clip((sprite_ptr)&explosions[index],double_buffer);

       } // end if found a good one

    } // end for index

} // end Erase_Explosions

/////////////////////////////////////////////////////////////////////////////

void Draw_Explosions(void)
{
// this function draws the explosion

int index; // looping variable

// scan for a useable explosion

for (index=0; index<NUM_EXPLOSIONS; index++)
    {

    // make sure this explosion is alive

    if (explosions[index].state == EXPLOSION_ACTIVE)
       {

       Sprite_Draw_Clip((sprite_ptr)&explosions[index],double_buffer,1);

       } // end if found a good one

    } // end for index

} // end Draw_Explosions

/////////////////////////////////////////////////////////////////////////////

void Animate_Explosions(void)
{
// this function aniamtes the explosions

int index;  // looping index

// scan for a useable explosion

for (index=0; index<NUM_EXPLOSIONS; index++)
    {
    // test if explosion is active

    if (explosions[index].state == EXPLOSION_ACTIVE)
       {
       // test if it's time to change frames

       if (++explosions[index].counter_1 >= explosions[index].threshold_1)
          {
          // is the explosion over?

          if (++explosions[index].curr_frame == NUM_EXPLOSION_FRAMES)
             explosions[index].state = EXPLOSION_INACTIVE;

          // reset animation clock for future

          explosions[index].counter_1 = 0;

          } // end if time to change frames

       } // end if found a good one

    } // end for index

} // end Animate_Explosions

//////////////////////////////////////////////////////////////////////////////

void Init_Explosions(void)
{
// clear out the states of all explosions

int index; // looping variable

for (index=0; index<NUM_EXPLOSIONS; index++)
    explosions[index].state = EXPLOSION_INACTIVE;

} // Init_Explosions

/////////////////////////////////////////////////////////////////////////////

void Load_Wormhole(void)
{
// this function loads in the imagery for the worm hole

int index; // looping variable

// load the imagery for the worm hole

PCX_Init((pcx_picture_ptr)&image_pcx);
PCX_Load("blazewrm.pcx", (pcx_picture_ptr)&image_pcx,1);

// initialize the wormhole sprite

Sprite_Init((sprite_ptr)&wormhole,0,0,26,22,0,0,0,0,0,0);

// extract the animation frames

for (index=0; index<NUM_WORMHOLE_FRAMES; index++)
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&wormhole,index,index,0);

PCX_Delete((pcx_picture_ptr)&image_pcx);

} // end Load_Wormhole

/////////////////////////////////////////////////////////////////////////////

void Init_Wormhole(void)
{
// this resets all the worm hole parameters


// set screen coordinates to 0

wormhole.x = 0;
wormhole.y = 0;

// set universe coordinates to proper position

wormhole.counter_2 = WORMHOLE_X; // note the counters are being used for
wormhole.counter_3 = WORMHOLE_Y; // universe positions

// reset the frame counter

wormhole.curr_frame = 0;

// these will be used to time animation

wormhole.counter_1 = 0;
wormhole.threshold_1 = 2;

} // end Init_Wormhole

/////////////////////////////////////////////////////////////////////////////

void Under_Wormhole(void)
{

// this function scans under the wormhole


int px_window, // the starting postion of the players window
    py_window;

// compute starting position of players window so screen mapping can be done

px_window = players_x - 160+11;
py_window = players_y - 100+9;

// translate wormhole to screen coordinates

wormhole.x = (wormhole.counter_2-px_window);
wormhole.y = (wormhole.counter_3-py_window);

Sprite_Under_Clip((sprite_ptr)&wormhole,double_buffer);

} // end Under_Wormhole

/////////////////////////////////////////////////////////////////////////////

void Erase_Wormhole(void)
{
// this function erase the wormhole

Sprite_Erase_Clip((sprite_ptr)&wormhole,double_buffer);

} // end Erase_Wormhole

/////////////////////////////////////////////////////////////////////////////

void Draw_Wormhole(void)
{
// this function draws the wormhole

Sprite_Draw_Clip((sprite_ptr)&wormhole,double_buffer,1);

} // end Draw_Wormhole

/////////////////////////////////////////////////////////////////////////////

void Animate_Wormhole(void)
{
// this function animates the wormhole

if (++wormhole.counter_1 >= wormhole.threshold_1)
   {
   // time to reset frame counter?

   if (++wormhole.curr_frame == NUM_WORMHOLE_FRAMES)
       wormhole.curr_frame = 0;

    // reset animation clock for future

    wormhole.counter_1 = 0;

    } // end if time to change frames

} // end Animate_Wormhole

/////////////////////////////////////////////////////////////////////////////

void Load_Fuel_Cells(void)
{
// this function loads in the imagery for the fuel cells

int index,  // looping variable
    frames; // looping variable

// load the imagery for the fuel cells

PCX_Init((pcx_picture_ptr)&image_pcx);
PCX_Load("blazeful.pcx", (pcx_picture_ptr)&image_pcx,1);

// initialize the fuel cells sprite and load bitmaps

for (index=0; index<NUM_FUEL_CELLS; index++)
    {

    Sprite_Init((sprite_ptr)&fuel_cells[index],0,0,20,18,0,0,0,0,0,0);

    // extract the animation frames

    for (frames=0; frames<NUM_FUEL_FRAMES; frames++)
        PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,
                       (sprite_ptr)&fuel_cells[index],frames,frames,0);

    } // end for index

PCX_Delete((pcx_picture_ptr)&image_pcx);

} // end Load_Fuel_Cells

/////////////////////////////////////////////////////////////////////////////

void Init_Fuel_Cells(void)
{
// this resets all the fuel cell parameters

int index; // looping variable

for (index=0; index<NUM_FUEL_CELLS; index++)
    {

    // set state

    fuel_cells[index].state = FUEL_CELL_ACTIVE;

    // set screen coordinates to 0

    fuel_cells[index].x = 0;
    fuel_cells[index].y = 0;

    // set universe coordinates to random position

    fuel_cells[index].counter_2 = 200+rand()%(UNIVERSE_WIDTH-400);
    fuel_cells[index].counter_3 = 200+rand()%(UNIVERSE_HEIGHT-400);

    // reset the frame counter

    fuel_cells[index].curr_frame = 0;

    // these will be used to time animation

    fuel_cells[index].counter_1 = 0;
    fuel_cells[index].threshold_1 = 2;

    } // end index

} // end Init_Fuel_Cells

/////////////////////////////////////////////////////////////////////////////

void Under_Fuel_Cells(void)
{
// this function scans under the Fuel cells


int px_window, // the starting postion of the players window
    py_window,
    index;     // looping variable

// compute starting position of players window so screen mapping can be done

px_window = players_x - 160+11;
py_window = players_y - 100+9;

// process each fuel cell

for (index=0; index<NUM_FUEL_CELLS; index++)
    {
    // test if fuel cell is active

    if (fuel_cells[index].state == FUEL_CELL_ACTIVE)
       {
       // translate Fuelcells to screen coordinates

       fuel_cells[index].x = (fuel_cells[index].counter_2-px_window);
       fuel_cells[index].y = (fuel_cells[index].counter_3-py_window);

       Sprite_Under_Clip((sprite_ptr)&fuel_cells[index],double_buffer);

       } // end fi active

    } // end for index

} // end Under_Fuel_Cells

/////////////////////////////////////////////////////////////////////////////

void Erase_Fuel_Cells(void)
{
// this function erase the Fuelcells

int index; // looping variable

for (index=0; index<NUM_FUEL_CELLS; index++)
    {
    // test if fuel cell is active

    if (fuel_cells[index].state == FUEL_CELL_ACTIVE)
       {

       Sprite_Erase_Clip((sprite_ptr)&fuel_cells[index],double_buffer);

       } // end if active

    } // end for index

} // end Erase_Fuel_Cells

/////////////////////////////////////////////////////////////////////////////

void Draw_Fuel_Cells(void)
{
// this function draws the Fuel cells

int index; // looping variable

for (index=0; index<NUM_FUEL_CELLS; index++)
    {
    // test if fuel cell is active

    if (fuel_cells[index].state == FUEL_CELL_ACTIVE)
       {

       Sprite_Draw_Clip((sprite_ptr)&fuel_cells[index],double_buffer,1);

       } // end if active

    } // end for index

} // end Draw_Fuel_Cells

/////////////////////////////////////////////////////////////////////////////

void Animate_Fuel_Cells(void)
{
// this function animates the fuel cells and test for collision

int index; // looping variable

for (index=0; index<NUM_FUEL_CELLS; index++)
    {
    // test if fuel cell is active

    if (fuel_cells[index].state == FUEL_CELL_ACTIVE)
       {
       // move cell to right

       if (++fuel_cells[index].counter_2 > (UNIVERSE_WIDTH+UNIVERSE_BORDER))
          fuel_cells[index].counter_2 = -UNIVERSE_BORDER;

       // perform animation

       if (++fuel_cells[index].counter_1 >= fuel_cells[index].threshold_1)
          {
          // time to reset frame counter?

          if (++fuel_cells[index].curr_frame == NUM_FUEL_FRAMES)
              fuel_cells[index].curr_frame = 0;

           // reset animation clock for future

           fuel_cells[index].counter_1 = 0;

          } // end if time to change frames

       } // end if active

    } // end for index

} // end Animate_Fuel_Cells

//////////////////////////////////////////////////////////////////////////////

void Load_Alien(void)
{
// this function loads in the imagery for the alien

int index; // looping variable

// load the imagery for the alien

PCX_Init((pcx_picture_ptr)&image_pcx);
PCX_Load("blazealn.pcx", (pcx_picture_ptr)&image_pcx,1);

// initialize the alien sprite

Sprite_Init((sprite_ptr)&alien.body,0,0,14,8,0,0,0,0,0,0);

// extract the animation frames

for (index=0; index<NUM_ALIEN_FRAMES; index++)
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&alien.body,index,index,0);


PCX_Delete((pcx_picture_ptr)&image_pcx);

} // end Load_Alien

/////////////////////////////////////////////////////////////////////////////

void Init_Alien(void)
{
// this resets all the alien parameters

// set screen coordinates to 0

alien.body.x = 0;
alien.body.y = 0;

// set universe coordinates to proper position

alien.x = 0;
alien.y = 0;

// reset the frame counter

alien.body.curr_frame = 0;

// these will be used to time animation

alien.body.counter_1 = 0;
alien.body.threshold_1 = 2;

// set state to active

alien.state = ALIEN_INACTIVE;

} // end Init_Alien

//////////////////////////////////////////////////////////////////////////////

void Alien_Control(void)
{
// this function will be called every cycle and decide if an alien
// will start up

// make sure alien is dead

if (alien.state==ALIEN_INACTIVE)
   {
   // throw a coin

   if ((rand()%ALIEN_ODDS)==1)
      {
      // position alien and set appropriate fields

      alien.body.x = 0; // screen coords
      alien.body.y = 0;

      // start alien from wormhole (universe coords)

      alien.x = WORMHOLE_X; // note the counters are being used for
      alien.y = WORMHOLE_Y; // universe positions

      // reset the frame counter

      alien.body.curr_frame = 0;

      // this will be used to time animation

      alien.body.counter_1   = 0;
      alien.body.threshold_1 = 2;

      // set state to random to start with

      alien.state = ALIEN_RANDOM;

      // select a random direction

      alien.xv = -4 + rand()%8;
      alien.yv = -4 + rand()%8;

      // counter 2 will be used to track how long to stay in a state

      alien.body.counter_2 = 50;

      } // end if start one

   } // end if dead

} // end Alien_Control

/////////////////////////////////////////////////////////////////////////////

void Under_Alien(void)
{
// this function scans under the alien

int px_window, // the starting postion of the players window
    py_window;

if (alien.state!=ALIEN_INACTIVE)
   {
   // compute starting position of players window so screen
   // mapping can be done

   px_window = players_x - 160+11;
   py_window = players_y - 100+9;

   // translate alien to screen coordinates

   alien.body.x = (alien.x-px_window);
   alien.body.y = (alien.y-py_window);

   // perform scan in screen coords

   Sprite_Under_Clip((sprite_ptr)&alien.body,double_buffer);

   } // end if alien alive

} // end Under_Alien

/////////////////////////////////////////////////////////////////////////////

void Erase_Alien(void)
{
// this function erases the alien

if (alien.state!=ALIEN_INACTIVE)
    Sprite_Erase_Clip((sprite_ptr)&alien.body,double_buffer);

} // end Erase_Alien

/////////////////////////////////////////////////////////////////////////////

void Draw_Alien(void)
{
// this function draws the alien

if (alien.state!=ALIEN_INACTIVE)
   Sprite_Draw_Clip((sprite_ptr)&alien.body,double_buffer,1);

} // end Draw_Alien

/////////////////////////////////////////////////////////////////////////////

void Move_Alien(void)
{
// this function moves the alien (if there is one)

if (alien.state!=ALIEN_INACTIVE)
   {

   // what state is alien in?

   switch(alien.state)
         {

         case ALIEN_RANDOM:
              {

              // move alien in direction

              alien.x+=alien.xv;
              alien.y+=alien.yv;

              } break;

         case ALIEN_CHASE_PLAYER:
              {


              } break;

         case ALIEN_CHASE_REMOTE:
              {


              } break;

          default:break;

         } // end switch

   // decrement state counter

   if (--alien.body.counter_2<=0)
      {
      // select a new state

      alien.state = ALIEN_RANDOM;   // change this line later

      // set up new state

      switch(alien.state)
            {
            case ALIEN_RANDOM:
                 {
                 // select new random direction and state time

                 alien.xv = -4 + rand()%8;
                 alien.yv = -4 + rand()%8;

                 // counter 2 will be used to track how long to stay in a state

                 alien.body.counter_2 = 25+rand()%75;

                 } break;

            case ALIEN_CHASE_PLAYER:
                 {
                 // this bud's for you!
                 } break;

            case ALIEN_CHASE_REMOTE:
                 {
                 // this is an exercise
                 } break;

            default:break;

            } // end switch

      } // end if time to select a new state

   // animate alien

   if (++alien.body.counter_1 >=alien.body.threshold_1)
      {
      // change animation frames

      if (++alien.body.curr_frame==NUM_ALIEN_FRAMES)
         alien.body.curr_frame = 0;

      // reset animation counter

      alien.body.counter_1 = 0;

      } // end if time to animate

   // do collision detection

   if ((alien.x>UNIVERSE_WIDTH+UNIVERSE_BORDER)  ||
       (alien.x<-UNIVERSE_BORDER)                ||
       (alien.y>UNIVERSE_HEIGHT+UNIVERSE_BORDER) ||
       (alien.y<-UNIVERSE_BORDER))
      {
      // kill alien

      alien.state = ALIEN_INACTIVE;

      } // end if alien has warped off universe edge

   } // end if alien alive

} // end Move_Alien

///////////////////////////////////////////////////////////////////////////////

void Load_Heads(void)
{
// this function loads various icons for the heads up display

int index; // looping variable

// load the imagery for the icons for display

PCX_Init((pcx_picture_ptr)&image_pcx);
PCX_Load("blazehu1.pcx", (pcx_picture_ptr)&image_pcx,1);

// intialize the button sprite

Sprite_Init((sprite_ptr)&heads_text,0,0,34,6,0,0,0,0,0,0);

// extract the bitmaps for heads up text

for (index=0; index<7; index++)
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,
                   (sprite_ptr)&heads_text,index,index,0);

// delete pcx file

PCX_Delete((pcx_picture_ptr)&image_pcx);

// load the imagery for the icons for display

PCX_Init((pcx_picture_ptr)&image_pcx);
PCX_Load("blazehu2.pcx", (pcx_picture_ptr)&image_pcx,1);

// intialize the button sprite

Sprite_Init((sprite_ptr)&heads_numbers,0,0,8,6,0,0,0,0,0,0);

// extract the bitmaps for heads up text

for (index=0; index<7; index++)
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,
                   (sprite_ptr)&heads_numbers,index,index,0);

// delete pcx file

PCX_Delete((pcx_picture_ptr)&image_pcx);

// load the imagery for the icons for display

PCX_Init((pcx_picture_ptr)&image_pcx);
PCX_Load("blazehu1.pcx", (pcx_picture_ptr)&image_pcx,1);

// intialize the button sprite

Sprite_Init((sprite_ptr)&heads_gauge,0,0,34,6,0,0,0,0,0,0);

// extract the bitmaps for heads up gauges

for (index=0; index<23; index++)
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,
                   (sprite_ptr)&heads_gauge,index,index%9,2+index/9);

// delete pcx file

PCX_Delete((pcx_picture_ptr)&image_pcx);

} // end Load_Heads

////////////////////////////////////////////////////////////////////////////

void Init_Heads(void)
{
// do nothing for now


} // end Init_Heads

////////////////////////////////////////////////////////////////////////////

void Draw_Heads(void)
{
// this function draws the heads up display during the tmie when
// the game background can be modified

int index; // looping variable

heads_text.x = LEFT_HEADS_TEXT_X;
heads_text.y = LEFT_HEADS_TEXT_Y;

// draw the left most messages first

heads_text.curr_frame = HEADS_CLOAK;

for (index=0; index<4; index++)
    {
    Sprite_Draw((sprite_ptr)&heads_text,double_buffer,0);

    // move down a row

    heads_text.y+=8;

    // select next message

    heads_text.curr_frame++;

    } //  end for index

// now draw buttons and numbers

heads_numbers.x = LEFT_HEADS_TEXT_X+40;
heads_numbers.y = LEFT_HEADS_TEXT_Y;

// draw cloaked button

if (players_cloak==1)
   {
   heads_numbers.curr_frame=2;
   Sprite_Draw((sprite_ptr)&heads_numbers,double_buffer,0);
   }
else
   {
   heads_numbers.curr_frame=1;
   Sprite_Draw((sprite_ptr)&heads_numbers,double_buffer,0);
   } // end else

// draw scanner enable

heads_numbers.x = LEFT_HEADS_TEXT_X+40;
heads_numbers.y = LEFT_HEADS_TEXT_Y+8;

if (players_scanner==1)
   {
   heads_numbers.curr_frame=2;
   Sprite_Draw((sprite_ptr)&heads_numbers,double_buffer,0);
   }
else
   {
   heads_numbers.curr_frame=1;
   Sprite_Draw((sprite_ptr)&heads_numbers,double_buffer,0);
   } // end else

// draw communications

heads_numbers.x = LEFT_HEADS_TEXT_X+40;
heads_numbers.y = LEFT_HEADS_TEXT_Y+16;

if (linked)
   {
   heads_numbers.curr_frame=2;
   Sprite_Draw((sprite_ptr)&heads_numbers,double_buffer,0);
   }
else
   {
   heads_numbers.curr_frame=1;
   Sprite_Draw((sprite_ptr)&heads_numbers,double_buffer,0);
   } // end else

// draw number of ships

heads_numbers.x = LEFT_HEADS_TEXT_X+40;
heads_numbers.y = LEFT_HEADS_TEXT_Y+24;

heads_numbers.curr_frame=3+players_num_ships;
Sprite_Draw((sprite_ptr)&heads_numbers,double_buffer,0);

// now draw right most messages

heads_text.x = RIGHT_HEADS_TEXT_X;
heads_text.y = RIGHT_HEADS_TEXT_Y;

heads_text.curr_frame = HEADS_ENERGY;

for (index=0; index<2; index++)
    {
    Sprite_Draw((sprite_ptr)&heads_text,double_buffer,0);

    // move down a row

    heads_text.y+=8;

    // select next message

    heads_text.curr_frame++;

    } //  end for index

// draw gauges

heads_gauge.x = RIGHT_HEADS_TEXT_X+40;
heads_gauge.y = RIGHT_HEADS_TEXT_Y;

// compute proper frame

heads_gauge.curr_frame = 22 - (players_energy/1000);

// draw the energy level

Sprite_Draw((sprite_ptr)&heads_gauge,double_buffer,0);

heads_gauge.x = RIGHT_HEADS_TEXT_X+40;
heads_gauge.y = RIGHT_HEADS_TEXT_Y+8;

// compute proper frame

heads_gauge.curr_frame = 22 - (players_shield_strength/1000);

// draw the shield strength

Sprite_Draw((sprite_ptr)&heads_gauge,double_buffer,0);

} // end Draw_Heads

////////////////////////////////////////////////////////////////////////////

void Erase_Heads(void)
{

// this function erases the heads up display during the time when
// the game background can be modified

int index; // looping variable

heads_text.x = LEFT_HEADS_TEXT_X;
heads_text.y = LEFT_HEADS_TEXT_Y;

// draw the left most messages first

heads_text.curr_frame = 0;

for (index=0; index<4; index++)
    {
    Sprite_Draw((sprite_ptr)&heads_text,double_buffer,0);

    // move down a row

    heads_text.y+=8;

    } //  end for index


// now erase buttons and numbers

heads_numbers.x = LEFT_HEADS_TEXT_X+40;
heads_numbers.y = LEFT_HEADS_TEXT_Y;

// erase cloaked button

heads_numbers.curr_frame=0;
Sprite_Draw((sprite_ptr)&heads_numbers,double_buffer,0);

// erase scanner

heads_numbers.x = LEFT_HEADS_TEXT_X+40;
heads_numbers.y = LEFT_HEADS_TEXT_Y+8;

Sprite_Draw((sprite_ptr)&heads_numbers,double_buffer,0);

// erase communications

heads_numbers.x = LEFT_HEADS_TEXT_X+40;
heads_numbers.y = LEFT_HEADS_TEXT_Y+16;

Sprite_Draw((sprite_ptr)&heads_numbers,double_buffer,0);

// erase ships number

heads_numbers.x = LEFT_HEADS_TEXT_X+40;
heads_numbers.y = LEFT_HEADS_TEXT_Y+24;

Sprite_Draw((sprite_ptr)&heads_numbers,double_buffer,0);

// now draw right most messages

heads_text.x = RIGHT_HEADS_TEXT_X;
heads_text.y = RIGHT_HEADS_TEXT_Y;

// draw the left most messages first

for (index=0; index<2; index++)
    {
    Sprite_Draw((sprite_ptr)&heads_text,double_buffer,0);

    // move down a row

    heads_text.y+=8;

    } //  end for index

// erase gauges

heads_gauge.x = RIGHT_HEADS_TEXT_X+40;
heads_gauge.y = RIGHT_HEADS_TEXT_Y;

// compute proper frame

heads_gauge.curr_frame = 22;

// draw the energy level

Sprite_Draw((sprite_ptr)&heads_gauge,double_buffer,0);

// draw gauges

heads_gauge.x = RIGHT_HEADS_TEXT_X+40;
heads_gauge.y = RIGHT_HEADS_TEXT_Y+8;

// erase the shield strength

Sprite_Draw((sprite_ptr)&heads_gauge,double_buffer,0);

} // end Erase_Heads

/////////////////////////////////////////////////////////////////////////////

void Init_Scanner(void)
{
// this function initializes the scanner

// not much to do at this point!

} // end Init_Scanner

/////////////////////////////////////////////////////////////////////////////

void Erase_Scanner(void)
{
// this function erases the scanner and the blips

// first erase scanner grid

Line_H_2(SCANNER_X,SCANNER_X+64,SCANNER_Y,   0,double_buffer);
Line_H_2(SCANNER_X,SCANNER_X+64,SCANNER_Y+16,0,double_buffer);
Line_H_2(SCANNER_X,SCANNER_X+64,SCANNER_Y+32,0,double_buffer);

Line_V_2(SCANNER_Y,SCANNER_Y+32,SCANNER_X,   0,double_buffer);
Line_V_2(SCANNER_Y,SCANNER_Y+32,SCANNER_X+16,0,double_buffer);
Line_V_2(SCANNER_Y,SCANNER_Y+32,SCANNER_X+32,0,double_buffer);
Line_V_2(SCANNER_Y,SCANNER_Y+32,SCANNER_X+48,0,double_buffer);
Line_V_2(SCANNER_Y,SCANNER_Y+32,SCANNER_X+64,0,double_buffer);

} // end Erase_Scanner

////////////////////////////////////////////////////////////////////////////

void Draw_Scanner(void)
{
// this function draws the scanner and the blips

// first draw scanner grid

Line_H_2(SCANNER_X,SCANNER_X+64,SCANNER_Y,   10,double_buffer);
Line_H_2(SCANNER_X,SCANNER_X+64,SCANNER_Y+16,10,double_buffer);
Line_H_2(SCANNER_X,SCANNER_X+64,SCANNER_Y+32,10,double_buffer);

Line_V_2(SCANNER_Y,SCANNER_Y+32,SCANNER_X,   10,double_buffer);
Line_V_2(SCANNER_Y,SCANNER_Y+32,SCANNER_X+16,10,double_buffer);
Line_V_2(SCANNER_Y,SCANNER_Y+32,SCANNER_X+32,10,double_buffer);
Line_V_2(SCANNER_Y,SCANNER_Y+32,SCANNER_X+48,10,double_buffer);
Line_V_2(SCANNER_Y,SCANNER_Y+32,SCANNER_X+64,10,double_buffer);

} // end Draw_Scanner

/////////////////////////////////////////////////////////////////////////////

void Under_Blips(void)
{
// this function is used to draw all the scanner blips

// draw blips, notice that the position of the remote and player is
// scaled to fit into the scanner window

under_players_blip = Read_Pixel_DB(SCANNER_X+players_x/40,SCANNER_Y+players_y/80);
under_remotes_blip = Read_Pixel_DB(SCANNER_X+remotes_x/40,SCANNER_Y+remotes_y/80);

} // end Under_Blips

/////////////////////////////////////////////////////////////////////////////

void Draw_Blips(void)
{
// this function is used to draw all the scanner blips

// draw blips, notice that the position of the remote and player is
// scaled to fit into the scanner window

if (players_cloak==-1)
   Write_Pixel_DB(SCANNER_X+players_x/40,SCANNER_Y+players_y/80,9);

if (remotes_cloak==-1)
   Write_Pixel_DB(SCANNER_X+remotes_x/40,SCANNER_Y+remotes_y/80,12);

} // end Draw_Blips

/////////////////////////////////////////////////////////////////////////////

void Erase_Blips(void)
{
// this function is used to erase all the scanner blips

// erase blips, notice that the position of the remote and player is
// scaled to fit into the scanner window

Write_Pixel_DB(SCANNER_X+players_x/40,SCANNER_Y+players_y/80,under_players_blip);
Write_Pixel_DB(SCANNER_X+remotes_x/40,SCANNER_Y+remotes_y/80,under_remotes_blip);

} // end Erase_Blips

////////////////////////////////////////////////////////////////////////////

void Music_Init(void)
{
// this function loads the music and resets all the indexes

static int loaded=0;

// has the music already been loaded

if (!music_enabled)
   return;

if (!loaded)
   {
   Music_Load("blazemus.xmi",(music_ptr)&song);
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

   Sound_Load("BLZCLK.VOC"  ,(_sound_ptr)&digital_FX[BLZCLK_VOC  ],1);
   Sound_Load("BLZEXP1.VOC" ,(_sound_ptr)&digital_FX[BLZEXP1_VOC ],1);
   Sound_Load("BLZEXP2.VOC" ,(_sound_ptr)&digital_FX[BLZEXP2_VOC ],1);
   Sound_Load("BLZLAS.VOC"  ,(_sound_ptr)&digital_FX[BLZLAS_VOC  ],1);
   Sound_Load("BLZNRG.VOC"  ,(_sound_ptr)&digital_FX[BLZNRG_VOC  ],1);
   Sound_Load("BLZSHLD.VOC" ,(_sound_ptr)&digital_FX[BLZSHLD_VOC ],1);
   Sound_Load("BLZTAC.VOC"  ,(_sound_ptr)&digital_FX[BLZTAC_VOC  ],1);
   Sound_Load("BLZSCN.VOC"  ,(_sound_ptr)&digital_FX[BLZSCN_VOC  ],1);
   Sound_Load("BLZMISS.VOC" ,(_sound_ptr)&digital_FX[BLZMISS_VOC ],1);

   Sound_Load("BLZBIOS.VOC" ,(_sound_ptr)&digital_FX[BLZBIOS_VOC ],1);
   Sound_Load("BLZENTR.VOC" ,(_sound_ptr)&digital_FX[BLZENTR_VOC ],1);
   Sound_Load("BLZABRT.VOC" ,(_sound_ptr)&digital_FX[BLZABRT_VOC ],1);
   Sound_Load("BLZSEL.VOC"  ,(_sound_ptr)&digital_FX[BLZSEL_VOC  ],1);
   Sound_Load("BLZKEY.VOC"  ,(_sound_ptr)&digital_FX[BLZKEY_VOC  ],1);
   Sound_Load("BLZDIAL.VOC"  ,(_sound_ptr)&digital_FX[BLZDIAL_VOC  ],1);

   Sound_Load("BLZLOS.VOC"  ,(_sound_ptr)&digital_FX[BLZLOS_VOC  ],1);
   Sound_Load("BLZWIN.VOC"  ,(_sound_ptr)&digital_FX[BLZWIN_VOC  ],1);

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

Sound_Unload((_sound_ptr)&digital_FX[BLZCLK_VOC  ]);
Sound_Unload((_sound_ptr)&digital_FX[BLZEXP1_VOC ]);
Sound_Unload((_sound_ptr)&digital_FX[BLZEXP2_VOC ]);
Sound_Unload((_sound_ptr)&digital_FX[BLZLAS_VOC  ]);
Sound_Unload((_sound_ptr)&digital_FX[BLZNRG_VOC  ]);
Sound_Unload((_sound_ptr)&digital_FX[BLZSHLD_VOC ]);
Sound_Unload((_sound_ptr)&digital_FX[BLZTAC_VOC  ]);
Sound_Unload((_sound_ptr)&digital_FX[BLZSCN_VOC  ]);
Sound_Unload((_sound_ptr)&digital_FX[BLZMISS_VOC ]);

Sound_Unload((_sound_ptr)&digital_FX[BLZENTR_VOC ]);
Sound_Unload((_sound_ptr)&digital_FX[BLZABRT_VOC ]);
Sound_Unload((_sound_ptr)&digital_FX[BLZSEL_VOC  ]);
Sound_Unload((_sound_ptr)&digital_FX[BLZKEY_VOC  ]);
Sound_Unload((_sound_ptr)&digital_FX[BLZDIAL_VOC ]);

Sound_Unload((_sound_ptr)&digital_FX[BLZLOS_VOC  ]);
Sound_Unload((_sound_ptr)&digital_FX[BLZWIN_VOC ]);


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

   Sound_Play((_sound_ptr)&digital_FX[the_effect]);

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

/////////////////////////////////////////////////////////////////////////////

int Get_Modem_String(char *buffer)
{
// this function open up the modem intialization file named
// blaze.mod, if the file exists then

FILE *fp;

// try and open the file

if ((fp=fopen("blaze.mod","r"))==NULL)
   {
   strcpy(buffer,"");
   return(0);
   }

// else load in modem initialization string

fscanf(fp,"%s",buffer);

// close the file

fclose(fp);

return(1);

} // end Get_Modem_String

// M A I N //////////////////////////////////////////////////////////////////

void main(int argc, char **argv)
{

// the main controls most of the platyer and remote logic, normally we would
// probably move most of the code into functions, but for instructional purposes
// this is easier to follow, believe me there are already enough function calls
// to make your head spin!

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

// the game begins by loading in all the imagery, sounds and so forth.

// parse the command line and set up configuration

Parse_Commands(argc,argv);

// get the modem initialization string

Get_Modem_String(modem_ini_string);

printf("\nExtra modem initialization string = %s",modem_ini_string);

Time_Delay(25);

// set the graphics mode to mode 13h

Set_Graphics_Mode(GRAPHICS_MODE13);

// start up music system

if (music_enabled)
   {
   Music_Init();
   Music_Play((music_ptr)&song,0);
   } // end if music enabled

// start up digital FX system

if (digital_enabled)
   {
   Digital_FX_Init();
   } // end if digital sound enabled

// put up Waite header

Intro_Waite();

// seed the random number generator with time

srand((unsigned int)Timer_Query());

// initialize font engine

Font_Engine_1(0,0,0,0,NULL,NULL);
Tech_Print(START_MESS_X,START_MESS_Y,"STARBLAZER 1.0 STARTING UP...",video_buffer);

Time_Delay(5);

// create the double buffer

Create_Double_Buffer(200);
Tech_Print(START_MESS_X,START_MESS_Y+16,"DOUBLE BUFFER CREATED",video_buffer);
Tech_Print(START_MESS_X,START_MESS_Y+24,"LANGUAGE TRANSLATION ENGAGED",video_buffer);

// install the keyboard driver

Keyboard_Install_Driver();
Tech_Print(START_MESS_X,START_MESS_Y+32,"NEURAL INTERFACE ACTIVATED",video_buffer);

// load in all gadget icons

Load_Icons();
Tech_Print(START_MESS_X,START_MESS_Y+40,"VISUAL ICONS LOADED",video_buffer);

// load in the ships

Load_Ships();
Tech_Print(START_MESS_X,START_MESS_Y+48,"SHIPS LOADED",video_buffer);

// assign ships to player and remote

Copy_Frames((sprite_ptr)&players_ship,(sprite_ptr)&gryfon_l);
Copy_Frames((sprite_ptr)&remotes_ship,(sprite_ptr)&raptor_r);

// start the asteroids up

Init_Asteroids(16,6,4);
Tech_Print(START_MESS_X,START_MESS_Y+56,"ASTEROID TRAJECTORIES COMPUTED",video_buffer);

// start the stars

Init_Stars();
Tech_Print(START_MESS_X,START_MESS_Y+64,"STARFIELD GENERATED",video_buffer);

// start the missiles

Init_Missiles();
Tech_Print(START_MESS_X,START_MESS_Y+72,"WEAPONS DAEMONS ONLINE",video_buffer);

// start the explosions

Init_Explosions();
Load_Explosions();
Init_Novas();
Tech_Print(START_MESS_X,START_MESS_Y+80,"EXPLOSION ANIMATION SYSTEM LOADED",video_buffer);

// initialize font engine

Font_Engine_1(0,0,0,0,NULL,NULL);
Tech_Print(START_MESS_X,START_MESS_Y+88,"FONT ENGINE ENGAGED",video_buffer);

// load the wormhole imagery

Load_Wormhole();
Init_Wormhole();
Tech_Print(START_MESS_X,START_MESS_Y+96,"WORM HOLE CREATED",video_buffer);

// load the aliens

Load_Alien();
Init_Alien();
Tech_Print(START_MESS_X,START_MESS_Y+104,"ENEMY AI GENERATED",video_buffer);

// the the fuel cells

Load_Fuel_Cells();
Init_Fuel_Cells();
Tech_Print(START_MESS_X,START_MESS_Y+112,"FUEL CELLS PLACED",video_buffer);

// load the heads up display

Load_Heads();
Init_Heads();
Tech_Print(START_MESS_X,START_MESS_Y+120,"HEADS UP DISPLAY ACTIVE",video_buffer);

// blink ok

Tech_Print(START_MESS_X,START_MESS_Y+136,"SYSTEM BIOS O.K.",video_buffer);

Digital_FX_Play(BLZBIOS_VOC,1);

for (index=0; index<3; index++)
    {
    // draw the message and the erase the message
    Font_Engine_1(START_MESS_X,START_MESS_Y+136,0,0,"SYSTEM BIOS O.K.",video_buffer);
    Time_Delay(8);

    Font_Engine_1(START_MESS_X,START_MESS_Y+136,0,0,"                ",video_buffer);
    Time_Delay(8);

    } // end for

// get rid off this sound to save a little memory (we are running low!)

Sound_Unload((_sound_ptr)&digital_FX[BLZBIOS_VOC]);

// do intro piece

Intro_Title();

// save the system pallete here because we are going to really thrash it!!!

Read_Palette(0,255,(RGB_palette_ptr)&game_palette);

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