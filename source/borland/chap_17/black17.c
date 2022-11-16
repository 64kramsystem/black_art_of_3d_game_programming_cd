
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
#include <search.h>             // this one is needed for qsort()

// include all of our stuff

#include "black3.h"
#include "black4.h"
#include "black5.h"
#include "black6.h"
#include "black8.h"
#include "black9.h"
#include "black17.h"

// G L O B A L S //////////////////////////////////////////////////////////////

float  clip_near_z   = 100,       // the near or hither clipping plane
       clip_far_z    = 3200,      // the far or yon clipping plane
       screen_width  = 320,       // dimensions of the screen
       screen_heigth = 200;

float viewing_distance = 200;     // distance of projection plane from camera

point_3d view_point  = {0,0,0,1}; // position of camera

vector_3d light_source = {-0.913913,0.389759,-0.113369}; // position of point light source

float ambient_light = 6;          // ambient light level

dir_3d view_angle = {0,0,0};      // angle of camera

matrix_4x4 global_view;           // the global inverse wortld to camera
                                  // matrix

RGB_palette color_palette_3d;     // the color palette used for the 3D system

int num_objects;                  // number of objects in the world

object_ptr world_object_list[MAX_OBJECTS];  // the objects in the world

int num_polys_frame;                        // the number of polys in this frame

facet_ptr world_polys[MAX_POLYS_PER_FRAME]; // the visible polygons for this frame

facet world_poly_storage[MAX_POLYS_PER_FRAME];  // the storage for the visible
                                                // polygons is pre-allocated
                                                // so it doesn't need to be
                                                // allocated frame by frame

// look up tables

float sin_look[360+1],                          // SIN from 0 to 360
      cos_look[360+1];                          // COSINE from 0 to 360

// the clipping region, set it to default on start up

int poly_clip_min_x = POLY_CLIP_MIN_X,          // the minimum boundaries
    poly_clip_min_y = POLY_CLIP_MIN_Y,

    poly_clip_max_x = POLY_CLIP_MAX_X,          // the maximum boundaries
    poly_clip_max_y = POLY_CLIP_MAX_Y;


sprite textures;                                // this holds the textures

// F U N C T I O N S /////////////////////////////////////////////////////////

void Triangle_Line(unsigned char far *dest_addr,
                   unsigned int xs,
                   unsigned int xe,
                   int color)
{

// this function draws a fast horizontal line by using WORD size writes
// the assembly language version is called  Triangle_16Line();


// plot pixels at ends of line first

if (xs & 0x0001)
   {
   // plot a single starting pixel and move xs to an even boundary

   dest_addr[xs++] = (unsigned char)color;

   } // end if

if (!(xe & 0x0001))
   {
   // plot a single terminating pixel and move xe back to an odd boundary

   dest_addr[xe--] = (unsigned char)color;

   } // end if

// now plot the line

dest_addr+=xs;

// now blast the middle part of the line a WORD at a time, later use
// an external assembly program to do it a DOUBLE WORD at a time!

_asm   {
	les di,dest_addr      // point es:di at data area

	mov al,BYTE PTR color // move into al and ah the color
	mov ah,al

	mov cx,xe             // compute number of words to move  (xe-xs+1)/2
	sub cx,xs
	inc cx
	shr cx,1              //divide by 2

	rep stosw             // draw the line

	} // end asm

} // end Triangle_Line

////////////////////////////////////////////////////////////////////////////////

void Triangle_QLine(unsigned char far *dest_addr,
                    unsigned int xs,
                    unsigned int xe,
                    unsigned int color)
{
// this is the C version of the quad byte line renderer, not much faster than
// the word version...not worth the extra complexity. the assembly language
// version is called Triangle_32Line()

long length,
     lcolor;

// test special cases

switch(xe-xs)
      {
      case 0:
         {

         dest_addr[xs] = color;
         return;

			} break;

      case 1:
         {

         dest_addr[xs++] = color;
         dest_addr[xs]   = color;
         return;

         } break;

      case 2:
         {

         dest_addr[xs++] = color;
         dest_addr[xs++] = color;
         dest_addr[xs]   = color;
         return;

         } break;

      case 3:
         {
         dest_addr[xs++] = color;
         dest_addr[xs++] = color;
         dest_addr[xs++] = color;
         dest_addr[xs]   = color;
         return;

         } break;

      default:break;

      } // end switch

// line is longer than 4 pixels, so we can use algorithm

// process left side

switch(xs & 0x03)
      {

      case 1:
           {
           dest_addr[xs++] = color;
           dest_addr[xs++] = color;
           dest_addr[xs++] = color;
           } break;

      case 2:
           {
           dest_addr[xs++] = color;
           dest_addr[xs++] = color;
           } break;

      case 3:
           {
           dest_addr[xs++] = color;
           } break;


      default: break;

      } // end switch

// process right side

switch(xe & 0x03)
      {

      case 0:
           {
           dest_addr[xe--] = color;
           } break;

      case 1:
           {
           dest_addr[xe--] = color;
           dest_addr[xe--] = color;
           } break;

      case 2:
           {

           dest_addr[xe--] = color;
           dest_addr[xe--] = color;
           dest_addr[xe--] = color;

           } break;

      default: break;

      } // end switch


// draw the middle part of the line now using words

// now plot the line

dest_addr+=xs;

lcolor = (color << 8 | color);
lcolor = (lcolor << 16 | lcolor);
length = ((xe-xs+1) >> 2);

fquadset(dest_addr,lcolor,length);

} // end Triangle_QLine

//////////////////////////////////////////////////////////////////////////////

void Draw_Top_Triangle(int x1,int y1, int x2,int y2, int x3,int y3,int color)
{
// this function draws a triangle that has a flat top

float dx_right,    // the dx/dy ratio of the right edge of line
      dx_left,     // the dx/dy ratio of the left edge of line
      xs,xe,       // the starting and ending points of the edges
      height;      // the height of the triangle

int temp_x,        // used during sorting as temps
    temp_y,
    right,         // used by clipping
    left;

unsigned char far *dest_addr;

// test order of x1 and x2

if (x2 < x1)
   {
   temp_x = x2;
   x2     = x1;
   x1     = temp_x;

   } // end if swap

// compute delta's

height = y3-y1;

dx_left  = (x3-x1)/height;
dx_right = (x3-x2)/height;

// set starting points

xs = (float)x1;
xe = (float)x2+(float)0.5;

// perform y clipping

if (y1<poly_clip_min_y)
   {
   // compute new xs and ys

   xs = xs+dx_left*(float)(-y1+poly_clip_min_y);
   xe = xe+dx_right*(float)(-y1+poly_clip_min_y);

   // reset y1

   y1=poly_clip_min_y;

   } // end if top is off screen

if (y3>poly_clip_max_y)
   y3=poly_clip_max_y;


// compute starting address in video memory

dest_addr = double_buffer+(y1<<8)+(y1<<6);

// test if x clipping is needed

if (x1>=poly_clip_min_x && x1<=poly_clip_max_x &&
    x2>=poly_clip_min_x && x2<=poly_clip_max_x &&
    x3>=poly_clip_min_x && x3<=poly_clip_max_x)
    {
    // draw the triangle

#if 0

	 for (temp_y=y1; temp_y<=y3; temp_y++,dest_addr+=320)
		  {

		  Triangle_16Line(dest_addr,(unsigned int)xs,(unsigned int)xe,color);

		  // adjust starting point and ending point

		  xs+=dx_left;
		  xe+=dx_right;

		  } // end for

#endif

#if 1

	 // use the external assembly language triangle engine based on fixed point

	 if (y3>y1)
		 Triangle_Asm(dest_addr,y1,y3,xs,xe,dx_left,dx_right,color);


#endif

	 } // end if no x clipping needed
else
	{
	// clip x axis with slower version

	// draw the triangle

   for (temp_y=y1; temp_y<=y3; temp_y++,dest_addr+=320)
       {

       // do x clip

       left  = (int)xs;
       right = (int)xe;

		 // adjust starting point and ending point

		 xs+=dx_left;
       xe+=dx_right;

       // clip line

       if (left < poly_clip_min_x)
          {
          left = poly_clip_min_x;

          if (right < poly_clip_min_x)
				 continue;

			 }

       if (right > poly_clip_max_x)
          {
          right = poly_clip_max_x;

          if (left > poly_clip_max_x)
             continue;

          }

		  Triangle_16Line(dest_addr,(unsigned int)left,(unsigned int)right,color);

       } // end for

   } // end else x clipping needed

} // end Draw_Top_Triangle

/////////////////////////////////////////////////////////////////////////////

void Draw_Bottom_Triangle(int x1,int y1, int x2,int y2, int x3,int y3,int color)
{

// this function draws a triangle that has a flat bottom

float dx_right,    // the dx/dy ratio of the right edge of line
      dx_left,     // the dx/dy ratio of the left edge of line
      xs,xe,       // the starting and ending points of the edges
      height;      // the height of the triangle

int temp_x,        // used during sorting as temps
    temp_y,
    right,         // used by clipping
    left;

unsigned char far *dest_addr;

// test order of x1 and x2

if (x3 < x2)
   {
   temp_x = x2;
   x2     = x3;
   x3     = temp_x;

   } // end if swap

// compute delta's

height = y3-y1;

dx_left  = (x2-x1)/height;
dx_right = (x3-x1)/height;

// set starting points

xs = (float)x1;
xe = (float)x1+(float)0.5;

// perform y clipping

if (y1<poly_clip_min_y)
   {
   // compute new xs and ys

   xs = xs+dx_left*(float)(-y1+poly_clip_min_y);
   xe = xe+dx_right*(float)(-y1+poly_clip_min_y);

   // reset y1

	y1=poly_clip_min_y;

   } // end if top is off screen

if (y3>poly_clip_max_y)
   y3=poly_clip_max_y;

// compute starting address in video memory

dest_addr = double_buffer+(y1<<8)+(y1<<6);

// test if x clipping is needed

if (x1>=poly_clip_min_x && x1<=poly_clip_max_x &&
    x2>=poly_clip_min_x && x2<=poly_clip_max_x &&
    x3>=poly_clip_min_x && x3<=poly_clip_max_x)
    {
    // draw the triangle


#if 0

	 for (temp_y=y1; temp_y<=y3; temp_y++,dest_addr+=320)
		  {

		  Triangle_16Line(dest_addr,(unsigned int)xs,(unsigned int)xe,color);

		  // adjust starting point and ending point

		  xs+=dx_left;
		  xe+=dx_right;

		  } // end for

#endif

#if 1

     // use the external assembly language triangle engine based on fixed point

     if (y3>y1)
        Triangle_Asm(dest_addr,y1,y3,xs,xe,dx_left,dx_right,color);

#endif

    } // end if no x clipping needed
else
   {
   // clip x axis with slower version

   // draw the triangle

   for (temp_y=y1; temp_y<=y3; temp_y++,dest_addr+=320)
       {
       // do x clip

       left  = (int)xs;
       right = (int)xe;

       // adjust starting point and ending point

		 xs+=dx_left;
       xe+=dx_right;

       // clip line

       if (left < poly_clip_min_x)
          {
          left = poly_clip_min_x;

          if (right < poly_clip_min_x)
             continue;

			 }

       if (right > poly_clip_max_x)
          {
          right = poly_clip_max_x;

          if (left > poly_clip_max_x)
             continue;
          }

       Triangle_16Line(dest_addr,(unsigned int)left,(unsigned int)right,color);

		 } // end for

   } // end else x clipping needed

} // end Draw_Bottom_Triangle

