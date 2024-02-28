#ifndef VARIAVEIS_H_INCLUDED
#define VARIAVEIS_H_INCLUDED

#define NVARMAX 999999

char  fname[512];
char v_name[NVARMAX][512]; // nome da variavel ou funcao
int  v_type[NVARMAX];      // 0 -> nao identificada, 1 -> int, 2 -> float
int  v_used[NVARMAX];      // se ID ja foi usado
int  v_asgn[NVARMAX];      // se variavel ja recebeu algum valor
int  v_fpar[NVARMAX];      // se ID eh uma funcao, diz a lista de parametros
int  v_fnid[NVARMAX];      // ID a qual a variavel pertence
int  v_isar[NVARMAX];      // se variavel eh um array
int  v_isco[NVARMAX];      // se variavel eh uma constante

int    find_var(char *val);
void    add_var(char *var);
void  check_var();
char *rem_fname(char *var, char *fname);

#endif // VARIAVEIS_H_INCLUDED
