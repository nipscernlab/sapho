// ----------------------------------------------------------------------------
// rotinas para uso no lexer --------------------------------------------------
// ----------------------------------------------------------------------------

void eval_init  (char      *path); // executado antes  do lexer
void eval_direct(int  next_state); // executado quando uma diretiva eh encontrada
void eval_itrad ();                // executado quando a interrupcao eh encontrada
void eval_opcode(int  next_state); // executado quando um opcode eh encontrado
void eval_opernd(char        *va); // executado quando um operando eh encontrado
void eval_label (char        *va); // executado quando um label eh encontrado
void eval_finish();                // executado depois do lexer
