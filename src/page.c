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

////////////////////////   GLOBAL_CONTROL    //////////////////////////
int64_t peek_next_oid() {
  
  return atomic_load(&bufferpool->global_control.next_oid);
};

int64_t use_next_oid() {
  printf("Generating oid");
  return atomic_fetch_add(&bufferpool->global_control.next_oid, 1);
}

//////////////////////////////////////////////////////////////////////


/////////////////////     PAGES HANDLING     ///////////////////////////

PageTable *load_page_table( ) {
  FILE *file = fopen(PAGE_TABLE_FILE, "rb");
  PageTable *page_table = NULL;
  fseek(file, 0, SEEK_END);
  long int file_size = ftell(file);
  fseek(file, 0, SEEK_SET);
  fread(page_table, file_size, 1, file);
  return page_table;
};

void register_table (  ) {

};


Page *get_page_address ( int64_t table_oid ) {
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


///////////////////////////////////////////////////////////////////
