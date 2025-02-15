#ifndef DATA_ASSIGN_H_INCLUDED
#define DATA_ASSIGN_H_INCLUDED

int  get_type    (int et);                                                       // funcao auxiliar para pegar o tipo de dado
void var_set     (int id, int et , int is_array, int pop, int warn, int is_cmp); // funcao principal de atribuicao de variaveis (SET)
void var_set_new (int id, int et);
void pplus_assign(int id);                                                       // implementa i++
void aplus_assign(int id, int et);                                               // implements i++ pra array 1D
void aplu2_assign(int id, int et1, int et2);                                     // implements i++ pra array 2D

// testes com variaveis complexas ---------------------------------------------
void split_cmp_const(int et, int *et_r, int *et_i);
void   get_cmp_ets  (int et, int *et_r, int *et_i);
int    get_img_id   (int id);
void   var_set_comp (int id, int  et  , int is_array);                           // atribuicao com variaveis complexas
// fim do teste ---------------------------------------------------------------

#endif // DATA_ASSIGN_H_INCLUDED
