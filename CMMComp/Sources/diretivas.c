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
        case 1: strcpy (prname,v_name[id]); break;
        case 3: nbmant = ival; break;
        case 4: nbexpo = ival; break;
        // rever essa questao do num de i/o
        case 7: if (ival < 1) fprintf(stderr, "Erro na linha %d: pra que você quer um processador sem entrada de dados?\n", line_num+1); break;
        case 8: if (ival < 1) fprintf(stderr, "Erro na linha %d: pra que você quer um processador sem saída de dados?\n"  , line_num+1); break;
    }
}