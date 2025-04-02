#include  <stdio.h>

extern int sim_v_cnt;
extern int sim_n_opc;             // numero de instrucoes no arquivo de traducao

extern FILE *f_tran;          // arquivo para traducao do opcode

void sim_init();
void sim_add (char *line);