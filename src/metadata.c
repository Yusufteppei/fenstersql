#include <stdio.h>
#include <stdlib.h>
#include "data.h"
#include "page.h"
#include "metadata.h"


TableMetadata *get_metadata_from_table_oid( int64_t table_oid){
  int i=0;
  while ( tables_metadata[i].table_oid != NULL ){
    if ( tables_metadata[i].table_oid == table_oid ) {
      return &tables_metadata[i];

    }
    i++;
  }
  return NULL;
};



