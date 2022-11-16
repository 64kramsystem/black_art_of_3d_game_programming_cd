
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

// include all of our stuff

#include "black3.h"
#include "black4.h"
#include "black5.h"
#include "black6.h"
#include "black8.h"
#include "black9.h"

#include "blazem0.h"


////////////////////////////////////////////////////////////////////////////////

void Shield_Control(int ship,int on)
{
// this function is used to activate or de-activate the shields of the player
// or remote ship

if (ship==THE_PLAYER)
   {

   // which ship does player have?

   if (players_ship_type==GRYFON_SHIP)
      {
      // activating or de-activating shields?

      if (on)
         players_shield_color = primary_blue;
      else
         players_shield_color = primary_black;

      } // end if gryfon
   else
      {
      // activating or de-activating shields?

      if (on)
         players_shield_color = primary_red;
      else
         players_shield_color = primary_black;

       } // else raptor

   // set the color

   Write_Color_Reg(PLAYERS_SHIELD_REG,(RGB_color_ptr)&players_shield_color);

   // record shield change

   players_shields = on;

   } // end if players ship
else
   {
   // must be remote ship

   // which ship does remote have?

   if (remotes_ship_type==GRYFON_SHIP)
      {
      // activating or de-activating shields?

      if (on)
         remotes_shield_color = primary_blue;
      else
         remotes_shield_color = primary_black;
      } // end if gryfon
   else
      {
      // activating or de-activating shields?

      if (on)
         remotes_shield_color = primary_red;
      else
         remotes_shield_color = primary_black;
       } // else raptor

   // set the color

   Write_Color_Reg(REMOTES_SHIELD_REG,(RGB_color_ptr)&remotes_shield_color);

   // record shield change

   remotes_shields = on;

   } // end must be remote

} // end Shield_Control

///////////////////////////////////////////////////////////////////////////////

void Erase_Missiles(void)
{
// this function erases all the missiles

int index; // lopping variable

for (index=0; index<NUM_MISSILES; index++)
    {
    // is this missile active and visible

    if (missiles[index].state == MISS_ACTIVE && missiles[index].visible)
       {
       Write_Pixel_DB(missiles[index].sx,missiles[index].sy,missiles[index].back_color);
       } // end if alive

    } // end for index

} // end Erase_Missiles

/////////////////////////////////////////////////////////////////////////////

void Under_Missiles(void)
{
// this function scans the background under the missiles

int index,       // looping varible
    px_window,   // the starting postion of the players window
    py_window,
    mx,my;       // local aliases for speed

// compute starting position of players window so screen mapping can be done

px_window = players_x - 160 + 11;
py_window = players_y - 100 + 9;

for (index=0; index<NUM_MISSILES; index++)
    {
    // is this missile active

    if (missiles[index].state == MISS_ACTIVE)
       {
       // remap to screen coordinates

       mx=missiles[index].sx = (missiles[index].x - px_window);
       my=missiles[index].sy = (missiles[index].y - py_window);

       // test if missile is visible on screen?

       if (mx>=320 || mx <0 || my>=200  || my<0)
          {

          // this missile is invisible and has been clipped

          missiles[index].visible = 0;

          // process next missile

          continue;

          } // end visibility test

       // scan under missile

       missiles[index].back_color =
                      Read_Pixel_DB(mx,my);

       // set visibility flag

       missiles[index].visible = 1;

       } // end if alive

    } // end for index

} // end Under_Missiles

/////////////////////////////////////////////////////////////////////////////

void Draw_Missiles(void)
{
// this function draws all the missiles
int index; // looping variable

for (index=0; index<NUM_MISSILES; index++)
    {
    // is this missile active and visible

    if (missiles[index].state == MISS_ACTIVE && missiles[index].visible)
       {

       Write_Pixel_DB(missiles[index].sx,missiles[index].sy,missiles[index].color);

       } // end if alive

    } // end for index

} // end Draw_Missiles

/////////////////////////////////////////////////////////////////////////////

void Init_Missiles(void)
{
// this function resets and intializes all missiles
int index; // looping variable

for (index=0; index<NUM_MISSILES; index++)
    missiles[index].state = MISS_INACTIVE;

} // Init_Missiles

////////////////////////////////////////////////////////////////////////////