///////////////////////////////////////////////////////////////////////////////

void Draw_Triangle_2D(int x1,int y1,
                      int x2,int y2,
                      int x3,int y3,int color)
{
int temp_x,
    temp_y,
    new_x;

// test for h lines and v lines

if ((x1==x2 && x2==x3)  ||  (y1==y2 && y2==y3))
   return;

// sort p1,p2,p3 in ascending y order

if (y2<y1)
   {
   temp_x = x2;
   temp_y = y2;
   x2     = x1;
   y2     = y1;
   x1     = temp_x;
   y1     = temp_y;
   } // end if

// now we know that p1 and p2 are in order

if (y3<y1)
   {
   temp_x = x3;
   temp_y = y3;
   x3     = x1;
   y3     = y1;
   x1     = temp_x;
   y1     = temp_y;
   } // end if

// finally test y3 against y2

if (y3<y2)
   {
   temp_x = x3;
   temp_y = y3;
   x3     = x2;
   y3     = y2;
   x2     = temp_x;
   y2     = temp_y;

   } // end if

// do trivial rejection tests

if ( y3<poly_clip_min_y || y1>poly_clip_max_y ||
    (x1<poly_clip_min_x && x2<poly_clip_min_x && x3<poly_clip_min_x) ||
    (x1>poly_clip_max_x && x2>poly_clip_max_x && x3>poly_clip_max_x) )
   return;

// test if top of triangle is flat

if (y1==y2)
   {

   Draw_Top_Triangle(x1,y1,x2,y2,x3,y3,color);

   } // end if
else
if (y2==y3)
   {

   Draw_Bottom_Triangle(x1,y1,x2,y2,x3,y3,color);


   } // end if bottom is flat
else
   {
   // general triangle that's needs to be broken up along long edge

   new_x = x1 + (int)((float)(y2-y1)*(float)(x3-x1)/(float)(y3-y1));

   // draw each sub-triangle

   Draw_Bottom_Triangle(x1,y1,new_x,y2,x2,y2,color);

   Draw_Top_Triangle(x2,y2,new_x,y2,x3,y3,color);

   } // end else

} // end Draw_Triangle_2D

////////////////////////////////////////////////////////////////////////////////

void Build_Look_Up_Tables(void)
{
// this function builds all the look up tables for the engine

int angle;  // the current angle being computed

float rad;  // used in conversion from degrees to radians

 // generate sin/cos look up tables

for (angle=0; angle<=360; angle++)
    {

    rad = (float) (3.14159*(float)angle/(float)180);

    cos_look[angle] = (float)cos(rad);
    sin_look[angle] = (float)sin(rad);

    } // end for angle

} // end Build_Look_Up_Tables

//////////////////////////////////////////////////////////////////////////////

float Dot_Product_3D(vector_3d_ptr u,vector_3d_ptr v)
{
// this function computes the dot product of two vectors

return( (u->x * v->x) + (u->y * v->y) + (u->z * v->z));

} // end Dot_Product

//////////////////////////////////////////////////////////////////////////////

void Make_Vector_3D(point_3d_ptr init,
                    point_3d_ptr term,
                    vector_3d_ptr result)
{
// this function creates a vector from two points in 3D space

result->x = term->x - init->x;
result->y = term->y - init->y;
result->z = term->z - init->z;

} // end Make_Vector

//////////////////////////////////////////////////////////////////////////////

void Cross_Product_3D(vector_3d_ptr u,
                      vector_3d_ptr v,
                      vector_3d_ptr normal)
{
// this function computes the cross product between two vectors

normal->x =  (u->y*v->z - u->z*v->y);
normal->y = -(u->x*v->z - u->z*v->x);
normal->z =  (u->x*v->y - u->y*v->x);

} // end Cross_Product_3D

///////////////////////////////////////////////////////////////////////////////

float Vector_Mag_3D(vector_3d_ptr v)
{
// computes the magnitude of a vector

return((float)sqrt(v->x*v->x + v->y*v->y + v->z*v->z));

} // end Vector_Mag_3D

///////////////////////////////////////////////////////////////////////////////

void Mat_Print_4x4(matrix_4x4 a)
{
// this function prints out a 4x4 matrix

int row,      // looping variables
    column;

for (row=0; row<4; row++)
    {
    printf("\n");

    for (column=0; column<4; column++)
        printf("%f ",a[row][column]);

    } // end for row

printf("\n");

} // end Mat_Print_4x4

//////////////////////////////////////////////////////////////////////////////

void Mat_Print_1x4(matrix_1x4 a)
{
// this function prints out a 1x4 matrix

int column;     // looping variable

printf("\n");

for (column=0; column<4; column++)
    printf("%f ",a[column]);

printf("\n");

} // end Mat_Print_1x4

//////////////////////////////////////////////////////////////////////////////

void Mat_Mul_4x4_4x4 (matrix_4x4 a,
                      matrix_4x4 b,
                      matrix_4x4 result)
{
// this function multiplies a 4x4 by a 4x4 and stores the result in a 4x4

// first row

result[0][0]=a[0][0]*b[0][0]+a[0][1]*b[1][0]+a[0][2]*b[2][0];
result[0][1]=a[0][0]*b[0][1]+a[0][1]*b[1][1]+a[0][2]*b[2][1];
result[0][2]=a[0][0]*b[0][2]+a[0][1]*b[1][2]+a[0][2]*b[2][2];
result[0][3]=0;  // can probably get rid of this too, it's always 0

// second row

result[1][0]=a[1][0]*b[0][0]+a[1][1]*b[1][0]+a[1][2]*b[2][0];
result[1][1]=a[1][0]*b[0][1]+a[1][1]*b[1][1]+a[1][2]*b[2][1];
result[1][2]=a[1][0]*b[0][2]+a[1][1]*b[1][2]+a[1][2]*b[2][2];
result[1][3]=0; // can probably get rid of this too, it's always 0

// third row

result[2][0]=a[2][0]*b[0][0]+a[2][1]*b[1][0]+a[2][2]*b[2][0];
result[2][1]=a[2][0]*b[0][1]+a[2][1]*b[1][1]+a[2][2]*b[2][1];
result[2][2]=a[2][0]*b[0][2]+a[2][1]*b[1][2]+a[2][2]*b[2][2];
result[2][3]=0; // can probably get rid of this too, it's always 0

// fourth row

result[3][0]=a[3][0]*b[0][0]+a[3][1]*b[1][0]+a[3][2]*b[2][0]+b[3][0];
result[3][1]=a[3][0]*b[0][1]+a[3][1]*b[1][1]+a[3][2]*b[2][1]+b[3][1];
result[3][2]=a[3][0]*b[0][2]+a[3][1]*b[1][2]+a[3][2]*b[2][2]+b[3][2];
result[3][3]=1; // can probably get rid of this too, it's always 0


} // end Mat_Mul_4x4_4x4

//////////////////////////////////////////////////////////////////////////////

void Mat_Mul_1x4_4x4(matrix_1x4 a,
                     matrix_4x4 b,
                     matrix_1x4 result)
{
// this function multiplies a 1x4 by a 4x4 and stores the result in a 1x4

int index_j,    // column index
    index_k;    // row index

float sum;    // temp used to hold sum of products

// loop thru columns of b

for (index_j=0; index_j<4; index_j++)
    {

    // multiply ith row of a by jth column of b and store the sum
    // of products in the position i,j of result

    sum=0;

    for (index_k=0; index_k<4; index_k++)
        sum+=a[index_k]*b[index_k][index_j];

    // store result

    result[index_j] = sum;

    } // end for index_j

} // end Mat_Mul_1x4_4x4

//////////////////////////////////////////////////////////////////////////////

void Mat_Identity_4x4(matrix_4x4 a)
{
// this function creates a 4x4 identity matrix

memset((void *)a,0,sizeof(float)*16);

// set main diagonal to 1's

a[0][0] = a[1][1] = a[2][2] = a[3][3] = 1;

} // end Mat_Identity_4x4

/////////////////////////////////////////////////////////////////////////////

void Mat_Zero_4x4(matrix_4x4 a)
{
// this function zero's out a 4x4 matrix

memset((void *)a,0,sizeof(float)*16);

} // end Mat_Zero_4x4

/////////////////////////////////////////////////////////////////////////////

void Mat_Copy_4x4(matrix_4x4 source, matrix_4x4 destination)
{
// this function copies one 4x4 matrix to another

memcpy((void *)destination,(void *)source,sizeof(float)*16);

} // end Mat_Copy_4x4

/////////////////////////////////////////////////////////////////////////////

void Local_To_World_Object(object_ptr the_object)
{
// this function converts an objects local coordinates to world coordinates
// by translating each point in the object by the objects current position

int index;  // looping variable

// move object from local position to world position

for (index=0; index<the_object->num_vertices; index++)
    {

    the_object->vertices_world[index].x = the_object->vertices_local[index].x +
                                          the_object->world_pos.x;

    the_object->vertices_world[index].y = the_object->vertices_local[index].y +
                                          the_object->world_pos.y;

    the_object->vertices_world[index].z = the_object->vertices_local[index].z +
                                          the_object->world_pos.z;

    } // end for index

// reset visibility flags for all polys

for (index=0; index<the_object->num_polys; index++)
    {
    the_object->polys[index].visible = 1;
    the_object->polys[index].clipped = 0;
    } // end for

} // end Local_To_World_Object

