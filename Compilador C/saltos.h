#ifndef SALTOS_H_INCLUDED
#define SALTOS_H_INCLUDED

// if/else --------------------------------------------------------------------

void  if_expp   (int    et); // executa o argumento e gera JZ pra saltar pro else (em caso negativo)
void  if_expfull();          // termina o if com um JMP pro final e cria o label pro else logo abaixo
void  if_expstmt(char *lab); // cria label do final do if/else

// while/break ----------------------------------------------------------------

void  while_expp();          // somente a palavra-chave while - gera um label nesse ponto
void  while_expexp(int  et); // executa o exp e cria um JZ pra ver se entra ou nao
void  while_stmt();          // final do while. Da um JMP para o inicio e cria um label pro final logo abaixo
void  exec_break();          // da um JMP pro final do while

#endif // SALTOS_H_INCLUDED
