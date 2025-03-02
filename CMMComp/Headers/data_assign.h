// funcoes auxiliares ---------------------------------------------------------

int  get_type    (int et);                          // pega o tipo de dado
int  get_img_id  (int id);                          // pega o id da parte imaginaria
void get_cmp_cst (int et, int *et_r, int *et_i);    // gera ets para uma constante complexa
void get_cmp_ets (int et, int *et_r, int *et_i);    // pega ets de uma variavel complexa

void get_1d_index(int id, int et);                  // pega o indice de um array 1D
void get_2d_index(int id, int et1, int et2);        // pega o indice de um array 2D

// funcoes de assign ----------------------------------------------------------

void var_set     (int id, int et);                  // assign padrao
void array_set   (int id, int et , int fft);        // assign em array
void pplus_assign(int id);                          // assign em i++
void aplus_assign(int id, int et);                  // assign em i++ pra array 1D
void aplu2_assign(int id, int et1, int et2);        // assign em i++ pra array 2D