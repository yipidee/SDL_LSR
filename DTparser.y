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
int curr_node;
int curr_yes;
int curr_no;
char* curr_func;
%}

%union {
    int ival;
    char* sval;
}

%type <ival> numnodes

%token AUTHOR CREATEDATE VERSION
%token NUMNODES BRANCH LEAF FUNC YES NO
%token END
%token <ival> INT
%token <sval> STRING

%%

decisiontreefile:
    header body footer {printf("Decision Tree successfully parsed!\n");}
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
    numnodes {printf("Creating decision tree of %i nodes\n", $1);
	      dt = malloc(NodeSize * $1);} nodedefinitions
    ;

numnodes:
    NUMNODES '=' INT {$$=$3;}
    ;

nodedefinitions:
    nodedefinitions nodedefinition
    | nodedefinition
    ;

nodedefinition:
    '&' INT ':' assignments	{AI_makeBranchNode(dt, $2, curr_func, curr_yes, curr_no);printf("Created a branch node, id %i \n",$2);}
    | '#' INT ':' assignment	{AI_makeLeafNode(dt, $2, curr_func);printf("Created a leaf node, id %i \n",$2);}
    ;

assignments:
    assignments assignment
    | assignment
    ;

assignment:
    FUNC '=' STRING	{curr_func = $3;}
    | YES '=' INT	{curr_yes = $3;}
    | NO '=' INT	{curr_no = $3;}
    ;

footer:
    END
    ;

%%

DecisionTree AI_parseDecisionTree(char* DTfile) {
	// open a file handle to a particular file:
	FILE *myfile = fopen(DTfile, "r");
	// make sure it's valid:
	if (!myfile) {
        printf("Can't open decision tree");
		return -1;
	}
	// set flex to read from it instead of defaulting to STDIN:
	yyin = myfile;

	// parse through the input until there is no more:
	do {
		yyparse();
	} while (!feof(yyin));

	return dt;
}

void yyerror(const char *s) {
    printf("Some godforsaken error has occured on line %i\n", line_num);
	// might as well halt now:
	exit(-1);
}
