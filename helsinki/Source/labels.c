#include "labels.h"
#include <stdio.h>
#include <stdlib.h>

#define LABMAX 99999

int lab_cnt = 0;
int lab_stk[LABMAX];
int stk_ind = 0;

int push_lab()
{
    lab_cnt++;

    if (lab_cnt == LABMAX)
    {
        fprintf (stderr, "Erro: Aumente num. de Labels permitidos. Atual = %d\n", LABMAX);
        exit(1);
    }

    lab_stk[stk_ind] = lab_cnt;
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
