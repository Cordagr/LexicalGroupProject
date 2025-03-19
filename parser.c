#include <stdio.h>
#include <stdlib.h>
#include "calcy.tab.h"
#include <ctype.h>
extern int yylex();


nodeType *createNewNumNode(int value);
nodeType *createNewOprNode(int oper, nodeType *leftOp, nodeType *rightOp);
nodeType *expression();
nodeType *term();
nodeType *factor();
nodeType *integer();
extern FILE *yyin; 
int token;

// nodes for integer tokens//
nodeType *createNewNumNode(int value)
{
    nodeType *newNode = (nodeType *)malloc(sizeof(nodeType));
    newNode->type = typeCon;
    newNode->value = value;
    return newNode;
}
// nodes for operator tokens // 
nodeType *createNewOprNode(int oper, nodeType *leftOp, nodeType *rightOp) {
    printf("Entered createNewOprNode()\n");
    

    nodeType *newNode = (nodeType *)malloc(sizeof(nodeType));
    if (!newNode) {
        printf("Memory allocation failed for operator node.\n");
        exit(1);
    }
    
    printf("Memory allocated for newNode\n");
    

    newNode->type = typeOpr;
    
   
    
    newNode->opr.oper = oper;    
    newNode->opr.nops = 2;    
    
 
    
    // Assign operands
    newNode->opr.op[0] = leftOp;
    newNode->opr.op[1] = rightOp;
    
   
    
    return newNode;
}

nodeType *expression() {

    
    nodeType *leftOperand = term();

    while (token == PLUS || token == MINUS) {
        int oper = token;
        token = yylex();  
        nodeType *rightOperand = term();  

        
        leftOperand = createNewOprNode(oper, leftOperand, rightOperand);
    }

    return leftOperand; 
}

nodeType *term() {
    
    
    nodeType *leftOperand = factor();  

    while (token == MULT || token == DIV) {
        int oper = token;
        token = yylex();  
        nodeType *rightOperand = factor();  

        leftOperand = createNewOprNode(oper, leftOperand, rightOperand);
    }

    return leftOperand;
}

nodeType *factor() {
    printf("Entering factor(), token: %d\n", token);
    
    nodeType *newNode;
    
    if (token == LPAREN) {  
        token = yylex();
        newNode = expression(); 
        if (token == RPAREN) {
            token = yylex();
        } else {
            printf("Syntax error: Expected ')'\n");
            exit(1);
        }
    }
    else if (token == INTEGER) { 
        newNode = integer(); 
    }
    else {
        printf("Syntax error: Unexpected token in factor: %d\n", token);
        exit(1);
    }

    return newNode;
}


nodeType *integer() {
    int value = yylval.iValue; 
    nodeType *newNode = createNewNumNode(value);     
    token = yylex();  
 
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
        
        printf("%c (%s)\n", 
               (node->opr.oper == PLUS)  ? '+' :
               (node->opr.oper == MINUS) ? '-' :
               (node->opr.oper == MULT)  ? '*' :
               (node->opr.oper == DIV)   ? '/' : '?',
               (node->opr.oper == PLUS)  ? "Add" :
               (node->opr.oper == MINUS) ? "Minus" :
               (node->opr.oper == MULT)  ? "Multiply" :
               (node->opr.oper == DIV)   ? "Divide" : "Unknown");

    
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

      if (token != 0 && token != '\n') {  
        printf("Warning: Unexpected tokens after expression: %d\n", token);
    }


    printf("\nAbstract Syntax Tree:\n");
    printAST(ast, 0);
    printf("AST finished printing");

    return 0;
}

