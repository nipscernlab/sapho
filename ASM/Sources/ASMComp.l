%{

#include "..\Headers\eval.h"
#include "..\Headers\hdl.h"

#include <string.h>

%}

INNUM  [-+]?[0-9]+
FLNUM  [-+]?(0|[1-9]+[0-9]*)\.?[0-9]*

VARIA    [a-zA-Z][a-zA-Z0-9_]*
LABEL   @[a-zA-Z][a-zA-Z0-9_]*
STRIN   \"(\\.|[^\\"])*\"

COMEN    "//"[^\n]*
EMPTY        [ \t]
LINHA          \n

%%

"#PRNAME"  eval_direct( 1);                       // nome do processador
"#NUBITS"  eval_direct( 2);                       // num de bits da ULA
"#NBMANT"  eval_direct( 3);                       // num de bits da mantissa
"#NBEXPO"  eval_direct( 4);                       // num de bits do expoente
"#NDSTAC"  eval_direct( 5);                       // tamanho da pilha de dados
"#SDEPTH"  eval_direct( 6);                       // tamanho da pilha de subrotinas
"#NUIOIN"  eval_direct( 7);                       // quantidade de portas de entrada
"#NUIOOU"  eval_direct( 8);                       // quantidade de portas de saida
"#NUGAIN"  eval_direct( 9);                       // constante de divisao da funcao norm()
"#FFTSIZ"  eval_direct(10);                       // tamanho da fft (em bits)

"#array"   eval_direct(11);                       // array normal
"#arrays"  eval_direct(13);                       // array com arquivo de inicializacao

"#ITRAD"                  ;                       // ponto de endereco da interrupcao

   "LOD"   eval_opcode( 0,17, yytext,   ""     ); // carrega dado da memoria
 "P_LOD"   eval_opcode( 1,17, yytext,   ""     ); // PUSH + LOD
   "LDI"   eval_opcode( 2,17, yytext,   "LDI"  ); // LOD com enderecamento indireto (indice no acc)
   "ILI"   eval_opcode( 3,17, yytext,   "ILI"  ); // LOD com enderecamento indireto invertido (FFT)

   "SET"   eval_opcode( 4,17, yytext,   ""     ); // salva dado na memoria
   "SET_P" eval_opcode( 5,17, yytext,   ""     ); // SET + POP
   "SRF"   eval_opcode( 6, 0, yytext,   "SRF"  ); // SET com enderecamento indireto (indice na pilha)
   "IRF"   eval_opcode( 7, 0, yytext,   "ILI"  ); // SET com enderecamento indireto invertido (FFT)

   "PSH"   eval_opcode( 8, 0, yytext,   ""     ); // xuxa dado na pilha
   "POP"   eval_opcode( 9, 0, yytext,   ""     ); // retira dado da pilha

   "INN"   eval_opcode(10, 0, yytext,   ""     ); // entrada de dados
   "OUT"   eval_opcode(11, 0, yytext,   ""     ); // saida   de dados

   "JMP"   eval_opcode(12,18, yytext,   ""     ); // salto incondicional
   "JIZ"   eval_opcode(13,18, yytext,   ""     ); // salta se acc for zero

   "CAL"   eval_opcode(14,18, yytext,   "CAL"  ); // chamada de subrotina
   "RET"   eval_opcode(15, 0, yytext,   ""     ); // retorno de subrotina

   "ADD"   eval_opcode(16,17, yytext,   "ADD"  ); // adicao em ponto fixo com memoria
 "S_ADD"   eval_opcode(17, 0, yytext,   "ADD"  ); // adicao em ponto fixo com pilha
 "F_ADD"   eval_opcode(18,17, yytext, "F_ADD"  ); // adicao em ponto flut com memoria
"SF_ADD"   eval_opcode(19, 0, yytext, "F_ADD"  ); // adicao em ponto flut com pilha

   "MLT"   eval_opcode(20,17, yytext,   "MLT"  ); // multip em ponto fixo com memoria
 "S_MLT"   eval_opcode(21, 0, yytext,   "MLT"  ); // multip em ponto fixo com pilha
 "F_MLT"   eval_opcode(22,17, yytext, "F_MLT"  ); // multip em ponto flut com memoria
"SF_MLT"   eval_opcode(23, 0, yytext, "F_MLT"  ); // multip em ponto flut com pilha

   "DIV"   eval_opcode(24,17, yytext,   "DIV"  ); // divisa em ponto fixo com memoria
 "S_DIV"   eval_opcode(25, 0, yytext,   "DIV"  ); // divisa em ponto fixo com pilha
 "F_DIV"   eval_opcode(26,17, yytext, "F_DIV"  ); // divisa em ponto flut com memoria
