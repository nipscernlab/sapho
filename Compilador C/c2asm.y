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

%code requires
{
    #include "ast.h"
}

%union
{
    int lval; // os tokens terminais sao com inteiro
    Node  *N; // os reduzidos sao com noh
}

// tokens que nao tem atribuicao ----------------------------------------------

%token PRNAME DIRNAM DATYPE NUBITS NBMANT NBEXPO NDSTAC SDEPTH // diretivas
%token NUIOIN NUIOOU NUGAIN USEMAC ENDMAC FFTSIZ ITRADD        // diretivas
%token IN OUT NRM PST ABS SIGN                                 // std lib
%token WHILE IF ELSE RETURN BREAK                              // saltos
%token SHIFTL SHIFTR SSHIFTR                                   // desloc de bits
%token GREQU LESEQ EQU DIF LAND LOR                            // operadores logicos de dois simbolos
%token NORM EQNE                                               // assignments especiais de dois caracteres
%token PPLUS                                                   // operador ++. pode ser usado pra reduzir exp e tb pra assignments

// tokens com atribuicao ------------------------------------------------------

%token <lval> TYPE ID STRING INUM FNUM                         // simbolos

%type <N> prog prog_elements direct funcao fim declar IID id_list stmt_list stmt stmt_full stmt_if while_if
%type <N> wbreak declar_full assignment while_stmt ifelse_stmt std_out void_call return_call use_macro end_macro use_inter
%type <N> par_list exp std_in std_pst std_abs std_sign std_nrm func_call exp_list while_exp if_exp if_exp_stmt

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

fim : prog                          {$$ = $1; /*xuxa($$);*/}

// Programa e seus elementos --------------------------------------------------

prog : prog_elements                {$$ =                         $1 ;}
     | prog prog_elements           {$$ = create(0,0,NULL,$1,$2,NULL);}

prog_elements : direct              {$$ = $1;}
              | declar              {$$ = $1;}
              | funcao              {$$ = $1;}

// Diretivas de compilacao ----------------------------------------------------

direct : PRNAME  ID    {exec_diretivas("#PRNAME",$2,0); $$ = create( 1,0,NULL,NULL,NULL,NULL);}
       | DIRNAM STRING {exec_diretivas("#DIRNAM",$2,0); $$ = create( 2,0,NULL,NULL,NULL,NULL);}
       | DATYPE INUM   {exec_diretivas("#DATYPE",$2,1); $$ = create( 3,0,NULL,NULL,NULL,NULL);}
       | NUBITS INUM   {exec_diretivas("#NUBITS",$2,0); $$ = create( 4,0,NULL,NULL,NULL,NULL);}
       | NBMANT INUM   {exec_diretivas("#NBMANT",$2,2); $$ = create( 5,0,NULL,NULL,NULL,NULL);}
       | NBEXPO INUM   {exec_diretivas("#NBEXPO",$2,3); $$ = create( 6,0,NULL,NULL,NULL,NULL);}
       | NDSTAC INUM   {exec_diretivas("#NDSTAC",$2,0); $$ = create( 7,0,NULL,NULL,NULL,NULL);}
       | SDEPTH INUM   {exec_diretivas("#SDEPTH",$2,0); $$ = create( 8,0,NULL,NULL,NULL,NULL);}
       | NUIOIN INUM   {exec_diretivas("#NUIOIN",$2,0); $$ = create( 9,0,NULL,NULL,NULL,NULL);}
       | NUIOOU INUM   {exec_diretivas("#NUIOOU",$2,0); $$ = create(10,0,NULL,NULL,NULL,NULL);}
       | NUGAIN INUM   {exec_diretivas("#NUGAIN",$2,0); $$ = create(11,0,NULL,NULL,NULL,NULL);}
       | FFTSIZ INUM   {exec_diretivas("#FFTSIZ",$2,0); $$ = create(12,0,NULL,NULL,NULL,NULL);}