void Move_Missiles(void)
{
// this function move all the missiles and performs collision detection

int index,              // looping variables
    a_index,
    miss_x,miss_y;      // speed aliases

// process each missile

for (index=0; index<NUM_MISSILES; index++)
    {

    // is missile active

    if (missiles[index].state == MISS_ACTIVE)
       {
       // move the missile

       miss_x = (missiles[index].x += missiles[index].xv);
       miss_y = (missiles[index].y += missiles[index].yv);

       // test if a missile has hit an asteroid

       for (a_index=0; a_index<NUM_ASTEROIDS; a_index++)
           {
           // test if asteroid is active

           if (asteroids[a_index].rock.state==ASTEROID_ACTIVE)
              {
              // test for collision

              if (miss_x >= asteroids[a_index].x &&
                  miss_y >= asteroids[a_index].y &&
                  miss_x <= asteroids[a_index].x + asteroids[a_index].rock.width &&
                  miss_y <= asteroids[a_index].y + asteroids[a_index].rock.height)
                  {
                  // kill the asteroid and the missile

                  asteroids[a_index].rock.state = ASTEROID_INACTIVE;

                  missiles[index].state = MISS_INACTIVE;

                  // what kind of asteroid did we have?

                  switch(asteroids[a_index].type)
                        {

                        case ASTEROID_LARGE:
                             {
                             // start an explosion at proper place

                             Start_Explosion(asteroids[a_index].x,
                                             asteroids[a_index].y,2);

                             // start one medium and one small asteroid
                             //  (if possible)

                             Start_Asteroid(asteroids[a_index].x,
                                            asteroids[a_index].y,
                                            ASTEROID_MEDIUM);

                             Start_Asteroid(asteroids[a_index].x+ASTEROID_LARGE_WIDTH/2,
                                            asteroids[a_index].y+ASTEROID_LARGE_HEIGHT/2,
                                            ASTEROID_SMALL);

                             if (missiles[index].type==PLAYER_MISSILE)
                                players_score+=100;
                             else
                                remotes_score+=100;


                             } break;

                        case ASTEROID_MEDIUM:
                             {
                             // start an explosion at proper place

                             Start_Explosion(asteroids[a_index].x-2,
                                             asteroids[a_index].y-2,2);


                             // start two small asteroids (if possible)

                             Start_Asteroid(asteroids[a_index].x,
                                            asteroids[a_index].y,
                                            ASTEROID_SMALL);

                             Start_Asteroid(asteroids[a_index].x+ASTEROID_MEDIUM_WIDTH/2,
                                            asteroids[a_index].y+ASTEROID_MEDIUM_HEIGHT/2,
                                            ASTEROID_SMALL);

                             if (missiles[index].type==PLAYER_MISSILE)
                                players_score+=50;
                             else
                                remotes_score+=50;

                             } break;

                        case ASTEROID_SMALL:
                             {
                             // start an explosion at proper place

                             Start_Explosion(asteroids[a_index].x-8,
                                             asteroids[a_index].y-8,2);

                             // start a randomly positioned asteroid of any size
                             // at the worm hole

                             Start_Asteroid(WORMHOLE_X,WORMHOLE_Y,rand()%3);

                             if (missiles[index].type==PLAYER_MISSILE)
                                players_score+=25;
                             else
                                remotes_score+=25;

                             } break;

                        default:break;

                        } // end switch

                  // break inner loop

                  break;

                  } // end if collision

              } // end if active

           } // end for a_index

       // test if missiles hit local player

       if (linked && players_state==ALIVE && missiles[index].type==REMOTE_MISSILE)
       {
       if (miss_x>players_x && miss_x < players_x+SHIP_WIDTH &&
           miss_y>players_y && miss_y < players_y+SHIP_HEIGHT)
           {

           // de-activate missile

           missiles[index].state = MISS_INACTIVE;

           // were shields up?

           if (players_shields)
              {
              // start explosion
              Start_Explosion(players_x,players_y,2);
              } // end if shields up
           else
              { // say bye bye!

              Start_Explosion(players_x,players_y,2);
              Start_Nova(players_x+SHIP_WIDTH/2,players_y+SHIP_HEIGHT/2);

              // start local player death sequence

              Start_Players_Death();

              } // end if else

           } // end if local player hit

       } // end if remote fired this missile

       // test if missile has hit remote player

       if (linked && remotes_state==ALIVE && missiles[index].type==PLAYER_MISSILE)
          {
          if (miss_x>remotes_x && miss_x < remotes_x+SHIP_WIDTH &&
              miss_y>remotes_y && miss_y < remotes_y+SHIP_HEIGHT)
              {

              // de-activate missile

              missiles[index].state = MISS_INACTIVE;

              // were shields up?

              if (remotes_shields)
                 {
                 // start explosion
                 Start_Explosion(remotes_x,remotes_y,2);
                 } // end if shields up
              else
                 { // say bye bye!

                 Start_Explosion(remotes_x,remotes_y,2);
                 Start_Nova(remotes_x+SHIP_WIDTH/2,remotes_y+SHIP_HEIGHT/2);

                 // start remote death sequence

                 Start_Remotes_Death();

                 } // end if else

              } // end if remote player hit

          } // end if remote is linked

       // test if it's hit the edge of the screen or a wall

       if ( (miss_x >= UNIVERSE_WIDTH+UNIVERSE_BORDER)  ||
            (miss_x < -UNIVERSE_BORDER)                 ||
            (miss_y >  UNIVERSE_HEIGHT+UNIVERSE_BORDER) ||
            (miss_y < -UNIVERSE_BORDER)                 ||
            (--missiles[index].lifetime < 0))
          {
          // de-activate the missile

          missiles[index].state = MISS_INACTIVE;

          } // end if off edge of screen

       // test if this missiles has been terminated in some way, if so update
       // the active missile variable for the player or remote

       if (missiles[index].state == MISS_INACTIVE)
          {
          // update number of active missiles

          if (missiles[index].type == PLAYER_MISSILE)
             players_active_missiles--;
          else
          if (missiles[index].type == REMOTE_MISSILE)
             remotes_active_missiles--;

          } // end if this missiles was terminated on this cycle

       } // end if alive

    } // end for index

} // end Move_Missiles

/////////////////////////////////////////////////////////////////////////////

int Start_Missile(int x,int y,int xv,int yv, int color,int type)
{
// this function start a photon missile with the given position and speed

int index; // looping variable

// scan for an inactive

for (index=0; index<NUM_MISSILES; index++)
    {
    // is this missile free?

    if (missiles[index].state == MISS_INACTIVE)
       {
       // set up fields

       missiles[index].state       = MISS_ACTIVE;
       missiles[index].x           = x;
       missiles[index].y           = y;
       missiles[index].sx          = 0;
       missiles[index].sy          = 0;
       missiles[index].counter     = 0;
       missiles[index].threshold   = 0;
       missiles[index].xv          = xv;
       missiles[index].yv          = yv;
       missiles[index].color       = color;
       missiles[index].back_color  = 0;
       missiles[index].type        = type;
       missiles[index].visible     = 0;
       missiles[index].lifetime    = 30 + rand()%10;

       // make some sound

       Digital_FX_Play(BLZLAS_VOC,2);

       // return success

       return(1);

       } // end if found a good one

    } // end for index

// must not have found one

return(0);

} // end Start_Missile

/////////////////////////////////////////////////////////////////////////////

void Start_Players_Death(void)
{
// this function starts the players death sequence

players_xv                  = 0;
players_yv                  = 0;
players_engine              = 0;
players_flame_count         = 0;
players_gravity             = 0;
players_shields             = 0;
players_shield_time         = 0;
players_cloak               = -1;
players_state               = DYING;
players_death_count         = 48;


debounce_hud                = 0;
debounce_scan               = 0;
debounce_cloak              = 0;
debounce_thrust             = 0;
debounce_fire               = 0;
debounce_shields            = 0;

} // end Start_Players_Death

//////////////////////////////////////////////////////////////////////////////

void Reset_Player(void)
{
// this function resets the player to his starting position

players_last_x              = game_start_x[master];
players_last_y              = game_start_y[master];
players_x                   = game_start_x[master];
players_y                   = game_start_y[master];
players_state               = ALIVE;

} // end Reset_Player

//////////////////////////////////////////////////////////////////////////////

void Reset_Remote(void)
{
// this function resets the remote to his starting position

remotes_last_x              = game_start_x[slave];
remotes_last_y              = game_start_y[slave];
remotes_x                   = game_start_x[slave];
remotes_y                   = game_start_y[slave];
remotes_state               = ALIVE;

} // end Reset_Remote

//////////////////////////////////////////////////////////////////////////////

void Start_Remotes_Death(void)
{
// this function starts the remotes death sequence

remotes_xv                  = 0;
remotes_yv                  = 0;
remotes_engine              = 0;
remotes_flame_count         = 0;
remotes_gravity             = 0;
remotes_shields             = 0;
remotes_shield_time         = 0;
remotes_cloak               = -1;
remotes_state               = DYING;
remotes_death_count         = 48;

} // end Start_Remotes_Death

//////////////////////////////////////////////////////////////////////////////

