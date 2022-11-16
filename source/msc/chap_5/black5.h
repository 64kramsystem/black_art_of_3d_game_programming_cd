
// Header File for Library Module BLACK5.C

// D E F I N E S  ////////////////////////////////////////////////////////////

// defines for the keyboard interface

#define KEYBOARD_INTERRUPT  0x09  // the keyboard interrupt number
#define KEY_BUFFER          0x60  // the port of the keyboard buffer
#define KEY_CONTROL         0x61  // the port of the keyboard controller
#define PIC_PORT            0x20  // the port of the programmable
                                  // interrupt controller (PIC)

// bitmasks for the "shift state"

// note there is a difference between "on" and "down"

#define SHIFT_RIGHT         0x0001    // right shift
#define SHIFT_LEFT          0x0002    // left shift
#define CTRL                0x0004    // control key
#define ALT                 0x0008    // alternate key
#define SCROLL_LOCK_ON      0x0010    // the scroll lock is on
#define NUM_LOCK_ON         0x0020    // the numeric lock is on
#define CAPS_LOCK_ON        0x0040    // the capitals lock is on
#define INSERT_MODE         0x0080    // insert or overlay mode
#define CTRL_LEFT           0x0100    // the left control key is pressed
#define ALT_LEFT            0x0200    // the left alternate key is pressed
#define CTRL_RIGHT          0x0400    // the right control key is pressed
#define ALT_RIGHT           0x0800    // the right alternate key is pressed
#define SCROLL_LOCK_DOWN    0x1000    // the scroll lock key is pressed
#define NUM_LOCK_DOWN       0x2000    // the numeric lock is pressed
#define CAPS_LOCK_DOWN      0x4000    // the captials lock key is pressed
#define SYS_REQ_DOWN        0x8000    // the system request key is pressed

// these are the scan codes for the keys on the keyboard, note they are all
// from 0-127 and hence are the "make" scan codes, 128-255 are for the break
// scan codes and are computed simply by adding 128 to each of the make codes

#define MAKE_ESC              1
#define MAKE_1                2
#define MAKE_2                3
#define MAKE_3                4
#define MAKE_4                5
#define MAKE_5                6
#define MAKE_6                7
#define MAKE_7                8
#define MAKE_8                9
#define MAKE_9                10
#define MAKE_0                11
#define MAKE_MINUS            12
#define MAKE_EQUALS           13
#define MAKE_BKSP             14
#define MAKE_TAB              15
#define MAKE_Q                16
#define MAKE_W                17
#define MAKE_E                18
#define MAKE_R                19
#define MAKE_T                20
#define MAKE_Y                21
#define MAKE_U                22
#define MAKE_I                23
#define MAKE_O                24
#define MAKE_P                25
#define MAKE_LFT_BRACKET      26
#define MAKE_RGT_BRACKET      27
#define MAKE_ENTER            28
#define MAKE_CTRL             29
#define MAKE_A                30
#define MAKE_S                31
#define MAKE_D                32
#define MAKE_F                33
#define MAKE_G                34
#define MAKE_H                35
#define MAKE_J                36
#define MAKE_K                37
#define MAKE_L                38
#define MAKE_SEMI             39
#define MAKE_APOS             40
#define MAKE_TILDE            41
#define MAKE_LEFT_SHIFT       42
#define MAKE_BACK_SLASH       43
#define MAKE_Z                44
#define MAKE_X                45
#define MAKE_C                46
#define MAKE_V                47
#define MAKE_B                48
#define MAKE_N                49
#define MAKE_M                50
#define MAKE_COMMA            51
#define MAKE_PERIOD           52
#define MAKE_FOWARD_SLASH     53
#define MAKE_RIGHT_SHIFT      54
#define MAKE_PRT_SCRN         55
#define MAKE_ALT              56
#define MAKE_SPACE            57
#define MAKE_CAPS_LOCK        58
#define MAKE_F1               59
#define MAKE_F2               60
#define MAKE_F3               61
#define MAKE_F4               62
#define MAKE_F5               63
#define MAKE_F6               64
#define MAKE_F7               65
#define MAKE_F8               66
#define MAKE_F9               67
#define MAKE_F10              68
#define MAKE_F11              87
#define MAKE_F12              88
#define MAKE_NUM_LOCK         69
#define MAKE_SCROLL_LOCK      70
#define MAKE_HOME             71
#define MAKE_UP               72
#define MAKE_PGUP             73
#define MAKE_KEYPAD_MINUS     74
#define MAKE_LEFT             75
#define MAKE_CENTER           76
#define MAKE_RIGHT            77
#define MAKE_KEYPAD_PLUS      78
#define MAKE_END              79
#define MAKE_DOWN             80
#define MAKE_PGDWN            81
#define MAKE_INS              82
#define MAKE_DEL              83

