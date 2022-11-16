
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
#include "black11.h"
#include "black15.h"

// G L O B A L S //////////////////////////////////////////////////////////////

int far *z_buffer,   // the current z buffer memory
    far *z_bank_1,   // memory bank 1 of z buffer
    far *z_bank_2;   // memory bank 2 of z buffer

unsigned int z_height    = 200,    // the height if the z buffer
             z_height_2  = 100,    // the height if half the z buffer
				 z_bank_size = 64000L; // size of a z buffer bank in bytes

FILE *fp_out;   // general output file

////////////////////////////////////////////////////////////////////////////////

void Draw_TB_Tri_3D_Z(int x1,int y1, int z1,
                      int x2,int y2, int z2,
                      int x3,int y3, int z3,
                      int color)

{
// this function draws a triangle that has a flat top

float dx_right,    // the dx/dy ratio of the right edge of line
      dx_left,     // the dx/dy ratio of the left edge of line
      xs,xe,       // the starting and ending points of the edges
      height,      // the height of the triangle

      dx,          // general delta's
      dy,
      z_left,      // the z value of the left edge of current line
      z_right,     // the z value for the right edge of current line
      ay,          // interpolator constant
      b1y,         // the change of z with respect to y on the left edge
      b2y;         // the change of z with respect to y on the right edge

int temp_x,        // used during sorting as temps
    temp_y,
    temp_z,
    xs_clip,       // used by clipping
    xe_clip,
    x,
    x_index,       // used as looping vars
    y_index;

// change these two back to float and remove all *32 and >>5
// if you don't want to use fixed point during horizontal z interpolation

int z_middle,  // the z value of the middle between the left and right
    bx;        // the change of z with respect to x

unsigned char far *dest_addr; // current image destination

// test order of x1 and x2, note y1=y2.

// test if top or bottom is flat and set constants appropriately

if (y1==y2)
   {
   // perform computations for a triangle with a flat top

   if (x2 < x1)
      {

      temp_x = x2;
      temp_z = z2;

      x2     = x1;
      z2     = z1;

      x1     = temp_x;
      z1     = temp_z;

      } // end if swap

   // compute delta's for scan conversion

   height = y3-y1;

   dx_left  = (x3-x1)/height;
   dx_right = (x3-x2)/height;

   // compute deltas for z interpolation

   z_left  = z1;
   z_right = z2;

   // vertical interpolants

   ay  = 1/height;

   b1y = ay*(z3-z1);
   b2y = ay*(z3-z2);

   // set starting points

   xs = (float)x1;
   xe = (float)x2;

   } // end top is flat
else
   { // bottom must be flat

   // test order of x3 and x2, note y2=y3.

   if (x3 < x2)
      {
      temp_x = x2;
      temp_z = z2;

      x2     = x3;
      z2     = z3;

      x3     = temp_x;
      z3     = temp_z;

      } // end if swap

   // compute delta's for scan conversion

   height = y3-y1;

   dx_left  = (x2-x1)/height;
   dx_right = (x3-x1)/height;

   // compute deltas for z interpolation

   z_left  = z1;
   z_right = z1;

   // vertical interpolants

   ay  = 1/height;

   b1y = ay*(z2-z1);
   b2y = ay*(z3-z1);

   // set starting points

   xs = (float)x1;
   xe = (float)x1;

   } // end else bottom is flat

// perform y clipping

// clip top

if (y1<poly_clip_min_y)
   {
   // compute new xs and ys

   dy = (float)(-y1+poly_clip_min_y);

   xs = xs+dx_left*dy;
   xe = xe+dx_right*dy;

   // re-compute z_left and z_right to take into consideration
   // vertical shift down

   z_left  += b1y*dy;
   z_right += b2y*dy;

   // reset y1

   y1=poly_clip_min_y;

   } // end if top is off screen

// clip bottom

if (y3>poly_clip_max_y)
   y3=poly_clip_max_y;

// compute starting address in video memory

dest_addr = double_buffer+(y1<<8)+(y1<<6);

// start z buffer at proper bank

if (y1<z_height_2)
   z_buffer = z_bank_1+(y1<<8)+(y1<<6);
else
   {

   temp_y = y1-z_height_2;

   z_buffer = z_bank_2+(temp_y<<8)+(temp_y<<6);

   } // end else

// test if x clipping is needed

if (x1>=poly_clip_min_x && x1<=poly_clip_max_x &&
    x2>=poly_clip_min_x && x2<=poly_clip_max_x &&
    x3>=poly_clip_min_x && x3<=poly_clip_max_x)
    {
    // draw the triangle

    for (y_index=y1; y_index<=y3; y_index++)
        {

        // test if we need to switch to z buffer bank two

        if (y_index==z_height_2)
           z_buffer = z_bank_2;

///////////////////////////////////////////////////////////////////////////////

// this section uses a bit of 16 bit fixed point for the horizontal
// interpolation. The format is 11:5 i.e 11 whole places and 5 decimal places
// Also, notice the use of >>5 and *32 to convert to fixed point and back to
// simple integer, also the old version of each line is commented out under each
// fixed point calculation, so you can see what was originally done...

        // compute horizontal z interpolant

        z_middle = 32*z_left;
        // z_middle = z_left;

        bx = 32*(z_right - z_left)/(1+xe-xs);
        // bx = (z_right - z_left)/(1+xe-xs);

        // draw the line

        for (x_index=xs; x_index<=xe; x_index++)
            {
            // if current z_middle is less than z-buffer then replace
            // and update image buffer

				if (z_middle>>5 < z_buffer[x_index])
            // if (z_middle < z_buffer[x_index])
               {
               // update z buffer

               z_buffer[x_index]=(int)z_middle>>5;
               // z_buffer[x_index]=(int)z_middle;

               // write to image buffer

               dest_addr[x_index] = color;

               // update video buffer

               } // end if update buffer

            // update current z value

            z_middle += bx;

// END FIXED POINT DEMO SECTION ////////////////////////////////////////////////

            } // end draw z buffered line

        // adjust starting point and ending point for scan conversion

        xs+=dx_left;
        xe+=dx_right;

        // adjust vertical z interpolants

        z_left  += b1y;
        z_right += b2y;

        // adjust video and z buffer offsets

        dest_addr += 320;
        z_buffer  += 320;

        } // end for

    } // end if no x clipping needed
else
   {
   // clip x axis with slower version

   // draw the triangle

   for (y_index=y1; y_index<=y3; y_index++)
       {

       // test if we need to switch to z buffer bank two

       if (y_index==z_height_2)
          z_buffer = z_bank_2;

       // do x clip

       xs_clip  = (int)xs;
       xe_clip  = (int)xe;

///////////////////////////////////////////////////////////////////////////////

// this section uses a bit of 16 bit fixed point for the horizontal
// interpolation. The format is 11:5 i.e 11 whole places and 5 decimal places
// Also, notice the use of >>5 and *32 to convert to fixed point and back to
// simple integer, also the old version of each line is commented out under each
// fixed point calculation.

       // compute horizontal z interpolant

       z_middle = 32*z_left;
       // z_middle = z_left;

       bx = 32*(z_right - z_left)/(1+xe-xs);
       // bx = 32*(z_right - z_left)/(1+xe-xs);

       // adjust starting point and ending point

       xs+=dx_left;
       xe+=dx_right;

       // adjust vertical z interpolants

       z_left  += b1y;
       z_right += b2y;

       // clip line

       if (xs_clip < poly_clip_min_x)
          {
          dx = (-xs_clip + poly_clip_min_x);

          xs_clip = poly_clip_min_x;

          // re-compute z_middle to take into consideration horizontal shift

          z_middle += 32*bx*dx;
          // z_middle += bx*dx;

          } // end if line is clipped on left

       if (xe_clip > poly_clip_max_x)
          {
          xe_clip = poly_clip_max_x;

          } // end if line is clipped on right

       // draw the line

       for (x_index=xs_clip; x_index<=xe_clip; x_index++)
           {
           // if current z_middle is less than z-buffer then replace
           // and update image buffer

			  if (z_middle>>5 < z_buffer[x_index])
           // if (z_middle < z_buffer[x_index])
              {
              // update z buffer

              z_buffer[x_index]=(int)z_middle>>5;
           // z_buffer[x_index]=(int)z_middle;

              // write to image buffer

              dest_addr[x_index] = color;

              // update video buffer

              } // end if update z buffer

           // update current z value

           z_middle += bx;

// END FIXED POINT DEMO SECTION ////////////////////////////////////////////////

           } // end draw z buffered line

       // adjust video and z buffer offsets

       dest_addr += 320;
       z_buffer  += 320;

       } // end for y_index

   } // end else x clipping needed

} // end Draw_TB_Tri_3D_Z

