%{

#include <stdio.h>

FILE *svgo;

%}

%%

\$paramod.*int2float.*<   { fprintf(svgo, "int2float<"  , yytext); }
\$paramod.*float2int.*<   { fprintf(svgo, "float2int<"  , yytext); }
\$paramod.*float2index.*< { fprintf(svgo, "float2index<", yytext); }
\$paramod.*proc_fl.*<     { fprintf(svgo, "proc_fl<"    , yytext); }
\$paramod.*addr_dec.*<    { fprintf(svgo, "addr_dec<"   , yytext); }
\$paramod.*stack.*<       { fprintf(svgo, "stack<"      , yytext); }
\$paramod.*rel_addr.*<    { fprintf(svgo, "rel_addr<"   , yytext); }
\$paramod.*instr_dec.*<   { fprintf(svgo, "instr_dec<"  , yytext); }
\$paramod.*pc.*<          { fprintf(svgo, "pc<"         , yytext); }
\$paramod.*prefetch.*<    { fprintf(svgo, "prefetch<"   , yytext); }
\$paramod.*ula.*<         { fprintf(svgo, "ula<"        , yytext); }

.|\n  { fprintf(svgo, "%s", yytext); }

%%

int main(int argc, char *argv[])
{
	yyin = fopen(argv[1], "r");
    svgo = fopen(argv[2], "w");
    yylex();
	fclose(yyin);
    fclose(svgo);

    return 0;
}

int yywrap(void) {return 1;}