/////////////////////////////////////////////////////////////////////////////

void Create_World_To_Camera(void)
{
// this function creates the global inverse transformation matrix
// used to transform world coordinate to camera coordinates

matrix_4x4 translate,   // the translation matrix

           rotate_x,    // the x,y and z rotation matrices
           rotate_y,
           rotate_z,
           result_1,
           result_2;

int active_axes=0;

// create identity matrices

Mat_Identity_4x4(translate);

// make a translation matrix based on the inverse of the viewpoint

translate[3][0] = -view_point.x;
translate[3][1] = -view_point.y;
translate[3][2] = -view_point.z;

// test if there is any X rotation in view angles

if (view_angle.ang_x)
   {
   Mat_Identity_4x4(rotate_x);

   // x matrix

   rotate_x[1][1] =  ( cos_look[view_angle.ang_x]);
   rotate_x[1][2] = -( sin_look[view_angle.ang_x]);
   rotate_x[2][1] = -(-sin_look[view_angle.ang_x]);
   rotate_x[2][2] =  ( cos_look[view_angle.ang_x]);

   active_axes += 1;

   } // end if

if (view_angle.ang_y)
   {
   Mat_Identity_4x4(rotate_y);

   // y matrix

   rotate_y[0][0] =  ( cos_look[view_angle.ang_y]);
   rotate_y[0][2] = -(-sin_look[view_angle.ang_y]);
   rotate_y[2][0] = -( sin_look[view_angle.ang_y]);
   rotate_y[2][2] =  ( cos_look[view_angle.ang_y]);

   active_axes += 2;

   } // end if


if (view_angle.ang_z)
   {
   Mat_Identity_4x4(rotate_z);

   // z matrix

   rotate_z[0][0] =  ( cos_look[view_angle.ang_z]);
   rotate_z[0][1] = -( sin_look[view_angle.ang_z]);
   rotate_z[1][0] = -(-sin_look[view_angle.ang_z]);
   rotate_z[1][1] =  ( cos_look[view_angle.ang_z]);

   active_axes += 4;

   } // end if

// multiply all the matrices together to obtain a final world to camera
// viewing transformation matrix i.e.
// translation * rotate_x * rotate_y * rotate_z, however, only, multiply
// matrices that can possible add to the final view

switch(active_axes)
      {

      case 0: // translation only
           {
           Mat_Copy_4x4(translate,global_view);
           } break;

      case 1: // translation and X axis
           {
           // since only a single axis is active manually set up the matrix

           // Mat_Mul_4x4_4x4(translate,rotate_x,global_view);

           // manually create matrix using knowledge that final product is
           // of the form

           // | 1            0            0          0|
           // | 0            c            -s         0|
           // | 0            s            c          0|
           // |(-tx)       (-ty*c-tz*s) (ty*s-tz*c)  1|

           Mat_Copy_4x4(rotate_x,global_view);

           // now copy last row into global_view


           global_view[3][0] = (-view_point.x);

           global_view[3][1] = (-view_point.y*cos_look[view_angle.ang_y] -
                                 view_point.z*sin_look[view_angle.ang_y]);

           global_view[3][2] = (view_point.y*sin_look[view_angle.ang_y] -
                                view_point.z*cos_look[view_angle.ang_y]);


           } break;


      case 2: // translation and Y axis
           {
           // Mat_Mul_4x4_4x4(translate,rotate_y,global_view);

           // manually create matrix using knowledge that final product is
           // of the form

           // | c            0            s          0|
           // | 0            1            0          0|
           // | -s           0            c          0|
           // |(tx*c+tz*s) (-ty)        (-tx*s-tz*c) 1|

           Mat_Copy_4x4(rotate_y,global_view);

           // now copy last row into global_view

           global_view[3][0] = (-view_point.x*cos_look[view_angle.ang_y] +
                                 view_point.z*sin_look[view_angle.ang_y]);

           global_view[3][1] = (-view_point.y);

           global_view[3][2] = (-view_point.x*sin_look[view_angle.ang_y] -
                                 view_point.z*cos_look[view_angle.ang_y]);


           } break;


      case 3: // translation and X and Y
           {
           Mat_Mul_4x4_4x4(translate,rotate_x,result_1);
           Mat_Mul_4x4_4x4(result_1,rotate_y,global_view);
           } break;


      case 4: // translation and Z
           {
            // Mat_Mul_4x4_4x4(translate,rotate_z,global_view);

           // manually create matrix using knowledge that final product is
           // of the form

           // | c            -s           0          0|
           // | s            c            0          0|
           // | 0            s            c          0|
           // |(-tx*c-ty*s) (tx*s-ty*c)  (-tz)       1|

           Mat_Copy_4x4(rotate_z,global_view);

           // now copy last row into global_view


           global_view[3][0] = (-view_point.x*cos_look[view_angle.ang_z] -
                                 view_point.y*sin_look[view_angle.ang_z]);

           global_view[3][1] = (view_point.x*sin_look[view_angle.ang_z] -
                                view_point.y*cos_look[view_angle.ang_z]);


           global_view[3][2] = (-view_point.z);

           } break;


      case 5: // translation and X and Z
           {
           Mat_Mul_4x4_4x4(translate,rotate_x,result_1);
           Mat_Mul_4x4_4x4(result_1,rotate_z,global_view);
           } break;

      case 6: // translation and Y and Z
           {
           Mat_Mul_4x4_4x4(translate,rotate_y,result_1);
           Mat_Mul_4x4_4x4(result_1,rotate_z,global_view);
           } break;

      case 7: // translation and X and Y and Z

           {

           Mat_Mul_4x4_4x4(translate,rotate_x,result_1);
           Mat_Mul_4x4_4x4(result_1,rotate_y,result_2);
           Mat_Mul_4x4_4x4(result_2,rotate_z,global_view);

           } break;

      default:break;

      } // end switch

} // end Create_World_To_Camera

////////////////////////////////////////////////////////////////////////////////

void World_To_Camera_Object(object_ptr the_object)
{
// this function converts an objects world coordinates to camera coordinates
// by multiplying each point of the object by the inverse viewing transformation
// matrix which is generated by concatenating the inverse of the view position
// and the view angles the result of which is in global_view

int index; // looping variable

int active_axes = 0;

if (view_angle.ang_x)
   active_axes+=1;

if (view_angle.ang_y)
   active_axes+=2;

if (view_angle.ang_z)
   active_axes+=4;

// based on active angles only compute what's neccessary

switch(active_axes)
      {
      case 0:   // T-1
           {

           for (index=0; index<=the_object->num_vertices; index++)
               {
               // multiply the point by the viewing transformation matrix

               // x component

               the_object->vertices_camera[index].x =

                             the_object->vertices_world[index].x +
                                                                   global_view[3][0];
               // y component

               the_object->vertices_camera[index].y =

                             the_object->vertices_world[index].y +
                                                                   global_view[3][1];
               // z component

               the_object->vertices_camera[index].z =

                             the_object->vertices_world[index].z +
                                                                   global_view[3][2];
               } // end for index

           } break;

      case 1:   // T-1 * Rx-1
           {
           for (index=0; index<=the_object->num_vertices; index++)
               {
               // multiply the point by the viewing transformation matrix
               // x component

               the_object->vertices_camera[index].x =

                             the_object->vertices_world[index].x +
                                                                   global_view[3][0];
               // y component

               the_object->vertices_camera[index].y =

                             the_object->vertices_world[index].y * global_view[1][1] +
                             the_object->vertices_world[index].z * global_view[2][1] +
                                                                   global_view[3][1];
               // z component

               the_object->vertices_camera[index].z =

                             the_object->vertices_world[index].y * global_view[1][2] +
                             the_object->vertices_world[index].z * global_view[2][2] +
                                                                   global_view[3][2];
               } // end for index



           } break;

      case 2:  // T-1 * Ry-1 , this is the standard rotation in a plane
           {

           for (index=0; index<=the_object->num_vertices; index++)
               {
               // multiply the point by the viewing transformation matrix

               // x component
               the_object->vertices_camera[index].x =

                             the_object->vertices_world[index].x * global_view[0][0] +
                             the_object->vertices_world[index].z * global_view[2][0] +
                                                                   global_view[3][0];
               // y component
               the_object->vertices_camera[index].y =

                             the_object->vertices_world[index].y +
                                                                   global_view[3][1];
               // z component
               the_object->vertices_camera[index].z =

                             the_object->vertices_world[index].x * global_view[0][2] +
                             the_object->vertices_world[index].z * global_view[2][2] +
                                                                   global_view[3][2];
               } // end for index



           } break;

      case 4:   // T-1 * Rz-1
           {

           for (index=0; index<=the_object->num_vertices; index++)
               {
               // multiply the point by the viewing transformation matrix

               // x component

               the_object->vertices_camera[index].x =

                             the_object->vertices_world[index].x * global_view[0][0] +
                             the_object->vertices_world[index].y * global_view[1][0] +
                                                                   global_view[3][0];
               // y component

               the_object->vertices_camera[index].y =

                             the_object->vertices_world[index].x * global_view[0][1] +
                             the_object->vertices_world[index].y * global_view[1][1] +
                                                                   global_view[3][1];
               // z component

               the_object->vertices_camera[index].z =

                             the_object->vertices_world[index].z +
                                                                   global_view[3][2];
               } // end for index


           } break;


      // these can all be optimized by pre-computing the form of the world
      // to camera matrix and using the same logic as the cases above

      case 3:  // T-1 * Rx-1 * Ry-1
      case 5:  // T-1 * Rx-1 * Rz-1
      case 6:  // T-1 * Ry-1 * Rz-1
      case 7:  // T-1 * Rx-1 * Ry-1 * Rz-1
           {

           for (index=0; index<=the_object->num_vertices; index++)
               {
               // multiply the point by the viewing transformation matrix

               // x component

               the_object->vertices_camera[index].x =

                             the_object->vertices_world[index].x * global_view[0][0] +
                             the_object->vertices_world[index].y * global_view[1][0] +
                             the_object->vertices_world[index].z * global_view[2][0] +
                                                                   global_view[3][0];
               // y component

               the_object->vertices_camera[index].y =

                             the_object->vertices_world[index].x * global_view[0][1] +
                             the_object->vertices_world[index].y * global_view[1][1] +
                             the_object->vertices_world[index].z * global_view[2][1] +
                                                                   global_view[3][1];
               // z component

               the_object->vertices_camera[index].z =

                             the_object->vertices_world[index].x * global_view[0][2] +
                             the_object->vertices_world[index].y * global_view[1][2] +
                             the_object->vertices_world[index].z * global_view[2][2] +
                                                                   global_view[3][2];
               } // end for index



           } break;

      default: break;

      } // end switch

} // end World_To_Camera_Object

