%{
#include <stdio.h>
#include <stdlib.h>
#include "AI.h"

extern int yylex();
extern int yyparse();

void yyerror(const char* s);

FILE* yyin;
extern int line_num;

DecisionTree dt = NULL;
int node_count = 0;
int tree_size;
int curr_yes;
int curr_no;
char* curr_func;
char curr_type;
%}

%union {
    int ival;
    char* sval;
}

%type <ival> numnodes

%token AUTHOR CREATEDATE VERSION
%token NUMNODES BRANCH LEAF FUNC YES NO END
%token <ival> INT
%token <sval> STRING

%%

decisiontreefile:
    header body footer{
			if(node_count > tree_size)yyerror("More nodes defined than tree contains.");
			if(node_count < tree_size)yyerror("Less nodes defined than tree contains.");
			printf("Decision Tree successfully parsed!\n");
            node_count = 0;
			}
    ;

header:
    propertylines
    ;

propertylines:
    propertylines propertyline
    | propertyline
    ;

propertyline:
    AUTHOR '=' '"' STRING '"' {printf("Parsing decision tree authored by %s\n", $4);}
    | CREATEDATE '=' '"' STRING '"' {printf("Decision tree created %s\n", $4);}
    | VERSION '=' '"' INT '"' {printf("Version %i of this decision tree\n", $4);}
    ;

body:
    numnodes {printf("Creating decision tree of %i nodes\n", tree_size);} nodedefinitions
    ;

numnodes:
    NUMNODES '=' INT {tree_size=$3; dt = malloc(NodeSize * $3);}
    ;

nodedefinitions:
    nodedefinitions nodedefinition
    | nodedefinition
    ;

nodedefinition:
    '&' INT {curr_type = 'b';} ':' assignments	{AI_makeBranchNode(dt, $2, curr_func, curr_yes, curr_no);printf("Created a branch node, id %i \n",$2);++node_count;}
    | '#' INT {curr_type = 'l';} ':' assignment	{AI_makeLeafNode(dt, $2, curr_func);printf("Created a leaf node, id %i \n",$2);++node_count;}
    ;

assignments:
    assignments assignment
    | assignment
    ;

assignment:
    FUNC '=' STRING	{curr_func = $3;}
    | YES '=' INT	{
			if(curr_type=='l')yyerror("addressing child from leaf node.");
			curr_yes = $3;
			}
    | NO '=' INT	{
			if(curr_type=='l')yyerror("addressing child from leaf node.");
			curr_no = $3;
			}
    ;

footer:
    END

%%

DecisionTree AI_parseDecisionTree(char* DTfile) {
	// open a file handle to a particular file:
	FILE *myfile = fopen(DTfile, "r");
	// make sure it's valid:
	if (!myfile) {
        printf("Can't open decision tree");
		return NULL;
	}
	// set flex to read from it instead of defaulting to STDIN:
	yyin = myfile;

	// parse through the input until there is no more:
	do {
		yyparse();
	} while (!feof(yyin));

    DecisionTree myDT = dt;
	dt = NULL;
    return myDT;
}

void yyerror(const char *s) {
    printf("Error on line %i: %s\n", line_num, s);
	// might as well halt now:
	exit(-1);
}
