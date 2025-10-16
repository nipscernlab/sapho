// ----------------------------------------------------------------------------
// tratamento de opcodes em assembly ------------------------------------------
// ----------------------------------------------------------------------------

#define NMNEMAX 999999 // usar array dinamico

// includes globais
#include <string.h>
#include  <stdio.h>

// ----------------------------------------------------------------------------
// variaveis locais -----------------------------------------------------------
// ----------------------------------------------------------------------------

int  m_count = 0;         // contador de opcodes (parameter)
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
	// se opcode ainda nao foi cadastrado...
	if ((find_opc(mne) == -1) && (strcmp(mne,"") != 0))
	{
		// --------------------------------------------------------------------
		// primeiro gera uma info dizendo que o recurso sera usado ------------
		// --------------------------------------------------------------------

		if (strcmp(mne, "LDI") == 0)
		{
			// se ainda nao tem o opcode STI, escreve a info
			if (find_opc("STI") == -1)
			{
				printf("Info: adding array handling\n");
			}
		}

		if (strcmp(mne, "ILI") == 0)
		{
			// se ainda nao tem o opcode ISI, escreve a info
			if (find_opc("ISI") == -1)
			{
				printf("Info: adding bit-reverse array index (FFT)\n");
			}
		}

		if (strcmp(mne, "STI") == 0)
		{
			// se ainda nao tem o opcode LDI, escreve a info
			if (find_opc("LDI") == -1)
			{
				printf("Info: adding array handling\n");
			}
		}

		if (strcmp(mne, "ISI") == 0)
		{
			// se ainda nao tem o opcode ILI, escreve a info
			if (find_opc("ILI") == -1)
			{
				printf("Info: adding bit-reverse array index (FFT)\n");
			}
		}

		if (strcmp(mne, "INN") == 0)
		{
			// se ainda nao tem o opcode P_INN, escreve a info
			if (find_opc("P_INN") == -1)
			{
				printf("Info: adding data input handling\n");
			}
		}

		if (strcmp(mne, "P_INN") == 0)
		{
			// se ainda nao tem o opcode INN, escreve a info
			if (find_opc("INN") == -1)
			{
				printf("Info: adding data input handling\n");
			}
		}

		if (strcmp(mne, "OUT") == 0)
		{
			printf("Info: adding data output handling\n");
		}

		if (strcmp(mne, "CAL") == 0)
		{
			printf("Info: adding stack memory for function calls\n");
		}

		if (strcmp(mne, "ADD") == 0)
		{
			// se ainda nao tem o opcode S_ADD, escreve a info
			if (find_opc("S_ADD") == -1)
			{
				printf("Info: ULA resource -> integer adder\n");
			}
		}

		if (strcmp(mne, "S_ADD") == 0)
		{
			// se ainda nao tem o opcode ADD, escreve a info
			if (find_opc("ADD") == -1)
			{
				printf("Info: ULA resource -> integer adder\n");
			}
		}

		if (strcmp(mne, "F_ADD") == 0)
		{
			// se ainda nao tem o opcode SF_ADD, escreve a info
			if (find_opc("SF_ADD") == -1)
			{
				printf("Info: ULA resource -> float-point adder\n");
			}
		}

		if (strcmp(mne, "SF_ADD") == 0)
		{
			// se ainda nao tem o opcode F_ADD, escreve a info
			if (find_opc("F_ADD") == -1)
			{
				printf("Info: ULA resource -> float-point adder\n");
			}
		}

		if (strcmp(mne, "MLT") == 0)
		{
			// se ainda nao tem o opcode S_MLT, escreve a info
			if (find_opc("S_MLT") == -1)
			{
				printf("Info: ULA resource -> integer multiplier\n");
			}
		}

		if (strcmp(mne, "S_MLT") == 0)
		{
			// se ainda nao tem o opcode MLT, escreve a info
			if (find_opc("MLT") == -1)
			{
				printf("Info: ULA resource -> integer multiplier\n");
			}
		}

		if (strcmp(mne, "F_MLT") == 0)
		{
			// se ainda nao tem o opcode SF_MLT, escreve a info
			if (find_opc("SF_MLT") == -1)
			{
				printf("Info: ULA resource -> float-point multiplier\n");
			}
		}

		if (strcmp(mne, "SF_MLT") == 0)
		{
			// se ainda nao tem o opcode F_MLT, escreve a info
			if (find_opc("F_MLT") == -1)
			{
				printf("Info: ULA resource -> float-point multiplier\n");
			}
		}

		if (strcmp(mne, "DIV") == 0)
		{
			// se ainda nao tem o opcode S_DIV, escreve a info
			if (find_opc("S_DIV") == -1)
			{
				printf("Info: ULA resource -> integer divider (not recomended for high frequency opperation)\n");
			}
		}

		if (strcmp(mne, "S_DIV") == 0)
		{
			// se ainda nao tem o opcode DIV, escreve a info
			if (find_opc("DIV") == -1)
			{
				printf("Info: ULA resource -> integer divider (not recomended for high frequency opperation)\n");
			}
		}

		if (strcmp(mne, "F_DIV") == 0)
		{
			// se ainda nao tem o opcode SF_DIV, escreve a info
			if (find_opc("SF_DIV") == -1)
			{
				printf("Info: ULA resource -> float-point divider (not recomended for high frequency opperation)\n");
			}
		}

		if (strcmp(mne, "SF_DIV") == 0)
		{
			// se ainda nao tem o opcode F_DIV, escreve a info
			if (find_opc("F_DIV") == -1)
			{
				printf("Info: ULA resource -> float-point divider (not recomended for high frequency opperation)\n");
			}
		}

		if (strcmp(mne, "MOD") == 0)
		{
			// se ainda nao tem o opcode S_MOD, escreve a info
			if (find_opc("S_MOD") == -1)
			{
				printf("Info: ULA resource -> modulo computation (not recomended for high frequency opperation)\n");
			}
		}

		if (strcmp(mne, "S_MOD") == 0)
		{
			// se ainda nao tem o opcode MOD, escreve a info
			if (find_opc("MOD") == -1)
			{
				printf("Info: ULA resource -> modulo computation (not recomended for high frequency opperation)\n");
			}
		}

		if (strcmp(mne, "SGN") == 0)
		{
			// se ainda nao tem o opcode S_SGN, escreve a info
			if (find_opc("S_SGN") == -1)
			{
				printf("Info: ULA resource -> integer sign computation (standard library)\n");
			}
		}

		if (strcmp(mne, "S_SGN") == 0)
		{
			// se ainda nao tem o opcode SGN, escreve a info
			if (find_opc("SGN") == -1)
			{
				printf("Info: ULA resource -> integer sign computation (standard library)\n");
			}
		}

		if (strcmp(mne, "F_SGN") == 0)
		{
			// se ainda nao tem o opcode SF_SGN, escreve a info
			if (find_opc("SF_SGN") == -1)
			{
				printf("Info: ULA resource -> float-point sign computation (standard library)\n");
			}
		}

		if (strcmp(mne, "SF_SGN") == 0)
		{
			// se ainda nao tem o opcode F_SGN, escreve a info
			if (find_opc("F_SGN") == -1)
			{
				printf("Info: ULA resource -> float-point sign computation (standard library)\n");
			}
		}

		if (strcmp(mne, "NEG") == 0)
		{
			// se ainda nao tem o opcode NEG_M ou P_NEG_M, escreve a info
			if ((find_opc("NEG_M") == -1) && (find_opc("P_NEG_M") == -1))
			{
				printf("Info: ULA resource -> integer negative operation\n");
			}
		}

		if (strcmp(mne, "NEG_M") == 0)
		{
			// se ainda nao tem o opcode NEG ou P_NEG_M, escreve a info
			if ((find_opc("NEG") == -1) && (find_opc("P_NEG_M") == -1))
			{
				printf("Info: ULA resource -> integer negative operation\n");
			}
		}

		if (strcmp(mne, "P_NEG_M") == 0)
		{
			// se ainda nao tem o opcode NEG ou NEG_M, escreve a info
			if ((find_opc("NEG") == -1) && (find_opc("NEG_M") == -1))
			{
				printf("Info: ULA resource -> integer negative operation\n");
			}
		}

		if (strcmp(mne, "F_NEG") == 0)
		{
			// se ainda nao tem o opcode F_NEG_M ou PF_NEG_M, escreve a info
			if ((find_opc("F_NEG_M") == -1) && (find_opc("PF_NEG_M") == -1))
			{
				printf("Info: ULA resource -> float-point negative operation\n");
			}
		}

		if (strcmp(mne, "F_NEG_M") == 0)
		{
			// se ainda nao tem o opcode F_NEG ou PF_NEG_M, escreve a info
			if ((find_opc("F_NEG") == -1) && (find_opc("PF_NEG_M") == -1))
			{
				printf("Info: ULA resource -> float-point negative operation\n");
			}
		}

		if (strcmp(mne, "PF_NEG_M") == 0)
		{
			// se ainda nao tem o opcode F_NEG ou F_NEG_M, escreve a info
			if ((find_opc("F_NEG") == -1) && (find_opc("F_NEG_M") == -1))
			{
				printf("Info: ULA resource -> float-point negative operation\n");
			}
		}

		if (strcmp(mne, "ABS") == 0)
		{
			// se ainda nao tem o opcode ABS_M ou P_ABS_M, escreve a info
			if ((find_opc("ABS_M") == -1) && (find_opc("P_ABS_M") == -1))
			{
				printf("Info: ULA resource -> integer absolute operation\n");
			}
		}

		if (strcmp(mne, "ABS_M") == 0)
		{
			// se ainda nao tem o opcode ABS ou P_ABS_M, escreve a info
			if ((find_opc("ABS") == -1) && (find_opc("P_ABS_M") == -1))
			{
				printf("Info: ULA resource -> integer absolute operation\n");
			}
		}

		if (strcmp(mne, "P_ABS_M") == 0)
		{
			// se ainda nao tem o opcode ABS ou ABS_M, escreve a info
			if ((find_opc("ABS") == -1) && (find_opc("ABS_M") == -1))
			{
				printf("Info: ULA resource -> integer absolute operation\n");
			}
		}

		if (strcmp(mne, "F_ABS") == 0)
		{
			// se ainda nao tem o opcode F_ABS_M ou PF_ABS_M, escreve a info
			if ((find_opc("F_ABS_M") == -1) && (find_opc("PF_ABS_M") == -1))
			{
				printf("Info: ULA resource -> float-point absolute operation\n");
			}
		}

		if (strcmp(mne, "F_ABS_M") == 0)
		{
			// se ainda nao tem o opcode F_ABS ou PF_ABS_M, escreve a info
			if ((find_opc("F_ABS") == -1) && (find_opc("PF_ABS_M") == -1))
			{
				printf("Info: ULA resource -> float-point absolute operation\n");
			}
		}

		if (strcmp(mne, "PF_ABS_M") == 0)
		{
			// se ainda nao tem o opcode F_ABS ou F_ABS_M, escreve a info
			if ((find_opc("F_ABS") == -1) && (find_opc("F_ABS_M") == -1))
			{
				printf("Info: ULA resource -> float-point absolute operation\n");
			}
		}

		if (strcmp(mne, "PST") == 0)
		{
			// se ainda nao tem o opcode PST_M ou P_PST_M, escreve a info
			if ((find_opc("PST_M") == -1) && (find_opc("P_PST_M") == -1))
			{
				printf("Info: ULA resource -> integer pset computation (standard library)\n");
			}
		}

		if (strcmp(mne, "PST_M") == 0)
		{
			// se ainda nao tem o opcode PST ou P_PST_M, escreve a info
			if ((find_opc("PST") == -1) && (find_opc("P_PST_M") == -1))
			{
				printf("Info: ULA resource -> integer pset computation (standard library)\n");
			}
		}

		if (strcmp(mne, "P_PST_M") == 0)
		{
			// se ainda nao tem o opcode PST ou PST_M, escreve a info
			if ((find_opc("PST") == -1) && (find_opc("PST_M") == -1))
			{
				printf("Info: ULA resource -> integer pset computation (standard library)\n");
			}
		}

		if (strcmp(mne, "F_PST") == 0)
		{
			// se ainda nao tem o opcode F_PST_M ou PF_PST_M, escreve a info
			if ((find_opc("F_PST_M") == -1) && (find_opc("PF_PST_M") == -1))
			{
				printf("Info: ULA resource -> float-point pset computation (standard library)\n");
			}
		}

		if (strcmp(mne, "F_PST_M") == 0)
		{
			// se ainda nao tem o opcode F_PST ou PF_PST_M, escreve a info
			if ((find_opc("F_PST") == -1) && (find_opc("PF_PST_M") == -1))
			{
				printf("Info: ULA resource -> float-point pset computation (standard library)\n");
			}
		}

		if (strcmp(mne, "PF_PST_M") == 0)
		{
			// se ainda nao tem o opcode F_PST ou F_PST_M, escreve a info
			if ((find_opc("F_PST") == -1) && (find_opc("F_PST_M") == -1))
			{
				printf("Info: ULA resource -> float-point pset computation (standard library)\n");
			}
		}

		if (strcmp(mne, "NRM") == 0)
		{
			// se ainda nao tem o opcode NRM_M ou P_NRM_M, escreve a info
			if ((find_opc("NRM_M") == -1) && (find_opc("P_NRM_M") == -1))
			{
				printf("Info: ULA resource -> normalization computation (standard library)\n");
			}
		}

		if (strcmp(mne, "NRM_M") == 0)
		{
			// se ainda nao tem o opcode NRM ou P_NRM_M, escreve a info
			if ((find_opc("NRM") == -1) && (find_opc("P_NRM_M") == -1))
			{
				printf("Info: ULA resource -> normalization computation (standard library)\n");
			}
		}

		if (strcmp(mne, "P_NRM_M") == 0)
		{
			// se ainda nao tem o opcode NRM ou NRM_M, escreve a info
			if ((find_opc("NRM") == -1) && (find_opc("NRM_M") == -1))
			{
				printf("Info: ULA resource -> normalization computation (standard library)\n");
			}
		}

		if (strcmp(mne, "I2F") == 0)
		{
			// se ainda nao tem o opcode I2F_M ou P_I2F_M, escreve a info
			if ((find_opc("I2F_M") == -1) && (find_opc("P_I2F_M") == -1))
			{
				printf("Info: ULA resource -> int to float converter\n");
			}
		}

		if (strcmp(mne, "I2F_M") == 0)
		{
			// se ainda nao tem o opcode I2F ou P_I2F_M, escreve a info
			if ((find_opc("I2F") == -1) && (find_opc("P_I2F_M") == -1))
			{
				printf("Info: ULA resource -> int to float converter\n");
			}
		}

		if (strcmp(mne, "P_I2F_M") == 0)
		{
			// se ainda nao tem o opcode I2F ou I2F_M, escreve a info
			if ((find_opc("I2F") == -1) && (find_opc("I2F_M") == -1))
			{
				printf("Info: ULA resource -> int to float converter\n");
			}
		}

		if (strcmp(mne, "F2I") == 0)
		{
			// se ainda nao tem o opcode F2I_M ou P_F2I_M, escreve a info
			if ((find_opc("F2I_M") == -1) && (find_opc("P_F2I_M") == -1))
			{
				printf("Info: ULA resource -> float to int converter\n");
			}
		}

		if (strcmp(mne, "F2I_M") == 0)
		{
			// se ainda nao tem o opcode F2I ou P_F2I_M, escreve a info
			if ((find_opc("F2I") == -1) && (find_opc("P_F2I_M") == -1))
			{
				printf("Info: ULA resource -> float to int converter\n");
			}
		}

		if (strcmp(mne, "P_F2I_M") == 0)
		{
			// se ainda nao tem o opcode F2I ou F2I_M, escreve a info
			if ((find_opc("F2I") == -1) && (find_opc("F2I_M") == -1))
			{
				printf("Info: ULA resource -> float to int converter\n");
			}
		}

		if (strcmp(mne, "AND") == 0)
		{
			// se ainda nao tem o opcode S_AND, escreve a info
			if (find_opc("S_AND") == -1)
			{
				printf("Info: ULA resource -> & operator\n");
			}
		}

		if (strcmp(mne, "S_AND") == 0)
		{
			// se ainda nao tem o opcode AND, escreve a info
			if (find_opc("AND") == -1)
			{
				printf("Info: ULA resource -> & operator\n");
			}
		}

		if (strcmp(mne, "ORR") == 0)
		{
			// se ainda nao tem o opcode S_ORR, escreve a info
			if (find_opc("S_ORR") == -1)
			{
				printf("Info: ULA resource -> | operator\n");
			}
		}

		if (strcmp(mne, "S_ORR") == 0)
		{
			// se ainda nao tem o opcode ORR, escreve a info
			if (find_opc("ORR") == -1)
			{
				printf("Info: ULA resource -> | operator\n");
			}
		}

		if (strcmp(mne, "XOR") == 0)
		{
			// se ainda nao tem o opcode S_XOR, escreve a info
			if (find_opc("S_XOR") == -1)
			{
				printf("Info: ULA resource -> ^ operator\n");
			}
		}

		if (strcmp(mne, "S_XOR") == 0)
		{
			// se ainda nao tem o opcode XOR, escreve a info
			if (find_opc("XOR") == -1)
			{
				printf("Info: ULA resource -> ^ operator\n");
			}
		}

		if (strcmp(mne, "INV") == 0)
		{
			// se ainda nao tem o opcode INV_M ou P_INV_M, escreve a info
			if ((find_opc("INV_M") == -1) && (find_opc("P_INV_M") == -1))
			{
				printf("Info: ULA resource -> ~ operator\n");
			}
		}

		if (strcmp(mne, "INV_M") == 0)
		{
			// se ainda nao tem o opcode INV ou P_INV_M, escreve a info
			if ((find_opc("INV") == -1) && (find_opc("P_INV_M") == -1))
			{
				printf("Info: ULA resource -> ~ operator\n");
			}
		}

		if (strcmp(mne, "P_INV_M") == 0)
		{
			// se ainda nao tem o opcode INV ou INV_M, escreve a info
			if ((find_opc("INV") == -1) && (find_opc("INV_M") == -1))
			{
				printf("Info: ULA resource -> ~ operator\n");
			}
		}

		if (strcmp(mne, "LAN") == 0)
		{
			// se ainda nao tem o opcode S_LAN, escreve a info
			if (find_opc("S_LAN") == -1)
			{
				printf("Info: ULA resource -> && operator\n");
			}
		}

		if (strcmp(mne, "S_LAN") == 0)
		{
			// se ainda nao tem o opcode LAN, escreve a info
			if (find_opc("LAN") == -1)
			{
				printf("Info: ULA resource -> && operator\n");
			}
		}

		if (strcmp(mne, "LOR") == 0)
		{
			// se ainda nao tem o opcode S_LOR, escreve a info
			if (find_opc("S_LOR") == -1)
			{
				printf("Info: ULA resource -> || operator\n");
			}
		}

		if (strcmp(mne, "S_LOR") == 0)
		{
			// se ainda nao tem o opcode LOR, escreve a info
			if (find_opc("LOR") == -1)
			{
				printf("Info: ULA resource -> || operator\n");
			}
		}

		if (strcmp(mne, "LIN") == 0)
		{
			// se ainda nao tem o opcode LIN_M ou P_LIN_M, escreve a info
			if ((find_opc("LIN_M") == -1) && (find_opc("P_LIN_M") == -1))
			{
				printf("Info: ULA resource -> ! operator\n");
			}
		}

		if (strcmp(mne, "LIN_M") == 0)
		{
			// se ainda nao tem o opcode LIN ou P_LIN_M, escreve a info
			if ((find_opc("LIN") == -1) && (find_opc("P_LIN_M") == -1))
			{
				printf("Info: ULA resource -> ! operator\n");
			}
		}

		if (strcmp(mne, "P_LIN_M") == 0)
		{
			// se ainda nao tem o opcode LIN ou LIN_M, escreve a info
			if ((find_opc("LIN") == -1) && (find_opc("LIN_M") == -1))
			{
				printf("Info: ULA resource -> ! operator\n");
			}
		}

		if (strcmp(mne, "LES") == 0)
		{
			// se ainda nao tem o opcode S_LES, escreve a info
			if (find_opc("S_LES") == -1)
			{
				printf("Info: ULA resource -> < operator for int\n");
			}
		}

		if (strcmp(mne, "S_LES") == 0)
		{
			// se ainda nao tem o opcode LES, escreve a info
			if (find_opc("LES") == -1)
			{
				printf("Info: ULA resource -> < operator for int\n");
			}
		}

		if (strcmp(mne, "F_LES") == 0)
		{
			// se ainda nao tem o opcode SF_LES, escreve a info
			if (find_opc("SF_LES") == -1)
			{
				printf("Info: ULA resource -> < operator for float\n");
			}
		}

		if (strcmp(mne, "SF_LES") == 0)
		{
			// se ainda nao tem o opcode F_LES, escreve a info
			if (find_opc("F_LES") == -1)
			{
				printf("Info: ULA resource -> < operator for float\n");
			}
		}

		if (strcmp(mne, "GRE") == 0)
		{
			// se ainda nao tem o opcode S_GRE, escreve a info
			if (find_opc("S_GRE") == -1)
			{
				printf("Info: ULA resource -> > operator for int\n");
			}
		}

		if (strcmp(mne, "S_GRE") == 0)
		{
			// se ainda nao tem o opcode GRE, escreve a info
			if (find_opc("GRE") == -1)
			{
				printf("Info: ULA resource -> > operator for int\n");
			}
		}

		if (strcmp(mne, "F_GRE") == 0)
		{
			// se ainda nao tem o opcode SF_GRE, escreve a info
			if (find_opc("SF_GRE") == -1)
			{
				printf("Info: ULA resource -> > operator for float\n");
			}
		}

		if (strcmp(mne, "SF_GRE") == 0)
		{
			// se ainda nao tem o opcode F_GRE, escreve a info
			if (find_opc("F_GRE") == -1)
			{
				printf("Info: ULA resource -> > operator for float\n");
			}
		}

		if (strcmp(mne, "EQU") == 0)
		{
			// se ainda nao tem o opcode S_EQU, escreve a info
			if (find_opc("S_EQU") == -1)
			{
				printf("Info: ULA resource -> == operator\n");
			}
		}

		if (strcmp(mne, "S_EQU") == 0)
		{
			// se ainda nao tem o opcode EQU, escreve a info
			if (find_opc("EQU") == -1)
			{
				printf("Info: ULA resource -> == operator\n");
			}
		}

		if (strcmp(mne, "SHL") == 0)
		{
			// se ainda nao tem o opcode S_SHL, escreve a info
			if (find_opc("S_SHL") == -1)
			{
				printf("Info: ULA resource -> << operator\n");
			}
		}

		if (strcmp(mne, "S_SHL") == 0)
		{
			// se ainda nao tem o opcode SHL, escreve a info
			if (find_opc("SHL") == -1)
			{
				printf("Info: ULA resource -> << operator\n");
			}
		}

		if (strcmp(mne, "SHR") == 0)
		{
			// se ainda nao tem o opcode S_SHR, escreve a info
			if (find_opc("S_SHR") == -1)
			{
				printf("Info: ULA resource -> >> operator\n");
			}
		}

		if (strcmp(mne, "S_SHR") == 0)
		{
			// se ainda nao tem o opcode SHR, escreve a info
			if (find_opc("SHR") == -1)
			{
				printf("Info: ULA resource -> >> operator\n");
			}
		}

		if (strcmp(mne, "SRS") == 0)
		{
			// se ainda nao tem o opcode S_SRS, escreve a info
			if (find_opc("S_SRS") == -1)
			{
				printf("Info: ULA resource -> >>> operator\n");
			}
		}

		if (strcmp(mne, "S_SRS") == 0)
		{
			// se ainda nao tem o opcode SRS, escreve a info
			if (find_opc("SRS") == -1)
			{
				printf("Info: ULA resource -> >>> operator\n");
			}
		}

		if (strcmp(mne, "F_ROT") == 0)
		{
			// se ainda nao tem o opcode F_ROT, escreve a info
			if (find_opc("F_ROT") == -1)
			{
				printf("Info: ULA resource -> root-square approximation for float\n");
			}
		}

		// --------------------------------------------------------------------
		// depois cadastra o opcode novo --------------------------------------
		// --------------------------------------------------------------------
		
    	strcpy(m_name[m_count], mne);
    	m_count++;
	}
}

// verifica se tem instrucao INN
int opc_inn() {return (find_opc("INN") != -1) | (find_opc("P_INN") != -1) | (find_opc("F_INN") != -1) | (find_opc("PF_INN") != -1);}
// verifica se tem instrucao OUT
int opc_out() {return  find_opc("OUT") != -1;}
// verifica se tem instrucao CAL
int opc_cal() {return  find_opc("CAL") != -1;}