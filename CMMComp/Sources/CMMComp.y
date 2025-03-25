/*
    Coisas que só tem em C+-

    - Diretivas #USEMAC #ENDMAC: seleciona trechos do código que serão substituidos por Macros otimizados em assembly
    - Diretiva  #INTERPOINT     : marca ponto de retorno para um reset no pino itr

    - tipo de dados comp (para números complexos) : ex: comp a = 3+4i;

    - StdLib   in(.,.): leitura de dados externos
    - StdLib  out(.,.): escrita pra fora do processador
    - StdLib   norm(.): função que divide o argumento pela constante dada por #NUGAIN (evita usar o circuito de divisão da ULA)
    - StdLib   pset(.): função que retorna zero se o argumento for negativo (evita if(x<0) x = 0;)
    - StdLib    abs(.): função que retorna o valor absoluto (evita if(x<0) x = -x;). Se for complexo, retorna o módulo
    - StdLib   sqrt(.): retorna raiz quadrada. Gera um float
    - StdLib   atan(.): retorna o arco-tg. Gera um float
    - StdLib sign(.,.): retorna o segundo argumento com o sinal do primeiro (evita muito codigo, faz ele aí pra vc ver)
    - StdLib   real(.): retorna a parte real de um número complexo
    - StdLib   imag(.): retorna a parte imag de um número complexo
    - StdLib   fase(.): retorna a fase de um número complexo

    - Operador   >>>  : deslocamento é direta com complemento a dois (desloca mantendo o sinal)

    - Array inicializável por arquivo. A memória do array já é preenchida em tempo de compilação. (ex: int x[128] "valores.txt";)
    - Array com índice invertido. Usado em FFT (ex: x[j) = exp;) os bits de i sâo invertidos.
*/

%{

#include "..\Headers\variaveis.h"   // tabela de variaveis
#include "..\Headers\labels.h"      // geracao de labels para saltos
#include "..\Headers\t2t.h"         // conversoes de tipos
#include "..\Headers\funcoes.h"     // criacao e uso de funcoes
#include "..\Headers\oper.h"        // operacoes da ULA
#include "..\Headers\stdlib.h"      // biblioteca padrao do sapho
#include "..\Headers\diretivas.h"   // diretivas de compilacao
#include "..\Headers\saltos.h"      // gerenciamento saltos (if/else while)
#include "..\Headers\data_declar.h" // declaracao de dados
#include "..\Headers\data_use.h"    // utilizacao de dados
#include "..\Headers\data_assign.h" // atribuicao de dados
#include "..\Headers\array_index.h"
#include "..\Headers\global.h"

#include <string.h>
#include <stdlib.h>

// variaveis obrigatorias do flex/bison ---------------------------------------

extern FILE *yyin;
int   yylex  (void);
void  yyerror(char const *s);

%}

%union {int ival;} // valor associado a um token

// tokens que nao tem atribuicao ----------------------------------------------

%token PRNAME NUBITS NBMANT NBEXPO NDSTAC SDEPTH          // diretivas
%token NUIOIN NUIOOU NUGAIN USEMAC ENDMAC FFTSIZ ITRADD   // diretivas
%token INN OUT NRM PST ABS SGN SQRT REAL IMAG ATAN FASE   // std lib
%token WHILE IF THEN ELSE SWITCH CASE DEFAULT RET BREAK   // saltos
%token SHIFTL SHIFTR SSHIFTR                              // deslocamento de bits
%token GREQU LESEQ EQU DIF LAN LOR                        // operadores logicos de dois simbolos
%token PPLUS                                              // operador ++. pode ser usado pra reduzir exp e tb pra assignments

// tokens terminais -----------------------------------------------------------

%token <ival> TYPE ID STRING INUM FNUM CNUM               // vem do lexer com um valor associado

// elimina conflito if com e sem else
%nonassoc THEN
%nonassoc ELSE

// importante para lista de parametros de uma funcao
// o primeiro parametro eh o ultimo a ser parseado
%right ','

