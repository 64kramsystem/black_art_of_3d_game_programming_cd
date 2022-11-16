
// Header file for chapter 3 - BLACK3.H

// D E F I N E S /////////////////////////////////////////////////////////////

#define GRAPHICS_MODE13     0x13  // 320x200x256
#define TEXT_MODE           0x03  // 80x25 text mode

#define COLOR_MASK        0x3C6 // the bit mask register
#define COLOR_REGISTER_RD 0x3C7 // set read index at this I/O
#define COLOR_REGISTER_WR 0x3C8 // set write index at this I/O
#define COLOR_DATA        0x3C9 // the R/W data is here

#define ROM_CHAR_SET_SEG 0xF000   // segment of 8x8 ROM character set
#define ROM_CHAR_SET_OFF 0xFA6E   // begining offset of 8x8 ROM character set

#define ROM_CHAR_WIDTH      8     // width of ROM 8x8 character set in pixels
#define ROM_CHAR_HEIGHT     8     // height of ROM 8x8 character set in pixels

#define MODE13_WIDTH   (unsigned int)320 // mode 13h screen dimensions
#define MODE13_HEIGHT  (unsigned int)200

// the VGA card controller ports

#define SEQUENCER             0x3C4    // the sequencer's index port
#define CRT_CONTROLLER        0x3D4    // the crt controller's index port
#define GFX_CONTROLLER        0x3CE    // the graphics controller's index port
#define ATTR_CONTROLLER_FF    0x3DA    // the attribute controller's Flip Flop
#define ATTR_CONTROLLER_DATA  0x3C0    // the attribute controller's data port

// defines for the CRT controller registers of interest

#define CRT_MAX_SCANLINE      0x09     // the maximum scanline register
                                       // used to select how many scanlines
                                       // per row

#define CRT_ADDR_MODE         0x14     // the address mode register
                                       // used to select byte addressing
                                       // for VGA
                                       // also known as the underline register

#define CRT_MODE_CONTROL      0x17     // the mode control register
                                       // used to select single byte addressing


// defines for the GFX controller registers of interest

#define GFX_WRITE_MODE       0x05      // the memory write mode register
                                       // used to deselect even/odd plane
                                       // addressing

#define GFX_MISC             0x06      // the miscellaneous register
                                       // used to deselect the chaining
                                       // of memory


// defines for the SEQUENCER registers of interest

#define SEQ_PLANE_ENABLE     0x02      // plane enable register, used to select
                                       // which planes are written to by a
                                       // CPU write
#define SEQ_MEMORY_MODE      0x04      // the memory mode register
                                       // used to deselect memory chain mode
                                       // and odd/even memory addressing

// M A C R O S ///////////////////////////////////////////////////////////////

#define SET_BITS(x,bits)   (x | bits)
#define RESET_BITS(x,bits) (x & ~bits)

// S T R U C T U R E S ///////////////////////////////////////////////////////

// this structure holds a RGB triple in three unsigned bytes

typedef struct RGB_color_typ
        {

        unsigned char red;    // red   component of color 0-63
        unsigned char green;  // green component of color 0-63
        unsigned char blue;   // blue  component of color 0-63

        } RGB_color, *RGB_color_ptr;

// this structure holds an entire color palette

typedef struct RGB_palette_typ
        {
        int start_reg;   // index of the starting register that is save
        int end_reg;     // index of the ending registe that is saved

        RGB_color colors[256];   // the storage area for the palette

        } RGB_palette, *RGB_palette_ptr;

// P R O T O T Y P E S ///////////////////////////////////////////////////////

void Print_Char(int xc,int yc,char c,int color,int transparent);

void Print_String(int x,int y,int color, char *string,int transparent);

void Write_Pixel(int x,int y,int color);

int Read_Pixel(int x,int y);

void Set_Graphics_Mode(int mode);

void Time_Delay(int clicks);

void Line_H(int x1,int x2,int y,int color);

void Line_V(int y1,int y2,int x,int color);

void Write_Color_Reg(int index, RGB_color_ptr color);

void Read_Palette(int start_reg,int end_reg, RGB_palette_ptr the_palette);

void Write_Palette(int start_reg,int end_reg, RGB_palette_ptr the_palette);

RGB_color_ptr Read_Color_Reg(int index, RGB_color_ptr color);

void Draw_Rectangle(int x1,int y1,int x2,int y2,int color);

void Fill_Screen(int color);

void Fill_Screen_Z(int color);

void Write_Pixel_Z(int x,int y,int color);

void Set_Mode_Z(void);

// G L O B A L S /////////////////////////////////////////////////////////////

extern unsigned char far *video_buffer;   // video ram byte ptr
extern unsigned char far *rom_char_set;   // rom characters 8x8


