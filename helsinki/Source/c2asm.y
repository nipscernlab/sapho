%{

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "variaveis.h"
#include "labels.h"
#include "t2t.h"

#define OFST 1000000

FILE *f_asm, *yyin;

int yylex(void);

int acc_ok = 0; // 0 -> acc vazio (use LOAD)  , 1 -> acc carregado (use PLD)
int prtype = 0; // 0 -> processador fonto fixo, 1 -> processador ponto flutuante
int p_test;     // testa parametros na chamada de funcoes
int fun_id1;    // guarda id da funcao atual durante declaracao
int fun_id2;    // guarda id da funcao atualmente chamada
int mainok = 0; // status da funcao main: 0 -> nao usada, 1 -> declarada, 2 -> chamada no inicio
int ret_ok = 0; // status de retorno da funcao atual;
int acc_id;     // id da ultima variavel carregada no acumulador

int   load      (int id, int is_const, int dtype, int is_array);
void  load_check(int et, int sinal);
void  yyerror   (char  const *s);
void  func_ret  (int id);
void  var_set   (int id, int et, int is_array, int set_type);
int   negacao   (int et);
int   neg         (int et);
int operacoes   (int et1, int et2, char *iop, char *fop, int *op);
void declar_var (int id1, int id2, int id3);
int  declar_par (int   t, int id );
void declar_fun (int id1, int id2);
void declar_ret (int et);
int  fcall      (int id);
void  vcall      (int id);
int   int_oper    (int t1, int t2, char *op, char *code, int fok);
void  array_check(int id, int et);
int   get_npar    (int par);
void  par_check  (int et);
int   exec_in    (int et);
int   exec_abs   (int et);
void  exec_sign1 (int et);
int   exec_sign2 (int et);
int   get_type   (int et);
void  exec_out1  (int et);
void  exec_out2  (int et);
%}

%token PRNAME DIRNAM DATYPE NUBITS NBMANT NBEXPO NDSTAC SDEPTH NUIOIN NUIOOU NUGAIN
%token TYPE
%token INUM FNUM ID STRING
%token IN OUT ABS SIGN
%token RETURN
%token WHILE IF ELSE
%token SHIFTL SHIFTR SSHIFTR
%token GREQU LESEQ EQU DIF LAND LOR
%token NORM


%right ','

%left LOR
%left LAND
%left '|'
%left '^'
%left '&'
%left EQU DIF
%left '>' '<' GREQU LESEQ
%left SHIFTL SHIFTR SSHIFTR
%left '+' '-'
%left '*' '/' '%'
%left '!' '~'
%left NORM


%%

// Programa e seus elementos --------------------------------------------------

prog         : prog_elements | prog prog_elements;
prog_elements: direct | declar | funcao;

// Diretivas de compilacao ----------------------------------------------------

direct:   PRNAME  ID                       {fprintf(f_asm, "#PRNAME %s\n", v_name[$2]);}
        | DIRNAM STRING                    {fprintf(f_asm, "#DIRNAM %s\n", v_name[$2]);}
        | DATYPE INUM                      {fprintf(f_asm, "#DATYPE %s\n", v_name[$2]); prtype = atoi(v_name[$2]);}
        | NUBITS INUM                      {fprintf(f_asm, "#NUBITS %s\n", v_name[$2]);}
        | NBMANT INUM                      {fprintf(f_asm, "#NBMANT %s\n", v_name[$2]); nbmant = atoi(v_name[$2]);}
        | NBEXPO INUM                      {fprintf(f_asm, "#NBEXPO %s\n", v_name[$2]); nbexpo = atoi(v_name[$2]);}
        | NDSTAC INUM                      {fprintf(f_asm, "#NDSTAC %s\n", v_name[$2]);}
        | SDEPTH INUM                      {fprintf(f_asm, "#SDEPTH %s\n", v_name[$2]);}
        | NUIOIN INUM                      {fprintf(f_asm, "#NUIOIN %s\n", v_name[$2]);}
        | NUIOOU INUM                      {fprintf(f_asm, "#NUIOOU %s\n", v_name[$2]);};
        | NUGAIN INUM                      {fprintf(f_asm, "#NUGAIN %s\n", v_name[$2]);};

// Declaracao de variaveis ----------------------------------------------------

declar:   TYPE id_list ';'
        | TYPE ID '[' INUM ']' STRING ';'  {declar_var($2,$4,$6); v_asgn[$2] = 1;};

id_list:  IID
        | id_list ',' IID

IID:      ID                               {declar_var($1,-1,-1);}
        | ID '[' INUM ']'                  {declar_var($1,$3,-1); v_asgn[$1] = 1;};

