
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

// D E F I N E S /////////////////////////////////////////////////////////////

#define MAX_LINES        64   // this is the maximum number of lines in the demo
                              // these lines will later become 3-D walls


#define LINE_INVALID     0    // constants that define the state of a line
#define LINE_VALID       1


#define ACTION_STARTING_LINE    0 // used to determine if user is starting a new line
#define ACTION_ENDING_LINE      1 // or ending one
#define ACTION_DELETE_LINE           2 // deleting a line

#define GADGET_WIDTH     64 // size of the gadgets (buttons)
#define GADGET_HEIGHT    12

// the pointers

#define POINTER_CROSS  0  // the cross hair pointer used to draw walls
#define POINTER_END    1  // the modified cross hair that indicates the end of
                          // a line is being drawn
#define POINTER_DEL    2  // this is a modified cross hair that indicates a line
                          // is to be deleted
#define POINTER_HAND   3  // the hand pointer used to make selections

// the control buttons sprite values

#define CONTROL_CLEAR             0
#define CONTROL_DEL_WALL          1
#define CONTROL_BUILD_BSP         2
#define CONTROL_PRINT_BSP         3
#define CONTROL_VIEW              4
#define CONTROL_EXIT              5
#define CONTROL_EDITOR            6

// the control buttons interface values

#define CONTROL_ID_CLEAR          0
#define CONTROL_ID_DEL_WALL       1
#define CONTROL_ID_BUILD_BSP      2
#define CONTROL_ID_PRINT_BSP      3
#define CONTROL_ID_VIEW           4
#define CONTROL_ID_EXIT           5
#define CONTROL_ID_EDITOR         6

// control areas of GUI

#define BSP_MIN_X           0     // the bounding rectangle of the
#define BSP_MAX_X           222   // BSP editor area
#define BSP_MIN_Y           0
#define BSP_MAX_Y           199

#define INTERFACE_MIN_X     226   // the bounding rectangle of the control
#define INTERFACE_MAX_X     319   // button area to the right of editor
#define INTERFACE_MIN_Y     0
#define INTERFACE_MAX_Y     199

// S T R U C T U R E S ///////////////////////////////////////////////////////

// this holds the ultra simple "gadget" basically a button

typedef struct gadget_typ
        {

        int x,y;   // position of gadget
        int id;    // id number of gadget

        } gadget, *gadget_ptr;

// this structure holds the 2-D line that is used to represent vertical walls
// later in the BSP creation process

typedef struct line_2d_typ
        {

        int sx,sy;       // starting point
        int ex,ey;       // ending point
        int valid;       // flags of this line is valid

        } line_2d, *line_2d_ptr;

// P R O T O T Y P E S ///////////////////////////////////////////////////////

void Convert_Lines_To_Walls(void);

int Delete_Line(int x,int y);

void Wait_For_Mouse_Up(void);

int Button_Pressed(gadget_ptr gadgets, int num_gadgets,int pos_x,int pos_y);


// G L O B A L S  ////////////////////////////////////////////////////////////

pcx_picture image_pcx,     // general PCX image used to load background and imagery
            interface_pcx; // holds interface


sprite controls_spr,         // the sprite that holds the control buttons
       pointer_spr;

// gadget lists

gadget bsp_controls[6] = { {240,94, CONTROL_ID_CLEAR},
                           {240,111,CONTROL_ID_DEL_WALL},
                           {240,128,CONTROL_ID_BUILD_BSP},
                           {240,145,CONTROL_ID_PRINT_BSP},
                           {240,162,CONTROL_ID_VIEW},
                           {240,179,CONTROL_ID_EXIT} };

line_2d lines[MAX_LINES]; // this holds the lines in the system, we could
                          // use a linked list, but let's not make this harder
                          // than it already is!

int total_lines=0;        // number of lines that have been defined

wall_ptr wall_list=NULL,  // pointer to the linked list of walls
         bsp_root=NULL;   // pointer to root of BSP tree

