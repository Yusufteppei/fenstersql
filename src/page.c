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

#include "page.h"

#define PAGE_SIZE 4096
#define BUFFERPOOL_SIZE 1024*1024*1024
#define PAGE_COUNT BUFFERPOOL_SIZE / PAGE_SIZE



Page *get_page_address ( ) {
  // CHECK PAGE TABLE ( FROM MEMORY ) TO FIND PAGE ADDRESS

  return 0;
};

Page *load_page( int page_id ){
  // PULL PAGE FROM STORAGE INTO MEMORY; RETURN PAGE ADDRESS
  
  return 0;
}


Tuple *read_tuple ( int page_id, int32_t offset ) {
  
  return 0;
};

Tuple *write_tuple () {

  return 0;
}
#include <unistd.h>
#include <stdint.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "page.h"

#define PAGE_SIZE 4096
#define BUFFERPOOL_SIZE 1024*1024*1024
#define PAGE_COUNT BUFFERPOOL_SIZE / PAGE_SIZE


struct PageTable{

};

typedef struct {

} Pager;

int get_page ( Pager *self,  int32_t page_id ) {
  int32_t offset = page_id * PAGE_SIZE;
  return 0;  
}




