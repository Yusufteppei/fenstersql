#define _GNU_SOURCE

#include "fenstersql.h"
#include "data.h"
#include "connection.h"
#include "fstr_strings.h"
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
  return atomic_fetch_add(&bufferpool->global_control.next_oid, 1);
}

//////////////////////////////////////////////////////////////////////


/////////////////////     PAGES HANDLING     ///////////////////////////

/*
PageTable *load_page_table( ) {
  FILE *file = fopen(PAGE_TABLE_FILE, "rb");
  PageTable *page_table = NULL;
  fseek(file, 0, SEEK_END);
  long int file_size = ftell(file);
  fseek(file, 0, SEEK_SET);
  fread(page_table, file_size, 1, file);
  return page_table;
};
*/


void register_table (  ) {

};


Page *get_start_page_from_table_oid ( int64_t table_oid ) {
  // CHECK PAGE TABLE ( FROM MEMORY ) TO FIND PAGE ADDRESS
  PageTableEntry *curr_table_entry;
  for (long int i=page_table->page_table_entries; i<sizeof(table_oid);i+=sizeof(PageTableEntry)){
    curr_table_entry = i;    
    if ( curr_table_entry->table_oid == table_oid ) {
        
        return bufferpool->pages + (PAGE_SIZE)*(curr_table_entry->page_id);
    }
  };
  return NULL;
};

void create_page_table_entry ( PageTableEntry page_table_entry){

  PageTableEntry *pte;
  pte = page_table->page_table_entries + sizeof(PageEntry) * page_table->entry_count ;
  page_table->entry_count++;

  printf("Page Table Entry Created.\n %64d : %32d \nEntry Count : %32d",page_table->page_table_entries[page_table->entry_count-1].table_oid, page_table->page_table_entries[page_table->entry_count-1].page_id, page_table->entry_count );
  
};


Page *load_table( Context *context, int table_oid ){
  // PULL TABLE FROM STORAGE INTO PAGES, UPDATE PAGE TABLE; RETURN START PAGE ADDRESS
  
  // GET TABLE FILE AND READ IN BATCHES OF PAGE_SIZE INTO PAGES, UPDATE PAGE TABLE 
  char table_filename[32];
  strcpy(table_filename, int_to_string(table_oid));
  char database_dir[32] = DATABASES_BASE_DIR;
  strcat(database_dir, int_to_string(ctx->database_oid));
  
  char pathname[128];
  strcat(database_dir, table_filename);
  strcpy(pathname, database_dir);

  //  READ FILE

  FILE *file = fopen(pathname, "rb");



  return 0;
}


Tuple *read_tuple ( int page_id, int32_t offset ) {
  
  return 0;
};

Tuple *write_tuple () {

  return 0;
}


///////////////////////////////////////////////////////////////////