use_macro : USEMAC STRING {use_macro(v_name[$2]);       $$ = create(72,0,NULL,NULL,NULL,NULL);}
end_macro : ENDMAC        {end_macro(          );       $$ = create(73,0,NULL,NULL,NULL,NULL);}
use_inter : ITRADD        {use_inter(          );       $$ = create(74,0,NULL,NULL,NULL,NULL);}

// Declaracao de variaveis ----------------------------------------------------

declar : TYPE id_list ';'                             {                            $$ = create(14,0,NULL,NULL,  $2,NULL);}
       | TYPE ID '[' INUM ']'              STRING ';' {declar_arr_1d($2,$4,$6   ); $$ = create(18,0,NULL,NULL,NULL,NULL);}
       | TYPE ID '[' INUM ']' '[' INUM ']' STRING ';' {declar_arr_2d($2,$4,$7,$9); $$ = create(19,0,NULL,NULL,NULL,NULL);}

id_list :             IID                  {$$ =                          $1 ;}
        | id_list ',' IID                  {$$ = create(15,0,NULL,$1,$3,NULL);}

IID : ID                                   {declar_var   ($1         ); $$ = create(13,0,NULL,NULL,NULL,NULL);}
    | ID '[' INUM ']'                      {declar_arr_1d($1,$3   ,-1); $$ = create(16,0,NULL,NULL,NULL,NULL);}
    | ID '[' INUM ']' '[' INUM ']'         {declar_arr_2d($1,$3,$6,-1); $$ = create(17,0,NULL,NULL,NULL,NULL);}

// Declaracao de funcoes ------------------------------------------------------

funcao : TYPE ID '('                      {declar_fun     ($1,$2   );                                     }
         par_list ')'                     {declar_firstpar($5->ival);                                     } // seta o primeiro parametro na variavel correspondente
         '{' stmt_list '}'                {func_ret       ($2      ); $$ = create(23,0,NULL,$5  ,$9,NULL);} // checa se foi tudo ok
       | TYPE ID '('   ')'                {declar_fun     ($1,$2   );                                     } // funcao sem parametros
         '{' stmt_list '}'                {func_ret       ($2      ); $$ = create(20,0,NULL,NULL,$7,NULL);}

// lista de parametros na declaracao
// nao pode usar array em parametro de funcao
// retorna o id do parametro
par_list : TYPE ID                        {int ival = declar_par($1,$2   ); $$ = create(22,ival,NULL,NULL,NULL,NULL);}
         | par_list ',' par_list          {              set_par($3->ival); $$ = create(24,$1->ival,NULL,$1,$3,NULL);}; // vai pegando da pilha

// lista de statments em C ----------------------------------------------------

stmt_list: stmt              {$$ =                          $1 ;}
         | stmt_list stmt    {$$ = create(25,0,NULL,$1,$2,NULL);}

stmt     : stmt_full         {$$ = $1;}
         | stmt_if           {$$ = $1;}// fiz o if geral aqui separado. nao lembro pq. rever essa parte

// todos os statements que posso escrever dentro de uma funcao;
stmt_full: '{' stmt_list '}' {$$ = $2;} // bloco de statments
         |     declar_full   {$$ = $1;} // declaracoes de variaveis
         |     assignment    {$$ = $1;} // atribuicao de expressoes a uma variavel = $ @ />
         |      while_stmt   {$$ = $1;} // loop while
         |     ifelse_stmt   {$$ = $1;} // if com else
         |     std_out       {$$ = $1;} // std lib de output de dados
         |       void_call   {$$ = $1;} // chamada de subrotina
         |     return_call   {$$ = $1;} // retorno de funcao
         |     use_macro     {$$ = $1;} // diz que vai usar uma macro passada como parametro ate achar um ENDMAC
         |     end_macro     {$$ = $1;} // termina uma chamada de macro assembler
         |       wbreak      {$$ = $1;} // break; dentro do while
         |     use_inter     {$$ = $1;} // ponto de interrupcao

