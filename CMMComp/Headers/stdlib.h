// ----------------------------------------------------------------------------
// entrada e saida ------------------------------------------------------------
// ----------------------------------------------------------------------------

int  exec_in  (int et);           // input de dados
void exec_out1(int et);           // primeiro argumento do out
void exec_out2(int et);           // segundo  argumento do out

// ----------------------------------------------------------------------------
// funcoes especiais que evitam codigo ----------------------------------------
// ----------------------------------------------------------------------------

int  exec_sign(int et1, int et2); // pega o sinal do primeiro argumento e coloca no segundo
int  exec_abs (int et);           // valor absoluto
int  exec_pst (int et);           // zera se for negativo
int  exec_norm(int et);           // divisao por constante

// ----------------------------------------------------------------------------
// funcoes aritmeticas --------------------------------------------------------
// ----------------------------------------------------------------------------

int  exec_sqrt(int et);           // raiz quadrada
int  exec_atan(int et);           // arco tangente

// ----------------------------------------------------------------------------
// funcoes especiais para numeros complexos -----------------------------------
// ----------------------------------------------------------------------------

int  exec_real(int et);           // pega parte real
int  exec_imag(int et);           // pega parte imaginaria
int  exec_sqr2(int et);           // modulo ao quadrado
int  exec_absc(int et);           // valor absoluto de um numero complexo
int  exec_fase(int et);           // fase em radianos