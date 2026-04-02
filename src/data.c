#define _GNU_SOURCE
#include <stdint.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"
#include "page.h"
#include <ctype.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

///////////////////////////    SYSTEM TABLES   /////////////////////////
void create_table_of_dbs(){};

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
/*
  1. Get OID
  2. If it's a metadata table, 
*/


void create_table(Table table){
  char oid_string[12]; 
  char database_oid_string[12];
  
  char directory_name[64] = DATABASES_BASE_DIR;
  char filename[64];
  printf("Generating DB OID string from OID int %d\n", global_control->database_oid);
  snprintf(&database_oid_string, sizeof(database_oid_string), "%d", global_control->database_oid);
  
  printf("\n%s\nConcatenating directory name from DB OID string\n", oid_string);

  strcat(&directory_name, database_oid_string);
  strcat(&directory_name, "/");
  printf("\n%s\nConcatenating directory name to TABLE OID string\n", directory_name);
  
  snprintf(&oid_string, sizeof(oid_string), "%d", table.table_oid);
  strcpy(filename,directory_name);
  strcat(&filename, oid_string);
  
  printf("Table File %s\n", filename);
  // CREATE COLUMNS

  // CREATE TABLE FILE WITHIN CONTEXT DB
  if ( fopen(filename, "wx") != 0 ){
      //creat(filename, 0755) ;
      printf("Table file created"); 
  }
  else {
    perror("Message");
  };

  // STORE TABLE METADATA IN TABLES_FILE
  FILE *tables_file = fopen( TABLES_FILE, "ab" );

  fwrite(&table, sizeof(Table), 1, tables_file );

  fclose(tables_file);
  

  // INCLUDE NEW TABLE INTO TABLE OF TABLES
  
};





///////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////

//////////////////////////                /////////////////////
void select_object( char* object ){

  if ( strcmp(toupper(object), "DATABASE") == 0) {
      Database d;
      int64_t database_oid;
      //if ( mkdir(
      
  }
};


void create_object( char* object, char *value ) {

  printf("Creating %s %s\n", object, value);
  FILE *global = fopen("global", "ab");
  string_to_upper(object);
  if ( strcmp(object, "DATABASE") == 0 || strcmp(object, "DB") == 0 ){
      
      char oid_string[12]; 

      Database d;
      strcpy(d.name, value);
      d.database_oid=use_next_oid();
      
      snprintf(oid_string, sizeof(oid_string), "%d", d.database_oid);
      printf("OID is %d\n", d.database_oid);
      d.created_at=0;
      
      char directory_name[32] = DATABASES_BASE_DIR ;
      printf("naming folder..\n"); 
      strcat(&directory_name, oid_string);
      printf("folder named %s\n", directory_name);
      if ( mkdir(directory_name, 0755) == 0) {
        printf("Database %s Created\n", d.name); 
      } else {
        perror("Error\n");
      }; 
      

  }
  else if ( strcmp(object, "TABLE") == 0 ){
      Table table;
      printf("Copying name into table struct\n");
      strcpy(&table.name, value);
      printf("Setting table OID\n");
      table.table_oid = use_next_oid();
      table.table_type = TABLE_TYPE_USER;

      create_table(table);
  }
};
//////////////////////////////////////////////////////////
