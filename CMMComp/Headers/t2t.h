#include <stdio.h>

extern int fgen, mgen;                  // se gera ou nao macros de ponto flutuante
extern int i2f , f2i, fadd, fmlt, fdiv; // quais macros de pf sao usadas
extern int fsqrt, fsqrti;               // gerar macro pra raiz quadrada
extern int fatan, fatani;               // gerar macro pra arco tangente

extern int nbmant;                      // numero de bits de mantissa
extern int nbexpo;                      // numero de bits de expoente

unsigned int  f2mf       (char  *va  ); // converte float para meu float
         void float_init (           ); // inicializa as variaveis acima
         void float_geni (char * fasm); // gera as instrucoes para float em ponto fixo que precisam
         void float_genf (char * fasm); // gera constantes para trigonometria
         void  math_gen  (char * fasm); // gera as instrucoes para funcoes matematicas

char *itob(int x, int w);               // converte inteiro para string binaria