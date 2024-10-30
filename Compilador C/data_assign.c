#include "data_assign.h"
#include "data_use.h"
#include "variaveis.h"
#include "diretivas.h"
#include "t2t.h"
#include <string.h>
#include <stdio.h>

// funcao auxiliar
// pega o tipo de variavel
// 0 -> void (ainda nao declarada)
// 1 -> ponto fixo
// 2 -> ponto flutuante
int get_type(int et)
{
    int t;

         if (et <   OFST) t = 0;
    else if (et < 2*OFST) t = 1;
    else if (et < 3*OFST) t = 2;
    else                  t =-1;

    return t;
}

// faz a checagem e execucao das funcoes SET na memoria
// tem que testar caso a caso e sao muitos
void var_set(int id, int et, int is_array, int set_type)
{
    // da LOAD no right, se necessario
    load_check(et,0);

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

    // se for array, antes de dar o SET tem que chamar SRF
    if (is_array)
    {
        if (exec_fft_set == 1) strcpy(cset, "ISRF\n"); else strcpy(cset, "SRF\n");
            exec_fft_set =  0;
    }
    else strcpy(cset, "");

    // ------------------------------------------------------------------------
    // prepara o tipo de assign -----------------------------------------------
    // ------------------------------------------------------------------------

    int left_type =   v_type[id];
    int righ_type = get_type(et);

    // testei todas as combinacoes
    // assim fica redundante, mas mais imune a erro
         if  (set_type  == 0) strcat(cset,  "SET" );
    else if  (set_type  == 1)
    {
         if (prtype == 0)
         {
            if (left_type == 2)
            {
                fprintf (stderr, "Erro na linha %d: ainda não implementei isso pra float no processador em ponto fixo.\n", line_num+1);
            }
         }

         strcat(cset, "PSETS");
    }
    else if  (set_type  == 2)
    {
         if ((  prtype  == 1) || (left_type == 2))
         {
            fprintf (stderr, "Erro na linha %d: essa atribuição não faz sentido aqui! Você entendeu o propósito dela?\n", line_num+1);
         }

         strcat(cset, "NORMS");
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

         strcat(cset, "ABSS" );
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

         strcat(cset, "NEGS" );
    }

    // ------------------------------------------------------------------------
    // executa o assign -------------------------------------------------------
    // ------------------------------------------------------------------------

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

// operador ++ tb eh um tipo de SET
void pplus_assign(int id)
{
    exp_pplus(id);
    acc_ok = 0; // liberou o acc;
}