// F U N C T I O N S ////////////////////////////////////////////////////////

void Convert_Lines_To_Walls(void)
{
// this function converts the list of 2-D lines into a linked list of 3-D
// walls, computes the normal of each wall and sets the pointers up
// also the function labels the lines on the screen so the user can see them


int index; // looping index

wall_ptr last_wall, // used to track the last wall processed
         temp_wall; // used as a temporary to build a wall up

vector_3d u,v;  // working vectors

float length;   // a general length var

char buffer[80];

// process each 2-d line and convert it into a 3-d wall

for (index=0; index<total_lines; index++)
    {
    // draw a numeric label near the line

    sprintf(buffer,"%d",index);

    Print_String((lines[index].sx+lines[index].ex)/2,
                 (lines[index].sy+lines[index].ey)/2,
                 10,
                 buffer,
                 1);

    // allocate the memory for the wall

    temp_wall = (wall_ptr)malloc(sizeof(wall));

    // set up links

    temp_wall->link =NULL;
    temp_wall->front=NULL;
    temp_wall->back =NULL;

    // assign points, note how y and z are transposed and the y's of the
    // walls are fixed, this is because we ae looking down on the universe
    // from an aerial view and the wall height is arbitrary, however, with
    // the constants we have selected the walls are 20 units tall centered
    // about the x-z plane

    // vertex 0

    temp_wall->wall_world[0].x = WORLD_SCALE_X*(SCREEN_TO_WORLD_X+lines[index].sx);
    temp_wall->wall_world[0].y = WALL_CEILING;
    temp_wall->wall_world[0].z = WORLD_SCALE_Z*(SCREEN_TO_WORLD_Z+lines[index].sy);

    // vertex 1

    temp_wall->wall_world[1].x = WORLD_SCALE_X*(SCREEN_TO_WORLD_X+lines[index].ex);
    temp_wall->wall_world[1].y = WALL_CEILING;
    temp_wall->wall_world[1].z = WORLD_SCALE_Z*(SCREEN_TO_WORLD_Z+lines[index].ey);

    // vertex 2

    temp_wall->wall_world[2].x = WORLD_SCALE_X*(SCREEN_TO_WORLD_X+lines[index].ex);
    temp_wall->wall_world[2].y = WALL_FLOOR;
    temp_wall->wall_world[2].z = WORLD_SCALE_Z*(SCREEN_TO_WORLD_Z+lines[index].ey);

    // vertex 3

    temp_wall->wall_world[3].x = WORLD_SCALE_X*(SCREEN_TO_WORLD_X+lines[index].sx);
    temp_wall->wall_world[3].y = WALL_FLOOR;
    temp_wall->wall_world[3].z = WORLD_SCALE_Z*(SCREEN_TO_WORLD_Z+lines[index].sy);

    // compute normal to wall

    // find two vectors co-planer in the wall

    Make_Vector_3D((point_3d_ptr)&temp_wall->wall_world[0],
                   (point_3d_ptr)&temp_wall->wall_world[1],
                   (vector_3d_ptr)&u);

    Make_Vector_3D((point_3d_ptr)&temp_wall->wall_world[0],
                   (point_3d_ptr)&temp_wall->wall_world[3],
                   (vector_3d_ptr)&v);

    // use cross product to compute normal

    Cross_Product_3D((vector_3d_ptr)&u,
                     (vector_3d_ptr)&v,
                     (vector_3d_ptr)&temp_wall->normal);

    // normalize the normal vector

    length = Vector_Mag_3D((vector_3d_ptr)&temp_wall->normal);

    temp_wall->normal.x/=length;
    temp_wall->normal.y/=length;
    temp_wall->normal.z/=length;

    // set id number for debugging

    temp_wall->id = index;

    // test if this is first wall

    if (index==0)
       {
       // set head of list pointer and last wall pointer

       wall_list = temp_wall;
       last_wall = temp_wall;

       } // end if first wall
    else
       {
       // the first wall has been taken care of

       // link the last wall to the next wall

       last_wall->link = temp_wall;

       // move the last wall to the next wall

       last_wall = temp_wall;

       } // end else

    } // end for index

} // end Convert_Lines_To_Walls

