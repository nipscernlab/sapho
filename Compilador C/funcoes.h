#ifndef FUNCOES_H_INCLUDED
#define FUNCOES_H_INCLUDED

int  ret_ok;  // diz se teve um retorno da funcao corretamente
int  mainok;  // status da funcao main: 0 -> nao usada, 1 -> declarada, 2 -> chamada no inicio
int fun_id1;  // guarda id da funcao sendo parseada
int fun_id2;  // guarda id da funcao sendo usada
int  p_test;  // identifica parametros na chamada de funcoes (parecido com OFST, mas de valor 10)

// declaracoes ----------------------------------------------------------------

void  declar_fun     (int id1, int id2); // tipo e nome
void  declar_firstpar(int id);           // primeiro/unico parametro
int   declar_par     (int   t, int id ); // proximo parametro
void  set_par        (int id);           // SET no parametro
void  declar_ret     (int et);           // achou uma palavra-chave return
void  func_ret       (int id);           // fim/return da funcao

// utilizacao -----------------------------------------------------------------

void  par_exp        (int et ); // carrega primeiro parametro (se houver)
void  par_listexp    (int et ); // carrega os proximos parametros (se houver)
void  vcall          (int id ); // CALL de funcao com retorno
int   fcall          (int id ); // CALL de funcao void

// funcoes auxiliares ---------------------------------------------------------

int   get_npar       (int par); // calcula  numero de parametros
void  par_check      (int et ); // checa se numero de parametros na declaracao eh igual na chamada

#endif // FUNCOES_H_INCLUDED
