%{

#include <stdio.h>

FILE *svgo;

%}

%%

\$paramod.*ula_fx.*<      { fprintf(svgo, "ula_fx<"     , yytext); }
\$paramod.*f2ima.*<       { fprintf(svgo, "f2ima<"      , yytext); }
\$paramod.*i2fma.*<       { fprintf(svgo, "i2fma<"      , yytext); }
\$paramod.*ula_out.*<     { fprintf(svgo, "ula_out<"    , yytext); }

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
