%{

#include "y.tab.h"
#include "..\Headers\variaveis.h"
#include "..\Headers\t2t.h"
#include "..\Headers\data_declar.h"
#include "..\Headers\data_use.h"
#include "..\Headers\diretivas.h"
#include "..\Headers\global.h"

#include <string.h>

void yyerror(char *);
int  conta_linhas(char *texto);

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
"atan"                        return ATAN;

"real"                        return REAL;
"imag"                        return IMAG;
"fase"                        return FASE;

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

"++"                          return PPLUS;

"comp"   {fgen = 1; type_tmp = 3; yylval.ival = 3; return TYPE;}
"float"  {fgen = 1; type_tmp = 2; yylval.ival = 2; return TYPE;}
"int"    {          type_tmp = 1; yylval.ival = 1; return TYPE;}
"void"   {                        yylval.ival = 0; return TYPE;}

{VARIA}           {
                     yylval.ival = exec_id(yytext);
                     return ID;
                  }

{INNUM}           {
                     yylval.ival = exec_num(yytext);
                     return INUM;
                  }

{FLNUM}           {  //[0-9]*\.?[0-9]+
                     yylval.ival = exec_num(yytext);
                     fgen = 1;
                     return FNUM;
                  }

{CONUM}           {
                     yylval.ival = exec_cnum(yytext);
                     fgen = 1;
                     return CNUM;
                  }

\"(\\.|[^\\"])*\" {
                     if (find_var(yytext) == -1) add_var(yytext);
                     yylval.ival = find_var(yytext);
                     return STRING;
                  }

[-+*()=$;:,\]\[{}<>~!%/&^|@?] {return *yytext;}

\n                   line_num++;


"/*"((\*+[^/*])|([^*]))*\**"*/"  {line_num += conta_linhas(yytext);} 

{WS}                             ;  /* ignore whitespace   */
"//"[^\n]*                       ;  /* comentario em linha */

. {if (yytext[0] != '\r') yyerror("");}
%%

int yywrap(void) {return 1;}

int conta_linhas(char *texto)
{
   int cnt =0;
   for(int i=0;i<strlen(texto);i++)
   {
      if (texto[i] == '\n') cnt++;
   }

   return cnt;
}
