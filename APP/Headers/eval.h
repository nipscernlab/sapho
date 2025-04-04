// ----------------------------------------------------------------------------
// rotinas para calcular o tamanho das memorias ... ---------------------------
// a medida que o lex vai escaneando o .asm -----------------------------------
// ----------------------------------------------------------------------------

void eval_direct(int  next_state);
void eval_opcode(int  next_state);
void eval_opernd(char        *va);
void eval_finish();
