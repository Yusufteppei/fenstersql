#include <stdio.h>
#include <stdlib.h>
#include "data.h"
/*
union{
  Table table;
  LoadedTable ltable;
}*/


void output_table_columns(int64_t table_oid){
    printf("Output columns for table %d\n", table_oid);
    Column *columns;
    int32_t column_count;
    TableMetadata *metadata = get_metadata_from_table_oid(table_oid);
    
    for ( int i=0; i<metadata->column_count; i++){
       printf("COl %s \n", metadata->columns[i].column_name);
     }
};


