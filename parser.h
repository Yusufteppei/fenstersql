#include <stdint.h>
#include <string.h>

typedef enum {
  CREATE
  INSERT,
  DELETE,
  SELECT
}
ACTION;


typedef struct {
  int action;
  int *execute;
  char value[];
} Command;

int parse_query( char* query ) 

int execute ( int action, char name[32], char* value ) 
