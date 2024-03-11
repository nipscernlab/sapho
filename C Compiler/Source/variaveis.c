#include "variaveis.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int  v_count = 0;

int find_var(char *val)
{
	int i, ind = -1;

	for (i = 0; i < v_count; i++)
	{
		if (strcmp(val, v_name[i]) == 0)
		{
			ind = i;
			break;
		}
	}
	return ind;
}

void add_var(char *var)
{
    if (v_count == NVARMAX)
    {
        fprintf (stderr, "Erro: Aumente num. de variaveis permitidas. Atual = %d\n", NVARMAX);
        exit(1);
    }
    else
    {
        strcpy(v_name[v_count], var);
        v_count++;
    }
}

void check_var()
{
    int i;
    for (i = 0; i < v_count; i++)
    {
        if (((v_type[i] == 1) || (v_type[i] == 2)) && (v_used[i] == 0))
        {

            if (strcmp(v_name[v_fnid[i]], "") == 0)
                fprintf (stderr, "Atencao: variavel global %s nao esta sendo usada. Economize memoria!\n", rem_fname(v_name[i], v_name[v_fnid[i]]));
            else
                fprintf (stderr, "Atencao: variavel %s na funcao %s nao esta sendo usada. Economize memoria!\n", rem_fname(v_name[i], v_name[v_fnid[i]]), v_name[v_fnid[i]]);
        }

        if (((v_type[i] == 3) || (v_type[i] == 4) || (v_type[i] == 5)) && v_used[i] == 0)
            fprintf (stderr, "Atencao: funcao %s nao esta sendo usada. Economize memoria!\n", v_name[i]);
    }
}

char *rem_fname(char *var, char *fname)
{
    //fprintf(stderr, "var = %s, fname = %s\n", var, fname);
    int ind = 0;
    while (var[ind] == fname[ind])  ind++;
    return var + ind;
}
