

/*
  Sets up curses and everything else we need.
 */
void init()
{
  srand(time(NULL));

  initscr();
  cbreak();
  noecho();
  nonl();
  intrflush(stdscr, FALSE);
  keypad(stdscr, TRUE);
  curs_set(0);
  start_color();

  board = newwin(BOARD_H, BOARD_W, 0, 0);
  disp  = newwin(DISP_H, DISP_W , BOARD_H, 0);

#ifdef WIN32
  key_left  = 260;
  key_right = 261;
  key_up    = 259;
  key_down  = 258;
#else
  key_left  = KEY_LEFT;
  key_right = KEY_RIGHT;
  key_up    = KEY_UP;
  key_down  = KEY_DOWN;
#endif
  
  init_pair(pair_white,   COLOR_WHITE,   COLOR_BLACK);
  init_pair(pair_red,     COLOR_RED,     COLOR_BLACK);
  init_pair(pair_green,   COLOR_GREEN,   COLOR_BLACK);
  init_pair(pair_cyan,    COLOR_CYAN,    COLOR_BLACK);
  init_pair(pair_brown,   COLOR_YELLOW,  COLOR_BLACK);
  init_pair(pair_black,   COLOR_BLACK,   COLOR_BLACK);
  init_pair(pair_magenta, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(pair_blue,    COLOR_BLUE,    COLOR_BLACK);

  erase();
  refresh();

  anml_glyph[the_player] = '@';
  anml_glyph[anml_anteater] = 'a' | COLOR_PAIR(pair_brown);
  anml_glyph[anml_alligator] = 'A' | COLOR_PAIR(pair_green);
  anml_glyph[anml_boar] = 'b' | COLOR_PAIR(pair_black) | A_BOLD;
  

  return;
}


/*
  Kills curses and everything else needed for a clean exit.
*/
void shutdown()
{
  clear();
  delwin(board);
  delwin(disp);
  endwin();
  exit(0);
}

