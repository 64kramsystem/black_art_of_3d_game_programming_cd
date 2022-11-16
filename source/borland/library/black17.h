
// BLACK17.H - header file


// D E F I N E S /////////////////////////////////////////////////////////////

#define MAX_POINTS_PER_POLY     4   // a maximum of four points per poly
#define MAX_VERTICES_PER_OBJECT 24  // this should be enough
#define MAX_POLYS_PER_OBJECT    16  // this will have to do!
#define MAX_OBJECTS             32  // maximum number of objects in world
#define MAX_POLYS_PER_FRAME     128 // maximum number of polys in a single
                                    // animation frame

// polygon shading specifiers for PLG files

// format = 0xh3 h2 h1h0
// h3   = shading type
// h2   = special attributes= d3d2d1d0

// d3,d2,d1 unused, d0 = two sided flag, 0 = 1 sided, 1 = 2 sided

// h1h0 = color 0-255

#define ONE_SIDED               0 // number of sides polygon has
#define TWO_SIDED               1

#define CONSTANT_SHADING        0
#define FLAT_SHADING            1
#define GOURAUD_SHADING         2 (actually metallic under PLG defintion)
#define SPFX_SHADING            3

#define ASPECT_RATIO            (float)0.8  // the aspect ratio
#define INVERSE_ASPECT_RATIO    (float)1.25 // the inverse of the aspect ratio

#define HALF_SCREEN_WIDTH       160 // center of screen
#define HALF_SCREEN_HEIGHT      100

#define POLY_CLIP_MIN_X         0  // miniumum x,y clip values
#define POLY_CLIP_MIN_Y         0

#define POLY_CLIP_MAX_X         319 // maxium x,y clip values
#define POLY_CLIP_MAX_Y         199

#define CLIP_Z_MODE             0  // this constant tells the clipper to do a
                                   // simple z extent clip

#define CLIP_XYZ_MODE           1  // this constant tells the clipper to do
                                   // a full 3D volume clip



#define OBJECT_CULL_Z_MODE      0  // this constant tells the object culler to do a
                                   // simple z extent clip

#define OBJECT_CULL_XYZ_MODE    1  // this constant tells the object culler to do
                                   // a full 3D volume clip


// these are the constants needed for the shader engine
// note that brightness increses with smaller values and there
// are 16 shades of each color eg. bright blue is 144 and dark blue is 159

#define SHADE_GREY         31    // hex value = 1F
#define SHADE_GREEN        111   // hex value = 6F
#define SHADE_BLUE         159   // hex value = 9F
#define SHADE_RED          47    // hex value = 2F
#define SHADE_YELLOW       79    // hex value = 4F
#define SHADE_BROWN        223   // hex value = DF
#define SHADE_LIGHT_BROWN  207   // hex value = CF
#define SHADE_PURPLE       175   // hex value = AF
#define SHADE_CYAN         127   // hex value = 7F
#define SHADE_LAVENDER     191   // hex value = BF


// defines for object collisions

#define NO_COLLISION       0
#define SOFT_COLLISION     1
#define HARD_COLLISION     2

// defines for polygon list generator

#define RESET_POLY_LIST   0
#define ADD_TO_POLY_LIST  1

// fixed point stuff

#define FP_SHIFT          16          // 16:16 format
#define FP_SCALE          65526L      // 2^16 = 65536, used to convert floats

// M A C R O S ///////////////////////////////////////////////////////////////



// T Y P E D E F S ///////////////////////////////////////////////////////////


// needed for qsort

typedef int (*qsort_cast)(const void *, const void *);

typedef float matrix_4x4[4][4];       // the standard 4x4 homogenous matrix

typedef float matrix_1x4[4];          // a 1x4 matrix or a row vector

// this structure holds a vector or a simple 3-D point

typedef struct vector_3d_typ
        {

        float x,y,z,w;    // a 3-D vector along with normalization factor
                          // if needed

        } point_3d,vector_3d,*point_3d_ptr,*vector_3d_ptr;

