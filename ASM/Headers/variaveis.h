// ----------------------------------------------------------------------------
// rotinas para manipular variaveis encontradas no arquivo .asm ---------------
// ----------------------------------------------------------------------------

void var_add (char *var, int val);  // adiciona variavel na tabela
void var_inc (int   val);           // incrementa o tamanho da memoria (para arrays)
int  var_find(char *val);           // retorna o indice da variavel na tabela
int  var_val (char *var);           // retorna o valor  da variavel
int  var_cnt ();                    // retorna o numero de variaveis
