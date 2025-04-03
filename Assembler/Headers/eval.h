// ----------------------------------------------------------------------------
// rotinas para gerar os arquivos .mif das memorias ... -----------------------
// a medida que o lex vai escaneando o .asm -----------------------------------
// ----------------------------------------------------------------------------

#include  <stdio.h>

// variaveis de estado
extern int  pp;                  // flag do pre-processamento
extern int  n_ins;               // numero de instrucoes adicionadas
extern int  n_dat;               // numero de variaveis  adicionadas
extern int  itr_addr;            // endereco da interrupcao
extern int  isrf;                // diz se achou uma instrucao pra fazer FFT

// guarda os valores das diretivas
extern char prname[128];         // nome do processador
extern int  nubits;              // tamanho da palavra da ula
extern int  nbmant;              // numero de bits da mantissa
extern int  nbexpo;              // numero de bits do expoente
extern int  ndstac;              // tamanho da pilha de dados
extern int  sdepth;              // tamanho da pilha de subrotinas
extern int  nuioin;              // numero de portas de entrada
extern int  nuioou;              // numero de portas de saida
extern int  nugain;              // constante de divisao
extern int  fftsiz;              // tamanho da fft (em bits)

extern char opcd[64];         // guarda opcode atual

extern int nbopr;             // num de bits de operando
extern int fim_addr;          // endereco de @fim JMP fim
extern FILE *f_data, *f_instr; // .mif das memorias de dado e instrucao

void eval_direct(int   next_state);
void eval_itrad ();

void eval_init  (int   prep);
void eval_opcode(int   op, int next_state, char *text, char *nome);
void eval_opernd(char *va, int is_const);
void eval_label (char *la);
void eval_finish();
