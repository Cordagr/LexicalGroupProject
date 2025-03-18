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
extern FILE *yyin; // The file input
int token;


nodeType *createNewNumNode(int value)
{
    nodeType *newNode = (nodeType *)malloc(sizeof(nodeType));
    newNode->type = typeCon;
    newNode->value = value;  
    return newNode;
}

nodeType *createNewOprNode(int oper, nodeType *leftOp, nodeType *rightOp) {
    printf("Entered createNewOprNode()\n");
    
   
    nodeType *newNode = (nodeType *)malloc(sizeof(nodeType));
    if (!newNode) {
        printf("Memory allocation failed for operator node.\n");
        exit(1);
    }
    
    printf("Memory allocated for newNode\n");
    
   
    newNode->type = typeOpr;
    
    printf("Set type to typeOpr\n");
    
    newNode->opr.oper = oper; 
    
    printf("Set oper to %d\n", oper);
    
    newNode->opr.nops = 2;    
    
    printf("Set nops to 2\n");
    

    newNode->opr.op[0] = leftOp;
    newNode->opr.op[1] = rightOp;
    
    printf("Assigned operands\n");
    printf("Exiting createNewOprNode()\n");
    
    return newNode;
}


nodeType *expression() {
    printf("Starting expression()\n");
    nodeType *newNode = term(); 
    printf("After term() in expression(), token = %d\n", token);

 
    while (token == PLUS || token == MINUS) {
        printf("Found operator: %d\n", token); 
        int oper = token; 
        printf("About to call yylex()\n");
        token = yylex(); 
        printf("After yylex(), token = %d\n", token);
        
        printf("About to call term()\n");
        nodeType *secondNode = term();
        printf("After term() call, secondNode = %p\n", (void*)secondNode);
        
        if (secondNode == NULL) {
            printf("Failed to parse second term.\n");
            exit(1);
        }
        
        printf("About to call createNewOprNode(%d, %p, %p)\n", 
               oper, (void*)newNode, (void*)secondNode);
        newNode = createNewOprNode(oper, newNode, secondNode); 
        printf("After createNewOprNode(), newNode = %p\n", (void*)newNode);
    }
    
    printf("Exiting expression()\n");
    return newNode;
}

nodeType *term() {
    printf("Token at term: %d\n", token);
    nodeType *newNode = factor(); 


    while (token == MULT || token == DIV) {
        int oper = token;
        token = yylex(); // Get next token
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
    else if (token == INTEGER) { // If token is an integer, create a number node
        printf("Found INTEGER token: %d\n", yylval.iValue);
        newNode = integer(); // Create number node
    }
    else {
        printf("Error: Invalid token in factor: %d\n", token);
        exit(1); // Error if invalid token
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

     
      if (token != 0 && token != '\n') {  // 0 or '\n' usually indicates end of input
        printf("Warning: Unexpected tokens after expression: %d\n", token);
    }


    printf("\nAbstract Syntax Tree:\n");
    printAST(ast, 0);
    printf("AST finished printing");

    return 0;
}