// this function holds a objects orientation or direction relative to the
// axis of a left handed system

typedef struct dir_3d_typ
        {

        int ang_x,    // angle relative to x axis
            ang_y,    // angle relative to y axis
            ang_z;    // angle relative to z axis

        } dir_3d, *dir_3d_ptr;

// this stucture holds a polygon, but is used internaly by the object defintion

typedef struct polygon_typ
        {

        int num_points;   // number of points in polygon (usually 3 or 4)

        int vertex_list[MAX_POINTS_PER_POLY];  // the index number of vertices

        int color;       // color of polygon

        int shade;       // the final shade of color after lighting

        int shading;     // type of lighting, flat or constant shading

        int two_sided;   // flags if the polygon is two sided

        int visible;     // used to remove backfaces

        int active;      // used to turn faces on and off

        int clipped;     // flags that polygon has been clipped or removed

        float normal_length; // pre-computed magnitude of normal


        } polygon, *polygon_ptr;

// this structure holds a final polygon facet and is self contained

typedef struct facet_typ
        {

        int num_points;  // number of vertices

        int color;       // color of polygon

        int shade;       // the final shade of color after lighting

        int shading;     // type of shading to use

        int two_sided;   // is the facet two sided

        int visible;     // is the facet transparent

        int clipped;     // has this poly been clipped

        int active;      // used to turn faces on and off

        point_3d vertex_list[MAX_POINTS_PER_POLY]; // the points that make
                                                   // up the polygon facet

        float normal_length;  // holds pre-computed length of normal

        int average_z;        // holds average z, used in sorting

        } facet, *facet_ptr;

// this structure holds an object

typedef struct object_typ
        {

        int id;             // identification number of object

        int num_vertices;   // total number of vertices in object

        point_3d vertices_local[MAX_VERTICES_PER_OBJECT];  // local vertices

        point_3d vertices_world[MAX_VERTICES_PER_OBJECT];  // world vertices

        point_3d vertices_camera[MAX_VERTICES_PER_OBJECT]; // camera vertices

        int num_polys;      // the number of polygons in the object

        polygon polys[MAX_POLYS_PER_OBJECT]; // the polygons that make up the object

        float radius;       // the average radius of object

        int state;          // state of object

        point_3d world_pos; // position of object in world coordinates

        } object, *object_ptr;


// fixed point type

typedef long fixed;


// P R O T O T Y P E S //////////////////////////////////////////////////////

int Load_Palette_Disk(char *filename, RGB_palette_ptr the_palette);

int Save_Palette_Disk(char *filename, RGB_palette_ptr the_palette);

float Compute_Object_Radius(object_ptr the_object);

void Build_Look_Up_Tables(void);

float Dot_Product_3D(vector_3d_ptr u,vector_3d_ptr v);

void Make_Vector_3D(point_3d_ptr init,
                    point_3d_ptr term,
                    vector_3d_ptr result);

void Cross_Product_3D(vector_3d_ptr u,
                      vector_3d_ptr v,
                      vector_3d_ptr normal);


float Vector_Mag_3D(vector_3d_ptr v);

void Mat_Print_4x4(matrix_4x4 a);

void Mat_Print_1x4(matrix_1x4 a);

void Mat_Mul_4x4_4x4(matrix_4x4 a,
                     matrix_4x4 b,
                     matrix_4x4 result);

void Mat_Mul_1x4_4x4(matrix_1x4 a,
                     matrix_4x4 b,
                     matrix_1x4 result);

void Mat_Identity_4x4(matrix_4x4 a);

void Mat_Zero_4x4(matrix_4x4 a);

void Mat_Copy_4x4(matrix_4x4 source, matrix_4x4 destination);

void Local_To_World_Object(object_ptr the_object);

void Create_World_To_Camera(void);

void World_To_Camera_Object(object_ptr the_object);