////////////////////////////////////////////////////////////////////////////

void Rotate_Object(object_ptr the_object,int angle_x,int angle_y,int angle_z)
{
// this function rotates an object relative to it's own local coordinate system
// and allows simultaneous rotations

int index,       //  looping variable
    product=0;   // used to determine which matrices need multiplying

matrix_4x4 rotate_x,    // the x,y and z rotation matrices
           rotate_y,
           rotate_z,
           rotate,      // the final rotation matrix
           temp;        // temporary working matrix

float temp_x, // used to hold intermediate results during rotation
      temp_y,
      temp_z;

// test if we need to rotate at all

if (angle_x==0 && angle_y==0 && angle_z==0)
   return;

// create identity matrix

Mat_Identity_4x4(rotate);

// figure out which axes are active

if (angle_x)
   product+=4;

if (angle_y)
   product+=2;

if (angle_z)
   product+=1;

// compute final rotation matrix and perform rotation all in one!

switch(product)
      {

      case 1: // final matrix = z
           {
           // set up matrix

           rotate[0][0] = ( cos_look[angle_z]);
           rotate[0][1] = ( sin_look[angle_z]);
           rotate[1][0] = (-sin_look[angle_z]);
           rotate[1][1] = ( cos_look[angle_z]);

           // matrix is of the form

           // | cos   sin  0     0 |
           // | -sin  cos  0     0 |
           // | 0     0    1     0 |
           // | 0     0    0     1 |

           // hence we can remove a number of multiplications during the
           // computations of x,y and z since many times each variable
           // isn't a function of the other two


           // perform rotation

           // now multiply each point in object by transformation matrix

           for (index=0; index<the_object->num_vertices; index++)
               {

               // x component

               temp_x = the_object->vertices_local[index].x * rotate[0][0] +
                        the_object->vertices_local[index].y * rotate[1][0];
                       // the_object->vertices_local[index].z * rotate[2][0];

               // y component

               temp_y = the_object->vertices_local[index].x * rotate[0][1] +
                        the_object->vertices_local[index].y * rotate[1][1];
                        // the_object->vertices_local[index].z * rotate[2][1];

               // z component

               temp_z =

                        the_object->vertices_local[index].z;

               // store rotated point back into local array

               the_object->vertices_local[index].x = temp_x;
               the_object->vertices_local[index].y = temp_y;
               the_object->vertices_local[index].z = temp_z;

               } // end for index


           } break;

      case 2: // final matrix = y
           {

           rotate[0][0] = ( cos_look[angle_y]);
           rotate[0][2] = (-sin_look[angle_y]);
           rotate[2][0] = ( sin_look[angle_y]);
           rotate[2][2] = ( cos_look[angle_y]);

           // matrix is of the form

           // | cos   0    -sin  0 |
           // | 0     1    0     0 |
           // | sin   0    cos   0 |
           // | 0     0    0     1 |

           // hence we can remove a number of multiplications during the
           // computations of x,y and z since many times each variable
           // isn't a function of the other two


           // now multiply each point in object by transformation matrix

           for (index=0; index<the_object->num_vertices; index++)
               {

               // x component

               temp_x = the_object->vertices_local[index].x * rotate[0][0] +
                        // the_object->vertices_local[index].y * rotate[1][0] +
                        the_object->vertices_local[index].z * rotate[2][0];

               // y component

               temp_y =
                        the_object->vertices_local[index].y;

               // z component

               temp_z = the_object->vertices_local[index].x * rotate[0][2] +
                        // the_object->vertices_local[index].y * rotate[1][2] +
                        the_object->vertices_local[index].z * rotate[2][2];

               // store rotated point back into local array

               the_object->vertices_local[index].x = temp_x;
               the_object->vertices_local[index].y = temp_y;
               the_object->vertices_local[index].z = temp_z;

               } // end for index

           } break;

      case 3: // final matrix = y*z
           {

           // take advantage of the fact that the product of Ry*Rz is
           //
           // | (cos angy)*(cos angz)  (cos angy)*(sin angz) -sin angy 0|
           // | -sin angz              cos angz              0         0|
           // | (sin angy)*(cos angz)  (sin angy)*(sin angz) cos angy  0|
           // | 0                      0                     0         1|

           // also notice the 0 in the 3rd column, we can use it to get
           // rid of one multiplication in the for loop below per point

           rotate[0][0] = cos_look[angle_y]*cos_look[angle_z];
           rotate[0][1] = cos_look[angle_y]*sin_look[angle_z];
           rotate[0][2] = -sin_look[angle_y];

           rotate[1][0] = -sin_look[angle_z];
           rotate[1][1] = cos_look[angle_z];

           rotate[2][0] = sin_look[angle_y]*cos_look[angle_z];
           rotate[2][1] = sin_look[angle_y]*sin_look[angle_z];
           rotate[2][2] = cos_look[angle_y];

           // now multiply each point in object by transformation matrix

           for (index=0; index<the_object->num_vertices; index++)
               {

               // x component

               temp_x = the_object->vertices_local[index].x * rotate[0][0] +
                        the_object->vertices_local[index].y * rotate[1][0] +
                        the_object->vertices_local[index].z * rotate[2][0];

               // y component

               temp_y = the_object->vertices_local[index].x * rotate[0][1] +
                        the_object->vertices_local[index].y * rotate[1][1] +
                        the_object->vertices_local[index].z * rotate[2][1];

               // z component

               temp_z = the_object->vertices_local[index].x * rotate[0][2] +
                        // the_object->vertices_local[index].y * rotate[1][2] +
                        the_object->vertices_local[index].z * rotate[2][2];

               // store rotated point back into local array

               the_object->vertices_local[index].x = temp_x;
               the_object->vertices_local[index].y = temp_y;
               the_object->vertices_local[index].z = temp_z;

               } // end for index

           } break;

      case 4: // final matrix = x
           {

           rotate[1][1] = ( cos_look[angle_x]);
           rotate[1][2] = ( sin_look[angle_x]);
           rotate[2][1] = (-sin_look[angle_x]);
           rotate[2][2] = ( cos_look[angle_x]);

           // matrix is of the form

           // | 1     s    0     0 |
           // | 0     cos  sin   0 |
           // | 0    -sin  cos   0 |
           // | 0     0    0     1 |

           // hence we can remove a number of multiplications during the
           // computations of x,y and z since many times each variable
           // isn't a function of the other two




           // now multiply each point in object by transformation matrix

           for (index=0; index<the_object->num_vertices; index++)
               {

               // x component

               temp_x =

                        the_object->vertices_local[index].x;

               // y component

               temp_y = // the_object->vertices_local[index].x * rotate[0][1] +
                        the_object->vertices_local[index].y * rotate[1][1] +
                        the_object->vertices_local[index].z * rotate[2][1];

               // z component

               temp_z = // the_object->vertices_local[index].x * rotate[0][2] +
                        the_object->vertices_local[index].y * rotate[1][2] +
                        the_object->vertices_local[index].z * rotate[2][2];

               // store rotated point back into local array

               the_object->vertices_local[index].x = temp_x;
               the_object->vertices_local[index].y = temp_y;
               the_object->vertices_local[index].z = temp_z;

               } // end for index

           } break;

      case 5: // final matrix = x*z
           {

           // take advantage of the fact that the product of Rx*Rz is
           //
           // | cos angz                sin angz               0         0|
           // | -(cos angx)*(sin angz)  (cos angx)*(cos angz)  sin angx  0|
           // | (sin angx)*(sin angz)  -(sin angx)*(cos angz)  cos angx  0|
           // | 0                       0                      0         1|

           // also notice the 0 in the 3rd column, we can use it to get
           // rid of one multiplication in the for loop below per point


           rotate[0][0] = cos_look[angle_z];
           rotate[0][1] = sin_look[angle_z];

           rotate[1][0] = -cos_look[angle_x]*sin_look[angle_z];
           rotate[1][1] = cos_look[angle_x]*cos_look[angle_z];
           rotate[1][2] = sin_look[angle_x];

           rotate[2][0] = sin_look[angle_x]*sin_look[angle_z];
           rotate[2][1] = -sin_look[angle_x]*cos_look[angle_z];
           rotate[2][2] = cos_look[angle_x];

           // now multiply each point in object by transformation matrix

           for (index=0; index<the_object->num_vertices; index++)
               {

               // x component

               temp_x = the_object->vertices_local[index].x * rotate[0][0] +
                        the_object->vertices_local[index].y * rotate[1][0] +
                        the_object->vertices_local[index].z * rotate[2][0];

               // y component

               temp_y = the_object->vertices_local[index].x * rotate[0][1] +
                        the_object->vertices_local[index].y * rotate[1][1] +
                        the_object->vertices_local[index].z * rotate[2][1];

               // z component

               temp_z = // the_object->vertices_local[index].x * rotate[0][2] +
                        the_object->vertices_local[index].y * rotate[1][2] +
                        the_object->vertices_local[index].z * rotate[2][2];

               // store rotated point back into local array

               the_object->vertices_local[index].x = temp_x;
               the_object->vertices_local[index].y = temp_y;
               the_object->vertices_local[index].z = temp_z;

               } // end for index

           } break;

      case 6: // final matrix = x*y
           {

           // take advantage of the fact that the product of Rx*Ry is
           //
           // | cos angy                0          -sin angy             0|
           // | (sin angx)*(sin angy)   cos angx   (sin angx)*(cos angy) 0|
           // | (cos angx)*(sin angy)   -sin angx  (cos angx)*(cos angy) 0|
           // | 0                       0          0                     1|

           // also notice the 0 in the 2nd column, we can use it to get
           // rid of one multiplication in the for loop below per point

           rotate[0][0] = cos_look[angle_y];
           rotate[0][2] = -sin_look[angle_y];

           rotate[1][0] = sin_look[angle_x]*sin_look[angle_y];
           rotate[1][1] = cos_look[angle_x];
           rotate[1][2] = sin_look[angle_x]*cos_look[angle_y];

           rotate[2][0] = cos_look[angle_x]*sin_look[angle_y];
           rotate[2][1] = -sin_look[angle_x];
           rotate[2][2] = cos_look[angle_x]*cos_look[angle_y];

           // now multiply each point in object by transformation matrix

           for (index=0; index<the_object->num_vertices; index++)
               {

               // x component

               temp_x = the_object->vertices_local[index].x * rotate[0][0] +
                        the_object->vertices_local[index].y * rotate[1][0] +
                        the_object->vertices_local[index].z * rotate[2][0];

               // y component

               temp_y = //the_object->vertices_local[index].x * rotate[0][1] +
                        the_object->vertices_local[index].y * rotate[1][1] +
                        the_object->vertices_local[index].z * rotate[2][1];

               // z component

               temp_z = the_object->vertices_local[index].x * rotate[0][2] +
                        the_object->vertices_local[index].y * rotate[1][2] +
                        the_object->vertices_local[index].z * rotate[2][2];

               // store rotated point back into local array

               the_object->vertices_local[index].x = temp_x;
               the_object->vertices_local[index].y = temp_y;
               the_object->vertices_local[index].z = temp_z;

               } // end for index

           } break;

      case 7: // final matrix = x*y*z, do it the hard way
           {
           Mat_Identity_4x4(rotate_x);

           rotate_x[1][1] = ( cos_look[angle_x]);
           rotate_x[1][2] = ( sin_look[angle_x]);
           rotate_x[2][1] = (-sin_look[angle_x]);
           rotate_x[2][2] = ( cos_look[angle_x]);

           Mat_Identity_4x4(rotate_y);

           rotate_y[0][0] = ( cos_look[angle_y]);
           rotate_y[0][2] = (-sin_look[angle_y]);
           rotate_y[2][0] = ( sin_look[angle_y]);
           rotate_y[2][2] = ( cos_look[angle_y]);

           Mat_Identity_4x4(rotate_z);

           rotate_z[0][0] = ( cos_look[angle_z]);
           rotate_z[0][1] = ( sin_look[angle_z]);
           rotate_z[1][0] = (-sin_look[angle_z]);
           rotate_z[1][1] = ( cos_look[angle_z]);

           Mat_Mul_4x4_4x4(rotate_x,rotate_y,temp);
           Mat_Mul_4x4_4x4(temp,rotate_z,rotate);

           // now multiply each point in object by transformation matrix

           for (index=0; index<the_object->num_vertices; index++)
               {

               // x component

               temp_x = the_object->vertices_local[index].x * rotate[0][0] +
                        the_object->vertices_local[index].y * rotate[1][0] +
                        the_object->vertices_local[index].z * rotate[2][0];

               // y component

               temp_y = the_object->vertices_local[index].x * rotate[0][1] +
                        the_object->vertices_local[index].y * rotate[1][1] +
                        the_object->vertices_local[index].z * rotate[2][1];

               // z component

               temp_z = the_object->vertices_local[index].x * rotate[0][2] +
                        the_object->vertices_local[index].y * rotate[1][2] +
                        the_object->vertices_local[index].z * rotate[2][2];

               // store rotated point back into local array

               the_object->vertices_local[index].x = temp_x;
               the_object->vertices_local[index].y = temp_y;
               the_object->vertices_local[index].z = temp_z;

               } // end for index

           } break;

      default:break;

      } // end switch

} // end Rotate_Object

