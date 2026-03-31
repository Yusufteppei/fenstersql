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
      FILE *databases_file = fopen(DATABASES_FILE, "rb");
      fseek(databases_file, 0, SEEK_END);
      long long int filesize = ftell(databases_file);
      fseek(databases_file, 0, SEEK_SET);
      while ( 1 == 1 ) {
        if ( ftell(databases_file) > filesize ) break;

        fread(&d, sizeof(Database), 1, databases_file);
        printf("%s", d.name);
      }
  }
};


void create_object( char* object, char *value ) {

  printf("Creating %s %s\n", object, value);
  FILE *global = fopen("global", "ab");
  string_to_upper(object);
  if ( strcmp(object, "DATABASE") == 0 ){
      
      FILE *databases_file = fopen( DATABASES_FILE, "ab+" );
      int32_t databases_count;
      fread(&databases_count, sizeof(int32_t), 1, global);
      fseek(databases_file, 0, SEEK_SET);

      Database d;
      strcpy(d.name, value);
      d.database_id=databases_count+1;
      d.created_at=0;

      fwrite(&d, sizeof(Database), 1, databases_file);
      
      

      fclose(databases_file);

  }
  else if ( strcmp(object, "TABLE") == 0 ){
      FILE *tables_file = fopen( TABLES_FILE, "ab" );

      fwrite( value, sizeof(Table), 1, tables_file );

      fclose(tables_file);
  }
};