// chamadas de funcoes --------------------------------------------------------

// funcao void
void_call   : ID '('            {fun_id2  = $1;}                                                 // fun_id2 . id da funcao chamada
              exp_list ')' ';'  {vcall($1);            $$ = create(75, 0,NULL,NULL,NULL,NULL);}; // ja pode dar o call void
// funcao com retorno
func_call   : ID '('            {fun_id2  =       $1;}
              exp_list ')'      {int ival = fcall($1); $$ = create(37,ival,NULL,NULL,$4,NULL);}; // da call e retorna o tipo de dado final

return_call : RETURN exp ';'    {declar_ret($2->ival); $$ = create(41,   0,NULL,NULL,$2,NULL);}; // ainda nao implementei return pra void

// eh preciso colocar os parametros na pilha
// pra cada exp achado, o valor resultante eh gravado na pilha com par_exp
// o primeiro parametro fica no acumulador (pega de tras pra frente)
// par_exp xuxa parametros na pilha e checa se o parametro esta consistente
exp_list :                                 {                       $$ = create(40,0,NULL,NULL,NULL,NULL);} // pode ser vazio
         | exp                             {par_exp    ($1->ival); $$ = create(38,0,NULL,  $1,NULL,NULL);} // primeiro parametro
         | exp_list ',' exp                {par_listexp($3->ival); $$ = create(39,0,NULL,  $1,  $3,NULL);} // demais parametros

// Standard library -----------------------------------------------------------

std_out  : OUT  '(' exp ','                {           exec_out1($3->ival         );                                        }
                    exp ')' ';'            {           exec_out2($6->ival         ); $$ = create(43,0   ,NULL,  $3,$6,NULL);}
std_in   : IN   '(' exp ')'                {int ival = exec_in  ($3->ival         ); $$ = create(44,ival,NULL,NULL,$3,NULL);}
std_pst  : PST  '(' exp ')'                {int ival = exec_pst ($3->ival         ); $$ = create(45,ival,NULL,NULL,$3,NULL);}
std_abs  : ABS  '(' exp ')'                {int ival = exec_abs ($3->ival         ); $$ = create(46,ival,NULL,NULL,$3,NULL);}
std_sign : SIGN '(' exp ',' exp ')'        {int ival = exec_sign($3->ival,$5->ival); $$ = create(47,ival,NULL,$3  ,$5,NULL);}
std_nrm  : NRM  '(' exp ')'                {int ival = exec_norm($3->ival         ); $$ = create(48,ival,NULL,NULL,$3,NULL);}

// if else --------------------------------------------------------------------

// esse stmt_if eh confuso. Nao lembro como cheguei a isso, mas ta funcionando

stmt_if : if_exp stmt                      {if_expstmt("@L%delse "); $$ = create(94,0,NULL,$1,  $2,NULL);} // um if dentro do outro?
        | if_exp_stmt ELSE stmt_if         {if_expstmt("@L%dend " ); $$ = create(98,0,NULL,$1,  $3,NULL);} // um if dentro de um else
        | while_if                         {$$ = $1;}

ifelse_stmt : if_exp_stmt ELSE stmt_full   {if_expstmt("@L%dend " ); $$ = create(97,0,NULL,$1  ,$3,NULL);} // terminou um if/else completo
if_exp_stmt : if_exp           stmt_full   {if_expfull(           ); $$ = create(96,0,NULL,$1  ,$2,NULL);} // if (exp) {} padrao
if_exp      : IF '(' exp ')'               {if_expp   ($3->ival   ); $$ = create(95,0,NULL,NULL,$3,NULL);} // inicio (JZ)

// while ----------------------------------------------------------------------

while_stmt : while_exp stmt_full           {while_stmt();            $$ = create(93,0,NULL,  $1,  $2,NULL);}
while_if   : while_exp stmt_if             {while_stmt();            $$ = create(99,0,NULL,  $1,  $2,NULL);}