// mais embaixo -> mais prioritario (usa padrao C)
%left LOR
%left LAN
%left '|'
%left '^'
%left '&'
%left EQU DIF
%left '>' '<' GREQU LESEQ
%left SHIFTL SHIFTR SSHIFTR
%left '+' '-'
%left '*' '/' '%'
%left '!' '~' PPLUS

%type <ival> par_list
%type <ival> func_call
%type <ival> std_in std_pst std_abs std_sign std_nrm std_sqrt std_real std_imag std_atan std_fase
%type <ival> exp terminal

%%

// Programa e seus elementos --------------------------------------------------

fim           : prog
prog          : prog_elements | prog prog_elements
prog_elements : direct | declar_full | funcao

// Diretivas de compilacao ----------------------------------------------------

direct : PRNAME  ID    {exec_dire("#PRNAME",$2,6);} // nome do processador
       | NUBITS INUM   {exec_dire("#NUBITS",$2,0);} // tamanho da palavra da ULA
       | NBMANT INUM   {exec_dire("#NBMANT",$2,2);} // numero de bits da mantissa
       | NBEXPO INUM   {exec_dire("#NBEXPO",$2,3);} // numero de bits do expoente
       | NDSTAC INUM   {exec_dire("#NDSTAC",$2,0);} // tamanho da pilha de dados
       | SDEPTH INUM   {exec_dire("#SDEPTH",$2,0);} // tamanho da pilha de subrotina
       | NUIOIN INUM   {exec_dire("#NUIOIN",$2,4);} // numero de portas de entrada
       | NUIOOU INUM   {exec_dire("#NUIOOU",$2,5);} // numero de portas de saida
       | NUGAIN INUM   {exec_dire("#NUGAIN",$2,0);} // contante de divisao (norm(.))
       | FFTSIZ INUM   {exec_dire("#FFTSIZ",$2,0);} // tamanho da FFT (2^FFTSIZ)

       | USEMAC STRING INUM {use_macro($2,1,$3);}   // substitui uma parte do codico por uma macro em assembler (fora de uma funcao)
       | ENDMAC             {end_macro();}          // ponto de termino do uso da macro

// Diretivas comportamentais --------------------------------------------------

use_macro : USEMAC STRING INUM {use_macro($2,0,$3);} // usa uma macro .asm no lugar do compilador (dentro de uma funcao)
end_macro : ENDMAC             {end_macro();}        // ponto final de uso de uma macro
use_inter : ITRADD             {use_inter();}        // ponto de inicio da interrupcao (usado com o pino itr)

// Declaracao de variaveis ----------------------------------------------------

declar : TYPE id_list ';'
       | TYPE ID '[' INUM ']'              STRING ';' {declar_arr_1d($2,$4,$6   );} // fazer pra complexo
       | TYPE ID '[' INUM ']' '[' INUM ']' STRING ';' {declar_arr_2d($2,$4,$7,$9);}

id_list : IID | id_list ',' IID

IID : ID                                  {declar_var   ($1         );}
    | ID '[' INUM ']'                     {declar_arr_1d($1,$3   ,-1);}
    | ID '[' INUM ']' '[' INUM ']'        {declar_arr_2d($1,$3,$6,-1);}

// Declaracao de funcoes ------------------------------------------------------

funcao : TYPE ID '('                      {declar_fun($1,$2);} // inicio da declaracao de uma funcao
         par_list ')'                     {declar_fst($5   );} // seta o primeiro parametro na variavel correspondente
         '{' stmt_list '}'                {func_ret  ($2   );} // checa se foi tudo ok
       | TYPE ID '('   ')'                {declar_fun($1,$2);} // funcao sem parametros
         '{' stmt_list '}'                {func_ret  ($2   );}

// lista de parametros na declaracao
// ainda nao pode usar array em parametro de funcao
// retorna o id do parametro
par_list : TYPE ID                        {$$ = declar_par($1,$2);}
         | par_list ',' par_list          {        set_par($3   );} // vai pegando da pilha

// retornos de funcao e void
return_call : RET exp ';'                 {declar_ret($2,1);}
            | RET     ';'                 {  void_ret(    );}

// lista de statments em C ----------------------------------------------------

stmt_list: stmt_full | stmt_list stmt_full