/////////////////////////////////////////////////////////////////////////////

int Delete_Line(int x,int y)
{
// this function hunts thru the lines and deletes the one closest to
// the sent position (which is the center of the cross hairs

int curr_line,    // current line being processed
    sx,sy,        // starting coordinates of test line
    ex,ey,        // ending coordinates of test line
    length_line,  // total length of line being tested
    length_1,     // length of lines from endpoints of test line to target area
    length_2,
    min_x,max_x,  // bounding box of test line
    min_y,max_y,
    best_line=-1; // the best match so far in process

float best_error=10000, // start error off really large
      test_error;       // current error being processed

// process each line and find best fit

for (curr_line=0; curr_line < total_lines; curr_line++)
    {
    // extract line parameters

    sx=lines[curr_line].sx;
    sy=lines[curr_line].sy;

    ex=lines[curr_line].ex;
    ey=lines[curr_line].ey;

    // first compute length of line

    length_line = sqrt((ex-sx)*(ex-sx)+(ey-sy)*(ey-sy));

    // compute length of first endpoint to selected position

    length_1 = sqrt((x-sx)*(x-sx)+(y-sy)*(y-sy));

    // compute length of second endpoint to selected position

    length_2 = sqrt((ex-x)*(ex-x)+(ey-y)*(ey-y));

    // compute the bounding box of line

    min_x = __min(sx,ex);
    min_y = __min(sy,ey);
    max_x = __max(sx,ex);
    max_y = __max(sy,ey);

    // if the selection position is within bounding box then compute distance
    // errors and save this line as a possibility

    if (x>=min_x && x<=max_x && y>=min_y && y<=max_y)
       {
       // compute percent error of total length to length of lines
       // from endpoint to selected position

       test_error = (float)(100*abs((length_1+length_2)-length_line))
                    /(float)length_line;

       // test if this line is a better selection than the last

       if (test_error < best_error)
          {
          // make this line the "best selection" so far

          best_error = test_error;
          best_line  = curr_line;

          } // end if

       } // end if in bounding box

    } // end for curr_line

// did we get a line to delete?

if (best_line!=-1)
   {
   // delete the line from the line array by copying another line into
   // the position

   // test for special cases

   if (total_lines==1) // a single line
      total_lines=0;
   else
   if (best_line==total_lines-1)  // the line to delete is the last in array
      total_lines--;
   else
      {
      // the line to delete must be in the 0th to total_lines-1 position
      // so copy the last line into the deleted one and decrement the
      // number of lines in system

      lines[best_line] = lines[--total_lines];

      } // end else

   // redraw the interface

   PCX_Copy_To_Buffer((pcx_picture_ptr)&interface_pcx,double_buffer);

   // redraw the remaining lines

   for (curr_line=0; curr_line<total_lines; curr_line++)
       {
       // draw a line between points

       Draw_Line(lines[curr_line].sx,lines[curr_line].sy,
                 lines[curr_line].ex,lines[curr_line].ey,
                 13,
                 double_buffer);

       // plot endpoint pixels

       Write_Pixel_DB(lines[curr_line].sx,lines[curr_line].sy,15);
       Write_Pixel_DB(lines[curr_line].ex,lines[curr_line].ey,15);

       } // end for curr_line

   // return the line number that was deleted

   return(curr_line);

   } // end if
else
    return(-1);

} // end Delete_Line

//////////////////////////////////////////////////////////////////////////////

