#ifndef VARIAVEIS_H_INCLUDED
#define VARIAVEIS_H_INCLUDED

#define NVARMAX 999999     // tamanho maximo da tabela de simbolos

extern char  fname         [512]; // nome da funcao atual sendo parseada
extern char v_name[NVARMAX][512]; // nome da variavel ou funcao
extern int  v_type[NVARMAX];      // 0 -> nao identificada, 1 -> int, 2 -> float
extern int  v_used[NVARMAX];      // se ID ja foi usado
extern int  v_asgn[NVARMAX];      // se variavel ja recebeu algum valor
extern int  v_fpar[NVARMAX];      // se ID eh uma funcao, diz a lista de parametros
extern int  v_fnid[NVARMAX];      // ID da funcao a qual a variavel pertence
extern int  v_isar[NVARMAX];      // se variavel eh um array
extern int  v_isco[NVARMAX];      // se variavel eh uma constante

int    find_var(char *val);
void    add_var(char *var);
void  check_var();
char *rem_fname(char *var, char *fname);

// usado no lexer para pegar variaveis e constantes
int exec_id  (char *text);
int exec_num (char *text);
int exec_cnum(char *text);

// testes com numeros complexos -----------------------------------------------
// tive que implementar isso soh por causa da bagunca do array no assigne
// s eeu puder mudar isso, vai melhorar bem o codigo
extern int a_cnt;
// fim do teste ---------------------------------------------------------------

#endif // VARIAVEIS_H_INCLUDED
