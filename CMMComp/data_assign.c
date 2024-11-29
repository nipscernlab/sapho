#include "data_assign.h"
#include "data_use.h"
#include "variaveis.h"
#include "diretivas.h"
#include "t2t.h"
#include "stdlib.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

// pega o tipo de variavel
int get_type(int et)
{
    int t;

         if (et <   OFST) t = 0; // indefinido
    else if (et < 2*OFST) t = 1; // int
    else if (et < 3*OFST) t = 2; // float
    // testes com variaveis complexas -----------------------------------------
    else if (et < 4*OFST) t = 3; // comp real
    else if (et < 5*OFST) t = 4; // comp img
    else if (et < 6*OFST) t = 5; // comp const (ex: 3+7.5i)
    // fim do teste -----------------------------------------------------------
    else                  t =-1;

    return t;
}

// pega o id da parte imag de uma var complexa
// a parte real esta no param id
int get_img_id(int id)
{
       char name[1024];
    sprintf(name, "%s_i", v_name[id]);

       if (find_var(name) == -1) add_var(name);
    return find_var(name);
}

// assign com PSET
void exec_setpset(int left_type, char *cset, int pop, char *sufixo)
{
    if ((prtype  == 0) && (left_type == 2))
    {
        // implementar
        fprintf (stderr, "Erro na linha %d: ainda não implementei isso pra float no processador em ponto fixo.\n", line_num+1);
    }

    strcat(cset, "PSETS");                     // SET se for positivo, zero caso contrario

    if (pop) strcat(sufixo, "SETP aux_pop\n"); // ainda nao tem PSETSP
}

// assign com NORM
void exec_setnorm(int left_type, int righ_type, int warn, char *cset, int pop, char *sufixo)
{
    // soh serve pra proc ponto fixo usando int
    if ((  prtype  == 1) || (left_type != 1) || (righ_type != 1))
    {
       if (warn) fprintf (stderr, "Erro na linha %d: essa atribuição não faz sentido aqui! Você entendeu o propósito dela?\n", line_num+1);
    }

    strcat(cset, "NORMS");                     // seta dividindo pela constante NUGAIN

    if (pop) strcat(sufixo, "SETP aux_pop\n"); // ainda nao tem NORMSP
}

// assign com ABS
void exec_setabs(int left_type, int righ_type, char *cset, int pop, char *sufixo, int et)
{
    // fazer uma funcao separada e colocar caso a caso tb. ta muito embolado
    // no proc em ponto fixo, usando float, tem que fazer o abs por software
    if ((prtype  == 0) && (left_type == 2) && (righ_type < 3))
    {
       char aux[512];

       // faz um AND pra zerar o bit de sinal
       sprintf(aux, "AND %d          // zera o bit de sinal (abs pra float em software)\n",(int)pow(2,nbmant+nbexpo)-1);
       strcat (aux,cset);
       strcpy (cset,aux);

       if (pop) strcat(cset, "SETP"); else strcat(cset, "SET");
    }
    // testes com numeros complexos --------------------------------------
    else if (righ_type > 2)
    {
        // faz o abs complex, mudando o tipo de dado pra dois
               et = abs_comp(et);
        righ_type = get_type(et);

        if (pop) strcat(cset, "SETP"); else strcat(cset, "SET");
    }
    // fim do teste ------------------------------------------------------
    else
    {
        strcat(cset, "ABSS" );                     // seta o valor absoluto

        if (pop) strcat(sufixo, "SETP aux_pop\n"); // ainda nao tem ABSSP
    }
}

// assign padrao
void exec_setset(int pop, char *cset)
{
    if  (pop == 0)
        strcat(cset,  "SET" ); // SET padrao
    else
        strcat(cset,  "SETP"); // SET com POP
}

// assign com NEG
void exec_setneg(int left_type, char *cset, int pop, int righ_type, int et, char *sufixo)
{
    // fazer uma funcao separada e colocar caso a caso tb. ta muito embolado
    // no proc em ponto fixo, usando float, faz o neg por software
    if ((prtype == 0) && (left_type == 2))
    {
       char aux[512];

       // quatro instrucoes pra inverter o bit de sinal? tentar melhorar
       sprintf(aux, "SET aux_neg\nINV\nAND %d\nADD aux_neg\n",(int)pow(2,nbmant+nbexpo));
       strcat (aux,cset);
       strcpy (cset,aux);

       if (pop) strcat(cset, "SETP"); else strcat(cset, "SET");
    }
    // testes com numeros complexos --------------------------------------
    else if ((prtype == 0) && (left_type > 2))
    {
        if (righ_type > 2)
        {
           // faz o neg complexo
                  et = neg_comp(et);
           righ_type = get_type(et);
        }

        if (pop) strcat(cset, "SETP"); else strcat(cset, "SET");
    }
    // fim do teste ------------------------------------------------------
    else
    {
        strcat(cset, "NEGS" ); // seta o valor absoluto

        if (pop) strcat(sufixo, "SETP aux_pop\n");  // ainda nao tem NEGSP
    }
}

