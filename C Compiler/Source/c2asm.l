%{
#include <string.h>
#include "y.tab.h"
#include "variaveis.h"
#include "t2t.h"

void yyerror(char *);

%}

DIGITO [0-9]
LETRA  [A-Za-z_]

%%

"#PRNAME"                       return PRNAME;
"#DIRNAM"                       return DIRNAM;
"#DATYPE"                       return DATYPE;
"#NUBITS"                       return NUBITS;
"#NBMANT"                       return NBMANT;
"#NBEXPO"                       return NBEXPO;
"#NDSTAC"                       return NDSTAC;
"#SDEPTH"                       return SDEPTH;
"#NUIOIN"                       return NUIOIN;
"#NUIOOU"                       return NUIOOU;
"#NUGAIN"                       return NUGAIN;

"float"  {fgen = 1; type_tmp = 2; yylval = 2; return TYPE;}
"int"              {type_tmp = 1; yylval = 1; return TYPE;}
"void"             {              yylval = 0; return TYPE;}

"in"                            return IN;
"out"                           return OUT;
"abs"                           return ABS;
"sign"                          return SIGN;
"return"                        return RETURN;
"while"                         return WHILE;
"if"                            return IF;
"else"                          return ELSE;

\"(\\.|[^\\"])*\"	       {
                              if (find_var(yytext) == -1) add_var(yytext);
                              yylval = find_var(yytext);
                              return STRING;
                           }

{LETRA}({LETRA}|{DIGITO})* {
                              char var_name[64];
                              if (find_var(yytext) == -1)
                              {
                                  strcpy(var_name, fname);
                                  strcat(var_name, yytext);
                                  if (find_var(var_name) == -1) add_var(var_name);
                              }
                              else
                              {
                                  strcpy(var_name, yytext);
                              }
                              yylval = find_var(var_name);
                              return ID;
                           }

[0-9]+                     {
                              if (find_var(yytext) == -1) add_var(yytext);
                              yylval = find_var(yytext);
                              return INUM;
                           }

[0-9]*\.?[0-9]+            {
                              if (find_var(yytext) == -1) add_var(yytext);
                              yylval = find_var(yytext);
                              fgen = 1;
                              return FNUM;
                           }

[-+*()=;,\]\[{}<>~!%/&^|@?]     return *yytext;

"<<"                          return  SHIFTL;
">>"                          return  SHIFTR;
">>>"                         return SSHIFTR;
">="                          return   GREQU;
"<="                          return   LESEQ;
"=="                          return     EQU;
"!="                          return     DIF;
"&&"                          return    LAND;
"||"                          return     LOR;
"/>"                          return    NORM;

\n                            line_num++;

[ \t]+                           ;       /* ignore whitespace   */
"/*"((\*+[^/*])|([^*]))*\**"*/"  ;       /* comentario em bloco */
"//"[^\n]*                       ;       /* comentario em linha */

.                             {yyerror("");}
%%

int yywrap(void) {return 1;}