// Declaracao de funcoes ------------------------------------------------------

funcao:   TYPE ID '('                      {declar_fun($1,$2);}
          par_list ')'                     {fprintf(f_asm, "SET %s\n", v_name[$5]); acc_id = $5;}
          '{' stmt_list '}'                {func_ret($2);}
        | TYPE ID '(' ')'                  {declar_fun($1,$2);}
          '{' stmt_list '}'                {func_ret($2);}

par_list:
          TYPE ID                          {$$ = declar_par($1,$2);}
        | par_list ',' par_list            {fprintf(f_asm, "SETP %s\n", v_name[$3]);};

// lista de statments em C ----------------------------------------------------

stmt_list: stmt      | stmt_list stmt;
stmt     : stmt_full | stmt_if;

stmt_full: ';'                  // statment vazio
         | '{' '}'              // statment vazio
         | '{' stmt_list '}'    // bloco de statments
         | declar_full          // declaracoes de variaveis
         | assignment           // atribuicao de expressoes a uma variavel
         | while_stmt           // loop while
         | ifelse_stmt          // if com else
         | std_out              // output de dados
         | void_call            // chamada de subrotina
         | return_call;         // retorno de funcao

// chamadas de funcoes --------------------------------------------------------

  void_call: ID '('                        { fun_id2 = $1;}
             exp_list ')' ';'              {     vcall($1);};
  func_call: ID '('                        { fun_id2 = $1;}
             exp_list ')'                  {$$ = fcall($1);};
return_call: RETURN exp ';'                {declar_ret($2);};

exp_list:  //vazio
        | exp                              {p_test = get_type($1);             par_check($1);}
        | exp_list ',' exp                 {p_test = p_test*10 + get_type($3); par_check($3);};

// Standard library -----------------------------------------------------------

std_out : OUT '(' exp ','                   {exec_out1($3);     }
         exp ')' ';'                        {exec_out2($6);     }
std_in  : IN  '(' exp ')'                   {$$ = exec_in($3);  }
std_abs : ABS  '(' exp ')'                  {$$ = exec_abs($3); }
std_sign :SIGN  '(' exp ','                 {exec_sign1($3);     }
         exp ')'                            {$$ = exec_sign2($6);}

// if else --------------------------------------------------------------------

stmt_if:  if_exp stmt                      {acc_id = -1; fprintf(f_asm, "@L%delse ", pop_lab());}
        | if_exp_stmt ELSE stmt_if         {acc_id = -1; fprintf(f_asm, "@L%dend " , pop_lab());}
        | while_if;

ifelse_stmt: if_exp_stmt ELSE stmt_full    {acc_id = -1; fprintf(f_asm, "@L%dend ", pop_lab());};
if_exp_stmt: if_exp stmt_full              {acc_id = -1; fprintf(f_asm, "JMP L%dend\n@L%delse ", get_lab(), get_lab());};

if_exp:   IF '(' exp ')'                   {load_check($3, 0); fprintf(f_asm, "JZ L%delse\n", push_lab()); acc_ok = 0; acc_id = -1;};

// while ----------------------------------------------------------------------

while_stmt: while_exp stmt_full            {acc_id = -1; fprintf(f_asm, "JMP L%d\n@L%dend ", pop_lab(), get_lab());};
while_if  : while_exp stmt_if              {acc_id = -1; fprintf(f_asm, "JMP L%d\n@L%dend ", pop_lab(), get_lab());};

while_exp: WHILE                           {acc_id = -1; fprintf(f_asm, "@L%d ", push_lab());}
          '(' exp ')'                      {load_check($4, 0); fprintf(f_asm, "JZ L%dend\n", get_lab()); acc_ok = 0;};

// declaracoes de variaveis ---------------------------------------------------

declar_full: declar
           | TYPE ID '=' exp ';'           {declar_var($2,-1,-1); var_set($2,$4,0,0);};
           | TYPE ID '@' exp ';'           {declar_var($2,-1,-1); var_set($2,$4,0,1);};
          // | TYPE ID "/>" exp ';'          {declar_var($2,-1,-1); var_set($2,$4,0,2);};



// assignments ----------------------------------------------------------------

assignment: ID '=' exp ';'                 {var_set($1,$3,0,0);}
          | ID '@' exp ';'                 {var_set($1,$3,0,1);}
          | ID NORM exp ';'                 {var_set($1,$3,0,2);}
          | ID '[' exp ']' '='             {array_check($1,$3);}
            exp ';'                        {var_set($1,$7,1,0);};



// expressoes -----------------------------------------------------------------

const:   INUM                         {                    $$ = load($1,1,1         ,0);}
        | '-' const                        {$$ = neg($2);}

