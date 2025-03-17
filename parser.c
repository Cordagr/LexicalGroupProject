#include <stdio.h>
#include <stdlib.h>
#include "calcy.tab.h"

nodeType *createNewNumNode(int value);
nodeType *createNewOprNode(int oper, nodeType *leftOp, nodeType *rightOp);
nodeType *expression();
nodeType *term();
nodeType *factor();
nodeType *integer();
extern int yylval, yylex(); //(from lexical analyzer) yylval stores value of token and yylex() gets next token
extern FILE *yyin; //The file input
int token;

//Creates a new number node, initializes the constant value
nodeType *createNewNumNode(int value)
{
	nodeType *newNode = (nodeType *)malloc(sizeof(nodeType));
	newNode->type=typeCon;
	newNode->con.value=value;
	return newNode;
}

//Creates a new operator node, initializes the left and right operands
nodeType *createNewOprNode(int oper, nodeType *leftOp, nodeType *rightOp)
{
	nodeType *newNode = (nodeType *)malloc(sizeof(nodeType));
	newNode->type=typeOpr;
	newNode->opr.oper=oper;
	newNode->opr.nops=2;
	newNode->opr.op[0]=leftOp;
	newNode->opr.op[1]=rightOp;
	return newNode;
}

//This function corresponds to the grammar rules <expression> and <expression_suffix>
nodeType *expression()
{
	nodeType *newNode=term(); //store parsed term
	
	//As long as the token is + or -, get next token and make a node
	while(token=='+' || token=='-')
	{
		int oper=token;
		token=yylex();
		nodeType *rightTerm = term();
		if (!rightTerm)
		{
			printf("Syntax error expected right term \n");
			exit(1);
		}
		newNode=createNewOprNode(oper, newNode, rightTerm);
	}
	
	return newNode;
}

//This function corresponds to the grammar rules <term> and <term_suffix>
nodeType *term()
{
	nodeType *newNode=factor();

	//As long as the token is * or /, get next token and make a node
	while(token=='*' || token=='/')
	{
		int oper=token;
		token=yylex();
		newNode=createNewOprNode(oper, newNode,factor());
	}
	return newNode;
}

nodeType *factor()
{
    nodeType * newNode;
    if(token == '(')
    {
        token = yylex();
        newNode = expression(); // comsume expression between parenthesis
        if (token == ')')
        {
            token = yylex(); // consume )
        }
        else
        {
            printf("Error: Missing parenthesis"); // no closing parenthesis matching
            exit(1);
        }
    }
    else if (token == INTEGER)
    {
        newNode = integer(); // create integer with lexeme value
    }
    else
    {
        printf("Error: Invalid tokens in factor"); // no valid tokens for factor
        exit(1);
    }
	return newNode;
}

nodeType *integer()
{
  if(token == INTEGER)
  {
	int value = yylval; // to integer
	token = yylex();
	return createNewNumNode(value);
  }
  else
  {
	printf("Invalid tokens in integer call");
	exit(1);
  }
}


void printAST(nodeType * node, int depth)
{
if (!node)
return;

// indentation //
for (int i=0;i<depth;i++)
{
	printf("    ");
}
	
if (node -> type == typeCon)
{
	printf("%d(int)\n", node -> con.value);
}
else if (node -> type == typeOpr)
{
	printf("%c(operator)\n", node -> opr.oper);

	// left opr recursive call //
	printAST(node->opr.op[0], depth + 1);
	// right opr recursive call //
	printAST(node->opr.op[1], depth + 1);		
}
}

int main(int argc, char *argv[])
{ 
// TODO: call lexical reading from input and get tokens from lexical program and then call 
token = yylex();
nodeType *ast = expression();
printAST(ast,0);
return 0;
}

