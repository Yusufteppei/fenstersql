#define _GNU_SOURCE
#include <stdint.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"
#include "page.h"
#include "metadata.h"
#include "connection.h"
#include <ctype.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

///////////////////////////    COLUMNS         /////////////////////////
void create_column(Column column) {
  // EMPTY NEXT POINTER
  //column.next = NULL;
  //Column col;
  // WRITE TO COLUMNS FILE

  printf("Create column function started\n");
  FILE *columns_file = fopen(COLUMNS_FILE, "ab");
  fseek(columns_file, 0, SEEK_END);

  fwrite(&column, sizeof(Column), 1, columns_file);
  printf("Write to columns file complete : %s\n", column.column_name);
  fclose(columns_file);

  // UPDATE TABLE COLUMNS ON MEMORY - TABLES_METADATA
  for ( int i=0; i<page_table->entry_count; i++) {
    if ( page_table->page_table_entries[i].table_oid == column.table_oid ) {

        int32_t col_count = page_table->page_table_entries[i].metadata->column_count;
        printf("Columns count : %31d\n",col_count);
        page_table->page_table_entries[i].metadata->columns[col_count] = column;
        page_table->page_table_entries[i].metadata->column_count++;
        
        printf("Column %s Added to Memory\n", page_table->page_table_entries[i].metadata->columns[col_count-1].column_name);

    }
  }


};

//INEFFICIENT - LATER A HASHTABLE THAT CONTAINS A POINTER TO THE COLUMNS LIST
int load_columns(int64_t table_oid) {
  //  CHECK IF IT'S ALREADY LOADED - SEQUENTIAL FOR NOW
  for (TableMetadata *i=tables_metadata; i->table_oid != NULL; i++) {
    if ( i->table_oid == table_oid && i->columns != NULL) {
      printf("Table %lld Metadata Already Loaded\n", table_oid);
      return 1;
    }
  }

  TableMetadata *metadata = get_metadata_from_table_oid(table_oid);
  if (metadata == NULL) {
    fprintf(stderr, "Error: Metadata for OID %lld not found.\n", table_oid);
    return; // Or handle error
  }
  if (metadata->columns == NULL) {
    metadata->column_count = 0;
    // Start with space for, say, 10 columns, or realloc as you go
    metadata->columns = malloc(sizeof(Column) * 10);
  }
  //printf("Metadata %d\n", metadata->table_oid);
  //
  Column *temp_col = malloc(sizeof(Column));
  FILE *columns_file = fopen(COLUMNS_FILE, "rb");
  fseek(columns_file, 0, SEEK_END);
  long int filesize = ftell(columns_file);
  fseek(columns_file, 0, SEEK_SET);

  for (int i=0; i<=filesize/sizeof(Column); i++) {
    if ( fread(temp_col, sizeof(Column), 1, columns_file) != 1 ) break;
    printf("Loop - col_oid: %d col_table_oid: %d table_oid: %d\n", temp_col->column_oid, temp_col->table_oid, table_oid);
    // PUSH COLUMN TO TABLE METADATA, UPDATE COLUMN COUNT
    if ( temp_col->table_oid == table_oid ) {
      printf("Column OID %d matched\n", temp_col->column_oid);
      metadata->columns[metadata->column_count] = *temp_col;
      metadata->column_count++;
    };
  

  };
  printf("Table %d has %d columns\n", table_oid, metadata->column_count);
  
  free(temp_col);
  fclose(columns_file);
  return 0;
}

void load_all_columns(){
  Column *col = malloc(sizeof(Column));
  FILE *columns_file = fopen(COLUMNS_FILE, "rb");
  fseek(columns_file, 0, SEEK_END);
  long int filesize = ftell(columns_file);
  fseek(columns_file, 0, SEEK_SET);
  
  int64_t t_oid;

  for (int i=0; i<filesize/sizeof(Column); i++){
    t_oid = col->table_oid; 
    
  }
}

char *get_columns_from_table_oid(int64_t table_oid){
  

}

///////////////////////////////////////////////////////////////////////

///////////////////////////    SYSTEM TABLES   /////////////////////////
void create_table_of_dbs(){


};

void create_table_of_tables(){};

void create_system_tables() {
/*
  1. Create a table of tables with columns ( name, oid, db_oid, type(database)  ) 
  2. Create a table of databases with columns ( name, oid, type(table) )
  3. Create a table of indexes with columns (name, oid, type(index) )
*/
  create_table_of_dbs();
  create_table_of_tables();
  
};

