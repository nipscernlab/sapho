#include <stdio.h>

extern FILE *f_asm; // arquivo .asm a ser gerado pelo compilador
extern FILE *f_log; // arquivo de log com algumas informacoes do projeto
extern FILE *f_lin; // memoria no pc.v que passa do endereco da instrucao para a linha em cmm

extern char pr_name  [128 ]; // nome do processador
extern char dir_macro[1024]; // diretorio das macros
extern char dir_soft [1024]; // diretorio software
extern char dir_tmp  [1024]; // diretorio Temp

extern int prtype;      // 0 -> processador fonto fixo, 1 -> processador ponto flutuante
extern int using_macro; // se estiver lendo uma macro, nao deve escrever o assembler durante o parse
extern int line_num;    // numero da linha sendo parseada
extern int itr_ok;      // se ja usou ou nao interrupcao
extern int num_ins;     // numero de instrucoes do parse

void  exec_diretivas(char *dir, int id , int t  ); // t ajuda nas expressoes extras
void  use_macro     (char *file_name, int global);
void  end_macro     ();
void  add_instr     (char *inst, ...) ;
void use_inter      ();