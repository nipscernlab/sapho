// ----------------------------------------------------------------------------
// funcoes e variaveis na criacao e utilizacao de macros ----------------------
// ----------------------------------------------------------------------------

extern int using_macro;                           // se estiver lendo uma macro, nao deve escrever o assembler durante o parse

extern int fsqrti;                                // vai usar macro de raiz quadrada
extern int fatani;                                // vai usar macro de arco tangente

// uso de macros do usuario ---------------------------------------------------

void dire_macro(int ids, int global, int id_num); // habilita o uso de macro pelo usuario
void dire_maend();                                // ponto de termino da macro

// uso de macros pre-definidas ------------------------------------------------

void mac_geni(char *fasm);                        // adiciona todo o codigo necessario no .asm