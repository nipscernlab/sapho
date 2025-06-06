// ----------------------------------------------------------------------------
// biblioteca padrao do sapho -------------------------------------------------
// ----------------------------------------------------------------------------

// essas rotinas devem ser chamadas pelo .y (parser)

// ----------------------------------------------------------------------------
// entrada e saida ------------------------------------------------------------
// ----------------------------------------------------------------------------

int  exec_in (int et);            //  input de dados
void exec_out(int et1, int et2);  // output de dados

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
int  exec_sin (int et);           // seno
int  exec_cos (int et);           // cosseno

// ----------------------------------------------------------------------------
// funcoes especiais para numeros complexos -----------------------------------
// ----------------------------------------------------------------------------

int  exec_real(int et);           // pega parte real
int  exec_imag(int et);           // pega parte imaginaria
int  exec_sqr2(int et);           // modulo ao quadrado (colocar no parser)
int  exec_fase(int et);           // fase em radianos
int  exec_comp(int etr, int eti); // junta dois numeros reais pra fazer um complexo