exp:       const
         | FNUM                            {                    $$ = load($1,1,2         ,0);}
         | ID                              {                    $$ = load($1,0,v_type[$1],0);}
         | ID '[' exp ']'                  {array_check($1,$3); $$ = load($1,0,v_type[$1],1);}
         | std_in                          {$$ =     $1*OFST;}
         | std_abs                         {$$ =     $1*OFST;}
         | std_sign                        {$$ =     $1*OFST;}
         | func_call                       {$$ =     $1*OFST;}
         | '(' exp ')'                     {$$ =         $2 ;}
         | '+' exp                         {$$ =         $2 ;}
         | '-' exp                         {$$ = negacao($2);}
         | '!' exp                         {$$ = int_oper ($2, 0, "!"  , "LINV", 1);}
         | '~' exp                         {$$ = int_oper ($2, 0, "~"  ,  "INV", 0);}
         | exp '%' exp                     {$$ = int_oper ($1,$3, "%"  ,  "MOD", 0);}
         | exp  SHIFTL exp                 {$$ = int_oper ($1,$3, "<<" ,  "SHL", 0);}
         | exp  SHIFTR exp                 {$$ = int_oper ($1,$3, ">>" ,  "SHR", 0);}
         | exp SSHIFTR exp                 {$$ = int_oper ($1,$3, ">>>",  "SRS", 0);}
         | exp '&' exp                     {$$ = int_oper ($1,$3, "&"  ,  "AND", 0);}
         | exp '|' exp                     {$$ = int_oper ($1,$3, "|"  ,  "OR" , 0);}
         | exp '^' exp                     {$$ = int_oper ($1,$3, "^"  ,  "XOR", 0);}
         | exp LAND exp                    {$$ = int_oper ($1,$3, "&&" , "LAND", 1);}
         | exp LOR exp                     {$$ = int_oper ($1,$3, "||" , "LOR" , 1);}
         | exp '*' exp                     {$$ = operacoes($1,$3, "MLT", "CALL float_mult"                             , &fmlt);}
         | exp '/' exp                     {$$ = operacoes($1,$3, "DIV", "CALL float_div"                              , &fdiv);}
         | exp '+' exp                     {$$ = operacoes($1,$3, "ADD", "CALL denorm\nCALL float_add"                 , &fadd);}
         | exp '-' exp  {int r=negacao($3); $$ = operacoes($1, r, "ADD", "CALL denorm\nCALL float_add"                 , &fadd);}
         | exp '<' exp                     {     operacoes($1,$3, "LES", "CALL denorm\nLOAD float_aux3\nLES float_aux1", &fgen); $$ = OFST;}
         | exp '>' exp                     {     operacoes($1,$3, "GRE", "CALL denorm\nLOAD float_aux3\nGRE float_aux1", &fgen); $$ = OFST;}
         | exp GREQU exp                   {     operacoes($1,$3, "LES", "CALL denorm\nLOAD float_aux3\nLES float_aux1", &fgen); $$ = OFST; fprintf(f_asm, "LINV\n");}
         | exp LESEQ exp                   {     operacoes($1,$3, "GRE", "CALL denorm\nLOAD float_aux3\nGRE float_aux1", &fgen); $$ = OFST; fprintf(f_asm, "LINV\n");}
         | exp EQU exp                     {     operacoes($1,$3, "EQU", "CALL denorm\nLOAD float_aux3\nEQU float_aux1", &fgen); $$ = OFST;}
         | exp DIF exp                     {     operacoes($1,$3, "EQU", "CALL denorm\nLOAD float_aux3\nEQU float_aux1", &fgen); $$ = OFST; fprintf(f_asm, "LINV\n");}
         | NORM exp                        {$$ = int_oper ($2, 0, "/>"  , "NORM", 0);}



%%

int main(int argc, char *argv[])
{
  yyin   = fopen(argv[1], "r");
	f_asm  = fopen(argv[2], "w");

  float_init();
    //fprintf(f_asm, "LOAD 0\n");
	yyparse();
	fclose(yyin );
	fclose(f_asm);

	if (fgen && prtype == 0) float_gen(argv[2]);

	check_var();

	return 0;
}

void yyerror (char const *s)
{
	//fprintf (stderr, "Erro de sintaxe na linha %d\n", line_num+1);
	fprintf (stderr, "Po, presta atencao na sintaxe da linha %d\n", line_num+1);
}