"SF_DIV"   eval_opcode(27, 0, yytext, "F_DIV"  ); // divisa em ponto flut com pilha

   "MOD"   eval_opcode(28,17, yytext,   "MOD"  ); // resto da divisao em ponto fixo com memoria
 "S_MOD"   eval_opcode(29, 0, yytext,   "MOD"  ); // resto da divisao em ponto fixo com pilha

   "SGN"   eval_opcode(30,17, yytext,   "SGN"  ); // pega sinal em ponto fixo com memoria
 "S_SGN"   eval_opcode(31, 0, yytext,   "SGN"  ); // pega sinal em ponto fixo com pilha
 "F_SGN"   eval_opcode(32,17, yytext, "F_SGN"  ); // pega sinal em ponto flut com memoria
"SF_SGN"   eval_opcode(33, 0, yytext, "F_SGN"  ); // pega sinal em ponto flut com pilha

   "NEG"   eval_opcode(34, 0, yytext,   "NEG"  ); // negacao em ponto fixo com acc
   "NEG_M" eval_opcode(35,17, yytext,   "NEG_M"); // negacao em ponto fixo com memoria
 "P_NEG_M" eval_opcode(36,17, yytext,   "NEG_M"); // PUSH +   NEG_M
 "F_NEG"   eval_opcode(37, 0, yytext, "F_NEG"  ); // negacao em ponto flut com acc
 "F_NEG_M" eval_opcode(38,17, yytext, "F_NEG_M"); // negacao em ponto flut com memoria
"PF_NEG_M" eval_opcode(39,17, yytext, "F_NEG_M"); // PUSH + F_NEG_M

   "ABS"   eval_opcode(40, 0, yytext,   "ABS"  ); // abs em ponto fixo com acc
   "ABS_M" eval_opcode(41,17, yytext,   "ABS_M"); // abs em ponto fixo com memoria
 "P_ABS_M" eval_opcode(42,17, yytext,   "ABS_M"); // PUSH +   ABS_M
 "F_ABS"   eval_opcode(43, 0, yytext, "F_ABS"  ); // abs em ponto flut com acc
 "F_ABS_M" eval_opcode(44,17, yytext, "F_ABS_M"); // abs em ponto flut com memoria
"PF_ABS_M" eval_opcode(45,17, yytext, "F_ABS_M"); // PUSH + F_ABS_M

   "PST"   eval_opcode(46, 0, yytext,   "PST"  ); // zera negativo em ponto fixo com acc
   "PST_M" eval_opcode(47,17, yytext,   "PST_M"); // zera negativo em ponto fixo com memoria
 "P_PST_M" eval_opcode(48,17, yytext,   "PST_M"); // PUSH +   PST_M
 "F_PST"   eval_opcode(49, 0, yytext, "F_PST"  ); // zera negativo em ponto flut com acc
 "F_PST_M" eval_opcode(50,17, yytext, "F_PST_M"); // zera negativo em ponto flut com memoria
