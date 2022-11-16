
// header file for black15.c

// D E F I N E S //////////////////////////////////////////////////////////////

// these constants are used to place the walls in world coordinates

#define WORLD_SCALE_X        2      // scaling factors used to scale the
#define WORLD_SCALE_Y        2      // screen coordinates that the BSP
#define WORLD_SCALE_Z       -2      // is drawn with

#define WORLD_POS_X          0      // the final position to move the
#define WORLD_POS_Y          0      // walls to when "view" is selected
#define WORLD_POS_Z          300

#define SCREEN_TO_WORLD_X    -112   // the translation factors to move the origin
#define SCREEN_TO_WORLD_Z    -100   // to the center of the screen in mode 320x200

#define WALL_CEILING         20 // the y coordinate of the artificial ceiling
#define WALL_FLOOR          -20 // the y coordinate of the artificial floor


#define BSP_WALL_COLOR       47 // color of all the walls in the bsp tree
#define BSP_WALL_SHADE       47

// M A C R O S ///////////////////////////////////////////////////////////////

// tests if two 3-d points are equal

#define POINTS_EQUAL_3D(p1,p2) (p1.x==p2.x && p1.y==p2.y && p1.z==p2.z)

// S T R U C T U R E S ////////////////////////////////////////////////////////


// this structure holds a wall, it's very similar to our standard polygon
// structure, but stripped down for demo purposes

typedef struct wall_typ
        {
        int id;                  // used for debugging

        int color;               // color of wall

        point_3d wall_world[4];  // the points that make up the wall
        point_3d wall_camera[4]; // the final camera coordinates of the wall

        vector_3d normal;        // the outward normal to the wall used during
                                 // creation of BSP only, after that it becomes
                                 // invalid

        struct wall_typ *link;   // pointer to next wall
        struct wall_typ *front;  // pointer to walls in front
        struct wall_typ *back;   // pointer to walls behind

        } wall, *wall_ptr;

//  P R O T O T Y P E S ///////////////////////////////////////////////////////

void Draw_TB_Tri_3D_Z(int x1,int y1, int z1,
                      int x2,int y2, int z2,
                      int x3,int y3, int z3,
                      int color);


void Draw_Tri_3D_Z(int x1,int y1,int z1,
                   int x2,int y2,int z2,
                   int x3,int y3,int z3,
                   int color);

void Draw_Poly_List_Z(void);

int Create_Z_Buffer(unsigned int height);

void Delete_Z_Buffer(void);

void Fill_Z_Buffer(int value);

void Bsp_World_To_Camera(wall_ptr root);

void Bsp_Translate(wall_ptr root,int x_trans,int y_trans,int z_trans);

void Bsp_Shade(wall_ptr root);

void Bsp_Traverse(wall_ptr root);

void Bsp_Delete(wall_ptr root);

void Bsp_Print(wall_ptr root);

void Bsp_View(wall_ptr bsp_root);

void Build_Bsp_Tree(wall_ptr root);

void Intersect_Lines(float x0,float y0,float x1,float y1,
                     float x2,float y2,float x3,float y3,
                     float *xi,float *yi);

// E X T E R N A L S///////////////////////////////////////////////////////////

extern int far *z_buffer;   // the current z buffer memory
extern int far *z_bank_1;   // memory bank 1 of z buffer
extern int far *z_bank_2;   // memory bank 2 of z buffer

extern unsigned int z_height; // the height if the z buffer
extern unsigned int z_height_2;      // the height if half the z buffer
extern unsigned int z_bank_size;     // size of a z buffer bank in bytes

extern FILE *fp_out;  // general output file

