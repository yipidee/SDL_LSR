%option noyywrap

%{
#include <string.h>
#include <math.h>
#include "DTparser.tab.h"

int line_num = 1;
%}

%x COMMENT

%%

[ \t]           ;
\/\/*		{BEGIN(COMMENT);}
<COMMENT>\n	{++line_num; BEGIN(INITIAL);}
<COMMENT>.	;
@author         {return AUTHOR;}
@datecreated    {return CREATEDATE;}
@version        {return VERSION;}
numnodes        {return NUMNODES;}
func            {return FUNC;}
yes             {return YES;}
no              {return NO;}
[0-9]+          {yylval.ival = atoi(yytext); return INT;}
[a-zA-Z0-9]+    {yylval.sval = strdup(yytext);return STRING;}
\n		{++line_num;}
=               {return '=';}
:               {return ':';}
\"              {return '\"';}
&		{return '&';}
#		{return '#';}
.               ;
%%
