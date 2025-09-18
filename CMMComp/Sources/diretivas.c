// ----------------------------------------------------------------------------
// Reune funcoes e variaveis de estado acionadas por diretivas ----------------
// ----------------------------------------------------------------------------

// includes globais
#include <string.h>
#include <stdlib.h>

// includes locais
#include "..\Headers\t2t.h"
#include "..\Headers\global.h"
#include "..\Headers\funcoes.h"
#include "..\Headers\variaveis.h"

// ----------------------------------------------------------------------------
// redeclaracao de variaveis globais ------------------------------------------
// ----------------------------------------------------------------------------

char prname[128] ; // nome do processador
int  nbmant  = 16; // numero de bits de mantissa
int  nbexpo  =  6; // numero de bits de expoente
int  nuioin  =  1; // numero de portas de entrada
int  nuioou  =  1; // numero de portas de saida

// ----------------------------------------------------------------------------
// Controle de diretivas ------------------------------------------------------
// ----------------------------------------------------------------------------

// escreve as diretivas de compilacao no arquivo asm
void dire_exec(char *dir, int id, int t)
{
    add_sinst(0, "%s %s\n", dir, v_name[id]);

    int ival = atoi(v_name[id]);

    // acao a tomar dependendo da diretiva
    // soh as diretivas 1 3 e 4 que tem efeito no compilador cmm
    switch(t)
    {
        case  1: strcpy (prname,v_name[id]); break;
        case  3: nbmant = ival ; break;
        case  4: nbexpo = ival ; break;
        case  7: nuioin = ival ; break;
        case  8: nuioou = ival ; break;
    }
}