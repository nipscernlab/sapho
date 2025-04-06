// ----------------------------------------------------------------------------
// rotinas para uso no lexer --------------------------------------------------
// ----------------------------------------------------------------------------

void eval_init  (char      *path);
void eval_direct(int  next_state);
void eval_itrad ();
void eval_opcode(int  next_state);
void eval_opernd(char        *va);
void eval_label (char        *va);
void eval_finish();
