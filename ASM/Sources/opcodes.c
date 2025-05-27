// ----------------------------------------------------------------------------
// tratamento de opcodes em assembly ------------------------------------------
// ----------------------------------------------------------------------------

#define NMNEMAX 999999 // usar array dinamico

// includes globais
#include <string.h>

// ----------------------------------------------------------------------------
// variaveis locais -----------------------------------------------------------
// ----------------------------------------------------------------------------

int  m_count;             // contador de opcodes (parameter)
char m_name[NMNEMAX][64]; // nome     do opcode  (parameter)

// ----------------------------------------------------------------------------
// funcoes auxiliares ---------------------------------------------------------
// ----------------------------------------------------------------------------

// ve se um opcode ja foi parametrizado
// se sim, pega o indice na tabela
// se nao, retorna -1
int find_opc(char *val)
{
	int ind = -1;

	for (int i = 0; i < m_count; i++)
	{
		if (strcmp(val, m_name[i]) == 0)
		{
			ind = i; break;
		}
	}

	return ind;
}

// ----------------------------------------------------------------------------
// funcoes de interface -------------------------------------------------------
// ----------------------------------------------------------------------------

char* opc_get(int i){return m_name[i];} // pega parameter
int   opc_cnt(     ){return m_count  ;} // pega numero de parameters cadastrados

// addiciona um novo opcode na tabela
void opc_add(char *mne)
{
    if ((find_opc(mne) != -1) || (strcmp(mne,"") == 0)) return;

    strcpy(m_name[m_count], mne);
    m_count++;
}