void load_tables_metadata(){
  // READ TABLES FILE
  FILE *tables_file = fopen(TABLES_FILE, "rb");
  long int filesize;
  fseek(tables_file, 0, SEEK_END);
  filesize = ftell(tables_file);
  fseek(tables_file, 0, SEEK_SET);
  
  Table *t = malloc(sizeof(Table));
  int count = 0;
  while( ftell(tables_file) < filesize ) {
    fread(t, sizeof(Table), 1, tables_file);
    tables_metadata[count].table_oid = t->table_oid;
    printf("Loading columns for Table %d \n", tables_metadata[count].table_oid);
    load_columns(tables_metadata[count].table_oid);
    count++;
  }

  free(t);
};

///////////////////////////////////////////////////////////////////////

///////////////////////////    TABLES     ///////////////////////////////
int64_t get_table_oid(Context *context, char *name) {
  // INPUT TABLE NAME AND GET TABLE_OID ELSE RETURNS 1 -  WITHIN CURRENT DB
  //printf("Confirming table doesn't exist\n");
  FILE *tables_file = fopen(TABLES_FILE, "rb");
  fseek(tables_file, 0, SEEK_END);
  long int filesize = ftell(tables_file);
  fseek(tables_file, 0, SEEK_SET);
  Table t;
  while( ftell(tables_file) < filesize){
    fread(&t, sizeof(Table), 1, tables_file);
    //printf("Table name : %s\n", t.name);
    if ( strcmp(t.name, name) == 0 ) {
      fclose(tables_file);
      return t.table_oid;
    }
  }
  //printf("Closing file..\n"); 
  fclose(tables_file);
  
  return 1;
}

void create_table(Context *context, Table table){
  // CHECK TABLE NAME EXISTENCE IN DATABASE
  if ( get_table_oid(ctx, table.name) != 1 ) {
    printf("Table %s already exists.\n", table.name);
    return;
  } 
  else {

    char oid_string[12]; 
    char database_oid_string[12];
    char directory_name[64] = DATABASES_BASE_DIR;
    char filename[64];
  
    printf("Generating DB OID string from OID int %ld\n", ctx->database_oid);
    snprintf(&database_oid_string, sizeof(database_oid_string), "%d", ctx->database_oid);
  
    printf("\n%s\nConcatenating directory name from DB OID string\n", oid_string);

    strcat(&directory_name, database_oid_string);
    strcat(&directory_name, "/");
    printf("\n%s\nConcatenating directory name to TABLE OID string\n", directory_name);
  
    snprintf(&oid_string, sizeof(oid_string), "%d", table.table_oid);
    strcpy(filename,directory_name);
    strcat(&filename, oid_string);
  
    printf("Table File %s\n", filename);

    // CREATE TABLE FILE WITHIN CONTEXT DB
    if ( fopen(filename, "wx") != NULL ){
      //creat(filename, 0755) ;
      printf("Table File Created\n"); 
     }
    else {
      perror("Table File Creation");
    };

    // STORE TABLE METADATA IN TABLES_FILE (TABLES OF TABLES)
    FILE *tables_file = fopen( TABLES_FILE, "ab" );

    fwrite(&table, sizeof(Table), 1, tables_file );

    printf("Metadata written to tables file\n");
    fclose(tables_file);

  }
};


/////////////////////////    END TABLES     //////////////////////////////

/////////////////////////    DATABASES    /////////////////////////////
int database_exists( char *database_name ) {
  
    
    FILE *databases_file = fopen( DATABASES_FILE, "rb");
    fseek(databases_file, 0, SEEK_END);
    long long int filesize = ftell(databases_file);

    fseek(databases_file, 0, SEEK_SET);

    Database *d;
    
    do {
      if ( strcmp(database_name, d->name) == 0 ) {
       return 1; 
      }
      fseek(databases_file, sizeof(Database), SEEK_CUR);
    }
    while( ftell(databases_file) < filesize);
    
    free(d);
    return 0;
}


int create_database( Database database ) {
      if ( database_exists(database.name) ){
        printf("Database Exists\n");
        return 1;
      }

      char oid_string[12]; 
      // CREATE FOLDER
      snprintf(oid_string, sizeof(oid_string), "%d", database.database_oid);
      printf("OID is %d\n", database.database_oid);
      database.created_at=0;
      
      char directory_name[32] = DATABASES_BASE_DIR ;
      printf("naming folder..\n"); 
      strcat(&directory_name, oid_string);
      printf("folder named %s\n", directory_name);
      if ( mkdir(directory_name, 0755) == 0) {
        printf("Database %s Created\n", database.name); 
      } else {
        perror("Error\n");
      }; 

      // UPDATE DATABASES FILE
};

//////////////////////////    END DATABASES        //////////////////////
