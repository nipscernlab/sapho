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

"#PRNAME" eval_direct( 5);                       // diretivas iniciais
"#NUBITS" eval_direct( 6);                       // eliminar essa diretiva?
"#NBMANT" eval_direct( 7);
"#NBEXPO" eval_direct( 8);
"#NDSTAC" eval_direct( 9);
"#SDEPTH" eval_direct(10);
"#NUIOIN" eval_direct(11);
"#NUIOOU" eval_direct(12);
"#NUGAIN" eval_direct(15);
"#FFTSIZ" eval_direct(20);

"#array"  eval_direct( 3);                       // array normal
"#arrays" eval_direct(16);                       // array com arquivo de inicializacao

"#ITRAD" {if (pp) itr_addr = n_ins;}             // eh uma diretiva sem argumento (sem eval_direct())

   "LOD"   eval_opcode( 0,1, yytext,   ""     ); // carrega dado da memoria
 "P_LOD"   eval_opcode( 1,1, yytext,   ""     ); // PUSH + LOD
   "LDI"   eval_opcode( 2,1, yytext,   "LDI"  ); // LOD com enderecamento indireto (indice no acc)
   "ILI"   eval_opcode( 3,1, yytext,   "ILI"  ); // LOD com enderecamento indireto invertido (FFT)

   "SET"   eval_opcode( 4,1, yytext,   ""     ); // salva dado na memoria
   "SET_P" eval_opcode( 5,1, yytext,   ""     ); // SET + POP
   "SRF"   eval_opcode( 6,0, yytext,   "SRF"  ); // SET com enderecamento indireto (indice na pilha)
   "IRF"   eval_opcode( 7,0, yytext,   "ILI"  ); // SET com enderecamento indireto invertido (FFT)

   "PSH"   eval_opcode( 8,0, yytext,   ""     ); // PUSH
   "POP"   eval_opcode( 9,0, yytext,   ""     ); // POP

   "INN"   eval_opcode(10,0, yytext,   ""     ); // entrada de dados
   "OUT"   eval_opcode(11,0, yytext,   ""     ); // saida de dados

   "JMP"   eval_opcode(12,2, yytext,   ""     ); // salto incondicional
   "JIZ"   eval_opcode(13,2, yytext,   ""     ); // salta se acc for zero

   "CAL"   eval_opcode(14,2, yytext,   "CAL"  ); // chamada de subrotina
   "RET"   eval_opcode(15,0, yytext,   ""     ); // retorno de subrotina

   "ADD"   eval_opcode(16,1, yytext,   "ADD"  );
 "S_ADD"   eval_opcode(17,0, yytext,   "ADD"  );
 "F_ADD"   eval_opcode(18,1, yytext, "F_ADD"  );
"SF_ADD"   eval_opcode(19,0, yytext, "F_ADD"  );

   "MLT"   eval_opcode(20,1, yytext,   "MLT"  );
 "S_MLT"   eval_opcode(21,0, yytext,   "MLT"  );
 "F_MLT"   eval_opcode(22,1, yytext, "F_MLT"  );
"SF_MLT"   eval_opcode(23,0, yytext, "F_MLT"  );

   "DIV"   eval_opcode(24,1, yytext,   "DIV"  );
 "S_DIV"   eval_opcode(25,0, yytext,   "DIV"  );
 "F_DIV"   eval_opcode(26,1, yytext, "F_DIV"  );
"SF_DIV"   eval_opcode(27,0, yytext, "F_DIV"  );

   "MOD"   eval_opcode(28,1, yytext,   "MOD"  );
 "S_MOD"   eval_opcode(29,0, yytext,   "MOD"  );

   "SGN"   eval_opcode(30,1, yytext,   "SGN"  );
 "S_SGN"   eval_opcode(31,0, yytext,   "SGN"  );
 "F_SGN"   eval_opcode(32,1, yytext, "F_SGN"  );
"SF_SGN"   eval_opcode(33,0, yytext, "F_SGN"  );

   "NEG"   eval_opcode(34,0, yytext,   "NEG"  );
   "NEG_M" eval_opcode(35,1, yytext,   "NEG_M");
 "P_NEG_M" eval_opcode(36,1, yytext,   "NEG_M");
 "F_NEG"   eval_opcode(37,0, yytext, "F_NEG"  );
 "F_NEG_M" eval_opcode(38,1, yytext, "F_NEG_M");
