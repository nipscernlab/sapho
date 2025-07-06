// ----------------------------------------------------------------------------
// implementa declaracao de variaveis -----------------------------------------
// ----------------------------------------------------------------------------

extern int type_tmp; // para pegar o tipo quando uma variavel eh declarada (ver c2asm.l)

// ----------------------------------------------------------------------------
// declaracoes padrao ---------------------------------------------------------
// ----------------------------------------------------------------------------

void declar_var   (int id);                                          // declaracao de variavel
void declar_arr_1d(int id_var , int id_arg,           int id_fname); // declaracao de array 1d
void declar_arr_2d(int id_var , int id_x  , int id_y, int id_fname); // declaracao de array 2d

// ----------------------------------------------------------------------------
// declaracoes de array com inicializacao em notacao de Dirac -----------------
// ----------------------------------------------------------------------------

void declar_Mv    (int id_name, int id_N  , int id_M, int id_v    ); // ex: float A[4,4] # |B|a>;
void declar_cv    (int id_name, int id_N  , int id_c, int id_v    ); // ex: float a[4] # c|a>;