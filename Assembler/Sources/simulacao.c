#include "..\Headers\simulacao.h"
#include "..\Headers\veri_comp.h"

int sim_v_cnt = 0;
int sim_n_opc     = 0;               // numero de instrucoes no arquivo de traducao

FILE *f_tran;

void sim_init()
{
    // abre arquivo de traducao de opcode na pasta Temp
    char path[1024];
    sprintf(path, "%s/trad_opcode.txt", temp_dir);
    f_tran  = fopen(path, "w");
}

void sim_add (char *line)
{
    fprintf(f_tran , "%d %s\n", sim_n_opc++, line);
}