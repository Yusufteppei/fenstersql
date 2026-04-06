#define FSTR_MAGIC 0x52545346
#ifndef DATA_H
#define DATA_H

#ifdef _WIN32
  #include <direct.h>
  #define MKDIR(path, perm) _mkdir(path)
#else
  #include <sys/stat.h>
  #define MKDIR(path, perm) mkdir(path, perm)
#endif

#define TABLES_FILE "data/sys/fstr_table"
#define COLUMNS_FILE "data/sys/fstr_column"
#define DATABASES_FILE "data/sys/fstr_database"
#define DATABASES_BASE_DIR "data/base/"
#define PAGES_FILE "data/sys/fstr_pages"
#define PAGE_TABLE_FILE "data/sys/fstr_pagetable"
#define GLOBAL_CONTROL_FILE "data/sys/fstr_global"

#include <stdint.h>
#include <errno.h>
#include <stdatomic.h>

// Built to Support Variable Sizes for Efficient Memory Usage


typedef enum {
  TABLE_TYPE_METADATA,
  TABLE_TYPE_USER
}
TableType;


typedef struct {
  char name[32];
  int64_t database_oid;
  long long int created_at;
}
Database;


typedef struct {
  char name[32];
  int64_t table_oid;
  TableType table_type;
}
Table;


typedef struct {
  int64_t column_oid;
  int64_t table_oid;
  char column_name[32];
  int64_t max_size;
}
Column;

typedef struct {
  int64_t tuple_oid;
  int64_t SERIAL;
  char *data;
}
Tuple;

// PERHAPS THERE'S NOTHING LIKE COLUMN
typedef struct {
  int64_t column_oid; // int64_t where the top32=table_id and btm32=col_id for unique
  char column_name[32];
  char *value;
}
Attribute;

typedef int64_t number;

typedef char string[32];

void select_object( char* object ) ;

void create_object( char* object, char value[32]) ;

#endif
