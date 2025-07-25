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
int  exec_mod2(int et);           // modulo ao quadrado (colocar no parser)
int  exec_fase(int et);           // fase em radianos
int  exec_comp(int etr, int eti); // junta dois numeros reais pra fazer um complexo

// ----------------------------------------------------------------------------
// funcoes especiais para trabalho com vetores --------------------------------
// ----------------------------------------------------------------------------

int  exec_vtv  (int id1, int id2);                   // multiplicacao entre dois vetores
void exec_Mv   (int idy, int idM, int idv);          // multiplicacao de matriz por vetor
void exec_cv   (int idy, int et , int idv);          // multiplicacao do vetor por uma constante
void exec_apcb (int idy, int ida, int etc, int idb); // soma ponderada no segundo vetor
void exec_vvt  (int idM, int ida, int idb);          // produto externo entre dois vetores
void exec_Mmvvt(int idA, int idB, int ida, int idb); // subtracao de matriz com produto externo
void exec_cM   (int idA, int etc, int idM);          // produto entre constante e matriz
void exec_cI   (int idM, int etc);                   // gera matriz identidade
void exec_v0   (int idv);                            // gera vetor de zeros
void exec_cvin (int idv, int etc, int idp);          // le vetor de entrada
void exec_vout (int idp, int etc, int idv);          // escreve vetor pra saida
void exec_shift(int ida, int etb, int idc);          // shift register no vetor a (a tem q ser igual a c)