#ifndef T2T_H_INCLUDED
#define T2T_H_INCLUDED

#include <stdio.h>

// nao consigo inicializar essas variaveis aqui, nao sei pq!
int fgen, mgen;                  // se gera ou nao macros de ponto flutuante
int i2f , f2i, fadd, fmlt, fdiv; // quais macros de pf sao usadas
int fsqrt, fsqrti;               // quais funcoes aritmeticas gerar

int nbmant;                      // numero de bits de mantissa
int nbexpo;                      // numero de bits de expoente

int  f2mf       (char  *va  );   // converte float para meu float
void float_init (           );   // inicializa as variaveis acima
void float_begin(FILE *f_asm);   // inicializacao de variaveis float no comeco do .asm
void float_geni (char * fasm);   // gera as instrucoes para float em ponto fixo que precisam
void float_genf (char * fasm);   // gera constantes para trigonometria
void  math_gen  (char * fasm);   // gera as instrucoes para funcoes matematicas

#endif // T2T_H_INCLUDED
