#ifndef PAGE_H
#define PAGE_H
#define FSTR_MAGIC 0x52545346
 
#include <stdint.h>
#include "connection.h"
#include "data.h"



#define PAGE_SIZE 128 // 4096  // KEPT TINY FOR DEVELOPMENT
#define BUFFERPOOL_SIZE 1024*1024*1024
#define MAX_LIVE_PAGE_COUNT BUFFERPOOL_SIZE / PAGE_SIZE  - 10  //10 is a safe estm8 4now


#define PAGE_STATUS_FREE    (1 << 0)
#define PAGE_STATUS_OCCUPIED (1 << 1)
#define PAGE_STATUS_LOADING (1 << 2)

#define FLAG_CLEAN   (1 << 0)
#define FLAG_DIRTY   (1 << 1)
#define FLAG_IO_BUSY (1 << 2)
#define FLAG_LOCKED (1 << 3)


typedef struct {
  _Atomic int64_t next_oid;
}
GlobalControl;
extern GlobalControl *global_control;


typedef struct {
  int64_t table_oid;
  TableMetadata *metadata;
  int32_t page_id;
}
PageTableEntry;

typedef struct {
  PageTableEntry *page_table_entries;
  int32_t entry_count;
}
PageTable;
extern PageTable *page_table;

typedef struct {
  int64_t page_id;
  int64_t table_oid;
  int8_t status;
  int8_t flags;
  int32_t empty_start_offset;
  int32_t empty_end_offset; // 4 BYTES(DIGITS) SINCE PAGES ARE 4096 BYTES(4 DIGITS)
}
PageHeader;


typedef struct {
  int64_t page_id;
  int64_t offset;
}
Slot;


typedef struct {
    int64_t size;
    char data[];
}
PageEntry;


typedef struct {
  PageHeader page_header;
  Slot *slots;
  PageEntry *page_entries;
}
Page;

typedef struct {
  GlobalControl global_control;
  Page pages[];
}
BufferPool;
extern BufferPool *bufferpool;

int64_t peek_next_oid();

int64_t use_next_oid();

Page *get_start_page_from_table_oid ( int64_t table_oid );

void create_page_table_entry ( PageTableEntry page_table_entry);

Page *load_table( Context *context, int table_oid );

Tuple *read_tuple ( int page_id, int32_t offset );

Tuple *write_tuple ();
#endif

