#include "..\Headers\opcodes.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NMNEMAX 999999           // o preco por nao usar C++ com o flex e bison

int  m_count;             // contador de mnem
char m_name[NMNEMAX][64]; // nome dos mnem

// ve se um mnemonico ja foi usado
// se sim, pega o indice na tabela
// se nao, retorna -1
int find_mne(char *val)
{
	int i, ind = -1;

	for (i = 0; i < m_count; i++)
	{
		if (strcmp(val, m_name[i]) == 0)
		{
			ind = i;
			break;
		}
	}
	return ind;
}

// addiciona um novo mnemonico na tabela
void opc_add(char *mne)
{
    if ((find_mne(mne) != -1) || (strcmp(mne,"") == 0)) return;

    strcpy(m_name[m_count], mne);
    m_count++;
}

int opc_cnt()
{
	return m_count;
}

char* opc_get(int i)
{
	return m_name[i];
}