// these are the defines for the break codes, they are computed by adding 128
// to each of the make codes

#define BREAK_ESC              (1 +128)
#define BREAK_1                (2 +128)
#define BREAK_2                (3 +128)
#define BREAK_3                (4 +128)
#define BREAK_4                (5 +128)
#define BREAK_5                (6 +128)
#define BREAK_6                (7 +128)
#define BREAK_7                (8 +128)
#define BREAK_8                (9 +128)
#define BREAK_9                (10+128)
#define BREAK_0                (11+128)
#define BREAK_MINUS            (12+128)
#define BREAK_EQUALS           (13+128)
#define BREAK_BKSP             (14+128)
#define BREAK_TAB              (15+128)
#define BREAK_Q                (16+128)
#define BREAK_W                (17+128)
#define BREAK_E                (18+128)
#define BREAK_R                (19+128)
#define BREAK_T                (20+128)
#define BREAK_Y                (21+128)
#define BREAK_U                (22+128)
#define BREAK_I                (23+128)
#define BREAK_O                (24+128)
#define BREAK_P                (25+128)
#define BREAK_LFT_BRACKET      (26+128)
#define BREAK_RGT_BRACKET      (27+128)
#define BREAK_ENTER            (28+128)
#define BREAK_CTRL             (29+128)
#define BREAK_A                (30+128)
#define BREAK_S                (31+128)
#define BREAK_D                (32+128)
#define BREAK_F                (33+128)
#define BREAK_G                (34+128)
#define BREAK_H                (35+128)
#define BREAK_J                (36+128)
#define BREAK_K                (37+128)
#define BREAK_L                (38+128)
#define BREAK_SEMI             (39+128)
#define BREAK_APOS             (40+128)
#define BREAK_TILDE            (41+128)
#define BREAK_LEFT_SHIFT       (42+128)
#define BREAK_BACK_SLASH       (43+128)
#define BREAK_Z                (44+128)
#define BREAK_X                (45+128)
#define BREAK_C                (46+128)
#define BREAK_V                (47+128)
#define BREAK_B                (48+128)
#define BREAK_N                (49+128)
#define BREAK_M                (50+128)
#define BREAK_COMMA            (51+128)
#define BREAK_PERIOD           (52+128)
#define BREAK_FOWARD_SLASH     (53+128)
#define BREAK_RIGHT_SHIFT      (54+128)
#define BREAK_PRT_SCRN         (55+128)
#define BREAK_ALT              (56+128)
#define BREAK_SPACE            (57+128)
#define BREAK_CAPS_LOCK        (58+128)
#define BREAK_F1               (59+128)
#define BREAK_F2               (60+128)
#define BREAK_F3               (61+128)
#define BREAK_F4               (62+128)
#define BREAK_F5               (63+128)
#define BREAK_F6               (64+128)
#define BREAK_F7               (65+128)
#define BREAK_F8               (66+128)
#define BREAK_F9               (67+128)
#define BREAK_F10              (68+128)
#define BREAK_F11              (87+128)
#define BREAK_F12              (88+128)
#define BREAK_NUM_LOCK         (69+128)
#define BREAK_SCROLL_LOCK      (70+128)
#define BREAK_HOME             (71+128)
#define BREAK_UP               (72+128)
#define BREAK_PGUP             (73+128)
#define BREAK_KEYPAD_MINUS     (74+128)
#define BREAK_LEFT             (75+128)
#define BREAK_CENTER           (76+128)
#define BREAK_RIGHT            (77+128)
#define BREAK_KEYPAD_PLUS      (78+128)
#define BREAK_END              (79+128)
#define BREAK_DOWN             (80+128)
#define BREAK_PGDWN            (81+128)
#define BREAK_INS              (82+128)
#define BREAK_DEL              (83+128)

