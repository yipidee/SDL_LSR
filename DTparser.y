%{
#include <stdio.h>
#include "AI.h"

int yylex();
int yyparse();
void yyerror(const char* s);
FILE* yyin;
extern int line_num;
%}

%union {
    int ival;
    char* sval;
}

%type <ival> numnodes
%type <sval> nodetype

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
    numnodes {printf("Creating decision tree of %i nodes\n", $1);} nodedefinitions
    ;

numnodes:
    NUMNODES '=' INT {$$=$3;}
    ;

nodedefinitions:
    nodedefinitions nodedefinition
    | nodedefinition
    ;

nodedefinition:
    nodetype INT ':' {printf("Defining %s node %i\n" $1, $2);} assignments
    ;

nodetype:
    BRANCH {$$="branch";}
    | LEAF {$$="leaf";}
    ;

assignments:
    assignments assignment
    | assignment
    ;

assignment:
    FUNC '=' STRING
    | YES '=' INT
    | NO '=' INT
    ;

footer:
    END
    ;

%%

int main(int n, char** args) {
	// open a file handle to a particular file:
	FILE *myfile = fopen("Assets/decision_trees/_default_.dt", "r");
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

}

void yyerror(const char *s) {
    printf("Some godforsaken error has occured on line %i", line_num);
	// might as well halt now:
	exit(-1);
}
