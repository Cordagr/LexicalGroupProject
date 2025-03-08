#ifndef CALCY_TAB_H
#define CALCY_TAB_H

typedef enum { typeCon, typeId, typeOpr } nodeEnum;

/* Constants */
typedef struct {
    nodeEnum type; /* type of node */
    int value;     /* value of constant */
} conNodeType;

/* Identifiers */
typedef struct {
    nodeEnum type; /* type of node */
    int i;         /* subscript to ident array */
} idNodeType;

/* Operators */
typedef struct {
    nodeEnum type; /* type of node */
    int oper;      /* operator */
    int nops;      /* number of operands */
    union nodeTypeTag *op[1]; /* operands (expandable) */
} oprNodeType;

typedef union nodeTypeTag {
    nodeEnum type;   /* type of node */
    conNodeType con; /* constants */
    idNodeType id;   /* identifiers */
    oprNodeType opr; /* operators */
} nodeType;




extern int sym[26];  // Symbol table for variable names

/* Token definitions */
#define INTEGER 1
#define VARIABLE 2
#define GE 3
#define LE 4
#define EQ 5
#define WHILE 6
#define IF 7
#define ELSE 8
#define PRINT 9
#define OPERATOR 10

#endif /* CALCY_TAB_H */
