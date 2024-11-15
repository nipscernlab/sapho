#include "data_assign.h"
#include "data_use.h"
#include "variaveis.h"
#include "diretivas.h"
#include "t2t.h"
#include <string.h>
#include <stdio.h>

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

int get_img_id(int id)
{
    char name[64];
    sprintf(name, "%s_i", v_name[id]);

    if (find_var(name) == -1) add_var(name);
    return find_var(name);
}

// faz a checagem e execucao das funcoes SET na memoria
void var_set(int id, int et, int is_array, int set_type, int pop)
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

    char cset[10];

    // se for array, antes de dar o SET tem que chamar SRF/ISRF
    if (is_array > 0)
    {
        if (exec_fft_set == 1) strcpy(cset, "ISRF\n"); else strcpy(cset, "SRF\n");
            exec_fft_set =  0;
    }
    else strcpy(cset,"");

    // ------------------------------------------------------------------------
    // prepara o tipo de assign -----------------------------------------------
    // ------------------------------------------------------------------------

    int left_type =   v_type[id];
    int righ_type = get_type(et);

    // pega o tipo de atribuicao ----------------------------------------------

    if  (set_type  == 0)
    {
        if (pop == 0)
            strcat(cset,  "SET" ); // SET padrao
        else
            strcat(cset,  "SETP"); // SET com POP
    }
    else if  (set_type  == 1)
    {
         if (prtype == 0)
         {
            if (left_type == 2)
            {
                fprintf (stderr, "Erro na linha %d: ainda não implementei isso pra float no processador em ponto fixo.\n", line_num+1);
            }
         }

         strcat(cset, "PSETS"); // SET se for positivo, zero caso contrario
    }
    else if  (set_type  == 2)
    {
         if ((  prtype  == 1) || (left_type == 2))
         {
            fprintf (stderr, "Erro na linha %d: essa atribuição não faz sentido aqui! Você entendeu o propósito dela?\n", line_num+1);
         }

         strcat(cset, "NORMS"); // seta dividindo pela constante NUGAIN
    }
    else if  (set_type  == 3)
    {
         // igual ao PSETS
         if (prtype == 0)
         {
            if (left_type == 2)
            {
                fprintf (stderr, "Erro na linha %d: ainda não implementei isso pra float no processador em ponto fixo.\n", line_num+1);
            }
         }

         strcat(cset, "ABSS" ); // seta o valor absoluto
    }
    else if  (set_type  == 4)
    {
         // igual ao PSETS
         if (prtype == 0)
         {
            if (left_type == 2)
            {
                fprintf (stderr, "Erro na linha %d: ainda não implementei isso pra float no processador em ponto fixo.\n", line_num+1);
            }
         }

         strcat(cset, "NEGS" ); // seta com o negativo da variavel
    }

    // ------------------------------------------------------------------------
    // executa o assign -------------------------------------------------------
    // ------------------------------------------------------------------------

    // teste com numeros complexos --------------------------------------------
    if ((v_type[id] > 2) || (get_type(et) > 2))
    {
        var_set_comp(id, et, is_array, set_type);
        return;
    }
    // fim do teste -----------------------------------------------------------

    // da LOAD no right, se necessario
    load_check(et,0);

    // sao 4 possibilidades
    // cada uma tem sua particuaridade

    if ((left_type == 1) && (righ_type == 1)) // int pra int ------------------
    {
        if (using_macro == 0) fprintf(f_asm, "%s %s\n", cset, v_name[id]);
    }

    if ((left_type == 1) && (righ_type == 2)) // float pra int ----------------
    {
        fprintf(stdout, "Atenção na linha %d: variável %s é int, mas recebe float.\n", line_num+1, rem_fname(v_name[id], fname));

        if (prtype == 0)
        {
            if (using_macro == 0) fprintf(f_asm, "CALL float2int\n");
            f2i = 1;
        }
        if (using_macro == 0) fprintf(f_asm, "%s %s\n", cset, v_name[id]);
    }

    if ((left_type == 2) && (righ_type == 1)) // int pra float ----------------
    {
        fprintf(stdout, "Atenção na linha %d: variável %s é float, mas recebe int.\n", line_num+1, rem_fname(v_name[id], fname));

        if (prtype == 0)
        {
            if (using_macro == 0) fprintf(f_asm, "CALL int2float\n");
            i2f = 1;
        }
        if (using_macro == 0) fprintf(f_asm, "%s %s\n", cset, v_name[id]);
    }

    if ((left_type == 2) && (righ_type == 2)) // float pra float --------------
    {
        if (using_macro == 0) fprintf(f_asm, "%s %s\n", cset, v_name[id]);
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
// sao 21 possibilidades!!! (completar)
void var_set_comp(int id, int et, int is_array, int set_type)
{
    int id_r, id_i;
    int et_r, et_i;

    int t_left  =   v_type[id];
    int t_right = get_type(et);

    int mem = et % OFST;

    get_cmp_ids(id,&id_r,&id_i); // pega os dois IDs do left

    // left comp e right comp const -------------------------------------------

    if ((t_left == 3) && (t_right == 5))
    {
        split_cmp_const(et,&et_r,&et_i);
        // aqui nao tem array
        v_type[id_r] = 2; var_set(id_r,et_r,is_array,set_type,0); v_type[id_r] = 3;
        v_type[id_i] = 2; var_set(id_i,et_i,is_array,set_type,0); v_type[id_i] = 4;
    }

    // left comp e right comp var ---------------------------------------------

    if ((t_left == 3) && (t_right == 3) && (mem != 0))
    {
        get_cmp_ets(et,&et_r,&et_i); // pega os IDs estendidos do right na memoria

        //acc_ok = 1; // pq fiz isso?
        v_type[id_r] = 2; var_set(id_r,et_r,is_array,set_type,0); v_type[id_r] = 3;

        if (is_array > 0)
        {   // tem que salvar o indice momentaneamente em aux_cmpx
            if (using_macro == 0) fprintf(f_asm, "LOAD aux_cmp%d\n", a_cnt);
            acc_ok = 1; // pra nao perder o indice
        }

        v_type[id_i] = 2; var_set(id_i,et_i,is_array,set_type,0); v_type[id_i] = 4;
    }

    // left comp e right comp acc ---------------------------------------------

    if ((t_left == 3) && (t_right == 3) && (mem == 0))
    {
        if (is_array > 0)
        {   // precisa tirar o img e o real da pilha pra botar o indice do array depois o img
            if (using_macro == 0) fprintf(f_asm, "SETP auxi_cmp\nSET auxr_cmp\nLOAD aux_cmp%d\nPLD auxi_cmp\n", a_cnt);
        }

        int pop = (is_array < 1); // se for array, usa PSET pra nao perder o img, se nao, usa SET
        v_type[id_i] = 2; var_set(id_i,2*OFST,is_array,set_type,pop); v_type[id_i] = 4;

        if (is_array > 0)
            // agora coloca o indice e a parte real pra fazer o SET
        {   // esse decremento eh para o indice do array do lado esquerdo (ex: d[3] = x;)
            if (using_macro == 0) fprintf(f_asm, "LOAD aux_cmp%d\nPLD auxr_cmp\n", a_cnt--);
        }

        v_type[id_r] = 2; var_set(id_r,2*OFST,is_array,set_type,0); v_type[id_r] = 3;
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