void Reset_System(void)
{
// this function resets everything so the game can be ran again
// I hope I didn't leave anything out?

// player variables

winner = WINNER_NONE;      // the winner of the game

players_last_x              = game_start_x[master];
players_last_y              = game_start_y[master];
players_x                   = game_start_x[master];
players_y                   = game_start_y[master];
players_dx                  = 0;
players_dy                  = 0;
players_xv                  = 0;
players_yv                  = 0;
players_engine              = 0;
players_stability           = 8;
players_flame_count         = 0;
players_flame_time          = 1;
players_ship.curr_frame     = 0;
players_gravity             = 0;
players_shields             = 0;
players_shield_time         = 0;
players_cloak               = -1;
players_heads               = -1;
players_comm                = -1;
players_scanner             = -1;
players_num_ships           = 3;
players_shield_strength     = 22000;
players_energy              = 22000;
players_score               = 0;
players_active_missiles     = 0;
players_state               = ALIVE;
players_death_count         = 0;

debounce_hud                = 0;
debounce_scan               = 0;
debounce_cloak              = 0;
debounce_thrust             = 0;
debounce_fire               = 0;
debounce_shields            = 0;

refresh_heads = 0;

// remote variables

remotes_last_x          = game_start_x[slave];
remotes_last_y          = game_start_y[slave];
remotes_x               = game_start_x[slave];
remotes_y               = game_start_y[slave];
remotes_dx              = 0;
remotes_dy              = 0;
remotes_xv              = 0;
remotes_yv              = 0;
remotes_engine          = 0;
remotes_stability       = 8;
remotes_flame_count     = 0;
remotes_flame_time      = 1;
remotes_gravity         = 0;
remotes_shields         = 0;
remotes_shield_time     = 0;
remotes_cloak           = -1;
remotes_heads           = -1;
remotes_comm            = -1;
remotes_scanner         = -1;
remotes_num_ships       = 3;
remotes_shield_strength = 22000;
remotes_energy          = 22000;
remotes_score           = 0;
remotes_active_missiles = 0;
remotes_state           = ALIVE;
remotes_death_count    = 0;

} // end Reset_System

///////////////////////////////////////////////////////////////////////////////

void Panel_FX(void)
{
// this function performs all of the special effects for the control panel

int index; // lopping variable

static int panel_counter = 0;  // used to time the color rotation of the panel

// is it time to update colors?

if (++panel_counter>2)
   {
   // reset counter

   panel_counter=0;

   // do animation to colors

   Read_Color_Reg(END_PANEL_REG, (RGB_color_ptr)&color_1);

   for (index=END_PANEL_REG; index>START_PANEL_REG; index--)
       {
       // read the (i-1)th register

       Read_Color_Reg(index-1, (RGB_color_ptr)&color_2);

       // assign it to the ith

       Write_Color_Reg(index, (RGB_color_ptr)&color_2);

       } // end rotate loop

    // place the value of the first color register into the last to
    // complete the rotation

    Write_Color_Reg(START_PANEL_REG, (RGB_color_ptr)&color_1);

   } // end if time

} // end Panel_FX

////////////////////////////////////////////////////////////////////////////////

void Start_Explosion(int x,int y,int speed)
{
// this function stars a generic explosion

int index;  // looping variable

// scan for a useable explosion

for (index=0; index<NUM_EXPLOSIONS; index++)
    {

    if (explosions[index].state == EXPLOSION_INACTIVE)
       {

       // set up fields

       explosions[index].state       = EXPLOSION_ACTIVE;
       explosions[index].x           = 0; // screen coordinates
       explosions[index].y           = 0;
       explosions[index].counter_2   = x; // the counters will be used as universe
       explosions[index].counter_3   = y; // coordinates


       explosions[index].curr_frame  = 0;
       explosions[index].threshold_1 = speed;
       explosions[index].counter_1   = 0;

       // make some sound

       Digital_FX_Play(BLZEXP1_VOC,1);

       break; // exit loop

       } // end if found a good one

    } // end for index

} // end Start_Explosion

/////////////////////////////////////////////////////////////////////////////

void Under_Explosions(void)
{

// this function scans under the explosions


int index,     // looping index
    px_window, // the starting postion of the players window
    py_window;

// compute starting position of players window so screen mapping can be done

px_window = players_x - 160+11;
py_window = players_y - 100+9;

// scan for a running explosions

for (index=0; index<NUM_EXPLOSIONS; index++)
    {

    if (explosions[index].state == EXPLOSION_ACTIVE)
       {

       // postion explosion correctly on view screen, note this is very similar
       // to what we will do in 3-D when we translate all the objects in the
       // universe to the viewer position, note counter_2 and counter_3
       // in the sprite structure are used as universe or world x,y

       explosions[index].x = (explosions[index].counter_2-px_window);
       explosions[index].y = (explosions[index].counter_3-py_window);

       Sprite_Under_Clip((sprite_ptr)&explosions[index],double_buffer);

       } // end if found a good one

    } // end for index

} // end Under_Explosions

/////////////////////////////////////////////////////////////////////////////

void Erase_Explosions(void)
{
// this function erases all the current explosions

int index;

// scan for a useable explosion

for (index=0; index<NUM_EXPLOSIONS; index++)
    {

    if (explosions[index].state == EXPLOSION_ACTIVE)
       {
       Sprite_Erase_Clip((sprite_ptr)&explosions[index],double_buffer);

       } // end if found a good one

    } // end for index

} // end Erase_Explosions

/////////////////////////////////////////////////////////////////////////////

void Draw_Explosions(void)
{
// this function draws the explosion

int index; // looping variable

// scan for a useable explosion

for (index=0; index<NUM_EXPLOSIONS; index++)
    {

    // make sure this explosion is alive

    if (explosions[index].state == EXPLOSION_ACTIVE)
       {

       Sprite_Draw_Clip((sprite_ptr)&explosions[index],double_buffer,1);

       } // end if found a good one

    } // end for index

} // end Draw_Explosions

/////////////////////////////////////////////////////////////////////////////

void Animate_Explosions(void)
{
// this function aniamtes the explosions

int index;  // looping index

// scan for a useable explosion

for (index=0; index<NUM_EXPLOSIONS; index++)
    {
    // test if explosion is active

    if (explosions[index].state == EXPLOSION_ACTIVE)
       {
       // test if it's time to change frames

       if (++explosions[index].counter_1 >= explosions[index].threshold_1)
          {
          // is the explosion over?

          if (++explosions[index].curr_frame == NUM_EXPLOSION_FRAMES)
             explosions[index].state = EXPLOSION_INACTIVE;

          // reset animation clock for future

          explosions[index].counter_1 = 0;

          } // end if time to change frames

       } // end if found a good one

    } // end for index

} // end Animate_Explosions

//////////////////////////////////////////////////////////////////////////////

void Init_Explosions(void)
{
// clear out the states of all explosions

int index; // looping variable

for (index=0; index<NUM_EXPLOSIONS; index++)
    explosions[index].state = EXPLOSION_INACTIVE;

} // Init_Explosions

/////////////////////////////////////////////////////////////////////////////

void Load_Wormhole(void)
{
// this function loads in the imagery for the worm hole

int index; // looping variable

// load the imagery for the worm hole

PCX_Init((pcx_picture_ptr)&image_pcx);
PCX_Load("blazewrm.pcx", (pcx_picture_ptr)&image_pcx,1);

// initialize the wormhole sprite

Sprite_Init((sprite_ptr)&wormhole,0,0,26,22,0,0,0,0,0,0);

// extract the animation frames

for (index=0; index<NUM_WORMHOLE_FRAMES; index++)
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&wormhole,index,index,0);

PCX_Delete((pcx_picture_ptr)&image_pcx);

} // end Load_Wormhole

/////////////////////////////////////////////////////////////////////////////

void Init_Wormhole(void)
{
// this resets all the worm hole parameters


// set screen coordinates to 0

wormhole.x = 0;
wormhole.y = 0;

// set universe coordinates to proper position

wormhole.counter_2 = WORMHOLE_X; // note the counters are being used for
wormhole.counter_3 = WORMHOLE_Y; // universe positions

// reset the frame counter

wormhole.curr_frame = 0;

// these will be used to time animation

wormhole.counter_1 = 0;
wormhole.threshold_1 = 2;

} // end Init_Wormhole

