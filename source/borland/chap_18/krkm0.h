
// KRKM0.H - Header file for modular kill or be killed

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

extern object static_obj[NUM_STATIONARY];      // there are six basic stationary object types

extern fixed_obj obstacles_1[NUM_OBSTACLES_1]; // general obstacles, rocks etc.
extern fixed_obj obstacles_2[NUM_OBSTACLES_2]; // general obstacles, rocks etc.

extern fixed_obj barriers[NUM_BARRIERS];       // boundary universe boundaries
extern fixed_obj towers[NUM_TOWERS];           // the control towers
extern fixed_obj stations[NUM_STATIONS];       // the power stations
extern fixed_obj telepods[NUM_TELEPODS];       // the teleporters


extern object dynamic_obj[NUM_DYNAMIC];        // this array holds the models
                                        // for the dynamic game objects

extern alien aliens[NUM_ALIENS];               // take a wild guess!

extern missile_obj missiles[NUM_MISSILES];     // holds the missiles

extern int active_player_missiles;         // how many missiles has player activated

extern int total_active_missiles;          // total active missiles

extern layer mountains;              // the background mountains

extern pcx_picture image_pcx,        // general PCX image used to load background and imagery
            image_controls;   // this holds the controls screen

extern sprite tactical_spr,          // holds the images for the tactical displays
       buttons_spr;           // holds the images for the control buttons

extern RGB_color color_1,color_2;    // used for temporaries during color rotation

extern RGB_palette game_palette;     // this will hold the startup system palette

extern bitmap tech_font[NUM_TECH_FONT];   // the tech font bitmaps

extern int game_state;      // the overall state of the game

extern int scanner_state,            // state of scanner
    hud_state,
    tactical_state;

extern int ship_pitch,   // current direction of ship
    ship_yaw    ,   // not used
    ship_roll   ,   // not used
    ship_speed  ,   // speed of ship
    ship_energy ,  // current energy of ship
    ship_damage ,   // current damage up to 50 points
    ship_message,  // current message to ship state machine
    ship_timer  ,  // a little timer
	 ship_kills  ,  // how many bad guys has player killed
    ship_deaths ;  // how many times has player been killed

extern vector_3d unit_z,  // a unit vector in the Z direction
          ship_direction;  // the ships direction

extern int players_ship_type;  // the player starts off with a tallon

// musical sequence information

extern int music_enabled,      // flags that enable music and sound FX
    digital_enabled;

extern int digital_FX_priority; // the priority tracker of the current effect

extern int intro_sequence[];

extern int intro_seq_index; // starting intro index number of sequence to be played

extern int game_sequence[];

extern int game_seq_index; // starting game index number of sequence to be played

extern music song;    // the music structure

// sound fx stuff

extern _sound digital_FX[NUM_SOUND_FX];

// basic colors

extern RGB_color  bright_red,    // bright red
           bright_blue,    // bright blue
           bright_green,    // bright green

           medium_red,    // medium red
			  medium_blue,    // medium blue
           medium_green,    // medium green

			  dark_red,    // dark red
           dark_blue,    // dark blue
           dark_green,    // dark green

           black,     // pure black
           white,  // pure white

           color_1,color_2,color_3;    // general color variables

// the instruction pages

extern char *instructions[];

// specification for the tallon

extern char *tallon_specs[];

// specifications for the slider

extern char *slider_specs[];

