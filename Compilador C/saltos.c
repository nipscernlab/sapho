#include "variaveis.h"
#include "diretivas.h"
#include "data_use.h"
#include "labels.h"

// cria label do final do if/else
void if_expstmt(char *lab)
{
    if (using_macro == 0) fprintf(f_asm, lab, pop_lab());
}

// termina o if com um JMP pro final e cria o label pro else logo abaixo
void if_expfull()
{
    if (using_macro == 0) fprintf(f_asm, "JMP L%dend\n@L%delse ", get_lab(), get_lab());
}

// executa o argumento e gera JZ pra saltar pro else (em caso negativo)
void if_expp(int et)
{
    load_check(et, 0);
    if (using_macro == 0) fprintf(f_asm, "JZ L%delse\n", push_lab(0));
    acc_ok = 0;
}

// final do while. Da um JMP para o inicio e cria um label pro final logo abaixo
void while_stmt()
{
    if (using_macro == 0) fprintf(f_asm, "JMP L%d\n@L%dend ", pop_lab(), get_lab());
}

// da um JMP pro final do while
void exec_break()
{
    if (get_while() == 0) fprintf(stderr, "Aten��o na linha %d: esse brake a� t� perdido!\n",  line_num+1);
    if (using_macro == 0) fprintf(f_asm , "JMP L%dend\n"                                    , get_while());
}

// somente a palavra-chave while - gera um label nesse ponto
void while_expp()
{
    if (using_macro == 0) fprintf(f_asm, "@L%d ", push_lab(1));
}

// executa o exp e cria um JZ pra ver se entra ou nao
void while_expexp(int et)
{
    load_check(et, 0);
    if (using_macro == 0) fprintf(f_asm, "JZ L%dend\n", get_lab());
    acc_ok = 0;
}
