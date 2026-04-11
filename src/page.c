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
//
int64_t peek_next_oid() {
  
  return atomic_load(&bufferpool->global_control.next_oid);
};

//////////////////////////
//
int64_t use_next_oid() {
  return atomic_fetch_add(&bufferpool->global_control.next_oid, 1);
}

//////////////////////////////////////////////////////////////////////


/////////////////////     PAGES HANDLING     ///////////////////////////
//
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
///////////////////////////////////////
// SEND PAGE TO BUFFERPOOL USING HASH - IGNORE COLLISION FOR NOW
Page *send_page_to_bufferpool(Page page){
  uint64_t virtual_id = MAKE_VIRTUAL_ID(page.page_header.table_oid, page.page_header.page_id);
  uint32_t index = get_hash(virtual_id, PAGE_COUNT);
  

  bufferpool->pages[index] = page;
  return &bufferpool->pages[index];

}

///////////////////////////////////////
//
Tuple *read_tuple ( int page_id, int32_t offset ) {
  // CHECK PAGE TABLE 
  return 0;
};

//////////////////////////////////////
//
void write_tuple_into_page(Page *page,  Tuple tuple ){
  uint32_t start = page->page_header.empty_start_offset;
  uint32_t end = page->page_header.empty_end_offset;

  // SET PAGE LOCK

  // SET PAGE AS DIRTY

  // WRITE SLOT
  Slot s;
  s.tuple_oid=tuple.tuple_oid;
  s.offset=end - sizeof(tuple);
  
  *(page->slots + start - sizeof(PageHeader)) = s;

  // UPDATE EMPTY_START_OFFSET
  page->page_header.empty_start_offset += sizeof(s);

  // WRITE PAGE ENTRY
  PageEntry pe;
  pe.full_entry_size = sizeof(tuple) + sizeof(pe.full_entry_size);
  pe.tuple = tuple;
  
  uint32_t page_entries_size = sizeof(Page) - end;
  page->page_entries = page + sizeof(Page) - page_entries_size;
  
  *(page->page_entries - sizeof(pe)) = pe;

  // UPDATE EMPTY_END_OFFSET
  page->page_header.empty_end_offset - sizeof(tuple);

}

//////////////////////////////////////
//
int32_t *write_tuple (int64_t table_oid, Tuple tuple) {

  // LOAD NEXT WRITE PAGE
  Page *page = load_next_write_page(table_oid, sizeof(tuple));
  
  // WRITE TUPLE INTO PAGE
  page + page->page_header.empty_start_offset;

  // MARK PAGE AS DIRTY


  return 0;
}

///////////////////////////////////////////////////////////////////

///////////////////////////  PAGE TABLE    ///////////////////////
Page *get_page_address(uint32_t table_oid, uint32_t page_id){
  uint64_t virtual_page_id = (uint64_t)table_oid << 32 | page_id;

}

/////////////////////////////////////
//
void create_page_table_entry ( uint32_t table_oid, uint32_t page_id){
  PageTableEntry pte;
  int32_t entry_count = page_table->entry_count;
  page_table->page_table_entries[entry_count] = pte ;
  page_table->entry_count++;

  printf("Page Table Entry Created.\n %64d : %64d \nEntry Count : %32d",page_table->page_table_entries[page_table->entry_count-1].table_oid, page_table->page_table_entries[page_table->entry_count-1].virtual_page_id, page_table->entry_count );
  
};

////////////////////////////////////
//
void load_page( uint32_t table_oid, uint32_t page_id ) {
  // GET PAGE FROM STORAGE
   char filename = DATABASES_BASE_DIR;
   strcat(filename, int_to_string(ctx->database_oid));
  
   FILE *f = fopen(filename, "rb");
   fseek(f, 0, SEEK_END);
   int64_t filesize = ftell(f);
    
   Page page;
   if ( fseek(f, page_id*PAGE_SIZE, SEEK_SET) != 0) {
      perror("Page seek error: \n");
   } else {
    fread(&page, sizeof(Page), 1, f);
    }

  // CREATE PAGE TABLE ENTRY
  TableMetadata *metadata = get_table_metadata(table_oid);
  if ( metadata == NULL ){
    perror("Table Metadata not found in memory"); 
  } else {
    create_page_table_entry(table_oid, page_id);
  }

  // PUSH PAGE TO BUFFERPOOL
  send_page_to_bufferpool(page);

}

/////////////////////////////////////
//  
Page *load_next_write_page( uint32_t table_oid, uint32_t tuple_size) {

  // CHECK THE LAST "PAGE" FOR THE FILE AND CONFIRM SPACE REMAINING
   FILE *f = get_table_file(table_oid, "rb");
   fseek(f, 0, SEEK_END);
   int64_t filesize = ftell(f);
   Page page;
   fseek(f, -sizeof(Page), SEEK_END);
   
   fread(&page, sizeof(Page), 1, f);
   uint32_t page_count = filesize / sizeof(Page);

   // IF NOT ENOUGH, CREATE NEW PAGE
   if ( (page.page_header.empty_end_offset - page.page_header.empty_start_offset) < sizeof(Page) ) {
     Page new_page;
     uint32_t next_page_id = page_count + 1;  
     new_page.page_header.page_id = next_page_id;
     new_page.page_header.table_oid = table_oid;
     return &new_page;
   }
   
   return &page;
}

/////////////////////////////////////////////////////////////////
