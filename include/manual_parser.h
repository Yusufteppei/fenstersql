#include <stdint.h>
#include <string.h>
/*
typedef enum {
  CREATE,
  INSERT,
  DELETE,
  SELECT
}
ACTION;
*/

typedef struct {
  int action;
  int *execute;
  char value[];
} Command;

void parse_query( char* query );

int execute ( char* action, char* object, char value[] );
