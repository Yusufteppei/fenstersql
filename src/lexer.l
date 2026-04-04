%{
#include "parser.tab.h"
#include <string.h>
#include <stdlib.h>
void yyerror(const char *s);
%}

%option caseless
%option noyywrap

%%
"CREATE"            { return CREATE; }
"TABLE"             { return TABLE; }
"DATABASE"          { return DATABASE; }
"INT"               { return TYPE_INT; }
"STRING"            { return TYPE_STRING; }
"("                 { return LPAREN; }
")"                 { return RPAREN; }
","                 { return COMMA; }
";"                 { return SEMICOLON; }
[a-zA-Z_][a-zA-Z0-9_]* { yylval.sval = strdup(yytext); return IDENTIFIER; }
"[^']*"             { yylval.sval = strdup(yytext); return STRING_LITERAL; }
[ \t\r\n]+          { /* ignore */ }
"\f"                { system("clear"); printf("fenstersql# "); fflush(stdout);}
.                   { return yytext[0]; }
%%