int load(int id, int is_const, int dtype, int is_array)
{
    if (dtype == 0)
        //fprintf (stderr, "Erro na linha %d: variavel %s nao foi declarada.\n", line_num+1, rem_fname(v_name[id]));
        fprintf (stderr, "Erro na linha %d: mane, declara a variavel %s direito!\n", line_num+1, rem_fname(v_name[id], fname));

    if ((v_asgn[id] == 0) && (is_const == 0))
        //fprintf (stderr, "Erro na linha %d: variavel %s nao foi inicializada.\n", line_num+1, rem_fname(v_name[id]));
        fprintf (stdout, "Atencao na linha %d: como voce quer usar a variavel %s se voce nem deu um valor pra ela?\n", line_num+1, rem_fname(v_name[id], fname));

    if ((v_isar[id] == 1) && (is_array == 0))
        fprintf (stderr, "Erro na linha %d: cade o indice de array da variavel %s?\n", line_num+1, rem_fname(v_name[id], fname));

    if ((v_isar[id] == 0) && (is_array == 1))
        fprintf (stderr, "Erro na linha %d: %s nao eh array nao, mane!\n", line_num+1, rem_fname(v_name[id], fname));

    v_used[id] = 1;
    v_isco[id] = is_const;
    v_isar[id] = is_array;

    if (is_array == 1)
    {
         load_check(dtype*OFST+id, 0);
         return dtype*OFST;
    }
    else return dtype*OFST+id;
}

void load_check(int et, int sinal)
{
    int id = et % OFST;

    if (v_isar[id] == 1)
    {
      if(sinal == 0)
      {
        fprintf(f_asm, "PUSH\nSRF\nLOAD %s\n", v_name[id]);
      }
      else
      {
        fprintf(f_asm, "PUSH\nSRF\nLOAD -%s\n", v_name[id]);
      }
    }
    else
    {
        char num[64];

        if ((v_isco[id] == 1) && (et >= 2*OFST) && (prtype == 0))
        {
            itoa(f2mf(v_name[id]), num, 10);
            strcat(num, " // ");
            strcat(num, v_name[id]);
        }
        else strcpy(num, v_name[id]);

        if ((et != OFST) && (et != 2*OFST)) // se nao esta na pilha, carrega
        {
            if (acc_ok == 0)
            {
                /*if (acc_id != id)*/
                if(sinal == 0)
                {
                  fprintf(f_asm, "LOAD %s\n", num);
                }
                else
                {
                  fprintf(f_asm, "LOAD -%s\n", num);
                }
            }
            else
            {

                                // se acc carregado
                if(sinal == 0)
                {
                    fprintf(f_asm, "PLD %s\n", num);
                }
                else
                {
                    fprintf(f_asm, "PLD -%s\n", num);
                }
            }
        }
    }

    acc_ok = 1;
}

void func_ret(int id)
{
    if (((v_type[id] == 4) || (v_type[id] == 5)) && (ret_ok == 0))
        fprintf (stderr, "Erro na funcao %s: voce nao definiu nenhum retorno pra essa funcao.\n", v_name[id]);

    if (strcmp(v_name[id], "main") == 0)
    {
        if (mainok == 0)
            fprintf(f_asm, "@fim JMP fim\n");
        else
            fprintf(f_asm, "RETURN\n");

        v_used[id] = 1;
            mainok = 1;
    }
    else if (v_type[id] == 3) fprintf(f_asm, "RETURN\n");

    strcpy(fname, "");
}

void var_set(int id, int et, int is_array, int set_type) // set_type =0 ->SET; set_type =1 -> PSET; set_type =2 ->NORMS
{
    load_check(et,0);

    if (v_type[id] == 0)
    {
        //fprintf (stderr, "Erro na linha %d: variavel %s nao foi declarada.\n", line_num+1, rem_fname(v_name[id]));
        fprintf (stderr, "Erro na linha %d: se voce declarar a variavel %s eu agradeco.\n", line_num+1, rem_fname(v_name[id], fname));
        return;
    }

    if ((v_isar[id] == 0) && (is_array == 1))
    {
        fprintf (stderr, "Erro na linha %d: %s nao eh um array.\n", line_num+1, rem_fname(v_name[id], fname));
        return;
    }

    if ((v_isar[id] == 1) && (is_array == 0))
    {
        fprintf (stderr, "Erro na linha %d: %s eh um array. Cade o indice?\n", line_num+1, rem_fname(v_name[id], fname));
        return;
    }

    if ((v_type[id] == 1) && (et >= 2*OFST))
    {
        fprintf(stdout, "Atencao na linha %d: variavel %s eh int, mas recebe float.\n", line_num+1, rem_fname(v_name[id], fname));
        if (prtype == 0)
        {
            fprintf(f_asm, "CALL float2int\n");
            f2i = 1;
        }
    }

    if ((v_type[id] == 2) && (et < 2*OFST))
    {
        fprintf(stdout, "Atencao na linha %d: variavel %s eh float, mas recebe int.\n", line_num+1, rem_fname(v_name[id], fname));
        if (prtype == 0)
        {
            fprintf(f_asm, "CALL int2float\n");
            i2f = 1;
        }
    }

    char cset[10];
    if (is_array)
        strcpy(cset, "SRF\nSET");
    else if (set_type == 1)
            strcpy(cset, "PSET");
    else if (set_type == 2)
            strcpy(cset, "NORMS");
        else
        strcpy(cset, "SET");

    fprintf(f_asm, "%s %s\n", cset, v_name[id]);

    acc_ok     = 0;
    acc_id     = id;
    v_asgn[id] = 1;
}


