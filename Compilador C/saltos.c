#include "variaveis.h"
#include "diretivas.h"
#include "data_use.h"
#include "labels.h"

void if_expstmt(char *lab)
{
    if (using_macro == 0) fprintf(f_asm, lab, pop_lab());
}

void if_expfull()
{
    if (using_macro == 0) fprintf(f_asm, "JMP L%dend\n@L%delse ", get_lab(), get_lab());
}

void if_expp(int et)
{
    load_check(et, 0);
    if (using_macro == 0) fprintf(f_asm, "JZ L%delse\n", push_lab(0));
    acc_ok = 0;
}

void while_stmt()
{
    if (using_macro == 0) fprintf(f_asm, "JMP L%d\n@L%dend ", pop_lab(), get_lab());
}

void exec_break()
{
    if (get_while() == 0) fprintf(stderr, "Atenção na linha %d: esse brake aí tá perdido!\n",  line_num+1);
    if (using_macro == 0) fprintf(f_asm , "JMP L%dend\n"                                    , get_while());
}

void while_expp()
{
    if (using_macro == 0) fprintf(f_asm, "@L%d ", push_lab(1));
}

void while_expexp(int et)
{
    load_check(et, 0);
    if (using_macro == 0) fprintf(f_asm, "JZ L%dend\n", get_lab());
    acc_ok = 0;
}