/////////////////////////////////////////////////////////////////////////////

void Under_Wormhole(void)
{

// this function scans under the wormhole


int px_window, // the starting postion of the players window
    py_window;

// compute starting position of players window so screen mapping can be done

px_window = players_x - 160+11;
py_window = players_y - 100+9;

// translate wormhole to screen coordinates

wormhole.x = (wormhole.counter_2-px_window);
wormhole.y = (wormhole.counter_3-py_window);

Sprite_Under_Clip((sprite_ptr)&wormhole,double_buffer);

} // end Under_Wormhole

/////////////////////////////////////////////////////////////////////////////

void Erase_Wormhole(void)
{
// this function erase the wormhole

Sprite_Erase_Clip((sprite_ptr)&wormhole,double_buffer);

} // end Erase_Wormhole

/////////////////////////////////////////////////////////////////////////////

void Draw_Wormhole(void)
{
// this function draws the wormhole

Sprite_Draw_Clip((sprite_ptr)&wormhole,double_buffer,1);

} // end Draw_Wormhole

/////////////////////////////////////////////////////////////////////////////

void Animate_Wormhole(void)
{
// this function animates the wormhole

if (++wormhole.counter_1 >= wormhole.threshold_1)
   {
   // time to reset frame counter?

   if (++wormhole.curr_frame == NUM_WORMHOLE_FRAMES)
       wormhole.curr_frame = 0;

    // reset animation clock for future

    wormhole.counter_1 = 0;

    } // end if time to change frames

} // end Animate_Wormhole

/////////////////////////////////////////////////////////////////////////////

void Load_Fuel_Cells(void)
{
// this function loads in the imagery for the fuel cells

int index,  // looping variable
    frames; // looping variable

// load the imagery for the fuel cells

PCX_Init((pcx_picture_ptr)&image_pcx);
PCX_Load("blazeful.pcx", (pcx_picture_ptr)&image_pcx,1);

// initialize the fuel cells sprite and load bitmaps

for (index=0; index<NUM_FUEL_CELLS; index++)
    {

    Sprite_Init((sprite_ptr)&fuel_cells[index],0,0,20,18,0,0,0,0,0,0);

    // extract the animation frames

    for (frames=0; frames<NUM_FUEL_FRAMES; frames++)
        PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,
                       (sprite_ptr)&fuel_cells[index],frames,frames,0);

    } // end for index

PCX_Delete((pcx_picture_ptr)&image_pcx);

} // end Load_Fuel_Cells

/////////////////////////////////////////////////////////////////////////////

void Init_Fuel_Cells(void)
{
// this resets all the fuel cell parameters

int index; // looping variable

for (index=0; index<NUM_FUEL_CELLS; index++)
    {

    // set state

    fuel_cells[index].state = FUEL_CELL_ACTIVE;

    // set screen coordinates to 0

    fuel_cells[index].x = 0;
    fuel_cells[index].y = 0;

    // set universe coordinates to random position

    fuel_cells[index].counter_2 = 200+rand()%(UNIVERSE_WIDTH-400);
    fuel_cells[index].counter_3 = 200+rand()%(UNIVERSE_HEIGHT-400);

    // reset the frame counter

    fuel_cells[index].curr_frame = 0;

    // these will be used to time animation

    fuel_cells[index].counter_1 = 0;
    fuel_cells[index].threshold_1 = 2;

    } // end index

} // end Init_Fuel_Cells

/////////////////////////////////////////////////////////////////////////////

void Under_Fuel_Cells(void)
{
// this function scans under the Fuel cells


int px_window, // the starting postion of the players window
    py_window,
    index;     // looping variable

// compute starting position of players window so screen mapping can be done

px_window = players_x - 160+11;
py_window = players_y - 100+9;

// process each fuel cell

for (index=0; index<NUM_FUEL_CELLS; index++)
    {
    // test if fuel cell is active

    if (fuel_cells[index].state == FUEL_CELL_ACTIVE)
       {
       // translate Fuelcells to screen coordinates

       fuel_cells[index].x = (fuel_cells[index].counter_2-px_window);
       fuel_cells[index].y = (fuel_cells[index].counter_3-py_window);

       Sprite_Under_Clip((sprite_ptr)&fuel_cells[index],double_buffer);

       } // end fi active

    } // end for index

} // end Under_Fuel_Cells

/////////////////////////////////////////////////////////////////////////////

void Erase_Fuel_Cells(void)
{
// this function erase the Fuelcells

int index; // looping variable

for (index=0; index<NUM_FUEL_CELLS; index++)
    {
    // test if fuel cell is active

    if (fuel_cells[index].state == FUEL_CELL_ACTIVE)
       {

       Sprite_Erase_Clip((sprite_ptr)&fuel_cells[index],double_buffer);

       } // end if active

    } // end for index

} // end Erase_Fuel_Cells

/////////////////////////////////////////////////////////////////////////////

void Draw_Fuel_Cells(void)
{
// this function draws the Fuel cells

int index; // looping variable

for (index=0; index<NUM_FUEL_CELLS; index++)
    {
    // test if fuel cell is active

    if (fuel_cells[index].state == FUEL_CELL_ACTIVE)
       {

       Sprite_Draw_Clip((sprite_ptr)&fuel_cells[index],double_buffer,1);

       } // end if active

    } // end for index

} // end Draw_Fuel_Cells

/////////////////////////////////////////////////////////////////////////////

void Animate_Fuel_Cells(void)
{
// this function animates the fuel cells and test for collision

int index; // looping variable

for (index=0; index<NUM_FUEL_CELLS; index++)
    {
    // test if fuel cell is active

    if (fuel_cells[index].state == FUEL_CELL_ACTIVE)
       {
       // move cell to right

       if (++fuel_cells[index].counter_2 > (UNIVERSE_WIDTH+UNIVERSE_BORDER))
          fuel_cells[index].counter_2 = -UNIVERSE_BORDER;

       // perform animation

       if (++fuel_cells[index].counter_1 >= fuel_cells[index].threshold_1)
          {
          // time to reset frame counter?

          if (++fuel_cells[index].curr_frame == NUM_FUEL_FRAMES)
              fuel_cells[index].curr_frame = 0;

           // reset animation clock for future

           fuel_cells[index].counter_1 = 0;

          } // end if time to change frames

       } // end if active

    } // end for index

} // end Animate_Fuel_Cells

//////////////////////////////////////////////////////////////////////////////

void Load_Alien(void)
{
// this function loads in the imagery for the alien

int index; // looping variable

// load the imagery for the alien

PCX_Init((pcx_picture_ptr)&image_pcx);
PCX_Load("blazealn.pcx", (pcx_picture_ptr)&image_pcx,1);

// initialize the alien sprite

Sprite_Init((sprite_ptr)&alien.body,0,0,14,8,0,0,0,0,0,0);

// extract the animation frames

for (index=0; index<NUM_ALIEN_FRAMES; index++)
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,(sprite_ptr)&alien.body,index,index,0);


PCX_Delete((pcx_picture_ptr)&image_pcx);

} // end Load_Alien