int neg(int et)
{
    load_check(et, 1);
    return (et < 2*OFST) ? OFST : 2*OFST;

}

int negacao(int et)
{
    load_check(et, 0);

    if (prtype == 1)
        fprintf(f_asm, "NEG\n");
    else if (et < 2*OFST)
        fprintf(f_asm, "MLT -1\n");
    else
        fprintf(f_asm, "PLD 1\nPLD float_nbits\nSSHL\nSADD\n");

    return (et < 2*OFST) ? OFST : 2*OFST;
}

int iacc(int et)
{
    return (et == OFST) ? 1 : 0;
}

int imem(int et)
{
    return ((et > OFST) && (et < 2*OFST)) ? 1 : 0;
}

int facc(int et)
{
    return (et == 2*OFST) ? 1 : 0;
}

int fmem(int et)
{
    return (et > 2*OFST) ? 1 : 0;
}

int operacoes(int et1, int et2, char *iop, char *fop, int *op)
{
    if (prtype == 1)
    {
             if ((et1 % OFST != 0) && (et2 % OFST != 0))         // memoria e memoria
        {
            load_check(et2, 0);
            fprintf(f_asm, "%s %s\n", iop, v_name[et1 % OFST]);
        }
        else if ((et1 % OFST == 0) && (et2 % OFST != 0))         // acc e memoria
        {
            load_check(et2, 0);
            fprintf(f_asm, "S%s\n", iop);
        }
        else if ((et1 % OFST != 0) && (et2 % OFST == 0))         // memoria e acc
        {
            fprintf(f_asm, "%s %s\n", iop, v_name[et1 % OFST]);
        }
        else                                                     // pilha e acc
        {
            fprintf(f_asm, "S%s\n", iop);
        }
    }
    else
    {
             if (imem(et1) && imem(et2))                         // int memoria e int memoria
        {
            load_check(et2, 0);
            fprintf(f_asm, "%s %s\n", iop, v_name[et1 % OFST]);
        }
        else if (imem(et1) && fmem(et2))                         // int memoria e float memoria
        {
            load_check(et1, 0);
            fprintf(f_asm, "CALL int2float\n");
            load_check(et2, 0);
            fprintf(f_asm, "%s\n", fop);
            *op = 1;
            i2f = 1;
        }
        else if (fmem(et1) && imem(et2))                         // float memoria e int memoria
        {
            load_check(et1, 0);
            load_check(et2, 0);
            fprintf(f_asm, "CALL int2float\n");
            fprintf(f_asm, "%s\n", fop);
            *op = 1;
            i2f = 1;
        }
        else if (fmem(et1) && fmem(et2))                         // float memoria e float memoria
        {
            load_check(et1, 0);
            load_check(et2, 0);
            fprintf(f_asm, "%s\n", fop);
            *op = 1;
        }
        else if (iacc(et1) && imem(et2))                         // int acc e int memoria
        {
            load_check(et2, 0);
            fprintf(f_asm, "S%s\n", iop);
        }
        else if (iacc(et1) && fmem(et2))                         // int acc e float memoria
        {
            fprintf(f_asm, "CALL int2float\n");
            load_check(et2, 0);
            fprintf(f_asm, "%s\n", fop);
            *op = 1;
            i2f = 1;
        }
        else if (facc(et1) && imem(et2))                         // float acc e int memoria
        {
            load_check(et2, 0);
            fprintf(f_asm, "CALL int2float\n");
            fprintf(f_asm, "%s\n", fop);
            *op = 1;
            i2f = 1;
        }
        else if (facc(et1) && fmem(et2))                         // float acc e float memoria
        {
            load_check(et2, 0);
            fprintf(f_asm, "%s\n", fop);
            *op = 1;
        }
        else if (imem(et1) && iacc(et2))                         // int memoria e int acc
        {
            fprintf(f_asm, "%s %s\n", iop, v_name[et1 % OFST]);
        }
        else if (imem(et1) && facc(et2))                         // int memoria e float acc
        {
            if ((strcmp(iop, "DIV") == 0) ||
                (strcmp(iop, "LES") == 0) ||
                (strcmp(iop, "GRE") == 0))
                fprintf(f_asm, "SET float_aux5\nLOAD %s\nCALL int2float\nPLD float_aux5\n", v_name[et1 % OFST]);
            else
            {
                load_check(et1, 0);
                fprintf(f_asm, "CALL int2float\n");
            }
            fprintf(f_asm, "%s\n", fop);
            *op = 1;
            i2f = 1;
        }
        else if (fmem(et1) && iacc(et2))                         // float memoria e int acc
        {
            fprintf(f_asm, "CALL int2float\n");
            if ((strcmp(iop, "DIV") == 0) ||
                (strcmp(iop, "LES") == 0) ||
                (strcmp(iop, "GRE") == 0))
            {
                fprintf(f_asm, "SET float_aux4\n");
                acc_ok = 0;
                load_check(et1, 0);
                fprintf(f_asm, "PLD float_aux4\n");
            }
            else
                load_check(et1, 0);
            fprintf(f_asm, "%s\n", fop);
            *op = 1;
            i2f = 1;
        }
        else if (fmem(et1) && facc(et2))                         // float memoria e float acc
        {
            if ((strcmp(iop, "DIV") == 0) ||
                (strcmp(iop, "LES") == 0) ||
                (strcmp(iop, "GRE") == 0))
            {
                fprintf(f_asm, "SET float_aux4\n");
                acc_ok = 0;
                load_check(et1, 0);
                fprintf(f_asm, "PLD float_aux4\n");
            }
            else
                load_check(et1, 0);
            fprintf(f_asm, "%s\n", fop);
            *op = 1;
        }
        else if (iacc(et1) && iacc(et2))                         // int pilha e int acc
        {
            fprintf(f_asm, "S%s\n", iop);
        }
        else if (iacc(et1) && facc(et2))                         // int pilha e float acc
        {
            fprintf(f_asm, "SETP float_aux5\nCALL int2float\nPLD float_aux5\n%s\n", fop);
            *op = 1;
            i2f = 1;
        }
        else if (facc(et1) && iacc(et2))                         // float pilha e int acc
        {
            fprintf(f_asm, "CALL int2float\n");
            fprintf(f_asm, "%s\n", fop);
            *op = 1;
            i2f = 1;
        }
        else if (facc(et1) && facc(et2))                         // float pilha e float acc
        {
            fprintf(f_asm, "%s\n", fop);
            *op = 1;
        }
    }

    return ((et1 >= 2*OFST) || (et2 >= 2*OFST)) ? 2*OFST : OFST;
}

