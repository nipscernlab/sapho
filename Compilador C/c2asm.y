/*
    Coisas que só tem em C+-

    - Diretivas #USEMAC #ENDMAC: seleciona trechos do código que serão substituidos por Macros otimizados em assembly
    - Diretiva  #INTERPOIN     : marca ponto de retorno para um reset no pino itr

    - StdLib   in(.,.): leitura de dados externos
    - StdLib  out(.,.): escrita pra fora do processador
    - StdLib   norm(.): funcao que divide o argumento pela constante dada por #NUGAIN (evita usar o circuito de divisao da ULA)
    - StdLib   pset(.): funcao que retorna zero se o argumento for negativo (evita if(x<0) x = 0;)
    - StdLib    abs(.): funcao que retorna o valor absoluto (evita if(x<0) x = -x;)
    - StdLib sign(.,.): retorna o segundo argumento com o sinal do primeiro (evita muito codigo, faz ele ai pra vc ver)

    - Operador   >>>  : deslocamento a direta com complemento a dois (desloca mantendo o sinal)

    - Atribuicao />   : seta uma variavel com o valor à direta dividido pelo valor em #NUGAIN (ex: x /> exp;) (evita x = exp/NUGAIN;)
    - Atribuicao @    : seta uma variavel com zero, caso a expressao a direita dê negativo (ex: x @ exp;) (evita if(exp<0) x = 0; else x = exp;)
    - Atribuicao $    : seta uma variavel com o valor absoluto da expressao a direita (ex: x $ exp) (evita if(exp<0) x = -exp; else x = exp;)
    - Atribuicao =-   : seta uma variavel com o negativo do valor passado (ex: x =- exp;) (evita x = -exp;)

    - Array inicializavel por arquivo. A memoria do array ja eh preenchida em tempo de compilacao. (ex: int x[128] "valores.txt";)
    - Array com indice invertido. Usado em FFT (ex: x[i) = exp;) os bits de i sao invertidos. (usar com arrays complexos - real seguido do imaginario)
*/

%{

#include "variaveis.h"   // tabela de variaveis
#include "labels.h"      // geracao de labels para saltos
#include "t2t.h"         // conversoes de tipos
#include "funcoes.h"     // criacao e uso de funcoes
#include "oper.h"        // operacoes da ULA
#include "stdlib.h"      // biblioteca padrao do sapho
#include "diretivas.h"   // diretivas de compilacao
#include "saltos.h"      // gerenciamento saltos (if/else while)
#include "data_declar.h" // declaracao de dados
#include "data_use.h"    // utilizacao de dados
#include "data_assign.h" // atribuicao de dados

// variaveis obrigatorias do flex/bison

FILE *yyin;
int   yylex  (void);
void  yyerror(char const *s);

%}

// tokens que nao tem atribuicao ----------------------------------------------

%token PRNAME DIRNAM DATYPE NUBITS NBMANT NBEXPO NDSTAC SDEPTH // diretivas
%token NUIOIN NUIOOU NUGAIN USEMAC ENDMAC FFTSIZ ITRADD        // diretivas
%token IN OUT NRM PST ABS SIGN                                 // std lib
%token WHILE IF ELSE SWITCH CASE DEFAULT RETURN BREAK          // saltos
%token SHIFTL SHIFTR SSHIFTR                                   // deslocamento de bits
%token GREQU LESEQ EQU DIF LAND LOR                            // operadores logicos de dois simbolos
%token NORM EQNE                                               // assignments especiais de dois caracteres
%token PPLUS                                                   // operador ++. pode ser usado pra reduzir exp e tb pra assignments

// tokens terminais -----------------------------------------------------------

%token TYPE ID STRING INUM FNUM                                // vem do lexer com um valor associado

// importante para lista de parametros de uma funcao
// o primeiro parametro eh o ultimo a ser parseado
%right ','

// mais embaixo -> mais prioritario (usa padrao C)
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
%left '!' '~' PPLUS

%%

// Programa e seus elementos --------------------------------------------------

fim  : prog
prog : prog_elements | prog prog_elements

prog_elements : direct | declar | funcao

// Diretivas de compilacao ----------------------------------------------------

