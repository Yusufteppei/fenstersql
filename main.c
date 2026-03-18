#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pager.h"
#include "data.h"
#include "penceredb.h"

void main(){
  
  BufferPool *bufferpool = malloc(1024*1024*1024);
  //FILE *global = fopen("global", "ab");
  char input[1024];
  int object;
  char value[1024];
  int action;

  int i = -1;

  system("figlet 'PENCEREDB'");
  printf("%d", TABLE);  
  while (1 == 1){
    
    printf("\npenceredb# ");
    //fgets(i, sizeof(i), stdin);
    
    scanf("%d %d %s", &action, &object, &value);
    execute(action, object, value);
  }
  free(bufferpool);
}
