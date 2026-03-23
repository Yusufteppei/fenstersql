#ifndef PAGE_H
#define PAGE_H

#include <stdint.h>


typedef struct {
  int64_t ID;
}
PageHeader;

typedef struct {
  int64_t ID;
  int64_t page_id;
  int64_t offset;
}
Slot;

typedef struct {
    int64_t offset;
    int64_t size;
    char data[];
}
PageEntry;

typedef struct {

  int64_t ID;
  int32_t table_id;
  int64_t used;
  PageEntry page_entries[];
}
Page;


#endif
