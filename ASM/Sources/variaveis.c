#include "..\Headers\variaveis.h"
#include "..\Headers\t2t.h"
#include "..\Headers\veri_comp.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int  v_count = 0;
char v_name[NVARMAX][512];
int  v_val [NVARMAX];

// ve se uma variavel ja foi usado
// se sim, pega o indice na tabela
// se nao, retorna -1
int var_find(char *val)
{
	int i, ind = -1;

	for (i = 0; i < v_count; i++)
		if (strcmp(val, v_name[i]) == 0)
		{
			ind = i;
			break;
		}
	return ind;
}

// addiciona uma nova variavel na tabela
// se o operando for uma constante, converte seu valor para binario ...
void var_add(char *var, int is_const)
{
    if (v_count == NVARMAX)
    {
        fprintf(stderr, "Erro: número de variáveis > %d", NVARMAX);
        return;
    }

    // transforma char *var pra int val
    int val;
    switch(is_const)
    {
        case 0: val = 0;         break; // nao eh constante
        case 1: val = atoi(var); break; // constante tipo int
        case 2: val = f2mf(var); break; // constante tipo float
    }

    strcpy(v_name [v_count], var);
    v_val [v_count]        = val ;
    v_count++;
}

void inc_vcont(int val)
{
    v_count += val;
}

void var_reset()
{
    v_count = 0;
}

int var_get(char *var, char *val)
{
    // abre o arquivo de log
    char path[1024];
    sprintf(path, "%s/app_log.txt", temp_dir);
    FILE *input = fopen(path, "r");
    
    char linha[1001];
    char nome[128];
    while (fgets(linha, sizeof(linha), input)) {
        if (sscanf(linha, "%s %s", nome, val) == 2) {
            if (strcmp(nome, var) == 0) {
                fclose(input);
                return 1; // encontrou a variavel
            }
        }
    }  
    fclose(input);
    return 0; // nao encontrou a variavel
}