void declar_var (int id1, int id2, int id3) // id3 -> indice para pegar string, declaracao de array.
{
    if (v_type[id1] != 0) // variavel ja existe
    {
        //fprintf (stderr, "Erro na linha %d: variavel %s ja existe.\n", line_num+1, rem_fname(v_name[id1]));
        fprintf (stderr, "Erro na linha %d: puts, a variavel %s ja existe, ta doido?\n", line_num+1, rem_fname(v_name[id1], fname));
        return;
    }

    v_type[id1] = type_tmp;
    v_used[id1] = 0;
    v_asgn[id1] = 0;
    v_fnid[id1] = find_var(fname);

    if (id2 != -1) // significa que eh array
    {
        if(id3 == -1)
          fprintf(f_asm, "#array %s %s\n", v_name[id1], v_name[id2]);
        else
          fprintf(f_asm, "#arrays %s %s %s\n", v_name[id1], v_name[id2], v_name[id3]);
          v_isar[id1] = 1;
    }
}

int declar_par(int t, int id)
{
    declar_var(id,-1,-1);
    v_asgn[id] = 1;
    v_fpar[fun_id1] = v_fpar[fun_id1]*10 + t;

    return id;
}

void declar_fun(int id1, int id2) //id1 -> tipo id2 -> indice para o nome; v_name -> tabela com os nomes
{

    if ((mainok == 0) && (strcmp(v_name[id2], "main") != 0))
    {
        fprintf(f_asm, "CALL main\n@fim JMP fim\n");
        mainok = 2;
    }

    fprintf(f_asm, "@%s ", v_name[id2]); strcpy(fname, v_name[id2]);

    v_type[id2] = id1+3;
    fun_id1     = id2;
    ret_ok      = 0;
}

