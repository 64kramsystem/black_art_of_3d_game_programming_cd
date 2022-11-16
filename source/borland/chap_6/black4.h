
// HEADER FILE FOR BLACK4.C

// D E F I N E S  ////////////////////////////////////////////////////////////

#define SCREEN_WIDTH      (unsigned int)320 // mode 13h screen dimensions
#define SCREEN_HEIGHT     (unsigned int)200

// screen transition commands

#define SCREEN_DARKNESS  0         // fade to black
#define SCREEN_WHITENESS 1         // fade to white
#define SCREEN_WARP      2         // warp the screen image
#define SCREEN_SWIPE_X   3         // do a horizontal swipe
#define SCREEN_SWIPE_Y   4         // do a vertical swipe
#define SCREEN_DISOLVE   5         // a pixel disolve

#define VGA_INPUT_STATUS_1   0x3DA // VGA input status register number 1
                                   // D3 is vertical retrace bit
                                   // 1 - retrace in progress
                                   // 0 - no retrace

#define VGA_VRETRACE_MASK 0x08     // masks off unwanted bit of status reg

#define SPRITE_WIDTH      16       // default width of a sprite
#define SPRITE_HEIGHT     16       // default height of a sprite

#define MAX_SPRITE_FRAMES 32

// sprite states

#define SPRITE_DEAD       0
#define SPRITE_ALIVE      1
#define SPRITE_DYING      2

// mode Z page stuff

#define PAGE_0            0
#define PAGE_1            1

// these are used to change the visual page of the VGA

#define CRT_ADDR_LOW      0x0D // the index of the low byte of the start address
#define CRT_ADDR_HI       0x0C // the index of the hi byte of the start address

// M A C R O S ///////////////////////////////////////////////////////////////

#define SET_SPRITE_SIZE(w,h) {sprite_width=w; sprite_height=h;}

// S T R U C T U R E S ///////////////////////////////////////////////////////

// the PCX file structure

typedef struct pcx_header_typ
        {
        char manufacturer;          // the manufacturer of the file
        char version;               // the file format version
        char encoding;              // type of compression
        char bits_per_pixel;        // number of bits per pixel
        int x,y;                    // starting location of image
        int width,height;           // size of image
        int horz_res;               // resolution in DPI (dots per inch)
        int vert_res;
        char ega_palette[48];       // the old EGA palette (usually ignored)
        char reserved;              // don't care
        char num_color_planes;      // number of color planes
        int bytes_per_line;         // number of bytes per line of the image
        int palette_type;           // 1 for color, 2 for grey scale palette
        char padding[58];           // extra bytes

        } pcx_header, *pcx_header_ptr;

// this holds the PCX header and the actual image

typedef struct pcx_picture_typ
        {
        pcx_header header;          // the header of the PCX file
        RGB_color palette[256];     // the palette data
        unsigned char far *buffer;  // a pointer to the 64,000 byte buffer
                                    // holding the decompressed image

        } pcx_picture, *pcx_picture_ptr;


// this is a sprite structure

typedef struct sprite_typ
        {
        int x,y;            // position of sprite
        int width,height;   // dimensions of sprite in pixels

        int counter_1;      // some counters for timing and animation
        int counter_2;
        int counter_3;

        int threshold_1;    // thresholds for the counters (if needed)
        int threshold_2;
        int threshold_3;

        unsigned char far *frames[MAX_SPRITE_FRAMES]; // array of pointers to
                                                      // the images

        int curr_frame;                // current frame being displayed
        int num_frames;                // total number of frames
        int state;                     // state of sprite, alive, dead...
        unsigned char far *background; // image under the sprite

        int x_clip,y_clip;             // clipped position of sprite
        int width_clip,height_clip;    // clipped size of sprite
        int visible;                   // used by sprite engine to flag
                                       // if a sprite was invisible last
                                       // time it was drawn hence the background
                                       // need not be replaced

        } sprite, *sprite_ptr;

// this is the typedef for a bitmap

