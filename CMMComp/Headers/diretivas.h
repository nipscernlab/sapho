#ifndef DIRETIVAS_H_INCLUDED
#define DIRETIVAS_H_INCLUDED

#include <stdio.h>

FILE *f_asm;
char pr_name[128];

int prtype;      // 0 -> processador fonto fixo, 1 -> processador ponto flutuante
int using_macro; // se estiver lendo uma macro, nao deve escrever o assembler durante o parse
int line_num;    // numero da linha sendo parseada
int itr_ok;      // se ja usou ou nao interrupcao

void  exec_diretivas(char *dir, int id , int t     ); // t ajuda nas expressoes extras
void  use_macro     (char *file_name, int global);
void  end_macro     ();
void use_inter      ();

#endif // DIRETIVAS_H_INCLUDED