direct : PRNAME  ID    {exec_diretivas("#PRNAME",$2,0);} // nome do processador
       | DIRNAM STRING {exec_diretivas("#DIRNAM",$2,0);} // diretorio
       | DATYPE INUM   {exec_diretivas("#DATYPE",$2,1);} // tipo: 0 -> ponto fixo, 1 -> ponto flutuante
       | NUBITS INUM   {exec_diretivas("#NUBITS",$2,0);} // tamanho da palavra da ULA
       | NBMANT INUM   {exec_diretivas("#NBMANT",$2,2);} // numero de bits da mantissa
       | NBEXPO INUM   {exec_diretivas("#NBEXPO",$2,3);} // numero de bits do expoente
       | NDSTAC INUM   {exec_diretivas("#NDSTAC",$2,0);} // tamanho da pilha de dados
       | SDEPTH INUM   {exec_diretivas("#SDEPTH",$2,0);} // tamanho da pilha de subrotina
       | NUIOIN INUM   {exec_diretivas("#NUIOIN",$2,4);} // numero de portas de entrada
       | NUIOOU INUM   {exec_diretivas("#NUIOOU",$2,5);} // numero de portas de saida
       | NUGAIN INUM   {exec_diretivas("#NUGAIN",$2,0);} // contante de divisao (norm(.))
       | FFTSIZ INUM   {exec_diretivas("#FFTSIZ",$2,0);} // tamanho da FFT (2^FFTSIZ)

// Diretivas comportamentais --------------------------------------------------

use_macro : USEMAC STRING {use_macro(v_name[$2]);} // usa uma macro .asm no lugar do compilador
end_macro : ENDMAC        {end_macro(          );} // ponto final de uso de uma macro
use_inter : ITRADD        {use_inter(          );} // ponto de inicio da interrupcao (usado com o pino itr)

// Declaracao de variaveis ----------------------------------------------------

declar : TYPE id_list ';'
       | TYPE ID '[' INUM ']'              STRING ';' {declar_arr_1d($2,$4,$6   );}
       | TYPE ID '[' INUM ']' '[' INUM ']' STRING ';' {declar_arr_2d($2,$4,$7,$9);}

id_list : IID | id_list ',' IID

IID : ID                                  {declar_var   ($1         );}
    | ID '[' INUM ']'                     {declar_arr_1d($1,$3   ,-1);}
    | ID '[' INUM ']' '[' INUM ']'        {declar_arr_2d($1,$3,$6,-1);}

// Declaracao de funcoes ------------------------------------------------------

funcao : TYPE ID '('                      {declar_fun     ($1,$2);} // inicio da declaracao de uma funcao
         par_list ')'                     {declar_firstpar($5   );} // seta o primeiro parametro na variavel correspondente
         '{' stmt_list '}'                {func_ret       ($2   );} // checa se foi tudo ok
       | TYPE ID '('   ')'                {declar_fun     ($1,$2);} // funcao sem parametros
         '{' stmt_list '}'                {func_ret       ($2   );}

// lista de parametros na declaracao
// nao pode usar array em parametro de funcao
// retorna o id do parametro
par_list : TYPE ID                        {$$ = declar_par($1,$2);}
         | par_list ',' par_list          {        set_par($3   );}; // vai pegando da pilha

// lista de statments em C ----------------------------------------------------

stmt_list: stmt | stmt_list stmt

stmt     : stmt_full
         | stmt_if // fiz o if geral aqui separado. nao lembro pq. rever essa parte

// todos os statements que posso escrever dentro de uma funcao
stmt_full: '{' stmt_list '}' // bloco de statments
         |     stmt_case     // todos os tipos de stmts aceitos no case
         |     switch_case   // switch case
         |       wbreak      // break; dentro do while
         |     use_inter     // ponto de interrupcao

// statments que podem ser usados dentro do case
stmt_case:     declar_full   // declaracoes de variaveis
         |     assignment    // atribuicao de expressoes a uma variavel = $ @ />
         |      while_stmt   // loop while
         |     ifelse_stmt   // if com else
         |     std_out       // std lib de output de dados
         |       void_call   // chamada de subrotina
         |     return_call   // retorno de funcao
         |     use_macro     // diz que vai usar uma macro passada como parametro ate achar um ENDMAC
         |     end_macro     // termina uma chamada de macro assembler

