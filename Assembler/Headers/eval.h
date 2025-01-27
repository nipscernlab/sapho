int pp;                // flag do pre-processador

char opcd[64];         // guarda opcode atual

int n_ins;             // numero de instrucoes adicionadas
int n_dat;             // numero de variaveis  adicionadas
int n_opc;             // numero de instrucoes no arquivo de traducao
int isrf;              // diz se achou a instrucao pra fazer FFT
int itr_addr;          // endereco da interrupcao
int fim_addr;          // endereco de @fim JMP fim

int  v_cont;           // numero   de variaveis encontradas
char v_namo[1000][64]; //     nome da variavel  encontrada
int  v_add [1000];     // endereco da variavel  encontrada
int  v_tipo[1000];     // tipo     da variavel  encontrada

void eval_init  (int   prep);
void eval_direct(int   next_state);
void eval_opcode(int   op, int next_state, char *text, char *nome);
void eval_opernd(char *va, int is_const);
void eval_label (char *la);
void eval_finish();