void Rotate_Object(object_ptr the_object,int angle_x,int angle_y,int angle_z);

void Translate_Object(object_ptr the_object,int x_trans,int y_trans,int z_trans);

int Objects_Collide(object_ptr object_1, object_ptr object_2);

void Scale_Object(object_ptr the_object,float scale_factor);

void Position_Object(object_ptr the_object,int x,int y,int z);

char *PLG_Get_Line(char *string, int max_length, FILE *fp);

int PLG_Load_Object(object_ptr the_object,char *filename,float scale);

void Clip_Object_3D(object_ptr the_object, int mode);

void Remove_Backfaces_And_Shade(object_ptr the_object);

int Remove_Object(object_ptr the_object, int mode);

void Generate_Poly_List(object_ptr the_object,int mode);

int Poly_Compare(facet **arg1, facet **arg2);

void Sort_Poly_List(void);

void Print_Poly_List(void);

void Draw_Poly_List(void);

void Draw_Triangle_2D(int x1,int y1,
                      int x2,int y2,
                      int x3,int y3,int color);

void Draw_Top_Triangle(int x1,int y1, int x2,int y2, int x3,int y3,int color);

void Draw_Bottom_Triangle(int x1,int y1, int x2,int y2, int x3,int y3,int color);

void Triangle_Line(unsigned char far *dest_addr,
                   unsigned int xs,
                   unsigned int xe,
                   int color);

void Draw_Triangle_2D(int x1,int y1,
                      int x2,int y2,
                      int x3,int y3,int color);


void Make_Grey_Palette(void);


// new 32 bit functions

void Display_Double_Buffer_32(unsigned char far *buffer,int y);

void Fill_Double_Buffer_32(int color);

// external assembly language functions

void fquadcpy(void far *dest, void far *source, long count);

void fquadset(void far *dest, long data, long count);

void Triangle_32Line(unsigned char far *dest_addr,
                     unsigned int xs,
                     unsigned int xe,
                     int color);

void Triangle_16Line(unsigned char far *dest_addr,
                     unsigned int xs,
                     unsigned int xe,
                     int color);


void Triangle_Asm(void far *dest_addr,int y1,int y3,
                  float xs,float xe,float dx_left,float dx_right,
                  int color);


// fixed point functions

fixed FP_Mul(fixed mulipicand, fixed multiplier);

fixed FP_Div(fixed dividend, fixed divisor);


// E X T E R N A L S//////////////////////////////////////////////////////////

extern float  clip_near_z,         // the near or hither clipping plane
              clip_far_z,          // the far or yon clipping plane
              screen_width,        // dimensions of the screen
              screen_heigth;

extern float viewing_distance;       // distance of projection plane from camera

extern point_3d view_point;        // position of camera

extern vector_3d light_source;     // position of point light source

extern float ambient_light;        // ambient light level

extern dir_3d view_angle;          // angle of camera

extern matrix_4x4 global_view;        // the global inverse wortld to camera

extern RGB_palette color_palette_3d;  // the color palette used for the 3D system

extern int num_objects;               // number of objects in the world

extern object_ptr world_object_list[MAX_OBJECTS];  // the objects in the world

extern int num_polys_frame;                        // the number of polys in this frame

extern facet_ptr world_polys[MAX_POLYS_PER_FRAME]; // the visible polygons for this frame

extern facet world_poly_storage[MAX_POLYS_PER_FRAME];  // the storage for the visible
                                                // polygons is pre-allocated
                                                // so it doesn't need to be
                                                // allocated frame by frame

// look up tables

extern float sin_look[360+1],   // SIN from 0 to 360
             cos_look[360+1];   // COSINE from 0 to 360

// the clipping region, set it to default on start up

extern int poly_clip_min_x,
           poly_clip_min_y,

           poly_clip_max_x,
           poly_clip_max_y;

extern sprite textures;              // this holds the textures


