// ----------------------------------------------------------------------------
// funcoes e variaveis globais do compilador ----------------------------------
// ----------------------------------------------------------------------------

#include <stdio.h>

// ----------------------------------------------------------------------------
// variaveis globais ----------------------------------------------------------
// ----------------------------------------------------------------------------

extern FILE *yyin ;              // precisa linkar o .cmm a esse ponteiro do bison
extern FILE *f_asm;              // arquivo .asm a ser gerado pelo compilador
extern FILE *f_log;              // arquivo de log com algumas informacoes do projeto
extern FILE *f_lin;              // memoria no pc.v para ver as instrucoes no gtkwave

extern char dir_macro[1024];     // diretorio Macros
extern char dir_soft [1024];     // diretorio Software
extern char dir_tmp  [1024];     // diretorio Temp

// ----------------------------------------------------------------------------
// variaveis de estado --------------------------------------------------------
// ----------------------------------------------------------------------------

extern int  acc_ok;              // 0 -> acc vazio (use LOD), 1 -> acc carregado (use P_LOD)
extern int  line_num;            // numero da linha sendo parseada
extern int  num_ins ;            // numero de instrucoes parseadas (sem macros finais)

// ----------------------------------------------------------------------------
// funcoes de interface -------------------------------------------------------
// ----------------------------------------------------------------------------

// inicio e fim do parse
void parse_init(char *prname, char *d_proc, char *d_macro, char *d_tmp);
void parse_end (char *prname, char *d_proc);

// cadastra as instrucoes assembly
void add_instr(char *inst, ...);           // padrao
void add_sinst(int type, char *inst, ...); // especiais