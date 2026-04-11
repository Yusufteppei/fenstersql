#ifndef PAGE_H
#define PAGE_H
#define FSTR_MAGIC 0x52545346
 
#include <stdint.h>
#include "connection.h"
#include "data.h"

#define PAGE_SIZE 128 // 4096  // KEPT TINY FOR DEVELOPMENT
#define BUFFERPOOL_SIZE 1024*1024*1024
#define PAGE_COUNT BUFFERPOOL_SIZE / PAGE_SIZE  - 10  //10 is a safe estm8 4now
#define PAGE_STATUS_FREE    (1 << 0)
#define PAGE_STATUS_OCCUPIED (1 << 1)
#define PAGE_STATUS_LOADING (1 << 2)

#define FLAG_CLEAN   (1 << 0)
#define FLAG_DIRTY   (1 << 1)
#define FLAG_IO_BUSY (1 << 2)
#define FLAG_LOCKED (1 << 3)

#define MAKE_VIRTUAL_ID(t, p) (((uint64_t)(t) << 32) | (p))


typedef struct {
  _Atomic int64_t next_oid;
}
GlobalControl;
extern GlobalControl *global_control;


typedef struct {
  uint32_t table_oid;
  uint32_t page_id;
  uint32_t *next_page;
  uint32_t empty_start_offset;
  uint32_t empty_end_offset; 
  uint8_t status;
  uint8_t flags;
}
PageHeader;


typedef struct {
  uint32_t tuple_oid;
  uint32_t offset;
}
Slot;


typedef struct {
    uint32_t full_entry_size;
    Tuple tuple;
}
PageEntry;


typedef struct {
  PageHeader page_header;
  Slot *slots;
  PageEntry *page_entries;
}
Page;

typedef struct {
  uint32_t table_oid;
  uint64_t virtual_page_id; // [table_oid][page_id]
  TableMetadata *metadata;
  //Page *page;
  uint32_t page_index;
}
PageTableEntry;

typedef struct {
  PageTableEntry *page_table_entries;
  uint32_t entry_count;
}
PageTable;
extern PageTable *page_table;


typedef struct {
  GlobalControl global_control;
  Page pages[];
}
BufferPool;
extern BufferPool *bufferpool;

int64_t peek_next_oid();

int64_t use_next_oid();

Page *get_start_page_from_table_oid ( int64_t table_oid );

void create_page_table_entry ( uint32_t, uint32_t);

Page *load_table( Context *context, int table_oid );

Page *send_page_to_bufferpool ( Page page );

Tuple *read_tuple ( int page_id, int32_t offset );

void write_tuple_into_page( Page *page, Tuple tuple );

int32_t *write_tuple (int64_t table_oid, Tuple tuple);

Page *get_page_address(uint32_t table_oid, uint32_t page_id);

void create_page_table_entry( uint32_t table_oid, uint32_t page_id );

void load_page( uint32_t table_oid, uint32_t page_id );

Page *load_next_write_page( uint32_t table_oid, uint32_t tuple_size);
#endif

