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

// variaeis de estado
extern int acc_ok;       // 0 -> acc vazio (use LOAD)  , 1 -> acc carregado (use PLD)
extern int exec_fft_use; // diz se o ultimo uso de um array foi ou nao com bit invertido
extern int exec_fft_set; // diz se o ultimo SET de um array foi ou nao com bit invertido

void      load_check(int et, int sinal);

// funcoes auxiliares para reducao exp
int       num2exp(int id, int dtype);
int        id2exp(int id);
int   array1d2exp(int id, int et , int fft);
int   array2d2exp(int id, int et1, int et2);
int     exp_pplus(int et);
int   array_pplus(int id, int et);
int   array_2plus(int id, int et1, int et2);

#endif // DATA_USE_H_INCLUDED
