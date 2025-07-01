// ----------------------------------------------------------------------------
// implementa tabela de identificadores ---------------------------------------
// ----------------------------------------------------------------------------

#define NVARMAX 999999                   // tamanho maximo da tabela de simbolos

// elementos da tabela --------------------------------------------------------

extern char v_name[NVARMAX][512];        // nome da variavel ou funcao
extern int  v_type[NVARMAX];             // 0 -> nao identificada, 1 -> int, 2 -> float, 3 -> comp, 5 -> comp const
extern int  v_used[NVARMAX];             // se o ID ja foi usado
extern int  v_asgn[NVARMAX];             // se variavel ja recebeu algum valor
extern int  v_fpar[NVARMAX];             // se ID eh uma funcao, diz a lista de parametros
extern int  v_fnid[NVARMAX];             // ID da funcao a qual a variavel pertence
extern int  v_isar[NVARMAX];             // se variavel eh um array
extern int  v_isco[NVARMAX];             // se variavel eh uma constante
extern int  v_size[NVARMAX];             // tamanho do array (caso seja array)

// manipulacao de elementos da tabela -----------------------------------------

int    find_var(char *val);              // procura  ID na tabela (-1 se nao achar)
void    add_var(char *var);              // adiciona ID na tabela
void  check_var();                       // checa o que foi usado da tabela(no final do parser)
char *rem_fname(char *var, char *fname); // remove nome da funcao da variavel

// usado no lexer para pegar variaveis e constantes ---------------------------

int exec_id  (char *text);               // achou um ID
int exec_num (char *text);               // achou uma constante
int exec_cnum(char *text);               // achou uma constante complexa