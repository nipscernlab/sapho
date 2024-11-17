%{

#include "y.tab.h"
#include "variaveis.h"
#include "t2t.h"
#include "data_declar.h"
#include "data_use.h"
#include "diretivas.h"

void yyerror(char *);

%}

DIGITO [0-9]
LETRA  [A-Za-z_]
WS     [ \t]+

VARIA  {LETRA}({LETRA}|{DIGITO})*
INNUM  {DIGITO}+
FLNUM  (0|[1-9]+{DIGITO}*)\.?{DIGITO}*
CONUM  [-]*{WS}*{FLNUM}{WS}*[+|-]{WS}*{FLNUM}{WS}*[i]

%%

"#PRNAME"                     return PRNAME;
"#DIRNAM"                     return DIRNAM;
"#DATYPE"                     return DATYPE;
"#NUBITS"                     return NUBITS;
"#NBMANT"                     return NBMANT;
"#NBEXPO"                     return NBEXPO;
"#NDSTAC"                     return NDSTAC;
"#SDEPTH"                     return SDEPTH;
"#NUIOIN"                     return NUIOIN;
"#NUIOOU"                     return NUIOOU;
"#NUGAIN"                     return NUGAIN;
"#USEMAC"                     return USEMAC;
"#ENDMAC"                     return ENDMAC;
"#FFTSIZ"                     return FFTSIZ;
"#INTERPOINT"                 return ITRADD;

"in"                          return IN;
"out"                         return OUT;
"norm"                        return NRM;
"pset"                        return PST;
"abs"                         return ABS;
"sign"                        return SIGN;
"sqrt"                        return SQRT;

"while"                       return WHILE;
"break"                       return BREAK;
"if"                          return IF;
"else"                        return ELSE;
"switch"                      return SWITCH;
"case"                        return CASE;
"default"                     return DEFAULT;
"return"                      return RETURN;

"<<"                          return  SHIFTL;
">>"                          return  SHIFTR;
">>>"                         return SSHIFTR;

">="                          return GREQU;
"<="                          return LESEQ;
"=="                          return EQU;
"!="                          return DIF;

"&&"                          return LAND;
"||"                          return LOR;

"/>"                          return NORM;
"=-"                          return EQNE;
"++"                          return PPLUS;

"comp"   {fgen = 1; type_tmp = 3; yylval = 3; return TYPE;}
"float"  {fgen = 1; type_tmp = 2; yylval = 2; return TYPE;}
"int"    {          type_tmp = 1; yylval = 1; return TYPE;}
"void"   {                        yylval = 0; return TYPE;}

{VARIA}           {
                     yylval = exec_id(yytext);
                     return ID;
                  }

{INNUM}           {
                     yylval = exec_num(yytext);
                     return INUM;
                  }

{FLNUM}           {  //[0-9]*\.?[0-9]+
                     yylval = exec_num(yytext);
                     fgen = 1;
                     return FNUM;
                  }

{CONUM}           {
                     yylval = exec_cnum(yytext);
                     fgen = 1;
                     return CNUM;
                  }

\"(\\.|[^\\"])*\" {
                     if (find_var(yytext) == -1) add_var(yytext);
                     yylval = find_var(yytext);
                     return STRING;
                  }

[-+*()=$;:,\]\[{}<>~!%/&^|@?] {return *yytext;}

\n                   line_num++;

{WS}                             ;       /* ignore whitespace   */
"/*"((\*+[^/*])|([^*]))*\**"*/"  ;       /* comentario em bloco */
"//"[^\n]*                       ;       /* comentario em linha */

.                 {yyerror("Erro do Lexer!\n");}
%%

int yywrap(void) {return 1;}
