#include "..\Headers\array_index.h"
#include "..\Headers\data_use.h"
#include "..\Headers\variaveis.h"
#include "..\Headers\diretivas.h"
#include "..\Headers\t2t.h"
#include "..\Headers\data_assign.h"
#include "..\Headers\global.h"
#include "..\Headers\funcoes.h"

#include <stdio.h>
#include <string.h>

// ----------------------------------------------------------------------------
// Array no assign. Ex: x[i] = y; ---------------------------------------------
// ----------------------------------------------------------------------------

// coloca o indice do array no acumulador (array 1D)
void get_1d_index(int id, int et)
{
    // ------------------------------------------------------------------------
    // teste de argumentos ----------------------------------------------------
    // ------------------------------------------------------------------------

    // tem que ver se eh array mesmo
    if (v_isar[id] == 0)
        fprintf (stderr, "Erro na linha %d: %s não é array não, abensoado!\n", line_num+1, rem_fname(v_name[id], fname));

    // tem que ver se eh array 1D mesmo
    if (v_isar[id] == 2)
        fprintf (stderr, "Erro na linha %d: array %s tem duas dimensões!\n"  , line_num+1, rem_fname(v_name[id], fname));

    // ------------------------------------------------------------------------
    // executa o get index ----------------------------------------------------
    // ------------------------------------------------------------------------

    int etr, eti;

    // se int na memoria --------------------------------------------------

    if ((get_type(et) == 1) && (et % OFST != 0))
    {
        add_instr("LOD %s\n", v_name[et % OFST]);
    }

    // se int no acc ------------------------------------------------------

    if ((get_type(et) == 1) && (et % OFST == 0))
    {
        // nao faz nada
    }

    // se float var na memoria --------------------------------------------

    if ((get_type(et) == 2) && (v_isco[et%OFST] == 0) && (et % OFST != 0))
    {
        fprintf (stdout, "Atenção na linha %d: tá vendo que o índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

        add_instr("FIM %s\n", v_name[et % OFST]);
    }

    // se float const na memoria ------------------------------------------

    if ((get_type(et) == 2) && (v_isco[et%OFST] == 1) && (et % OFST != 0))
    {
        fprintf (stdout, "Atenção na linha %d: tá vendo que o índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

        add_instr("FIM %u // %s\n", f2mf(v_name[et % OFST]), v_name[et % OFST]);
    }

    // se float no acc ----------------------------------------------------

    if ((get_type(et) == 2) && (et % OFST == 0))
    {
        fprintf (stdout, "Atenção na linha %d: tá vendo que o índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

        add_instr("FIA\n");
    }

    // se comp const na memoria -------------------------------------------

    if (get_type(et) == 5)
    {
        fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou arredondar a parte real.\n", line_num+1);

        get_cmp_cst(et, &etr, &eti);

        add_instr("FIM %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
    }

    // se comp no acc -----------------------------------------------------

    if ((get_type(et) == 3) && (et % OFST == 0))
    {
        fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou arredondar a parte real.\n", line_num+1);

        add_instr("POP\n");
        add_instr("FIA\n");
    }

    // se comp na memoria -------------------------------------------------

    if ((get_type(et) == 3) && (et % OFST != 0))
    {
        fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou arredondar a parte real.\n", line_num+1);

        add_instr("FIM %s\n", v_name[et % OFST]);
    }

    acc_ok = 1; // acc carregado
}

// coloca o indice do array no acumulador (array 2D)
void get_2d_index(int id, int et1, int et2)
{
    // ------------------------------------------------------------------------
    // teste de argumentos ----------------------------------------------------
    // ------------------------------------------------------------------------

    // tem que ver se eh array mesmo
    if (v_isar[id] == 0)
        fprintf (stderr, "Erro na linha %d: %s não é array não, abensoado!\n", line_num+1, rem_fname(v_name[id], fname));

    // tem que ver se nao eh array 1D
    if (v_isar[id] == 1)
        fprintf (stderr, "Erro na linha %d: array %s tem uma dimensão só!\n", line_num+1, rem_fname(v_name[id], fname));

    // ------------------------------------------------------------------------
    // executa o get index ----------------------------------------------------
    // ------------------------------------------------------------------------

    int etr, eti;

    // int no acc e int no acc
    if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 1) && (et2 % OFST == 0))
    {
        add_instr("SETP aux_index_y\n");
        add_instr("MLT  %s_arr_size\n", v_name[id]);
        add_instr("ADD  aux_index_y\n");
    }

    // int no acc e int na memoria
    if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 1) && (et2 % OFST != 0))
    {
        add_instr("MLT %s_arr_size\n", v_name[id]);
        add_instr("ADD %s\n",  v_name[et2 % OFST]);
    }

    // int no acc e float no acc
    if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 2) && (et2 % OFST == 0))
    {
        fprintf (stdout, "Atenção na linha %d: tá vendo que o segundo índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

        add_instr("FIA\n");
        add_instr("SETP aux_index_y\n");
        add_instr("MLT %s_arr_size\n", v_name[id]);
        add_instr("ADD aux_index_y\n");
    }

    // int no acc e float const na memoria
    if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 2) && (v_isco[et2%OFST] == 1) && (et2 % OFST != 0))
    {
        fprintf (stdout, "Atenção na linha %d: tá vendo que o segundo índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

        add_instr("MLT %s_arr_size\n", v_name[id]);
        add_instr("PFIM %d // %s\n", f2mf(v_name[et2 % OFST]), v_name[et2 % OFST]);
        add_instr("SADD\n");
    }

    // int no acc e float var na memoria
    if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 2) && (v_isco[et2%OFST] == 0) && (et2 % OFST != 0))
    {
        fprintf (stdout, "Atenção na linha %d: tá vendo que o segundo índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

        add_instr("MLT %s_arr_size\n", v_name[id]);
        add_instr("PFIM %s\n", v_name[et2 % OFST]);
        add_instr("SADD\n");
    }

    // int no acc e comp const na memoria
    if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 5) && (et2 % OFST != 0))
    {
        fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

        get_cmp_cst(et2, &etr, &eti);

        add_instr("MLT %s_arr_size\n", v_name[id]);
        add_instr("PFIM %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
        add_instr("SADD\n");
    }

    // int no acc e comp no acc
    if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 3) && (et2 % OFST == 0))
    {
        fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

        add_instr("SETP aux_index_y\n");
        add_instr("FIA\n");
        add_instr("SETP aux_index_y\n");
        add_instr("MLT %s_arr_size\n", v_name[id]);
        add_instr("ADD aux_index_y\n");
    }

    // int no acc e comp na memoria
    if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 3) && (et2 % OFST != 0))
    {
        fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

        add_instr("MLT %s_arr_size\n", v_name[id]);
        add_instr("PFIM %s\n", v_name[et2 % OFST]);
        add_instr("SADD\n");
    }

    // int na memoria e int no acc
    if ((get_type(et1) == 1) && (et1 % OFST != 0) && (get_type(et2) == 1) && (et2 % OFST == 0))
    {
        add_instr("PLD %s\n",  v_name[et1 % OFST]);
        add_instr("MLT %s_arr_size\n", v_name[id]);
        add_instr("SADD\n");
    }

    // int na memoria e int na memoria
    if ((get_type(et1) == 1) && (et1 % OFST != 0) && (get_type(et2) == 1) && (et2 % OFST != 0))
    {
        add_instr("LOD %s\n",   v_name[et1 % OFST]);
        add_instr("MLT  %s_arr_size\n", v_name[id]);
        add_instr("ADD  %s\n",  v_name[et2 % OFST]);
    }

    // int na memoria e float no acc
    if ((get_type(et1) == 1) && (et1 % OFST != 0) && (get_type(et2) == 2) && (et2 % OFST == 0))
    {
        fprintf (stdout, "Atenção na linha %d: tá vendo que o segundo índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

        add_instr("FIA\n");
        add_instr("PLD %s\n",  v_name[et1 % OFST]);
        add_instr("MLT %s_arr_size\n", v_name[id]);
        add_instr("SADD\n");
    }

    // int na memoria e float var na memoria
    if ((get_type(et1) == 1) && (et1 % OFST != 0) && (get_type(et2) == 2) && (v_isco[et2%OFST] == 0) && (et2 % OFST != 0))
    {
        fprintf (stdout, "Atenção na linha %d: tá vendo que o segundo índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

        add_instr("LOD %s\n",   v_name[et1 % OFST]);
        add_instr("MLT  %s_arr_size\n", v_name[id]);
        add_instr("PFIM %s\n",  v_name[et2 % OFST]);
        add_instr("SADD\n");
    }

    // int na memoria e float const na memoria
    if ((get_type(et1) == 1) && (et1 % OFST != 0) && (get_type(et2) == 2) && (v_isco[et2%OFST] == 1) && (et2 % OFST != 0))
    {
        fprintf (stdout, "Atenção na linha %d: tá vendo que o segundo índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

        add_instr("LOD %s\n",   v_name[et1 % OFST]);
        add_instr("MLT  %s_arr_size\n", v_name[id]);
        add_instr("PFIM %d // %s\n", f2mf(v_name[et2 % OFST]), v_name[et2 % OFST]);
        add_instr("SADD\n");
    }

    // int na memoria e comp const na memoria
    if ((get_type(et1) == 1) && (et1 % OFST != 0) && (get_type(et2) == 5) && (et2 % OFST != 0))
    {
        fprintf(stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

        get_cmp_cst(et2, &etr, &eti);

        add_instr("LOD %s\n",  v_name[et1 % OFST]);
        add_instr("MLT %s_arr_size\n", v_name[id]);
        add_instr("PFIM %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
        add_instr("SADD\n");
    }

    // int na memoria e comp no acc
    if ((get_type(et1) == 1) && (et1 % OFST != 0) && (get_type(et2) == 3) && (et2 % OFST == 0))
    {
        fprintf(stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

        add_instr("SETP aux_index_y\n");
        add_instr("FIA\n");
        add_instr("SET  aux_index_y\n");
        add_instr("LOD %s\n",   v_name[et1 % OFST]);
        add_instr("MLT  %s_arr_size\n", v_name[id]);
        add_instr("ADD  aux_index_y\n");
    }

    // int na memoria e comp na memoria
    if ((get_type(et1) == 1) && (et1 % OFST != 0) && (get_type(et2) == 3) && (et2 % OFST != 0))
    {
        fprintf(stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

        add_instr("LOD %s\n",   v_name[et1 % OFST]);
        add_instr("MLT  %s_arr_size\n", v_name[id]);
        add_instr("PFIM %s\n",  v_name[et2 % OFST]);
        add_instr("SADD\n");
    }

    // float no acc e int no acc
    if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 1) && (et2 % OFST == 0))
    {
        fprintf(stdout, "Atenção na linha %d: tá vendo que o primeiro índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

        add_instr("SETP aux_index_y\n");
        add_instr("FIA\n");
        add_instr("MLT %s_arr_size\n", v_name[id]);
        add_instr("ADD aux_index_y\n");
    }

    // float no acc e int na memoria
    if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 1) && (et2 % OFST != 0))
    {
        fprintf(stdout, "Atenção na linha %d: tá vendo que o primeiro índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

        add_instr("FIA\n");
        add_instr("MLT %s_arr_size\n", v_name[id]);
        add_instr("ADD %s\n",  v_name[et2 % OFST]);
    }

    // float no acc e float no acc
    if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 2) && (et2 % OFST == 0))
    {
        fprintf(stdout, "Atenção na linha %d: tá vendo que os índices do array estão em ponto flutuante né? Vou arredondar!\n", line_num+1);

        add_instr("FIA\n");
        add_instr("SETP aux_index_y\n");
        add_instr("FIA\n");
        add_instr("MLT %s_arr_size\n", v_name[id]);
        add_instr("ADD aux_index_y\n");
    }

    // float no acc e float const na memoria
    if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 2) && (v_isco[et2%OFST] == 1) && (et2 % OFST != 0))
    {
        fprintf(stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

        add_instr("FIA\n");
        add_instr("MLT %s_arr_size\n", v_name[id]);
        add_instr("PFIM %d // %s\n", f2mf(v_name[et2 % OFST]), v_name[et2 % OFST]);
        add_instr("SADD\n");
    }

    // float no acc e float var na memoria
    if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 2) && (v_isco[et2%OFST] == 0) && (et2 % OFST != 0))
    {
        fprintf(stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

        add_instr("FIA\n");
        add_instr("MLT %s_arr_size\n", v_name[id]);
        add_instr("PFIM %s\n", v_name[et2 % OFST]);
        add_instr("SADD\n");
    }

    // float no acc e comp const na memoria
    if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 5) && (et2 % OFST != 0))
    {
        fprintf(stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

        get_cmp_cst(et2, &etr, &eti);

        add_instr("FIA\n");
        add_instr("MLT %s_arr_size\n",    v_name[id]);
        add_instr("PFIM %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
        add_instr("SADD\n");
    }

    // float no acc e comp no acc
    if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 3) && (et2 % OFST == 0))
    {
        fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

        add_instr("SETP aux_index_y\n");
        add_instr("FIA\n");
        add_instr("SETP aux_index_y\n");
        add_instr("FIA\n");
        add_instr("MLT %s_arr_size\n", v_name[id]);
        add_instr("ADD aux_index_y\n");
    }

    // float no acc e comp na memoria
    if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 3) && (et2 % OFST != 0))
    {
        fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

        add_instr("FIA\n");
        add_instr("MLT %s_arr_size\n", v_name[id]);
        add_instr("PFIM %s\n", v_name[et2 % OFST]);
        add_instr("SADD\n");
    }

    // comp const na memoria e int no acc
    if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 1) && (et2 % OFST == 0))
    {
        fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

        get_cmp_cst(et1, &etr, &eti);

        add_instr("FIA\n");
        add_instr("SET aux_index_y\n");
        add_instr("FIM %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
        add_instr("MLT %s_arr_size\n", v_name[id]);
        add_instr("ADD aux_index_y\n");
    }

    // comp const na memoria e int na memoria
    if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 1) && (et2 % OFST != 0))
    {
        fprintf(stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

        get_cmp_cst(et1, &etr, &eti);

        add_instr("FIM %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
        add_instr("MLT %s_arr_size\n",   v_name[id]);
        add_instr("ADD %s\n",            v_name[et2 % OFST]);
    }

    // comp const na memoria e float no acc
    if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 2) && (et2 % OFST == 0))
    {
        fprintf(stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

        get_cmp_cst(et1, &etr, &eti);

        add_instr("FIA\n");
        add_instr("SET aux_index_y\n");
        add_instr("FIM %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
        add_instr("MLT %s_arr_size\n", v_name[id]);
        add_instr("ADD aux_index_y\n");
    }

    // comp const na memoria e float var na memoria
    if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 2) && (v_isco[et2%OFST] == 0) && (et2 % OFST != 0))
    {
        fprintf(stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

        get_cmp_cst(et1, &etr, &eti);

        add_instr("FIM  %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
        add_instr("MLT  %s_arr_size\n",   v_name[id]);
        add_instr("PFIM %s\n",            v_name[et2 % OFST]);
        add_instr("SADD\n");
    }

    // comp const na memoria e float const na memoria
    if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 2) && (v_isco[et2%OFST] == 1) && (et2 % OFST != 0))
    {
        fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

        get_cmp_cst(et1, &etr, &eti);

        add_instr("FIM  %d // %s\n",  f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
        add_instr("MLT  %s_arr_size\n",    v_name[id]);
        add_instr("PFIM %d // %s\n",  f2mf(v_name[et2 % OFST]), v_name[et2 % OFST]);
        add_instr("SADD\n");
    }

    // comp const na memoria e comp const na memoria
    if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 5) && (et2 % OFST != 0))
    {
        fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

        get_cmp_cst(et1, &etr, &eti);
        add_instr("FIM %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
        add_instr("MLT %s_arr_size\n", v_name[id]);

        get_cmp_cst(et2, &etr, &eti);
        add_instr("PFIM %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
        add_instr("SADD\n");
    }

    // comp const na memoria e comp no acc
    if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 3) && (et2 % OFST == 0))
    {
        fprintf(stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

        get_cmp_cst(et1, &etr, &eti);

        add_instr("SETP aux_index_y\n");
        add_instr("FIA\n");
        add_instr("SET  aux_index_y\n");
        add_instr("FIM %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
        add_instr("MLT %s_arr_size\n",   v_name[id]);
        add_instr("ADD aux_index_y\n");
    }

    // comp const na memoria e comp na memoria
    if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 3) && (et2 % OFST != 0))
    {
        fprintf(stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

        get_cmp_cst(et1, &etr, &eti);

        add_instr("FIM  %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
        add_instr("MLT  %s_arr_size\n",   v_name[id]);
        add_instr("PFIM %s\n",            v_name[et2 % OFST]);
        add_instr("SADD\n");
    }

    // comp no acc e int no acc
    if ((get_type(et1) == 3) && (et1 % OFST == 0) && (get_type(et2) == 1) && (et2 % OFST == 0))
    {
        fprintf(stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

        add_instr("SETP aux_index_y\n");
        add_instr("POP\n");
        add_instr("FIA\n");
        add_instr("MLT %s_arr_size\n", v_name[id]);
        add_instr("ADD aux_index_y\n");
    }

    // comp no acc e int na memoria
    if ((get_type(et1) == 3) && (et1 % OFST == 0) && (get_type(et2) == 1) && (et2 % OFST != 0))
    {
        fprintf(stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

        add_instr("POP\n");
        add_instr("FIA\n");
        add_instr("MLT %s_arr_size\n", v_name[id]);
        add_instr("ADD %s\n",  v_name[et2 % OFST]);
    }

    // comp no acc e float no acc
    if ((get_type(et1) == 3) && (et1 % OFST == 0) && (get_type(et2) == 2) && (et2 % OFST == 0))
    {
        fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

        add_instr("FIA\n");
        add_instr("SETP aux_index_y\n");
        add_instr("POP\n");
        add_instr("FIA\n");
        add_instr("MLT %s_arr_size\n", v_name[id]);
        add_instr("ADD aux_index_y\n");
    }

    // comp no acc e float var na memoria
    if ((get_type(et1) == 3) && (et1 % OFST == 0) && (get_type(et2) == 2) && (v_isco[et2%OFST] == 0) && (et2 % OFST != 0))
    {
        fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

        add_instr("SETP aux_index_y\n");
        add_instr("FIA\n");
        add_instr("MLT %s_arr_size\n", v_name[id]);
        add_instr("PFIM %s\n", v_name[et2 % OFST]);
        add_instr("SADD\n");
    }

    // comp no acc e float const na memoria
    if ((get_type(et1) == 3) && (et1 % OFST == 0) && (get_type(et2) == 2) && (v_isco[et2%OFST] == 1) && (et2 % OFST != 0))
    {
        fprintf(stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

        add_instr("POP\n");
        add_instr("FIA\n");
        add_instr("MLT %s_arr_size\n"   , v_name[id]);
        add_instr("PFIM %d // %s\n", f2mf(v_name[et2 % OFST]), v_name[et2 % OFST]);
        add_instr("SADD\n");
    }

    // comp no acc e comp const na memoria
    if ((get_type(et1) == 3) && (et1 % OFST == 0) && (get_type(et2) == 5) && (et2 % OFST != 0))
    {
        fprintf(stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

        get_cmp_cst(et2, &etr, &eti);

        add_instr("POP\n");
        add_instr("FIA\n");
        add_instr("MLT %s_arr_size\n",    v_name[id]);
        add_instr("PFIM %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
        add_instr("SADD\n");
    }

    // comp no acc e comp no acc
    if ((get_type(et1) == 3) && (et1 % OFST == 0) && (get_type(et2) == 3) && (et2 % OFST == 0))
    {
        fprintf(stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

        add_instr("SETP aux_index_y\n");
        add_instr("FIA\n");
        add_instr("SETP aux_index_y\n");
        add_instr("POP\n");
        add_instr("FIA\n");
        add_instr("MLT %s_arr_size\n", v_name[id]);
        add_instr("ADD aux_index_y\n");
    }

    // comp no acc e comp na memoria
    if ((get_type(et1) == 3) && (et1 % OFST == 0) && (get_type(et2) == 3) && (et2 % OFST != 0))
    {
        fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

        add_instr("POP\n");
        add_instr("FIA\n");
        add_instr("MLT %s_arr_size\n", v_name[id]);
        add_instr("PFIM %s\n", v_name[et2 % OFST]);
        add_instr("SADD\n");
    }

    // comp na memoria e int no acc
    if ((get_type(et1) == 3) && (et1 % OFST != 0) && (get_type(et2) == 1) && (et2 % OFST == 0))
    {
        fprintf(stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

        add_instr("PFIM %s\n",  v_name[et1 % OFST]);
        add_instr("MLT  %s_arr_size\n", v_name[id]);
        add_instr("SADD\n");
    }

    // comp na memoria e int na memoria
    if ((get_type(et1) == 3) && (et1 % OFST != 0) && (get_type(et2) == 1) && (et2 % OFST != 0))
    {
        fprintf(stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

        add_instr("FIM %s\n",  v_name[et1 % OFST]);
        add_instr("MLT %s_arr_size\n", v_name[id]);
        add_instr("ADD %s\n",  v_name[et2 % OFST]);
    }

    // comp na memoria e float no acc
    if ((get_type(et1) == 3) && (et1 % OFST != 0) && (get_type(et2) == 2) && (et2 % OFST == 0))
    {
        fprintf(stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

        add_instr("FIA\n");
        add_instr("PFIM %s\n",  v_name[et1 % OFST]);
        add_instr("MLT  %s_arr_size\n", v_name[id]);
        add_instr("SADD\n");
    }

    // comp na memoria e float var na memoria
    if ((get_type(et1) == 3) && (et1 % OFST != 0) && (get_type(et2) == 2) && (v_isco[et2%OFST] == 0) && (et2 % OFST != 0))
    {
        fprintf(stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

        add_instr("FIM  %s\n",  v_name[et1 % OFST]);
        add_instr("MLT  %s_arr_size\n", v_name[id]);
        add_instr("PFIM %s\n",  v_name[et2 % OFST]);
        add_instr("SADD\n");
    }

    // comp na memoria e float const na memoria
    if ((get_type(et1) == 3) && (et1 % OFST != 0) && (get_type(et2) == 2) && (v_isco[et2%OFST] == 1) && (et2 % OFST != 0))
    {
        fprintf(stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

        add_instr("FIM  %s\n",            v_name[et1 % OFST]);
        add_instr("MLT  %s_arr_size\n",   v_name[id]);
        add_instr("PFIM %d // %s\n", f2mf(v_name[et2 % OFST]), v_name[et2 % OFST]);
        add_instr("SADD\n");
    }

    // comp na memoria e comp const na memoria
    if ((get_type(et1) == 3) && (et1 % OFST != 0) && (get_type(et2) == 5) && (et2 % OFST != 0))
    {
        fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

        get_cmp_cst(et2, &etr, &eti);

        add_instr("FIM %s\n", v_name[et1 % OFST]);
        add_instr("MLT %s_arr_size\n", v_name[id]);
        add_instr("PFIM %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
        add_instr("SADD\n");
    }

    // comp na memoria e comp no acc
    if ((get_type(et1) == 3) && (et1 % OFST != 0) && (get_type(et2) == 3) && (et2 % OFST == 0))
    {
        fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

        add_instr("SETP aux_index_y\n");
        add_instr("FIA\n");
        add_instr("SET  aux_index_y\n");
        add_instr("FIM %s\n", v_name[et1 % OFST]);
        add_instr("MLT %s_arr_size\n", v_name[id]);
        add_instr("ADD aux_index_y\n");
    }

    // comp na memoria e comp na memoria
    if ((get_type(et1) == 3) && (et1 % OFST != 0) && (get_type(et2) == 3) && (et2 % OFST != 0))
    {
        fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

        add_instr("FIM %s\n", v_name[et1 % OFST]);
        add_instr("MLT %s_arr_size\n", v_name[id]);
        add_instr("PFIM %s\n", v_name[et2 % OFST]);
        add_instr("SADD\n");
    }

    acc_ok = 1; // acc carregado
}

// ----------------------------------------------------------------------------
// Array em expressoes. EX: x = y[i]; -----------------------------------------
// ----------------------------------------------------------------------------

// transforma array 1d num exp
// parametro fft diz se eh pra usar indice invertido
int array1d2exp(int id, int et, int fft)
{
    // testes de consistencia -------------------------------------------------

    // testa se a variavel ja foi declarada
    if (v_type[id] == 0)
        fprintf (stderr, "Erro na linha %d: mané, declara a variável %s direito!\n", line_num+1, rem_fname(v_name[id], fname));

    // testa se a variavel ja recebeu um valor
    if (v_asgn[id] == 0)
        fprintf (stdout, "Atenção na linha %d: como você quer usar %s se você nem deu um valor ainda?\n", line_num+1, rem_fname(v_name[id], fname));

    // tem que ver se eh array mesmo
    if (v_isar[id] == 0)
        fprintf (stderr, "Erro na linha %d: %s não é array não, abensoado!\n", line_num+1, rem_fname(v_name[id], fname));

    // tem que ver se eh array 1D mesmo
    if (v_isar[id] == 2)
        fprintf (stderr, "Erro na linha %d: array %s tem duas dimensões!\n"  , line_num+1, rem_fname(v_name[id], fname));

    // prepara os comandos de LOD --------------------------------------------

    char ldv[10]; if (fft    == 1) strcpy(ldv,"ILDI"); else strcpy(ldv,"LDI" );
    char ldi[10]; if (acc_ok == 1) strcpy(ldi,"PLD" ); else strcpy(ldi,"LOD");
    char f2i[10]; if (acc_ok == 1) strcpy(f2i,"PFIM"); else strcpy(f2i,"FIM" );

    // ------------------------------------------------------------------------
    // escreve as instrucoes --------------------------------------------------
    // ------------------------------------------------------------------------

    char num[32];
    int  etr, eti;
    int  type = v_type[id];

    // left int/float ---------------------------------------------------------

    if (type < 3)
    {
        // int no acc
        if ((get_type(et) == 1) && (et % OFST == 0))
        {
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // int na memoria
        if ((get_type(et) == 1) && (et % OFST != 0))
        {
            add_instr("%s %s\n", ldi, v_name[et % OFST]);
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float no acc
        if ((get_type(et) == 2) && (et % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array tá dando float. Vai gerar muito código pra arredondar!\n", line_num+1);

            add_instr("FIA\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float var na memoria
        if ((get_type(et) == 2) && (v_isco[et%OFST] == 0) && (et % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array tá dando float. Vai gerar muito código pra arredondar!\n", line_num+1);

            add_instr("%s %s\n", f2i, v_name[et % OFST]);
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float const na memoria
        if ((get_type(et) == 2) && (v_isco[et%OFST] == 1) && (et % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array tá dando float. Vai gerar muito código pra arredondar!\n", line_num+1);

            add_instr("%s %d // %s\n", f2i, f2mf(v_name[et % OFST]), v_name[et % OFST]);
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp const na memoria
        if ((get_type(et) == 5) && (et % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou arredondar a parte real.\n", line_num+1);

            get_cmp_cst(et, &etr, &eti);

            add_instr("%s %d // %s\n", f2i, f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp no acc
        if ((get_type(et) == 3) && (et % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou arredondar a parte real.\n", line_num+1);

            add_instr("POP\n");
            add_instr("FIA\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp na memoria
        if ((get_type(et) == 3) && (et % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou arredondar a parte real.\n", line_num+1);

            add_instr("%s %s\n", f2i, v_name[et%OFST]);
            add_instr("%s %s\n", ldv, v_name[id]);
        }
    }

    // left comp --------------------------------------------------------------

    else
    {
        // int no acc
        if ((get_type(et) == 1) && (et % OFST == 0))
        {
            add_instr("SET aux_ind_right\n");
            add_instr("%s %s\n"  , ldv, v_name[id]);
            add_instr("PLD aux_ind_right\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // int na memoria
        if ((get_type(et) == 1) && (et % OFST != 0))
        {
            add_instr("%s %s\n"  , ldi, v_name[et % OFST]);
            add_instr("%s %s\n"  , ldv, v_name[id]);
            add_instr("PLD %s\n" ,      v_name[et % OFST]);
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // float no acc
        if ((get_type(et) == 2) && (et % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array tá dando float. Vai gerar muito código pra arredondar!\n", line_num+1);

            add_instr("FIA\n");
            add_instr("SET aux_ind_right\n");
            add_instr("%s %s\n"  , ldv, v_name[id]);
            add_instr("PLD aux_ind_right\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // float var na memoria
        if ((get_type(et) == 2) && (v_isco[et%OFST] == 0) && (et % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array tá dando float. Vai gerar muito código pra arredondar!\n", line_num+1);

            add_instr("%s %s\n", f2i, v_name[et % OFST]);
            add_instr("SET aux_ind_right\n");
            add_instr("%s %s\n"  , ldv, v_name[id]);
            add_instr("PLD aux_ind_right\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // float const na memoria
        if ((get_type(et) == 2) && (v_isco[et%OFST] == 1) && (et % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array tá dando float. Vai gerar muito código pra arredondar!\n", line_num+1);

            add_instr("%s %d // %s\n", f2i, f2mf(v_name[et % OFST]), v_name[et % OFST]);
            add_instr("SET aux_ind_right\n");
            add_instr("%s %s\n"  , ldv, v_name[id]);
            add_instr("PLD aux_ind_right\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // comp const na memoria
        if ((get_type(et) == 5) && (et % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou arredondar a parte real.\n", line_num+1);

            get_cmp_cst(et, &etr, &eti);

            add_instr("%s %d // %s\n", f2i, f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("SET aux_ind_right\n");
            add_instr("%s %s\n"  , ldv, v_name[id]);
            add_instr("PLD aux_ind_right\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // comp no acc
        if ((get_type(et) == 3) && (et % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou arredondar a parte real.\n", line_num+1);

            add_instr("POP\n");
            add_instr("FIA\n");
            add_instr("SET aux_ind_right\n");
            add_instr("%s %s\n"  , ldv, v_name[id]);
            add_instr("PLD aux_ind_right\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // comp na memoria
        if ((get_type(et) == 3) && (et % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou arredondar a parte real.\n", line_num+1);

            add_instr("%s %s\n", f2i, v_name[et%OFST]);
            add_instr("SET aux_ind_right\n");
            add_instr("%s %s\n"  , ldv, v_name[id]);
            add_instr("PLD aux_ind_right\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }
    }

    acc_ok     = 1;
    v_used[id] = 1;

    return v_type[id]*OFST;
}

// transforma array 2D em exp
int array2d2exp(int id, int et1, int et2)
{
    // testes de consistencia -------------------------------------------------

    // testa se a variavel ja foi declarada
    if (v_type[id] == 0)
        fprintf (stderr, "Erro na linha %d: mané, declara a variável %s direito!\n", line_num+1, rem_fname(v_name[id], fname));

    // testa se a variavel ja recebeu um valor
    if (v_asgn[id] == 0)
        fprintf (stdout, "Atenção na linha %d: como você quer usar %s se você nem deu um valor ainda?\n", line_num+1, rem_fname(v_name[id], fname));

    // tem que ver se eh array 2D mesmo
    if (v_isar[id] == 0)
        fprintf (stderr, "Erro na linha %d: %s não é array não, abensoado!\n", line_num+1, rem_fname(v_name[id], fname));

    // tem que ver se nao eh array 1D
    if (v_isar[id] == 1)
        fprintf (stderr, "Erro na linha %d: array %s tem uma dimensão só!\n" , line_num+1, rem_fname(v_name[id], fname));

    // prepara os comandos de LOD --------------------------------------------

    char ldv[10];                  strcpy(ldv,"LDI" ); // ainda nao tem fft 2d
    char ldi[10]; if (acc_ok == 1) strcpy(ldi,"PLD" ); else strcpy(ldi,"LOD");
    char f2i[10]; if (acc_ok == 1) strcpy(f2i,"PFIM"); else strcpy(f2i,"FIM" );

    // ------------------------------------------------------------------------
    // escreve as instrucoes --------------------------------------------------
    // ------------------------------------------------------------------------

    char num[32];
    int  etr, eti;
    int  type = v_type[id];

    // left int/float ---------------------------------------------------------

    if (type < 3)
    {
        // int no acc e int no acc
        if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 1) && (et2 % OFST == 0))
        {
            add_instr("SETP aux_index_y\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // int no acc e int na memoria
        if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 1) && (et2 % OFST != 0))
        {
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // int no acc e float no acc
        if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 2) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o segundo índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("FIA\n");
            add_instr("SETP aux_index_y\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // int no acc e float var na memoria
        if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 2) && (v_isco[et2%OFST]==0) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o segundo índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %s\n", v_name[et2 % OFST]);
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // int no acc e float const na memoria
        if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 2) && (v_isco[et2%OFST]==1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o segundo índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %d // %s\n", f2mf(v_name[et2 % OFST]), v_name[et2 % OFST]);
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // int no acc e comp const na memoria
        if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 5) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            get_cmp_cst(et2, &etr, &eti);

            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // int no acc e comp no acc
        if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 3) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("FIA\n");
            add_instr("SETP aux_index_y\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // int no acc e comp na memoria
        if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 3) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %s\n", v_name[et2 % OFST]);
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // int na memoria e int no acc
        if ((get_type(et1) == 1) && (et1 % OFST != 0) && (get_type(et2) == 1) && (et2 % OFST == 0))
        {
            add_instr("PLD %s\n", v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // int na memoria e int na memoria
        if ((get_type(et1) == 1) && (et1 % OFST != 0) && (get_type(et2) == 1) && (et2 % OFST != 0))
        {
            add_instr("%s %s\n", ldi, v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // int na memoria e float no acc
        if ((get_type(et1) == 1) && (et1 % OFST != 0) && (get_type(et2) == 2) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o segundo índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("FIA\n");
            add_instr("PLD %s\n", v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // int na memoria e float var na memoria
        if ((get_type(et1) == 1) && (et1 % OFST != 0) && (get_type(et2) == 2) && (v_isco[et2%OFST]==0) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o segundo índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("%s %s\n", ldi, v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %s\n", v_name[et2 % OFST]);
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // int na memoria e float const na memoria
        if ((get_type(et1) == 1) && (et1 % OFST != 0) && (get_type(et2) == 2) && (v_isco[et2%OFST]==1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o segundo índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("%s %s\n", ldi, v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %d // %s\n", f2mf(v_name[et2 % OFST]), v_name[et2 % OFST]);
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // int na memoria e comp const na memoria
        if ((get_type(et1) == 1) && (et1 % OFST != 0) && (get_type(et2) == 5) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            get_cmp_cst(et2, &etr, &eti);

            add_instr("%s %s\n", ldi, v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // int na memoria e comp no acc
        if ((get_type(et1) == 1) && (et1 % OFST != 0) && (get_type(et2) == 3) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("FIA\n");
            add_instr("SET  aux_index_y\n");
            add_instr("LOD %s\n",   v_name[et1 % OFST]);
            add_instr("MLT  %s_arr_size\n", v_name[id]);
            add_instr("ADD  aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // int na memoria e comp na memoria
        if ((get_type(et1) == 1) && (et1 % OFST != 0) && (get_type(et2) == 3) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            add_instr("%s %s\n", ldi, v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %s\n", v_name[et2 % OFST]);
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float no acc e int no acc
        if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 1) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o primeiro índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("FIA\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float no acc e int na memoria
        if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o primeiro índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("FIA\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float no acc e float no acc
        if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 2) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que os índices do array estão em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("FIA\n");
            add_instr("SETP aux_index_y\n");
            add_instr("FIA\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float no acc e float var na memoria
        if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 2) && (v_isco[et2%OFST]==0) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("FIA\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %s\n", v_name[et2 % OFST]);
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float no acc e float const na memoria
        if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 2) && (v_isco[et2%OFST]==1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("FIA\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %d // %s\n", f2mf(v_name[et2 % OFST]), v_name[et2 % OFST]);
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float no acc e comp const na memoria
        if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 5) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            get_cmp_cst(et2, &etr, &eti);

            add_instr("FIA\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float no acc e comp no acc
        if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 3) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("FIA\n");
            add_instr("SETP aux_index_y\n");
            add_instr("FIA\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float no acc e comp na memoria
        if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 3) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("FIA\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %s\n", v_name[et2 % OFST]);
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float var na memoria e int no acc
        if ((get_type(et1) == 2) && (et1 % OFST != 0) && (v_isco[et1%OFST]==0) && (get_type(et2) == 1) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o primeiro índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("SET aux_index_y\n");
            add_instr("FIM %s\n", v_name[et1%OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float var na memoria e int na memoria
        if ((get_type(et1) == 2) && (et1 % OFST != 0) && (v_isco[et1%OFST]==0) && (get_type(et2) == 1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o primeiro índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("%s %s\n", f2i, v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float var na memoria e float no acc
        if ((get_type(et1) == 2) && (et1 % OFST != 0) && (v_isco[et1%OFST]==0) && (get_type(et2) == 2) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que os índices do array estão em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("FIA\n");
            add_instr("SET aux_index_y\n");
            add_instr("FIM %s\n", v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float var na memoria e float var na memoria
        if ((get_type(et1) == 2) && (et1 % OFST != 0) && (v_isco[et1%OFST]==0) && (get_type(et2) == 2) && (v_isco[et2%OFST]==0) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("%s %s\n", f2i, v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %s\n", v_name[et2 % OFST]);
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float var na memoria e float const na memoria
        if ((get_type(et1) == 2) && (et1 % OFST != 0) && (v_isco[et1%OFST]==0) && (get_type(et2) == 2) && (v_isco[et2%OFST]==1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("%s %s\n", f2i, v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %d // %s\n", f2mf(v_name[et2 % OFST]), v_name[et2 % OFST]);
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float var na memoria e comp const na memoria
        if ((get_type(et1) == 2) && (et1 % OFST != 0) && (v_isco[et1%OFST]==0) && (get_type(et2) == 5) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            get_cmp_cst(et2, &etr, &eti);

            add_instr("%s %s\n", f2i, v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float var na memoria e comp no acc
        if ((get_type(et1) == 2) && (et1 % OFST != 0) && (v_isco[et1%OFST]==0) && (get_type(et2) == 3) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("FIA\n");
            add_instr("SET aux_index_y\n");
            add_instr("FIM %s \n", v_name[et1%OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float var na memoria e comp na memoria
        if ((get_type(et1) == 2) && (et1 % OFST != 0) && (v_isco[et1%OFST]==0) && (get_type(et2) == 3) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("%s %s\n", f2i, v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %s\n", v_name[et2 % OFST]);
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float const na memoria e int no acc
        if ((get_type(et1) == 2) && (et1 % OFST != 0) && (v_isco[et1%OFST]==1) && (get_type(et2) == 1) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o primeiro índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("SET aux_index_y\n");
            add_instr("FIM %d // %s\n", f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float const na memoria e int na memoria
        if ((get_type(et1) == 2) && (et1 % OFST != 0) && (v_isco[et1%OFST]==1) && (get_type(et2) == 1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o primeiro índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("%s %d // %s\n", f2i, f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float const na memoria e float no acc
        if ((get_type(et1) == 2) && (et1 % OFST != 0) && (v_isco[et1%OFST]==1) && (get_type(et2) == 2) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que os índices do array estão em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("FIA\n");
            add_instr("SET aux_index_y\n");
            add_instr("FIM %d // %s\n", f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float const na memoria e float var na memoria
        if ((get_type(et1) == 2) && (et1 % OFST != 0) && (v_isco[et1%OFST]==1) && (get_type(et2) == 2) && (v_isco[et2%OFST]==0) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("%s %d // %s\n", f2i, f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %s\n", v_name[et2 % OFST]);
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float const na memoria e float const na memoria
        if ((get_type(et1) == 2) && (et1 % OFST != 0) && (v_isco[et1%OFST]==1) && (get_type(et2) == 2) && (v_isco[et2%OFST]==1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("%s %d // %s\n", f2i, f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %d // %s\n", f2mf(v_name[et2 % OFST]), v_name[et2 % OFST]);
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float const na memoria e comp const na memoria
        if ((get_type(et1) == 2) && (et1 % OFST != 0) && (v_isco[et1%OFST]==1) && (get_type(et2) == 5) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            get_cmp_cst(et2, &etr, &eti);

            add_instr("%s %d // %s\n", f2i, f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float const na memoria e comp no acc
        if ((get_type(et1) == 2) && (et1 % OFST != 0) && (v_isco[et1%OFST]==1) && (get_type(et2) == 3) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("FIA\n");
            add_instr("SET aux_index_y\n");
            add_instr("FIM %d // %s\n", f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float const na memoria e comp na memoria
        if ((get_type(et1) == 2) && (et1 % OFST != 0) && (v_isco[et1%OFST]==1) && (get_type(et2) == 3) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("%s %d // %s\n", f2i, f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %s\n", v_name[et2 % OFST]);
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp const na memoria e int no acc
        if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 1) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            get_cmp_cst(et1, &etr, &eti);

            add_instr("FIA\n");
            add_instr("SET aux_index_y\n");
            add_instr("FIM %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp const na memoria e int na memoria
        if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            get_cmp_cst(et1, &etr, &eti);

            add_instr("%s %d // %s\n", f2i, f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp const na memoria e float no acc
        if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 2) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            get_cmp_cst(et1, &etr, &eti);

            add_instr("FIA\n");
            add_instr("SET aux_index_y\n");
            add_instr("FIM %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp const na memoria e float var na memoria
        if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 2) && (v_isco[et2%OFST]==0) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            get_cmp_cst(et1, &etr, &eti);

            add_instr("%s %d // %s\n", f2i, f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %s\n", v_name[et2 % OFST]);
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp const na memoria e float const na memoria
        if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 2) && (v_isco[et2%OFST]==1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            get_cmp_cst(et1, &etr, &eti);

            add_instr("%s %d // %s\n", f2i, f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %d // %s\n", f2mf(v_name[et2 % OFST]), v_name[et2 % OFST]);
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp const na memoria e comp const na memoria
        if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 5) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            get_cmp_cst(et1, &etr, &eti);
            add_instr("%s %d // %s\n", f2i, f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);

            get_cmp_cst(et2, &etr, &eti);
            add_instr("PFIM %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp const na memoria e comp no acc
        if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 3) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            get_cmp_cst(et1, &etr, &eti);

            add_instr("SETP aux_index_y\n");
            add_instr("FIA\n");
            add_instr("SET  aux_index_y\n");
            add_instr("FIM %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp const na memoria e comp na memoria
        if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 3) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            get_cmp_cst(et1, &etr, &eti);

            add_instr("%s %d // %s\n", f2i, f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %s\n", v_name[et2 % OFST]);
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp no acc e int no acc
        if ((get_type(et1) == 3) && (et1 % OFST == 0) && (get_type(et2) == 1) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("POP\n");
            add_instr("FIA\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp no acc e int na memoria
        if ((get_type(et1) == 3) && (et1 % OFST == 0) && (get_type(et2) == 1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("FIA\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp no acc e float no acc
        if ((get_type(et1) == 3) && (et1 % OFST == 0) && (get_type(et2) == 2) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("FIA\n");
            add_instr("SETP aux_index_y\n");
            add_instr("POP\n");
            add_instr("FIA\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp no acc e float var na memoria
        if ((get_type(et1) == 3) && (et1 % OFST == 0) && (get_type(et2) == 2) && (v_isco[et2%OFST]==0) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("FIA\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %s\n", v_name[et2 % OFST]);
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp no acc e float const na memoria
        if ((get_type(et1) == 3) && (et1 % OFST == 0) && (get_type(et2) == 2) && (v_isco[et2%OFST]==1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("FIA\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %d // %s\n", f2mf(v_name[et2 % OFST]), v_name[et2 % OFST]);
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp no acc e comp const na memoria
        if ((get_type(et1) == 3) && (et1 % OFST == 0) && (get_type(et2) == 5) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            get_cmp_cst(et2, &etr, &eti);

            add_instr("SETP aux_index_y\n");
            add_instr("FIA\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp no acc e comp no acc
        if ((get_type(et1) == 3) && (et1 % OFST == 0) && (get_type(et2) == 3) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("FIA\n");
            add_instr("SETP aux_index_y\n");
            add_instr("POP\n");
            add_instr("FIA\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp no acc e comp na memoria
        if ((get_type(et1) == 3) && (et1 % OFST == 0) && (get_type(et2) == 3) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("FIA\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %s\n", v_name[et2 % OFST]);
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp na memoria e int no acc
        if ((get_type(et1) == 3) && (et1 % OFST != 0) && (get_type(et2) == 1) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            add_instr("PFIM %s\n", v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp na memoria e int na memoria
        if ((get_type(et1) == 3) && (et1 % OFST != 0) && (get_type(et2) == 1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            add_instr("%s %s\n", f2i, v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp na memoria e float no acc
        if ((get_type(et1) == 3) && (et1 % OFST != 0) && (get_type(et2) == 2) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("FIA\n");
            add_instr("PFIM %s\n", v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp na memoria e float var na memoria
        if ((get_type(et1) == 3) && (et1 % OFST != 0) && (get_type(et2) == 2) && (v_isco[et2%OFST]==0) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("%s %s\n", f2i, v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %s\n", v_name[et2 % OFST]);
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp na memoria e float const na memoria
        if ((get_type(et1) == 3) && (et1 % OFST != 0) && (get_type(et2) == 2) && (v_isco[et2%OFST]==1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("%s %s\n", f2i, v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %d // %s\n", f2mf(v_name[et2 % OFST]), v_name[et2 % OFST]);
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp na memoria e comp const na memoria
        if ((get_type(et1) == 3) && (et1 % OFST != 0) && (get_type(et2) == 5) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            get_cmp_cst(et2, &etr, &eti);

            add_instr("%s %s\n", f2i, v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp na memoria e comp no acc
        if ((get_type(et1) == 3) && (et1 % OFST != 0) && (get_type(et2) == 3) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("FIA\n");
            add_instr("SET  aux_index_y\n");
            add_instr("FIM %s\n", v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp na memoria e comp na memoria
        if ((get_type(et1) == 3) && (et1 % OFST != 0) && (get_type(et2) == 3) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("%s %s\n", f2i, v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %s\n", v_name[et2 % OFST]);
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }
    }

    // left comp --------------------------------------------------------------

    else
    {
        // int no acc e int no acc
        if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 1) && (et2 % OFST == 0))
        {
            add_instr("SETP aux_index_y\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // int no acc e int na memoria
        if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 1) && (et2 % OFST != 0))
        {
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // int no acc e float no acc
        if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 2) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o segundo índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("FIA\n");
            add_instr("SETP aux_index_y\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // int no acc e float var na memoria
        if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 2) && (v_isco[et2%OFST] == 0) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o segundo índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %s\n", v_name[et2 % OFST]);
            add_instr("SADD\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // int no acc e float const na memoria
        if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 2) && (v_isco[et2%OFST] == 1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o segundo índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %d // %s\n", f2mf(v_name[et2 % OFST]), v_name[et2 % OFST]);
            add_instr("SADD\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // int no acc e comp const na memoria
        if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 5) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            get_cmp_cst(et2, &etr, &eti);

            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("SADD\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // int no acc e comp no acc
        if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 3) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("FIA\n");
            add_instr("SETP aux_index_y\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // int no acc e comp na memoria
        if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 3) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %s\n", v_name[et2 % OFST]);
            add_instr("SADD\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // int na memoria e int no acc
        if ((get_type(et1) == 1) && (et1 % OFST != 0) && (get_type(et2) == 1) && (et2 % OFST == 0))
        {
            add_instr("PLD %s\n", v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("SADD\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // int na memoria e int na memoria
        if ((get_type(et1) == 1) && (et1 % OFST != 0) && (get_type(et2) == 1) && (et2 % OFST != 0))
        {
            add_instr("%s %s\n", ldi, v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // int na memoria e float no acc
        if ((get_type(et1) == 1) && (et1 % OFST != 0) && (get_type(et2) == 2) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o segundo índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("FIA\n");
            add_instr("PLD %s\n", v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("SADD\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // int na memoria e float var na memoria
        if ((get_type(et1) == 1) && (et1 % OFST != 0) && (get_type(et2) == 2) && (v_isco[et2%OFST] == 0) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o segundo índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("%s %s\n", ldi, v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %s\n", v_name[et2 % OFST]);
            add_instr("SADD\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // int na memoria e float const na memoria
        if ((get_type(et1) == 1) && (et1 % OFST != 0) && (get_type(et2) == 2) && (v_isco[et2%OFST] == 1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o segundo índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("%s %s\n", ldi, v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %d // %s\n", f2mf(v_name[et2 % OFST]), v_name[et2 % OFST]);
            add_instr("SADD\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // int na memoria e comp const na memoria
        if ((get_type(et1) == 1) && (et1 % OFST != 0) && (get_type(et2) == 5) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            get_cmp_cst(et2, &etr, &eti);

            add_instr("%s %s\n", ldi, v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("SADD\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // int na memoria e comp no acc
        if ((get_type(et1) == 1) && (et1 % OFST != 0) && (get_type(et2) == 3) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("FIA\n");
            add_instr("SET  aux_index_y\n");
            add_instr("LOD %s\n",   v_name[et1 % OFST]);
            add_instr("MLT  %s_arr_size\n", v_name[id]);
            add_instr("ADD  aux_index_y\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // int na memoria e comp na memoria
        if ((get_type(et1) == 1) && (et1 % OFST != 0) && (get_type(et2) == 3) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            add_instr("%s %s\n", ldi, v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %s\n", v_name[et2 % OFST]);
            add_instr("SADD\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // float no acc e int no acc
        if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 1) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o primeiro índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("FIA\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // float no acc e int na memoria
        if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o primeiro índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("FIA\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // float no acc e float no acc
        if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 2) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que os índices do array estão em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("FIA\n");
            add_instr("SETP aux_index_y\n");
            add_instr("FIA\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // float no acc e float var na memoria
        if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 2) && (v_isco[et2%OFST]==0) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("FIA\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %s\n", v_name[et2 % OFST]);
            add_instr("SADD\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // float no acc e float const na memoria
        if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 2) && (v_isco[et2%OFST]==1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("FIA\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %d // %s\n", f2mf(v_name[et2 % OFST]), v_name[et2 % OFST]);
            add_instr("SADD\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // float no acc e comp const na memoria
        if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 5) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            get_cmp_cst(et2, &etr, &eti);

            add_instr("FIA\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("SADD\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // float no acc e comp no acc
        if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 3) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("FIA\n");
            add_instr("SETP aux_index_y\n");
            add_instr("FIA\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // float no acc e comp na memoria
        if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 3) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("FIA\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %s\n", v_name[et2 % OFST]);
            add_instr("SADD\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // float var na memoria e int no acc
        if ((get_type(et1) == 2) && (v_isco[et1%OFST] == 0) && (et1 % OFST != 0) && (get_type(et2) == 1) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            add_instr("FIA\n");
            add_instr("SET aux_index_y\n");
            add_instr("FIM %s\n", v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // float var na memoria e int na memoria
        if ((get_type(et1) == 2) && (v_isco[et1%OFST] == 0) && (et1 % OFST != 0) && (get_type(et2) == 1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            add_instr("%s %s\n", f2i, v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // float var na memoria e float no acc
        if ((get_type(et1) == 2) && (v_isco[et1%OFST] == 0) && (et1 % OFST != 0) && (get_type(et2) == 2) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("FIA\n");
            add_instr("SET aux_index_y\n");
            add_instr("FIM %s\n", v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // float var na memoria e float var na memoria
        if ((get_type(et1) == 2) && (v_isco[et1%OFST] == 0) && (et1 % OFST != 0) && (get_type(et2) == 2) && (v_isco[et2%OFST] == 0) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("%s %s\n", f2i, v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %s\n", v_name[et2 % OFST]);
            add_instr("SADD\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // float var na memoria e float const na memoria
        if ((get_type(et1) == 2) && (v_isco[et1%OFST] == 0) && (et1 % OFST != 0) && (get_type(et2) == 2) && (v_isco[et2%OFST] == 1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("%s %s\n", f2i, v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %d // %s\n", f2mf(v_name[et2 % OFST]), v_name[et2 % OFST]);
            add_instr("SADD\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // float var na memoria e comp const na memoria
        if ((get_type(et1) == 2) && (v_isco[et1%OFST] == 0) && (et1 % OFST != 0) && (get_type(et2) == 5) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("%s %s\n", f2i, v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);

            get_cmp_cst(et2, &etr, &eti);

            add_instr("PFIM %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("SADD\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // float var na memoria e comp no acc
        if ((get_type(et1) == 2) && (v_isco[et1%OFST] == 0) && (et1 % OFST != 0) && (get_type(et2) == 3) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("FIA\n");
            add_instr("SET  aux_index_y\n");
            add_instr("FIM %s\n", v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // float var na memoria e comp na memoria
        if ((get_type(et1) == 2) && (v_isco[et1%OFST] == 0) && (et1 % OFST != 0) && (get_type(et2) == 3) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("%s %s\n", f2i, v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %s\n", v_name[et2 % OFST]);
            add_instr("SADD\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // float const na memoria e int no acc
        if ((get_type(et1) == 2) && (v_isco[et1%OFST] == 1) && (et1 % OFST != 1) && (get_type(et2) == 1) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            add_instr("FIA\n");
            add_instr("SET aux_index_y\n");
            add_instr("FIM %d // %s\n", f2mf(v_name[et1 % OFST]), v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // float const na memoria e int na memoria
        if ((get_type(et1) == 2) && (v_isco[et1%OFST] == 1) && (et1 % OFST != 0) && (get_type(et2) == 1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            add_instr("%s %d // %s\n", f2i, f2mf(v_name[et1 % OFST]), v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // float const na memoria e float no acc
        if ((get_type(et1) == 2) && (v_isco[et1%OFST] == 1) && (et1 % OFST != 0) && (get_type(et2) == 2) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("FIA\n");
            add_instr("SET aux_index_y\n");
            add_instr("FIM %d // %s\n", f2mf(v_name[et1 % OFST]), v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // float const na memoria e float var na memoria
        if ((get_type(et1) == 2) && (v_isco[et1%OFST] == 1) && (et1 % OFST != 0) && (get_type(et2) == 2) && (v_isco[et2%OFST] == 0) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("%s %d // %s\n", f2i, f2mf(v_name[et1 % OFST]), v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %s\n", v_name[et2 % OFST]);
            add_instr("SADD\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // float const na memoria e float const na memoria
        if ((get_type(et1) == 2) && (v_isco[et1%OFST] == 1) && (et1 % OFST != 0) && (get_type(et2) == 2) && (v_isco[et2%OFST] == 1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("%s %d // %s\n", f2i, f2mf(v_name[et1 % OFST]), v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %d // %s\n", f2mf(v_name[et2 % OFST]), v_name[et2 % OFST]);
            add_instr("SADD\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // float const na memoria e comp const na memoria
        if ((get_type(et1) == 2) && (v_isco[et1%OFST] == 1) && (et1 % OFST != 0) && (get_type(et2) == 5) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("%s %d // %s\n", f2i, f2mf(v_name[et1 % OFST]), v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);

            get_cmp_cst(et2, &etr, &eti);

            add_instr("PFIM %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("SADD\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // float const na memoria e comp no acc
        if ((get_type(et1) == 2) && (v_isco[et1%OFST] == 1) && (et1 % OFST != 0) && (get_type(et2) == 3) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("FIA\n");
            add_instr("SET  aux_index_y\n");
            add_instr("FIM %d // %s\n", f2mf(v_name[et1 % OFST]), v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // float const na memoria e comp na memoria
        if ((get_type(et1) == 2) && (v_isco[et1%OFST] == 1) && (et1 % OFST != 0) && (get_type(et2) == 3) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("%s %d // %s\n", f2i, f2mf(v_name[et1 % OFST]), v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %s\n", v_name[et2 % OFST]);
            add_instr("SADD\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // comp const na memoria e int no acc
        if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 1) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            get_cmp_cst(et1, &etr, &eti);

            add_instr("FIA\n");
            add_instr("SET aux_index_y\n");
            add_instr("FIM %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // comp const na memoria e int na memoria
        if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            get_cmp_cst(et1, &etr, &eti);

            add_instr("%s %d // %s\n", f2i, f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // comp const na memoria e float no acc
        if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 2) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            get_cmp_cst(et1, &etr, &eti);

            add_instr("FIA\n");
            add_instr("SET aux_index_y\n");
            add_instr("FIM %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // comp const na memoria e float var na memoria
        if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 2) && (v_isco[et2%OFST] == 0) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            get_cmp_cst(et1, &etr, &eti);

            add_instr("%s %d // %s\n", f2i, f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %s\n", v_name[et2 % OFST]);
            add_instr("SADD\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // comp const na memoria e float const na memoria
        if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 2) && (v_isco[et2%OFST] == 1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            get_cmp_cst(et1, &etr, &eti);

            add_instr("%s %d // %s\n", f2i, f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %d // %s\n", f2mf(v_name[et2 % OFST]), v_name[et2 % OFST]);
            add_instr("SADD\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // comp const na memoria e comp const na memoria
        if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 5) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            get_cmp_cst(et1, &etr, &eti);
            add_instr("%s %d // %s\n", f2i, f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);

            get_cmp_cst(et2, &etr, &eti);
            add_instr("PFIM %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("SADD\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // comp const na memoria e comp no acc
        if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 3) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            get_cmp_cst(et1, &etr, &eti);

            add_instr("SETP aux_index_y\n");
            add_instr("FIA\n");
            add_instr("SET  aux_index_y\n");
            add_instr("FIM %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // comp const na memoria e comp na memoria
        if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 3) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            get_cmp_cst(et1, &etr, &eti);

            add_instr("%s %d // %s\n", f2i, f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %s\n", v_name[et2 % OFST]);
            add_instr("SADD\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // comp no acc e int no acc
        if ((get_type(et1) == 3) && (et1 % OFST == 0) && (get_type(et2) == 1) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("POP\n");
            add_instr("FIA\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // comp no acc e int na memoria
        if ((get_type(et1) == 3) && (et1 % OFST == 0) && (get_type(et2) == 1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("FIA\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // comp no acc e float no acc
        if ((get_type(et1) == 3) && (et1 % OFST == 0) && (get_type(et2) == 2) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("FIA\n");
            add_instr("SETP aux_index_y\n");
            add_instr("POP\n");
            add_instr("FIA\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // comp no acc e float var na memoria
        if ((get_type(et1) == 3) && (et1 % OFST == 0) && (get_type(et2) == 2) && (v_isco[et2%OFST] == 0)  && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("FIA\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %s\n", v_name[et2 % OFST]);
            add_instr("SADD\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // comp no acc e float const na memoria
        if ((get_type(et1) == 3) && (et1 % OFST == 0) && (get_type(et2) == 2) && (v_isco[et2%OFST] == 1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("FIA\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %d // %s\n", f2mf(v_name[et2 % OFST]), v_name[et2 % OFST]);
            add_instr("SADD\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // comp no acc e comp const na memoria
        if ((get_type(et1) == 3) && (et1 % OFST == 0) && (get_type(et2) == 5) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            get_cmp_cst(et2, &etr, &eti);

            add_instr("SETP aux_index_y\n");
            add_instr("FIA\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("SADD\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // comp no acc e comp no acc
        if ((get_type(et1) == 3) && (et1 % OFST == 0) && (get_type(et2) == 3) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("FIA\n");
            add_instr("SETP aux_index_y\n");
            add_instr("POP\n");
            add_instr("FIA\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // comp no acc e comp na memoria
        if ((get_type(et1) == 3) && (et1 % OFST == 0) && (get_type(et2) == 3) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("FIA\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %s\n", v_name[et2 % OFST]);
            add_instr("SADD\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // comp na memoria e int no acc
        if ((get_type(et1) == 3) && (et1 % OFST != 0) && (get_type(et2) == 1) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            add_instr("PFIM %s\n", v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("SADD\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // comp na memoria e int na memoria
        if ((get_type(et1) == 3) && (et1 % OFST != 0) && (get_type(et2) == 1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            add_instr("%s %s\n", f2i, v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // comp na memoria e float no acc
        if ((get_type(et1) == 3) && (et1 % OFST != 0) && (get_type(et2) == 2) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("FIA\n");
            add_instr("PFIM %s\n", v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("SADD\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // comp na memoria e float var na memoria
        if ((get_type(et1) == 3) && (et1 % OFST != 0) && (get_type(et2) == 2) && (v_isco[et2%OFST] == 0) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("%s %s\n", f2i, v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %s\n", v_name[et2 % OFST]);
            add_instr("SADD\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // comp na memoria e float const na memoria
        if ((get_type(et1) == 3) && (et1 % OFST != 0) && (get_type(et2) == 2) && (v_isco[et2%OFST] == 1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("%s %s\n", f2i, v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %d // %s\n", f2mf(v_name[et2 % OFST]), v_name[et2 % OFST]);
            add_instr("SADD\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // comp na memoria e comp const na memoria
        if ((get_type(et1) == 3) && (et1 % OFST != 0) && (get_type(et2) == 5) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            get_cmp_cst(et2, &etr, &eti);

            add_instr("%s %s\n", f2i, v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("SADD\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // comp na memoria e comp no acc
        if ((get_type(et1) == 3) && (et1 % OFST != 0) && (get_type(et2) == 3) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("FIA\n");
            add_instr("SET  aux_index_y\n");
            add_instr("FIM %s\n", v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // comp na memoria e comp na memoria
        if ((get_type(et1) == 3) && (et1 % OFST != 0) && (get_type(et2) == 3) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("%s %s\n", f2i, v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PFIM %s\n", v_name[et2 % OFST]);
            add_instr("SADD\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }
    }

    acc_ok     = 1;
    v_used[id] = 1;

    return v_type[id]*OFST;
}