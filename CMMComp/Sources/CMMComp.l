%{

#include "y.tab.h"

#include "..\Headers\global.h"
#include "..\Headers\variaveis.h"
#include "..\Headers\data_declar.h"

void yyerror     (char *);
int  conta_linhas(char *texto); // para comentarios em bloco

%}

LETRA  [A-Za-z_]
WS     [ \t]+

VARIA  {LETRA}({LETRA}|[0-9])*
INNUM  [0-9]+
FLNUM  (0|[1-9]+[0-9]*)\.?[0-9]*
CONUM  [-]*{WS}*{FLNUM}{WS}*[+|-]{WS}*{FLNUM}{WS}*[i]

STRIN  \"(\\.|[^\\"])*\"
CARES  [-+*()=$;:,\]\[{}<>~!%/&^|@?]
BLOCO  "/*"((\*+[^/*])|([^*]))*\**"*/"
COMEN  "//"[^\n]*

%%

"#PRNAME"     return PRNAME;
"#NUBITS"     return NUBITS;
"#NBMANT"     return NBMANT;
"#NBEXPO"     return NBEXPO;
"#NDSTAC"     return NDSTAC;
"#SDEPTH"     return SDEPTH;
"#NUIOIN"     return NUIOIN;
"#NUIOOU"     return NUIOOU;
"#NUGAIN"     return NUGAIN;
"#USEMAC"     return USEMAC;
"#ENDMAC"     return ENDMAC;
"#FFTSIZ"     return FFTSIZ;
"#PIPELN"     return PIPELN;
"#INTERPOINT" return ITRADD;

"in"          return INN;
"out"         return OUT;
"norm"        return NRM;
"pset"        return PST;
"abs"         return ABS;
"sign"        return SGN;
"sqrt"        return SQRT;
"atan"        return ATAN;
"sin"         return SIN;
"cos"         return COS;

"real"        return REAL;
"imag"        return IMAG;
"fase"        return FASE;
"complex"     return COMP;

"vtv"         return VTV;

"while"       return WHILE;
"break"       return BREAK;
"if"          return IF;
"else"        return ELSE;
"switch"      return SWITCH;
"case"        return CASE;
"default"     return DEFAULT;
"return"      return RET;

"<<"          return  SHIFTL;
">>"          return  SHIFTR;
">>>"         return SSHIFTR;

">="          return GREQU;
"<="          return LESEQ;
"=="          return EQU;
"!="          return DIF;

"&&"          return LAN;
"||"          return LOR;

"++"          return PPLUS;
"|I|"         return EYE;
"|0>"         return VZERO;

"comp"       {type_tmp = 3; yylval.ival = 3; return TYPE;}
"float"      {type_tmp = 2; yylval.ival = 2; return TYPE;}
"int"        {type_tmp = 1; yylval.ival = 1; return TYPE;}
"void"       {              yylval.ival = 0; return TYPE;}
			 
{VARIA}      {
                yylval.ival = exec_id(yytext);
                return ID;
             }
			 
{INNUM}      {
                yylval.ival = exec_num(yytext);
                return INUM;
             }
			 
{FLNUM}      {
                yylval.ival = exec_num(yytext);
                return FNUM;
             }
			 
{CONUM}      {
                yylval.ival = exec_cnum(yytext);
                return CNUM;
             }
			 
{STRIN}      {
                if (find_var(yytext) == -1) add_var(yytext);
                yylval.ival = find_var(yytext);
                return STRING;
             }
			 
{CARES}      {return *yytext;}
			 
\n            line_num++;

{BLOCO}      {line_num += conta_linhas(yytext);}
{COMEN}      ;
{WS}         ;
.            ;
%%

int yywrap(void) {return 1;}

int conta_linhas(char *texto)
{
   int cnt = 0;
   for(int i=0;i<strlen(texto);i++) if (texto[i] == '\n') cnt++;
   return cnt;
}