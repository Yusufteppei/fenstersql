#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"
#include "data.h"

void string_to_upper(char *s) {
    for (; *s; ++s) *s = toupper((unsigned char)*s);
}

void parse_query( char query[1024]  ) {
  int action;
  char* tokens;
  Command c;

  for ( size_t i=0; i<strlen(query); i++ ) {
    printf("Iter %ld", i);
    if ( isspace(*(query+i)) == 0 ) {
      tokens[i] = *query+i;
      printf(*(query+i));
    }
    else{
      break;
    }

  }
};


int execute( char *action, char *object, char *value ) {
  

  string_to_upper(action);
  string_to_upper(object);
  
  if ( strcmp( action, "CREATE" ) == 0 ){
        create_object ( object, value );
  }

  else if ( strcmp( action, "SELECT" ) == 0 ){
        select_object ( object );
  }

  else {
    printf("Action unrecognized\n");
  }

  return 0;
};
