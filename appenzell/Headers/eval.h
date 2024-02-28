#ifndef EVAL_H_INCLUDED
#define EVAL_H_INCLUDED

int  n_ins;                         // numero de instrucoes adicionadas
int  n_dat;

void eval_init  (int prep);
void eval_direct(int next_state);
void eval_opcode(int op, int next_state);
void eval_opernd(char *va, int is_const);
void eval_label (char *la);
void eval_finish();


#endif // EVAL_H_INCLUDED
