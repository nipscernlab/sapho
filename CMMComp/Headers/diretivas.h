// ----------------------------------------------------------------------------
// reune funcoes e variaveis de estado acionadas por diretivas ----------------
// ----------------------------------------------------------------------------

extern char prname[128];                           // nome do processador
extern int  nbmant;                                // numero de bits de mantissa
extern int  nbexpo;                                // numero de bits de expoente

extern int using_macro;                            // se estiver lendo uma macro, nao deve escrever o assembler durante o parse
extern int itr_ok;                                 // se ja usou ou nao interrupcao

void dire_exec (char *dir, int id    , int t     ); // atualiza as variaveis de estado de diretivas
void dire_macro(int   ids, int global, int id_num); // habilita o uso de macro pelo usuario
void dire_maend();                                  // ponto de termino da macro
void dire_inter();                                  // ponto de inicio da interrupcao