////////////////////////////////////////////////////////////////////////////

void Position_Object(object_ptr the_object,int x,int y,int z)
{
// this function positions an object in the world

the_object->world_pos.x = x;
the_object->world_pos.y = y;
the_object->world_pos.z = z;

} // end Position_Object

////////////////////////////////////////////////////////////////////////////

void Translate_Object(object_ptr the_object,int x_trans,int y_trans,int z_trans)
{
// this function translates an object relative to it's own local
// coordinate system

the_object->world_pos.x+=x_trans;
the_object->world_pos.y+=y_trans;
the_object->world_pos.z+=z_trans;

} // end Translate_Object

/////////////////////////////////////////////////////////////////////////////

void Scale_Object(object_ptr the_object,float scale_factor)
{
// this function scales an object relative to it's own local coordinate system
// equally in x,y and z

int curr_poly,    // the current polygon being processed
    curr_vertex;  // the current vertex being processed

float scale_2;    // holds the sqaure of the scaling factor, needed to
                  // resize the surface normal for lighting calculations

// multiply each vertex in the object definition by the scaling factor

for (curr_vertex=0; curr_vertex<the_object->num_vertices; curr_vertex++)
    {

    the_object->vertices_local[curr_vertex].x*=scale_factor;
    the_object->vertices_local[curr_vertex].y*=scale_factor;
    the_object->vertices_local[curr_vertex].z*=scale_factor;

    } // end for curr_vertex

// compute scaling factor squared

scale_2 = scale_factor*scale_factor;

// now scale all pre-computed normals

for (curr_poly=0; curr_poly<the_object->num_polys; curr_poly++)
    the_object->polys[curr_poly].normal_length*=scale_2;

// finally scale the radius up

the_object->radius*=scale_factor;

} // end Scale_Object

/////////////////////////////////////////////////////////////////////////////

int Objects_Collide(object_ptr object_1, object_ptr object_2)
{
// this function tests if the bounding spheres of two objects overlaps
// if a more accurate test is needed then polygons should be tested against
// polygons. note the function uses the fact that if x > y then x^2 > y^2
// to avoid using square roots. Finally, the function might be altered
// so that the bounding spheres are shrank to make sure that the collision
// is "solid"/ finally, soft and hard collisions are both detected

float dx,dy,dz,           // deltas in x,y and z
      radius_1,radius_2,  // radi of each object
      distance;           // distance between object centers

// compute delta's

dx = (object_1->world_pos.x - object_2->world_pos.x);
dy = (object_1->world_pos.y - object_2->world_pos.y);
dz = (object_1->world_pos.z - object_2->world_pos.z);

// compute length

distance = dx*dx + dy*dy + dz*dx;

// compute radius of each object squared

radius_1 = object_1->radius*object_1->radius;
radius_2 = object_2->radius*object_2->radius;

// test if distance is smaller than of radi

if (distance<radius_1 || distance <radius_2)
   return(HARD_COLLISION); // hard collision
else
if (distance < radius_1+radius_2)
   return(SOFT_COLLISION); // soft collision
else
   return(NO_COLLISION);

} // end Objects_Collide

/////////////////////////////////////////////////////////////////////////////

char *PLG_Get_Line(char *string, int max_length, FILE *fp)
{
// this function gets a line from a PLG file and strips comments
// just pretend it's a black box!

char buffer[80]; // temporary string storage

int length,      // length of line read
    index=0,     // looping variables
    index_2=0,
    parsed=0;    // has the current input line been parsed

// get the next line of input, make sure there is something on the line

while(1)
     {
     // get the line

     if (!fgets(buffer,max_length,fp))
        return(NULL);

     // get length of line

     length = strlen(buffer);

     // kill the carriage return

     buffer[length-1] = 0;

     // reset index

     index = 0;

     // eat leading white space

     while(buffer[index]==' ')
           index++;

     // read line into buffer, if "#" arrives in data stream then disregard
     // rest of line

     parsed=0;
     index_2=0;

     while(!parsed)
          {

          if (buffer[index]!='#' && buffer[index]!=';')
             {
             // insert character into output string

             string[index_2] = buffer[index];

             // test if this is a null terminator

             if (string[index_2]==0)
                parsed=1;

             // move to next character

             index++;
             index_2++;

             } // end if not comment
          else
             {
             // insert a null termination since this is the end of the
             // string for all intense purposes

             string[index_2] = 0;

             parsed=1;

             } // end else

          } // end while parsing string

          // make sure we got a string and not a blank line

          if (strlen(string))
             return(string);

     } // end while

} // end PLG_Get_Line

/////////////////////////////////////////////////////////////////////////////

