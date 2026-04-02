#define _GNU_SOURCE
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"
#include <ctype.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>



int64_t peek_next_oid() {
  
  return atomic_load(&global_control->next_oid);
};

int64_t use_next_oid() {
  return atomic_fetch_add(&global_control->next_oid, 1);
}

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
      
      char directory_name[32] = DATABASES_BASE_DIR  "/" ;
      printf("naming folder..\n"); 
      strcat(&directory_name, oid_string);
      printf("folder named %s\n", directory_name);
      if ( mkdir(directory_name, 0755) == 0) {
        printf("Database Dog Created\n"); 
      } else {
        perror("Error\n");
      }; 
      

  }
  else if ( strcmp(object, "TABLE") == 0 ){
      FILE *tables_file = fopen( TABLES_FILE, "ab" );

      fwrite( value, sizeof(Table), 1, tables_file );

      fclose(tables_file);
  }
};
