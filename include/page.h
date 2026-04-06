#ifndef PAGE_H
#define PAGE_H
#define FSTR_MAGIC 0x52545346
 
#include <stdint.h>
#define PAGE_SIZE 4096
#define BUFFERPOOL_SIZE 1024*1024*1024
#define MAX_LIVE_PAGE_COUNT BUFFERPOOL_SIZE / PAGE_SIZE  - 10  //10 is a safe estm8 4now

typedef struct {
  _Atomic int64_t next_oid;
}
GlobalControl;
extern GlobalControl *global_control;


typedef struct {
  int32_t page_id;
  int32_t *page_address;
}
PageTableEntry;

typedef struct {
  PageTableEntry *page_table_entries;
}
PageTable;

typedef struct {
  int64_t page_id;
  int64_t table_id;
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
#endif

