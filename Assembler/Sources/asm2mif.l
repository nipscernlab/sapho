%{

#include "eval.h"
#include "veri_comp.h"
#include "mnemonicos.h"

%}

CONST    [-+]?[0-9]*\.?[0-9]+
VARIA    [a-zA-Z][a-zA-Z0-9_]*
STRIN     \"(\\.|[^\\"])*\"
LABEL   @[a-zA-Z][a-zA-Z0-9_]*

COMEN    "//"[^\n]*
EMPTY        [ \t]
LINHA          \n

%%

"#array"  eval_direct( 3);  // array normal
"#arrays" eval_direct(16);  // array com arquivo de inicializacao

"#PRNAME" eval_direct( 5);  // diretivas
"#NUBITS" eval_direct( 6);
"#NBMANT" eval_direct( 7);
"#NBEXPO" eval_direct( 8);
"#NDSTAC" eval_direct( 9);
"#SDEPTH" eval_direct(10);
"#NUIOIN" eval_direct(11);
"#NUIOOU" eval_direct(12);
"#DATYPE" eval_direct(13);
"#DIRNAM" eval_direct(14);
"#NUGAIN" eval_direct(15);
"#FFTSIZ" eval_direct(20);
"#ITRAD" {if (pp) itr_addr = n_ins;} // isso nao eh mais uma instrucao. trocar por uma diretiva

"LOAD"    eval_opcode( 0,1); // todas as instrucoes
"PLD"     eval_opcode( 1,1);
"SET"     eval_opcode( 2,1);
"SETP"    eval_opcode( 3,1);
"PUSH"    eval_opcode( 4,0);
"JZ"      eval_opcode( 5,2);
"JMP"     eval_opcode( 6,2);
"CALL"   {eval_opcode( 7,2); add_mne("CAL" );} // add_mne aloca recurso no arquivo verilog
"RETURN"  eval_opcode( 8,0);
"SRF"    {eval_opcode( 9,0); add_mne("SRF" );}
"IN"      eval_opcode(10,0);
"OUT"     eval_opcode(11,0);
 "NEG"   {eval_opcode(12,0); add_mne("NEG" );}
 "ADD"   {eval_opcode(13,1); add_mne("ADD" );}
"SADD"   {eval_opcode(14,0); add_mne("ADD" );}
 "MLT"   {eval_opcode(15,1); add_mne("MLT" );}
"SMLT"   {eval_opcode(16,0); add_mne("MLT" );}
 "DIV"   {eval_opcode(17,1); add_mne("DIV" );}
"SDIV"   {eval_opcode(18,0); add_mne("DIV" );}
 "MOD"   {eval_opcode(19,1); add_mne("MOD" );}
"SMOD"   {eval_opcode(20,0); add_mne("MOD" );}
  "AND"  {eval_opcode(21,1); add_mne("AND" );}
 "SAND"  {eval_opcode(22,0); add_mne("AND" );}
 "LAND"  {eval_opcode(23,1); add_mne("LAN" );}
"SLAND"  {eval_opcode(24,0); add_mne("LAN" );}
  "OR"   {eval_opcode(25,1); add_mne("OR"  );}
 "SOR"   {eval_opcode(26,0); add_mne("OR"  );}
 "LOR"   {eval_opcode(27,1); add_mne("LOR" );}
"SLOR"   {eval_opcode(28,0); add_mne("LOR" );}
 "XOR"   {eval_opcode(29,1); add_mne("XOR" );}
"SXOR"   {eval_opcode(30,0); add_mne("XOR" );}
 "INV"   {eval_opcode(31,0); add_mne("INV" );}
"LINV"   {eval_opcode(32,0); add_mne("LIN" );}
 "EQU"   {eval_opcode(33,1); add_mne("EQU" );}
"SEQU"   {eval_opcode(34,0); add_mne("EQU" );}
 "GRE"   {eval_opcode(35,1); add_mne("GRE" );}
"SGRE"   {eval_opcode(36,0); add_mne("GRE" );}
 "LES"   {eval_opcode(37,1); add_mne("LES" );}
"SLES"   {eval_opcode(38,0); add_mne("LES" );}
 "SHR"   {eval_opcode(39,1); add_mne("SHR" );}
"SSHR"   {eval_opcode(40,0); add_mne("SHR" );}
 "SHL"   {eval_opcode(41,1); add_mne("SHL" );}
"SSHL"   {eval_opcode(42,0); add_mne("SHL" );}
 "SRS"   {eval_opcode(43,1); add_mne("SRS" );}
"SSRS"   {eval_opcode(44,0); add_mne("SRS" );}
"PSET"   {eval_opcode(45,0); add_mne("PST" );}
"PSETS"  {eval_opcode(46,1); add_mne("PSTS");}
"NORM"   {eval_opcode(47,0); add_mne("NRM" );}
"NORMS"  {eval_opcode(48,1); add_mne("NRMS");}
"ABS"    {eval_opcode(49,0); add_mne("ABS" );}
"ABSS"   {eval_opcode(50,1); add_mne("ABSS");}
"SIGN"   {eval_opcode(51,1); add_mne("SGN" );}
"SSIGN"  {eval_opcode(52,0); add_mne("SGN" );}
"ISRF"   {eval_opcode(53,0); add_mne("FFT" ); isrf = 1;} // isrf auxilia no teste do tamanho da memoria no final
"NEGS"   {eval_opcode(54,1); add_mne("NEGS");}

{CONST}   eval_opernd(yytext  ,1);
{STRIN}   eval_opernd(yytext+1,0);
{VARIA}   eval_opernd(yytext  ,0);
{LABEL}   eval_label (yytext+1  );

{COMEN}
{EMPTY}
{LINHA}
.         fprintf(stderr, "Erro de sintaxe: %s\n", yytext);
%%

int main(int argc, char *argv[])
{
    yyin = fopen(argv[1], "r");
	eval_init(1);
    yylex();
	fclose(yyin);

	yyin = fopen(argv[1], "r");
	eval_init(0);
	yylex();
    fclose(yyin);
	eval_finish();

	fprintf(stdout, "###### Memória de instruções: %d\n", n_ins);
	fprintf(stdout, "###### Memória de dados: %d\n"     , n_dat);

    return 0;
}

int yywrap(void) {return 1;}
