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

void  sim_reg    (char *va);                        // registra         variavel
char* sim_name   (int   i);                         // pega nome     da variavel
int   sim_addr   (int   i);                         // pega endereco da variavel
int   sim_type   (int   i);                         // pega tipo     da variavel
int   sim_cnt    ();                                // pega numero   de variaveis registradas