/////////////////////////////////////////////////////////////////////////////

void Init_Alien(void)
{
// this resets all the alien parameters

// set screen coordinates to 0

alien.body.x = 0;
alien.body.y = 0;

// set universe coordinates to proper position

alien.x = 0;
alien.y = 0;

// reset the frame counter

alien.body.curr_frame = 0;

// these will be used to time animation

alien.body.counter_1 = 0;
alien.body.threshold_1 = 2;

// set state to active

alien.state = ALIEN_INACTIVE;

} // end Init_Alien

//////////////////////////////////////////////////////////////////////////////

void Alien_Control(void)
{
// this function will be called every cycle and decide if an alien
// will start up

// make sure alien is dead

if (alien.state==ALIEN_INACTIVE)
   {
   // throw a coin

   if ((rand()%ALIEN_ODDS)==1)
      {
      // position alien and set appropriate fields

      alien.body.x = 0; // screen coords
      alien.body.y = 0;

      // start alien from wormhole (universe coords)

      alien.x = WORMHOLE_X; // note the counters are being used for
      alien.y = WORMHOLE_Y; // universe positions

      // reset the frame counter

      alien.body.curr_frame = 0;

      // this will be used to time animation

      alien.body.counter_1   = 0;
      alien.body.threshold_1 = 2;

      // set state to random to start with

      alien.state = ALIEN_RANDOM;

      // select a random direction

      alien.xv = -4 + rand()%8;
      alien.yv = -4 + rand()%8;

      // counter 2 will be used to track how long to stay in a state

      alien.body.counter_2 = 50;

      } // end if start one

   } // end if dead

} // end Alien_Control

/////////////////////////////////////////////////////////////////////////////

void Under_Alien(void)
{
// this function scans under the alien

int px_window, // the starting postion of the players window
    py_window;

if (alien.state!=ALIEN_INACTIVE)
   {
   // compute starting position of players window so screen
   // mapping can be done

   px_window = players_x - 160+11;
   py_window = players_y - 100+9;

   // translate alien to screen coordinates

   alien.body.x = (alien.x-px_window);
   alien.body.y = (alien.y-py_window);

   // perform scan in screen coords

   Sprite_Under_Clip((sprite_ptr)&alien.body,double_buffer);

   } // end if alien alive

} // end Under_Alien

/////////////////////////////////////////////////////////////////////////////

void Erase_Alien(void)
{
// this function erases the alien

if (alien.state!=ALIEN_INACTIVE)
    Sprite_Erase_Clip((sprite_ptr)&alien.body,double_buffer);

} // end Erase_Alien

/////////////////////////////////////////////////////////////////////////////

void Draw_Alien(void)
{
// this function draws the alien

if (alien.state!=ALIEN_INACTIVE)
   Sprite_Draw_Clip((sprite_ptr)&alien.body,double_buffer,1);

} // end Draw_Alien

/////////////////////////////////////////////////////////////////////////////

void Move_Alien(void)
{
// this function moves the alien (if there is one)

if (alien.state!=ALIEN_INACTIVE)
   {

   // what state is alien in?

   switch(alien.state)
         {

         case ALIEN_RANDOM:
              {

              // move alien in direction

              alien.x+=alien.xv;
              alien.y+=alien.yv;

              } break;

         case ALIEN_CHASE_PLAYER:
              {


              } break;

         case ALIEN_CHASE_REMOTE:
              {


              } break;

          default:break;

         } // end switch

   // decrement state counter

   if (--alien.body.counter_2<=0)
      {
      // select a new state

      alien.state = ALIEN_RANDOM;   // change this line later

      // set up new state

      switch(alien.state)
            {
            case ALIEN_RANDOM:
                 {
                 // select new random direction and state time

                 alien.xv = -4 + rand()%8;
                 alien.yv = -4 + rand()%8;

                 // counter 2 will be used to track how long to stay in a state

                 alien.body.counter_2 = 25+rand()%75;

                 } break;

            case ALIEN_CHASE_PLAYER:
                 {
                 // this bud's for you!
                 } break;

            case ALIEN_CHASE_REMOTE:
                 {
                 // this is an exercise
                 } break;

            default:break;

            } // end switch

      } // end if time to select a new state

   // animate alien

   if (++alien.body.counter_1 >=alien.body.threshold_1)
      {
      // change animation frames

      if (++alien.body.curr_frame==NUM_ALIEN_FRAMES)
         alien.body.curr_frame = 0;

      // reset animation counter

      alien.body.counter_1 = 0;

      } // end if time to animate

   // do collision detection

   if ((alien.x>UNIVERSE_WIDTH+UNIVERSE_BORDER)  ||
       (alien.x<-UNIVERSE_BORDER)                ||
       (alien.y>UNIVERSE_HEIGHT+UNIVERSE_BORDER) ||
       (alien.y<-UNIVERSE_BORDER))
      {
      // kill alien

      alien.state = ALIEN_INACTIVE;

      } // end if alien has warped off universe edge

   } // end if alien alive

} // end Move_Alien

///////////////////////////////////////////////////////////////////////////////

void Load_Heads(void)
{
// this function loads various icons for the heads up display

int index; // looping variable

// load the imagery for the icons for display

PCX_Init((pcx_picture_ptr)&image_pcx);
PCX_Load("blazehu1.pcx", (pcx_picture_ptr)&image_pcx,1);

// intialize the button sprite

Sprite_Init((sprite_ptr)&heads_text,0,0,34,6,0,0,0,0,0,0);

// extract the bitmaps for heads up text

for (index=0; index<7; index++)
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,
                   (sprite_ptr)&heads_text,index,index,0);

// delete pcx file

PCX_Delete((pcx_picture_ptr)&image_pcx);

// load the imagery for the icons for display

PCX_Init((pcx_picture_ptr)&image_pcx);
PCX_Load("blazehu2.pcx", (pcx_picture_ptr)&image_pcx,1);

// intialize the button sprite

Sprite_Init((sprite_ptr)&heads_numbers,0,0,8,6,0,0,0,0,0,0);

// extract the bitmaps for heads up text

for (index=0; index<7; index++)
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,
                   (sprite_ptr)&heads_numbers,index,index,0);

// delete pcx file

PCX_Delete((pcx_picture_ptr)&image_pcx);

// load the imagery for the icons for display

PCX_Init((pcx_picture_ptr)&image_pcx);
PCX_Load("blazehu1.pcx", (pcx_picture_ptr)&image_pcx,1);

// intialize the button sprite

Sprite_Init((sprite_ptr)&heads_gauge,0,0,34,6,0,0,0,0,0,0);

// extract the bitmaps for heads up gauges

for (index=0; index<23; index++)
    PCX_Get_Sprite((pcx_picture_ptr)&image_pcx,
                   (sprite_ptr)&heads_gauge,index,index%9,2+index/9);

// delete pcx file

PCX_Delete((pcx_picture_ptr)&image_pcx);

} // end Load_Heads

////////////////////////////////////////////////////////////////////////////

void Init_Heads(void)
{
// do nothing for now


} // end Init_Heads

////////////////////////////////////////////////////////////////////////////