// chamadas de funcoes --------------------------------------------------------

// funcao void
void_call   : ID '('            {fun_id2  = $1 ;} // fun_id2 -> id da funcao chamada
              exp_list ')' ';'  {vcall     ($1);} // ja pode dar o call void
// funcao com retorno
func_call   : ID '('            {fun_id2  = $1 ;}
              exp_list ')'      {$$ = fcall($1);} // da call e retorna o tipo de dado final

return_call : RETURN exp ';'    {declar_ret($2);}
            | RETURN     ';'    // ainda nao implementei return pra void

// eh preciso colocar os parametros na pilha
// pra cada exp achado, o valor resultante eh gravado na pilha com par_exp
// o primeiro parametro fica no acumulador (pega de tras pra frente)
// par_exp xuxa parametros na pilha e checa se o parametro esta consistente
exp_list :                                                    // pode ser vazio
         | exp                             {par_exp    ($1);} // primeiro parametro
         | exp_list ',' exp                {par_listexp($3);} // demais parametros

// Standard library -----------------------------------------------------------

std_out  : OUT  '(' exp ','                {     exec_out1($3   );} // saida de dados
                    exp ')' ';'            {     exec_out2($6   );}
std_in   : IN   '(' exp ')'                {$$ = exec_in  ($3   );} // entrada de dados
std_pst  : PST  '(' exp ')'                {$$ = exec_pst ($3   );} // funcao pset(x)   -> zera se negativo
std_abs  : ABS  '(' exp ')'                {$$ = exec_abs ($3   );} // funcao  abs(x)   -> valor absoluto de x
std_sign : SIGN '(' exp ',' exp ')'        {$$ = exec_sign($3,$5);} // funcao sign(x,y) -> pega o sinal de x e coloca em y
std_nrm  : NRM  '(' exp ')'                {$$ = exec_norm($3   );} // funcao norm(x)   -> divide x pela constante NUGAIN

// if else --------------------------------------------------------------------

// esse stmt_if eh confuso. recomendado pelo bison pra nao aparecer conflito shift/reduce
// ex: if (exp) if (exp) stmt else stmt

stmt_if : if_exp stmt                      {if_expstmt("@L%delse ");} // um if dentro do outro sem else
        | if_exp_stmt ELSE stmt_if         {if_expstmt("@L%dend " );} // um if dentro de um else
        | while_if                                                    // um if dentro de um while

ifelse_stmt : if_exp_stmt ELSE stmt_full   {if_expstmt("@L%dend " );} // terminou um if/else completo
if_exp_stmt : if_exp           stmt_full   {if_expfull(           );} // if (exp) {} padrao
if_exp      : IF '(' exp ')'               {if_expp   ($3         );} // inicio (JZ)

// switch case ----------------------------------------------------------------

switch_case : SWITCH '(' exp ')' {exec_switch($3);}
              '{' cases '}'      { end_switch(  );}

cases       : case | default | case cases

case        : CASE INUM ':' {  case_test($2,1);} case_list
            | CASE FNUM ':' {  case_test($2,2);} case_list
default     : DEFAULT   ':' {defaut_test(    );} case_list

case_list   :           stmt_case
            | case_list stmt_case
            | case_list BREAK ';' {switch_break();}

// while ----------------------------------------------------------------------

while_stmt : while_exp stmt_full           {while_stmt();}
while_if   : while_exp stmt_if             {while_stmt();}

while_exp  : WHILE                         {while_expp  (  );}
          '(' exp ')'                      {while_expexp($4);}

wbreak     : BREAK ';'                     {exec_break();}

// declaracoes com assignment -------------------------------------------------

declar_full : declar
            | TYPE ID '='     exp ';'      {declar_var($2); var_set($2,$4,0,0);} // SET
            | TYPE ID '@'     exp ';'      {declar_var($2); var_set($2,$4,0,1);} // PSETS (PSET + SET)
            | TYPE ID NORM    exp ';'      {declar_var($2); var_set($2,$4,0,2);} // NORMS (NORM + SET)
            | TYPE ID '$'     exp ';'      {declar_var($2); var_set($2,$4,0,3);} // ABSS  (ABS  + SET)
            | TYPE ID EQNE    exp ';'      {declar_var($2); var_set($2,$4,0,4);} // NEGS  (NEG  + SET)

