// implementacao da tabela de indentificadores
// e suas rotinas auxiliares

#include "variaveis.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int  v_count = 0; // guarda o tamanho da tabela

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
        fprintf (stderr, "Erro: Aumente o número de variáveis permitidas. Atual = %d\n", NVARMAX);
        exit(1);
    }
    else
    {
        strcpy(v_name[v_count], var);
        v_count++;
    }
}

// checa quais variaveis e funcoes foram usadas (no final do parse)
void check_var()
{
    int i;
    // varre toda a tabela de variaveis
    for (i = 0; i < v_count; i++)
    {
        // checa se variavel foi declarada e nao foi usada ...
        if (((v_type[i] == 1) || (v_type[i] == 2) || (v_type[i] == 3)) && (v_used[i] == 0))
        {
            // checa se eh ou nao global
            if (strcmp(v_name[v_fnid[i]], "") == 0)
                fprintf (stderr, "Atenção: variável global %s não está sendo usada. Economize memória!\n", v_name[i]);
            else
                fprintf (stderr, "Atenção: variável %s na função %s não está sendo usada. Economize memória!\n", rem_fname(v_name[i], v_name[v_fnid[i]]), v_name[v_fnid[i]]);
        }

        // checa se a funcao foi declarada e nao foi usada
        if (((v_type[i] == 5) || (v_type[i] == 6) || (v_type[i] == 7)) && v_used[i] == 0)
            fprintf (stderr, "Atenção: função %s não está sendo usada. Economize memória!\n", v_name[i]);
    }
}

// tira o nome da funcao da variavel
char* rem_fname(char *var, char *fname)
{
    int    ind = 0;
    while (var[ind] == fname[ind]) ind++;
    return var + ind + 1; // o +1 eh pra tirar o '_'
}

int exec_id(char *text)
{
    char var_name[64];
    if (find_var(text) == -1)
    {
            strcpy  (var_name, fname);
        if (strcmp  (var_name, "") != 0) strcat (var_name, "_");
            strcat  (var_name, text);
        if (find_var(var_name)    == -1) add_var(var_name);
    }
    else
    {
        strcpy(var_name, text);
    }
    return find_var(var_name);
}

int exec_num(char *text)
{
    if (find_var(text) == -1) add_var(text);
    return find_var(text);
}