void Draw_Heads(void)
{
// this function draws the heads up display during the tmie when
// the game background can be modified

int index; // looping variable

heads_text.x = LEFT_HEADS_TEXT_X;
heads_text.y = LEFT_HEADS_TEXT_Y;

// draw the left most messages first

heads_text.curr_frame = HEADS_CLOAK;

for (index=0; index<4; index++)
    {
    Sprite_Draw((sprite_ptr)&heads_text,double_buffer,0);

    // move down a row

    heads_text.y+=8;

    // select next message

    heads_text.curr_frame++;

    } //  end for index

// now draw buttons and numbers

heads_numbers.x = LEFT_HEADS_TEXT_X+40;
heads_numbers.y = LEFT_HEADS_TEXT_Y;

// draw cloaked button

if (players_cloak==1)
   {
   heads_numbers.curr_frame=2;
   Sprite_Draw((sprite_ptr)&heads_numbers,double_buffer,0);
   }
else
   {
   heads_numbers.curr_frame=1;
   Sprite_Draw((sprite_ptr)&heads_numbers,double_buffer,0);
   } // end else

// draw scanner enable

heads_numbers.x = LEFT_HEADS_TEXT_X+40;
heads_numbers.y = LEFT_HEADS_TEXT_Y+8;

if (players_scanner==1)
   {
   heads_numbers.curr_frame=2;
   Sprite_Draw((sprite_ptr)&heads_numbers,double_buffer,0);
   }
else
   {
   heads_numbers.curr_frame=1;
   Sprite_Draw((sprite_ptr)&heads_numbers,double_buffer,0);
   } // end else

// draw communications

heads_numbers.x = LEFT_HEADS_TEXT_X+40;
heads_numbers.y = LEFT_HEADS_TEXT_Y+16;

if (linked)
   {
   heads_numbers.curr_frame=2;
   Sprite_Draw((sprite_ptr)&heads_numbers,double_buffer,0);
   }
else
   {
   heads_numbers.curr_frame=1;
   Sprite_Draw((sprite_ptr)&heads_numbers,double_buffer,0);
   } // end else

// draw number of ships

heads_numbers.x = LEFT_HEADS_TEXT_X+40;
heads_numbers.y = LEFT_HEADS_TEXT_Y+24;

heads_numbers.curr_frame=3+players_num_ships;
Sprite_Draw((sprite_ptr)&heads_numbers,double_buffer,0);

// now draw right most messages

heads_text.x = RIGHT_HEADS_TEXT_X;
heads_text.y = RIGHT_HEADS_TEXT_Y;

heads_text.curr_frame = HEADS_ENERGY;

for (index=0; index<2; index++)
    {
    Sprite_Draw((sprite_ptr)&heads_text,double_buffer,0);

    // move down a row

    heads_text.y+=8;

    // select next message

    heads_text.curr_frame++;

    } //  end for index

// draw gauges

heads_gauge.x = RIGHT_HEADS_TEXT_X+40;
heads_gauge.y = RIGHT_HEADS_TEXT_Y;

// compute proper frame

heads_gauge.curr_frame = 22 - (players_energy/1000);

// draw the energy level

Sprite_Draw((sprite_ptr)&heads_gauge,double_buffer,0);

heads_gauge.x = RIGHT_HEADS_TEXT_X+40;
heads_gauge.y = RIGHT_HEADS_TEXT_Y+8;

// compute proper frame

heads_gauge.curr_frame = 22 - (players_shield_strength/1000);

// draw the shield strength

Sprite_Draw((sprite_ptr)&heads_gauge,double_buffer,0);

} // end Draw_Heads

////////////////////////////////////////////////////////////////////////////

void Erase_Heads(void)
{

// this function erases the heads up display during the time when
// the game background can be modified

int index; // looping variable

heads_text.x = LEFT_HEADS_TEXT_X;
heads_text.y = LEFT_HEADS_TEXT_Y;

// draw the left most messages first

heads_text.curr_frame = 0;

for (index=0; index<4; index++)
    {
    Sprite_Draw((sprite_ptr)&heads_text,double_buffer,0);

    // move down a row

    heads_text.y+=8;

    } //  end for index


// now erase buttons and numbers

heads_numbers.x = LEFT_HEADS_TEXT_X+40;
heads_numbers.y = LEFT_HEADS_TEXT_Y;

// erase cloaked button

heads_numbers.curr_frame=0;
Sprite_Draw((sprite_ptr)&heads_numbers,double_buffer,0);

// erase scanner

heads_numbers.x = LEFT_HEADS_TEXT_X+40;
heads_numbers.y = LEFT_HEADS_TEXT_Y+8;

Sprite_Draw((sprite_ptr)&heads_numbers,double_buffer,0);

// erase communications

heads_numbers.x = LEFT_HEADS_TEXT_X+40;
heads_numbers.y = LEFT_HEADS_TEXT_Y+16;

Sprite_Draw((sprite_ptr)&heads_numbers,double_buffer,0);

// erase ships number

heads_numbers.x = LEFT_HEADS_TEXT_X+40;
heads_numbers.y = LEFT_HEADS_TEXT_Y+24;

Sprite_Draw((sprite_ptr)&heads_numbers,double_buffer,0);

// now draw right most messages

heads_text.x = RIGHT_HEADS_TEXT_X;
heads_text.y = RIGHT_HEADS_TEXT_Y;

// draw the left most messages first

for (index=0; index<2; index++)
    {
    Sprite_Draw((sprite_ptr)&heads_text,double_buffer,0);

    // move down a row

    heads_text.y+=8;

    } //  end for index

// erase gauges

heads_gauge.x = RIGHT_HEADS_TEXT_X+40;
heads_gauge.y = RIGHT_HEADS_TEXT_Y;

// compute proper frame

heads_gauge.curr_frame = 22;

// draw the energy level

Sprite_Draw((sprite_ptr)&heads_gauge,double_buffer,0);

// draw gauges

heads_gauge.x = RIGHT_HEADS_TEXT_X+40;
heads_gauge.y = RIGHT_HEADS_TEXT_Y+8;

// erase the shield strength

Sprite_Draw((sprite_ptr)&heads_gauge,double_buffer,0);

} // end Erase_Heads

/////////////////////////////////////////////////////////////////////////////

void Init_Scanner(void)
{
// this function initializes the scanner

// not much to do at this point!

} // end Init_Scanner

/////////////////////////////////////////////////////////////////////////////

void Erase_Scanner(void)
{
// this function erases the scanner and the blips

// first erase scanner grid

Line_H_2(SCANNER_X,SCANNER_X+64,SCANNER_Y,   0,double_buffer);
Line_H_2(SCANNER_X,SCANNER_X+64,SCANNER_Y+16,0,double_buffer);
Line_H_2(SCANNER_X,SCANNER_X+64,SCANNER_Y+32,0,double_buffer);

Line_V_2(SCANNER_Y,SCANNER_Y+32,SCANNER_X,   0,double_buffer);
Line_V_2(SCANNER_Y,SCANNER_Y+32,SCANNER_X+16,0,double_buffer);
Line_V_2(SCANNER_Y,SCANNER_Y+32,SCANNER_X+32,0,double_buffer);
Line_V_2(SCANNER_Y,SCANNER_Y+32,SCANNER_X+48,0,double_buffer);
Line_V_2(SCANNER_Y,SCANNER_Y+32,SCANNER_X+64,0,double_buffer);

} // end Erase_Scanner

