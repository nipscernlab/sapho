#include "variaveis.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NVARMAX 999999

int  v_count = 0;
char v_name[NVARMAX][512];
int  v_val [NVARMAX];

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

void add_var(char *var, int val)
{
    if (v_count == NVARMAX)
    {
        fprintf(stderr, "Erro: numero de variaveis > %d", NVARMAX);
    }
    else
    {
        strcpy(v_name[v_count], var);
        v_val[v_count] = val;
        v_count++;
    }
}

void inc_vcont(int val)
{
    v_count += val;
}

void var_reset()
{
    v_count = 0;
}
