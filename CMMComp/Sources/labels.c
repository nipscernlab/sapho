// implementa a pilha para labels em instrucoes de salto

#include "..\Headers\labels.h"
#include <stdio.h>

#define LABMAX 99999

int stk_ind = 0;
int lab_cnt = 0;
int lab_stk[LABMAX];
int lab_typ[LABMAX]; // 0 para if/else e 1 para while

int push_lab(int typ)
{
    lab_cnt++;

    if (lab_cnt == LABMAX)
    {
        fprintf (stderr, "Erro: Aumente o número de Labels permitidos (if, while). Atual = %d\n", LABMAX);
        return 0;
    }

    lab_stk[stk_ind] = lab_cnt;
    lab_typ[stk_ind] = typ;
    stk_ind++;
    return lab_cnt;
}

int pop_lab()
{
    stk_ind--;
    return lab_stk[stk_ind];
}

int get_lab()
{
    return lab_stk[stk_ind-1];
}

// pega o indice do while mais recente na pilha (ou 0 se nao existir)
int get_while()
{
    int i = stk_ind-1;
    while ((lab_typ[i] != 1) && (i >= 0)) i--;
    return i+1;
}

// pega o indice do if/else mais recente na pilha (ou 0 se nao existir)
int get_if()
{
    int i = stk_ind-1;
    while ((lab_typ[i] != 0) && (i >= 0)) i--;
    return i+1;
}
