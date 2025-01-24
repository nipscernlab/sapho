#ifndef EVAL_H_INCLUDED
#define EVAL_H_INCLUDED

int pp;       // flag do pre-processador

char opcd[64];

int n_ins;    // numero de instrucoes adicionadas
int n_dat;    // numero de variaveis adicionadas
int n_opc;    // numero de instrucoes no arquivo de traducao
int isrf;     // diz se achou a instrucao pra fazer FFT
int itr_addr; // endereco da interrupcao

char v_namo[1000][64];
int  v_add [1000];
int  v_tipo[1000];
int  v_cont;

void eval_init  (int prep);
void eval_direct(int next_state);
void eval_opcode(int op, int next_state, char *text);
void eval_opernd(char *va, int is_const);
void eval_label (char *la);
void eval_finish();

#endif // EVAL_H_INCLUDED
