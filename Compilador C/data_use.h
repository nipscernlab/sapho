#ifndef DATA_USE_H_INCLUDED
#define DATA_USE_H_INCLUDED

// retorno pra exp (ver regra pra exp)
//   OFST    -> inteiro reduzido
// 2*OFST    -> float   reduzido
//   OFTS+id -> identificador int
// 2*OFST+id -> identificador float
// essa separacao de um valor de OFST entre tipos de dados funcionou
// pra determinar a ordem corretas das operacoes (ver funcao "operacoes")
// embora nao seja uma coisa muito convencional, acho que deu muito certo
#define OFST 1000000

void      load_check(int et, int sinal);
void  array_1d_check(int id, int et , int flag);
void  array_2d_check(int id, int et1, int et2 );

// funcoes auxiliares para reducao exp
int     num2exp  (int id, int dtype);
int      id2exp  (int id);
int   array1d2exp(int id, int et , int fft);
int   array2d2exp(int id, int et1, int et2);
int     exp_pplus(int et);

// variaeis de estado
int acc_ok;       // 0 -> acc vazio (use LOAD)  , 1 -> acc carregado (use PLD)
int exec_fft_use; // diz se a ultima chamada de um array foi ou nao com bit invertido
int exec_fft_set;

#endif // DATA_USE_H_INCLUDED
