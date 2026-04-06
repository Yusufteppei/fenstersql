#define _GNU_SOURCE
#include <stdint.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"
#include "page.h"
#include "connection.h"
#include <ctype.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

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

///////////////////////////////////////////////////////////////////////

///////////////////////////    TABLES     ///////////////////////////////
int64_t get_table_oid(Context *context, char *name) {
  // INPUT TABLE NAME AND GET TABLE_OID ELSE RETURNS 1 -  WITHIN CURRENT DB
  printf("Confirming table doesn't exist\n");
  FILE *tables_file = fopen(TABLES_FILE, "rb");
  fseek(tables_file, 0, SEEK_END);
  long int filesize = ftell(tables_file);
  fseek(tables_file, 0, SEEK_SET);
  Table t;
  while( ftell(tables_file) < filesize){
    fread(&t, sizeof(Table), 1, tables_file);
    printf("Table name : %s\n", t.name);
    if ( strcmp(t.name, name) == 0 ) {
      fclose(tables_file);
      return t.table_oid;
    }
  }
  printf("Closing file..\n"); 
  fclose(tables_file);
  
  return 1;
}

void create_table(Context *context, Table table){
  // CHECK TABLE NAME EXISTENCE IN DATABASE
  if ( get_table_oid(ctx, table.name) != 1 ) {
    printf("Table %s already exists.\n", table.name);
    return;
  };
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
  if ( fopen(filename, "wx") != 0 ){
      //creat(filename, 0755) ;
      printf("Table File Created\n"); 
  }
  else {
    perror("Table File Creation\n");
  };

  // STORE TABLE METADATA IN TABLES_FILE (TABLES OF TABLES)
  FILE *tables_file = fopen( TABLES_FILE, "ab" );

  fwrite(&table, sizeof(Table), 1, tables_file );

  fclose(tables_file);

  // CREATE COLUMNS
  
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

      char oid_string[12]; 

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

};

//////////////////////////    END DATABASES        //////////////////////
