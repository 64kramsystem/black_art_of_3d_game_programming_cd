
// BLAZEM0.H

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

extern pcx_picture image_pcx,  // general PCX image used to load background and imagery
            image_controls;  // this holds the controls screen


extern RGB_color color_1,color_2; // used for temporaries during color rotation

extern RGB_palette game_palette;  // this will hold the startup system palette
                           // so we can restore it after screen FX

extern sprite button_1,        // the setup buttons
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


extern monster alien;                     // the alien saucer that comes out of the
                                   // wormhole

extern sprite explosions[NUM_EXPLOSIONS]; // the explosions in the game

extern nova novas[NUM_NOVAS];             // the super nova explosions!

extern int active_explosions;           // number of active explosions

extern asteroid asteroids[NUM_ASTEROIDS]; // the asteroid fields

extern star stars[NUM_STARS];             // the star fields

extern particle missiles[NUM_MISSILES];   // all the projectiles

extern bitmap tech_font[NUM_TECH_FONT];   // the tech font bitmaps

extern int game_state;  // the overall state of the game

extern int comm_port,    // currently selected comm port
    players_ship_type,    // currently selected local ship type
    remotes_ship_type;    // the remotes ships type


extern int comm_port_to_address[2]; // converts the selected com port
                                            // to an address

extern int master,    // the the player dials up a player
    slave,    // then he is master else he is slave
    linked,    // state of the modem communications system
    winner;     // the winner of the game

// the start up arrays used to differentiate the player and remote

// master is index 1, slave is index 0

extern int game_start_x[];
extern int game_start_y[];

// the variables for the player

extern int players_last_x,    // the last postiion of player
    players_last_y,
    players_x,             // current players position
    players_y,
    players_dx,               // players position deltas since last frame
    players_dy,
    players_xv,               // velocity of ship
    players_yv,
    players_engine,       // state of engines
    players_stability,          // how long it takes for gravity
    players_flame_count,        // used for engine flicker
    players_flame_time,
    players_gravity,           // current gravitron count
    players_shields,           // state of the shields
    players_shield_time,     // how long sheilds have been on
    players_cloak,           // state of the shields  -1 off 1 on
    players_heads,            // state of heads up display
    players_comm,             // state of comm link
    players_scanner,           // space scanner
    players_num_ships,        // number of players ship
    players_shield_strength, // the amount of energy players shields have
	 players_energy,          // the amount of ship energy
    players_score,           // the score of the player
    players_active_missiles, // the number of missiles the player has fired
    players_state,           // satte of player
	 players_death_count;     // how long death sequence will last


extern int debounce_hud,       // these are used to debounce the players
	 debounce_scan,         // inputs one some keys
	 debounce_cloak,
	 debounce_thrust,
	 debounce_fire,
	 debounce_shields;

extern int refresh_heads,              // used to track when hud needs refreshing
	 under_players_blip,             // these hold the pixels under the scanner
    under_remotes_blip;             // blip image of the player and remote

// remote variables

extern int remotes_last_x,    // the last postiion of player
    remotes_last_y      ,
    remotes_x           ,    // current remotes position
    remotes_y           ,
	 remotes_dx          ,       // remotes position deltas since last frame
    remotes_dy          ,
    remotes_xv          ,       // velocity of ship
    remotes_yv          ,
    remotes_engine      ,       // state of engines
    remotes_stability   ,       // how long it takes for gravity
    remotes_flame_count ,       // used for engine flicker
    remotes_flame_time  ,
    remotes_gravity     ,       // current gravitron count
    remotes_shields     ,       // state of the shields
    remotes_shield_time ,       // how long sheilds have been on
    remotes_cloak       ,      // state of the shields  -1 off 1 on
	 remotes_heads       ,      // state of heads up display
    remotes_comm        ,      // comm link
    remotes_scanner     ,      // space scanner
    remotes_num_ships   ,       // number of remotes ship
    remotes_shield_strength, // the amount of energy remotes shields have
    remotes_energy         , // the amount of ship energy
    remotes_score          ,     // the remotes score
    remotes_active_missiles,     // the number of missiles the remote has fired
    remotes_state          , // state of remote
    remotes_death_count    ;     // how long death sequence will last

// unit motion look up tables

extern int motion_dx[16];
extern int motion_dy[16];

// musical sequence information

extern int music_enabled,      // flags that enable music and sound FX
    digital_enabled;

extern int digital_FX_priority; // the priority tracker of the current effect

extern int intro_sequence[];
extern int intro_seq_index;

extern int game_sequence[];

extern int game_seq_index;

extern music song;    // the music structure

// sound fx stuff

extern _sound digital_FX[NUM_SOUND_FX];

// these colors are used for the shields on the ships

extern RGB_color  primary_red,    // pure red
           primary_blue,     // pure blue
           primary_green,    // pure green
           primary_black,     // pure black
           primary_white, // pure white

           remotes_shield_color, // the current color of the remotes shield
           players_shield_color, // the current color of the players shields

           players_engine_color, // the color or the players engine
								  remotes_engine_color; // the color or the remotes engine

// the instruction pages

extern char *instructions[];

