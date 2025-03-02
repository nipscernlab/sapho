// ----------------------------------------------------------------------------
// Reune funcoes e variaveis de estado acionadas por diretivas ----------------
// ----------------------------------------------------------------------------

extern char pr_name[128 ];                    // nome do processador
extern int  prtype;                           // 0 -> processador fonto fixo, 1 -> processador ponto flutuante

extern int  using_macro;                      // se estiver lendo uma macro, nao deve escrever o assembler durante o parse
extern int  itr_ok;                           // se ja usou ou nao interrupcao

void  exec_dire(char *dir, int id, int t   ); // atualiza as variaveis de estado de diretivas
void  use_macro(char *file_name, int global); // habilita o uso de macro pelo usuario
void  end_macro();                            // ponto de termino da macro
void  use_inter();                            // ponto de inicio da interrupcao