#ifndef CALCY_TAB_H
#define CALCY_TAB_H

/* Token definitions */
#define PLUS  1
#define MINUS 2
#define MULT  3
#define DIV   4
#define INTEGER 5
#define LPAREN 6
#define RPAREN 7
#define ELSE 8
#define PRINT 9
#define ERROR 10
#define LE 11
#define WHILE 12
#define IF 13
#define VARIABLE 14
#define EQ 15
#define GE 16
#define SEMI 17

typedef enum { typeCon, typeOpr } nodeEnum;

typedef struct nodeTypeTag {
    nodeEnum type; /* type of node */
    union {
        int value; /* for constant numbers */
        struct {  /* for operator nodes */
            int oper;     /* operator */
            int nops;     /* number of operands */
            struct nodeTypeTag *op[2];  /* operands as a fixed array */
        } opr;
    };
} nodeType;

/* External variable declaration for yylval */
union {
    int iValue;  // Integer value for constants
    int sIndex;  // Not used in current code
} yylval;

#endif /* CALCY_TAB_H */
