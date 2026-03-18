#include <stdint.h>
#include <string.h>
#include <ctype.h>

typedef enum {
  CREATE,
  SELECT,
  INSERT,
  DELETE,
}
Action;


typedef struct {
  int action;
  int *execute;
  char value[];
} Command;


int parse_query( char* query  ) {
  int action;
  char* token;
  Command c;

  for ( int i=0; i<strlen(query); i++ ) {
    if ( isspace(*query+i) == 0 ) {
      token[i] = *query+i;
    }
    else{
      break;
    }

  }
};


int execute( int action, int object, char value[] ) {
  printf("Executing...Val : ");
  switch ( action ) {
      case CREATE:
        create_object ( object, value );
        break;
      
      case SELECT:
        select_object ( object );
        break;

      default:
        break;
  };
  
  return 0;
}