// assignments ----------------------------------------------------------------

           // atribuicao padrao
assignment : ID  '='    exp ';'               {var_set($1,$3,0,0);} // SET
           | ID  '@'    exp ';'               {var_set($1,$3,0,1);} // PSETS (PSET + SET)
           | ID NORM    exp ';'               {var_set($1,$3,0,2);} // NORMS (NORM + SET)
           | ID  '$'    exp ';'               {var_set($1,$3,0,3);} // ABSS  (ABS  + SET)
           | ID EQNE    exp ';'               {var_set($1,$3,0,4);} // NEGS  (NEG  + SET)
           // incremento
           | ID                          PPLUS ';' {pplus_assign($1      );}
           | ID  '[' exp ']'             PPLUS ';' {aplus_assign($1,$3   );}
           | ID  '[' exp ']' '[' exp ']' PPLUS ';' {aplu2_assign($1,$3,$6);}
           // array normal
           | ID  '[' exp ']'  '='             {array_1d_check($1,$3,0  );} // baguncou o array essa separacao
                     exp ';'                  {var_set       ($1,$7,1,0);} // tentar juntar
           | ID  '[' exp ']'  '@'             {array_1d_check($1,$3,0  );}
                     exp ';'                  {var_set       ($1,$7,1,1);}
           | ID  '[' exp ']' NORM             {array_1d_check($1,$3,0  );}
                     exp ';'                  {var_set       ($1,$7,1,2);}
           | ID  '[' exp ']'  '$'             {array_1d_check($1,$3,0  );}
                     exp ';'                  {var_set       ($1,$7,1,3);}
           | ID  '[' exp ']' EQNE             {array_1d_check($1,$3,0  );}
                     exp ';'                  {var_set       ($1,$7,1,4);}
           // array invertido
           | ID  '[' exp ')'  '='             {array_1d_check($1,$3,2  );}
                     exp ';'                  {var_set       ($1,$7,1,0);}
           | ID  '[' exp ')'  '@'             {array_1d_check($1,$3,2  );}
                     exp ';'                  {var_set       ($1,$7,1,1);}
           | ID  '[' exp ')' NORM             {array_1d_check($1,$3,2  );}
                     exp ';'                  {var_set       ($1,$7,1,2);}
           | ID  '[' exp ')'  '$'             {array_1d_check($1,$3,2  );}
                     exp ';'                  {var_set       ($1,$7,1,3);}
           | ID  '[' exp ')' EQNE             {array_1d_check($1,$3,2  );}
                     exp ';'                  {var_set       ($1,$7,1,4);}
           // array 2D (completar)
           | ID  '[' exp ']' '[' exp ']' '='  {array_2d_check($1, $3, $6  );}
                     exp ';'                  {var_set       ($1,$10,  2,0);}
           | ID  '[' exp ']' '[' exp ']' '@'  {array_2d_check($1, $3, $6  );}
                     exp ';'                  {var_set       ($1,$10,  2,1);}
           | ID  '[' exp ']' '[' exp ']' NORM {array_2d_check($1, $3, $6  );}
                     exp ';'                  {var_set       ($1,$10,  2,2);}
           | ID  '[' exp ']' '[' exp ']' '$'  {array_2d_check($1, $3, $6  );}
                     exp ';'                  {var_set       ($1,$10,  2,3);}
           | ID  '[' exp ']' '[' exp ']' EQNE {array_2d_check($1, $3, $6  );}
                     exp ';'                  {var_set       ($1,$10,  2,4);}

// expressoes -----------------------------------------------------------------

// $$ gera o id extendido (et) da variavel
// se vem de uma reducao generica, o valor pode ser OFST ou 2*OFST (int ou float)
// se vem de uma variavel ou constante o valor eh OFST + id (int) ou 2*OFST + id (float)
// a regra aqui eh muito importante: cada reducao pra exp (menos variaveis e constantes) deve ser
// associada a um valor novo no acc!

         // constantes