// todos os statements que posso escrever dentro de uma funcao
stmt_full: '{' stmt_list '}' // bloco de statments
         |     stmt_case     // todos os tipos de stmts aceitos no case
         |   switch_case     // switch case
         |         break     // break; dentro do while
         |     use_inter     // ponto de interrupcao

// statments que podem ser usados dentro do case :
stmt_case:   declar_full     // declaracoes de variaveis
         |    assignment     // atribuicao de expressoes a uma variavel = $ @ />
         |    while_stmt     // loop while
         |  if_else_stmt     // if/else
         |       std_out     // std lib de output de dados
         |     void_call     // chamada de subrotina
         |   return_call     // retorno de funcao
         |     use_macro     // diz que vai usar uma macro passada como parametro ate achar um ENDMAC
         |     end_macro     // termina uma chamada de macro assembler

// chamadas de funcoes --------------------------------------------------------

// funcao void
void_call   : ID '('            {fun_id2  = $1 ;} // fun_id2 -> id da funcao chamada
              exp_list ')' ';'  {vcall     ($1);} // ja pode dar o call void
// funcao com retorno
func_call   : ID '('            {fun_id2  = $1 ;}
              exp_list ')'      {$$ = fcall($1);} // da call e retorna o tipo de dado final

// eh preciso colocar os parametros na pilha
// pra cada exp achado, o valor resultante eh gravado na pilha com par_exp
// o primeiro parametro fica no acumulador (pega de tras pra frente)
// par_exp xuxa parametros na pilha e checa se o parametro esta consistente
exp_list :                                                          // pode ser vazio (testar)
         | exp                             {par_exp    ($1);}       // primeiro parametro
         | exp_list ',' exp                {par_listexp($3);}       // demais parametros

// Standard library -----------------------------------------------------------

std_out  : OUT  '(' INUM ','               {     exec_out1($3   );} // saida de dados
                    exp ')'     ';'        {     exec_out2($6   );}
std_in   : INN  '(' INUM ')'               {$$ = exec_in  ($3   );} // entrada de dados
std_pst  : PST  '(' exp ')'                {$$ = exec_pst ($3   );} // funcao pset(x)   -> zera se negativo
std_abs  : ABS  '(' exp ')'                {$$ = exec_abs ($3   );} // funcao  abs(x)   -> valor absoluto de x
std_sign : SGN  '(' exp ',' exp ')'        {$$ = exec_sign($3,$5);} // funcao sign(x,y) -> pega o sinal de x e coloca em y
std_nrm  : NRM  '(' exp ')'                {$$ = exec_norm($3   );} // funcao norm(x)   -> divide x pela constante NUGAIN
std_sqrt : SQRT '(' exp ')'                {$$ = exec_sqrt($3   );} // funcao sqrt(x)   -> raiz quadrada
std_real : REAL '(' exp ')'                {$$ = exec_real($3   );} // funcao real(x)   -> pega a parte real de um comp
std_imag : IMAG '(' exp ')'                {$$ = exec_imag($3   );} // funcao imag(x)   -> pega a parte imag de um comp
std_atan : ATAN '(' exp ')'                {$$ = exec_atan($3   );} // funcao atan(x)   -> arctg
std_fase : FASE '(' exp ')'                {$$ = exec_fase($3   );} // funcao fase(x)   -> pega a fase de um comp

// if/else --------------------------------------------------------------------

if_else_stmt : if_exp stmt_full ELSE             {else_stmt(  );} // if/else completo
               stmt_full                         {if_fim   (  );}
             | if_exp stmt_full     %prec THEN   {if_stmt  (  );} // if sem else
if_exp       : IF '(' exp ')'                    {if_exp   ($3);} // inicio (JIZ)

// switch/case ----------------------------------------------------------------

switch_case : SWITCH '(' exp ')'  {exec_switch($3);}
              '{' cases '}'       { end_switch(  );}

case_list   :           stmt_case
            | case_list stmt_case
            | case_list BREAK ';' {switch_break();} // case tem seu proprio break (diferente do while e for)

case        : CASE INUM ':'       {  case_test($2,1);} case_list
            | CASE FNUM ':'       {  case_test($2,2);} case_list
