%{

#include <stdio.h>

FILE *svgo;

%}

%%

\$paramod.*proc_fx.*<     { fprintf(svgo, "proc_fx<"    , yytext); }
\$paramod.*core_fx.*<     { fprintf(svgo, "core_fx<"    , yytext); }
\$paramod.*addr_dec.*<    { fprintf(svgo, "addr_dec<"   , yytext); }
\$paramod.*stack.*<       { fprintf(svgo, "stack<"      , yytext); }
\$paramod.*rel_addr.*<    { fprintf(svgo, "rel_addr<"   , yytext); }
\$paramod.*instr_dec.*<   { fprintf(svgo, "instr_dec<"  , yytext); }
\$paramod.*pc.*<          { fprintf(svgo, "pc<"         , yytext); }
\$paramod.*prefetch.*<    { fprintf(svgo, "prefetch<"   , yytext); }
\$paramod.*mem_instr.*<   { fprintf(svgo, "mem_instr<"  , yytext); }
\$paramod.*mem_data.*<    { fprintf(svgo, "mem_data<"   , yytext); }
\$paramod.*ula_fx.*<      { fprintf(svgo, "ula_fx<"     , yytext); }
\$paramod.*f2ima.*<       { fprintf(svgo, "f2ima<"      , yytext); }

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
