#ifndef DATA_H
#define DATA_H

#define TABLES_FILE "tables"
#define COLUMNS_FILE "columns"
#define DATABASES_FILE "databases"

// Built to Support Variable Sizes for Efficient Memory Usage

typedef enum {
  DATABASE,
  TABLE,
  COLUMN,
  TUPLE,
  ATTRIBUTE
}
Object;

typedef struct {
  int32_t database_count;
}
Global;

typedef struct {
  int32_t database_id;
  char name[32];
  long long int created_at;
}
Database;

typedef struct {
  char name[32];
  int32_t database_id;
  //char *column_names;
  int64_t max_size; // CALCULATE FROM THE SUM OF COLUMN MAX_SIZE(S)
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

#endif
