// ----------------------------------------------------------------------------
// Reune funcoes e variaveis de estado acionadas por diretivas ----------------
// ----------------------------------------------------------------------------

extern int  using_macro;                         // se estiver lendo uma macro, nao deve escrever o assembler durante o parse
extern int  itr_ok;                              // se ja usou ou nao interrupcao

void exec_dire(char *dir, int id, int t);        // atualiza as variaveis de estado de diretivas
void use_macro(int ids, int global, int id_num); // habilita o uso de macro pelo usuario
void end_macro();                                // ponto de termino da macro
void use_inter();                                // ponto de inicio da interrupcao