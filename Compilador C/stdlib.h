#ifndef STDLIB_H_INCLUDED
#define STDLIB_H_INCLUDED

int   exec_in   (int et );          // input de dados
int   exec_abs  (int et );          // valor absoluto
int   exec_pst  (int et );          // zera se for negativo
void  exec_out1 (int et );          // primeiro argumento do out
void  exec_out2 (int et );          // segundo argumento do out
int   exec_sign (int et1, int et2); // pega o sinal do primeiro argumento e coloca no segundo
int   exec_norm (int et );          // divisao por constante

#endif // STDLIB_H_INCLUDED
