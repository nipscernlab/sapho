#ifndef DATA_ASSIGN_H_INCLUDED
#define DATA_ASSIGN_H_INCLUDED

int  get_type    (int et);                                      // funcao auxiliar para pegar o tipo de dado
void var_set     (int id, int et , int is_array, int set_type); // funcao principal de atribuicao de variaveis (SET)
void pplus_assign(int id);                                      // implementa i++
void aplus_assign(int id, int et);                              // implements i++ pra array 1D
void aplu2_assign(int id, int et1, int et2);                    // implements i++ pra array 2D

#endif // DATA_ASSIGN_H_INCLUDED
