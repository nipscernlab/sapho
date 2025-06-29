// ----------------------------------------------------------------------------
// funcoes e variaveis na criacao e utilizacao de macros ----------------------
// ----------------------------------------------------------------------------

// uso de macros do usuario ---------------------------------------------------

extern int mac_using;                          // se estiver lendo uma macro, nao deve escrever o assembler durante o parse

void mac_use(int ids, int global, int id_num); // habilita o uso de macro pelo usuario
void mac_end();                                // ponto de termino da macro

// uso de macros pre-definidas ------------------------------------------------

void mac_add (char *name);                     // adiciona uma macro pre-definida
void mac_gera(char *fasm);                     // adiciona todo o codigo necessario no .asm