int PLG_Load_Object(object_ptr the_object,char *filename,float scale)
{
// this function loads an object off disk and allows it to be scaled, the files
// are in a slightly enhanced PLG format that allows polygons to be defined
// as one or two sided, this helps the hidden surface removal system. This
// extra functionality has been encoded in the 2nd nibble from the left of the
// color descriptor

FILE *fp; // disk file

static int id_number = 0; // used to set object id's

char buffer[80],          // holds input string
     object_name[32],     // name of 3-D object
     *token;              // current parsing token


unsigned int total_vertices,    // total vertices in object
             total_polys,       // total polygons per object
             num_vertices,      // number of vertices on a polygon
             color_des,         // the color descriptor of a polygon
             logical_color,     // the final color of polygon
             shading,           // the type of shading used on polygon
             two_sided,         // flags if poly has two sides
             index,             // looping variables
             index_2,
             vertex_num,        // vertex numbers
             vertex_0,
             vertex_1,
             vertex_2;

float x,y,z;                    // a single vertex

vector_3d u,v,normal;           // working vectors

// open the disk file

if ((fp=fopen(filename,"r"))==NULL)
   {
   printf("\nCouldn't open file %s",filename);
   return(0);

   } // end if

// first we are looking for the header line that has the object name and
// the number of vertices and polygons

if (!PLG_Get_Line(buffer, 80,fp))
   {
   printf("\nError with PLG file %s",filename);
   fclose(fp);
   return(0);
   } // end if error

// extract object name and number of vertices and polygons

sscanf(buffer,"%s %d %d",object_name, &total_vertices, &total_polys);

// set proper fields in object

the_object->num_vertices = total_vertices;
the_object->num_polys    = total_polys;
the_object->state        = 1;

the_object->world_pos.x  = 0;
the_object->world_pos.y  = 0;
the_object->world_pos.z  = 0;

// set id number, maybe later also add the name of object in the
// structure???

the_object->id = id_number++;

// based on number of vertices, read vertex list into object

for (index=0; index<total_vertices; index++)
    {

    // read in vertex

    if (!PLG_Get_Line(buffer, 80,fp))
       {
       printf("\nError with PLG file %s",filename);
       fclose(fp);
       return(0);
       } // end if error

    sscanf(buffer,"%f %f %f",&x,&y,&z);

    // insert vertex into object

    the_object->vertices_local[index].x = x*scale;
    the_object->vertices_local[index].y = y*scale;
    the_object->vertices_local[index].z = z*scale;

    } // end for index

// now read in polygon list

for (index=0; index<total_polys; index++)
    {
    // read in color and number of vertices for next polygon

    if (!PLG_Get_Line(buffer, 80,fp))
       {
       printf("\nError with PLG file %s",filename);
       fclose(fp);
       return(0);
       } // end if error

    // intialize token getter and get first token which is color descriptor

    if (!(token = strtok(buffer," ")) )
       {
       printf("\nError with PLG file %s",filename);
       fclose(fp);
       return(0);
       } // end if problem

    // test if number is hexadecimal

    if (token[0]=='0' && (token[1]=='x' || token[1]=='X'))
       {

       sscanf(&token[2],"%x",&color_des);

       } // end if hex color specifier
    else
       {

       color_des = atoi(token);

       } // end if decimal

    // extract base color and type of shading

    logical_color = color_des & 0x00ff;
    shading       = color_des >> 12;
    two_sided     = ((color_des >> 8) & 0x0f);

    // read number of vertices in polygon

    if (!(token = strtok(NULL," ")))
       {
       printf("\nError with PLG file %s",filename);
       fclose(fp);
       return(0);
       } // end if problem

    if ((num_vertices = atoi(token))<=0)
       {
       printf("\nError with PLG file (number of vertices) %s",filename);
       fclose(fp);
       return(0);
       } // end if no vertices or error

    // set fields in polygon structure

    the_object->polys[index].num_points = num_vertices;
    the_object->polys[index].color      = logical_color;
    the_object->polys[index].shading    = shading;
    the_object->polys[index].two_sided  = two_sided;
    the_object->polys[index].visible    = 1;
    the_object->polys[index].clipped    = 0;
    the_object->polys[index].active     = 1;

    // now read in polygon vertice list

    for (index_2=0; index_2<num_vertices; index_2++)
        {

        // read in next vertex number

		  if (!(token = strtok(NULL," ")) )
           {
           printf("\nError with PLG file %s",filename);
           fclose(fp);
           return(0);
           } // end if problem

        vertex_num = atoi(token);

        // insert vertex number into polygon

        the_object->polys[index].vertex_list[index_2] = vertex_num;

        } // end for index_2

    // compute length of the two co-planer edges of the polygon, since they
    // will be used in the computation of the dot-product later

    vertex_0 = the_object->polys[index].vertex_list[0];
    vertex_1 = the_object->polys[index].vertex_list[1];
    vertex_2 = the_object->polys[index].vertex_list[2];

    // the vector u = vo->v1

    Make_Vector_3D((point_3d_ptr)&the_object->vertices_local[vertex_0],
                   (point_3d_ptr)&the_object->vertices_local[vertex_1],
                   (vector_3d_ptr)&u);

    // the vector v = vo->v2

    Make_Vector_3D((point_3d_ptr)&the_object->vertices_local[vertex_0],
                   (point_3d_ptr)&the_object->vertices_local[vertex_2],
                   (vector_3d_ptr)&v);

    Cross_Product_3D((vector_3d_ptr)&v,
                     (vector_3d_ptr)&u,
                     (vector_3d_ptr)&normal);


    // compute magnitude of normal, take its inverse and multiply it by
    // 15, this will change the shading calculation of 15*dp/normal into
    // dp*normal_length, removing one division

    the_object->polys[index].normal_length = 15.0/Vector_Mag_3D((vector_3d_ptr)&normal);

    } // end for index

// close the file

fclose(fp);

// compute object radius

Compute_Object_Radius(the_object);

// return success

return(1);

} // end PLG_Load_Object

///////////////////////////////////////////////////////////////////////////////

float Compute_Object_Radius(object_ptr the_object)
{

// this function computes maximum radius of object, maybe a better method would
// use average radius? Note that this functiopn shouldn't be used during
// runtime but when an object is created

float new_radius,   // used in average radius calculation of object
      x,y,z;        // a single vertex

int index;          // looping variable

// reset object radius

the_object->radius=0;

for (index=0; index<the_object->num_vertices; index++)
    {

    x = the_object->vertices_local[index].x;
    y = the_object->vertices_local[index].y;
    z = the_object->vertices_local[index].z;

    // compute distance to point

    new_radius = (float)sqrt(x*x + y*y + z*z);

    // is this radius bigger than last?

    if (new_radius > the_object->radius)
       the_object->radius = new_radius;

    } // end for index

// return radius just in case

return(the_object->radius);

} // end Compute_Object_Radius

/////////////////////////////////////////////////////////////////////////////

void Clip_Object_3D(object_ptr the_object, int mode)
{
// this function clips an object in camera coordinates against the 3D viewing
// volume. the function has two modes of operation. In CLIP_Z_MODE the
// function performs only a simple z extend clip with the near and far clipping
// planes. In CLIP_XYZ_MODE mode the function performs a full 3-D clip

int curr_poly;    // the current polygon being processed

float x1,y1,z1,
      x2,y2,z2,
      x3,y3,z3,
      x4,y4,z4,   // working variables used to hold vertices

      x1_compare, // used to hold clipping points on x and y
      y1_compare,
      x2_compare,
      y2_compare,
      x3_compare,
      y3_compare,
      x4_compare,
      y4_compare,
      fov_width,  // width and height of projected viewing plane
      fov_height; // used to speed up computations, since it's constant
                  // for any frame


// test if trivial z clipping is being requested

if (mode==CLIP_Z_MODE)
   {
   // attempt to clip each polygon against viewing volume

   for (curr_poly=0; curr_poly<the_object->num_polys; curr_poly++)
       {

       // extract z components

       z1=the_object->vertices_camera[the_object->polys[curr_poly].vertex_list[0]].z;
       z2=the_object->vertices_camera[the_object->polys[curr_poly].vertex_list[1]].z;
       z3=the_object->vertices_camera[the_object->polys[curr_poly].vertex_list[2]].z;

       // test if this is a quad

       if (the_object->polys[curr_poly].num_points==4)
          {
          // extract 4th z component

          z4=the_object->vertices_camera[the_object->polys[curr_poly].vertex_list[3]].z;

          } // end if quad
       else
         z4=z3;

       // perform near and far z clipping test

       if ( (z1<clip_near_z && z2<clip_near_z && z3<clip_near_z && z4<clip_near_z) ||
            (z1>clip_far_z && z2>clip_far_z && z3>clip_far_z && z4>clip_far_z) )
          {
          // set clipped flag

          the_object->polys[curr_poly].clipped=1;

          } // end if clipped

       } // end for curr_poly

    } // end if CLIP_Z_MODE
else
   {
   // CLIP_XYZ_MODE, perform full 3D viewing volume clip

   // compute dimensions of clipping extents at current viewing distance

   fov_width  = ((float)HALF_SCREEN_WIDTH/viewing_distance);
   fov_height = ((float)HALF_SCREEN_HEIGHT/viewing_distance);


   // process each polygon

   for (curr_poly=0; curr_poly<the_object->num_polys; curr_poly++)
       {

       // extract x,y and z components

       x1=the_object->vertices_camera[the_object->polys[curr_poly].vertex_list[0]].x;
       y1=the_object->vertices_camera[the_object->polys[curr_poly].vertex_list[0]].y;
       z1=the_object->vertices_camera[the_object->polys[curr_poly].vertex_list[0]].z;

       x2=the_object->vertices_camera[the_object->polys[curr_poly].vertex_list[1]].x;
       y2=the_object->vertices_camera[the_object->polys[curr_poly].vertex_list[1]].y;
       z2=the_object->vertices_camera[the_object->polys[curr_poly].vertex_list[1]].z;

       x3=the_object->vertices_camera[the_object->polys[curr_poly].vertex_list[2]].x;
       y3=the_object->vertices_camera[the_object->polys[curr_poly].vertex_list[2]].y;
       z3=the_object->vertices_camera[the_object->polys[curr_poly].vertex_list[2]].z;

       // test if this is a quad

       if (the_object->polys[curr_poly].num_points==4)
          {
          // extract 4th vertex

          x4=the_object->vertices_camera[the_object->polys[curr_poly].vertex_list[3]].x;
          y4=the_object->vertices_camera[the_object->polys[curr_poly].vertex_list[3]].y;
          z4=the_object->vertices_camera[the_object->polys[curr_poly].vertex_list[3]].z;

          // do clipping tests

          // perform near and far z clipping test first

          if (!((z1>clip_near_z || z2>clip_near_z || z3>clip_near_z || z4>clip_near_z) &&
                (z1<clip_far_z || z2<clip_far_z || z3<clip_far_z || z4<clip_far_z)) )
             {
             // set clipped flag

             the_object->polys[curr_poly].clipped=1;
             continue;

             } // end if clipped

          // pre-compute x comparision ranges

          x1_compare = fov_width*z1;
          x2_compare = fov_width*z2;
          x3_compare = fov_width*z3;
          x4_compare = fov_width*z4;

          // perform x test

          if (!((x1>-x1_compare || x2>-x1_compare || x3>-x3_compare || x4>-x4_compare) &&
                (x1<x1_compare || x2<x2_compare || x3<x3_compare || x4<x4_compare))  )
             {
             // set clipped flag

             the_object->polys[curr_poly].clipped=1;
             continue;

             } // end if clipped

          // pre-compute x comparision ranges

          y1_compare = fov_height*z1;
          y2_compare = fov_height*z2;
          y3_compare = fov_height*z3;
          y4_compare = fov_height*z4;

          // perform x test

          if (!((y1>-y1_compare || y2>-y1_compare || y3>-y3_compare || y4>-y4_compare) &&
                (y1<y1_compare || y2<y2_compare || y3<y3_compare || y4<y4_compare))  )
             {
             // set clipped flag

             the_object->polys[curr_poly].clipped=1;
             continue;

             } // end if clipped

          } // end if quad
       else
          {
          // must be triangle, perform clipping tests on only 3 vertices

          // do clipping tests

          // perform near and far z clipping test first

          if (!((z1>clip_near_z || z2>clip_near_z || z3>clip_near_z) &&
                (z1<clip_far_z || z2<clip_far_z || z3<clip_far_z)) )
             {
             // set clipped flag

             the_object->polys[curr_poly].clipped=1;
             continue;

             } // end if clipped

          // pre-compute x comparision ranges

          x1_compare = fov_width*z1;
          x2_compare = fov_width*z2;
          x3_compare = fov_width*z3;

          // perform x test

          if (!((x1>-x1_compare || x2>-x1_compare || x3>-x3_compare ) &&
                (x1<x1_compare || x2<x2_compare || x3<x3_compare ))  )
             {
             // set clipped flag

             the_object->polys[curr_poly].clipped=1;
             continue;

             } // end if clipped

          // pre-compute x comparision ranges

          y1_compare = fov_height*z1;                                            (HALF_SCREEN_HEIGHT*z1)/viewing_distance;
          y2_compare = fov_height*z2;                                            (HALF_SCREEN_HEIGHT*z2)/viewing_distance;
          y3_compare = fov_height*z3;                                            (HALF_SCREEN_HEIGHT*z3)/viewing_distance;

          // perform x test

          if (!((y1>-y1_compare || y2>-y1_compare || y3>-y3_compare) &&
                (y1<y1_compare || y2<y2_compare || y3<y3_compare ))  )
             {
             // set clipped flag

             the_object->polys[curr_poly].clipped=1;
             continue;

             } // end if clipped

           } // end else triangle

       } // end for curr_poly

   } // end else clip everything

} // end Clip_Object_3D

