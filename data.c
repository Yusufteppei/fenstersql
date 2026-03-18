#include <stdint.h>
#include "data.h"
#include <stdio.h>


void select_object( int32_t object ){
  
  switch ( object ) {
    case DATABASE:
      Database d;
      FILE *database_file = fopen(DATABASES_FILE, "rb");
      fseek(database_file, 0, SEEK_END);
      long long int filesize = ftell(database_file);
      fseek(database_file, 0, SEEK_SET);
      while ( 1==1 ) {
        if ( ftell(database_file) > filesize ) break;

        fread(&d, sizeof(Database), 1, database_file);
        printf("%s", d.name);
      }
    default:
      break;
  }
};

void create_object( int32_t object, char value[32] ) {
  printf("Creating %d %s", object, value);
  FILE *global = fopen("global", "ab");

  switch ( object ) {

    case DATABASE:
      FILE *databases_file = fopen( DATABASES_FILE, "ab+");
      int32_t database_count;
      fread(&database_count, sizeof(int32_t), 1, global);
      fseek(databases_file, 0, SEEK_SET);
      
      Database d;
      strcpy(d.name, value);
      d.database_id=database_count+1;
      d.created_at=system("date");

      fwrite(&d, sizeof(Database), 1, databases_file);

      fclose(databases_file);
      break;

    case TABLE:
      FILE *tables_file = fopen( TABLES_FILE, "ab" );
      //Table table;
      fwrite( value, sizeof(Table), 1, tables_file );

      fclose(tables_file);
      break;

    default:
      break;

  };
};
