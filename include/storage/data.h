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
#define TABLES_METADATA_FILE "data/sys/fstr_tables_metadata"
#define COLUMNS_FILE "data/sys/fstr_column"
#define DATABASES_FILE "data/sys/fstr_database"
#define DATABASES_BASE_DIR "data/base/"
#define PAGES_FILE "data/sys/fstr_pages"
#define GLOBAL_CONTROL_FILE "data/sys/fstr_global"

#include <stdint.h>
#include <errno.h>
#include <stdatomic.h>
#include "connection.h"
#include "parser/parser.tab.h"

// Built to Support Variable Sizes for Efficient Memory Usage

/*
typedef int64_t NUMBER;

typedef char STRING[32];
typedef enum {
  TYPE_INT,
  TYPE_STRING
}
DataType;
*/

typedef enum {
  TABLE_TYPE_METADATA,
  TABLE_TYPE_USER
}
TableType;

typedef enum {
  DTYPE_INT,
  DTYPE_STRING
}
DataType;


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
  int64_t table_oid;
  int64_t column_oid;
  char column_name[32];
  int32_t column_order;
  DataType data_type;
  int64_t next_column_oid; //WITHIN THE SAME TABLE
}
Column;


typedef struct {
  int64_t table_oid;
  int32_t column_count;
  Column *columns;
}
TableMetadata;
extern TableMetadata *tables_metadata;


// FOR THE BISON PARSING RECURSION
typedef struct TempCol {
    Column data;
    struct TempCol *next;
} TempCol;


typedef struct {
  int64_t tuple_oid;
  int64_t SERIAL;
  char *data;
}
Tuple;


// PERHAPS THERE'S NOTHING LIKE COLUMN
typedef struct {
  int64_t column_oid; 
  char column_name[32];
  char *value;
}
Attribute;

typedef enum {
    REF_PHYSICAL_TABLE,
    REF_JOIN
} RefType;

typedef struct TableRef {
    RefType type;
    char* table_name;   // The name Bison just found
    char* alias;        // For when you add "AS u"
    struct TableRef* left;  // NULL for now
    struct TableRef* right; // NULL for now
} TableRef;
/*
typedef struct {
  int64_t oid;
  Tuple *tuples;
}
LoadedTable;
*/

void create_table_of_tables ();

void create_table_of_dbs ();

int64_t get_table_oid( Context *context, char *name);

void create_table( Context *context, Table table);

int database_exists( char *database_name );

int create_database( Database database );

TableMetadata *get_metadata_from_table_oid( int64_t table_oid);
#endif