////////////////////////////////////////////////////////////////////////////

void Draw_Scanner(void)
{
// this function draws the scanner and the blips

// first draw scanner grid

Line_H_2(SCANNER_X,SCANNER_X+64,SCANNER_Y,   10,double_buffer);
Line_H_2(SCANNER_X,SCANNER_X+64,SCANNER_Y+16,10,double_buffer);
Line_H_2(SCANNER_X,SCANNER_X+64,SCANNER_Y+32,10,double_buffer);

Line_V_2(SCANNER_Y,SCANNER_Y+32,SCANNER_X,   10,double_buffer);
Line_V_2(SCANNER_Y,SCANNER_Y+32,SCANNER_X+16,10,double_buffer);
Line_V_2(SCANNER_Y,SCANNER_Y+32,SCANNER_X+32,10,double_buffer);
Line_V_2(SCANNER_Y,SCANNER_Y+32,SCANNER_X+48,10,double_buffer);
Line_V_2(SCANNER_Y,SCANNER_Y+32,SCANNER_X+64,10,double_buffer);

} // end Draw_Scanner

/////////////////////////////////////////////////////////////////////////////

void Under_Blips(void)
{
// this function is used to draw all the scanner blips

// draw blips, notice that the position of the remote and player is
// scaled to fit into the scanner window

under_players_blip = Read_Pixel_DB(SCANNER_X+players_x/40,SCANNER_Y+players_y/80);
under_remotes_blip = Read_Pixel_DB(SCANNER_X+remotes_x/40,SCANNER_Y+remotes_y/80);

} // end Under_Blips

/////////////////////////////////////////////////////////////////////////////

void Draw_Blips(void)
{
// this function is used to draw all the scanner blips

// draw blips, notice that the position of the remote and player is
// scaled to fit into the scanner window

if (players_cloak==-1)
   Write_Pixel_DB(SCANNER_X+players_x/40,SCANNER_Y+players_y/80,9);

if (remotes_cloak==-1)
   Write_Pixel_DB(SCANNER_X+remotes_x/40,SCANNER_Y+remotes_y/80,12);

} // end Draw_Blips

/////////////////////////////////////////////////////////////////////////////

void Erase_Blips(void)
{
// this function is used to erase all the scanner blips

// erase blips, notice that the position of the remote and player is
// scaled to fit into the scanner window

Write_Pixel_DB(SCANNER_X+players_x/40,SCANNER_Y+players_y/80,under_players_blip);
Write_Pixel_DB(SCANNER_X+remotes_x/40,SCANNER_Y+remotes_y/80,under_remotes_blip);

} // end Erase_Blips

////////////////////////////////////////////////////////////////////////////

void Music_Init(void)
{
// this function loads the music and resets all the indexes

static int loaded=0;

// has the music already been loaded

if (!music_enabled)
   return;

if (!loaded)
   {
   Music_Load("blazemus.xmi",(music_ptr)&song);
   loaded=1;
   } // end if not loaded

// reset sequence counters

game_seq_index=0;
intro_seq_index=0;

} // end Music_Init

/////////////////////////////////////////////////////////////////////////////

void Music_Close(void)
{
// this function unloads the music files

if (!music_enabled)
   return;

// turn off music and unload song

Music_Stop();
Music_Unload((music_ptr)&song);

} // end Music_Close

//////////////////////////////////////////////////////////////////////////////

void Digital_FX_Init(void)
{
// this function initializes the digital sound fx system

static int loaded=0;

if (!digital_enabled)
   return;

// have the sound fx been loaded?

if (!loaded)
   {
   // load int sounds

   Sound_Load("BLZCLK.VOC"  ,(_sound_ptr)&digital_FX[BLZCLK_VOC  ],1);
   Sound_Load("BLZEXP1.VOC" ,(_sound_ptr)&digital_FX[BLZEXP1_VOC ],1);
   Sound_Load("BLZEXP2.VOC" ,(_sound_ptr)&digital_FX[BLZEXP2_VOC ],1);
   Sound_Load("BLZLAS.VOC"  ,(_sound_ptr)&digital_FX[BLZLAS_VOC  ],1);
   Sound_Load("BLZNRG.VOC"  ,(_sound_ptr)&digital_FX[BLZNRG_VOC  ],1);
   Sound_Load("BLZSHLD.VOC" ,(_sound_ptr)&digital_FX[BLZSHLD_VOC ],1);
   Sound_Load("BLZTAC.VOC"  ,(_sound_ptr)&digital_FX[BLZTAC_VOC  ],1);
   Sound_Load("BLZSCN.VOC"  ,(_sound_ptr)&digital_FX[BLZSCN_VOC  ],1);
   Sound_Load("BLZMISS.VOC" ,(_sound_ptr)&digital_FX[BLZMISS_VOC ],1);

   Sound_Load("BLZBIOS.VOC" ,(_sound_ptr)&digital_FX[BLZBIOS_VOC ],1);
   Sound_Load("BLZENTR.VOC" ,(_sound_ptr)&digital_FX[BLZENTR_VOC ],1);
   Sound_Load("BLZABRT.VOC" ,(_sound_ptr)&digital_FX[BLZABRT_VOC ],1);
   Sound_Load("BLZSEL.VOC"  ,(_sound_ptr)&digital_FX[BLZSEL_VOC  ],1);
   Sound_Load("BLZKEY.VOC"  ,(_sound_ptr)&digital_FX[BLZKEY_VOC  ],1);
   Sound_Load("BLZDIAL.VOC"  ,(_sound_ptr)&digital_FX[BLZDIAL_VOC  ],1);

   Sound_Load("BLZLOS.VOC"  ,(_sound_ptr)&digital_FX[BLZLOS_VOC  ],1);
   Sound_Load("BLZWIN.VOC"  ,(_sound_ptr)&digital_FX[BLZWIN_VOC  ],1);

   // set loaded flag

   loaded=1;

   } // end if sound effects aren't loaded

} // end Digital_FX_Init

///////////////////////////////////////////////////////////////////////////////

void Digital_FX_Close(void)
{
// this function unloads all the digital FX

if (!digital_enabled)
   return;

// unload all the sound fx from memory

Sound_Unload((_sound_ptr)&digital_FX[BLZCLK_VOC  ]);
Sound_Unload((_sound_ptr)&digital_FX[BLZEXP1_VOC ]);
Sound_Unload((_sound_ptr)&digital_FX[BLZEXP2_VOC ]);
Sound_Unload((_sound_ptr)&digital_FX[BLZLAS_VOC  ]);
Sound_Unload((_sound_ptr)&digital_FX[BLZNRG_VOC  ]);
Sound_Unload((_sound_ptr)&digital_FX[BLZSHLD_VOC ]);
Sound_Unload((_sound_ptr)&digital_FX[BLZTAC_VOC  ]);
Sound_Unload((_sound_ptr)&digital_FX[BLZSCN_VOC  ]);
Sound_Unload((_sound_ptr)&digital_FX[BLZMISS_VOC ]);

Sound_Unload((_sound_ptr)&digital_FX[BLZENTR_VOC ]);
Sound_Unload((_sound_ptr)&digital_FX[BLZABRT_VOC ]);
Sound_Unload((_sound_ptr)&digital_FX[BLZSEL_VOC  ]);
Sound_Unload((_sound_ptr)&digital_FX[BLZKEY_VOC  ]);
Sound_Unload((_sound_ptr)&digital_FX[BLZDIAL_VOC ]);

Sound_Unload((_sound_ptr)&digital_FX[BLZLOS_VOC  ]);
Sound_Unload((_sound_ptr)&digital_FX[BLZWIN_VOC ]);


} // end Digital_FX_Close

