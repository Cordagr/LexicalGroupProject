%{
#include <stdio.h>
#include <stdlib.h>
#include "calcy.tab.h"

union {
    int iValue;
    int sIndex;
} yylval;

void yyerror(const char *s) {
    fprintf(stderr, "%s\n", s);
}
%}

%% 

[a-z] {
    yylval.sIndex = *yytext - 'a'; 
    printf("%s,variable\n", yytext); 
    return VARIABLE; 
}

[0-9]+ {
    yylval.iValue = atoi(yytext); 
    printf("%s,integer\n", yytext); 
    return INTEGER;
}

[ \t\n]+ {}

[+*/()=;] { 
    printf("%c,operator\n", *yytext); 
    return OPERATOR;
}

">=" { 
    printf(">=,operator\n"); 
    return GE; 
}

"<=" { 
    printf("<=,operator\n"); 
    return LE; 
}

"==" { 
    printf("==,operator\n"); 
    return EQ; 
}

"while" { 
    printf("while,keyword\n"); 
    return WHILE; 
}

"if" { 
    printf("if,keyword\n"); 
    return IF; 
}

"else" { 
    printf("else,keyword\n"); 
    return ELSE; 
}

"print" { 
    printf("print,keyword\n"); 
    return PRINT; 
}

. { 
    yyerror("Unknown character");
}

%% 

int yywrap(void) {
    return 1;
}

int main(int argc, char *argv[]) {
     printf("Enter input: ");
     while (yylex()) {}
    return 0;
}
