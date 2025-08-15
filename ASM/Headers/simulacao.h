// ----------------------------------------------------------------------------
// rotinas para simulacao com o iverilog/gtkwave ------------------------------
// ----------------------------------------------------------------------------

// acessa parametros da simulacao ---------------------------------------------

int   sim_clk    ();                                // pega frequencia do clock de simulacao
int   sim_clk_num();                                // pega numero de clocks a simular
int   sim_multi  ();                                // pega tipo de simulacao (um proc ou multicore)

// acoes com o arquivo de traducao --------------------------------------------

void  sim_init   (int   clk, int clk_n, int s_typ); // cria arquivo de traducao
void  sim_add    (char *opc, char *opr);            // adiciona opcode e operando
void  sim_set_fim(int   fim);                       // define  endereco de @fim
int   sim_get_fim();                                // retorna endereco de @fim
void  sim_finish ();                                // fecha arquivo de traducao

// acoes com variaveis do usuario ---------------------------------------------

void  sim_regi   (char *va);                        // registra         variavel
char* sim_name   (int    i);                        // pega nome     da variavel
int   sim_addr   (int    i);                        // pega endereco da variavel
int   sim_type   (int    i);                        // pega tipo     da variavel
int   sim_cont   (        );                        // pega numero   de variaveis registradas

// acoes com arrays do usuario ---------------------------------------------

void  sim_regi_arr(char *va);                       // registra         array
char* sim_name_arr(int    i);                       // pega nome     do array
int   sim_addr_arr(int    i);                       // pega endereco do array
int   sim_type_arr(int    i);                       // pega tipo     do array
int   sim_cont_arr(        );                       // pega numero   de arrays registradas
int   sim_size_arr(int    i);                       // pega tamanho do array