default     : DEFAULT   ':'       {defaut_test(    );} case_list

cases       : case | default | case cases

// while ----------------------------------------------------------------------

while_stmt : while_exp stmt_full           {while_stmt  (  );}
while_exp  : WHILE                         {while_expp  (  );}
            '(' exp ')'                    {while_expexp($4);}
break      : BREAK ';'                     {exec_break  (  );}

// declaracoes com assignment -------------------------------------------------

declar_full : declar
            | TYPE ID '=' exp ';'          {declar_var($2); var_set($2,$4);}

// assignments ----------------------------------------------------------------

           // atribuicao padrao
assignment : ID  '=' exp ';'                       {var_set($1,$3);}
           // incremento
           | ID                          PPLUS ';' {pplus_assign($1      );}
           | ID  '[' exp ']'             PPLUS ';' {aplus_assign($1,$3   );}
           | ID  '[' exp ']' '[' exp ']' PPLUS ';' {aplu2_assign($1,$3,$6);}
           // array normal
           | ID  '[' exp ']'  '='                  {get_1d_index($1,$3);}
                     exp ';'                       {array_set ($1,$7,0);}
           // array invertido
           | ID  '[' exp ')'  '='                  {get_1d_index($1,$3);}
                     exp ';'                       {array_set ($1,$7,1);}
           // array 2D (completar)
           | ID  '[' exp ']' '[' exp ']' '='       {get_2d_index($1, $3,$6);}
                     exp ';'                       {array_set   ($1,$10, 0);}

// expressoes -----------------------------------------------------------------

exp:       terminal                           {$$ = $1;}
         // arrays
         | ID '[' exp ']'                     {$$ = array1d2exp($1,$3, 0);}
         | ID '[' exp ')'                     {$$ = array1d2exp($1,$3, 1);}
         | ID '[' exp ']' '[' exp ']'         {$$ = array2d2exp($1,$3,$6);}
         // std library que retorna valores
         | std_in                             {$$ = $1;}
         | std_pst                            {$$ = $1;}
         | std_abs                            {$$ = $1;}
         | std_sign                           {$$ = $1;}
         | std_nrm                            {$$ = $1;}
         | std_sqrt                           {$$ = $1;}
         | std_real                           {$$ = $1;}
         | std_imag                           {$$ = $1;}
         | std_atan                           {$$ = $1;}
         | std_fase                           {$$ = $1;}
         // chamada de funcao
         | func_call                          {$$ = $1;}
         // operadores nulos
         |    '(' exp ')'                     {$$ = $2;}
         |    '+' exp                         {$$ = $2;}
         // operadores unarios
         |    '-' exp                         {$$ =    oper_neg($2      );}
         |    '!' exp                         {$$ =   oper_lin ($2      );}
         |    '~' exp                         {$$ =    oper_inv($2      );}
         | ID                         PPLUS   {$$ =   pplus2exp($1      );}
         | ID '[' exp ']'             PPLUS   {$$ = pplus1d2exp($1,$3   );}
         | ID '[' exp ']' '[' exp ']' PPLUS   {$$ = pplus2d2exp($1,$3,$6);}
         // operadores de deslocamento
         | exp  SHIFTL exp                    {$$ = oper_shift($1,$3, 0);}
         | exp  SHIFTR exp                    {$$ = oper_shift($1,$3, 1);}
         | exp SSHIFTR exp                    {$$ = oper_shift($1,$3, 2);}
         // operadores bitwise
         | exp   '&'   exp                    {$$ = oper_bitw ($1,$3, 0);}
         | exp   '|'   exp                    {$$ = oper_bitw ($1,$3, 1);}
         | exp   '^'   exp                    {$$ = oper_bitw ($1,$3, 2);}
         // operadores aritmeticos
         | exp   '%'   exp                    {$$ = oper_mod ($1,$3);}
         | exp   '+'   exp                    {$$ = oper_soma($1,$3);}
         | exp   '-'   exp                    {$$ = oper_subt($1,$3);}
         | exp   '*'   exp                    {$$ = oper_mult($1,$3);}
         | exp   '/'   exp                    {$$ = oper_divi($1,$3);}
         // operadores true/false
         | exp  LAN    exp                    {$$ = oper_lanor($1,$3,0);}
         | exp  LOR    exp                    {$$ = oper_lanor($1,$3,1);}
         | exp   '<'   exp                    {$$ = oper_cmp  ($1,$3,0);}
         | exp   '>'   exp                    {$$ = oper_cmp  ($1,$3,1);}
         | exp  EQU    exp                    {$$ = oper_cmp  ($1,$3,2);}
         | exp  GREQU  exp                    {$$ = oper_greq ($1,$3  );}
         | exp  LESEQ  exp                    {$$ = oper_leeq ($1,$3  );}
         | exp  DIF    exp                    {$$ = oper_dife ($1,$3  );}