void declar_ret(int et)
{
    load_check(et, 0);

    if ((v_type[fun_id1] != 4) && (v_type[fun_id1] != 5))
        fprintf (stderr, "Erro na linha %d: retorno em funcao void? hi o cara!\n", line_num+1);

    if ((v_type[fun_id1] == 4) && (et >= 2*OFST))
    {
        fprintf(stdout, "Atencao na linha %d: convertendo float para int no retorno da funcao %s.\n", line_num+1, v_name[fun_id1]);
        if (prtype == 0)
        {
            fprintf(f_asm, "CALL float2int\n");
            f2i = 1;
        }
    }

    if ((v_type[fun_id1] == 5) && (et < 2*OFST))
    {
        fprintf(stdout, "Atencao na linha %d: convertendo int para float no retorno da funcao %s.\n", line_num+1, v_name[fun_id1]);
        if (prtype == 0)
        {
            fprintf(f_asm, "CALL int2float\n");
            i2f = 1;
        }
    }

    fprintf(f_asm, "RETURN\n");

    acc_ok = 0;
    ret_ok = 1;
}

int fcall(int id)
{
    if  (v_type[id] == 3)
        fprintf (stderr, "Erro na linha %d: funcao %s nao retorna nada.\n", line_num+1, rem_fname(v_name[id], fname));
    else if ((v_type[id] != 4) && (v_type[id] != 5))
        fprintf (stderr, "Erro na linha %d: nao estou achando a funcao %s.\n", line_num+1, rem_fname(v_name[id], fname));

    if (get_npar(p_test) != get_npar(v_fpar[id]))
        fprintf(stderr, "Erro na linha %d: lista de parametros da funcao %s difere da original.\n", line_num+1, rem_fname(v_name[id], fname));
    //fprintf (stderr, "Indice %d, nome %s\n",id, v_name[id]);
    fprintf(f_asm, "CALL %s\n",v_name[id]);
    //fprintf(f_asm, "CALL %s\n", rem_fname(v_name[id], fname));

    v_used[id] = 1;

    return v_type[id]-3;
}

void vcall(int id)
{
    if  ((v_type[id] != 3) && (v_type[id] != 4) && (v_type[id] != 5))
        fprintf (stderr, "Erro na linha %d: nao existe a funcao %s.\n", line_num+1, rem_fname(v_name[id], fname));

    if (get_npar(p_test) != get_npar(v_fpar[id]))
        fprintf(stderr, "Erro na linha %d: lista de parametros da funcao %s difere da original.\n", line_num+1, rem_fname(v_name[id], fname));

    fprintf(f_asm, "CALL %s\n", v_name[id]);

    v_used[id] = 1;
    acc_ok     = 0;
}

int int_oper(int et1, int et2, char *op, char *code, int fok)
{
    if  ((prtype == 1) && (fok == 0))
        fprintf(stderr, "Erro na linha %d: processador em ponto flutuante nao aceita operador %s. Arruma outra logica ai.\n", line_num+1, op);
    if ((prtype == 0) && ((et1 >= 2*OFST) || (et2 >= 2*OFST)))
        fprintf(stderr, "Erro na linha %d: uso incorreto do operador %s. Os operandos tem que ser do tipo int.\n", line_num+1, op);


    if (et2 == 0)
    {
        if (imem(et1)) load_check(et1, 0);
        fprintf(f_asm, "%s\n", code);
    }
    else
    {
        int aux;
        operacoes(et1, et2, code, "", &aux);
    }

    return OFST;
}

void array_check(int id, int et)
{
    if (v_isar[id] == 0)
    {
        fprintf(stderr, "Erro na linha %d: %s nao eh um array.\n", line_num+1, rem_fname(v_name[id], fname));
        return;
    }

    load_check(et, 0);

    if (et >= 2*OFST)
    {
        fprintf(stderr, "Atencao na linha %d: Indice de array tem que ser do tipo int. Estou convertendo float pra int aqui pra quebrar o teu galho.\n", line_num+1);
        if (prtype == 0)
        {
            fprintf(f_asm, "CALL float2int\n");
            f2i = 1;
        }
    }
}

int get_npar(int par)
{
    int t_fun = par;
    int n_par = 0;
    while (t_fun != 0)
    {
        t_fun = t_fun/10;
        n_par++;
    }
    return n_par;
}