/////////////////////////////////////////////////////////////////////////////

int Digital_FX_Play(int the_effect, int priority)
{
// this function is used to play a digital effect using a pre-emptive priority
// scheme. The algorithm works like this: if a sound is playing then its
// priority is compared to the sound that is being requested to be played
// if the new sound has higher priority (a smaller number) then the currenlty
// playing sound is pre-empted for the new sound and the global FX priority
// is set to the new sound. If there is no sound playing then the new sound
// is simple played and the global priority is set

// is the digital fx system on-line?

if (!digital_enabled)
   return(0);

// is there a sound playing?

if (!Sound_Status() || (priority <= digital_FX_priority))
   {
   // start new sound

   Sound_Stop();

   Sound_Play((_sound_ptr)&digital_FX[the_effect]);

   // set the priority

   digital_FX_priority = priority;

   return(1);

   } // end if
else // the current sound is of higher priority
   return(0);

} // end Digital_FX_Play

/////////////////////////////////////////////////////////////////////////////

int Parse_Commands(int argc, char **argv)
{
// this function is used to parse the commands line parameters that are to be
// used as switched to enable different modes of operation

int index;  // looping variable

for (index=1; index<argc; index++)
    {
    // get the first character from the string

    switch(argv[index][0])
          {

          case 's': // enable sound effects
          case 'S':
               {
               digital_enabled=1;
               } break;

          case 'm': // enable nusic
          case 'M':
               {
               music_enabled=1;
               } break;

          // more commands would go here...

          default:break;

          } // end switch

    } // end for index

} // end Parse_Commands

/////////////////////////////////////////////////////////////////////////////

int Get_Modem_String(char *buffer)
{
// this function open up the modem intialization file named
// blaze.mod, if the file exists then

FILE *fp;

// try and open the file

if ((fp=fopen("blaze.mod","r"))==NULL)
   {
   strcpy(buffer,"");
   return(0);
   }

// else load in modem initialization string

fscanf(fp,"%s",buffer);

// close the file

fclose(fp);

return(1);

} // end Get_Modem_String

////////////////////////////////////////////////////////////////////////////////

void Start_Up(void)
{

int index; // looping variable

// set the graphics mode to mode 13h

Set_Graphics_Mode(GRAPHICS_MODE13);

// start up music system

if (music_enabled)
   {
   Music_Init();
   Music_Play((music_ptr)&song,0);
   } // end if music enabled

// start up digital FX system

if (digital_enabled)
   {
   Digital_FX_Init();
   } // end if digital sound enabled

// put up Waite header

Intro_Waite();

// seed the random number generator with time

srand((unsigned int)Timer_Query());

// initialize font engine

Font_Engine_1(0,0,0,0,NULL,NULL);
Tech_Print(START_MESS_X,START_MESS_Y,"STARBLAZER 1.0 STARTING UP...",video_buffer);

Time_Delay(5);

// create the double buffer

Create_Double_Buffer(200);
Tech_Print(START_MESS_X,START_MESS_Y+16,"DOUBLE BUFFER CREATED",video_buffer);
Tech_Print(START_MESS_X,START_MESS_Y+24,"LANGUAGE TRANSLATION ENGAGED",video_buffer);

// install the keyboard driver

Keyboard_Install_Driver();
Tech_Print(START_MESS_X,START_MESS_Y+32,"NEURAL INTERFACE ACTIVATED",video_buffer);

// load in all gadget icons

Load_Icons();
Tech_Print(START_MESS_X,START_MESS_Y+40,"VISUAL ICONS LOADED",video_buffer);

// load in the ships

Load_Ships();
Tech_Print(START_MESS_X,START_MESS_Y+48,"SHIPS LOADED",video_buffer);

// assign ships to player and remote

Copy_Frames((sprite_ptr)&players_ship,(sprite_ptr)&gryfon_l);
Copy_Frames((sprite_ptr)&remotes_ship,(sprite_ptr)&raptor_r);

// start the asteroids up

Init_Asteroids(16,6,4);
Tech_Print(START_MESS_X,START_MESS_Y+56,"ASTEROID TRAJECTORIES COMPUTED",video_buffer);

// start the stars

Init_Stars();
Tech_Print(START_MESS_X,START_MESS_Y+64,"STARFIELD GENERATED",video_buffer);

// start the missiles

Init_Missiles();
Tech_Print(START_MESS_X,START_MESS_Y+72,"WEAPONS DAEMONS ONLINE",video_buffer);

// start the explosions

Init_Explosions();
Load_Explosions();
Init_Novas();
Tech_Print(START_MESS_X,START_MESS_Y+80,"EXPLOSION ANIMATION SYSTEM LOADED",video_buffer);

// initialize font engine

Font_Engine_1(0,0,0,0,NULL,NULL);
Tech_Print(START_MESS_X,START_MESS_Y+88,"FONT ENGINE ENGAGED",video_buffer);

// load the wormhole imagery

Load_Wormhole();
Init_Wormhole();
Tech_Print(START_MESS_X,START_MESS_Y+96,"WORM HOLE CREATED",video_buffer);

// load the aliens

Load_Alien();
Init_Alien();
Tech_Print(START_MESS_X,START_MESS_Y+104,"ENEMY AI GENERATED",video_buffer);

// the the fuel cells

Load_Fuel_Cells();
Init_Fuel_Cells();
Tech_Print(START_MESS_X,START_MESS_Y+112,"FUEL CELLS PLACED",video_buffer);

// load the heads up display

Load_Heads();
Init_Heads();
Tech_Print(START_MESS_X,START_MESS_Y+120,"HEADS UP DISPLAY ACTIVE",video_buffer);

// blink ok

Tech_Print(START_MESS_X,START_MESS_Y+136,"SYSTEM BIOS O.K.",video_buffer);

Digital_FX_Play(BLZBIOS_VOC,1);

for (index=0; index<3; index++)
    {
    // draw the message and the erase the message
    Font_Engine_1(START_MESS_X,START_MESS_Y+136,0,0,"SYSTEM BIOS O.K.",video_buffer);
    Time_Delay(8);

    Font_Engine_1(START_MESS_X,START_MESS_Y+136,0,0,"                ",video_buffer);
    Time_Delay(8);

    } // end for

// get rid off this sound to save a little memory (we are running low!)

Sound_Unload((_sound_ptr)&digital_FX[BLZBIOS_VOC]);

// do intro piece

Intro_Title();

// save the system pallete here because we are going to really thrash it!!!

Read_Palette(0,255,(RGB_palette_ptr)&game_palette);

} // end Start_Up