//////////////////////////////////////////////////////////////////////////////

void Remove_Backfaces_And_Shade(object_ptr the_object)
{
// this function removes all the backfaces of an object by setting the visibility
// flag. This function assumes that the object has been transformed into
// camera coordinates. Also, the function takes into consideration is the
// polygons are one or two sided and executed the minimum amount of code
// in addition to perform the shading calculations


int vertex_0,         // vertex indices
    vertex_1,
    vertex_2,
    curr_poly;        // current polygon

float dp,             // the result of the dot product
      intensity;      // the final intensity of the surface

vector_3d u,v,        // general working vectors
          normal,     // the normal to the surface begin processed
          sight;      // line of sight vector

// for each polygon in the object determine if it is pointing away from the
// viewpoint and direction

for (curr_poly=0; curr_poly<the_object->num_polys; curr_poly++)
    {

    // is this polygon two sised or one sided

    if (the_object->polys[curr_poly].two_sided == ONE_SIDED)
       {

       // compute two vectors on polygon that have the same intial points

       vertex_0 = the_object->polys[curr_poly].vertex_list[0];
       vertex_1 = the_object->polys[curr_poly].vertex_list[1];
       vertex_2 = the_object->polys[curr_poly].vertex_list[2];

       // the vector u = vo->v1

       Make_Vector_3D((point_3d_ptr)&the_object->vertices_world[vertex_0],
                      (point_3d_ptr)&the_object->vertices_world[vertex_1],
                      (vector_3d_ptr)&u);

       // the vector v = vo-v2

       Make_Vector_3D((point_3d_ptr)&the_object->vertices_world[vertex_0],
                      (point_3d_ptr)&the_object->vertices_world[vertex_2],
                      (vector_3d_ptr)&v);

       // compute the normal to polygon v x u

       Cross_Product_3D((vector_3d_ptr)&v,
                        (vector_3d_ptr)&u,
                        (vector_3d_ptr)&normal);

       // compute the line of sight vector, since all coordinates are world all
       // object vertices are already relative to (0,0,0), thus

       sight.x = view_point.x-the_object->vertices_world[vertex_0].x;
       sight.y = view_point.y-the_object->vertices_world[vertex_0].y;
       sight.z = view_point.z-the_object->vertices_world[vertex_0].z;

       // compute the dot product between line of sight vector and normal to surface

       dp = Dot_Product_3D((vector_3d_ptr)&normal,(vector_3d_ptr)&sight);

       // is surface visible

       if (dp>0)
          {
          // set visible flag

          the_object->polys[curr_poly].visible = 1;

          // compute light intensity if needed

          if (the_object->polys[curr_poly].shading==FLAT_SHADING)
             {

             // compute the dot product between the light source vector
             // and normal vector to surface

             dp = Dot_Product_3D((vector_3d_ptr)&normal,
                                 (vector_3d_ptr)&light_source);

             // test if light ray is reflecting off surface

             if (dp>0)
                {
                // now cos 0 = (u.v)/|u||v| or

                intensity = ambient_light + (dp*(the_object->polys[curr_poly].normal_length));

                // test if intensity has overflowed

                if (intensity > 15)
                    intensity = 15;

                // intensity now varies from 0-1, 0 being black or grazing and 1 being
                // totally illuminated. use the value to index into color table

                the_object->polys[curr_poly].shade =
                           the_object->polys[curr_poly].color - (int)intensity;

                } // end if light is reflecting off surface
             else
                the_object->polys[curr_poly].shade =
                the_object->polys[curr_poly].color - (int)ambient_light;

             } // end if use flat shading
          else
             {
             // assume constant shading and simply assign color to shade

             the_object->polys[curr_poly].shade = the_object->polys[curr_poly].color;

             } // end else constant shading

          } // end if dp>0
       else
          the_object->polys[curr_poly].visible = 0;


       } // end if one sided
    else
       {
       // else polygon is always visible i.e. two sided, set visibility flag
       // so engine renders it

       // set visibility

       the_object->polys[curr_poly].visible = 1;

       // perform shading calculation

       if (the_object->polys[curr_poly].shading==FLAT_SHADING)
          {
          // compute normal

          // compute two vectors on polygon that have the same intial points

          vertex_0 = the_object->polys[curr_poly].vertex_list[0];
          vertex_1 = the_object->polys[curr_poly].vertex_list[1];
          vertex_2 = the_object->polys[curr_poly].vertex_list[2];

          // the vector u = vo->v1

          Make_Vector_3D((point_3d_ptr)&the_object->vertices_world[vertex_0],
                         (point_3d_ptr)&the_object->vertices_world[vertex_1],
                         (vector_3d_ptr)&u);

          // the vector v = vo-v2

          Make_Vector_3D((point_3d_ptr)&the_object->vertices_world[vertex_0],
                         (point_3d_ptr)&the_object->vertices_world[vertex_2],
                         (vector_3d_ptr)&v);

          // compute the normal to polygon v x u

          Cross_Product_3D((vector_3d_ptr)&v,
                           (vector_3d_ptr)&u,
                           (vector_3d_ptr)&normal);


          // compute the dot product between the light source vector
          // and normal vector to surface

          dp = Dot_Product_3D((vector_3d_ptr)&normal,
                              (vector_3d_ptr)&light_source);

          // test if light ray is reflecting off surface

          if (dp>0)
             {
             // now cos 0 = (u.v)/|u||v| or

             intensity = ambient_light + (dp*(the_object->polys[curr_poly].normal_length));

             // test if intensity has overflowed

             if (intensity > 15)
                 intensity = 15;

             // intensity now varies from 0-1, 0 being black or grazing and 1 being
             // totally illuminated. use the value to index into color table

             the_object->polys[curr_poly].shade =
                        the_object->polys[curr_poly].color - (int)intensity;

             } // end if light is reflecting off surface
          else
             the_object->polys[curr_poly].shade =
             the_object->polys[curr_poly].color - (int)ambient_light;

          } // end if use flat shading
       else
          {
          // assume constant shading and simply assign color to shade

          the_object->polys[curr_poly].shade = the_object->polys[curr_poly].color;

          } // end else constant shading

       } // end else two sided

    } // end for curr_poly

} // end Remove_Backfaces_And_Shade

//////////////////////////////////////////////////////////////////////////////

int Remove_Object(object_ptr the_object, int mode)
{
// this function determines if an entire object is within the viewing volume
// or not by testing if the bounding sphere of the object in question
// is within the viewing volume.In essence, this function "culls" entire objects

float x_bsphere,   // the x,y and z components of the projected center of object
      y_bsphere,
      z_bsphere,
      radius,     // the radius of object
      x_compare,  // the extents of the clipping volume in x and y at the
      y_compare;  // bounding spheres current z

// first transform world position of object into camera coordinates

// compute x component

x_bsphere = the_object->world_pos.x * global_view[0][0] +
            the_object->world_pos.y * global_view[1][0] +
            the_object->world_pos.z * global_view[2][0] +
                                      global_view[3][0];

// compute y component

y_bsphere = the_object->world_pos.x * global_view[0][1] +
            the_object->world_pos.y * global_view[1][1] +
            the_object->world_pos.z * global_view[2][1] +
                                      global_view[3][1];
// compute z component

z_bsphere = the_object->world_pos.x * global_view[0][2] +
            the_object->world_pos.y * global_view[1][2] +
            the_object->world_pos.z * global_view[2][2] +
                                      global_view[3][2];

// extract radius of object

radius = the_object->radius;

if (mode==OBJECT_CULL_Z_MODE)
   {
   // first test against near and far z planes

   if ( ((z_bsphere-radius) > clip_far_z) ||
        ((z_bsphere+radius) < clip_near_z) )
        return(1);
   else
        return(0);

   } // end if z only
else
   {
   // perform full x,y,z test

   if ( ((z_bsphere-radius) > clip_far_z) ||
        ((z_bsphere+radius) < clip_near_z) )
        return(1);

   // test against x right and left planes, first compute viewing volume
   // extents at position z position of bounding sphere

   x_compare = (HALF_SCREEN_WIDTH*z_bsphere)/viewing_distance;

   if ( ((x_bsphere-radius) > x_compare) ||
        ((x_bsphere+radius) < -x_compare) )
      return(1);

   // finally test against y top and bottom planes

   y_compare = (INVERSE_ASPECT_RATIO*HALF_SCREEN_HEIGHT*z_bsphere)/viewing_distance;

   if ( ((y_bsphere-radius) > y_compare) ||
        ((y_bsphere+radius) < -y_compare) )
      return(1);

   // else it just can't be removed!!!

   return(0);

   } // end else

} // end Remove_Object