void par_check(int et)
{
    // pega numero de parametros original
    int n_par = get_npar(v_fpar[fun_id2]);

    // pega tipo e posicao do parametro atual a ser chamado
    int t_cal = p_test;
    int aux   = p_test;
    int id_cal = n_par;
    int index = 1;
    while (aux > 10)
    {
        aux = aux /10;
        t_cal = t_cal % 10;
        id_cal--;
        index++;
    }

    // pega tipo do parametro atual na funcao original
    int t_fun = v_fpar[fun_id2];
    int i;
    for (i = 1; i < id_cal; i++)  t_fun = t_fun/10;
    t_fun = t_fun % 10;

    // ufa, agora sim, posso testar os parametros
    load_check(et, 0);

    if ((t_fun == 1) && (t_cal == 2))
    {
        fprintf(stdout, "Atencao na linha %d: convertendo float para int no parametro %d da funcao %s.\n", line_num+1, index, v_name[fun_id1]);
        if (prtype == 0)
        {
            fprintf(f_asm, "CALL float2int\n");
            f2i = 1;
        }
    }
    else if ((t_fun == 2) && (t_cal == 1))
    {
        fprintf(stdout, "Atencao na linha %d: convertendo int para float no parametro %d da funcao %s.\n", line_num+1, index, v_name[fun_id1]);
        if (prtype == 0)
        {
            fprintf(f_asm, "CALL int2float\n");
            i2f = 1;
        }
    }
}

int exec_in(int et)
{
    load_check(et, 0);

    if (et >= 2*OFST)
    {
        fprintf(stdout, "Atencao na linha %d: endereco de entrada tem que ser int. Soh me dando trabalho a toa!\n", line_num+1);
        if (prtype == 0)
        {
            fprintf(f_asm, "CALL float2int\n");
            f2i = 1;
        }
    }

    fprintf(f_asm, "PUSH\nIN\n");

    return (prtype == 0) ? 1 : 2;
}

int exec_abs(int et)
{
    load_check(et, 0);

    if (et >= 2*OFST)
    {
        //fprintf(stdout, "Atencao na linha %d: endereco de entrada tem que ser int. Soh me dando trabalho a toa!\n", line_num+1);
        if (prtype == 0)
        {
            fprintf(f_asm, "CALL float2int\n");
            f2i = 1;
        }
    }

    fprintf(f_asm, "ABS\n");

    return (prtype == 0) ? 1 : 2;
}

int exec_sign(int et, int et2)
{
    load_check(et, 0);
    load_check(et2, 0);

    if (et >= 2*OFST)
    {
        //fprintf(stdout, "Atencao na linha %d: endereco de entrada tem que ser int. Soh me dando trabalho a toa!\n", line_num+1);
        if (prtype == 0)
        {
            fprintf(f_asm, "CALL float2int\n");
            f2i = 1;
        }
    }

    fprintf(f_asm, "SIGN\n");

    return (prtype == 0) ? 1 : 2;
}

void exec_sign1(int et)
{
    load_check(et, 0);

    if (et >= 2*OFST)
    {
        fprintf(stdout, "Atencao na linha %d: endereco de saida tem que ser int. Soh me dando trabalho a toa!\n", line_num+1);
        if (prtype == 0)
        {
            fprintf(f_asm, "CALL float2int\n");
            f2i = 1;
        }
    }
}

int exec_sign2(int et)
{
    load_check(et, 0);

    if (et >= 2*OFST)
    {
        //fprintf(stdout, "Atencao na linha %d: endereco de entrada tem que ser int. Soh me dando trabalho a toa!\n", line_num+1);
        if (prtype == 0)
        {
            fprintf(f_asm, "CALL float2int\n");
            f2i = 1;
        }
    }

    fprintf(f_asm, "SIGN\n");

    return (prtype == 0) ? 1 : 2;
}

int get_type(int et)
{
    int t;
    if (et == 0)
        t = 0;
    else
        t = (et >= 2*OFST) ? 2 : 1;

    return t;
}

void exec_out1(int et)
{
    load_check(et, 0);

    if (et >= 2*OFST)
    {
        fprintf(stdout, "Atencao na linha %d: endereco de saida tem que ser int. Soh me dando trabalho a toa!\n", line_num+1);
        if (prtype == 0)
        {
            fprintf(f_asm, "CALL float2int\n");
            f2i = 1;
        }
    }
}

void exec_out2(int et)
{
    load_check(et, 0);

    if ((et >= 2*OFST) && (prtype == 0))
    {
        fprintf(stdout, "Atencao na linha %d: o processador eh ponto fixo e voce quer mandar um numero em ponto flutuante pra fora. Soh me dando dor de cabeca!\n", line_num+1);
        fprintf(f_asm, "CALL float2int\n");
        f2i = 1;
    }

    if ((et <  2*OFST) && (prtype == 1))
        fprintf(stdout, "Atencao na linha %d: o processador eh ponto flutuante e voce quer mandar um ponto fixo pra fora.\n", line_num+1);

    fprintf(f_asm, "OUT\n");

    acc_ok = 0;
}
