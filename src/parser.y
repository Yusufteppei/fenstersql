%{
#include <stdio.h>
#include <stdlib.h>
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
%token CREATE TABLE DATABASE SEMICOLON COMMA RPAREN LPAREN TYPE_STRING TYPE_INT
%token <sval> IDENTIFIER STRING_LITERAL
%token <ival> NUMBER

%%

/* The entry point: allows for multiple statements */
input:
    /* empty */
    | input statement
    ;

statement:
    create_db_stmt
    | create_table_stmt
    ;

create_db_stmt:
    CREATE DATABASE IDENTIFIER SEMICOLON {
        printf("FensterSQL: Database '%s' created successfully.\n", $3);
        free($3); // Clean up the strdup memory
        return 0;
    }
    ;

create_table_stmt:
    CREATE TABLE IDENTIFIER LPAREN column_defs RPAREN SEMICOLON {
      printf("FensterSQL: Table %s created successfully.\n", $3);
      free($3);
    }
    ;

column_defs:
    column_def
    | column_defs COMMA column_def
    ;

column_def:
    IDENTIFIER data_type


data_type:
    TYPE_INT | TYPE_STRING

    

%%

void yyerror(const char *s) {
    fprintf(stderr, "\nSQL Error: %s\n", s);
    //signal(SIGINT, handle_sigint);
}
