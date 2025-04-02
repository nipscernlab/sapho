#include  <stdio.h>

extern char v_namo[1000][64]; //     nome da variavel  encontrada
extern int  v_add [1000];     // endereco da variavel  encontrada
extern int  v_tipo[1000];     // tipo     da variavel  encontrada

extern int sim_v_cnt;
extern int sim_n_opc;             // numero de instrucoes no arquivo de traducao

extern FILE *f_tran;          // arquivo para traducao do opcode

void sim_init();
void sim_add (char *opc, char *opr);
int  sim_is_var(char *va, int *tipo, int *is_global, char *nome);
void sim_reg(char *va);
void sim_check_fim(char *la);