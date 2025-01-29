#ifndef DIRETIVAS_H_INCLUDED
#define DIRETIVAS_H_INCLUDED

#include <stdio.h>

FILE *f_asm;
FILE *f_log;
FILE *f_lin;
FILE *f_ltp;
char pr_name[128];

char dir_macro[1024];
char dir_soft [1024];
char dir_tmp  [1024];

int prtype;      // 0 -> processador fonto fixo, 1 -> processador ponto flutuante
int using_macro; // se estiver lendo uma macro, nao deve escrever o assembler durante o parse
int line_num;    // numero da linha sendo parseada
int itr_ok;      // se ja usou ou nao interrupcao
int num_ins;     // numero de instrucoes do parse

void  exec_diretivas(char *dir, int id , int t     ); // t ajuda nas expressoes extras
void  use_macro     (char *file_name, int global);
void  end_macro     ();
int    is_macro     ();
void use_inter      ();

char *itob(int x, int w);

#endif // DIRETIVAS_H_INCLUDED
