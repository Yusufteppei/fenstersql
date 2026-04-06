%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "signals.h"
#include <signal.h>
#include "data.h"


/* Prototypes to keep the compiler happy */
void yyerror(const char *s);
int yylex();
%}

/* Define the data types for tokens */
%union {
    int ival;
    char *sval;
}

/* Define tokens and associate IDENTIFIER with sval */
%token CREATE SELECT INSERT INTO TABLE DATABASE VALUES FROM
%token RIGHT LEFT INNER OUTER JOIN ON AS
%token SEMICOLON DOT COMMA RPAREN LPAREN EQUAL OPERATOR ASTERISK
%token TYPE_STRING TYPE_INT
%token QUIT
%token <sval> IDENTIFIER STRING_LITERAL
%token <ival> INT_LITERAL

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
      strcpy(t.name, $3);
      t.table_type = TABLE_TYPE_USER;
      create_table(t);
      printf("Table %s created successfully.\n", $3);
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
    printf("SELECT success.\n");
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
    | column_defs COMMA column_def
    ;

column_def:
    IDENTIFIER data_type
    ;


data_type:
    TYPE_INT | TYPE_STRING
    ;

    
%%

void yyerror(const char *s) {
    fprintf(stderr, "\nSQL Error: %s\n", s);
    //signal(SIGINT, handle_sigint);
}
