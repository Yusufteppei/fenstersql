#define FSTR_MAGIC 0x52545346
#ifndef DATA_H
#define DATA_H

#define TABLES_FILE "tables"
#define COLUMNS_FILE "columns"
#define DATABASES_FILE "databases"
#define PAGES_FILE "pages"
#define GLOBAL_FILE "data/global"

#include <stdint.h>
// Built to Support Variable Sizes for Efficient Memory Usage

const char MAGIC[4] = {'F', 'S', 'T', 'R'};

typedef struct {
  int32_t magic_number;
  int32_t next_db_id;
  int32_t db_count;
}
GlobalControl;

typedef struct {
  int32_t page_id;
  int32_t *page_address;
}
PageTableEntry;

typedef struct {
  int32_t magic_number;
  PageTableEntry *page_table_entries;
}
PageTable;


typedef enum {
  TABLE_TYPE_METADATA,
  TABLE_TYPE_USER
}
TableType;


typedef struct {
  char name[32];
  int32_t database_oid;
  long long int created_at;
}
Database;


typedef struct {
  char name[32];
  int32_t database_oid;
  int32_t table_oid;
  int16_t table_type;
}
Table;


typedef struct {
  int32_t column_oid;
  char column_name[32];
  int32_t table_oid;
  int64_t max_size;
}
Column;


typedef struct {
  int32_t tuple_oid;
  int64_t SERIAL;
  char *data;
}
Tuple;


typedef struct {
  int32_t column_id; // int64_t where the top32=table_id and btm32=col_id for unique
  char column_name[32];
  char *value;
}
Attribute;


void select_object( char* object ) ;

void create_object( char* object, char value[32]) ;

#endif
