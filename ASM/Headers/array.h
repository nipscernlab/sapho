// ----------------------------------------------------------------------------
// tratamento de arrays em assembly -------------------------------------------
// ----------------------------------------------------------------------------

#include <stdio.h>

// adiciona array na memoria de dados
// se for array normal (f_name = ""), completa com zero
// se for array inicializado, chama fill_mem para preencher
void arr_add(int size, int type, char *f_name, FILE *f_data);