%{
#include <stdio.h>
#include <stdlib.h>
#include "calcy.tab.h"

void yyerror(const char *s) {
    fprintf(stderr, "%s\n", s);
}

%}

%option noyywrap

%%

[a-z] {
    printf("Variable: %s\n", yytext);
    return VARIABLE;
}

[0-9]+ {
    printf("Integer: %s\n", yytext);
    yylval.iValue = atoi(yytext);
    return INTEGER;
}

[ \t\n\r]+   { /* Ignore whitespace */ }

"+"          { printf("+ operator\n"); return PLUS; }
"-"          { printf("- operator\n"); return MINUS; }
"*"          { printf("* operator\n"); return MULT; }
"/"          { printf("/ operator\n"); return DIV; }
"("          { printf("( operator\n"); return '('; }
")"          { printf(") operator\n"); return ')'; }
"="          { printf("= operator\n"); return EQ; }
";"          { printf("; operator\n"); return ';'; }
">="         { printf(">= operator\n"); return GE; }
"<="         { printf("<= operator\n"); return LE; }
"=="         { printf("== operator\n"); return EQ; }
"while"      { printf("while keyword\n"); return WHILE; }
"if"         { printf("if keyword\n"); return IF; }
"else"       { printf("else keyword\n"); return ELSE; }
"print"      { printf("print keyword\n"); return PRINT; }

. {
    printf("Unknown character: %s\n", yytext);
    return ERROR;
}

<<EOF>> {
    return 0;
}

%%
