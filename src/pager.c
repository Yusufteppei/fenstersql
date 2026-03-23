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


