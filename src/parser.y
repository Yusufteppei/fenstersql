%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "signals.h"
#include <signal.h>
#include "data.h"
#include "connection.h"


/* Prototypes to keep the compiler happy */
void yyerror(const char *s);
int yylex();
%}

/* Define the data types for tokens */
%union {
    int ival;
    char *sval;
    struct TempCol* col;
    struct Column* column;
    int dtype;
    struct Table* table_data;
}

/* Define tokens and associate IDENTIFIER with sval */
%token CREATE SELECT INSERT INTO TABLE DATABASE VALUES FROM
%token RIGHT LEFT INNER OUTER JOIN ON AS
%token SEMICOLON DOT COMMA RPAREN LPAREN EQUAL OPERATOR ASTERISK
%token QUIT
%token <sval> IDENTIFIER STRING_LITERAL TYPE_INT TYPE_STRING
%token <ival> INT_LITERAL

%type <column> column_defs
%type <table_data> joined_table
%type <column> column_def
%type <dtype> data_type
%%

/* The entry point: allows for multiple statements */
input:
    /* empty */
    | input statement
    ;

statement:
    create_db_stmt
    | create_table_stmt
    | insert_stmt
    | select_stmt

    | quit_stmt
    ;

create_db_stmt:
    CREATE DATABASE IDENTIFIER SEMICOLON {
        Database d;
        d.database_oid = use_next_oid();
        strcpy(d.name, $3);
        create_database(d);
        printf("Database '%s' created successfully.\n", $3);
        free($3); // Clean up the strdup memory
        return 0;
    }
    ;

create_table_stmt:
    CREATE TABLE IDENTIFIER LPAREN column_defs RPAREN SEMICOLON {
      Table t;
      t.table_oid = use_next_oid();
      t.table_type = TABLE_TYPE_USER;
      strcpy(t.name, $3);
      create_table(ctx, t);

      TempCol* curr = $5;
      int i = 0;
      while (curr != NULL) {
        Column col;
        col.column_oid = use_next_oid();
        col.table_oid = t.table_oid;
        strcpy(col.column_name, curr->data.column_name);
        col.column_order = i++;
        col.data_type = curr->data.data_type;
        //printf("Data Type: %s\nCol Name: %s\n ", col.data_type, col.column_name);
        //printf("Curr : Data Type: %s\nCol Name: %s\n ", curr->data.data_type, curr->data.column_name);
        create_column(col);
        printf("Create column %s %d function complete\n", col.column_name, col.column_oid);
        // Clean up as you go or after
        Column* temp = curr;
        curr = curr->next;
        //free(temp->column_name);
        free(temp);
      }
      free($3);
    }
    ;

insert_stmt:
  INSERT INTO IDENTIFIER VALUES LPAREN records RPAREN SEMICOLON {
    printf("INSERT INTO success.\n");
  }
  ;

select_stmt:
  SELECT column_sels FROM joined_table SEMICOLON {
    printf("Planning...\n");
    //plan();
    printf("Optimizing...\n");
    //execute();
    int64_t table_oid = get_table_oid(ctx, $4);;
    
    output_table_columns(table_oid);
  }
  ;

quit_stmt:
  QUIT {
    handle_sigint(0);
  }
  ;

column_sels:
  column_sel
  | column_sels COMMA column_sel

column_sel:
  value
  | ASTERISK
  | IDENTIFIER DOT ASTERISK
  | IDENTIFIER DOT IDENTIFIER

joined_table:
  IDENTIFIER
  | joined_table join_command IDENTIFIER ON join_predicate
  | joined_table join_command IDENTIFIER AS IDENTIFIER ON join_predicate
  ;
 
join_command:
  JOIN
  | join_type JOIN
  
join_predicate:
  IDENTIFIER DOT IDENTIFIER EQUAL IDENTIFIER
  | IDENTIFIER DOT IDENTIFIER EQUAL IDENTIFIER DOT IDENTIFIER
  | IDENTIFIER DOT IDENTIFIER EQUAL value

join_type:
  OUTER | INNER | LEFT | RIGHT

records:
    record
    | records COMMA record
    ;

record:
    LPAREN values RPAREN
    ;

values:
    value
    | values COMMA value
    ;

value:
    INT_LITERAL | STRING_LITERAL
    | INT_LITERAL OPERATOR INT_LITERAL
    | IDENTIFIER OPERATOR IDENTIFIER
    | STRING_LITERAL OPERATOR STRING_LITERAL
    | STRING_LITERAL OPERATOR INT_LITERAL
    ;

column_defs:
    column_def 
    {
      $$ = $1;
    }
    | column_defs COMMA column_def
    {
      TempCol* current = $1;
      while(current->next != NULL) current = current->next;
      current->next = $3;
      $$ = $1;
    }
    ;

column_def:
    IDENTIFIER data_type 
    {
        //printf("ID: %s dtype: %s\n", $1, $2);
        TempCol* node = malloc(sizeof(TempCol));
        memset(&node->data, 0, sizeof(Column));
        
        strncpy(node->data.column_name, $1, 31);
        node->data.data_type = $2;
        node->next = NULL;
        $$ = node;
    }
    ;

data_type:
    TYPE_INT { $$ = DTYPE_INT; }
    | TYPE_STRING { $$ = DTYPE_STRING; }
    ;

    
%%

void yyerror(const char *s) {
    fprintf(stderr, "\nSQL Error: %s\n", s);
    //signal(SIGINT, handle_sigint);
}
