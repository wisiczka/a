void parse(){



	int input;












	 new_input:
    input = getch();
    
    if (input == key_left)
    {
      if (move_player(0, -1) == false)
	goto new_input;
    }
    else if (input == key_right)
    {
      if (move_player(0, +1) == false)
	goto new_input;
    }
    else if (input == key_up)
    {
      if (move_player(-1, 0) == false)
	goto new_input;
    }
    else if (input == key_down)
    {
      if (move_player(+1, 0) == false)
	goto new_input;
    }
    else if (input == '.' || input == ' ')
    {
      // Wait
    }

    else if (input == 't')
    {
      text_parse();

    }




    else if (input == 'Q')
    {
      shutdown();
    }
    else
    {
      // Anything we don't recognize; no need to redraw the screen
      goto new_input;
    }
}





bool text_parse(){


    char comm[MAX_STR_SZ];
  char string_to_parse[MAX_STR_SZ];
  char tokenized_parse[20][20];
  int tokencounter =0;
  static const char *verbs[] = { "pat", "tickle", "kill", "dev", "examine", "go", "take", "drop", "wield", "unwield", "attack", "give", "move", "wear", "inventory", "buy", "sell", "talk", "BUFFER NAME"};
  static const char *prepositions[] = { "to", "from", "BUFFER NAME" };

       echo();
      move(DISP_H + BOARD_H -1, 0);
      addstr( ">>>");
      getstr(comm);
      noecho();
      strcpy(string_to_parse, " ");

      strcat(string_to_parse, comm);



///TOKENIZATION
    char * token = strtok(comm, " ");
   // loop through the string to extract all other tokens
    for(tokencounter=0; token != NULL ; tokencounter++) {

      strcpy(tokenized_parse[tokencounter], token);
      removeStringTrailingNewline(tokenized_parse[tokencounter]);

    /// printf("token %d  = %s \n",tokencounter,tokenized_parse[tokencounter]);
      token = strtok(NULL, " ");
    }

   ///TOKENIZATION END














}







  void removeStringTrailingNewline(char *str) {
  if (str == NULL)
    return;
  int length = strlen(str);
  if ((str[length-1] == '\n') || (str[length-1] == '\n'))
    str[length-1]  = '\0';
}
