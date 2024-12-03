#include "variaveis.h"
#include "diretivas.h"
#include "data_use.h"
#include "data_assign.h"
#include "labels.h"
#include "oper.h"

// variaveis de estado para switch case
int switching = 0;
int case_cnt  = 0;
int swit_cnt  = 0;

// ----------------------------------------------------------------------------
// if/else --------------------------------------------------------------------
// ----------------------------------------------------------------------------

// inicio do if -> JZ pra saltar pro else (em caso negativo)
void if_exp(int et)
{
    load_check(et, 0);
    int n = push_lab(0);
    if (using_macro == 0) fprintf(f_asm, "JZ L%delse\n", n); // 0 -> if
    acc_ok = 0;
}

// cria label do final do if sem else
void if_stmt()
{
    int n = pop_lab();
    if (using_macro == 0) fprintf(f_asm, "@L%delse ", n);
}

// antes dos statments do else
void else_stmt()
{
    if (using_macro == 0) fprintf(f_asm, "JMP L%dend\n@L%delse ", get_lab(), get_lab());
}

// cria label do final do if/else
void if_fim()
{
    int n = pop_lab();
    if (using_macro == 0) fprintf(f_asm, "@L%dend ", n);
}

// ----------------------------------------------------------------------------
// while ----------------------------------------------------------------------
// ----------------------------------------------------------------------------

// final do while. Da um JMP para o inicio e cria um label pro final logo abaixo
void while_stmt()
{
    int n = pop_lab();
    if (using_macro == 0) fprintf(f_asm, "JMP L%d\n@L%dend ",n,n);
}

// da um JMP pro final do while
void exec_break()
{
    // checa se o break esta dentro de um while
    if (get_while() == 0) fprintf(stderr, "Erro na linha %d: esse brake aí tá perdido!\n",  line_num+1);
    if (using_macro == 0) fprintf(f_asm , "JMP L%dend\n"                                 , get_while());
}

// somente a palavra-chave while - gera um label nesse ponto
void while_expp()
{
    int n = push_lab(1);
    if (using_macro == 0) fprintf(f_asm, "@L%d ", n);
}

// executa o exp e cria um JZ pra ver se entra ou nao
void while_expexp(int et)
{
    load_check(et, 0);
    if (using_macro == 0) fprintf(f_asm, "JZ L%dend\n", get_lab());
    acc_ok = 0;
}

// ----------------------------------------------------------------------------
// switch/case ----------------------------------------------------------------
// ----------------------------------------------------------------------------

// executa case x: do switch case
void case_test(int id, int type)
{
    case_cnt++;
    if (using_macro == 0) fprintf(f_asm, "@sw_case_%d_%d ", swit_cnt, case_cnt);

    // gera o exp do valor do case
    int et1 = num2exp(id,type);
    // gera o exp da variavel de controle
    int et2 =  id2exp(find_var("switch_exp"));
    // faz operacao de comparacao
    oper_cmp(et1,et2,4);

    if (using_macro == 0) fprintf(f_asm, "JZ sw_case_%d_%d\n", swit_cnt, case_cnt+1);
    acc_ok = 0;
}

// executa default do switch case
void defaut_test()
{
    case_cnt++;
    if (using_macro == 0) fprintf(f_asm, "@sw_case_%d_%d ", swit_cnt, case_cnt);
}

// executa break do switch case
void switch_break()
{
    if (using_macro == 0) fprintf(f_asm, "JMP switch_end_%d\n", swit_cnt);
}

// inicio do switch case
void exec_switch(int et)
{
    if (switching == 1)
    fprintf(stderr, "Erro na linha %d: um switch/case dentro de outro? Você é uma pessoa confusa!\n",  line_num+1);

    // acha a variavel switch_exp (lexer)
    if (find_var("switch_exp") == -1) add_var("switch_exp");
    int id = find_var("switch_exp");
    // equivalente a declar_var
    v_type[id] = get_type(et);
    v_used[id] = 0;
    // equivalente a var_set
    load_check(et,0);
    if (using_macro == 0) fprintf(f_asm, "SET switch_exp\n");

    acc_ok     = 0;
    v_asgn[id] = 1;

    switching  = 1;
    case_cnt   = 0;
    swit_cnt++;
}

// fim do switch case
void end_switch()
{
    if (using_macro == 0) fprintf(f_asm, "@sw_case_%d_%d ", swit_cnt, case_cnt+1);
    if (using_macro == 0) fprintf(f_asm, "@switch_end_%d ", swit_cnt);
    switching = 0;
}
