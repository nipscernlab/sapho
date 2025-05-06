// ----------------------------------------------------------------------------
// rotinas e variaveis globais para gerar os arquivos .mif das memorias ... ---
// a medida que o lex vai escaneando o .asm -----------------------------------
// ----------------------------------------------------------------------------

// variaveis globais ----------------------------------------------------------

// diretorios de acesso aos arquivos .mif e .v
extern char proc_dir[1024];    // diretorio do processador
extern char temp_dir[1024];    // diretorio da pasta Tmp
extern char  hdl_dir[1024];    // diretorio da pasta HDL

// guarda os valores das diretivas
extern char prname  [128 ];    // nome do processador
extern int  nubits;            // tamanho da palavra da ula
extern int  nbmant;            // numero de bits da mantissa
extern int  nbexpo;            // numero de bits do expoente
extern int  ddepth;            // tamanho da pilha de dados
extern int  sdepth;            // tamanho da pilha de subrotinas
extern int  nuioin;            // numero de portas de entrada
extern int  nuioou;            // numero de portas de saida
extern int  nugain;            // constante de divisao
extern int  fftsiz;            // tamanho da fft (em bits)

// funcoes usadas no lexer (.l) -----------------------------------------------

int  eval_get   (char *fname, char *var, char *val);
void eval_init  (int   clk  , int clk_n, int s_typ);
void eval_direct(int   next_state);
void eval_opcode(int   op   , int next_state, char *text, char *nome);
void eval_opernd(char *va   , int is_const);
void eval_finish();