// terminais usados em reducao pra expressoes ---------------------------------

         // constantes
terminal : INUM                               {$$ = num2exp($1,1);}
         | FNUM                               {$$ = num2exp($1,2);}
         | CNUM                               {$$ = num2exp($1,5);}
         // variaveis
         | ID                                 {$$ =  id2exp($1  );}

%%

int main(int argc, char *argv[])
{
  // pega os argumentos -------------------------------------------------------

  char cmm_file[1024];
  char asm_file[1024];

  sprintf(cmm_file, "%s/Software/%s.cmm", argv[2],argv[1]);
  sprintf(asm_file, "%s/Software/%s.asm", argv[2],argv[1]);

  yyin  = fopen(cmm_file, "r"); // arquivo .cmm de entrada
  f_asm = fopen(asm_file, "w"); // arquivo .asm de saida

  sprintf(dir_soft , "%s/Software", argv[2]); // pega o diretorio software
  strcpy (dir_macro, argv[3]);                // pega o diretorio Macro
  strcpy (dir_tmp  , argv[4]);                // pega o diretorio Tmp

  // cria arquivos auxiliares -------------------------------------------------

  char path[1024];
  sprintf(path,   "%s/cmm_log.txt", dir_tmp         ); f_log = fopen(path,"w"); // log com infos pro assembler e gtkwave
  sprintf(path, "%s/pc_%s_mem.txt", dir_tmp, argv[1]); f_lin = fopen(path,"w"); // memoria no pc.v que passa de asm para cmm

  // gera uma instrucao LOD NULL no inicio (tentar tirar isso) ---------------

  add_sinst(-1,"LOD NULL\n");

  // executa o parse no arquivo .cmm ------------------------------------------

	yyparse(); // aqui a magica acontece!!

  // terminou o parse, entao libera arquivos pra pos-processamento ------------

	fclose( yyin);
	fclose(f_asm);
  fclose(f_lin);

  // checa se precisa adicionar macros no arquivo .asm ------------------------

	mac_geni(asm_file);

	// checa consistencia de todas as variaveis e funcoes -----------------------
  
	check_var(); // (variaveis.c)

  // termina o arquivo de log do cmm ------------------------------------------

  fprintf(f_log, "#\n%d\n", num_ins); // numero de instrucoes vem na linha depois de #
  fclose (f_log);

  // gera o arquivo de traducao pro codigo cmm --------------------------------

  sprintf(path, "%s/%s", dir_tmp, "trad_cmm.txt");
  
  FILE *output = fopen(path    , "w");
  FILE *input  = fopen(cmm_file, "r");

  char linha[1001], texto[1001] = "";
  fputs("-1 INTERNO\n"     , output); // codigo para inicio do arquivo
  fputs("-2 void main();\n", output); // codigo pra CAL main
  fputs("-3 FIM\n"         , output); // codigo para @fim JMP fim
  fputs("-4 User Macro\n"  , output); // codigo asm do usuario (#USEMAC)

  int cnt = 1;
  while(fgets(texto, 1001, input) != NULL)
  {
    sprintf(linha, "%d %s", cnt++, texto);
      fputs(linha, output);
     memset(texto, 0, sizeof(char) * 1001);
  }

  fclose(input );
  fclose(output);

	return 0;
}

// erro de sintaxes do bison
void yyerror (char const *s)
{
	fprintf (stderr, "Pô, presta atenção na sintaxe da linha %d!\n", line_num+1);
}
