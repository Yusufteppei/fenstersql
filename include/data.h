#ifndef DATA_H
#define DATA_H

#define TABLES_FILE "tables"
#define COLUMNS_FILE "columns"
#define DATABASES_FILE "databases"
#define PAGES_FILE "pages"


#include <stdint.h>
// Built to Support Variable Sizes for Efficient Memory Usage

typedef enum {
  TABLE_TYPE_METADATA,
  TABLE_TYPE_USER
}
TableType;


typedef struct {
  int32_t database_id;
  char name[32];
  long long int created_at;
}
Database;


typedef struct {
  char name[32];
  int32_t database_id;
  int16_t table_type;
}
Table;


typedef struct {
  int32_t column_id;
  char column_name[32];
  int32_t table_id;
  int64_t max_size;
}
Column;


typedef struct {
  int64_t SERIAL;
  int32_t table_id;
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
