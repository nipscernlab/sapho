// ----------------------------------------------------------------------------
// funcoes e variaveis na criacao e utilizacao de macros ----------------------
// ----------------------------------------------------------------------------

extern int using_macro;                           // se estiver lendo uma macro, nao deve escrever o assembler durante o parse

extern int idiv;                                  // vai fazer divisao de inteiro por software
extern int imod;                                  // vai fazer resto   da divisao por software
extern int finv;                                  // vai fazer o inverso de um float
extern int fsqrt;                                 // vai usar macro de raiz quadrada
extern int fatan;                                 // vai usar macro de arco tangente
extern int fsin;                                  // vai usar macro de seno e cosseno

// uso de macros do usuario ---------------------------------------------------

void dire_macro(int ids, int global, int id_num); // habilita o uso de macro pelo usuario
void dire_maend();                                // ponto de termino da macro

// uso de macros pre-definidas ------------------------------------------------

void mac_geni(char *fasm);                        // adiciona todo o codigo necessario no .asm