void Wait_For_Mouse_Up(void)
{
// this function waits for the mouse buttons to be released

int mx,my,      // mouse position dummy variables
    buttons=1;  // mouse buttons

// loop until mouse is released

while(buttons)
     Mouse_Control(MOUSE_POSITION_BUTTONS,&mx,&my,&buttons);

} // end Wait_For_Mouse_Release

////////////////////////////////////////////////////////////////////////////

int Button_Pressed(gadget_ptr gadgets, int num_gadgets,int pos_x,int pos_y)
{
// this function takes as input an array of gadgets (buttons) and determines
// which button is pressed, then the function returns the id number of the
// gadget or -1 if there isn't a hit

int curr_gadget; // looping variable

// loop thru all gadgets in list and try to find the right one

for (curr_gadget=0; curr_gadget<num_gadgets; curr_gadget++)
    {
    // test if sent selection position is within bounding box of one of the
    // gadgets

    if (pos_x >  gadgets[curr_gadget].x                 &&
        pos_x < (gadgets[curr_gadget].x + GADGET_WIDTH) &&
        pos_y >  gadgets[curr_gadget].y                 &&
        pos_y < (gadgets[curr_gadget].y + GADGET_HEIGHT) )
       {
       // click the button

       controls_spr.x          = gadgets[curr_gadget].x;
       controls_spr.y          = gadgets[curr_gadget].y;

       controls_spr.curr_frame = gadgets[curr_gadget].id+7;
       Sprite_Draw_Clip((sprite_ptr)&controls_spr,video_buffer,1);
       Sprite_Draw_Clip((sprite_ptr)&pointer_spr,video_buffer,1);

       // wait for user to release buttons on mouse

       Wait_For_Mouse_Up();

       controls_spr.curr_frame = gadgets[curr_gadget].id;
       Sprite_Draw_Clip((sprite_ptr)&controls_spr,video_buffer,1);

       // return the id number of the gadget

       return(gadgets[curr_gadget].id);

       } // end if hit

    } // end for curr_gadget

// return no hit

return(-1);

} // end Button_Pressed

// M A I N //////////////////////////////////////////////////////////////////

void main(void)
{
int index,            // loop variable
    mouse_x,          // mouse position
    mouse_y,
    buttons,          // state of buttons
    button_down = 0,
    exit=0,           // system exit flag
    sel,
    action = ACTION_STARTING_LINE; // this is used to track the current action the
                                   // user is in the middle of

char buffer[80];      // string input buffer

// build all look up tables for standard poly 3-d engine

Build_Look_Up_Tables();

// set the graphics mode to mode 13h

Set_Graphics_Mode(GRAPHICS_MODE13);

// create the double buffer

Create_Double_Buffer(200);

// load the imagery for control buttons

PCX_Init((pcx_picture_ptr)&image_pcx);
PCX_Load("bspbutt.pcx", (pcx_picture_ptr)&image_pcx,1);

// intialize the controls  sprite

Sprite_Init((sprite_ptr)&controls_spr,0,0,64,12,0,0,0,0,0,0);

// extract the bitmaps for control buttons

for (index=0; index<14; index++)
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,
                   (sprite_ptr)&controls_spr,index,index/7,index%7);

// done with this PCX file so delete memory associated with it

PCX_Delete((pcx_picture_ptr)&image_pcx);

// load the imagery for pointer

PCX_Init((pcx_picture_ptr)&image_pcx);
PCX_Load("bsppoint.pcx", (pcx_picture_ptr)&image_pcx,1);

// intialize the pointer sprite

Sprite_Init((sprite_ptr)&pointer_spr,0,0,9,9,0,0,0,0,0,0);

// extract the bitmaps for pointer

for (index=0; index<4; index++)
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,
                   (sprite_ptr)&pointer_spr,index,index,0);

// done with this PCX file so delete memory associated with it

PCX_Delete((pcx_picture_ptr)&image_pcx);

// set pointer to cross hair

pointer_spr.curr_frame=POINTER_CROSS;