// faz a checagem e execucao das funcoes de atribuicao
void var_set(int id, int et, int is_array, int set_type, int pop, int warn)
{
    // testa se ja foi declarada pra poder dar uma atribuicao
    if (v_type[id] == 0)
    {
        fprintf (stderr, "Erro na linha %d: se você declarar a variável %s eu agradeço.\n", line_num+1, rem_fname(v_name[id], fname));
        return;
    }

    // ------------------------------------------------------------------------
    // varias checagens entre array e nao-array -------------------------------
    // ------------------------------------------------------------------------

    if ((v_isar[id] == 0) && (is_array > 0))
    {
        fprintf (stderr, "Erro na linha %d: %s não é um array.\n", line_num+1, rem_fname(v_name[id], fname));
        return;
    }

    if ((v_isar[id] > 0) && (is_array == 0))
    {
        fprintf (stderr, "Erro na linha %d: %s é um array. Cadê o índice?\n", line_num+1, rem_fname(v_name[id], fname));
        return;
    }

    if ((v_isar[id] == 1) && (is_array == 2))
    {
        fprintf (stderr, "Erro na linha %d: tá sobrando índice no array %s. Era pra ser de uma dimensão só!\n", line_num+1, rem_fname(v_name[id], fname));
        return;
    }

    if ((v_isar[id] == 2) && (is_array == 1))
    {
        fprintf (stderr, "Erro na linha %d: tá faltando índice no array %s. Era pra ser de duas dimensões!\n", line_num+1, rem_fname(v_name[id], fname));
        return;
    }

    char   cset[512] = "";
    char sufixo[128] = "";

    // se for array, antes de dar o SET tem que chamar SRF/ISRF
    if (is_array > 0)
    {
        if (exec_fft_set == 1) strcpy(cset, "ISRF\n"); else strcat(cset, "SRF\n");
            exec_fft_set =  0;
    }

    // ------------------------------------------------------------------------
    // prepara o tipo de assign -----------------------------------------------
    // ------------------------------------------------------------------------

    int left_type =   v_type[id];
    int righ_type = get_type(et);

    switch (set_type)
    {
          case 1: exec_setpset(left_type, cset, pop, sufixo);                  break;
          case 2: exec_setnorm(left_type, righ_type, warn, cset, pop, sufixo); break;
          case 3: exec_setabs (left_type, righ_type, cset, pop, sufixo, et);   break;
          case 4: exec_setneg (left_type, cset, pop, righ_type, et, sufixo);   break;
        default : exec_setset (pop, cset);
    }

    // ------------------------------------------------------------------------
    // executa o assign -------------------------------------------------------
    // ------------------------------------------------------------------------

    // teste com numeros complexos --------------------------------------------
    if ((v_type[id] > 2) || (get_type(et) > 2))
    {
        // faz o assign complexo e sai da funcao
        var_set_comp(id, et, is_array, set_type);
        return;
    }
    // fim do teste -----------------------------------------------------------

    // da LOAD no right, se necessario
    load_check(et,0);

    // int pra int ------------------------------------------------------------

    if ((left_type == 1) && (righ_type == 1))
    {
        if (using_macro == 0) fprintf(f_asm, "%s %s\n%s", cset, v_name[id],sufixo);
    }

    // float pra int ----------------------------------------------------------

    if ((left_type == 1) && (righ_type == 2))
    {
        if (warn) fprintf(stdout, "Atenção na linha %d: variável %s é int, mas recebe float.\n", line_num+1, rem_fname(v_name[id], fname));

        if (prtype == 0)
        {
            if (using_macro == 0) fprintf(f_asm, "CALL float2int\n");
            f2i = 1;
        }
            if (using_macro == 0) fprintf(f_asm, "%s %s\n%s", cset, v_name[id],sufixo);
    }

    // int pra float ----------------------------------------------------------

    if ((left_type == 2) && (righ_type == 1))
    {
        if (warn) fprintf(stdout, "Atenção na linha %d: variável %s é float, mas recebe int.\n", line_num+1, rem_fname(v_name[id], fname));

        if (prtype == 0)
        {
            if (using_macro == 0) fprintf(f_asm, "CALL int2float\n");
            i2f = 1;
        }
        if (using_macro == 0) fprintf(f_asm, "%s %s\n%s", cset, v_name[id],sufixo);
    }

    // float pra float --------------------------------------------------------

    if ((left_type == 2) && (righ_type == 2))
    {
        if (using_macro == 0) fprintf(f_asm, "%s %s\n%s", cset, v_name[id],sufixo);
    }

    acc_ok     = 0;  // liberou o acc
    v_asgn[id] = 1;  // variavel recebeu um valor
}