/////////////////////////////////////////////////////////////////////////////

void Draw_Tri_3D_Z(int x1,int y1,int z1,
                   int x2,int y2,int z2,
                   int x3,int y3,int z3,
                   int color)
{

int temp_x,    // used for sorting
    temp_y,
    temp_z,
    new_x,     // used to compute new x and z at triangle spliting point
    new_z;

// test for h lines and v lines

if ((x1==x2 && x2==x3)  ||  (y1==y2 && y2==y3))
   return;

// sort p1,p2,p3 in ascending y order

if (y2<y1)
   {
   temp_x = x2;
   temp_y = y2;
   temp_z = z2;

   x2     = x1;
   y2     = y1;
   z2     = z1;

   x1     = temp_x;
   y1     = temp_y;
   z1     = temp_z;

   } // end if

// now we know that p1 and p2 are in order

if (y3<y1)
   {
   temp_x = x3;
   temp_y = y3;
   temp_z = y3;

   x3     = x1;
   y3     = y1;
   z3     = z1;

   x1     = temp_x;
   y1     = temp_y;
   z1     = temp_z;

   } // end if

// finally test y3 against y2

if (y3<y2)
   {
   temp_x = x3;
   temp_y = y3;
   temp_z = z3;

   x3     = x2;
   y3     = y2;
   z3     = z2;

   x2     = temp_x;
   y2     = temp_y;
   z2     = temp_z;

   } // end if

// do trivial rejection tests

if ( y3<poly_clip_min_y || y1>poly_clip_max_y ||
    (x1<poly_clip_min_x && x2<poly_clip_min_x && x3<poly_clip_min_x) ||
    (x1>poly_clip_max_x && x2>poly_clip_max_x && x3>poly_clip_max_x) )
   return;

// test if top of triangle is flat

if (y1==y2 || y2==y3)
   {

   Draw_TB_Tri_3D_Z(x1,y1,z1,x2,y2,z2,x3,y3,z3,color);

   } // end if
else
   {
   // general triangle that's needs to be broken up along long edge

   // commpute new x,z at split point

   new_x = x1 + (int)((float)(y2-y1)*(float)(x3-x1)/(float)(y3-y1));
   new_z = z1 + (int)((float)(y2-y1)*(float)(z3-z1)/(float)(y3-y1));

   // draw each sub-triangle

   if (y2>=poly_clip_min_y && y1<poly_clip_max_y)
      Draw_TB_Tri_3D_Z(x1,y1,z1,new_x,y2,new_z,x2,y2,z2,color);

   if (y3>=poly_clip_min_y && y1<poly_clip_max_y)
      Draw_TB_Tri_3D_Z(x2,y2,z2,new_x,y2,new_z,x3,y3,z3,color);

   } // end else

} // end Draw_Tri_3D_Z

///////////////////////////////////////////////////////////////////////////////