// load the imagery for interface

PCX_Init((pcx_picture_ptr)&interface_pcx);
PCX_Load("bspint.pcx", (pcx_picture_ptr)&interface_pcx,1);

// copy PCX image to double buffer

PCX_Copy_To_Buffer((pcx_picture_ptr)&interface_pcx,double_buffer);

// reset the mouse and hide the pointer

Mouse_Control(MOUSE_RESET,NULL,NULL,&buttons);
Mouse_Control(MOUSE_HIDE,NULL,NULL,NULL);

// scan under the pointer

Sprite_Under_Clip((sprite_ptr)&pointer_spr,double_buffer);

// main event loop, process until keyboard hit

while(!exit)
     {
     // erase the pointer

     Sprite_Erase_Clip((sprite_ptr)&pointer_spr,double_buffer);

     Mouse_Control(MOUSE_POSITION_BUTTONS,&mouse_x,&mouse_y,&buttons);

     // map the mouse position to the screen and assign it to pointer

     mouse_x = pointer_spr.x = (mouse_x >> 1)-16;
     mouse_y = pointer_spr.y = mouse_y;

     // test if pointer icon needs to be changed

     if (pointer_spr.x > BSP_MAX_X)
        pointer_spr.curr_frame = POINTER_HAND;
     else
        {
        // pointer must be in bsp editor section

        // test if a line is being deleted

        if (action==ACTION_DELETE_LINE)
            pointer_spr.curr_frame = POINTER_DEL;
        else
        if (action==ACTION_STARTING_LINE)
            pointer_spr.curr_frame = POINTER_CROSS;
        else
        if (action==ACTION_ENDING_LINE)
            pointer_spr.curr_frame = POINTER_END;

        } // end else

     // test if player is trying to do something

     if (buttons==MOUSE_LEFT_BUTTON)
        {

        // figure out which area this is happening in

        if (mouse_x>=BSP_MIN_X && mouse_x<=BSP_MAX_X &&
            mouse_y>=BSP_MIN_Y && mouse_y<=BSP_MAX_Y)
           {
           // process bsp editor area click

           // test if this is starting point or endpoint

           if (action == ACTION_STARTING_LINE)
              {
              // set starting field, note the offsets to center the
              // starting point in middle of cross hairs

              lines[total_lines].sx = mouse_x + 4;
              lines[total_lines].sy = mouse_y + 4;

              // set point type to ending point

              action = ACTION_ENDING_LINE;

              // plot pixel

              Write_Pixel_DB(lines[total_lines].sx,lines[total_lines].sy,15);

              // wait for mouse button release

              Wait_For_Mouse_Up();

              } // end if start of wall
           else
           if (action == ACTION_ENDING_LINE)
              {
              // must be the end of a wall or ending point

              lines[total_lines].ex = mouse_x + 4;
              lines[total_lines].ey = mouse_y + 4;

              // set point type to ending point

              action = ACTION_STARTING_LINE;

              // draw a line between points

              Draw_Line(lines[total_lines].sx,lines[total_lines].sy,
                        lines[total_lines].ex,lines[total_lines].ey,
                        13,
                        double_buffer);

              // plot endpoint pixels

              Write_Pixel_DB(lines[total_lines].sx,lines[total_lines].sy,15);
              Write_Pixel_DB(lines[total_lines].ex,lines[total_lines].ey,15);

              // advance number of lines

              if (++total_lines>=64)
                 total_lines=64;

              // wait for mouse button release

              Wait_For_Mouse_Up();

              } // end else if
           else
           if (action==ACTION_DELETE_LINE)
              {
              // try and delete the line nearest selected point

              Delete_Line(mouse_x+4,mouse_y+4);

              // wait for mouse release

              Wait_For_Mouse_Up();

              // reset action to starting line

              action=ACTION_STARTING_LINE;

              } // end if

           } // end if in bsp editor area
        else
           {
           // must be a click in the interface control area

           sel = Button_Pressed((gadget_ptr)bsp_controls,6,mouse_x,mouse_y);

           // which button was pressed

           switch(sel)
                 {

                 case CONTROL_ID_CLEAR: // clear the screen and delete all lines
                      {
                      // reset number of lines

                      total_lines=0;

                      // reset state

                      action = ACTION_STARTING_LINE;

                      // refresh screen

                      PCX_Copy_To_Buffer((pcx_picture_ptr)&interface_pcx,double_buffer);

                      // delete BPS tree of there is one

                      if (bsp_root)
                         {
                         // delete the bsp tree

                         Bsp_Delete(bsp_root);

                         // reset pointers

                         wall_list = bsp_root = NULL;

                         } // end if

                      } break;

                 case CONTROL_ID_DEL_WALL:  // delete a single line (wall)
                      {

                      action = ACTION_DELETE_LINE;

                      } break;

                 case CONTROL_ID_BUILD_BSP: // show the building of bsp
                      {
                      // delete the last bsp tree if there is one

                      Bsp_Delete(bsp_root);

                      // convert 2-d lines to 3-d walls

                      Convert_Lines_To_Walls();

                      // build the 3-D bsp tree

                      Build_Bsp_Tree(wall_list);

                      // alias the bsp tree to the wall list first node
                      // which has now become the root of the tree

                      bsp_root = wall_list;

                      // translate the walls in the bsp tree out on the
                      // z axis a bit

                      Bsp_Translate(bsp_root,WORLD_POS_X,WORLD_POS_Y,WORLD_POS_Z);

                      // shade the walls in the bsp

                      Bsp_Shade(bsp_root);

                      // wait for user to key keyboard to continue

                      getch();

                      } break;

                 case CONTROL_ID_PRINT_BSP: // print the bsp data stucture
                      {
                      // open output file

                      fp_out = fopen("bspdemo.out","w");

                      fprintf(fp_out,"\nPrinting BSP tree:\n");

                      Bsp_Print(bsp_root);

                      fclose(fp_out);

                      } break;

                 case CONTROL_ID_VIEW: // fly thru 3-d bsp world
                      {
                      // call the self contained viewing function

                      Bsp_View(bsp_root);

                      // clear the editor out for a new bsp

                      // reset number of lines

                      total_lines=0;

                      // reset state

                      action = ACTION_STARTING_LINE;

                      // refresh screen

                      PCX_Copy_To_Buffer((pcx_picture_ptr)&interface_pcx,double_buffer);

                      // delete BPS tree of there is one

                      if (bsp_root)
                         {
                         // delete the bsp tree

                         Bsp_Delete(bsp_root);

                         // reset pointers

                         wall_list = bsp_root = NULL;

                         } // end if

                      } break;

                 case CONTROL_ID_EXIT: // exit system
                      {
                      // set exit flag

                      exit=1;

                      } break;

                 default:break;

                 } // end switch

           } // end else

        } // end if button pressed

    if (buttons==MOUSE_RIGHT_BUTTON)
       {
       // reset system to starting line

       action = ACTION_STARTING_LINE;

       } // end if

     // scan under pointer

     Sprite_Under_Clip((sprite_ptr)&pointer_spr,double_buffer);

     // draw the pointer

     Sprite_Draw_Clip((sprite_ptr)&pointer_spr,double_buffer,1);

     // draw the coordinates

     sprintf(buffer,"(%d,%d) ",(mouse_x+4)+SCREEN_TO_WORLD_X,
                              -((mouse_y+4)+SCREEN_TO_WORLD_Z));

     Print_String_DB(228,56,10,buffer,0);

     // display double buffer

     Display_Double_Buffer(double_buffer,0);

     } // end while

// free up all resources

Delete_Double_Buffer();

// restore graphics to text mode

Set_Graphics_Mode(TEXT_MODE);

} // end main