// separa a parte real e imaginaria de uma constante complexa
// gerando duas entradas na tabela pra ponto flutuante
void split_cmp_const(int et, int *et_r, int *et_i)
{
    char  txt[64];
    float real, img;

    sscanf(v_name[et % OFST],"%f %f",&real,&img);

    sprintf(txt,"%f",real);
    *et_r = 2*OFST + exec_num(txt);

    sprintf(txt,"%f",img);
    *et_i = 2*OFST + exec_num(txt);
}

// pega id para a parte real e imaginaria
// de um num complexo na memoria
void get_cmp_ids(int id, int *id_r, int *id_i)
{
    *id_r = id;
    *id_i = get_img_id(id);
}

// gera ID estendido float pra parte real e imaginaria
// de um num complexo na memoria
void get_cmp_ets(int et, int *et_r, int *et_i)
{
    *et_r = 2*OFST + (et % OFST);
    *et_i = 2*OFST + get_img_id(et % OFST);
}

// faz o assign envolvendo numeros complexos
void var_set_comp(int id, int et, int is_array, int set_type)
{
    int id_r, id_i;
    int et_r, et_i;

    int t_left  =   v_type[id];
    int t_right = get_type(et);

    int mem = et % OFST;

    // left int e right comp const --------------------------------------------

    if ((t_left == 1) && (t_right == 5))
    {
        fprintf (stdout, "Atenção na linha %d: nessa conversão, eu vou arredondar a parte real hein!\n", line_num+1);

        split_cmp_const(et,&et_r,&et_i);
                var_set(id, et_r,is_array,set_type,0,0);
    }

    // left int e right comp var ----------------------------------------------

    if ((t_left == 1) && (t_right == 3) && (mem != 0))
    {
        fprintf (stdout, "Atenção na linha %d: nessa conversão, eu vou arredondar a parte real hein!\n", line_num+1);

        get_cmp_ets(et,&et_r,&et_i); // pega os IDs estendidos do right na memoria
            var_set(id, et_r,is_array,set_type,0,0);
    }

    // left int e right comp acc ----------------------------------------------

    if ((t_left == 1) && (t_right == 3) && (mem == 0))
    {
        fprintf (stdout, "Atenção na linha %d: nessa conversão, eu vou arredondar a parte real hein!\n", line_num+1);

        if (using_macro == 0) fprintf(f_asm, "SETP aux_cmp\n");
        var_set(id,2*OFST,is_array,set_type,0,0);
    }

    // left float e right comp const -----------------------------------------

    if ((t_left == 2) && (t_right == 5))
    {
        fprintf (stdout, "Atenção na linha %d: vou pegar só a parte real!\n", line_num+1);

        split_cmp_const(et,&et_r,&et_i);
                var_set(id, et_r,is_array,set_type,0,0);
    }

    // left float e right comp var --------------------------------------------

    if ((t_left == 2) && (t_right == 3) && (mem != 0))
    {
        fprintf (stdout, "Atenção na linha %d: vou pegar só a parte real!\n", line_num+1);

        get_cmp_ets(et,&et_r,&et_i); // pega os IDs estendidos do right na memoria
            var_set(id, et_r,is_array,set_type,0,0);
    }

    // left float e right comp acc ----------------------------------------------

    if ((t_left == 2) && (t_right == 3) && (mem == 0))
    {
        fprintf (stdout, "Atenção na linha %d: vou pegar só a parte real!\n", line_num+1);

        if (using_macro == 0) fprintf(f_asm, "SETP aux_cmp\n");
        var_set(id,2*OFST,is_array,set_type,0,0);
    }

    // left comp e right int var ----------------------------------------------

    if ((t_left == 3) && (t_right == 1) && (mem != 0))
    {
        if (is_array) acc_ok = 1;
        v_type[id] = 2; var_set(id,et,is_array,set_type,0,0); v_type[id] = 3;
        if (is_array) a_cnt--;
    }

    // left comp e right int acc ----------------------------------------------

    if ((t_left == 3) && (t_right == 1) && (mem == 0))
    {
        if (is_array > 0)
        {
            if (using_macro == 0) fprintf(f_asm, "SET auxr_cmp\nLOAD aux_idx%d\nPLD auxr_cmp\n", a_cnt);
        }

        v_type[id] = 2; var_set(id,et,is_array,set_type,0,0); v_type[id] = 3;

        if (is_array) a_cnt--;
    }

    // left comp e right float var --------------------------------------------

    if ((t_left == 3) && (t_right == 2) && (mem != 0))
    {
        if (is_array) acc_ok = 1;
        v_type[id] = 2; var_set(id,et,is_array,set_type,0,0); v_type[id] = 3;
        if (is_array) a_cnt--;
    }

    // left comp e right float acc --------------------------------------------

    if ((t_left == 3) && (t_right == 2) && (mem == 0))
    {
        if (is_array > 0)
        {
            if (using_macro == 0) fprintf(f_asm, "SET auxr_cmp\nLOAD aux_idx%d\nPLD auxr_cmp\n", a_cnt);
        }

        v_type[id] = 2; var_set(id,et,is_array,set_type,0,0); v_type[id] = 3;

        if (is_array) a_cnt--;
    }

    // left comp e right comp const -------------------------------------------

    if ((t_left == 3) && (t_right == 5))
    {
        get_cmp_ids    (id,&id_r,&id_i); // pega os dois IDs do left
        split_cmp_const(et,&et_r,&et_i);

        if (is_array > 0)
        {   // tem que salvar o indice momentaneamente em aux_cmpx
            if (using_macro == 0) fprintf(f_asm, "LOAD aux_idx%d\n", a_cnt);
            acc_ok = 1; // pra nao perder o indice
        }

        v_type[id_r] = 2; var_set(id_r,et_r,is_array,set_type,0,0); v_type[id_r] = 3;

        if (is_array > 0)
        {   // tem que salvar o indice momentaneamente em aux_cmpx
            if (using_macro == 0) fprintf(f_asm, "LOAD aux_idx%d\n", a_cnt--);
            acc_ok = 1; // pra nao perder o indice
        }

        v_type[id_i] = 2; var_set(id_i,et_i,is_array,set_type,0,0); v_type[id_i] = 4;
    }

    // left comp e right comp var ---------------------------------------------

    if ((t_left == 3) && (t_right == 3) && (mem != 0))
    {
        get_cmp_ids(id,&id_r,&id_i); // pega os dois IDs do left
        get_cmp_ets(et,&et_r,&et_i); // pega os IDs estendidos do right na memoria

        if (is_array > 0)
        {   // tem que salvar o indice momentaneamente em aux_cmpx
            if (using_macro == 0) fprintf(f_asm, "LOAD aux_idx%d\n", a_cnt);
            acc_ok = 1; // pra nao perder o indice
        }

        v_type[id_r] = 2; var_set(id_r,et_r,is_array,set_type,0,0); v_type[id_r] = 3;

        if (is_array > 0)
        {   // tem que salvar o indice momentaneamente em aux_cmpx
            if (using_macro == 0) fprintf(f_asm, "LOAD aux_idx%d\n", a_cnt--);
            acc_ok = 1; // pra nao perder o indice
        }

        v_type[id_i] = 2; var_set(id_i,et_i,is_array,set_type,0,0); v_type[id_i] = 4;
    }

    // left comp e right comp acc ---------------------------------------------

    if ((t_left == 3) && (t_right == 3) && (mem == 0))
    {
        get_cmp_ids(id,&id_r,&id_i); // pega os dois IDs do left

        if (is_array > 0)
        {   // precisa tirar o img e o real da pilha pra botar o indice do array depois o img
            if (using_macro == 0) fprintf(f_asm, "SETP auxi_cmp\nSET auxr_cmp\nLOAD aux_idx%d\nPLD auxi_cmp\n", a_cnt);
        }

        int pop = (is_array < 1); // se for array, usa PSET pra nao perder o img, se nao, usa SET
        v_type[id_i] = 2; var_set(id_i,2*OFST,is_array,set_type,pop,0); v_type[id_i] = 4;

        if (is_array > 0)
            // agora coloca o indice e a parte real pra fazer o SET
        {   // esse decremento eh para o indice do array do lado esquerdo (ex: d[3] = x;)
            if (using_macro == 0) fprintf(f_asm, "LOAD aux_idx%d\nPLD auxr_cmp\n", a_cnt--);
        }

        v_type[id_r] = 2; var_set(id_r,2*OFST,is_array,set_type,0,0); v_type[id_r] = 3;
    }
}

// operador ++ tb eh um tipo de SET
void pplus_assign(int id)
{
    exp_pplus(id);
    acc_ok = 0; // liberou o acc;
}

// operador ++ pra array 1D sozinho
void aplus_assign(int id, int et)
{
    array_pplus(id,et);
    acc_ok = 0; // liberou o acc;
}

// operador ++ pra array 2D sozinho
void aplu2_assign(int id, int et1, int et2)
{
    array_2plus(id,et1,et2);
    acc_ok = 0; // liberou o acc;
}
