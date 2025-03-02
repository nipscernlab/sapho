#include <stdio.h>

// ----------------------------------------------------------------------------
// Globais do compilador ------------------------------------------------------
// ----------------------------------------------------------------------------

extern FILE *f_asm;              // arquivo .asm a ser gerado pelo compilador

extern char dir_macro[1024];     // diretorio das macros
extern char dir_soft [1024];     // diretorio software

extern int line_num;             // numero da linha sendo parseada
extern int num_ins;              // numero de instrucoes parseadas

void add_instr(char *inst, ...); // cadastra as instrucoes assembly

// ----------------------------------------------------------------------------
// Globais da simulacao -------------------------------------------------------
// ----------------------------------------------------------------------------

extern FILE *f_log;          // arquivo de log com algumas informacoes do projeto
extern FILE *f_lin;          // memoria no pc.v que passa do endereco da instrucao para a linha em cmm

extern char dir_tmp  [1024]; // diretorio Temp