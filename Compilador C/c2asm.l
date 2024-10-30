%{

#include "y.tab.h"
#include "variaveis.h"
#include "t2t.h"
#include "data_declar.h"
#include "data_use.h"
#include "diretivas.h"

void yyerror(char *);

%}

DIGITO   [0-9]
LETRA    [A-Za-z_]
WS       [ \t]+

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
"#USEMAC"                       return USEMAC;
"#ENDMAC"                       return ENDMAC;
"#FFTSIZ"                       return FFTSIZ;
"#INTERPOINT"                   return ITRADD;

"float"  {fgen = 1; type_tmp = 2; yylval = 2; return TYPE;}
"int"    {          type_tmp = 1; yylval = 1; return TYPE;}
"void"   {                        yylval = 0; return TYPE;}

"in"                            return IN;
"out"                           return OUT;
"norm"                          return NRM;
"pset"                          return PST;
"abs"                           return ABS;
"sign"                          return SIGN;

"while"                         return WHILE;
"break"                         return BREAK;
"if"                            return IF;
"else"                          return ELSE;
"return"                        return RETURN;

\"(\\.|[^\\"])*\"	       {
                              if (find_var(yytext) == -1) add_var(yytext);
                              yylval = find_var(yytext);
                              return STRING;
                           }

{LETRA}({LETRA}|{DIGITO})* {
                              yylval = exec_id(yytext);
                              return ID;
                           }

[0-9]+                     {
                              yylval = exec_num(yytext);
                              return INUM;
                           }

[0-9]*\.?[0-9]+            {

                              yylval = exec_num(yytext);
                              fgen = 1;
                              return FNUM;
                           }

[-+*()=$;,\]\[{}<>~!%/&^|@?]  return *yytext;

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
"=-"                          return    EQNE;
"++"                          return   PPLUS;

\n                            line_num++;

{WS}                             ;       /* ignore whitespace   */
"/*"((\*+[^/*])|([^*]))*\**"*/"  ;       /* comentario em bloco */
"//"[^\n]*                       ;       /* comentario em linha */

.                             {yyerror("Erro do Lexer!\n");}
%%

int yywrap(void) {return 1;}