typedef struct bitmap_typ
        {
        int x,y;                    // position of bitmap
        int width,height;           // size of bitmap
        unsigned char far *buffer;  // buffer holding image

        } bitmap, *bitmap_ptr;

// this is a typdef used for the layers in parallax scrolling
// note it is identical to a bitmap, but we'll make a separate typedef
// in the event we later need to add fields to it

typedef struct layer_typ
        {
        int x,y;           // used to hold position information
                           // no specific function

        int width,height;  // size of layer,note:width must be divisible by 2

        unsigned char far *buffer;  // the layer buffer

        } layer, *layer_ptr;

// E X T E R N A L S /////////////////////////////////////////////////////////

extern unsigned char far *double_buffer;     // the double buffer

extern unsigned int double_buffer_height;    // height of double buffer

extern unsigned int double_buffer_size;      // total size of buffer in bytes

extern unsigned int sprite_width;            // width of a sprite

extern unsigned int sprite_height;           // of a sprite

extern unsigned char far *page_0_buffer;     // pointer to mode z page 0

extern unsigned char far *page_1_buffer;     // pointer to mode z page 0

extern int mode_z_page;                      // current mode z page

// P R O T O T Y P E S ///////////////////////////////////////////////////////

int  PCX_Init(pcx_picture_ptr image);

int  PCX_Load(char *filename, pcx_picture_ptr image,int enable_palette);

void PCX_Delete(pcx_picture_ptr image);

void PCX_Show_Buffer(pcx_picture_ptr image);

void PCX_Copy_To_Buffer(pcx_picture_ptr image,unsigned char far *buffer);

void PCX_Get_Sprite(pcx_picture_ptr image,
                     sprite_ptr sprite,
                     int sprite_frame,
                     int cell_x, int cell_y);

void Sprite_Init(sprite_ptr sprite,int x,int y, int width, int height,
                                   int c1,int c2,int c3,
                                   int t1,int t2,int t3);

void Sprite_Delete(sprite_ptr sprite);

void Sprite_Under(sprite_ptr sprite, unsigned char far *buffer);

void Sprite_Erase(sprite_ptr sprite, unsigned char far *buffer);

void Sprite_Draw(sprite_ptr sprite, unsigned char far *buffer,int transparent);


void Sprite_Under_Clip(sprite_ptr sprite, unsigned char far *buffer);

void Sprite_Erase_Clip(sprite_ptr sprite, unsigned char far *buffer);

void Sprite_Draw_Clip(sprite_ptr sprite, unsigned char far *buffer,int transparent);

int  Sprite_Collide(sprite_ptr sprite_1, sprite_ptr sprite_2);

void Display_Double_Buffer(unsigned char far *buffer,int y);

int  Create_Double_Buffer(int num_lines);

void Fill_Double_Buffer(int color);

void Delete_Double_Buffer(void);

void Screen_Transition(int effect);

void Wait_For_Vertical_Retrace(void);

void fwordcpy(void far *destination, void far *source,int num_words);

void Bitmap_Get(bitmap_ptr image, unsigned char far *source);

void Bitmap_Put(bitmap_ptr image, unsigned char far *destination,int transparent);

int Bitmap_Allocate(bitmap_ptr image, int width, int height);

void Bitmap_Delete(bitmap_ptr the_bitmap);

int Layer_Create(layer_ptr dest_layer, int width, int height);

void Layer_Build(layer_ptr dest_layer,int dest_x, int dest_y,
                unsigned char far *source_buffer,int source_x,int source_y,
                int width,int height);

void Layer_Draw(layer_ptr source_layer, int source_x, int source_y,
                unsigned char far *dest_buffer,int dest_y,int dest_height,
                int transparent);

void Layer_Delete(layer_ptr the_layer);

void Print_Char_DB(int xc,int yc,char c,int color,int transparent);

void Print_String_DB(int x,int y,int color, char *string,int transparent);

void Write_Pixel_DB(int x,int y,int color);

int Read_Pixel_DB(int x,int y);

void Set_Working_Page_Mode_Z(int page);

void Set_Visual_Page_Mode_Z(int page);


