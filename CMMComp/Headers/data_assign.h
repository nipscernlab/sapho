int  get_type    (int et);                                                       // funcao auxiliar para pegar o tipo de dado
void var_set (int id, int et);
void pplus_assign(int id);                                                       // implementa i++
void aplus_assign(int id, int et);                                               // implements i++ pra array 1D
void aplu2_assign(int id, int et1, int et2);                                     // implements i++ pra array 2D

void get_1d_index(int id, int et);                                               // pega o indice de um array
void get_2d_index(int id, int et1, int et2);                                     // pega o indice de um array
void array_set   (int id, int et , int fft);                                     // faz o assign de um array 1D

// testes com variaveis complexas ---------------------------------------------
void split_cmp_const(int et, int *et_r, int *et_i);
void   get_cmp_ets  (int et, int *et_r, int *et_i);
int    get_img_id   (int id);
// fim do teste ---------------------------------------------------------------
