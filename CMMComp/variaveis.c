// implementacao da tabela de indentificadores
// e suas rotinas auxiliares

#include "variaveis.h"
#include "t2t.h"
#include "diretivas.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

int v_count = 0; // guarda o tamanho da tabela

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
                fprintf (stdout, "Atenção: variável global %s não está sendo usada. Economize memória!\n", v_name[i]);
            else
                fprintf (stdout, "Atenção: variável %s na função %s não está sendo usada. Economize memória!\n", rem_fname(v_name[i], v_name[v_fnid[i]]), v_name[v_fnid[i]]);
        }

        // checa se a funcao foi declarada e nao foi usada
        if (((v_type[i] == 5) || (v_type[i] == 6) || (v_type[i] == 7)) && v_used[i] == 0)
            fprintf (stdout, "Atenção: função %s não está sendo usada. Economize memória!\n", v_name[i]);
    }
}

// tira o nome da funcao da variavel
char* rem_fname(char *var, char *fname)
{
    int    ind = 0;
    while (var[ind] == fname[ind]) ind++;
    // se ind != strlen(fname) eh pq a variavel nao
    // contem todo o nome da funcao no comeco
    // entao melhor nao remover nada (deve ser global)
    return (ind == strlen(fname)) ? var + ind + 1 : var; // o +1 eh pra tirar o '_'
}

// usado quando o lexer acha um ID
int exec_id(char *text)
{
    // testes com numeros complexos -------------------------------------------
    if (strcmp(text,"i") == 0)
        fprintf (stderr, "Erro na linha %d: símbolo i é reservado para indicar a parte imaginária de uma constante complexa.\n", line_num+1);
    // fim dos teste ----------------------------------------------------------

    char var_name[64];

    if (find_var(text) == -1)                                     // primeiro ve se tem uma variavel global
    {
            strcpy  (var_name, fname);
        if (strcmp  (var_name, "") != 0) strcat (var_name, "_");  // se nao, coloca o nome da funcao atual antes
            strcat  (var_name, text);
        if (find_var(var_name)    == -1) add_var(var_name);       // cria a variavel local, caso ela ainda nao exista
    }
    else
    {
        strcpy(var_name, text);
    }
    return find_var(var_name);
}

// usado quando o lexer acha uma constante int ou float
int exec_num(char *text)
{
    float f = (atof(text) < 0) ? -atof(text) : atof(text); // valor absoluto do num, em float
    float s =  pow(2,nbmant-1)*pow(2,-pow(2,nbexpo-1));    // menor valor permitido pra float = 2^(m-1)*2^(-(2^(e-1)))

    // se o numero for menor do que o menor permitido pra float, printa um erro
    if ((f < s) && (f != 0))
        fprintf (stderr, "Erro na linha %d: o menor número que pode ser representado é 2^(%d)!\n", line_num+1, (int)(nbmant-1 -pow(2,nbexpo-1)));

    if (find_var(text) == -1) add_var(text);

    int id = find_var(text);

    v_isco[id] = 1;

    return id;
}

// usado quando o lexer acha uma constante comp
int exec_cnum(char *text)
{
    // remova espacos em branco -----------------------------------------------

    int i = 0, j = 0;
    char temp[strlen(text) + 1];
    strcpy(temp, text);
    while (temp[i] != '\0')
    {
        while (temp[i] == ' ') i++;
        text[j] = temp[i];
        i++;
        j++;
    }
    text[j] = '\0';

    // adiciona na tabela -----------------------------------------------------

    if (find_var(text) == -1) add_var(text);
    return find_var(text);
}
