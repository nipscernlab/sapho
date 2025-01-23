#include "..\Headers\data_assign.h"
#include "..\Headers\data_use.h"
#include "..\Headers\variaveis.h"
#include "..\Headers\diretivas.h"
#include "..\Headers\t2t.h"
#include "..\Headers\stdlib.h"
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

// faz a checagem e execucao das funcoes de atribuicao
// o hardware ja tem ULA com pros processamento para
// ja fazer o assign com operacoes unarias
// pensar em como implementar isso no compilador
// acho que vai ter que mudar completamente essa funcao
void var_set(int id, int et, int is_array, int pop, int warn, int is_cmp)
{
    // testa se ja foi declarada pra poder dar uma atribuicao
    if (v_type[id] == 0)
    {
        fprintf (stderr, "Erro na linha %d: se voc� declarar a vari�vel %s eu agrade�o.\n", line_num+1, rem_fname(v_name[id], fname));
        return;
    }

    // ------------------------------------------------------------------------
    // varias checagens entre array e nao-array -------------------------------
    // ------------------------------------------------------------------------

    if ((v_isar[id] == 0) && (is_array > 0))
    {
        fprintf (stderr, "Erro na linha %d: %s n�o � um array.\n", line_num+1, rem_fname(v_name[id], fname));
        return;
    }

    if ((v_isar[id] > 0) && (is_array == 0))
    {
        fprintf (stderr, "Erro na linha %d: %s � um array. Cad� o �ndice?\n", line_num+1, rem_fname(v_name[id], fname));
        return;
    }

    if ((v_isar[id] == 1) && (is_array == 2))
    {
        fprintf (stderr, "Erro na linha %d: t� sobrando �ndice no array %s. Era pra ser de uma dimens�o s�!\n", line_num+1, rem_fname(v_name[id], fname));
        return;
    }

    if ((v_isar[id] == 2) && (is_array == 1))
    {
        fprintf (stderr, "Erro na linha %d: t� faltando �ndice no array %s. Era pra ser de duas dimens�es!\n", line_num+1, rem_fname(v_name[id], fname));
        return;
    }

    // teste com numeros complexos --------------------------------------------
    if ((v_type[id] > 2) || (get_type(et) > 2))
    {
        // faz o assign complexo e sai da funcao
        var_set_comp(id,et,is_array); return;
    }
    // fim do teste -----------------------------------------------------------

    char cset[512] = "";

    // se for array, antes de dar o SET tem que chamar SRF/ISRF
    if (is_array > 0)
    {
        if (exec_fft_set == 1) strcpy(cset, "ISRF\n"); else strcat(cset, "SRF\n");
        is_macro();
    }

    // ------------------------------------------------------------------------
    // prepara o tipo de assign -----------------------------------------------
    // ------------------------------------------------------------------------

    int left_type =   v_type[id];
    int righ_type = get_type(et);

    if (pop == 0) strcat(cset,"SET"); else strcat(cset,"SETP");

    // ------------------------------------------------------------------------
    // executa o assign -------------------------------------------------------
    // ------------------------------------------------------------------------

    // da LOAD no right, se necessario
    load_check(et,0);

    // int pra int ------------------------------------------------------------

    if ((left_type == 1) && (righ_type == 1))
    {
        if (is_macro() == 0) fprintf(f_asm, "%s %s\n", cset, v_name[id]);
    }

    // float pra int ----------------------------------------------------------

    if ((left_type == 1) && (righ_type == 2))
    {
        if (warn) fprintf(stdout, "Aten��o na linha %d: vari�vel %s � int, mas recebe float.\n", line_num+1, rem_fname(v_name[id], fname));

        if (prtype == 0)
        {
            if (is_macro() == 0) fprintf(f_asm, "CALL float2int\n");
            f2i = 1;
        }
            if (is_macro() == 0) fprintf(f_asm, "%s %s\n", cset, v_name[id]);
    }

    // int pra float ----------------------------------------------------------

    if ((left_type == 2) && (righ_type == 1))
    {
        if (warn) fprintf(stdout, "Aten��o na linha %d: vari�vel %s � float, mas recebe int.\n", line_num+1, rem_fname(v_name[id], fname));

        if (prtype == 0)
        {
            if (is_macro() == 0) fprintf(f_asm, "CALL int2float\n");
            i2f = 1;
        }
        if (is_macro() == 0) fprintf(f_asm, "%s %s\n", cset, v_name[id]);
    }

    // float pra float --------------------------------------------------------

    if ((left_type == 2) && (righ_type == 2))
    {
        if (is_macro() == 0) fprintf(f_asm, "%s %s\n", cset, v_name[id]);
    }

    // se vem do var_set_comp, nao altera por aqui
    if ((is_array > 0) && (is_cmp == 0)) exec_fft_set = 0;

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
void var_set_comp(int id, int et, int is_array)
{
    int id_r, id_i;
    int et_r, et_i;

    int t_left  =   v_type[id];
    int t_right = get_type(et);

    int mem = et % OFST;

    // left int e right comp const --------------------------------------------

    if ((t_left == 1) && (t_right == 5))
    {
        fprintf (stdout, "Aten��o na linha %d: nessa convers�o, eu vou arredondar a parte real hein!\n", line_num+1);

        split_cmp_const(et,&et_r,&et_i);
                var_set(id, et_r,is_array,0,0,1);
    }

    // left int e right comp var ----------------------------------------------

    if ((t_left == 1) && (t_right == 3) && (mem != 0))
    {
        fprintf (stdout, "Aten��o na linha %d: nessa convers�o, eu vou arredondar a parte real hein!\n", line_num+1);

        get_cmp_ets(et,&et_r,&et_i); // pega os IDs estendidos do right na memoria
            var_set(id, et_r,is_array,0,0,1);
    }

    // left int e right comp acc ----------------------------------------------

    if ((t_left == 1) && (t_right == 3) && (mem == 0))
    {
        fprintf (stdout, "Aten��o na linha %d: nessa convers�o, eu vou arredondar a parte real hein!\n", line_num+1);

        if (is_macro() == 0) fprintf(f_asm, "SETP aux_cmp\n");
        var_set(id,2*OFST,is_array,0,0,1);
    }

    // left float e right comp const -----------------------------------------

    if ((t_left == 2) && (t_right == 5))
    {
        fprintf (stdout, "Aten��o na linha %d: vou pegar s� a parte real!\n", line_num+1);

        split_cmp_const(et,&et_r,&et_i);
                var_set(id, et_r,is_array,0,0,1);
    }

    // left float e right comp var --------------------------------------------

    if ((t_left == 2) && (t_right == 3) && (mem != 0))
    {
        fprintf (stdout, "Aten��o na linha %d: vou pegar s� a parte real!\n", line_num+1);

        get_cmp_ets(et,&et_r,&et_i); // pega os IDs estendidos do right na memoria
            var_set(id, et_r,is_array,0,0,1);
    }

    // left float e right comp acc ----------------------------------------------

    if ((t_left == 2) && (t_right == 3) && (mem == 0))
    {
        fprintf (stdout, "Aten��o na linha %d: vou pegar s� a parte real!\n", line_num+1);

        if (is_macro() == 0) fprintf(f_asm, "SETP aux_cmp\n");
        var_set(id,2*OFST,is_array,0,0,1);
    }

    // left comp e right int var ----------------------------------------------

    // falta completar o imag com zero nos 4 casos abaixo
    if ((t_left == 3) && (t_right == 1) && (mem != 0))
    {
        if (is_array) acc_ok = 1;
        v_type[id] = 2; var_set(id,et,is_array,0,0,1); v_type[id] = 3;
        if (is_array) a_cnt--;
    }

    // left comp e right int acc ----------------------------------------------

    if ((t_left == 3) && (t_right == 1) && (mem == 0))
    {
        if (is_array > 0)
        {
            if (is_macro() == 0) fprintf(f_asm, "SET auxr_cmp\n");
            if (is_macro() == 0) fprintf(f_asm, "LOAD aux_idx%d\n", a_cnt);
            if (is_macro() == 0) fprintf(f_asm, "PLD auxr_cmp\n");
        }

        v_type[id] = 2; var_set(id,et,is_array,0,0,1); v_type[id] = 3;

        if (is_array) a_cnt--;
    }

    // left comp e right float var --------------------------------------------

    if ((t_left == 3) && (t_right == 2) && (mem != 0))
    {
        if (is_array) acc_ok = 1;
        v_type[id] = 2; var_set(id,et,is_array,0,0,1); v_type[id] = 3;
        if (is_array) a_cnt--;
    }

    // left comp e right float acc --------------------------------------------

    if ((t_left == 3) && (t_right == 2) && (mem == 0))
    {
        if (is_array > 0)
        {
            if (is_macro() == 0) fprintf(f_asm, "SET auxr_cmp\n");
            if (is_macro() == 0) fprintf(f_asm, "LOAD aux_idx%d\n", a_cnt);
            if (is_macro() == 0) fprintf(f_asm, "PLD auxr_cmp\n");
        }

        v_type[id] = 2; var_set(id,et,is_array,0,0,1); v_type[id] = 3;

        if (is_array) a_cnt--;
    }

    // left comp e right comp const -------------------------------------------

    if ((t_left == 3) && (t_right == 5))
    {
        get_cmp_ids    (id,&id_r,&id_i); // pega os dois IDs do left
        split_cmp_const(et,&et_r,&et_i);

        if (is_array > 0)
        {   // tem que salvar o indice momentaneamente em aux_cmpx
            if (is_macro() == 0) fprintf(f_asm, "LOAD aux_idx%d\n", a_cnt);
            acc_ok = 1; // pra nao perder o indice
        }

        v_type[id_r] = 2; var_set(id_r,et_r,is_array,0,0,1); v_type[id_r] = 3;

        if (is_array > 0)
        {   // tem que salvar o indice momentaneamente em aux_cmpx
            if (is_macro() == 0) fprintf(f_asm, "LOAD aux_idx%d\n", a_cnt--);
            acc_ok = 1; // pra nao perder o indice
        }

        v_type[id_i] = 2; var_set(id_i,et_i,is_array,0,0,1); v_type[id_i] = 4;
    }

    // left comp e right comp var ---------------------------------------------

    if ((t_left == 3) && (t_right == 3) && (mem != 0))
    {
        get_cmp_ids(id,&id_r,&id_i); // pega os dois IDs do left
        get_cmp_ets(et,&et_r,&et_i); // pega os IDs estendidos do right na memoria

        if (is_array > 0)
        {   // tem que salvar o indice momentaneamente em aux_cmpx
            if (is_macro() == 0) fprintf(f_asm, "LOAD aux_idx%d\n", a_cnt);
            acc_ok = 1; // pra nao perder o indice
        }

        v_type[id_r] = 2; var_set(id_r,et_r,is_array,0,0,1); v_type[id_r] = 3;

        if (is_array > 0)
        {   // tem que salvar o indice momentaneamente em aux_cmpx
            if (is_macro() == 0) fprintf(f_asm, "LOAD aux_idx%d\n", a_cnt--);
            acc_ok = 1; // pra nao perder o indice
        }

        v_type[id_i] = 2; var_set(id_i,et_i,is_array,0,0,1); v_type[id_i] = 4;
    }

    // left comp e right comp acc ---------------------------------------------

    if ((t_left == 3) && (t_right == 3) && (mem == 0))
    {
        get_cmp_ids(id,&id_r,&id_i); // pega os dois IDs do left

        if (is_array > 0)
        {   // precisa tirar o img e o real da pilha pra botar o indice do array depois o img
            if (is_macro() == 0) fprintf(f_asm, "SETP auxi_cmp\n");
            if (is_macro() == 0) fprintf(f_asm, "SET auxr_cmp\n");
            if (is_macro() == 0) fprintf(f_asm, "LOAD aux_idx%d\n", a_cnt);
            if (is_macro() == 0) fprintf(f_asm, "PLD auxi_cmp\n");
        }

        int pop = (is_array < 1); // se for array, usa PSET pra nao perder o img, se nao, usa SET
        v_type[id_i] = 2; var_set(id_i,2*OFST,is_array,pop,0,1); v_type[id_i] = 4;

        if (is_array > 0)
            // agora coloca o indice e a parte real pra fazer o SET
        {   // esse decremento eh para o indice do array do lado esquerdo (ex: d[3] = x;)
            if (is_macro() == 0) fprintf(f_asm, "LOAD aux_idx%d\n", a_cnt--);
            if (is_macro() == 0) fprintf(f_asm, "PLD auxr_cmp\n");
        }

        v_type[id_r] = 2; var_set(id_r,2*OFST,is_array,0,0,1); v_type[id_r] = 3;
    }

    if (is_array > 0) exec_fft_set = 0;
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
