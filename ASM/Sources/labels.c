#include "..\Headers\labels.h"
#include "..\Headers\veri_comp.h"
#include "..\Headers\eval.h"

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
        fprintf(stderr, "Erro: n�mero de labels > %d", NLABMAX);
    }
    else
    {
        strcpy(l_name[l_count], la);
        l_val[l_count] = val;
        l_count++;
    }
}

void lab_reg()
{
    // abre o arquivo de log
    char path[1024];
    sprintf(path, "%s/app_log.txt", temp_dir);
    FILE *input = fopen(path, "r");
    
    char linha[1001];
    char nome[128];
    int val;
    while (fgets(linha, sizeof(linha), input)) {
        if (sscanf(linha, "@%s %d", nome, &val) == 2) {
            add_label(nome, val); // cadastra label
            if (strcmp(nome,"fim") == 0) fim_addr = val;
        }
    }  
    fclose(input);
}