void Draw_Poly_List_Z(void)
{
// this function draws the global polygon list generated by calls to
// Generate_Poly_List using the z buffer triangle system

int curr_poly,          // the current polygon
    is_quad=0;          // quadrilateral flag

float x1,y1,z1,         // working variables
      x2,y2,z2,
      x3,y3,z3,
      x4,y4,z4;

// draw each polygon in list

for (curr_poly=0; curr_poly<num_polys_frame; curr_poly++)
    {

    // do Z clipping first before projection

    z1=world_polys[curr_poly]->vertex_list[0].z;
    z2=world_polys[curr_poly]->vertex_list[1].z;
    z3=world_polys[curr_poly]->vertex_list[2].z;

    // test if this is a quad

    if (world_polys[curr_poly]->num_points==4)
       {
       // extract vertex number and z component for clipping and projection

       z4=world_polys[curr_poly]->vertex_list[3].z;

       // set quad flag

       is_quad=1;

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
        y1=(HALF_SCREEN_HEIGHT - ASPECT_RATIO*y1*viewing_distance/z1);

        x2=(HALF_SCREEN_WIDTH  + x2*viewing_distance/z2);
        y2=(HALF_SCREEN_HEIGHT - ASPECT_RATIO*y2*viewing_distance/z2);

        x3=(HALF_SCREEN_WIDTH  + x3*viewing_distance/z3);
        y3=(HALF_SCREEN_HEIGHT - ASPECT_RATIO*y3*viewing_distance/z3);

        // draw triangle

        Draw_Tri_3D_Z((int)x1,(int)y1,(int)z1,
                      (int)x2,(int)y2,(int)z2,
                      (int)x3,(int)y3,(int)z3,
                      world_polys[curr_poly]->shade);


        // draw second poly if this is a quad

        if (is_quad)
           {
           // extract the point

           x4 = world_polys[curr_poly]->vertex_list[3].x;
           y4 = world_polys[curr_poly]->vertex_list[3].y;

           // poject to screen

           x4=(HALF_SCREEN_WIDTH  + x4*viewing_distance/z4);
           y4=(HALF_SCREEN_HEIGHT - ASPECT_RATIO*y4*viewing_distance/z4);

           // draw triangle

           Draw_Tri_3D_Z((int)x1,(int)y1,(int)z1,
                         (int)x3,(int)y3,(int)z3,
                         (int)x4,(int)y4,(int)z4,
                         world_polys[curr_poly]->shade);

           } // end if quad

    } // end for curr_poly

} // end Draw_Poly_List_Z

////////////////////////////////////////////////////////////////////////////////

int Create_Z_Buffer(unsigned int height)
{
// this function allocates the z buffer in two banks

// set global z buffer values

z_height   = height;
z_height_2 = height/2;

z_bank_size = (height/2)*(unsigned int)640;

// allocate the memory

z_bank_1 = (int far *)_fmalloc(z_bank_size);
z_bank_2 = (int far *)_fmalloc(z_bank_size);

// return success or failure

if (z_bank_1 && z_bank_2)
   return(1);
else
   return(0);

} // end Create_Z_Buffer

////////////////////////////////////////////////////////////////////////////////

void Delete_Z_Buffer(void)
{

// this function free's up the memory used by the z buffer memory banks

_ffree(z_bank_1);
_ffree(z_bank_2);


} // end Delete_Z_Buffer

////////////////////////////////////////////////////////////////////////////////

void Fill_Z_Buffer(int value)
{

// this function fills the entire z buffer (both banks) with the sent value

_asm    {

// bank 1

	les di,z_bank_1       // point es:di to z buffer bank 1
	mov ax,value          // move the value into ax
	mov cx,z_bank_size    // number of bytes to fill
	shr cx,1              // convert to number of words
	rep stosw             // move the value into z buffer

// bank 2

	les di,z_bank_2       // point es:di to z buffer bank 1
	mov ax,value          // move the value into ax (redundant)
	mov cx,z_bank_size    // number of bytes to fill  (so is this)
	shr cx,1              // convert to number of words
	rep stosw             // move the value into z buffer

	} // end asm

} // end Fill_Z_Buffer

///////////////////////////////////////////////////////////////////////////////

void Bsp_World_To_Camera(wall_ptr root)
{
// this function traverses the bsp tree and converts the world coordinates
// to camera coordinates using the global transformation matrix note the
// function is recursive and uses and inorder traversal, other traversals
// such as preorder and postorder will would just as well...

static int index; // looping variable

// test if we have hit a dead end

if (root==NULL)
	return;

// transform back most sub-tree

Bsp_World_To_Camera(root->back);

// iterate thru all vertices of current wall and transform them into
// camera coordinates

for (index=0; index<4; index++)
	 {

	 // multiply the point by the viewing transformation matrix

	 // x component

	 root->wall_camera[index].x =

						root->wall_world[index].x * global_view[0][0] +
						root->wall_world[index].y * global_view[1][0] +
						root->wall_world[index].z * global_view[2][0] +
															 global_view[3][0];

	 // y component

	 root->wall_camera[index].y =

						root->wall_world[index].x * global_view[0][1] +
						root->wall_world[index].y * global_view[1][1] +
						root->wall_world[index].z * global_view[2][1] +
															 global_view[3][1];
	 // z component

	 root->wall_camera[index].z =

						root->wall_world[index].x * global_view[0][2] +
						root->wall_world[index].y * global_view[1][2] +
						root->wall_world[index].z * global_view[2][2] +
															 global_view[3][2];

	 } // end for index

// transform front most sub-tree

Bsp_World_To_Camera(root->front);

} // end Bsp_World_To_Camera

/////////////////////////////////////////////////////////////////////////////

void Bsp_Translate(wall_ptr root,int x_trans,int y_trans,int z_trans)
{
// this function translates all the walls that make up the bsp world
// note function is recursive, we don't really need this function, but
// it's a good example of how we might perform transformations on the BSP
// tree and similar tree like structures using recursion

static int index; // looping variable

// test if we have hit a dead end

if (root==NULL)
	return;

// translate back most sub-tree

Bsp_Translate(root->back,x_trans,y_trans,z_trans);

// iterate thru all vertices of current wall and translate them

for (index=0; index<4; index++)
	 {
	 // perform translation

	 root->wall_world[index].x+=x_trans;
	 root->wall_world[index].y+=y_trans;
	 root->wall_world[index].z+=z_trans;

	 } // end for index

// translate front most sub-tree

Bsp_Translate(root->front,x_trans,y_trans,z_trans);

} // end Bsp_Translate

///////////////////////////////////////////////////////////////////////////////

void Bsp_Shade(wall_ptr root)
{
// this function shades the bsp tree and need only be called if the global
// lightsource changes position

static int index; // looping variable

static float normal_length, // length of surface normal
             intensity,     // intensity of light falling on surface being processed
             dp;            // result of dot product

// test if we have hit a dead end

if (root==NULL)
   return;

// shade the back most sub-tree

Bsp_Shade(root->back);

// compute the dot product between line of sight vector and normal to surface

dp = Dot_Product_3D((vector_3d_ptr)&root->normal,(vector_3d_ptr)&light_source);

// compute length of normal of surface normal, remember this function
// doesn't need to be time critical since it is only called once at startup
// or whenever the light source moves

normal_length = Vector_Mag_3D((vector_3d_ptr)&root->normal);

// cos 0 = (u.v)/|u||v| or

intensity = ambient_light + (15*dp/normal_length);

// test if intensity has overflowed

if (intensity >15)
    intensity = 15;

// intensity now varies from 0-1, 0 being black or grazing and 1 being
// totally illuminated. use the value to index into color table

root->color = BSP_WALL_SHADE - (int)(fabs(intensity));

// shade the front most sub-tree

Bsp_Shade(root->front);

} // end Bsp_Shade

/////////////////////////////////////////////////////////////////////////////

void Bsp_Traverse(wall_ptr root)
{
// this function traverses the BSP tree and generates the polygon list used
// by Draw_Polys() for the current global viewpoint (note the view angle is
// irrelevant), also as the polygon list is being generated, only polygons
// that are within the z extents are added to the polygon, in essence, the
// function is performing Z clipping also, this is to minimize the amount
// of polygons in the graphics pipeline that will have to be processed during
// rendering

// this function works be testing the viewpoint against the current wall
// in the bsp, then depending on the side the viewpoint is the algorithm
// proceeds. the search takes place as the rest in an "inorder" method
// with hooks to process and add each node into the polygon list at the
// right time

static vector_3d test_vector;

static float dot_wall,
             z1,z2,z3,z4;

// SECTION 1  ////////////////////////////////////////////////////////////////

// is this a dead end?

if (root==NULL)
	return;

// test which side viewpoint is on relative to the current wall

Make_Vector_3D((point_3d_ptr)&root->wall_world[0],
               (point_3d_ptr)&view_point,
               (vector_3d_ptr)&test_vector);

// now dot test vector with the surface normal and analyze signs

dot_wall = Dot_Product_3D((vector_3d_ptr)&test_vector,
                          (vector_3d_ptr)&root->normal);

// SECTION 2  ////////////////////////////////////////////////////////////////

// if the sign of the dot product is positive then the viewer on on the
// front side of current wall, so recursively process the walls behind then
// in front of this wall, else do the opposite

if (dot_wall>0)
   {
   // viewer is in front of this wall

   // process the back wall sub tree

   Bsp_Traverse(root->back);

   // try to add this wall to the polygon list if it's within the Z extents

   z1=root->wall_camera[0].z;
   z2=root->wall_camera[1].z;
   z3=root->wall_camera[2].z;
   z4=root->wall_camera[3].z;

   // perform the z extents clipping test


   if ( (z1>clip_near_z && z1<clip_far_z ) || (z2>clip_near_z && z2<clip_far_z ) ||
        (z3>clip_near_z && z3<clip_far_z ) || (z4>clip_near_z && z4<clip_far_z ))
      {

      // first copy data and vertices into an open slot in storage area

      world_poly_storage[num_polys_frame].num_points = 4;
      world_poly_storage[num_polys_frame].color      = BSP_WALL_COLOR;
      world_poly_storage[num_polys_frame].shade      = root->color;
      world_poly_storage[num_polys_frame].shading    = 0;
      world_poly_storage[num_polys_frame].two_sided  = 1;
		world_poly_storage[num_polys_frame].visible    = 1;
		world_poly_storage[num_polys_frame].clipped    = 0;
      world_poly_storage[num_polys_frame].active     = 1;

      // now copy vertices

      world_poly_storage[num_polys_frame].vertex_list[0].x = root->wall_camera[0].x;
      world_poly_storage[num_polys_frame].vertex_list[0].y = root->wall_camera[0].y;
      world_poly_storage[num_polys_frame].vertex_list[0].z = root->wall_camera[0].z;

      world_poly_storage[num_polys_frame].vertex_list[1].x = root->wall_camera[1].x;
      world_poly_storage[num_polys_frame].vertex_list[1].y = root->wall_camera[1].y;
      world_poly_storage[num_polys_frame].vertex_list[1].z = root->wall_camera[1].z;

      world_poly_storage[num_polys_frame].vertex_list[2].x = root->wall_camera[2].x;
      world_poly_storage[num_polys_frame].vertex_list[2].y = root->wall_camera[2].y;
      world_poly_storage[num_polys_frame].vertex_list[2].z = root->wall_camera[2].z;

		world_poly_storage[num_polys_frame].vertex_list[3].x = root->wall_camera[3].x;
      world_poly_storage[num_polys_frame].vertex_list[3].y = root->wall_camera[3].y;
      world_poly_storage[num_polys_frame].vertex_list[3].z = root->wall_camera[3].z;


      // assign poly list pointer to it

      world_polys[num_polys_frame] = &world_poly_storage[num_polys_frame];

      // increment number of polys in this frame

      num_polys_frame++;

      } // end if polygon is visible

   // now process the front walls sub tree

	Bsp_Traverse(root->front);

   } // end if

// SECTION 3 ////////////////////////////////////////////////////////////////

else
   {
   // viewer is behind this wall

   // process the front wall sub tree

   Bsp_Traverse(root->front);

   // try to add this wall to the polygon list if it's within the Z extents

   z1=root->wall_camera[0].z;
	z2=root->wall_camera[1].z;
   z3=root->wall_camera[2].z;
   z4=root->wall_camera[3].z;

   // perform the z extents clipping test

   if ( (z1>clip_near_z && z1<clip_far_z ) || (z2>clip_near_z && z2<clip_far_z )  ||
        (z3>clip_near_z && z3<clip_far_z ) || (z4>clip_near_z && z4<clip_far_z ))
      {

      // first copy data and vertices into an open slot in storage area

      world_poly_storage[num_polys_frame].num_points = 4;
      world_poly_storage[num_polys_frame].color      = BSP_WALL_COLOR;
      world_poly_storage[num_polys_frame].shade      = root->color;
      world_poly_storage[num_polys_frame].shading    = 0;
      world_poly_storage[num_polys_frame].two_sided  = 1;
		world_poly_storage[num_polys_frame].visible    = 1;
      world_poly_storage[num_polys_frame].clipped    = 0;
      world_poly_storage[num_polys_frame].active     = 1;

      // now copy vertices, note that we don't use a structure copy, it's
      // not dependable

      world_poly_storage[num_polys_frame].vertex_list[0].x = root->wall_camera[0].x;
      world_poly_storage[num_polys_frame].vertex_list[0].y = root->wall_camera[0].y;
      world_poly_storage[num_polys_frame].vertex_list[0].z = root->wall_camera[0].z;

      world_poly_storage[num_polys_frame].vertex_list[1].x = root->wall_camera[1].x;
      world_poly_storage[num_polys_frame].vertex_list[1].y = root->wall_camera[1].y;
      world_poly_storage[num_polys_frame].vertex_list[1].z = root->wall_camera[1].z;

      world_poly_storage[num_polys_frame].vertex_list[2].x = root->wall_camera[2].x;
      world_poly_storage[num_polys_frame].vertex_list[2].y = root->wall_camera[2].y;
		world_poly_storage[num_polys_frame].vertex_list[2].z = root->wall_camera[2].z;

      world_poly_storage[num_polys_frame].vertex_list[3].x = root->wall_camera[3].x;
      world_poly_storage[num_polys_frame].vertex_list[3].y = root->wall_camera[3].y;
      world_poly_storage[num_polys_frame].vertex_list[3].z = root->wall_camera[3].z;


      // assign poly list pointer to it

      world_polys[num_polys_frame] = &world_poly_storage[num_polys_frame];

      // increment number of polys in this frame

      num_polys_frame++;

      } // end if polygon is visible

	// now process the front walls sub tree

   Bsp_Traverse(root->back);

   } // end else

} // end Bsp_Traverse

/////////////////////////////////////////////////////////////////////////////

void Bsp_Delete(wall_ptr root)
{
// this function recursively deletes all the nodes in the bsp tree and free's
// the memory back to the OS.

wall_ptr temp_wall; // a temporary wall

// test if we have hit a dead end

if (root==NULL)
   return;

// delete back sub tree

Bsp_Delete(root->back);

// delete this node, but first save the front sub-tree

temp_wall = root->front;

// delete the memory

free(root);

// assign the root to the saved front most sub-tree

root = temp_wall;

// delete front sub tree

Bsp_Delete(root);

} // end Bsp_Delete

/////////////////////////////////////////////////////////////////////////////

void Bsp_Print(wall_ptr root)
{
// this function performs a recursive in-order traversal of the BSP tree and
// prints the results out to the file opened with fp_out as the handle

// test if this child is null

if (root==NULL)
   {
   fprintf(fp_out,"\nReached NULL node returning...");
   return;
   } // end if

// search left tree (back walls)

fprintf(fp_out,"\nTraversing back sub-tree...");

Bsp_Print(root->back);

// visit node

fprintf(fp_out,"\n\n\nWall ID #%d",root->id);
fprintf(fp_out,"\nVertices...");
fprintf(fp_out,"\nVertex 0: (%f,%f,%f)",root->wall_world[0].x,
                                        root->wall_world[0].y,
                                        root->wall_world[0].z);

fprintf(fp_out,"\nVertex 1: (%f,%f,%f)",root->wall_world[1].x,
                                        root->wall_world[1].y,
                                        root->wall_world[1].z);

fprintf(fp_out,"\nVertex 2: (%f,%f,%f)",root->wall_world[2].x,
                                        root->wall_world[2].y,
                                        root->wall_world[2].z);

fprintf(fp_out,"\nVertex 3: (%f,%f,%f)",root->wall_world[3].x,
                                        root->wall_world[3].y,
                                        root->wall_world[3].z);

fprintf(fp_out,"\nNormal (%f,%f,%f)",root->normal.x,
                                     root->normal.y,
                                     root->normal.z);

fprintf(fp_out,"\nEnd wall data\n");

// search right tree (front walls)

fprintf(fp_out,"\nTraversing front sub-tree..");

Bsp_Print(root->front);

} // end Bsp_Print

//////////////////////////////////////////////////////////////////////////////

void Intersect_Lines(float x0,float y0,float x1,float y1,
							float x2,float y2,float x3,float y3,
                     float *xi,float *yi)
{
// this function computes the intersection of the sent lines
// and returns the intersection point, note that the function assumes
// the lines intersect. the function can handle vertical as well
// as horizontal lines. note the function isn't very clever, it simply applies
// the math, but we don't need speed since this is a pre-processing step

float a1,b1,c1, // constants of linear equations
      a2,b2,c2,
      det_inv,  // the inverse of the determinant of the coefficient matrix
      m1,m2;    // the slopes of each line

// compute slopes, note the cludge for infinity, however, this will
// be close enough

if ((x1-x0)!=0)
   m1 = (y1-y0)/(x1-x0);
else
   m1 = (float)1e+10;   // close enough to infinity

if ((x3-x2)!=0)
   m2 = (y3-y2)/(x3-x2);
else
   m2 = (float)1e+10;   // close enough to infinity

// compute constants

a1 = m1;
a2 = m2;

b1 = -1;
b2 = -1;

c1 = (y0-m1*x0);
c2 = (y2-m2*x2);

// compute the inverse of the determinate

det_inv = 1/(a1*b2 - a2*b1);

// use Kramers rule to compute xi and yi

*xi=((b1*c2 - b2*c1)*det_inv);
*yi=((a2*c1 - a1*c2)*det_inv);

} // end Intersect_Lines

/////////////////////////////////////////////////////////////////////////////

void Build_Bsp_Tree(wall_ptr root)
{
// this function recursively builds the bsp tree from the sent wall list
// note the function has sone calls to Draw_Line() and a Time_Delay() at
// the end, these are for illustrative purposes only for the demo interface
// and should be removed if you wish to use this function in a real
// application

static wall_ptr next_wall,     // pointer to next wall to be processed
                front_wall,    // the front wall
                back_wall,     // the back wall
                temp_wall;     // a temporary wall

static float

      dot_wall_1,                // dot products for test wall
      dot_wall_2,
		wall_x0,wall_y0,wall_z0,   // working vars for test wall
      wall_x1,wall_y1,wall_z1,
      pp_x0,pp_y0,pp_z0,         // working vars for partitioning plane
      pp_x1,pp_y1,pp_z1,
      xi,zi;                     // points of intersection when the partioning
                                 // plane cuts a wall in two

static vector_3d test_vector_1,  // test vectors from the partioning plane
           test_vector_2;        // to the test wall to test the side
                                 // of the partioning plane the test wall
                                 // lies on

static int front_flag =0,        // flags if a wall is on the front or back
           back_flag = 0,        // of the partioning plane
           index;                // looping index

// SECTION 1 ////////////////////////////////////////////////////////////////

// test if this tree is complete

if (root==NULL)
   return;

// the root is the partitioning plane, partition the polygons using it

next_wall  = root->link;
root->link = NULL;

// extract top two vertices of partioning plane wall for ease of calculations

pp_x0 = root->wall_world[0].x;
pp_y0 = root->wall_world[0].y;
pp_z0 = root->wall_world[0].z;

pp_x1 = root->wall_world[1].x;
pp_y1 = root->wall_world[1].y;
pp_z1 = root->wall_world[1].z;

// highlight space partition green

Draw_Line(pp_x0/WORLD_SCALE_X-SCREEN_TO_WORLD_X,
          pp_z0/WORLD_SCALE_Z-SCREEN_TO_WORLD_Z,
          pp_x1/WORLD_SCALE_X-SCREEN_TO_WORLD_X,
          pp_z1/WORLD_SCALE_Z-SCREEN_TO_WORLD_Z,
          10,
          video_buffer);

// SECTION 2  ////////////////////////////////////////////////////////////////

// test if all walls have been partitioned

while(next_wall)
     {

     // test which side test wall is relative to partioning plane
     // defined by root

     // first compute vectors from point on partioning plane to point on
     // test wall

     Make_Vector_3D((point_3d_ptr)&root->wall_world[0],
                    (point_3d_ptr)&next_wall->wall_world[0],
                    (vector_3d_ptr)&test_vector_1);

     Make_Vector_3D((point_3d_ptr)&root->wall_world[0],
                    (point_3d_ptr)&next_wall->wall_world[1],
                    (vector_3d_ptr)&test_vector_2);

	  // now dot each test vector with the surface normal and analyze signs

     dot_wall_1 = Dot_Product_3D((vector_3d_ptr)&test_vector_1,
                                 (vector_3d_ptr)&root->normal);


     dot_wall_2 = Dot_Product_3D((vector_3d_ptr)&test_vector_2,
                                 (vector_3d_ptr)&root->normal);


// SECTION 3  ////////////////////////////////////////////////////////////////

     // perform the tests

     // case 0, the partioning plane and the test wall have a point in common
     // this is a special case and must be accounted for, shorten the code
     // we will set a pait of flags and then the next case will handle
	  // the actual insertion of the wall into BSP

     // reset flags

     front_flag = back_flag = 0;

	  // determine of wall is tangent to endpoints of partitioning wall

	  if (POINTS_EQUAL_3D(root->wall_world[0],next_wall->wall_world[0]) )
		  {
		  // p0 of partioning plane is the same at p0 of test wall
		  // we only need to see what side p1 of test wall in on

		  if (dot_wall_2 > 0)
			  front_flag = 1;
		  else
			  back_flag = 1;

        } // end if
     else
     if (POINTS_EQUAL_3D(root->wall_world[0],next_wall->wall_world[1]) )
        {
        // p0 of partioning plane is the same at p1 of test wall
		  // we only need to see what side p0 of test wall in on

        if (dot_wall_1 > 0)
           front_flag = 1;
        else
           back_flag = 1;


        } // end if
     else
     if (POINTS_EQUAL_3D(root->wall_world[1],next_wall->wall_world[0]) )
        {
        // p1 of partioning plane is the same at p0 of test wall
        // we only need to see what side p1 of test wall in on

        if (dot_wall_2 > 0)
           front_flag = 1;
		  else
           back_flag = 1;

        } // end if
     else
     if (POINTS_EQUAL_3D(root->wall_world[1],next_wall->wall_world[1]) )
        {
        // p1 of partioning plane is the same at p1 of test wall
        // we only need to see what side p0 of test wall in on

        if (dot_wall_1 > 0)
           front_flag = 1;
        else
           back_flag = 1;

        } // end if

// SECTION 4  ////////////////////////////////////////////////////////////////

     // case 1 both signs are the same or the front or back flag has been set

     if ( (dot_wall_1 >= 0 && dot_wall_2 >= 0) || front_flag )
        {

        // highlight the wall blue

        Draw_Line(next_wall->wall_world[0].x/WORLD_SCALE_X-SCREEN_TO_WORLD_X,
                  next_wall->wall_world[0].z/WORLD_SCALE_Z-SCREEN_TO_WORLD_Z,
                  next_wall->wall_world[1].x/WORLD_SCALE_X-SCREEN_TO_WORLD_X,
                  next_wall->wall_world[1].z/WORLD_SCALE_Z-SCREEN_TO_WORLD_Z,
                  9,
                  video_buffer);

        // place this wall on the front list

        if (root->front==NULL)
           {
			  // this is the first node

           root->front      = next_wall;
           next_wall        = next_wall->link;
           front_wall       = root->front;
           front_wall->link = NULL;

           } // end if
        else
           {
           // this is the nth node

           front_wall->link = next_wall;
           next_wall        = next_wall->link;
			  front_wall       = front_wall->link;
           front_wall->link = NULL;

           } // end else

        } // end if both positive

// SECTION  5 ////////////////////////////////////////////////////////////////
     else
     if ( (dot_wall_1 < 0 && dot_wall_2 < 0) || back_flag)
        {

        // highlight the wall red

		  Draw_Line(next_wall->wall_world[0].x/WORLD_SCALE_X-SCREEN_TO_WORLD_X,
                  next_wall->wall_world[0].z/WORLD_SCALE_Z-SCREEN_TO_WORLD_Z,
                  next_wall->wall_world[1].x/WORLD_SCALE_X-SCREEN_TO_WORLD_X,
                  next_wall->wall_world[1].z/WORLD_SCALE_Z-SCREEN_TO_WORLD_Z,
                  12,
                  video_buffer);

        // place this wall on the back list

        if (root->back==NULL)
           {
           // this is the first node

           root->back      = next_wall;
			  next_wall       = next_wall->link;
           back_wall       = root->back;
           back_wall->link = NULL;

           } // end if
        else
           {
           // this is the nth node

           back_wall->link = next_wall;
           next_wall       = next_wall->link;
           back_wall       = back_wall->link;
           back_wall->link = NULL;

			  } // end else

        } // end if both negative

     // case 2 both signs are different

// SECTION 6  ////////////////////////////////////////////////////////////////

     else
     if ( (dot_wall_1 < 0 && dot_wall_2 >= 0) ||
          (dot_wall_1 >= 0 && dot_wall_2 < 0))

        {
        // the partioning plane cuts the wall in half, the wall
		  // must be split into two walls

        // extract top two vertices of test wall for ease of calculations

        wall_x0 = next_wall->wall_world[0].x;
        wall_y0 = next_wall->wall_world[0].y;
        wall_z0 = next_wall->wall_world[0].z;

        wall_x1 = next_wall->wall_world[1].x;
        wall_y1 = next_wall->wall_world[1].y;
        wall_z1 = next_wall->wall_world[1].z;

        // compute the point of intersection between the walls
        // note that x and z are the plane that the intersection takes place in

        Intersect_Lines(wall_x0,wall_z0,wall_x1,wall_z1,
                        pp_x0,pp_z0,pp_x1,pp_z1,
                        &xi,&zi);

        // here comes the tricky part, we need to slit the wall in half and
        // create two walls. We'll do this by creating two new walls,
        // placing them on the appropriate front and back lists and
        // then deleting the original wall

        // process first wall

        // allocate the memory for the wall

		  temp_wall = (wall_ptr)malloc(sizeof(wall));

        temp_wall->front = NULL;
        temp_wall->back  = NULL;
        temp_wall->link  = NULL;

        temp_wall->normal = next_wall->normal;
        temp_wall->id     = next_wall->id+1000; // add 1000 to denote a split

        // compute wall vertices

        for (index=0; index<4; index++)
            {
            temp_wall->wall_world[index].x = next_wall->wall_world[index].x;
				temp_wall->wall_world[index].y = next_wall->wall_world[index].y;
            temp_wall->wall_world[index].z = next_wall->wall_world[index].z;
            } // end for index

        // now modify vertices 1 and 2 to reflect intersection point
        // but leave y alone since it's invariant for the wall spliting

        temp_wall->wall_world[1].x = xi;
        temp_wall->wall_world[1].z = zi;

        temp_wall->wall_world[2].x = xi;
        temp_wall->wall_world[2].z = zi;

// SECTION 7  ////////////////////////////////////////////////////////////////

        // insert new wall into front or back of root

        if (dot_wall_1 >= 0)
           {

           // highlight the wall blue

           Draw_Line(temp_wall->wall_world[0].x/WORLD_SCALE_X-SCREEN_TO_WORLD_X,
                     temp_wall->wall_world[0].z/WORLD_SCALE_Z-SCREEN_TO_WORLD_Z,
                     temp_wall->wall_world[1].x/WORLD_SCALE_X-SCREEN_TO_WORLD_X,
                     temp_wall->wall_world[1].z/WORLD_SCALE_Z-SCREEN_TO_WORLD_Z,
                     9,
                     video_buffer);


           // place this wall on the front list

           if (root->front==NULL)
              {
              // this is the first node

              root->front      = temp_wall;
              front_wall       = root->front;
              front_wall->link = NULL;

              } // end if
           else
				  {
              // this is the nth node

              front_wall->link = temp_wall;
              front_wall       = front_wall->link;
              front_wall->link = NULL;

              } // end else

           } // end if positive
        else
        if (dot_wall_1 < 0)
           {
           // highlight the wall red

           Draw_Line(temp_wall->wall_world[0].x/WORLD_SCALE_X-SCREEN_TO_WORLD_X,
                     temp_wall->wall_world[0].z/WORLD_SCALE_Z-SCREEN_TO_WORLD_Z,
                     temp_wall->wall_world[1].x/WORLD_SCALE_X-SCREEN_TO_WORLD_X,
                     temp_wall->wall_world[1].z/WORLD_SCALE_Z-SCREEN_TO_WORLD_Z,
                     12,
                     video_buffer);

           // place this wall on the back list

           if (root->back==NULL)
              {
              // this is the first node

				  root->back      = temp_wall;
              back_wall       = root->back;
              back_wall->link = NULL;

              } // end if
           else
              {
              // this is the nth node

              back_wall->link = temp_wall;
              back_wall       = back_wall->link;
              back_wall->link = NULL;

              } // end else

           } // end if negative

// SECTION  8 ////////////////////////////////////////////////////////////////

        // process second wall

        // allocate the memory for the wall

        temp_wall = (wall_ptr)malloc(sizeof(wall));

        temp_wall->front = NULL;
        temp_wall->back  = NULL;
        temp_wall->link  = NULL;

        temp_wall->normal = next_wall->normal;
        temp_wall->id     = next_wall->id+1000;

        // compute wall vertices

        for (index=0; index<4; index++)
            {
            temp_wall->wall_world[index].x = next_wall->wall_world[index].x;
            temp_wall->wall_world[index].y = next_wall->wall_world[index].y;
            temp_wall->wall_world[index].z = next_wall->wall_world[index].z;
            } // end for index

        // now modify vertices 0 and 0 to reflect intersection point
		  // but leave y alone since it's invariant for the wall spliting

        temp_wall->wall_world[0].x = xi;
        temp_wall->wall_world[0].z = zi;

        temp_wall->wall_world[3].x = xi;
        temp_wall->wall_world[3].z = zi;

        // insert new wall into front or back of root

        if (dot_wall_2 >= 0)
           {
           // highlight the wall blue

			  Draw_Line(temp_wall->wall_world[0].x/WORLD_SCALE_X-SCREEN_TO_WORLD_X,
                     temp_wall->wall_world[0].z/WORLD_SCALE_Z-SCREEN_TO_WORLD_Z,
                     temp_wall->wall_world[1].x/WORLD_SCALE_X-SCREEN_TO_WORLD_X,
                     temp_wall->wall_world[1].z/WORLD_SCALE_Z-SCREEN_TO_WORLD_Z,
                     9,
                     video_buffer);

           // place this wall on the front list

           if (root->front==NULL)
              {
              // this is the first node

              root->front      = temp_wall;
				  front_wall       = root->front;
              front_wall->link = NULL;

              } // end if
           else
              {
              // this is the nth node

              front_wall->link = temp_wall;
              front_wall       = front_wall->link;
              front_wall->link = NULL;

              } // end else


           } // end if positive
        else
        if (dot_wall_2 < 0)
           {
           // highlight the wall red

           Draw_Line(temp_wall->wall_world[0].x/WORLD_SCALE_X-SCREEN_TO_WORLD_X,
                     temp_wall->wall_world[0].z/WORLD_SCALE_Z-SCREEN_TO_WORLD_Z,
                     temp_wall->wall_world[1].x/WORLD_SCALE_X-SCREEN_TO_WORLD_X,
                     temp_wall->wall_world[1].z/WORLD_SCALE_Z-SCREEN_TO_WORLD_Z,
                     12,
                     video_buffer);

			  // place this wall on the back list

           if (root->back==NULL)
              {
              // this is the first node

              root->back      = temp_wall;
              back_wall       = root->back;
              back_wall->link = NULL;

              } // end if
           else
              {
              // this is the nth node

              back_wall->link = temp_wall;
              back_wall       = back_wall->link;
              back_wall->link = NULL;

              } // end else

           } // end if negative

// SECTION  9  ////////////////////////////////////////////////////////////////

        // we are now done splitting the wall, so we can delete it

        temp_wall = next_wall;
		  next_wall = next_wall->link;
        free(temp_wall);

        } // end else

     } // end while

// SECTION  10 ////////////////////////////////////////////////////////////////

// delay a bit so user can see BSP being created

Time_Delay(5);

// recursively process front and back walls

Build_Bsp_Tree(root->front);

Build_Bsp_Tree(root->back);

} // end Build_Bsp_Tree

//////////////////////////////////////////////////////////////////////////////

void Bsp_View(wall_ptr bsp_root)
{
// this function is a self contained viewing processor that has it's own event
// loop, the display will continue to be generated until the ESC key is pressed

int done=0;

// install the isr keyboard driver

Keyboard_Install_Driver();

// change the light source direction

light_source.x =(float)0.398636;
light_source.y =(float)-0.374248;
light_source.z =(float)0.8372275;

// reset viewpoint to (0,0,0)

view_point.x = 0;
view_point.y = 0;
view_point.z = 0;

// main event loop

while(!done)
     {
     // compute starting time of this frame

     starting_time = Timer_Query();

     // erase all objects

	  Fill_Double_Buffer(0);

     // move viewpoint

     if (keyboard_state[MAKE_UP])
        view_point.y+=20;

     if (keyboard_state[MAKE_DOWN])
        view_point.y-=20;

     if (keyboard_state[MAKE_RIGHT])
        view_point.x+=20;

     if (keyboard_state[MAKE_LEFT])
		  view_point.x-=20;

     if (keyboard_state[MAKE_KEYPAD_PLUS])
        view_point.z+=20;

     if (keyboard_state[MAKE_KEYPAD_MINUS])
        view_point.z-=20;

     if (keyboard_state[MAKE_Z])
        if ((view_angle.ang_x+=10)>360)
           view_angle.ang_x = 0;

     if (keyboard_state[MAKE_A])
        if ((view_angle.ang_x-=10)<0)
			  view_angle.ang_x = 360;

     if (keyboard_state[MAKE_X])
        if ((view_angle.ang_y+=10)>360)
           view_angle.ang_y = 0;

     if (keyboard_state[MAKE_S])
        if ((view_angle.ang_y-=10)<0)
           view_angle.ang_y = 360;

     if (keyboard_state[MAKE_C])
        if ((view_angle.ang_z+=10)>360)
           view_angle.ang_z = 0;

	  if (keyboard_state[MAKE_D])
        if ((view_angle.ang_z-=10)<0)
           view_angle.ang_z = 360;

     if (keyboard_state[MAKE_ESC])
        done=1;

     // now that user has possible moved viewpoint, create the global
     // world to camera transformation matrix

     Create_World_To_Camera();

     // now convert the bsp tree world coordinates into camera coordinates

	  Bsp_World_To_Camera(bsp_root);

     // reset number of polygons in polygon list

     num_polys_frame = 0;

     // traverse the BSP tree and generate the polygon list

     Bsp_Traverse(bsp_root);

     // draw the polygon list generated by traversing the BSP tree

     Draw_Poly_List();

	  // display double buffer

     Display_Double_Buffer(double_buffer,0);

     // lock onto 18 frames per second max

     while((Timer_Query()-starting_time)<1);

     } // end while

// restore the old keyboard driver

Keyboard_Remove_Driver();

} // end Bsp_View


