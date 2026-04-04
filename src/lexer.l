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
"SELECT"            { return SELECT; }
"FROM"              { return FROM; }
"INSERT"            { return INSERT; }
"INTO"              { return INTO; }
"TABLE"             { return TABLE; }
"JOIN"              { return JOIN; }
"INNER"             { return INNER; }
"OUTER"             { return OUTER; }
"LEFT"              { return LEFT; }
"RIGHT"             { return RIGHT; }
"AS"                { return AS; }
"ON"                { return ON; }
"VALUES"            { return VALUES; }
"DATABASE"          { return DATABASE; }
"INT"               { return TYPE_INT; }
"STRING"            { return TYPE_STRING; }
"*"                 { return ASTERISK; }
"("                 { return LPAREN; }
")"                 { return RPAREN; }
","                 { return COMMA; }
";"                 { return SEMICOLON; }
"."                 { return DOT; }
[a-zA-Z_][a-zA-Z0-9_]* { yylval.sval = strdup(yytext); return IDENTIFIER; }
'[^']*'             { yylval.sval = strdup(yytext); return STRING_LITERAL; }
\"[^']*\"             { yylval.sval = strdup(yytext); return STRING_LITERAL; }
[0-9]*              { yylval.ival = yytext; return INT_LITERAL; }
[ \t\r\n]+          { /* ignore */ }
[+|-|=]             { return OPERATOR; }
.                   { return yytext[0]; }
%