while_exp  : WHILE                         {while_expp();                                                  }
          '(' exp ')'                      {while_expexp($4->ival);  $$ = create(92,0,NULL,NULL,  $4,NULL);}

wbreak     : BREAK ';'                     {exec_break();            $$ = create(21,0,NULL,NULL,NULL,NULL);}

// declaracoes com assignment -------------------------------------------------

declar_full : declar                       {$$ = $1;}
            | TYPE ID '='     exp ';'      {declar_var($2); var_set($2,$4->ival,0,0); $$ = create(76,0,NULL,NULL,$4,NULL);}  // SET
            | TYPE ID '@'     exp ';'      {declar_var($2); var_set($2,$4->ival,0,1); $$ = create(77,0,NULL,NULL,$4,NULL);}  // PSETS (PSET + SET)
            | TYPE ID NORM    exp ';'      {declar_var($2); var_set($2,$4->ival,0,2); $$ = create(78,0,NULL,NULL,$4,NULL);}  // NORMS (NORM + SET)
            | TYPE ID '$'     exp ';'      {declar_var($2); var_set($2,$4->ival,0,3); $$ = create(79,0,NULL,NULL,$4,NULL);}  // ABSS  (ABS  + SET)
            | TYPE ID EQNE    exp ';'      {declar_var($2); var_set($2,$4->ival,0,4); $$ = create(80,0,NULL,NULL,$4,NULL);}  // NEGS  (NEG  + SET)

// assignments ----------------------------------------------------------------

           // atribuicao padrao
assignment : ID  '='    exp ';'              {var_set($1,$3->ival,0,0); $$ = create(32,0,NULL,NULL,$3,NULL);} // SET
           | ID  '@'    exp ';'              {var_set($1,$3->ival,0,1); $$ = create(33,0,NULL,NULL,$3,NULL);} // PSETS (PSET + SET)
           | ID NORM    exp ';'              {var_set($1,$3->ival,0,2); $$ = create(34,0,NULL,NULL,$3,NULL);} // NORMS (NORM + SET)
           | ID  '$'    exp ';'              {var_set($1,$3->ival,0,3); $$ = create(35,0,NULL,NULL,$3,NULL);} // ABSS  (ABS  + SET)
           | ID EQNE    exp ';'              {var_set($1,$3->ival,0,4); $$ = create(36,0,NULL,NULL,$3,NULL);} // NEGS  (NEG  + SET)
           // incremento e decremento
           | ID PPLUS       ';'              {pplus_assign($1); $$ = create(52,0,NULL,NULL,NULL,NULL);}
           // array normal
           | ID  '[' exp ']'  '='            {array_1d_check($1,$3->ival,0 /*baguncou o array essa separacao*/ );}
                     exp ';'                 {var_set       ($1,$7->ival,1,0); $$ = create(81,0,NULL,$3,$7,NULL);}
           | ID  '[' exp ']'  '@'            {array_1d_check($1,$3->ival,0                                     );}
                     exp ';'                 {var_set       ($1,$7->ival,1,1); $$ = create(82,0,NULL,$3,$7,NULL);}
           | ID  '[' exp ']' NORM            {array_1d_check($1,$3->ival,0                                     );}
                     exp ';'                 {var_set       ($1,$7->ival,1,2); $$ = create(83,0,NULL,$3,$7,NULL);}
           | ID  '[' exp ']'  '$'            {array_1d_check($1,$3->ival,0                                     );}
                     exp ';'                 {var_set       ($1,$7->ival,1,3); $$ = create(84,0,NULL,$3,$7,NULL);}
           | ID  '[' exp ']' EQNE            {array_1d_check($1,$3->ival,0                                     );}
                     exp ';'                 {var_set       ($1,$7->ival,1,4); $$ = create(85,0,NULL,$3,$7,NULL);}
           // array invertido
           | ID  '[' exp ')'  '='            {array_1d_check($1,$3->ival,2                                     );}
                     exp ';'                 {var_set       ($1,$7->ival,1,0); $$ = create(86,0,NULL,$3,$7,NULL);}
           | ID  '[' exp ')'  '@'            {array_1d_check($1,$3->ival,2                                     );}
                     exp ';'                 {var_set       ($1,$7->ival,1,1); $$ = create(87,0,NULL,$3,$7,NULL);}
           | ID  '[' exp ')' NORM            {array_1d_check($1,$3->ival,2                                     );}
                     exp ';'                 {var_set       ($1,$7->ival,1,2); $$ = create(88,0,NULL,$3,$7,NULL);}
           | ID  '[' exp ')'  '$'            {array_1d_check($1,$3->ival,2                                     );}
                     exp ';'                 {var_set       ($1,$7->ival,1,3); $$ = create(89,0,NULL,$3,$7,NULL);}
           | ID  '[' exp ')' EQNE            {array_1d_check($1,$3->ival,2                                     );}
                     exp ';'                 {var_set       ($1,$7->ival,1,4); $$ = create(90,0,NULL,$3,$7,NULL);}
           // array 2D (completar)
           | ID  '[' exp ']' '[' exp ']' '=' {array_2d_check($1, $3->ival,$6->ival);                             }
                     exp ';'                 {var_set       ($1,$10->ival,2,0);$$ = create(91,0,$3,$6,$10 ,NULL);}

