%{
    #include "..\Headers\eval.h"
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

"#PRNAME"  eval_direct( 1); // nome do processador
"#NUBITS"  eval_direct( 2); // num de bits da ULA
"#NBMANT"  eval_direct( 3); // num de bits da mantissa
"#NBEXPO"  eval_direct( 4); // num de bits do expoente
"#NDSTAC"  eval_direct( 5); // tamanho da pilha de dados
"#SDEPTH"  eval_direct( 6); // tamanho da pilha de subrotinas
"#NUIOIN"  eval_direct( 7); // quantidade de portas de entrada
"#NUIOOU"  eval_direct( 8); // quantidade de portas de saida
"#NUGAIN"  eval_direct( 9); // constante de divisao da funcao norm()
"#FFTSIZ"  eval_direct(10); // tamanho da fft (em bits)

"#array"   eval_direct(11); // array normal
"#arrays"  eval_direct(13); // array com arquivo de inicializacao

"#ITRAD"   eval_itrad (  ); // ponto de endereco da interrupcao

   "LOD"   eval_opcode(17); // carrega dado da memoria
 "P_LOD"   eval_opcode(17); // PUSH + LOD
   "LDI"   eval_opcode(17); // LOD com enderecamento indireto (indice no acc)
   "ILI"   eval_opcode(17); // LOD com enderecamento indireto invertido (FFT)

   "SET"   eval_opcode(17); // salva dado na memoria
   "SET_P" eval_opcode(17); // SET + POP
   "STI"   eval_opcode( 0); // SET com enderecamento indireto (indice na pilha)
   "ISI"   eval_opcode( 0); // SET com enderecamento indireto invertido (FFT)

   "PSH"   eval_opcode( 0); // xuxa dado na pilha
   "POP"   eval_opcode( 0); // retira dado da pilha

   "INN"   eval_opcode(19); // entrada de dados
 "P_INN"   eval_opcode(19); // PUSH + INN
   "OUT"   eval_opcode(19); // saida de dados

   "JMP"   eval_opcode(18); // salto incondicional
   "JIZ"   eval_opcode(18); // salta se acc for zero

   "CAL"   eval_opcode(18); // chamada de subrotina
   "RET"   eval_opcode( 0); // retorno de subrotina

   "ADD"   eval_opcode(17); // adicao em ponto fixo com memoria
 "S_ADD"   eval_opcode( 0); // adicao em ponto fixo com pilha
 "F_ADD"   eval_opcode(17); // adicao em ponto flut com memoria
"SF_ADD"   eval_opcode( 0); // adicao em ponto flut com pilha

   "MLT"   eval_opcode(17); // multip em ponto fixo com memoria
 "S_MLT"   eval_opcode( 0); // multip em ponto fixo com pilha
 "F_MLT"   eval_opcode(17); // multip em ponto flut com memoria
"SF_MLT"   eval_opcode( 0); // multip em ponto flut com pilha

   "DIV"   eval_opcode(17); // divisa em ponto fixo com memoria
 "S_DIV"   eval_opcode( 0); // divisa em ponto fixo com pilha
 "F_DIV"   eval_opcode(17); // divisa em ponto flut com memoria
"SF_DIV"   eval_opcode( 0); // divisa em ponto flut com pilha

   "MOD"   eval_opcode(17); // resto da divisao em ponto fixo com memoria
 "S_MOD"   eval_opcode( 0); // resto da divisao em ponto fixo com pilha

   "SGN"   eval_opcode(17); // pega sinal em ponto fixo com memoria
 "S_SGN"   eval_opcode( 0); // pega sinal em ponto fixo com pilha
 "F_SGN"   eval_opcode(17); // pega sinal em ponto flut com memoria
"SF_SGN"   eval_opcode( 0); // pega sinal em ponto flut com pilha

   "NEG"   eval_opcode( 0); // negacao em ponto fixo com acc
   "NEG_M" eval_opcode(17); // negacao em ponto fixo com memoria
 "P_NEG_M" eval_opcode(17); // PUSH +   NEG_M
 "F_NEG"   eval_opcode( 0); // negacao em ponto flut com acc
 "F_NEG_M" eval_opcode(17); // negacao em ponto flut com memoria
"PF_NEG_M" eval_opcode(17); // PUSH + F_NEG_M

   "ABS"   eval_opcode( 0); // abs em ponto fixo com acc
   "ABS_M" eval_opcode(17); // abs em ponto fixo com memoria
 "P_ABS_M" eval_opcode(17); // PUSH +   ABS_M
 "F_ABS"   eval_opcode( 0); // abs em ponto flut com acc
 "F_ABS_M" eval_opcode(17); // abs em ponto flut com memoria
"PF_ABS_M" eval_opcode(17); // PUSH + F_ABS_M

   "PST"   eval_opcode( 0); // zera negativo em ponto fixo com acc
   "PST_M" eval_opcode(17); // zera negativo em ponto fixo com memoria
 "P_PST_M" eval_opcode(17); // PUSH +   PST_M
 "F_PST"   eval_opcode( 0); // zera negativo em ponto flut com acc
 "F_PST_M" eval_opcode(17); // zera negativo em ponto flut com memoria
"PF_PST_M" eval_opcode(17); // PUSH + F_PST_M

   "NRM"   eval_opcode( 0); // divide por constante em ponto fixo com acc
   "NRM_M" eval_opcode(17); // divide por constante em ponto fixo com memoria
 "P_NRM_M" eval_opcode(17); // PUSH +   NRM_M

   "I2F"   eval_opcode( 0); // int2float com acc
   "I2F_M" eval_opcode(17); // int2float com mem
 "P_I2F_M" eval_opcode(17); // PUSH + I2F_M

   "F2I"   eval_opcode( 0); // float2int com acc
   "F2I_M" eval_opcode(17); // float2int com mem
 "P_F2I_M" eval_opcode(17); // PUSH + F2I_M

   "AND"   eval_opcode(17); // and com memoria
 "S_AND"   eval_opcode( 0); // and com pilha
   "ORR"   eval_opcode(17); // or  com memoria
 "S_ORR"   eval_opcode( 0); // or  cop pilha
   "XOR"   eval_opcode(17); // xor com memoria
 "S_XOR"   eval_opcode( 0); // xor com pilha
   
   "INV"   eval_opcode( 0); // not com acc
   "INV_M" eval_opcode(17); // not com mem
 "P_INV_M" eval_opcode(17); // PUSH + INV_M

   "LAN"   eval_opcode(17); // and logico com memoria
 "S_LAN"   eval_opcode( 0); // and logico com pilha
   "LOR"   eval_opcode(17); // or  logico com memoria
 "S_LOR"   eval_opcode( 0); // or  logico com pilha

   "LIN"   eval_opcode( 0); // inversao logica com acc
   "LIN_M" eval_opcode(17); // inversao logica com mem
 "P_LIN_M" eval_opcode(17); // PUSH + LIN_M

   "LES"   eval_opcode(17); // menor que em ponto fixo com memoria
 "S_LES"   eval_opcode( 0); // menor que em ponto fixo com pilha
 "F_LES"   eval_opcode(17); // menor que em ponto flut com memoria
"SF_LES"   eval_opcode( 0); // menor que em ponto flut com pilha

   "GRE"   eval_opcode(17); // maior que em ponto fixo com memoria
 "S_GRE"   eval_opcode( 0); // maior que em ponto fixo com pilha
 "F_GRE"   eval_opcode(17); // maior que em ponto flut com memoria
"SF_GRE"   eval_opcode( 0); // maior que em ponto flut com pilha

   "EQU"   eval_opcode(17); // igual a com memoria
 "S_EQU"   eval_opcode( 0); // igual a com pilha

   "SHL"   eval_opcode(17); // shift left  com memoria
 "S_SHL"   eval_opcode( 0); // shift left  com pilha
   "SHR"   eval_opcode(17); // shift right com memoria
 "S_SHR"   eval_opcode( 0); // shift right com pilha
   "SRS"   eval_opcode(17); // shift right aritmetico com memoria
 "S_SRS"   eval_opcode( 0); // shift right aritmetico com pilha

{VARIA}    eval_opernd(yytext); // usado em instrucoes da ula com variaveis (IDs)
{INNUM}    eval_opernd(yytext); // usado em instrucoes da ula com constante int
{FLNUM}    eval_opernd(yytext); // usado em instrucoes da ula com constante float

{STRIN}    eval_opernd(yytext); // usado em instrucoes com strings (ex: arquivo de incializacao de array)
{LABEL}    eval_label (yytext); // usado em instrucoes de salto

{COMEN}
{EMPTY}
{LINHA}

.;
%%

void main(int argc, char *argv[])
{
	  yyin = fopen(argv[1], "r"); // primeiro argumento eh o arquivo .asm de entrada
    eval_init(argv[2]);         // segundo  argumento eh o arquivo de log
    yylex();                    // executa o lexer
	  fclose(yyin);               // fecha o arquivo .asm
    eval_finish();              // finaliza o arquivo de log
}

int yywrap(void) {return 1;}