"PF_NEG_M" eval_opcode(39,1, yytext, "F_NEG_M");

   "ABS"   eval_opcode(40,0, yytext,   "ABS"  );
   "ABS_M" eval_opcode(41,1, yytext,   "ABS_M");
 "P_ABS_M" eval_opcode(42,1, yytext,   "ABS_M");
 "F_ABS"   eval_opcode(43,0, yytext, "F_ABS"  );
 "F_ABS_M" eval_opcode(44,1, yytext, "F_ABS_M");
"PF_ABS_M" eval_opcode(45,1, yytext, "F_ABS_M");

   "PST"   eval_opcode(46,0, yytext,   "PST"  );
   "PST_M" eval_opcode(47,1, yytext,   "PST_M");
 "P_PST_M" eval_opcode(48,1, yytext,   "PST_M");
 "F_PST"   eval_opcode(49,0, yytext, "F_PST"  );
 "F_PST_M" eval_opcode(50,1, yytext, "F_PST_M");
"PF_PST_M" eval_opcode(51,1, yytext, "F_PST_M");

   "NRM"   eval_opcode(52,0, yytext,   "NRM"  );
   "NRM_M" eval_opcode(53,1, yytext,   "NRM_M");
 "P_NRM_M" eval_opcode(54,1, yytext,   "NRM_M");

   "I2F"   eval_opcode(55,0, yytext,   "I2F"  );
   "I2F_M" eval_opcode(56,1, yytext,   "I2F_M");
 "P_I2F_M" eval_opcode(57,1, yytext,   "I2F_M");

   "F2I"   eval_opcode(58,0, yytext,   "F2I"  );
   "F2I_M" eval_opcode(59,1, yytext,   "F2I_M");
 "P_F2I_M" eval_opcode(60,1, yytext,   "F2I_M");

   "AND"   eval_opcode(61,1, yytext,   "AND"  );
 "S_AND"   eval_opcode(62,0, yytext,   "AND"  );
   "ORR"   eval_opcode(63,1, yytext,   "ORR"  );
 "S_ORR"   eval_opcode(64,0, yytext,   "ORR"  );
   "XOR"   eval_opcode(65,1, yytext,   "XOR"  );
 "S_XOR"   eval_opcode(66,0, yytext,   "XOR"  );
   
   "INV"   eval_opcode(67,0, yytext,   "INV"  );
   "INV_M" eval_opcode(68,1, yytext,   "INV_M");
 "P_INV_M" eval_opcode(69,1, yytext,   "INV_M");

   "LAN"   eval_opcode(70,1, yytext,   "LAN"  );
 "S_LAN"   eval_opcode(71,0, yytext,   "LAN"  );
   "LOR"   eval_opcode(72,1, yytext,   "LOR"  );
 "S_LOR"   eval_opcode(73,0, yytext,   "LOR"  );

   "LIN"   eval_opcode(74,0, yytext,   "LIN"  );
   "LIN_M" eval_opcode(75,1, yytext,   "LIN_M");
 "P_LIN_M" eval_opcode(76,1, yytext,   "LIN_M");

   "LES"   eval_opcode(77,1, yytext,   "LES"  );
 "S_LES"   eval_opcode(78,0, yytext,   "LES"  );
 "F_LES"   eval_opcode(79,1, yytext, "F_LES"  );
"SF_LES"   eval_opcode(80,0, yytext, "F_LES"  );

   "GRE"   eval_opcode(81,1, yytext,   "GRE"  );
 "S_GRE"   eval_opcode(82,0, yytext,   "GRE"  );
 "F_GRE"   eval_opcode(83,1, yytext, "F_GRE"  );
"SF_GRE"   eval_opcode(84,0, yytext, "F_GRE"  );

   "EQU"   eval_opcode(85,1, yytext,   "EQU"  );
 "S_EQU"   eval_opcode(86,0, yytext,   "EQU"  );

   "SHL"   eval_opcode(87,1, yytext,   "SHL"  );
 "S_SHL"   eval_opcode(88,0, yytext,   "SHL"  );
   "SHR"   eval_opcode(89,1, yytext,   "SHR"  );
 "S_SHR"   eval_opcode(90,0, yytext,   "SHR"  );
   "SRS"   eval_opcode(91,1, yytext,   "SRS"  );
 "S_SRS"   eval_opcode(92,0, yytext,   "SRS"  );

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
