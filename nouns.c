void init_nouns(){



// Clear the mob list
  for (i = 1; i < MAX_MOBS; i++)
    mob[i].type = nobody;

  // Fill the animals array; first two are reserved.
  animals[nobody] = nobody;
  animals[the_player] = nobody;

  for (i = anml_anteater; i < anml_last; i++)
    animals[i] = i;




	
}