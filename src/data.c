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
/*
  1. Get OID
  2. If it's a metadata table, 
*/


void create_table(Table table){

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
  // CREATE COLUMNS

  // CREATE TABLE FILE WITHIN CONTEXT DB
  if ( fopen(filename, "wx") != 0 ){
      //creat(filename, 0755) ;
      printf("Table File Created"); 
  }
  else {
    perror("Table File Creation");
  };

  // STORE TABLE METADATA IN TABLES_FILE
  FILE *tables_file = fopen( TABLES_FILE, "ab" );

  fwrite(&table, sizeof(Table), 1, tables_file );

  fclose(tables_file);
  

  // INCLUDE NEW TABLE INTO TABLE OF TABLES
  
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

