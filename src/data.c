#include <stdint.h>
#include <string.h>
#include "data.h"
#include <ctype.h>
#include <stdio.h>


void select_object( char* object ){

  if ( strcmp(toupper(object), "DATABASE") == 0) {
      Database d;
      FILE *database_file = fopen(DATABASES_FILE, "rb");
      fseek(database_file, 0, SEEK_END);
      long long int filesize = ftell(database_file);
      fseek(database_file, 0, SEEK_SET);
      while ( 1 == 1 ) {
        if ( ftell(database_file) > filesize ) break;

        fread(&d, sizeof(Database), 1, database_file);
        printf("%s", d.name);
      }
  }
};


void create_object( char* object, char *value ) {

  printf("Creating %s %s\n", object, value);
  FILE *global = fopen("global", "ab");
  string_to_upper(object);
  if ( strcmp(object, "DATABASE") == 0 ){
      FILE *databases_file = fopen( DATABASES_FILE, "ab+");
      int32_t database_count;
      fread(&database_count, sizeof(int32_t), 1, global);
      fseek(databases_file, 0, SEEK_SET);

      Database d;
      strcpy(d.name, value);
      d.database_id=database_count+1;
      d.created_at=0;//system("date");

      fwrite(&d, sizeof(Database), 1, databases_file);

      fclose(databases_file);

  }
  else if ( strcmp(object, "TABLE") == 0 ){
      FILE *tables_file = fopen( TABLES_FILE, "ab" );

      fwrite( value, sizeof(Table), 1, tables_file );

      fclose(tables_file);
  }
};
