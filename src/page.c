#define _GNU_SOURCE

#include "fenstersql.h"
#include "data.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>


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