//////////////////////////////////////////////////////////////////////////////

void Generate_Poly_List(object_ptr the_object,int mode)
{
// this function is used to generate the final polygon list that will be
// rendered. Object by object the list is built up


int vertex,
    curr_vertex,
    curr_poly;

// test if this is the first object to be inserted

if (mode==RESET_POLY_LIST)
   {
   // reset number of polys to zero

   num_polys_frame=0;
   return;

   } // end if first

// insert all visible polygons into polygon list

for (curr_poly=0; curr_poly<the_object->num_polys; curr_poly++)
    {

    // test if this poly is visible, if so add it to poly list

    if (the_object->polys[curr_poly].visible &&
        !the_object->polys[curr_poly].clipped)
       {
       // add this poly to poly list

       // first copy data and vertices into an open slot in storage area

       world_poly_storage[num_polys_frame].num_points = the_object->polys[curr_poly].num_points;
       world_poly_storage[num_polys_frame].color      = the_object->polys[curr_poly].color;
       world_poly_storage[num_polys_frame].shade      = the_object->polys[curr_poly].shade;
       world_poly_storage[num_polys_frame].shading    = the_object->polys[curr_poly].shading;
       world_poly_storage[num_polys_frame].two_sided  = the_object->polys[curr_poly].two_sided;
       world_poly_storage[num_polys_frame].visible    = the_object->polys[curr_poly].visible;
       world_poly_storage[num_polys_frame].clipped    = the_object->polys[curr_poly].clipped;
       world_poly_storage[num_polys_frame].active     = the_object->polys[curr_poly].active;

       // now copy vertices

       for (curr_vertex=0; curr_vertex<the_object->polys[curr_poly].num_points; curr_vertex++)
           {
           // extract vertex number

           vertex=the_object->polys[curr_poly].vertex_list[curr_vertex];

           // extract x,y and z

           world_poly_storage[num_polys_frame].vertex_list[curr_vertex].x
                                    = the_object->vertices_camera[vertex].x;

           world_poly_storage[num_polys_frame].vertex_list[curr_vertex].y
                                    = the_object->vertices_camera[vertex].y;

           world_poly_storage[num_polys_frame].vertex_list[curr_vertex].z
                                    = the_object->vertices_camera[vertex].z;


           } // end for curr_vertex

       // assign pointer to it

       world_polys[num_polys_frame] = &world_poly_storage[num_polys_frame];

       // increment number of polys

       num_polys_frame++;

       } // end if poly visible

    } // end for curr_poly

} // end Generate_Poly_List

//////////////////////////////////////////////////////////////////////////////

void Draw_Poly_List(void)
{

// this function draws the global polygon list generated by calls to
// Generate_Poly_List

int curr_poly;          // the current polygon

float x1,y1,z1,         // working variables
      x2,y2,z2,
      x3,y3,z3,
      x4,y4,z4,
      viewing_distance_aspect;  // the y axis corrected viewing distance


viewing_distance_aspect = viewing_distance*ASPECT_RATIO;

// draw each polygon in list

for (curr_poly=0; curr_poly<num_polys_frame; curr_poly++)
    {
    // get z's for perspective

    z1=world_polys[curr_poly]->vertex_list[0].z;
    z2=world_polys[curr_poly]->vertex_list[1].z;
    z3=world_polys[curr_poly]->vertex_list[2].z;

    // test if this is a quad

    if (world_polys[curr_poly]->num_points==4)
       {
       // extract vertex number and z component for clipping and projection

       z4=world_polys[curr_poly]->vertex_list[3].z;

       } // end if quad
    else
      z4=z3;

#if 0

    // perform z clipping test

    if ( (z1<clip_near_z && z2<clip_near_z && z3<clip_near_z && z4<clip_near_z) ||
         (z1>clip_far_z && z2>clip_far_z && z3>clip_far_z && z4>clip_far_z) )
       continue;

#endif

        // extract points of polygon

        x1 = world_polys[curr_poly]->vertex_list[0].x;
        y1 = world_polys[curr_poly]->vertex_list[0].y;

        x2 = world_polys[curr_poly]->vertex_list[1].x;
        y2 = world_polys[curr_poly]->vertex_list[1].y;

        x3 = world_polys[curr_poly]->vertex_list[2].x;
        y3 = world_polys[curr_poly]->vertex_list[2].y;

        // compute screen position of points

        x1=(HALF_SCREEN_WIDTH  + x1*viewing_distance/z1);
        y1=(HALF_SCREEN_HEIGHT - y1*viewing_distance_aspect/z1);

        x2=(HALF_SCREEN_WIDTH  + x2*viewing_distance/z2);
        y2=(HALF_SCREEN_HEIGHT - y2*viewing_distance_aspect/z2);

        x3=(HALF_SCREEN_WIDTH  + x3*viewing_distance/z3);
        y3=(HALF_SCREEN_HEIGHT - y3*viewing_distance_aspect/z3);

        // draw triangle

        Draw_Triangle_2D((int)x1,(int)y1,(int)x2,(int)y2,(int)x3,(int)y3,
                         world_polys[curr_poly]->shade);

        // draw second poly if this is a quad

        if (world_polys[curr_poly]->num_points==4)
           {
           // extract the point

           x4 = world_polys[curr_poly]->vertex_list[3].x;
           y4 = world_polys[curr_poly]->vertex_list[3].y;

           // poject to screen

           x4=(HALF_SCREEN_WIDTH  + x4*viewing_distance/z4);
           y4=(HALF_SCREEN_HEIGHT - y4*viewing_distance_aspect/z4);

           // draw triangle

           Draw_Triangle_2D((int)x1,(int)y1,(int)x3,(int)y3,(int)x4,(int)y4,
                            world_polys[curr_poly]->shade);

           } // end if quad

    } // end for curr_poly

} // end Draw_Poly_List

/////////////////////////////////////////////////////////////////////////////

void Compute_Average_Z(void)
{
// this function pre-computes the average z of each polygon, so that the
// polygon sorter doesn't compute each z more than once

int index;

for (index=0; index<num_polys_frame; index++)
    {

    if (world_poly_storage[index].num_points==3)
       {
       world_poly_storage[index].average_z =

                   (int)(0.3333*(world_poly_storage[index].vertex_list[0].z+
                                 world_poly_storage[index].vertex_list[1].z+
                                 world_poly_storage[index].vertex_list[2].z));

       } // end if
    else
       {
       world_poly_storage[index].average_z =

                      (int)(0.25*(world_poly_storage[index].vertex_list[0].z+
                                  world_poly_storage[index].vertex_list[1].z+
                                  world_poly_storage[index].vertex_list[2].z+
                                  world_poly_storage[index].vertex_list[3].z));

       } // end if

    } // end for index

} // end Compute_Average_Z

//////////////////////////////////////////////////////////////////////////////

int Poly_Compare(facet **arg1, facet **arg2)
{
// this function comapares the average z's of two polygons and is used by the
// depth sort surface ordering algorithm

facet_ptr poly_1,poly_2;

// dereference the poly pointers

poly_1 = (facet_ptr)*arg1;
poly_2 = (facet_ptr)*arg2;

if (poly_1->average_z>poly_2->average_z)
   return(-1);
else
if (poly_1->average_z<poly_2->average_z)
   return(1);
else
   return(0);

} // end Poly_Compare

/////////////////////////////////////////////////////////////////////////////

void Sort_Poly_List(void)
{
// this function does a simple z sort on the poly list to order surfaces
// the list is sorted in descending order, i.e. farther polygons first

Compute_Average_Z();

qsort((void *)world_polys,num_polys_frame, sizeof(facet_ptr), (qsort_cast)Poly_Compare);

} // end Sort_Poly_List

///////////////////////////////////////////////////////////////////////////////

int Load_Palette_Disk(char *filename, RGB_palette_ptr the_palette)
{
// this function loads a color palette from disk

int index; // used for looping

RGB_color color;

FILE *fp;

// open the disk file

if (!(fp = fopen(filename,"r")))
   return(0);

// load in all the colors

for (index=0; index<=255; index++)
    {
    // get the next color

    fscanf(fp,"%d %d %d",&color.red,&color.green,&color.blue);

    // store the color in next element of palette

    the_palette->colors[index].red   = color.red;
    the_palette->colors[index].green = color.green;
    the_palette->colors[index].blue  = color.blue;

    } // end for index

// set palette size to a full palette

the_palette->start_reg = 0;
the_palette->end_reg   = 255;

// close the file and return success

fclose(fp);

return(1);

} // end Load_Palette_Disk

///////////////////////////////////////////////////////////////////////////////

int Save_Palette_Disk(char *filename, RGB_palette_ptr the_palette)
{
// this function saves a palette to disk

int index; // used for looping

RGB_color color;

FILE *fp;

// open the disk file

if (!(fp = fopen(filename,"w")))
   return(0);

// write 255 lines of r g b

for (index=0; index<=255; index++)
    {
    // get the next color

    // store the color in next element of palette

    color.red   = the_palette->colors[index].red;
    color.green = the_palette->colors[index].green;
    color.blue  = the_palette->colors[index].blue;

    // write the color to disk file

    fprintf(fp,"\n%d %d %d",color.red, color.green, color.blue);

    } // end for index

// close the file and return success

fclose(fp);

return(1);

} // end Save_Palette_Disk

///////////////////////////////////////////////////////////////////////////////

void Fill_Double_Buffer_32(int icolor)
{
// this function fills in the double buffer with the sent color a QUAD at
// a time

long color;  // used to create a 4 byte color descriptor

color = icolor | (icolor << 8);

// replicate color into all four bytes of quad

color = color | (color<<16);

fquadset((void far *)double_buffer,color,(double_buffer_size >> 1));

} // end Fill_Double_Buffer_32

//////////////////////////////////////////////////////////////////////////////

void Display_Double_Buffer_32(unsigned char far *buffer,int y)
{
// this functions copies the double buffer into the video buffer at the
// starting y location using quad byte transfers

fquadcpy((void far *)(video_buffer+y*320),
			(void far *)double_buffer,
			(double_buffer_size >> 1));


} // end Display_Double_Buffer_32


