// ----------------------------------------------------------------------------
// tratamento de interrupcao --------------------------------------------------
// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

#include "..\Headers\global.h"

int itr_ok = 0; // se ja usou ou nao interrupcao

// gera diretiva #ITRAD
// ainda tenho q checar os lugares q nao podem ter isso
// ex: dentro de loop, dentro de switch case, pensar ...
// talvez um warning ja sirva
void dire_inter()
{
    if (itr_ok == 1) {fprintf(stderr, "Erro na linha %d: já tem uma interrupção rolando em outro ponto antes desse!\n", line_num+1); exit(EXIT_FAILURE);}

    printf("Info: interruption directive found at line %d\n", line_num+1);

    add_sinst(0, "#ITRAD\n");
    itr_ok = 1;
}