#include <stdio.h>

// ----------------------------------------------------------------------------
// Globais do compilador ------------------------------------------------------
// ----------------------------------------------------------------------------

// variaveis globais ----------------------------------------------------------

extern FILE *f_asm;              // arquivo .asm a ser gerado pelo compilador

extern char pr_name[128];        // nome do processador
extern int  prtype;              // 0 -> processador fonto fixo, 1 -> processador ponto flutuante
extern int  nbmant;              // numero de bits de mantissa
extern int  nbexpo;              // numero de bits de expoente

extern char dir_macro[1024];     // diretorio Macros
extern char dir_soft [1024];     // diretorio Software

// variaveis de estado --------------------------------------------------------

extern int  acc_ok;              // 0 -> acc vazio (use LOAD), 1 -> acc carregado (use PLD)
extern int  line_num;            // numero da linha sendo parseada
extern int  num_ins ;            // numero de instrucoes parseadas

// funcoes globais ------------------------------------------------------------

void add_instr(char *inst, ...); // cadastra as instrucoes assembly
void add_sinst(int type, char *inst, ...);

// ----------------------------------------------------------------------------
// Globais da simulacao -------------------------------------------------------
// ----------------------------------------------------------------------------

extern FILE *f_log;              // arquivo de log com algumas informacoes do projeto
extern FILE *f_lin;              // memoria no pc.v que passa do endereco da instrucao para a linha em cmm

extern char dir_tmp[1024];       // diretorio Temp