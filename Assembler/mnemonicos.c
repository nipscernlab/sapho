#include "mnemonicos.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

void add_mne(char *mne)
{
    if (find_mne(mne) != -1) return;

    strcpy(m_name[m_count], mne);
    m_count++;
}
