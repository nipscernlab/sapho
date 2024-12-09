#ifndef STDLIB_H_INCLUDED
#define STDLIB_H_INCLUDED

int  exec_in  (int et );          // input de dados
int  exec_abs (int et );          // valor absoluto
int  exec_pst (int et );          // zera se for negativo
void exec_out1(int et );          // primeiro argumento do out
void exec_out2(int et );          // segundo argumento do out
int  exec_sign(int et1, int et2); // pega o sinal do primeiro argumento e coloca no segundo
int  exec_norm(int et );          // divisao por constante
int  exec_sqrt(int et );          // raiz quadrada
int  exec_atan(int et );          // raiz quadrada
int  exec_fase(int et );

// testes com numeros complexos -----------------------------------------------
int    mod_sqr(int et );          // modulo ao quadrado
int   abs_comp(int et );
int   neg_comp(int et );
int  exec_real(int et );
int  exec_imag(int et );
// fim do teste ---------------------------------------------------------------

#endif // STDLIB_H_INCLUDED
