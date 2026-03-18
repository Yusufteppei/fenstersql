#define _GNU_SOURCE

#include "penceredb.h"
#include "data.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>


typedef struct {
  int64_t ID;
  int64_t used;
  char *data;

  int32_t table_id;
}
Page;


int read_page( int page_number){
  

  int fd = open(DATABASES_FILE, O_RDWR | O_CREAT, 0644);
   
  if (fd == -1) {
      perror("Error opening file");
      return 1;
  }

return 0;
}

int write_tuple () { 

  return 0;
}
