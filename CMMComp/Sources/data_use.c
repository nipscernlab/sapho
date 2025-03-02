#include "..\Headers\data_use.h"
#include "..\Headers\variaveis.h"
#include "..\Headers\diretivas.h"
#include "..\Headers\t2t.h"
#include "..\Headers\data_assign.h"
#include "..\Headers\oper.h"

#include <string.h>
#include <stdlib.h>

// redeclaracao de variaveis globais
int v_isco[NVARMAX];  // se variavel eh uma constante

// reducao de constantes para exp
// nao da load, soh atualiza estados das variaveis
int num2exp(int id, int dtype)
{
    v_used[id] = 1;
    v_isco[id] = 1;
    v_isar[id] = 0;
    v_type[id] = dtype;

    return dtype*OFST+id;
}

// reducao de ID pra exp
// ainda nao da load, soh checa e atualiza estados da variavel
int id2exp(int id)
{
    // Testa se a variavel ja foi declarada
    if (v_type[id] == 0)
        fprintf (stderr, "Erro na linha %d: mané, declara a variável %s direito!\n", line_num+1, rem_fname(v_name[id], fname));

    // Testa se a variavel ja recebeu um valor
    if (v_asgn[id] == 0)
        fprintf (stdout, "Atenção na linha %d: como você quer usar %s se você nem deu um valor ainda?\n", line_num+1, rem_fname(v_name[id], fname));

    // Se for um array, esqueceram o indice
    if (v_isar[id] > 0)
        fprintf (stderr, "Erro na linha %d: cadê o índice de array da variável %s?\n", line_num+1, rem_fname(v_name[id], fname));

    v_used[id] = 1;

    return v_type[id]*OFST+id;
}

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

    // prepara os comandos de LOAD --------------------------------------------

    // prepara o tipo de load do array (ivertido ou nao)
    char ldv[10];
    if (fft    == 1) strcpy(ldv,"ILDI"); else strcpy(ldv,"LDI" );

    // prepara o tipo de load do indice (se necessario)
    char ldi[10];
    if (acc_ok == 1) strcpy(ldi,"PLD" ); else strcpy(ldi,"LOAD");

    // ------------------------------------------------------------------------
    // escreve as instrucoes --------------------------------------------------
    // ------------------------------------------------------------------------

    char num[32];
    int  etr, eti;
    int  type = v_type[id];

    // left int/float no proc em ponto flutuante ------------------------------

    if ((type < 3) && (prtype == 1))
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
            fprintf (stdout, "Atenção na linha %d: tá vendo que o índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float na memoria
        if ((get_type(et) == 2) && (et % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("%s %s\n", ldi, v_name[et % OFST]);
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp const na memoria
        if ((get_type(et) == 5) && (et % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            split_cmp_const(et, &etr, &eti);

            add_instr("%s %s\n", ldi, v_name[etr % OFST]);
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp no acc
        if ((get_type(et) == 3) && (et % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            add_instr("SETP aux_cmp\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp na memoria
        if ((get_type(et) == 3) && (et % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            add_instr("%s %s\n", ldi, v_name[et % OFST]);
            add_instr("%s %s\n", ldv, v_name[id]);
        }
    }

    // left int/float no proc em ponto fixo -----------------------------------

    else if ((type < 3) && (prtype == 0))
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

            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float var na memoria
        if ((get_type(et) == 2) && (v_isco[et%OFST] == 0) && (et % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array tá dando float. Vai gerar muito código pra arredondar!\n", line_num+1);

            add_instr("%s %s\n", ldi, v_name[et % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float const na memoria
        if ((get_type(et) == 2) && (v_isco[et%OFST] == 1) && (et % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array tá dando float. Vai gerar muito código pra arredondar!\n", line_num+1);

            add_instr("%s %d // %s\n", ldi, f2mf(v_name[et % OFST]), v_name[et % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp const na memoria
        if ((get_type(et) == 5) && (et % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou arredondar a parte real.\n", line_num+1);

            split_cmp_const(et, &etr, &eti);
            add_instr("%s %d // %s\n", ldi, f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp no acc
        if ((get_type(et) == 3) && (et % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou arredondar a parte real.\n", line_num+1);

            add_instr("SETP aux_cmp\n");
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp na memoria
        if ((get_type(et) == 3) && (et % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou arredondar a parte real.\n", line_num+1);

            add_instr("%s %s\n", ldi, v_name[et%OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("%s %s\n", ldv, v_name[id]);
        }
    }

    // left comp no proc em ponto flutuante -----------------------------------

    else if ((type > 2) && (prtype == 1))
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
            fprintf (stdout, "Atenção na linha %d: tá vendo que o índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("SET aux_ind_right\n");
            add_instr("%s %s\n"  , ldv, v_name[id]);
            add_instr("PLD aux_ind_right\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // float na memoria
        if ((get_type(et) == 2) && (et % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("%s %s\n"  , ldi, v_name[et % OFST]);
            add_instr("%s %s\n"  , ldv, v_name[id]);
            add_instr("PLD %s\n" ,      v_name[et % OFST]);
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // comp const na memoria
        if ((get_type(et) == 5) && (et % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            split_cmp_const(et, &etr, &eti);

            add_instr("%s %s\n"  , ldi, v_name[etr % OFST]);
            add_instr("%s %s\n"  , ldv, v_name[id]);
            add_instr("PLD %s\n" ,      v_name[etr % OFST]);
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // comp no acc
        if ((get_type(et) == 3) && (et % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            add_instr("SETP aux_ind_right\n");
            add_instr("SET  aux_ind_right\n");
            add_instr("%s %s\n"  , ldv, v_name[id]);
            add_instr("PLD aux_ind_right\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // comp na memoria
        if ((get_type(et) == 3) && (et % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            add_instr("%s %s\n"  , ldi, v_name[et % OFST]);
            add_instr("%s %s\n"  , ldv, v_name[id]);
            add_instr("PLD %s\n" ,      v_name[et % OFST]);
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }
    }

    // left comp no proc em ponto fixo ----------------------------------------

    else if ((type > 2) && (prtype == 0))
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

            add_instr("CALL float2int\n"   ); f2i = 1;
            add_instr("SET aux_ind_right\n");
            add_instr("%s %s\n"  , ldv, v_name[id]);
            add_instr("PLD aux_ind_right\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // float var na memoria
        if ((get_type(et) == 2) && (v_isco[et%OFST] == 0) && (et % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array tá dando float. Vai gerar muito código pra arredondar!\n", line_num+1);

            add_instr("%s %s\n", ldi, v_name[et % OFST]);
            add_instr("CALL float2int\n" ); f2i = 1;
            add_instr("SET aux_ind_right\n");
            add_instr("%s %s\n"  , ldv, v_name[id]);
            add_instr("PLD aux_ind_right\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // float const na memoria
        if ((get_type(et) == 2) && (v_isco[et%OFST] == 1) && (et % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array tá dando float. Vai gerar muito código pra arredondar!\n", line_num+1);

            add_instr("%s %d // %s\n", ldi, f2mf(v_name[et % OFST]), v_name[et % OFST]);
            add_instr("CALL float2int\n" ); f2i = 1;
            add_instr("SET aux_ind_right\n");
            add_instr("%s %s\n"  , ldv, v_name[id]);
            add_instr("PLD aux_ind_right\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // comp const na memoria
        if ((get_type(et) == 5) && (et % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou arredondar a parte real.\n", line_num+1);

            split_cmp_const(et, &etr, &eti);
            add_instr("%s %d // %s\n", ldi, f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SET aux_ind_right\n");
            add_instr("%s %s\n"  , ldv, v_name[id]);
            add_instr("PLD aux_ind_right\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // comp no acc
        if ((get_type(et) == 3) && (et % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou arredondar a parte real.\n", line_num+1);

            add_instr("SETP aux_cmp\n");
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SET aux_ind_right\n");
            add_instr("%s %s\n"  , ldv, v_name[id]);
            add_instr("PLD aux_ind_right\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // comp na memoria
        if ((get_type(et) == 3) && (et % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou arredondar a parte real.\n", line_num+1);

            add_instr("%s %s\n", ldi, v_name[et%OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
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
        fprintf (stderr, "Erro na linha %d: array %s tem uma dimensão só!\n", line_num+1, rem_fname(v_name[id], fname));

    // prepara os comandos de LOAD --------------------------------------------

    // prepara o tipo de load da variavel
    char ldv[10];
    strcpy(ldv,"LDI" ); // ainda nao tem fft 2d

    // prepara o tipo de load do indice (se necessario)
    char ldi[10];
    if (acc_ok == 1) strcpy(ldi,"PLD" ); else strcpy(ldi,"LOAD");

    // ------------------------------------------------------------------------
    // escreve as instrucoes --------------------------------------------------
    // ------------------------------------------------------------------------

    char num[32];
    int  etr, eti;
    int  type = v_type[id];

    // left int/float no proc em ponto flutuante ------------------------------

    if ((type < 3) && (prtype == 1))
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

            add_instr("SETP aux_index_y\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // int no acc e float na memoria
        if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 2) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o segundo índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // int no acc e comp const na memoria
        if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 5) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            split_cmp_const(et2, &etr, &eti);

            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[etr % OFST]);
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // int no acc e comp no acc
        if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 3) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            add_instr("SETP aux_index_y\n");
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
            add_instr("ADD %s\n", v_name[et2 % OFST]);
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

            add_instr("PLD %s\n", v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // int na memoria e float na memoria
        if ((get_type(et1) == 1) && (et1 % OFST != 0) && (get_type(et2) == 2) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o segundo índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("%s %s\n", ldi, v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // int na memoria e comp const na memoria
        if ((get_type(et1) == 1) && (et1 % OFST != 0) && (get_type(et2) == 5) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            split_cmp_const(et2, &etr, &eti);

            add_instr("%s %s\n", ldi, v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[etr % OFST]);
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // int na memoria e comp no acc
        if ((get_type(et1) == 1) && (et1 % OFST != 0) && (get_type(et2) == 3) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("SET  aux_index_y\n");
            add_instr("LOAD %s\n", v_name[et1 % OFST]);
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
            add_instr("ADD %s\n", v_name[et2 % OFST]);
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float no acc e int no acc
        if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 1) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o primeiro índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float no acc e int na memoria
        if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o primeiro índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float no acc e float no acc
        if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 2) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que os índices do array estão em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float no acc e comp const na memoria
        if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 5) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            split_cmp_const(et2, &etr, &eti);

            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[etr % OFST]);
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float no acc e comp no acc
        if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 3) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("SETP aux_index_y\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float no acc e comp na memoria
        if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 3) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp const na memoria e int no acc
        if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 1) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            split_cmp_const(et1, &etr, &eti);

            add_instr("SET aux_index_y\n");
            add_instr("LOAD %s\n", v_name[etr % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp const na memoria e int na memoria
        if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            split_cmp_const(et1, &etr, &eti);

            add_instr("%s %s\n", ldi, v_name[etr % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp const na memoria e float no acc
        if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 2) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            split_cmp_const(et1, &etr, &eti);

            add_instr("SET aux_index_y\n");
            add_instr("LOAD %s\n", v_name[etr % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp const na memoria e float na memoria
        if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 2) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            split_cmp_const(et1, &etr, &eti);

            add_instr("%s %s\n", ldi, v_name[etr % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp const na memoria e comp const na memoria
        if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 5) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            split_cmp_const(et1, &etr, &eti);
            add_instr("%s %s\n", ldi, v_name[etr % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);

            split_cmp_const(et2, &etr, &eti);
            add_instr("ADD %s\n", v_name[etr % OFST]);
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp const na memoria e comp no acc
        if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 3) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            split_cmp_const(et1, &etr, &eti);

            add_instr("SETP aux_index_y\n");
            add_instr("SET  aux_index_y\n");
            add_instr("LOAD %s\n", v_name[etr % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp const na memoria e comp na memoria
        if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 3) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            split_cmp_const(et1, &etr, &eti);

            add_instr("%s %s\n", ldi, v_name[etr % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp no acc e int no acc
        if ((get_type(et1) == 3) && (et1 % OFST == 0) && (get_type(et2) == 1) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("SETP aux_lixo\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp no acc e int na memoria
        if ((get_type(et1) == 3) && (et1 % OFST == 0) && (get_type(et2) == 1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp no acc e float no acc
        if ((get_type(et1) == 3) && (et1 % OFST == 0) && (get_type(et2) == 2) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("SETP aux_lixo\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp no acc e float na memoria
        if ((get_type(et1) == 3) && (et1 % OFST == 0) && (get_type(et2) == 2) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp no acc e comp const na memoria
        if ((get_type(et1) == 3) && (et1 % OFST == 0) && (get_type(et2) == 5) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            split_cmp_const(et2, &etr, &eti);

            add_instr("SETP aux_index_y\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[etr % OFST]);
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp no acc e comp no acc
        if ((get_type(et1) == 3) && (et1 % OFST == 0) && (get_type(et2) == 3) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("SETP aux_index_y\n");
            add_instr("SETP aux_lixo\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp no acc e comp na memoria
        if ((get_type(et1) == 3) && (et1 % OFST == 0) && (get_type(et2) == 3) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp na memoria e int no acc
        if ((get_type(et1) == 3) && (et1 % OFST != 0) && (get_type(et2) == 1) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            add_instr("PLD %s\n", v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp na memoria e int na memoria
        if ((get_type(et1) == 3) && (et1 % OFST != 0) && (get_type(et2) == 1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            add_instr("%s %s\n", ldi, v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp na memoria e float no acc
        if ((get_type(et1) == 3) && (et1 % OFST != 0) && (get_type(et2) == 2) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("PLD %s\n", v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp na memoria e float na memoria
        if ((get_type(et1) == 3) && (et1 % OFST != 0) && (get_type(et2) == 2) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("%s %s\n", ldi, v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp na memoria e comp const na memoria
        if ((get_type(et1) == 3) && (et1 % OFST != 0) && (get_type(et2) == 5) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            split_cmp_const(et2, &etr, &eti);

            add_instr("%s %s\n", ldi, v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[etr % OFST]);
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp na memoria e comp no acc
        if ((get_type(et1) == 3) && (et1 % OFST != 0) && (get_type(et2) == 3) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("SET  aux_index_y\n");
            add_instr("LOAD %s\n", v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp na memoria e comp na memoria
        if ((get_type(et1) == 3) && (et1 % OFST != 0) && (get_type(et2) == 3) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("%s %s\n", ldi, v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
            add_instr("%s %s\n", ldv, v_name[id]);
        }
    }

    // left int/float no proc em ponto fixo -----------------------------------

    else if ((type < 3) && (prtype == 0))
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

            add_instr("CALL float2int\n"); f2i = 1;
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
            add_instr("PLD %s\n", v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // int no acc e float const na memoria
        if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 2) && (v_isco[et2%OFST]==1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o segundo índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %d // %s\n", f2mf(v_name[et2 % OFST]), v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // int no acc e comp const na memoria
        if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 5) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            split_cmp_const(et2, &etr, &eti);

            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // int no acc e comp no acc
        if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 3) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("CALL float2int\n"); f2i = 1;
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
            add_instr("PLD %s\n", v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
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

            add_instr("CALL float2int\n"); f2i = 1;
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
            add_instr("PLD %s\n", v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // int na memoria e float const na memoria
        if ((get_type(et1) == 1) && (et1 % OFST != 0) && (get_type(et2) == 2) && (v_isco[et2%OFST]==1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o segundo índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("%s %s\n", ldi, v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %d // %s\n", f2mf(v_name[et2 % OFST]), v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // int na memoria e comp const na memoria
        if ((get_type(et1) == 1) && (et1 % OFST != 0) && (get_type(et2) == 5) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            split_cmp_const(et2, &etr, &eti);

            add_instr("%s %s\n", ldi, v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // int na memoria e comp no acc
        if ((get_type(et1) == 1) && (et1 % OFST != 0) && (get_type(et2) == 3) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SET  aux_index_y\n");
            add_instr("LOAD %s\n", v_name[et1 % OFST]);
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
            add_instr("PLD %s\n", v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float no acc e int no acc
        if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 1) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o primeiro índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float no acc e int na memoria
        if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o primeiro índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float no acc e float no acc
        if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 2) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que os índices do array estão em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SETP aux_index_y\n");
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float no acc e float var na memoria
        if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 2) && (v_isco[et2%OFST]==0) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %s\n", v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float no acc e float const na memoria
        if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 2) && (v_isco[et2%OFST]==1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %d // %s\n", f2mf(v_name[et2 % OFST]), v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float no acc e comp const na memoria
        if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 5) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            split_cmp_const(et2, &etr, &eti);

            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float no acc e comp no acc
        if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 3) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SETP aux_index_y\n");
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float no acc e comp na memoria
        if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 3) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %s\n", v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float var na memoria e int no acc
        if ((get_type(et1) == 2) && (et1 % OFST != 0) && (v_isco[et1%OFST]==0) && (get_type(et2) == 1) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o primeiro índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("SET aux_index_y\n");
            add_instr("LOAD %s\n", v_name[et1%OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float var na memoria e int na memoria
        if ((get_type(et1) == 2) && (et1 % OFST != 0) && (v_isco[et1%OFST]==0) && (get_type(et2) == 1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o primeiro índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("%s %s\n", ldi, v_name[et1 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float var na memoria e float no acc
        if ((get_type(et1) == 2) && (et1 % OFST != 0) && (v_isco[et1%OFST]==0) && (get_type(et2) == 2) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que os índices do array estão em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SET aux_index_y\n");
            add_instr("LOAD %s\n", v_name[et1 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float var na memoria e float var na memoria
        if ((get_type(et1) == 2) && (et1 % OFST != 0) && (v_isco[et1%OFST]==0) && (get_type(et2) == 2) && (v_isco[et2%OFST]==0) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("%s %s\n", ldi, v_name[et1 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %s\n", v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float var na memoria e float const na memoria
        if ((get_type(et1) == 2) && (et1 % OFST != 0) && (v_isco[et1%OFST]==0) && (get_type(et2) == 2) && (v_isco[et2%OFST]==1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("%s %s\n", ldi, v_name[et1 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %d // %s\n", f2mf(v_name[et2 % OFST]), v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float var na memoria e comp const na memoria
        if ((get_type(et1) == 2) && (et1 % OFST != 0) && (v_isco[et1%OFST]==0) && (get_type(et2) == 5) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            split_cmp_const(et2, &etr, &eti);

            add_instr("%s %s\n", ldi, v_name[et1 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float var na memoria e comp no acc
        if ((get_type(et1) == 2) && (et1 % OFST != 0) && (v_isco[et1%OFST]==0) && (get_type(et2) == 3) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SET aux_index_y\n");
            add_instr("LOAD %s \n", v_name[et1%OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float var na memoria e comp na memoria
        if ((get_type(et1) == 2) && (et1 % OFST != 0) && (v_isco[et1%OFST]==0) && (get_type(et2) == 3) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("%s %s\n", ldi, v_name[et1 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %s\n", v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float const na memoria e int no acc
        if ((get_type(et1) == 2) && (et1 % OFST != 0) && (v_isco[et1%OFST]==1) && (get_type(et2) == 1) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o primeiro índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("SET aux_index_y\n");
            add_instr("LOAD %d // %s\n", f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float const na memoria e int na memoria
        if ((get_type(et1) == 2) && (et1 % OFST != 0) && (v_isco[et1%OFST]==1) && (get_type(et2) == 1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o primeiro índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("%s %d // %s\n", ldi, f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float const na memoria e float no acc
        if ((get_type(et1) == 2) && (et1 % OFST != 0) && (v_isco[et1%OFST]==1) && (get_type(et2) == 2) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que os índices do array estão em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SET aux_index_y\n");
            add_instr("LOAD %d // %s\n", f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float const na memoria e float var na memoria
        if ((get_type(et1) == 2) && (et1 % OFST != 0) && (v_isco[et1%OFST]==1) && (get_type(et2) == 2) && (v_isco[et2%OFST]==0) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("%s %d // %s\n", ldi, f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %s\n", v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float const na memoria e float const na memoria
        if ((get_type(et1) == 2) && (et1 % OFST != 0) && (v_isco[et1%OFST]==1) && (get_type(et2) == 2) && (v_isco[et2%OFST]==1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("%s %d // %s\n", ldi, f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %d // %s\n", f2mf(v_name[et2 % OFST]), v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float const na memoria e comp const na memoria
        if ((get_type(et1) == 2) && (et1 % OFST != 0) && (v_isco[et1%OFST]==1) && (get_type(et2) == 5) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            split_cmp_const(et2, &etr, &eti);

            add_instr("%s %d // %s\n", ldi, f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float const na memoria e comp no acc
        if ((get_type(et1) == 2) && (et1 % OFST != 0) && (v_isco[et1%OFST]==1) && (get_type(et2) == 3) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SET aux_index_y\n");
            add_instr("LOAD %d // %s\n", f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // float const na memoria e comp na memoria
        if ((get_type(et1) == 2) && (et1 % OFST != 0) && (v_isco[et1%OFST]==1) && (get_type(et2) == 3) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("%s %d // %s\n", ldi, f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %s\n", v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp const na memoria e int no acc
        if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 1) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            split_cmp_const(et1, &etr, &eti);

            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SET aux_index_y\n");
            add_instr("LOAD %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp const na memoria e int na memoria
        if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            split_cmp_const(et1, &etr, &eti);

            add_instr("%s %d // %s\n", ldi, f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp const na memoria e float no acc
        if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 2) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            split_cmp_const(et1, &etr, &eti);

            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SET aux_index_y\n");
            add_instr("LOAD %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp const na memoria e float var na memoria
        if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 2) && (v_isco[et2%OFST]==0) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            split_cmp_const(et1, &etr, &eti);

            add_instr("%s %d // %s\n", ldi, f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %s\n", v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp const na memoria e float const na memoria
        if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 2) && (v_isco[et2%OFST]==1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            split_cmp_const(et1, &etr, &eti);

            add_instr("%s %d // %s\n", ldi, f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %d // %s\n", f2mf(v_name[et2 % OFST]), v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp const na memoria e comp const na memoria
        if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 5) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            split_cmp_const(et1, &etr, &eti);
            add_instr("%s %d // %s\n", ldi, f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);

            split_cmp_const(et2, &etr, &eti);
            add_instr("PLD %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp const na memoria e comp no acc
        if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 3) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            split_cmp_const(et1, &etr, &eti);

            add_instr("SETP aux_index_y\n");
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SET  aux_index_y\n");
            add_instr("LOAD %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp const na memoria e comp na memoria
        if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 3) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            split_cmp_const(et1, &etr, &eti);

            add_instr("%s %d // %s\n", ldi, f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %s\n", v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp no acc e int no acc
        if ((get_type(et1) == 3) && (et1 % OFST == 0) && (get_type(et2) == 1) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("SETP aux_lixo\n");
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp no acc e int na memoria
        if ((get_type(et1) == 3) && (et1 % OFST == 0) && (get_type(et2) == 1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp no acc e float no acc
        if ((get_type(et1) == 3) && (et1 % OFST == 0) && (get_type(et2) == 2) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SETP aux_index_y\n");
            add_instr("SETP aux_lixo\n");
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp no acc e float var na memoria
        if ((get_type(et1) == 3) && (et1 % OFST == 0) && (get_type(et2) == 2) && (v_isco[et2%OFST]==0) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %s\n", v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp no acc e float const na memoria
        if ((get_type(et1) == 3) && (et1 % OFST == 0) && (get_type(et2) == 2) && (v_isco[et2%OFST]==1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %d // %s\n", f2mf(v_name[et2 % OFST]), v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp no acc e comp const na memoria
        if ((get_type(et1) == 3) && (et1 % OFST == 0) && (get_type(et2) == 5) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            split_cmp_const(et2, &etr, &eti);

            add_instr("SETP aux_index_y\n");
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp no acc e comp no acc
        if ((get_type(et1) == 3) && (et1 % OFST == 0) && (get_type(et2) == 3) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SETP aux_index_y\n");
            add_instr("SETP aux_lixo\n");
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp no acc e comp na memoria
        if ((get_type(et1) == 3) && (et1 % OFST == 0) && (get_type(et2) == 3) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %s\n", v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp na memoria e int no acc
        if ((get_type(et1) == 3) && (et1 % OFST != 0) && (get_type(et2) == 1) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            add_instr("PLD %s\n", v_name[et1 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp na memoria e int na memoria
        if ((get_type(et1) == 3) && (et1 % OFST != 0) && (get_type(et2) == 1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            add_instr("%s %s\n", ldi, v_name[et1 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp na memoria e float no acc
        if ((get_type(et1) == 3) && (et1 % OFST != 0) && (get_type(et2) == 2) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("PLD %s\n", v_name[et1 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp na memoria e float var na memoria
        if ((get_type(et1) == 3) && (et1 % OFST != 0) && (get_type(et2) == 2) && (v_isco[et2%OFST]==0) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("%s %s\n", ldi, v_name[et1 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %s\n", v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp na memoria e float const na memoria
        if ((get_type(et1) == 3) && (et1 % OFST != 0) && (get_type(et2) == 2) && (v_isco[et2%OFST]==1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("%s %s\n", ldi, v_name[et1 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %d // %s\n", f2mf(v_name[et2 % OFST]), v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp na memoria e comp const na memoria
        if ((get_type(et1) == 3) && (et1 % OFST != 0) && (get_type(et2) == 5) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            split_cmp_const(et2, &etr, &eti);

            add_instr("%s %s\n", ldi, v_name[et1 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp na memoria e comp no acc
        if ((get_type(et1) == 3) && (et1 % OFST != 0) && (get_type(et2) == 3) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SET  aux_index_y\n");
            add_instr("LOAD %s\n", v_name[et1 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }

        // comp na memoria e comp na memoria
        if ((get_type(et1) == 3) && (et1 % OFST != 0) && (get_type(et2) == 3) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("%s %s\n", ldi, v_name[et1 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %s\n", v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SADD\n");
            add_instr("%s %s\n", ldv, v_name[id]);
        }
    }

    // left comp no proc em ponto flutuante -----------------------------------

    else if ((type > 2) && (prtype == 1))
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

            add_instr("SETP aux_index_y\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // int no acc e float na memoria
        if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 2) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o segundo índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // int no acc e comp const na memoria
        if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 5) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            split_cmp_const(et2, &etr, &eti);

            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[etr % OFST]);
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
            add_instr("ADD %s\n", v_name[et2 % OFST]);
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
            add_instr("%s %s\n", ldv, v_name[id]);
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

            add_instr("PLD %s\n", v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("SADD\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // int na memoria e float na memoria
        if ((get_type(et1) == 1) && (et1 % OFST != 0) && (get_type(et2) == 2) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o segundo índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("%s %s\n", ldi, v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // int na memoria e comp const na memoria
        if ((get_type(et1) == 1) && (et1 % OFST != 0) && (get_type(et2) == 5) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            split_cmp_const(et2, &etr, &eti);

            add_instr("%s %s\n", ldi, v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[etr % OFST]);
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
            add_instr("SET  aux_index_y\n");
            add_instr("LOAD %s\n", v_name[et1 % OFST]);
            add_instr("MLT  %s_arr_size\n", v_name[id]);
            add_instr("ADD  aux_index_y\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
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
            add_instr("ADD %s\n", v_name[et2 % OFST]);
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

            add_instr("SETP aux_index_y\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // float no acc e comp const na memoria
        if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 5) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            split_cmp_const(et2, &etr, &eti);

            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[etr % OFST]);
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
            add_instr("SETP aux_index_y\n");
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

            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // comp const na memoria e int no acc
        if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 1) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            split_cmp_const(et1, &etr, &eti);

            add_instr("SET aux_index_y\n");
            add_instr("LOAD %s\n", v_name[etr % OFST]);
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

            split_cmp_const(et1, &etr, &eti);

            add_instr("%s %s\n", ldi, v_name[etr % OFST]);
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

            split_cmp_const(et1, &etr, &eti);

            add_instr("SET aux_index_y\n");
            add_instr("LOAD %s\n", v_name[etr % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // comp const na memoria e float na memoria
        if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 2) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            split_cmp_const(et1, &etr, &eti);

            add_instr("%s %s\n", ldi, v_name[etr % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // comp const na memoria e comp const na memoria
        if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 5) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            split_cmp_const(et1, &etr, &eti);
            add_instr("%s %s\n", ldi, v_name[etr % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);

            split_cmp_const(et2, &etr, &eti);
            add_instr("ADD %s\n", v_name[etr % OFST]);
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // comp const na memoria e comp no acc
        if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 3) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            split_cmp_const(et1, &etr, &eti);

            add_instr("SETP aux_index_y\n");
            add_instr("SET  aux_index_y\n");
            add_instr("LOAD %s\n", v_name[etr % OFST]);
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

            split_cmp_const(et1, &etr, &eti);

            add_instr("%s %s\n", ldi, v_name[etr % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
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
            add_instr("SETP aux_lixo\n");
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

            add_instr("SETP aux_index_y\n");
            add_instr("SETP aux_lixo\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // comp no acc e float na memoria
        if ((get_type(et1) == 3) && (et1 % OFST == 0) && (get_type(et2) == 2) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // comp no acc e comp const na memoria
        if ((get_type(et1) == 3) && (et1 % OFST == 0) && (get_type(et2) == 5) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            split_cmp_const(et2, &etr, &eti);

            add_instr("SETP aux_index_y\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[etr % OFST]);
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
            add_instr("SETP aux_index_y\n");
            add_instr("SETP aux_lixo\n");
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
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // comp na memoria e int no acc
        if ((get_type(et1) == 3) && (et1 % OFST != 0) && (get_type(et2) == 1) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            add_instr("PLD %s\n", v_name[et1 % OFST]);
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

            add_instr("%s %s\n", ldi, v_name[et1 % OFST]);
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

            add_instr("PLD %s\n", v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("SADD\n");
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // comp na memoria e float na memoria
        if ((get_type(et1) == 3) && (et1 % OFST != 0) && (get_type(et2) == 2) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("%s %s\n", ldi, v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }

        // comp na memoria e comp const na memoria
        if ((get_type(et1) == 3) && (et1 % OFST != 0) && (get_type(et2) == 5) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            split_cmp_const(et2, &etr, &eti);

            add_instr("%s %s\n", ldi, v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[etr % OFST]);
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
            add_instr("SET  aux_index_y\n");
            add_instr("LOAD %s\n", v_name[et1 % OFST]);
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

            add_instr("%s %s\n", ldi, v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
            add_instr("SET aux_index_y\n");
            add_instr("%s %s\n", ldv, v_name[id]);
            add_instr("PLD aux_index_y\n");
            add_instr("%s %s_i\n", ldv, v_name[id]);
        }
    }

    // left comp no proc em ponto fixo ----------------------------------------

    else if ((type > 2) && (prtype == 0))
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

            add_instr("CALL float2int\n"); f2i = 1;
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
            add_instr("PLD %s\n", v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
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
            add_instr("PLD %d // %s\n", f2mf(v_name[et2 % OFST]), v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
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

            split_cmp_const(et2, &etr, &eti);

            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
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
            add_instr("CALL float2int\n"); f2i = 1;
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
            add_instr("PLD %s\n", v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
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

            add_instr("CALL float2int\n"); f2i = 1;
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
            add_instr("PLD %s\n", v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
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
            add_instr("PLD %d // %s\n", f2mf(v_name[et2 % OFST]), v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
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

            split_cmp_const(et2, &etr, &eti);

            add_instr("%s %s\n", ldi, v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
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
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SET  aux_index_y\n");
            add_instr("LOAD %s\n", v_name[et1 % OFST]);
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
            add_instr("PLD %s\n", v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
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
            add_instr("CALL float2int\n"); f2i = 1;
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

            add_instr("CALL float2int\n"); f2i = 1;
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

            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SETP aux_index_y\n");
            add_instr("CALL float2int\n"); f2i = 1;
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

            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %s\n", v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
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

            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %d // %s\n", f2mf(v_name[et2 % OFST]), v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
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

            split_cmp_const(et2, &etr, &eti);

            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
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
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SETP aux_index_y\n");
            add_instr("CALL float2int\n"); f2i = 1;
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

            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %s\n", v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
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

            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SET aux_index_y\n");
            add_instr("LOAD %s\n", v_name[et1 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
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

            add_instr("%s %s\n", ldi, v_name[et1 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
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

            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SET aux_index_y\n");
            add_instr("LOAD %s\n", v_name[et1 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
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

            add_instr("%s %s\n", ldi, v_name[et1 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %s\n", v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
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

            add_instr("%s %s\n", ldi, v_name[et1 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %d // %s\n", f2mf(v_name[et2 % OFST]), v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
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

            add_instr("%s %s\n", ldi, v_name[et1 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);

            split_cmp_const(et2, &etr, &eti);

            add_instr("PLD %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
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
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SET  aux_index_y\n");
            add_instr("LOAD %s\n", v_name[et1 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
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

            add_instr("%s %s\n", ldi, v_name[et1 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %s\n", v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
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

            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SET aux_index_y\n");
            add_instr("LOAD %d // %s\n", f2mf(v_name[et1 % OFST]), v_name[et1 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
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

            add_instr("%s %d // %s\n", ldi, f2mf(v_name[et1 % OFST]), v_name[et1 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
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

            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SET aux_index_y\n");
            add_instr("LOAD %d // %s\n", f2mf(v_name[et1 % OFST]), v_name[et1 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
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

            add_instr("%s %d // %s\n", ldi, f2mf(v_name[et1 % OFST]), v_name[et1 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %s\n", v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
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

            add_instr("%s %d // %s\n", ldi, f2mf(v_name[et1 % OFST]), v_name[et1 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %d // %s\n", f2mf(v_name[et2 % OFST]), v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
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

            add_instr("%s %d // %s\n", ldi, f2mf(v_name[et1 % OFST]), v_name[et1 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);

            split_cmp_const(et2, &etr, &eti);

            add_instr("PLD %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
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
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SET  aux_index_y\n");
            add_instr("LOAD %d // %s\n", f2mf(v_name[et1 % OFST]), v_name[et1 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
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

            add_instr("%s %d // %s\n", ldi, f2mf(v_name[et1 % OFST]), v_name[et1 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %s\n", v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
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

            split_cmp_const(et1, &etr, &eti);

            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SET aux_index_y\n");
            add_instr("LOAD %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
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

            split_cmp_const(et1, &etr, &eti);

            add_instr("%s %d // %s\n", ldi, f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
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

            split_cmp_const(et1, &etr, &eti);

            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SET aux_index_y\n");
            add_instr("LOAD %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
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

            split_cmp_const(et1, &etr, &eti);

            add_instr("%s %d // %s\n", ldi, f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %s\n", v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
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

            split_cmp_const(et1, &etr, &eti);

            add_instr("%s %d // %s\n", ldi, f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %d // %s\n", f2mf(v_name[et2 % OFST]), v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
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

            split_cmp_const(et1, &etr, &eti);
            add_instr("%s %d // %s\n", ldi, f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);

            split_cmp_const(et2, &etr, &eti);
            add_instr("PLD %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
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

            split_cmp_const(et1, &etr, &eti);

            add_instr("SETP aux_index_y\n");
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SET  aux_index_y\n");
            add_instr("LOAD %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
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

            split_cmp_const(et1, &etr, &eti);

            add_instr("%s %d // %s\n", ldi, f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %s\n", v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
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
            add_instr("SETP aux_lixo\n");
            add_instr("CALL float2int\n"); f2i = 1;
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
            add_instr("CALL float2int\n"); f2i = 1;
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

            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SETP aux_index_y\n");
            add_instr("SETP aux_lixo\n");
            add_instr("CALL float2int\n"); f2i = 1;
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
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %s\n", v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
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
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %d // %s\n", f2mf(v_name[et2 % OFST]), v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
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

            split_cmp_const(et2, &etr, &eti);

            add_instr("SETP aux_index_y\n");
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
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
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SETP aux_index_y\n");
            add_instr("SETP aux_lixo\n");
            add_instr("CALL float2int\n"); f2i = 1;
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
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %s\n", v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
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

            add_instr("PLD %s\n", v_name[et1 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
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

            add_instr("%s %s\n", ldi, v_name[et1 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
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

            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("PLD %s\n", v_name[et1 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
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

            add_instr("%s %s\n", ldi, v_name[et1 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %s\n", v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
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

            add_instr("%s %s\n", ldi, v_name[et1 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %d // %s\n", f2mf(v_name[et2 % OFST]), v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
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

            split_cmp_const(et2, &etr, &eti);

            add_instr("%s %s\n", ldi, v_name[et1 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
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
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SET  aux_index_y\n");
            add_instr("LOAD %s\n", v_name[et1 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
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

            add_instr("%s %s\n", ldi, v_name[et1 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %s\n", v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
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

// reducao de ++ pra exp
int exp_pplus(int id)
{
    if (v_type[id] > 2)
        fprintf (stderr, "Erro na linha %d: o que você bebeu pra querer incrementar um número complexo?\n", line_num+1);

    // equivalente a pegar o x na expressao (x+1)
    int et = id2exp(id);

    // agora transforma o 1 em um exp
    // primeiro faz o lexer do 1
    if (find_var("1") == -1) add_var("1");
    int lval = find_var("1");
    // pega se deve vir de INUM ou FNUM
    int type = get_type(et);
    // depois o parser
    int et1 = num2exp(lval,type);
    // depois faz operacao de soma
    int ret = oper_soma(et,et1);
    // por ultimo, atribui de volta pra id
    var_set(id, ret);

    acc_ok = 1; //nao pode liberar o acc, pois eh um exp

    return ret;
}

// reducao de ++ pra exp em array 1D
int array_pplus(int id, int ete)
{
    if (v_type[id] > 2)
        fprintf (stderr, "Erro na linha %d: o que você bebeu pra querer incrementar um número complexo?\n", line_num+1);

    // equivalente a pegar o x na expressao (x+1)
    int et = array1d2exp(id,ete,0);
    // agora transforma o 1 em um exp
    // primeiro faz o lexer do 1
    if (find_var("1") == -1) add_var("1");
    int lval = find_var("1");
    // pega se deve vir de INUM ou FNUM
    int type = get_type(et);
    // depois o parser
    int et1 = num2exp(lval,type);
    // depois faz operacao de soma
    int ret = oper_soma(et,et1);
    // faz o load no indice do array novamente
    get_1d_index(id, ete);
    // por ultimo, atribui de volta pra id
    array_set(id, ret, 0);

    acc_ok = 1; //nao pode liberar o acc, pois eh um exp

    return ret;
}

// reducao de ++ pra exp em array 2D
int array_2plus(int id, int et1, int et2)
{
    if (v_type[id] > 2)
        fprintf (stderr, "Erro na linha %d: o que você bebeu pra querer incrementar um número complexo?\n", line_num+1);

    // equivalente a pegar o x na expressao (x+1)
    int et = array2d2exp(id,et1,et2);
    // agora transforma o 1 em um exp
    // primeiro faz o lexer do 1
    if (find_var("1") == -1) add_var("1");
    int lval = find_var("1");
    // pega se deve vir de INUM ou FNUM
    int type = get_type(et);
    // depois o parser
    int etx = num2exp(lval,type);
    // depois faz operacao de soma
    int ret = oper_soma(et,etx);
    // faz o load no indice do array novamente
    get_2d_index(id, et1, et2);
    // por ultimo, atribui de volta pra id
    array_set(id, ret, 0);

    acc_ok = 1; //nao pode liberar o acc, pois eh um exp

    return ret;
}
