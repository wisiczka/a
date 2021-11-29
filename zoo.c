/*
  A DAY @ THE ZOO
  a roguelike-something for the 7-day roguelike challenge 2013
  by Ulf Åström (ulf.astrom@gmail.com / happyponyland.net)
  
  This software comes with no warranty, blah blah blah. You may use
  this code for your own needs, but if you do, please give something
  back to the community by releasing your changes.

  Compilation should look essentially like:

    gcc zoo.c -lcurses -o zoo

  HPL 2013-03-10
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <time.h>
#include "defines.h"
#include "nouns.h"
#include "inits.h"
#include "map.h"
#include "render.h"
#include "parse.h"



int main(int argc, char ** argv)
{
  init();

  init_nouns();

  init_map();
  bsp_split(0, 0, MAP_H, MAP_W, true);
  make_entrance();

  recenter();
  calc_fov();

  play();

  shutdown();

  return 0;
}


/*
  Passes time in the zoo, gets player input and responds to it.
*/
void play()
{
  

  while (1)
  {
    move_animals();

    draw_map();
    legend();
    parsebox();

    wnoutrefresh(board);
    wnoutrefresh(disp);
    wnoutrefresh(disp2);
    doupdate();

    parse();

 
  }
}





/*
  Tries to move the player SPEED_Y north/south and SPEED_X left/right.
  Returns false if the way is blocked.
*/
bool move_player(int speed_y, int speed_x)
{
  int new_y;
  int new_x;

  new_y = player->y + speed_y;
  new_x = player->x + speed_x;

  if (new_y < 0 || new_y >= MAP_H || new_x < 0 || new_x >= MAP_W)
  {
    // Leaving the map ends the game
    shutdown();
  }

  // If there is a door in the way, open it
  if (gtile(new_y, new_x) == tile_door_closed)
  {
    stile(new_y, new_x, tile_door_open);
    calc_fov();
    return true;
  }

  // Is something in the way?

  if (blocking(new_y, new_x) & (CANT_MOVE | SOMEONE_THERE))
    return false;

  // The way is clear; go there

  player->y = new_y;
  player->x = new_x;

  recenter();
  calc_fov();

  return true;
}




/*
  Moves the animals around randomly.
  They won't move into obstacles or each other.
*/
void move_animals()
{
  int i;
  int y;
  int x;

  for (i = 1; i < MAX_MOBS; i++)
  {
    if (mob[i].type == nobody)
      break; // No use proceeding

    if (rand() % lazy(mob[i].type) > 0)
      continue;

    y = mob[i].y;
    x = mob[i].x;

    y = y - 1 + rand() % 3;
    x = x - 1 + rand() % 3;

    if ((blocking(y, x) & (CANT_MOVE | SOMEONE_THERE)) == 0)
    {
      mob[i].y = y;
      mob[i].x = x;
    }
  }

  return;
}



/*
  Returns a bitmask of the blocking properties of the tile at (Y,X).
*/
int blocking(int y, int x)
{
  int i;

  if (y < 0 || y >= MAP_H || x < 0 || x >= MAP_W)
    return (CANT_MOVE | CANT_SEE | OUTSIDE_MAP);

  // Some tiles we can see past, but not move over. Some tiles we
  // can't see or move through. In theory there could be "dark" tiles
  // that we can move through but not see where we are going.

  switch (map[y][x])
  {
  case tile_wall:
  case tile_rock:
  case tile_tree:
  case tile_door_closed:
    return (CANT_MOVE | CANT_SEE);
    
 /// case tile_fence:
  case tile_water:
  case tile_toilet:
  case tile_sink:
    return (CANT_MOVE);

  default:
    break;
  }

  // Check if there is a mob occupying that space

  for (i = 0; i < MAX_MOBS; i++)
  {
    if (mob[i].type == nobody)
      break;

    if (mob[i].y == y && mob[i].x == x)
      return SOMEONE_THERE;
  }

  return 0;
}