// expressoes -----------------------------------------------------------------

// $$ gera o id extendido (et) da variavel
// se vem de uma reducao generica, o valor pode ser OFST ou 2*OFST (int ou float)
// se vem de uma variavel ou constante o valor eh OFST + id (int) ou 2*OFST + id (float)
// a regra aqui eh muito importante: cada reducao pra exp (menos variaveis e constantes) deve ser
// associada a um valor novo no acc!

         // constantes
exp:       INUM                               {int ival = num2exp($1,1); $$ = create(26,ival,NULL,NULL,NULL,NULL);}
         | FNUM                               {int ival = num2exp($1,2); $$ = create(27,ival,NULL,NULL,NULL,NULL);}
         // variaveis
         | ID                                 {int ival =      id2exp($1                  ); $$ = create(28,ival,NULL,NULL,NULL,NULL);}
         | ID '[' exp ']'                     {int ival = array1d2exp($1,$3->ival,0       ); $$ = create(29,ival,NULL,NULL,  $3,NULL);}
         | ID '[' exp ')'                     {int ival = array1d2exp($1,$3->ival,1       ); $$ = create(30,ival,NULL,NULL,  $3,NULL);}
         | ID '[' exp ']' '[' exp ']'         {int ival = array2d2exp($1,$3->ival,$6->ival); $$ = create(31,ival,NULL,  $3,  $6,NULL);}
         // std library
         | std_in                             {$$ = $1;}
         | std_pst                            {$$ = $1;}
         | std_abs                            {$$ = $1;}
         | std_sign                           {$$ = $1;}
         | std_nrm                            {$$ = $1;}
         | func_call                          {$$ = $1;}
         // operadores nulos
         |     '('     exp ')'                {$$ = $2;}
         |     '+'     exp                    {$$ = $2;}
         // operadores unarios
         |     '-'     exp                    {int ival =   negacao($2->ival                       ); $$ = create(42,ival,NULL,NULL,$2,NULL);}
         |     '!'     exp                    {int ival =  int_oper($2->ival, 0,      "!" ,"LINV",1); $$ = create(49,ival,NULL,NULL,$2,NULL);}
         |     '~'     exp                    {int ival =  int_oper($2->ival, 0,      "~" , "INV",0); $$ = create(50,ival,NULL,NULL,$2,NULL);}
         | exp PPLUS                          {int ival = exp_pplus($1->ival                       ); $$ = create(51,ival,NULL,$1,NULL,NULL);}
         // operadores logicos
         | exp  SHIFTL exp                    {int ival = int_oper($1->ival,$3->ival,"<<" , "SHL",0); $$ = create(53,ival,NULL,$1,$3,NULL);}
         | exp  SHIFTR exp                    {int ival = int_oper($1->ival,$3->ival,">>" , "SHR",0); $$ = create(54,ival,NULL,$1,$3,NULL);}
         | exp SSHIFTR exp                    {int ival = int_oper($1->ival,$3->ival,">>>", "SRS",0); $$ = create(55,ival,NULL,$1,$3,NULL);}
         | exp '&'     exp                    {int ival = int_oper($1->ival,$3->ival, "&" , "AND",0); $$ = create(56,ival,NULL,$1,$3,NULL);}
         | exp '|'     exp                    {int ival = int_oper($1->ival,$3->ival, "|" ,  "OR",0); $$ = create(57,ival,NULL,$1,$3,NULL);}
         | exp '^'     exp                    {int ival = int_oper($1->ival,$3->ival, "^" , "XOR",0); $$ = create(58,ival,NULL,$1,$3,NULL);}
         | exp LAND    exp                    {int ival = int_oper($1->ival,$3->ival, "&&","LAND",1); $$ = create(59,ival,NULL,$1,$3,NULL);}
         | exp LOR     exp                    {int ival = int_oper($1->ival,$3->ival, "||", "LOR",1); $$ = create(60,ival,NULL,$1,$3,NULL);}
         // operadore aritmetico
         | exp '%'     exp                    {int ival = int_oper($1->ival,$3->ival,  "%", "MOD",0); $$ = create(61,ival,NULL,$1,$3,NULL);}
         | exp '*'     exp                    {int ival = oper_ari($1->ival,$3->ival             ,0); $$ = create(62,ival,NULL,$1,$3,NULL);}
         | exp '/'     exp                    {int ival = oper_ari($1->ival,$3->ival             ,1); $$ = create(63,ival,NULL,$1,$3,NULL);}
         | exp '+'     exp                    {int ival = oper_ari($1->ival,$3->ival             ,2); $$ = create(64,ival,NULL,$1,$3,NULL);}
         | exp '-'     exp                    {int ival = oper_ari($1->ival,$3->ival             ,3); $$ = create(65,ival,NULL,$1,$3,NULL);}
         // operadores de comparacao
         | exp '<'     exp                    {int ival = oper_cmp($1->ival,$3->ival             ,0); $$ = create(66,ival,NULL,$1,$3,NULL);}
         | exp '>'     exp                    {int ival = oper_cmp($1->ival,$3->ival             ,1); $$ = create(67,ival,NULL,$1,$3,NULL);}
         | exp GREQU   exp                    {int ival = oper_cmp($1->ival,$3->ival             ,2); $$ = create(68,ival,NULL,$1,$3,NULL);}
         | exp LESEQ   exp                    {int ival = oper_cmp($1->ival,$3->ival             ,3); $$ = create(69,ival,NULL,$1,$3,NULL);}
         | exp EQU     exp                    {int ival = oper_cmp($1->ival,$3->ival             ,4); $$ = create(70,ival,NULL,$1,$3,NULL);}
         | exp DIF     exp                    {int ival = oper_cmp($1->ival,$3->ival             ,5); $$ = create(71,ival,NULL,$1,$3,NULL);}

%%

int main(int argc, char *argv[])
{
    yyin   = fopen(argv[1], "r");
    f_asm  = fopen(argv[2], "w");

    // da problema com o reset se nao colocar isso se
    // a primeira instrucao for CALL main. Resolver ...
    fprintf(f_asm, "LOAD NULL\n");

    using_macro  = 0;
    exec_fft_use = 0;
    exec_fft_set = 0;
    prtype       = 0;
    acc_ok       = 0;
    ret_ok       = 0;
    mainok       = 0;

    float_init (); // inicializa variaveis de estado (t2t.c)
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
