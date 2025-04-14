// ----------------------------------------------------------------------------
// implementa declaracao de variaveis -----------------------------------------
// ----------------------------------------------------------------------------

extern int type_tmp; // para pegar o tipo quando uma variavel eh declarada (ver c2asm.l)

void declar_var   (int id);                                         // declaracao de variavel
void declar_arr_1d(int id_var, int id_arg,           int id_fname); // declaracao de array 1d
void declar_arr_2d(int id_var, int id_x  , int id_y, int id_fname); // declaracao de array 2d