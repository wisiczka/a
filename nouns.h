// Tile types; these are used on the map
typedef enum
{
  tile_error,
  tile_grass,
  tile_path,
  tile_sand,
  tile_gravel,
  tile_wall,
  tile_fence,
  tile_water,
  tile_tree,
  tile_rock,
  tile_door_closed,
  tile_door_open,
  tile_toilet,
  tile_sink
} tile_t;


// These are used during map generation
// Each animal has a preferred terrain type
typedef enum
{
  terr_grass,
  terr_forest,
  terr_savannah,
  terr_arctic,
  terr_pond
} terrain_t;



// Animal types
typedef enum
{
  nobody,
  the_player,
  anml_anteater,
  anml_alligator,
  anml_boar,
 
  anml_last // Size of array
} animal_t;

// Holds the character ORed with curses
// attributes used to display each animal
chtype anml_glyph[anml_last];

// animal_t with type "nobody" (0) are disabled
typedef struct
{
  animal_t type;
  int y;
  int x;
  int health;
  int strength;
} mob_t;



mob_t mob[MAX_MOBS];

// Shorthand for mob[0]
mob_t * player;

// Used during generation; this is the index of the next mob_t we can
// use for placing animals. Index 0 is reserved for the player.
int next_mob = 1;

// Used during generation; these are the animals not yet placed on the
// map. After one is used its slot will be set to "nobody".
int animals[anml_last];





// Animal names
char * anml_name[anml_last] =
{
  [anml_anteater] = "anteater",
  [anml_alligator] = "alligator",
  [anml_boar] = "boar",
  
};







void init_nouns(){
	int i;


	// Clear the mob list
  for (i = 1; i < MAX_MOBS; i++){
    mob[i].type = nobody;
    mob[i].health = 100;
    mob[i].strength = 10;
}

	///all nouns held in mob array mobx., mob.type, mob[1].x etc

  player = &mob[0];
  player->type = the_player;
  player->y = 10;
  player->x = 10;
  player->health = 300;
  player->strength = 20;
  //players true start is made at make_entrance in map.h
  //animals true start and type is made at populate in map.h

 

  anml_glyph[the_player] = '@';
  anml_glyph[anml_anteater] = 'a' | COLOR_PAIR(pair_brown);
  anml_glyph[anml_alligator] = 'A' | COLOR_PAIR(pair_green);
  anml_glyph[anml_boar] = 'b' | COLOR_PAIR(pair_black) | A_BOLD;
  


}