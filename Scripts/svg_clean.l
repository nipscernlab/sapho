%{

#include <stdio.h>

FILE *svgo;

%}

%%

\$paramod.*ula_out.*<     { fprintf(svgo, "ula_out<"    , yytext); }
\$paramod.*neg.*<         { fprintf(svgo, "neg<"        , yytext); }
\$paramod.*ula_fx_mux.*<  { fprintf(svgo, "ula_fx_mux<" , yytext); }

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