#define KEY_UP                 0
#define KEY_DOWN               1

// defines for the joystick interface

#define JOYPORT               0x201  // joyport is at 201 hex

#define JOYSTICK_BUTTON_1_1   0x10   // joystick 1, button 1
#define JOYSTICK_BUTTON_1_2   0x20   // joystick 1, button 2
#define JOYSTICK_BUTTON_2_1   0x40   // joystick 2, button 1
#define JOYSTICK_BUTTON_2_2   0x80   // joystick 2, button 2

#define JOYSTICK_1            0x01   // joystick 1
#define JOYSTICK_2            0x02   // joystick 2

#define JOYSTICK_1_X          0x01   // joystick 1, x axis
#define JOYSTICK_1_Y          0x02   // joystick 1, y axis
#define JOYSTICK_2_X          0x04   // joystick 2, x axis
#define JOYSTICK_2_Y          0x08   // joystick 2, y axis

#define USE_BIOS              0      // command to use BIOS version of something
#define USE_LOW_LEVEL         1      // command to use our own low level version

// defines for mouse interface

#define MOUSE_INTERRUPT          0x33 //mouse interrupt number
#define MOUSE_RESET              0x00 // reset the mouse
#define MOUSE_SHOW               0x01 // show the mouse
#define MOUSE_HIDE               0x02 // hide the mouse
#define MOUSE_POSITION_BUTTONS   0x03 // get buttons and postion
#define MOUSE_MOTION_REL         0x0B // query motion counters to compute
                                      // relative motion
#define MOUSE_SET_SENSITIVITY    0x1A // set the sensitivity of mouse


// mouse button bitmasks

#define MOUSE_LEFT_BUTTON        0x01 // left mouse button mask
#define MOUSE_MIDDLE_BUTTON      0x04 // middle mouse button mask
#define MOUSE_RIGHT_BUTTON       0x02 // right mouse button mask

// E X T E R N A L S /////////////////////////////////////////////////////////

extern void (_interrupt _far *Old_Keyboard_ISR)();  // holds old keyboard interrupt handler

extern int raw_scan_code;  // the global keyboard scan code

// this holds the keyboard state table which tracks the state of every key
// on the keyboard, if any element is a one then the key is down

extern int keyboard_state[128];

// this tracks the number of keys that are currently pressed, helps
// with keyboard testing logic

extern int keys_active;

// these values hold the maximum, minimum and neutral joystick values for
// both joysticks

extern unsigned int joystick_1_max_x,     // maximum deflection of X axis joy 1
                    joystick_1_max_y,     // maximum deflection of Y axis joy 1
                    joystick_1_min_x,     // minimum deflection of X axis joy 1
                    joystick_1_min_y,     // minimum deflection of Y axis joy 1
                    joystick_1_neutral_x, // neutral or center of X axis joy 1
                    joystick_1_neutral_y, // neutral or center of Y axis joy 1

                    joystick_2_max_x,     // maximum deflection of X axis joy 2
                    joystick_2_max_y,     // maximum deflection of Y axis joy 2
                    joystick_2_min_x,     // minimum deflection of X axis joy 2
                    joystick_2_min_y,     // minimum deflection of Y axis joy 2
                    joystick_2_neutral_x, // neutral or center of X axis joy 2
                    joystick_2_neutral_y; // neutral or center of Y axis joy 2

// P R O T O T Y P E S ////////////////////////////////////////////////////////

// keyboard stuff

void _interrupt _far Keyboard_Driver();

void Keyboard_Install_Driver(void);

void Keyboard_Remove_Driver(void);

unsigned char Get_Key(void);

unsigned int Get_Control_Keys(unsigned int mask);

unsigned char Get_Scan_Code(void);

// joystick stuff

unsigned char Joystick_Buttons(unsigned char button);

unsigned int Joystick(unsigned char joystick);

unsigned int Joystick_Bios(unsigned char joystick);

unsigned char Buttons_Bios(unsigned char button);

void Joystick_Calibrate(int joystick,int method);

int Joystick_Available(int joystick);

// mouse stuff

int Mouse_Control(int command, int *x, int *y,int *buttons);


