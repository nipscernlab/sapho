#ifndef DATA_ASSIGN_H_INCLUDED
#define DATA_ASSIGN_H_INCLUDED

int get_type(int et); // funcao auxiliar para pegar o tipo de dado

void  var_set    (int id, int et, int is_array, int set_type); // funcao principal de atribuicao de variaveis (SET)
void pplus_assign(int id);                                     // implementa i++

#endif // DATA_ASSIGN_H_INCLUDED
