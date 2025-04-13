// ----------------------------------------------------------------------------
// rotinas para implementacao de saltos ---------------------------------------
// ----------------------------------------------------------------------------

// if/else --------------------------------------------------------------------

void    if_exp (int et);             // inicio do if
void    if_stmt();                   // antes dos stmts do if
void  else_stmt();                   // antes dos stmts do else
void    if_fim ();                   // final do if/else

// while ----------------------------------------------------------------------

void  while_expp();                  // somente a palavra-chave while - gera um label nesse ponto
void  while_expexp(int  et);         // executa o exp e cria um JIZ pra ver se entra ou nao
void  while_stmt();                  // final do while. Da um JMP para o inicio e cria um label pro final logo abaixo
void  exec_break();                  // da um JMP pro final do while

// switch/case ----------------------------------------------------------------

void   case_test (int id, int type); // testa se eh o case correto
void defaut_test ();                 // testa se eh o default
void switch_break();                 // achou um break do switch/case
void exec_switch (int et);           // inicio do switch/case
void  end_switch ();                 // final do switch/case