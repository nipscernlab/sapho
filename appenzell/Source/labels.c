#include "labels.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define NLABMAX 99999

int  l_count;
char l_name[NLABMAX][512];
int  l_val [NLABMAX];

int find_label(char *la)
{
	int i;

	for (i = 0; i < l_count; i++)
	{
		if (strcmp(la, l_name[i]) == 0) return l_val[i];
	}
	return -1;
}

void add_label(char *la, int val)
{
    if (l_count == NLABMAX)
    {
        fprintf(stderr, "Erro: numero de labels > %d", NLABMAX);
    }
    else
    {
        strcpy(l_name[l_count], la);
        l_val[l_count] = val;
        l_count++;
    }
}