exp:       INUM                               {$$ = num2exp($1,1);}
         | FNUM                               {$$ = num2exp($1,2);}
         // variaveis
         | ID                                 {$$ =      id2exp($1      );}
         | ID '[' exp ']'                     {$$ = array1d2exp($1,$3,0 );}
         | ID '[' exp ')'                     {$$ = array1d2exp($1,$3,1 );}
         | ID '[' exp ']' '[' exp ']'         {$$ = array2d2exp($1,$3,$6);}
         // std library
         | std_in                             {$$ = $1;}
         | std_pst                            {$$ = $1;}
         | std_abs                            {$$ = $1;}
         | std_sign                           {$$ = $1;}
         | std_nrm                            {$$ = $1;}
         | func_call                          {$$ = $1;}
         // operadores nulos
         |    '(' exp ')'                     {$$ = $2;}
         |    '+' exp                         {$$ = $2;}
         // operadores unarios
         |    '-' exp                         {$$ =     negacao($2                );}
         |    '!' exp                         {$$ =    int_oper($2,0,"!" ,"LINV",1);}
         |    '~' exp                         {$$ =    int_oper($2,0,"~" , "INV",0);}
         | ID                         PPLUS   {$$ =   exp_pplus($1                );}
         | ID '[' exp ']'             PPLUS   {$$ = array_pplus($1,$3             );}
         | ID '[' exp ']' '[' exp ']' PPLUS   {$$ = array_2plus($1,$3,$6          );}
         // operadores logicos
         | exp  SHIFTL exp                    {$$ = int_oper($1,$3,"<<" , "SHL",0);}
         | exp  SHIFTR exp                    {$$ = int_oper($1,$3,">>" , "SHR",0);}
         | exp SSHIFTR exp                    {$$ = int_oper($1,$3,">>>", "SRS",0);}
         | exp '&'     exp                    {$$ = int_oper($1,$3, "&" , "AND",0);}
         | exp '|'     exp                    {$$ = int_oper($1,$3, "|" ,  "OR",0);}
         | exp '^'     exp                    {$$ = int_oper($1,$3, "^" , "XOR",0);}
         | exp LAND    exp                    {$$ = int_oper($1,$3, "&&","LAND",1);}
         | exp LOR     exp                    {$$ = int_oper($1,$3, "||", "LOR",1);}
         // operadore aritmetico
         | exp '%'     exp                    {$$ = int_oper($1,$3,  "%", "MOD",0);}
         | exp '*'     exp                    {$$ = oper_ari($1,$3             ,0);}
         | exp '/'     exp                    {$$ = oper_ari($1,$3             ,1);}
         | exp '+'     exp                    {$$ = oper_ari($1,$3             ,2);}
         | exp '-'     exp                    {$$ = oper_ari($1,$3             ,3);}
         // operadores de comparacao
         | exp '<'     exp                    {$$ = oper_cmp($1,$3,0);}
         | exp '>'     exp                    {$$ = oper_cmp($1,$3,1);}
         | exp GREQU   exp                    {$$ = oper_cmp($1,$3,2);}
         | exp LESEQ   exp                    {$$ = oper_cmp($1,$3,3);}
         | exp EQU     exp                    {$$ = oper_cmp($1,$3,4);}
         | exp DIF     exp                    {$$ = oper_cmp($1,$3,5);}

%%

int main(int argc, char *argv[])
{
    yyin   = fopen(argv[1], "r");
    f_asm  = fopen(argv[2], "w");

    // da problema com o reset se nao colocar isso se
    // a primeira instrucao for CALL main. Resolver ...
    fprintf(f_asm, "LOAD NULL\n");

    // iniciaiza variaveis de estado
    using_macro  = 0;
    exec_fft_use = 0;
    exec_fft_set = 0;
    prtype       = 0;
    acc_ok       = 0;
    ret_ok       = 0;
    mainok       = 0;

    float_init (); // inicializa variaveis de estado pra float (t2t.c)
	yyparse    (); // aqui a magica acontece!!
	fclose(yyin );
	fclose(f_asm);

	// carrega macros de ponto flutuante pra proc de ponto fixo
	// caso precise (espero que nao)
	if (fgen && prtype == 0) float_gen(argv[2]); //(t2t.c)

	// checa consistencia de todas as variaveis e funcoes
	check_var(); // (variaveis.c)

	return 0;
}

// erro de sintaxes do bison
void yyerror (char const *s)
{
	fprintf (stderr, "Pô, presta atenção na sintaxe da linha %d\n", line_num+1);
}
