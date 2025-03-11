%{

#include "..\Headers\eval.h"
#include "..\Headers\veri_comp.h"

#include <string.h>

// redeclaracao de variaveis globais
char temp_dir[1024], hdl_dir[1024], proc_dir[1024];
int  sim_typ, clk_frq, clk_num;
int  itr_addr = 0;
int  n_ins	  = 0;
int  pp       = 0;

%}

CONST    [-+]?[0-9]*\.?[0-9]+
VARIA    [a-zA-Z][a-zA-Z0-9_]*
STRIN     \"(\\.|[^\\"])*\"
LABEL   @[a-zA-Z][a-zA-Z0-9_]*

COMEN    "//"[^\n]*
EMPTY        [ \t]
LINHA          \n

%%

"#PRNAME" eval_direct( 5);                  // diretivas iniciais
"#NUBITS" eval_direct( 6);                  // eliminar essa diretiva
"#NBMANT" eval_direct( 7);
"#NBEXPO" eval_direct( 8);
"#NDSTAC" eval_direct( 9);
"#SDEPTH" eval_direct(10);
"#NUIOIN" eval_direct(11);
"#NUIOOU" eval_direct(12);
"#DATYPE" eval_direct(13);
"#NUGAIN" eval_direct(15);
"#FFTSIZ" eval_direct(20);

"#array"  eval_direct( 3);                  // array normal
"#arrays" eval_direct(16);                  // array com arquivo de inicializacao

"#ITRAD" {if (pp) itr_addr = n_ins;}        // eh uma diretiva sem argumento (sem eval_direct())

"LOAD"   eval_opcode( 0,1, yytext, ""    ); // todas as instrucoes
"PLD"    eval_opcode( 1,1, yytext, ""    );
"SET"    eval_opcode( 2,1, yytext, ""    );
"SETP"   eval_opcode( 3,1, yytext, ""    );
"PUSH"   eval_opcode( 4,0, yytext, ""    );
"JZ"     eval_opcode( 5,2, yytext, ""    );
"JMP"    eval_opcode( 6,2, yytext, ""    );
"CALL"   eval_opcode( 7,2, yytext, "CAL" );
"RETURN" eval_opcode( 8,0, yytext, ""    );
"SRF"    eval_opcode( 9,0, yytext, "SRF" );
"IN"     eval_opcode(10,0, yytext, ""    );
"OUT"    eval_opcode(11,0, yytext, ""    );
 "NEG"   eval_opcode(12,0, yytext, "NEG" );
 "ADD"   eval_opcode(13,1, yytext, "ADD" );
"SADD"   eval_opcode(14,0, yytext, "ADD" );
 "MLT"   eval_opcode(15,1, yytext, "MLT" );
"SMLT"   eval_opcode(16,0, yytext, "MLT" );
 "DIV"   eval_opcode(17,1, yytext, "DIV" );
"SDIV"   eval_opcode(18,0, yytext, "DIV" );
 "MOD"   eval_opcode(19,1, yytext, "MOD" );
"SMOD"   eval_opcode(20,0, yytext, "MOD" );
  "AND"  eval_opcode(21,1, yytext, "AND" );
 "SAND"  eval_opcode(22,0, yytext, "AND" );
 "LAND"  eval_opcode(23,1, yytext, "LAN" );
"SLAND"  eval_opcode(24,0, yytext, "LAN" );
  "OR"   eval_opcode(25,1, yytext, "OR"  );
 "SOR"   eval_opcode(26,0, yytext, "OR"  );
 "LOR"   eval_opcode(27,1, yytext, "LOR" );
"SLOR"   eval_opcode(28,0, yytext, "LOR" );
 "XOR"   eval_opcode(29,1, yytext, "XOR" );
"SXOR"   eval_opcode(30,0, yytext, "XOR" );
 "INV"   eval_opcode(31,0, yytext, "INV" );
"LINV"   eval_opcode(32,0, yytext, "LIN" );
 "EQU"   eval_opcode(33,1, yytext, "EQU" );
"SEQU"   eval_opcode(34,0, yytext, "EQU" );
 "GRE"   eval_opcode(35,1, yytext, "GRE" );
"SGRE"   eval_opcode(36,0, yytext, "GRE" );
 "LES"   eval_opcode(37,1, yytext, "LES" );
"SLES"   eval_opcode(38,0, yytext, "LES" );
 "SHR"   eval_opcode(39,1, yytext, "SHR" );
"SSHR"   eval_opcode(40,0, yytext, "SHR" );
 "SHL"   eval_opcode(41,1, yytext, "SHL" );
"SSHL"   eval_opcode(42,0, yytext, "SHL" );
 "SRS"   eval_opcode(43,1, yytext, "SRS" );
"SSRS"   eval_opcode(44,0, yytext, "SRS" );
"PSET"   eval_opcode(45,0, yytext, "PST" );
"SSGN"   eval_opcode(46,0, yytext, "SGN" );
"NORM"   eval_opcode(47,0, yytext, "NRM" );
"FIA"    eval_opcode(48,0, yytext, "FIA" );
"ABS"    eval_opcode(49,0, yytext, "ABS" );
"ILDI"   eval_opcode(50,1, yytext, "FFT" );
"SIGN"   eval_opcode(51,1, yytext, "SGN" );
"LDI"    eval_opcode(52,1, yytext, "LDI" );
"ISRF"   eval_opcode(53,0, yytext, "FFT" );

{CONST}   eval_opernd(yytext  ,1); // instrucoes da ula com constantes
{STRIN}   eval_opernd(yytext+1,0); // tirar aspas dos arquivos pra nao usar mais isso
{VARIA}   eval_opernd(yytext  ,0); // instrucoes da ula com variaveis
{LABEL}   eval_label (yytext+1  ); // instrucoes de salto

{COMEN}
{EMPTY}
{LINHA}
.    {}//fprintf(stderr, "Erro de sintaxe: %s\n", yytext);
%%

int main(int argc, char *argv[])
{
	strcpy(proc_dir,argv[2]);        // path da pasta Hardware
	strcpy( hdl_dir,argv[3]);        // path da pasta HDL
	strcpy(temp_dir,argv[4]);        // path da pasta Tmp

	clk_frq =  atoi(argv[5]);        // frequencia do clock  de simulacao
	clk_num =  atoi(argv[6]);        // numero     de clocks da simulacao
	sim_typ =  atoi(argv[7]);        // 0 pra 1 proc apenas, 1 para multicore

    yyin = fopen(argv[1], "r");      // primeiro argumento eh o arquivo .asm de entrada
	eval_init(1);                    // inicializa fase de pre-processamento (pp)
    yylex();                         // executa fase pp
	fclose(yyin);                    // fecha o arquivo .asm

	yyin = fopen(argv[1], "r");      // reabre o arquivo
	eval_init(0);                    // inicializa fase padrao
	yylex();                         // executa fase padrao
    fclose(yyin);                    // fecha o arquivo .asm

	eval_finish();                   // executa finalizacao do assembler

	fprintf(stdout, "######  Memória de instruções: %d\n", n_ins);
	fprintf(stdout, "######  Memória de dados: %d\n"     , n_dat);

    return 0;
}

int yywrap(void) {return 1;}
