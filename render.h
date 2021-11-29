

/*
  Draws the map and updates the screen.
*/
void draw_map()
{
  int y;
  int x;
  int i;
  int tile;

  werase(board);

  for (y = 0; y < BOARD_H; y++)
  {
    for (x = 0; x < BOARD_W; x++)
    {
      // Only draw tiles that we have visited (FOV_SEEN bit, but we
      // can just check for anything)

      if (fov[y + view_y][x + view_x] == 0)
	continue;

      wmove(board, y, x);

      // No need for gtile() here, we know what we are doing
      tile = map[y + view_y][x + view_x];

      switch (tile)
      {
      case tile_grass:
	waddch(board, '.' | COLOR_PAIR(pair_green));
	break;

      case tile_path:
	waddch(board, '.' | COLOR_PAIR(pair_red));
	break;

      case tile_gravel:
	waddch(board, '.' | COLOR_PAIR(pair_white));
	break;

      case tile_sand:
	waddch(board, '.' | COLOR_PAIR(pair_brown));
	break;

      case tile_door_closed:
	waddch(board, '+' | COLOR_PAIR(pair_brown) | A_REVERSE);
	break;

      case tile_door_open:
	waddch(board, '/' | COLOR_PAIR(pair_brown));
	break;

      case tile_toilet:
	waddch(board, 'o' | COLOR_PAIR(pair_white));
	break;

      case tile_sink:
	waddch(board, 'D' | COLOR_PAIR(pair_cyan));
	break;

      case tile_wall:
	waddch(board, ACS_CKBOARD);
	break;

      case tile_fence:
	waddch(board, ACS_PLUS);
	break;

      case tile_water:
	waddch(board, ACS_CKBOARD | COLOR_PAIR(pair_cyan));
	break;

      case tile_tree:
	waddch(board, '&' | COLOR_PAIR(pair_green));
	break;

      case tile_rock:
	waddch(board, '*');
	break;

      case tile_error:
      default:
	waddch(board, '?' | COLOR_PAIR(pair_red) | A_REVERSE);
	break;
      }
    }
  }

  // Draws mobs, but only those currently within view.

  for (i = 0; i < MAX_MOBS; i++)
  {
    if (mob[i].type == nobody)
      break; // No use proceeding

    y = mob[i].y;
    x = mob[i].x;

    if (y < view_y || y >= view_y + BOARD_H ||
	x < view_x || x >= view_x + BOARD_W ||
	(fov[y][x] & FOV_IN_VIEW) == 0)
    {
      continue;
    }

    wmove(board, y - view_y, x - view_x);

    waddch(board, anml_glyph[mob[i].type]);
  }

  return;
}



/*
  Display names of the animals currently on screen.
*/
void parsebox()
{
  int type;
  int i;
  int printed = 0;
  int y;
  int x;
  bool seen;

  werase(disp2);

  wmove(disp2, 0, 0);
  waddstr(disp2, "PARSEBOX");

 

  wmove(disp2, DISP_H -1, 0);
  waddstr(disp2, ">>>");

  return;
}




void legend()
{
  int type;
  int i;
  int printed = 0;
  int y;
  int x;
  bool seen;

  werase(disp);

  wmove(disp, 0, 0);
  waddstr(disp, "A DAY @ THE ZOO");

  // Loop through each type of animal, check if any of them are
  // visible to the player.

  for (type = anml_anteater; type < anml_last; type++)
  {
    seen = false;

    for (i = 1; i < MAX_MOBS; i++)
    {
      if (mob[i].type == nobody)
  break; // No use proceeding

      if (mob[i].type != type)
  continue;

      // This is the animal we are looking for

      y = mob[i].y;
      x = mob[i].x;

      // Is it on the map and within FOV?

      if (y < 0 || y >= MAP_H ||
    x < 0 || x >= MAP_W ||
    (fov[y][x] & FOV_IN_VIEW) == 0)
      {
  continue;
      }

      // Is it on the screen? (Just being within FOV isn't enough)
      
      if (y - view_y >= 0 && y - view_y < BOARD_H &&
    x - view_x >= 0 && x - view_x < BOARD_W)
      {
  seen = true;
  break;
      }
    }

    if (seen)
    {
      // The animal is on screen. Draw its glyph followed by its name.

      wmove(disp, 2 + printed, 0);

      waddch(disp, anml_glyph[type]);
      waddch(disp, ' ');
      waddstr(disp, anml_name[type]);

      printed++;

      if (printed >= 20)
  break; // We've run out of space (highly unlikely)
    }
  }

  wmove(disp, (BOARD_H+DISP_H) - 1, 0);
  waddstr(disp, "cutee");

  return;
}
































/*
  Recenters the viewport around the player, if needed.
*/
void recenter()
{
  int p_y;
  int p_x;

  p_y = player->y;
  p_x = player->x;

  if (p_y - view_y < 5 || p_y - view_y >= BOARD_H - 4)
  {
    view_y = MIN(MAP_H - BOARD_H, MAX(0, p_y - (BOARD_H / 2)));
  }

  if (p_x - view_x < 10 || p_x - view_x >= BOARD_W - 10)
  {
    view_x = MIN(MAP_W - BOARD_W, MAX(0, p_x - (BOARD_W / 2)));
  }

  return;
}




/*
  Recalculates field of view.
*/
void calc_fov()
{
  float fy;
  float fx;
  float speed_y;
  float speed_x;
  int y;
  int x;
  int radius;
  int d;
  int deg;

  // Wipe the "in view" bit of the FOV map. By ANDing the FOV_SEEN
  // bit, we get to keep the areas we have previously explored.

  for (y = 0; y < MAP_H; y++)
    for (x = 0; x < MAP_W; x++)
      fov[y][x] &= FOV_SEEN;

  radius = FOV_RADIUS;

  fov[player->y][player->x] = true;
  
  // Project a ray in each direction and mark touched tiles as
  // "in view". Stop when we leave the map run into something
  // that obstructs the view.

  for (deg = 0; deg < 360; deg++)
  {
    // Add 0.5 since the player is actually in _the middle_ of the
    // tile; otherwise the result will have strangely offset edges.

    fy = (float)player->y + 0.5f;
    fx = (float)player->x + 0.5f;

    speed_y = sin_tbl[deg];
    speed_x = sin_tbl[(deg + 90) % 360];

    for (d = 0; d < radius; d++)
    {
      fy += speed_y;
      fx += speed_x;

      // Truncate coordinates so we can use them as array indices
      y = (int)fy;
      x = (int)fx;

      // Outside map?
      if (y < 0 || y >= MAP_H || x < 0 || x >= MAP_W)
  break;

      fov[y][x] |= FOV_IN_VIEW | FOV_SEEN;

      if (blocking(y, x) & CANT_SEE)
  break; // Can't see any further in this direction
    }
  }

  return;
}
