#include <stdio.h>
#include <stdlib.h>
#include "calcy.tab.h"
#include <ctype.h>
#include "lex.yy.c"

nodeType *createNewNumNode(int value);
nodeType *createNewOprNode(int oper, nodeType *leftOp, nodeType *rightOp);
nodeType *expression();
nodeType *term();
nodeType *factor();
nodeType *integer();
extern FILE *yyin; 
int token;

nodeType *createNewNumNode(int value)
{
    nodeType *newNode = (nodeType *)malloc(sizeof(nodeType));
    newNode->type = typeCon;
    newNode->value = value;  
    return newNode;
}

nodeType *createNewOprNode(int oper, nodeType *leftOp, nodeType *rightOp) {
    nodeType *newNode = (nodeType *)malloc(sizeof(nodeType));
    if (!newNode) {
        printf("Memory allocation failed for operator node.\n");
        exit(1);
    }

    if (leftOp != NULL) {
        printf("Left operand value: %d\n", leftOp->value);
    } else {
        printf("Left operand is NULL\n");
    }

    if (rightOp != NULL) {
        printf("Right operand value: %d\n", rightOp->value);
    } else {
        printf("Right operand is NULL\n");
    }

    newNode->type = typeOpr;
    newNode->opr.oper = oper; 
    newNode->opr.nops = 2;    

    newNode->opr.op[0] = leftOp;
    newNode->opr.op[1] = rightOp;

    return newNode;
}

nodeType *expression() {
    nodeType *newNode = term();

    while (token == PLUS || token == MINUS) {
        printf("Found operator: %d\n", token); 
        int oper = token;  
        token = yylex();   
        nodeType *secondNode = term();
        if (secondNode == NULL) {
            printf("Failed to parse second term.\n");
            exit(1);
        }
        newNode = createNewOprNode(oper, newNode, secondNode);  
        printf("New node value: %d\n", newNode->opr.op[0]->value); 
    }
    return newNode;
}

nodeType *term() {
    printf("Token at term: %d\n", token);
    nodeType *newNode = factor();

    while (token == MULT || token == DIV) {
        int oper = token;
        token = yylex();
        newNode = createNewOprNode(oper, newNode, factor());
    }
    return newNode;
}

nodeType *factor() {
    printf("Entering factor()\n");
    printf("Current token in factor: %d\n", token);

    nodeType *newNode;
    if (token == '(') {  
        token = yylex(); 
        newNode = expression(); 
        if (token == ')') {
            token = yylex(); 
        } else {
            printf("Syntax error: Expected ')'\n");
            exit(1); 
        }
    }
    else if (token == INTEGER) { 
        printf("Found INTEGER token: %d\n", yylval.iValue);
        newNode = integer(); 
    }
    else {
        printf("Error: Invalid token in factor: %d\n", token);
        exit(1); 
    }

    return newNode;
}

nodeType *integer() {
    int value = yylval.iValue;  
    nodeType *newNode = createNewNumNode(value);  
    printf("Created new node with value: %d\n", newNode->value);
    printf("Current token: %d\n", token);
    token = yylex();  
    printf("Next token: %d\n", token);
    return newNode;
}

void printAST(nodeType *node, int depth)
{
    if (!node)
        return;

    for (int i = 0; i < depth; i++)
        printf("    ");

    if (node->type == typeCon)
    {
        printf("%d (int)\n", node->value); 
    }
    else if (node->type == typeOpr)
    {
        printf("%s (operator)\n", 
               (node->opr.oper == PLUS) ? "+" : (node->opr.oper == MINUS) ? "-"
                                         : (node->opr.oper == MULT)   ? "*"
                                         : (node->opr.oper == DIV)   ? "/"
                                         : "?");

        if (node->opr.op[0]) 
            printAST(node->opr.op[0], depth + 1);
        if (node->opr.op[1])
            printAST(node->opr.op[1], depth + 1);
    }
}

int main()
{
    printf("Starting parser...\n");
    printf("Enter expression: ");

    yyin = stdin; 

    token = yylex();

    nodeType *ast = expression();

    printf("\nAbstract Syntax Tree:\n");
    printAST(ast, 0);

    return 0;
}