"PF_PST_M" eval_opcode(51,17, yytext, "F_PST_M"); // PUSH + F_PST_M

   "NRM"   eval_opcode(52, 0, yytext,   "NRM"  ); // divide por constante em ponto fixo com acc
   "NRM_M" eval_opcode(53,17, yytext,   "NRM_M"); // divide por constante em ponto fixo com memoria
 "P_NRM_M" eval_opcode(54,17, yytext,   "NRM_M"); // PUSH +   NRM_M

   "I2F"   eval_opcode(55, 0, yytext,   "I2F"  ); // int2float com acc
   "I2F_M" eval_opcode(56,17, yytext,   "I2F_M"); // int2float com mem
 "P_I2F_M" eval_opcode(57,17, yytext,   "I2F_M"); // PUSH + I2F_M

   "F2I"   eval_opcode(58, 0, yytext,   "F2I"  ); // float2int com acc
   "F2I_M" eval_opcode(59,17, yytext,   "F2I_M"); // float2int com mem
 "P_F2I_M" eval_opcode(60,17, yytext,   "F2I_M"); // PUSH + F2I_M

   "AND"   eval_opcode(61,17, yytext,   "AND"  ); // and com memoria
 "S_AND"   eval_opcode(62, 0, yytext,   "AND"  ); // and com pilha
   "ORR"   eval_opcode(63,17, yytext,   "ORR"  ); // or  com memoria
 "S_ORR"   eval_opcode(64, 0, yytext,   "ORR"  ); // or  cop pilha
   "XOR"   eval_opcode(65,17, yytext,   "XOR"  ); // xor com memoria
 "S_XOR"   eval_opcode(66, 0, yytext,   "XOR"  ); // xor com pilha
   
   "INV"   eval_opcode(67, 0, yytext,   "INV"  ); // not com acc
   "INV_M" eval_opcode(68,17, yytext,   "INV_M"); // not com mem
 "P_INV_M" eval_opcode(69,17, yytext,   "INV_M"); // PUSH + INV_M

   "LAN"   eval_opcode(70,17, yytext,   "LAN"  ); // and logico com memoria
 "S_LAN"   eval_opcode(71, 0, yytext,   "LAN"  ); // and logico com pilha
   "LOR"   eval_opcode(72,17, yytext,   "LOR"  ); // or  logico com memoria
 "S_LOR"   eval_opcode(73, 0, yytext,   "LOR"  ); // or  logico com pilha

   "LIN"   eval_opcode(74, 0, yytext,   "LIN"  ); // inversao logica com acc
   "LIN_M" eval_opcode(75,17, yytext,   "LIN_M"); // inversao logica com mem
 "P_LIN_M" eval_opcode(76,17, yytext,   "LIN_M"); // PUSH + LIN_M

   "LES"   eval_opcode(77,17, yytext,   "LES"  ); // menor que em ponto fixo com memoria
 "S_LES"   eval_opcode(78, 0, yytext,   "LES"  ); // menor que em ponto fixo com pilha
 "F_LES"   eval_opcode(79,17, yytext, "F_LES"  ); // menor que em ponto flut com memoria
"SF_LES"   eval_opcode(80, 0, yytext, "F_LES"  ); // menor que em ponto flut com pilha

   "GRE"   eval_opcode(81,17, yytext,   "GRE"  ); // maior que em ponto fixo com memoria
 "S_GRE"   eval_opcode(82, 0, yytext,   "GRE"  ); // maior que em ponto fixo com pilha
 "F_GRE"   eval_opcode(83,17, yytext, "F_GRE"  ); // maior que em ponto flut com memoria
"SF_GRE"   eval_opcode(84, 0, yytext, "F_GRE"  ); // maior que em ponto flut com pilha

   "EQU"   eval_opcode(85,17, yytext,   "EQU"  ); // igual a com memoria
 "S_EQU"   eval_opcode(86, 0, yytext,   "EQU"  ); // igual a com pilha

   "SHL"   eval_opcode(87,17, yytext,   "SHL"  ); // shift left  com memoria
 "S_SHL"   eval_opcode(88, 0, yytext,   "SHL"  ); // shift left  com pilha
   "SHR"   eval_opcode(89,17, yytext,   "SHR"  ); // shift right com memoria
 "S_SHR"   eval_opcode(90, 0, yytext,   "SHR"  ); // shift right com pilha
   "SRS"   eval_opcode(91,17, yytext,   "SRS"  ); // shift right aritmetico com memoria
 "S_SRS"   eval_opcode(92, 0, yytext,   "SRS"  ); // shift right aritmetico com pilha

{VARIA}    eval_opernd(yytext,0); // usado em instrucoes da ula com variaveis (IDs)
{INNUM}    eval_opernd(yytext,1); // usado em instrucoes da ula com constante int
{FLNUM}    eval_opernd(yytext,2); // usado em instrucoes da ula com constante float

{STRIN}    eval_opernd(yytext,0); // usado em instrucoes com strings (ex: arquivo de incializacao de array)
{LABEL}                         ; // usado em instrucoes de salto

{COMEN}
{EMPTY}
{LINHA}

.;
%%

void main(int argc, char *argv[])
{
	strcpy(proc_dir,argv[2]);                             // path da pasta Hardware
	strcpy( hdl_dir,argv[3]);                             // path da pasta HDL
	strcpy(temp_dir,argv[4]);                             // path da pasta Temp

  yyin = fopen(argv[1], "r");                           // primeiro argumento eh o arquivo .asm de entrada
	eval_init(atoi(argv[5]),atoi(argv[6]),atoi(argv[7])); // inicializa as variaveis de estado
  yylex();                                              // executa o lexer
	fclose(yyin);                                         // fecha o arquivo .asm
	eval_finish();                                        // executa finalizacao do assembler
}

int yywrap(void) {return 1;}