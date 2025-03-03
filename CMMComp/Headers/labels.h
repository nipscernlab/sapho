// ----------------------------------------------------------------------------
// gera labels em assembly pras instrucoes de salto ---------------------------
// ----------------------------------------------------------------------------

int  push_lab(int type); // coloca   um label na pilha
int   pop_lab();         // pega ultimo label na pilha
int   get_lab();         // le   indice do ultimo lab
int get_while();         // pega indice do ultimo while
int get_if   ();         // pega indice do ultimo if