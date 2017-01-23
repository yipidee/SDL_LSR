%{
#include <string.h>
#include "AI.h"
#include "DTparser.tab.h"
%}

%%

[ \t\n]           ;
@author         {return AUTHOR;}
@datecreated    {return CREATEDATE;}
@version        {return VERSION;}
numnodes        {return NUMNODES;}
branchnode      {return BRANCH;}
leafnode        {return LEAF;}
func            {return FUNC;}
yes             {return YES;}
no              {return NO;}
end             {return END;}
[0-9]+            { yylval.ival = atoi(yytext); return INT; }
[a-zA-Z0-9]+      {
	// we have to copy because we can't rely on yytext not changing underneath us:
	yylval.sval = strdup(yytext);
	return STRING;
}
=               {return '=';}
:               {return ':';}
\"               {return '\"';}
.                ;
%%
