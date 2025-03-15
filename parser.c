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
nodeType *createNewNumNode(int type)
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
		newNode=createNewOprNode(oper, newNode, term());
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

}

nodeType *factor()
{
}

nodeType *integer()
{
}

void abstractSyntaxTree()
{
}

int main(int argc, char *argv[])
{
}
