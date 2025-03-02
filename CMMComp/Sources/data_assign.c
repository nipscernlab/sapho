#include "..\Headers\data_assign.h"
#include "..\Headers\data_use.h"
#include "..\Headers\variaveis.h"
#include "..\Headers\diretivas.h"
#include "..\Headers\t2t.h"
#include "..\Headers\stdlib.h"

#include <string.h>
#include <stdio.h>
#include <math.h>

// redeclaracao de variaveis globais
FILE *f_asm;               // arquivo de saida
int  a_cnt        = 0;     // contador de variaveis complexas
int  acc_ok       = 0;     // 0 -> acc vazio (use LOAD)  , 1 -> acc carregado (use PLD)
int  i2f = 0, f2i = 0;     // se vai precisar de macros de ponto flutuante
int  prtype       = 0;     // 0 -> processador fonto fixo, 1 -> processador ponto flutuante
int  line_num     = 0;     // numero da linha sendo parseada
char fname [512];          // nome da funcao atual sendo parseada
int  v_asgn[NVARMAX];      // se variavel ja recebeu algum valor
int  v_isar[NVARMAX];      // se variavel eh um array
int  v_type[NVARMAX];      // 0 -> nao identificada, 1 -> int, 2 -> float
char v_name[NVARMAX][512]; // nome da variavel ou funcao

// ----------------------------------------------------------------------------
// Funcoes auxiliares ---------------------------------------------------------
// ----------------------------------------------------------------------------

// pega o tipo da variavel
int get_type(int et)
{
    int t;

         if (et <   OFST) t = 0; // indefinido
    else if (et < 2*OFST) t = 1; // int
    else if (et < 3*OFST) t = 2; // float
    else if (et < 4*OFST) t = 3; // comp real
    else if (et < 5*OFST) t = 4; // comp img
    else if (et < 6*OFST) t = 5; // comp const (ex: 3+7.5i)
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

// separa a parte real e imaginaria de uma constante complexa
// gerando duas entradas na tabela pra ponto flutuante
void get_cmp_cst(int et, int *et_r, int *et_i)
{
    char  txt[64];
    float real, img;

    sscanf(v_name[et % OFST],"%f %f",&real,&img);

    sprintf(txt,"%f",real);
    *et_r = 2*OFST + exec_num(txt);

    sprintf(txt,"%f",img);
    *et_i = 2*OFST + exec_num(txt);
}

// gera ID estendido float pra parte real e imaginaria
// de um num complexo na memoria
void get_cmp_ets(int et, int *et_r, int *et_i)
{
    *et_r = 2*OFST + (et % OFST);
    *et_i = 2*OFST + get_img_id(et % OFST);
}

// coloca o indice do array no acumulador
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

    if (prtype == 0)
    {
        // se int na memoria --------------------------------------------------

        if ((get_type(et) == 1) && (et % OFST != 0))
        {
            add_instr("LOAD %s\n", v_name[et % OFST]);
        }

        // se int no acc ------------------------------------------------------

        if ((get_type(et) == 1) && (et % OFST == 0))
        {
            
        }

        // se float var na memoria --------------------------------------------

        if ((get_type(et) == 2) && (v_isco[et%OFST] == 0) && (et % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("LOAD %s\n", v_name[et % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
        }

        // se float const na memoria ------------------------------------------

        if ((get_type(et) == 2) && (v_isco[et%OFST] == 1) && (et % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("LOAD %u // %s\n", f2mf(v_name[et % OFST]), v_name[et % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
        }

        // se float no acc ----------------------------------------------------

        if ((get_type(et) == 2) && (et % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("CALL float2int\n"); f2i = 1;
        }

        // se comp const na memoria -------------------------------------------

        if (get_type(et) == 5)
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou arredondar a parte real.\n", line_num+1);

            get_cmp_cst(et, &etr, &eti);

            add_instr("LOAD %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
        }

        // se comp no acc -----------------------------------------------------

        if ((get_type(et) == 3) && (et % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou arredondar a parte real.\n", line_num+1);

            add_instr("SETP lixo\n");
            add_instr("CALL float2int\n"); f2i = 1;
        }

        // se comp na memoria -------------------------------------------------

        if ((get_type(et) == 3) && (et % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou arredondar a parte real.\n", line_num+1);

            get_cmp_ets(et,&etr,&eti);

            add_instr("LOAD %s\n", v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
        }
    }
    else
    {
        // se int na memoria --------------------------------------------------

        if ((get_type(et) == 1) && (et % OFST != 0))
        {
            add_instr("LOAD %s\n", v_name[et % OFST]);
        }

        // se int no acc ------------------------------------------------------

        if ((get_type(et) == 1) && (et % OFST == 0))
        {
            
        }

        // se float na memoria ------------------------------------------------

        if ((get_type(et) == 2) && (et % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("LOAD %s\n", v_name[et % OFST]);
        }

        // se float no acc ----------------------------------------------------

        if ((get_type(et) == 2) && (et % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);
        }

        // se comp const na memoria -------------------------------------------

        if (get_type(et) == 5)
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou arredondar a parte real.\n", line_num+1);

            get_cmp_cst(et, &etr, &eti);

            add_instr("LOAD %s\n", v_name[etr % OFST]);
        }

        // se comp no acc -----------------------------------------------------

        if ((get_type(et) == 3) && (et % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou arredondar a parte real.\n", line_num+1);

            add_instr("SETP lixo\n");
        }

        // se comp na memoria -------------------------------------------------

        if ((get_type(et) == 3) && (et % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou arredondar a parte real.\n", line_num+1);

            get_cmp_ets(et,&etr,&eti);

            add_instr("LOAD %s\n", v_name[etr % OFST]);
        }
    }

    acc_ok = 1; // acc carregado
}

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

    if (prtype == 0)
    {
        // int no acc e int no acc
        if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 1) && (et2 % OFST == 0))
        {
            add_instr("SETP aux_index_y\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
        }

        // int no acc e int na memoria
        if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 1) && (et2 % OFST != 0))
        {
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
        }

        // int no acc e float no acc
        if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 2) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o segundo índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SETP aux_index_y\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
        }

        // int no acc e float const na memoria
        if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 2) && (v_isco[et2%OFST] == 1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o segundo índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %d // %s\n", f2mf(v_name[et2 % OFST]), v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SADD\n");
        }

        // int no acc e float var na memoria
        if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 2) && (v_isco[et2%OFST] == 0) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o segundo índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %s\n", v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SADD\n");
        }

        // int no acc e comp const na memoria
        if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 5) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            get_cmp_cst(et2, &etr, &eti);

            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SADD\n");
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
        }

        // int no acc e comp na memoria
        if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 3) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %s\n", v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SADD\n");
        }

        // int na memoria e int no acc
        if ((get_type(et1) == 1) && (et1 % OFST != 0) && (get_type(et2) == 1) && (et2 % OFST == 0))
        {
            add_instr("PLD %s\n", v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("SADD\n");
        }

        // int na memoria e int na memoria
        if ((get_type(et1) == 1) && (et1 % OFST != 0) && (get_type(et2) == 1) && (et2 % OFST != 0))
        {
            add_instr("LOAD %s\n", v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
        }

        // int na memoria e float no acc
        if ((get_type(et1) == 1) && (et1 % OFST != 0) && (get_type(et2) == 2) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o segundo índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("PLD %s\n", v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("SADD\n");
        }

        // int na memoria e float var na memoria
        if ((get_type(et1) == 1) && (et1 % OFST != 0) && (get_type(et2) == 2) && (v_isco[et2%OFST] == 0) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o segundo índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("LOAD %s\n", v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %s\n", v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SADD\n");
        }

        // int na memoria e float const na memoria
        if ((get_type(et1) == 1) && (et1 % OFST != 0) && (get_type(et2) == 2) && (v_isco[et2%OFST] == 1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o segundo índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("LOAD %s\n", v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %d // %s\n", f2mf(v_name[et2 % OFST]), v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SADD\n");
        }

        // int na memoria e comp const na memoria
        if ((get_type(et1) == 1) && (et1 % OFST != 0) && (get_type(et2) == 5) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            get_cmp_cst(et2, &etr, &eti);

            add_instr("LOAD %s\n", v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SADD\n");
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
        }

        // int na memoria e comp na memoria
        if ((get_type(et1) == 1) && (et1 % OFST != 0) && (get_type(et2) == 3) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            add_instr("LOAD %s\n", v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %s\n", v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SADD\n");
        }

        // float no acc e int no acc
        if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 1) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o primeiro índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
        }

        // float no acc e int na memoria
        if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o primeiro índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
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
        }

        // float no acc e float const na memoria
        if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 2) && (v_isco[et2%OFST] == 1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %d // %s\n", f2mf(v_name[et2 % OFST]), v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SADD\n");
        }

        // float no acc e float var na memoria
        if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 2) && (v_isco[et2%OFST] == 0) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %s\n", v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SADD\n");
        }

        // float no acc e comp const na memoria
        if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 5) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            get_cmp_cst(et2, &etr, &eti);

            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SADD\n");
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
        }

        // comp const na memoria e int no acc
        if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 1) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            get_cmp_cst(et1, &etr, &eti);

            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SET aux_index_y\n");
            add_instr("LOAD %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
        }

        // comp const na memoria e int na memoria
        if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            get_cmp_cst(et1, &etr, &eti);

            add_instr("LOAD %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
        }

        // comp const na memoria e float no acc
        if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 2) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            get_cmp_cst(et1, &etr, &eti);

            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SET aux_index_y\n");
            add_instr("LOAD %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
        }

        // comp const na memoria e float var na memoria
        if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 2) && (v_isco[et2%OFST] == 0) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            get_cmp_cst(et1, &etr, &eti);

            add_instr("LOAD %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %s\n", v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SADD\n");
        }

        // comp const na memoria e float const na memoria
        if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 2) && (v_isco[et2%OFST] == 1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            get_cmp_cst(et1, &etr, &eti);

            add_instr("LOAD %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %d // %s\n", f2mf(v_name[et2 % OFST]), v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SADD\n");
        }

        // comp const na memoria e comp const na memoria
        if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 5) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            get_cmp_cst(et1, &etr, &eti);
            add_instr("LOAD %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);

            get_cmp_cst(et2, &etr, &eti);
            add_instr("PLD %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SADD\n");
        }

        // comp const na memoria e comp no acc
        if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 3) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            get_cmp_cst(et1, &etr, &eti);

            add_instr("SETP aux_index_y\n");
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SET  aux_index_y\n");
            add_instr("LOAD %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
        }

        // comp const na memoria e comp na memoria
        if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 3) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            get_cmp_cst(et1, &etr, &eti);

            add_instr("LOAD %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %s\n", v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SADD\n");
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
        }

        // comp no acc e int na memoria
        if ((get_type(et1) == 3) && (et1 % OFST == 0) && (get_type(et2) == 1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
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
        }

        // comp no acc e float var na memoria
        if ((get_type(et1) == 3) && (et1 % OFST == 0) && (get_type(et2) == 2) && (v_isco[et2%OFST] == 0) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %s\n", v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SADD\n");
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
        }

        // comp no acc e comp const na memoria
        if ((get_type(et1) == 3) && (et1 % OFST == 0) && (get_type(et2) == 5) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            get_cmp_cst(et2, &etr, &eti);

            add_instr("SETP aux_index_y\n");
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SADD\n");
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
        }

        // comp na memoria e int no acc
        if ((get_type(et1) == 3) && (et1 % OFST != 0) && (get_type(et2) == 1) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            add_instr("PLD %s\n", v_name[et1 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("SADD\n");
        }

        // comp na memoria e int na memoria
        if ((get_type(et1) == 3) && (et1 % OFST != 0) && (get_type(et2) == 1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            add_instr("LOAD %s\n", v_name[et1 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
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
        }

        // comp na memoria e float var na memoria
        if ((get_type(et1) == 3) && (et1 % OFST != 0) && (get_type(et2) == 2) && (v_isco[et2%OFST] == 0) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("LOAD %s\n", v_name[et1 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %s\n", v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SADD\n");
        }

        // comp na memoria e float const na memoria
        if ((get_type(et1) == 3) && (et1 % OFST != 0) && (get_type(et2) == 2) && (v_isco[et2%OFST] == 1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("LOAD %s\n", v_name[et1 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %d // %s\n", f2mf(v_name[et2 % OFST]), v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SADD\n");
        }

        // comp na memoria e comp const na memoria
        if ((get_type(et1) == 3) && (et1 % OFST != 0) && (get_type(et2) == 5) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            get_cmp_cst(et2, &etr, &eti);

            add_instr("LOAD %s\n", v_name[et1 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %d // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SADD\n");
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
        }

        // comp na memoria e comp na memoria
        if ((get_type(et1) == 3) && (et1 % OFST != 0) && (get_type(et2) == 3) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("LOAD %s\n", v_name[et1 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %s\n", v_name[et2 % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SADD\n");
        }
    }
    else
    {
        // int no acc e int no acc
        if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 1) && (et2 % OFST == 0))
        {
            add_instr("SETP aux_index_y\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
        }

        // int no acc e int na memoria
        if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 1) && (et2 % OFST != 0))
        {
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
        }

        // int no acc e float no acc
        if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 2) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o segundo índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
        }

        // int no acc e float na memoria
        if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 2) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o segundo índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %s\n", v_name[et2 % OFST]);
            add_instr("SADD\n");
        }

        // int no acc e comp const na memoria
        if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 5) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            get_cmp_cst(et2, &etr, &eti);

            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %s\n", v_name[etr % OFST]);
            add_instr("SADD\n");
        }

        // int no acc e comp no acc
        if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 3) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("SETP aux_index_y\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
        }

        // int no acc e comp na memoria
        if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 3) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %s\n", v_name[et2 % OFST]);
            add_instr("SADD\n");
        }

        // int na memoria e int no acc
        if ((get_type(et1) == 1) && (et1 % OFST != 0) && (get_type(et2) == 1) && (et2 % OFST == 0))
        {
            add_instr("PLD %s\n", v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("SADD\n");
        }

        // int na memoria e int na memoria
        if ((get_type(et1) == 1) && (et1 % OFST != 0) && (get_type(et2) == 1) && (et2 % OFST != 0))
        {
            add_instr("LOAD %s\n", v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
        }

        // int na memoria e float no acc
        if ((get_type(et1) == 1) && (et1 % OFST != 0) && (get_type(et2) == 2) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o segundo índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("PLD %s\n", v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("SADD\n");
        }

        // int na memoria e float na memoria
        if ((get_type(et1) == 1) && (et1 % OFST != 0) && (get_type(et2) == 2) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o segundo índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("LOAD %s\n", v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %s\n", v_name[et2 % OFST]);
            add_instr("SADD\n");
        }

        // int na memoria e comp const na memoria
        if ((get_type(et1) == 1) && (et1 % OFST != 0) && (get_type(et2) == 5) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            get_cmp_cst(et2, &etr, &eti);

            add_instr("LOAD %s\n", v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %s\n", v_name[etr % OFST]);
            add_instr("SADD\n");
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
        }

        // int na memoria e comp na memoria
        if ((get_type(et1) == 1) && (et1 % OFST != 0) && (get_type(et2) == 3) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            add_instr("LOAD %s\n", v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %s\n", v_name[et2 % OFST]);
            add_instr("SADD\n");
        }

        // float no acc e int no acc
        if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 1) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o primeiro índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
        }

        // float no acc e int na memoria
        if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o primeiro índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
        }

        // float no acc e float no acc
        if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 2) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que os índices do array estão em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
        }

        // float no acc e comp const na memoria
        if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 5) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            get_cmp_cst(et2, &etr, &eti);

            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %s\n", v_name[etr % OFST]);
            add_instr("SADD\n");
        }

        // float no acc e comp no acc
        if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 3) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("SETP aux_index_y\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
        }

        // float no acc e comp na memoria
        if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 3) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %s\n", v_name[et2 % OFST]);
            add_instr("SADD\n");
        }

        // comp const na memoria e int no acc
        if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 1) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            get_cmp_cst(et1, &etr, &eti);

            add_instr("SET aux_index_y\n");
            add_instr("LOAD %s\n", v_name[etr % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
        }

        // comp const na memoria e int na memoria
        if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            get_cmp_cst(et1, &etr, &eti);

            add_instr("LOAD %s\n", v_name[etr % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
        }

        // comp const na memoria e float no acc
        if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 2) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            get_cmp_cst(et1, &etr, &eti);

            add_instr("SET aux_index_y\n");
            add_instr("LOAD %s\n", v_name[etr % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
        }

        // comp const na memoria e float na memoria
        if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 2) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            get_cmp_cst(et1, &etr, &eti);

            add_instr("LOAD %s\n", v_name[etr % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %s\n", v_name[et2 % OFST]);
            add_instr("SADD\n");
        }

        // comp const na memoria e comp const na memoria
        if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 5) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            get_cmp_cst(et1, &etr, &eti);
            add_instr("LOAD %s\n", v_name[etr % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);

            get_cmp_cst(et2, &etr, &eti);
            add_instr("PLD %s\n", v_name[etr % OFST]);
            add_instr("SADD\n");
        }

        // comp const na memoria e comp no acc
        if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 3) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            get_cmp_cst(et1, &etr, &eti);

            add_instr("SETP aux_index_y\n");
            add_instr("SET  aux_index_y\n");
            add_instr("LOAD %s\n", v_name[etr % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
        }

        // comp const na memoria e comp na memoria
        if ((get_type(et1) == 5) && (et1 % OFST != 0) && (get_type(et2) == 3) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            get_cmp_cst(et1, &etr, &eti);

            add_instr("LOAD %s\n", v_name[etr % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %s\n", v_name[et2 % OFST]);
            add_instr("SADD\n");
        }

        // comp no acc e int no acc
        if ((get_type(et1) == 3) && (et1 % OFST == 0) && (get_type(et2) == 1) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("SETP aux_lixo\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
        }

        // comp no acc e int na memoria
        if ((get_type(et1) == 3) && (et1 % OFST == 0) && (get_type(et2) == 1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
        }

        // comp no acc e float no acc
        if ((get_type(et1) == 3) && (et1 % OFST == 0) && (get_type(et2) == 2) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("SETP aux_lixo\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD aux_index_y\n");
        }

        // comp no acc e float na memoria
        if ((get_type(et1) == 3) && (et1 % OFST == 0) && (get_type(et2) == 2) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %s\n", v_name[et2 % OFST]);
            add_instr("SADD\n");
        }

        // comp no acc e comp const na memoria
        if ((get_type(et1) == 3) && (et1 % OFST == 0) && (get_type(et2) == 5) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            get_cmp_cst(et2, &etr, &eti);

            add_instr("SETP aux_index_y\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %s\n", v_name[etr % OFST]);
            add_instr("SADD\n");
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
        }

        // comp no acc e comp na memoria
        if ((get_type(et1) == 3) && (et1 % OFST == 0) && (get_type(et2) == 3) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("SETP aux_index_y\n");
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %s\n", v_name[et2 % OFST]);
            add_instr("SADD\n");
        }

        // comp na memoria e int no acc
        if ((get_type(et1) == 3) && (et1 % OFST != 0) && (get_type(et2) == 1) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            add_instr("PLD %s\n", v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("SADD\n");
        }

        // comp na memoria e int na memoria
        if ((get_type(et1) == 3) && (et1 % OFST != 0) && (get_type(et2) == 1) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: índice de array complexo? Sério?! Vou pegar a parte real.\n", line_num+1);

            add_instr("LOAD %s\n", v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("ADD %s\n", v_name[et2 % OFST]);
        }

        // comp na memoria e float no acc
        if ((get_type(et1) == 3) && (et1 % OFST != 0) && (get_type(et2) == 2) && (et2 % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("PLD %s\n", v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("SADD\n");
        }

        // comp na memoria e float na memoria
        if ((get_type(et1) == 3) && (et1 % OFST != 0) && (get_type(et2) == 2) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("LOAD %s\n", v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %s\n", v_name[et2 % OFST]);
            add_instr("SADD\n");
        }

        // comp na memoria e comp const na memoria
        if ((get_type(et1) == 3) && (et1 % OFST != 0) && (get_type(et2) == 5) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            get_cmp_cst(et2, &etr, &eti);

            add_instr("LOAD %s\n", v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %s\n", v_name[etr % OFST]);
            add_instr("SADD\n");
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
        }

        // comp na memoria e comp na memoria
        if ((get_type(et1) == 3) && (et1 % OFST != 0) && (get_type(et2) == 3) && (et2 % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: Esses índices do array estão uma bagunça. você é uma pessoa confusa!\n", line_num+1);

            add_instr("LOAD %s\n", v_name[et1 % OFST]);
            add_instr("MLT %s_arr_size\n", v_name[id]);
            add_instr("PLD %s\n", v_name[et2 % OFST]);
            add_instr("SADD\n");
        }
    }

    acc_ok = 1; // acc carregado
}

// ----------------------------------------------------------------------------
// funcoes de assign ----------------------------------------------------------
// ----------------------------------------------------------------------------

void var_set(int id, int et)
{
    // testa se ja foi declarada pra poder dar uma atribuicao
    if (v_type[id] == 0)
    {
        fprintf (stderr, "Erro na linha %d: se você declarar a variável %s eu agradeço.\n", line_num+1, rem_fname(v_name[id], fname));
        return;
    }

    // testa se eh um array que esqueceram o indice
    if (v_isar[id] > 0)
    {
        fprintf (stderr, "Erro na linha %d: %s é um array. Cadê o índice?\n", line_num+1, rem_fname(v_name[id], fname));
        return;
    }

    // ------------------------------------------------------------------------
    // executa o assign -------------------------------------------------------
    // ------------------------------------------------------------------------

    int etr, eti;
    char ld [10];

    // se for assign em array, tem q empurrar o indice pra pilha
    if (acc_ok == 0) strcpy(ld, "LOAD"); else strcpy(ld, "PLD");

    if (prtype == 0)
    {
        // left int e right int na memoria ------------------------------------

        if ((v_type[id] == 1) && (get_type(et) == 1) && (et % OFST != 0))
        {
            add_instr("%s %s\n", ld, v_name[et % OFST]);
            add_instr("SET %s\n",  v_name[id]);
        }

        // left int e right int no acc ----------------------------------------

        if ((v_type[id] == 1) && (get_type(et) == 1) && (et % OFST == 0))
        {
            add_instr("SET %s\n",  v_name[id]);
        }

        // left int e right float var na memoria ------------------------------

        if ((v_type[id] == 1) && (get_type(et) == 2) && (v_isco[et % OFST] == 0) && (et % OFST != 0))
        {
            fprintf(stdout, "Atenção na linha %d: variável %s é int, mas recebe float.\n", line_num+1, rem_fname(v_name[id], fname));

            add_instr("%s %s\n", ld, v_name[et % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SET %s\n",  v_name[id]);
        }

        // left int e right float const na memoria ----------------------------

        if ((v_type[id] == 1) && (get_type(et) == 2) && (v_isco[et % OFST] == 1) && (et % OFST != 0))
        {
            fprintf(stdout, "Atenção na linha %d: variável %s é int, mas recebe float.\n", line_num+1, rem_fname(v_name[id], fname));

            add_instr("%s %u // %s\n", ld, f2mf(v_name[et % OFST]), v_name[et % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SET %s\n",  v_name[id]);
        }

        // left int e right float no acc --------------------------------------

        if ((v_type[id] == 1) && (get_type(et) == 2) && (et % OFST == 0))
        {
            fprintf(stdout, "Atenção na linha %d: variável %s é int, mas recebe float.\n", line_num+1, rem_fname(v_name[id], fname));
            
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SET %s\n",  v_name[id]);
        }

        // left int e right comp const ----------------------------------------

        if ((v_type[id] == 1) && (get_type(et) == 5))
        {
            fprintf (stdout, "Atenção na linha %d: nessa conversão, eu vou arredondar a parte real hein!\n", line_num+1);

            get_cmp_cst(et,&etr,&eti);
            
            add_instr("%s %u // %s\n", ld, f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SET %s\n",  v_name[id]);
        }

        // left int e right comp na memoria -----------------------------------

        if ((v_type[id] == 1) && (get_type(et) == 3) && (et % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: nessa conversão, eu vou arredondar a parte real hein!\n", line_num+1);

            get_cmp_ets(et,&etr,&eti);
            
            add_instr("%s %s\n", ld, v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SET %s\n",  v_name[id]);
        }

        // left int e right comp no acc ---------------------------------------

        if ((v_type[id] == 1) && (get_type(et) == 3) && (et % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: nessa conversão, eu vou arredondar a parte real hein!\n", line_num+1);
    
            add_instr("SETP lixo\n");
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("SET %s\n",  v_name[id]);
        }

        // left float e right int na memoria ----------------------------------

        if ((v_type[id] == 2) && (get_type(et) == 1) && (et % OFST != 0))
        {
            fprintf(stdout, "Atenção na linha %d: variável %s é float, mas recebe int.\n", line_num+1, rem_fname(v_name[id], fname));

            add_instr("%s %s\n", ld, v_name[et % OFST]);
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("SET %s\n",  v_name[id]);
        }

        // left float e right int no acc --------------------------------------

        if ((v_type[id] == 2) && (get_type(et) == 1) && (et % OFST == 0))
        {
            fprintf(stdout, "Atenção na linha %d: variável %s é float, mas recebe int.\n", line_num+1, rem_fname(v_name[id], fname));
            
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("SET %s\n",  v_name[id]);
        }

        // left float e right float const na memoria --------------------------

        if ((v_type[id] == 2) && (get_type(et) == 2) && (v_isco[et%OFST] == 1) && (et%OFST != 0))
        {
            add_instr("%s %u // %s\n", ld, f2mf(v_name[et % OFST]), v_name[et % OFST]);
            add_instr("SET %s\n" , v_name[id]);
        }

        // left float e right float var na memoria ----------------------------

        if ((v_type[id] == 2) && (get_type(et) == 2) && (v_isco[et%OFST] == 0) && (et%OFST != 0))
        {
            add_instr("%s %s\n", ld, v_name[et % OFST]);
            add_instr("SET %s\n" , v_name[id]);
        }

        // left float e right float no acc ------------------------------------

        if ((v_type[id] == 2) && (get_type(et) == 2) && (et % OFST == 0))
        {
            add_instr("SET %s\n",  v_name[id]);
        }

        // left float e right comp const --------------------------------------

        if ((v_type[id] == 2) && (get_type(et) == 5))
        {
            fprintf (stdout, "Atenção na linha %d: nessa conversão, eu vou pegar só a parte real hein!\n", line_num+1);

            get_cmp_cst(et,&etr,&eti);
            
            add_instr("%s %u // %s\n", ld, f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("SET %s\n",  v_name[id]);
        }

        // left float e right comp na memoria ---------------------------------

        if ((v_type[id] == 2) && (get_type(et) == 3) && (et % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: nessa conversão, eu vou pegar só a parte real hein!\n", line_num+1);

            get_cmp_ets(et,&etr,&eti);
            
            add_instr("%s %s\n", ld, v_name[etr % OFST]);
            add_instr("SET %s\n",  v_name[id]);
        }

        // left float e right comp no acc -------------------------------------

        if ((v_type[id] == 2) && (get_type(et) == 3) && (et % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: nessa conversão, eu vou pegar só a parte real hein!\n", line_num+1);
    
            add_instr("SETP lixo\n");
            add_instr("SET %s\n",  v_name[id]);
        }

        // left comp e right int na memoria -----------------------------------

        if ((v_type[id] == 3) && (get_type(et) == 1) && (et % OFST != 0))
        {
            fprintf(stdout, "Atenção na linha %d: variável %s é comp, mas recebe int.\n", line_num+1, rem_fname(v_name[id], fname));

            add_instr("%s %s\n", ld, v_name[et % OFST]);
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("SET %s\n",  v_name[id]);

            add_instr("LOAD float_zero\n");
            add_instr("SET %s_i\n",  v_name[id]);
        }

        // left comp e right int no acc ---------------------------------------

        if ((v_type[id] == 3) && (get_type(et) == 1) && (et % OFST == 0))
        {
            fprintf(stdout, "Atenção na linha %d: variável %s é comp, mas recebe int.\n", line_num+1, rem_fname(v_name[id], fname));
            
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("SET %s\n",  v_name[id]);

            add_instr("LOAD float_zero\n");
            add_instr("SET %s_i\n",  v_name[id]);
        }

        // left comp e right float var na memoria -----------------------------

        if ((v_type[id] == 3) && (get_type(et) == 2) && (v_isco[et%OFST] == 0) && (et % OFST != 0))
        {
            fprintf(stdout, "Atenção na linha %d: variável %s é comp, mas recebe float.\n", line_num+1, rem_fname(v_name[id], fname));

            add_instr("%s %s\n", ld, v_name[et % OFST]);
            add_instr("SET %s\n" , v_name[id]);

            add_instr("LOAD float_zero\n");
            add_instr("SET %s_i\n",  v_name[id]);
        }

        // left comp e right float const na memoria -----------------------------

        if ((v_type[id] == 3) && (get_type(et) == 2) && (v_isco[et%OFST] == 1) && (et % OFST != 0))
        {
            fprintf(stdout, "Atenção na linha %d: variável %s é comp, mas recebe float.\n", line_num+1, rem_fname(v_name[id], fname));

            add_instr("%s %u // %s\n", ld, f2mf(v_name[et % OFST]), v_name[et % OFST]);
            add_instr("SET %s\n" , v_name[id]);

            add_instr("LOAD float_zero\n");
            add_instr("SET %s_i\n",  v_name[id]);
        }

        // left comp e right float no acc -------------------------------------

        if ((v_type[id] == 3) && (get_type(et) == 2) && (et % OFST == 0))
        {
            fprintf(stdout, "Atenção na linha %d: variável %s é comp, mas recebe float.\n", line_num+1, rem_fname(v_name[id], fname));
            
            add_instr("SET %s\n",  v_name[id]);

            add_instr("LOAD float_zero\n");
            add_instr("SET %s_i\n",  v_name[id]);
        }

        // left comp e right comp const ---------------------------------------

        if ((v_type[id] == 3) && (get_type(et) == 5))
        {
            get_cmp_cst(et,&etr,&eti);
            
            add_instr("%s %u // %s\n", ld, f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("SET %s\n",  v_name[id]);

            add_instr("%s %u // %s\n", ld, f2mf(v_name[eti % OFST]), v_name[eti % OFST]);
            add_instr("SET %s_i\n",  v_name[id]);
        }

        // left comp e right comp na memoria ----------------------------------

        if ((v_type[id] == 3) && (get_type(et) == 3) && (et % OFST != 0))
        {
            get_cmp_ets(et,&etr,&eti);
            
            add_instr("%s %s\n", ld, v_name[etr % OFST]);
            add_instr("SET %s\n",  v_name[id]);

            add_instr("%s %s\n", ld, v_name[eti % OFST]);
            add_instr("SET %s_i\n",  v_name[id]);
        }

        // left comp e right comp no acc --------------------------------------

        if ((v_type[id] == 3) && (get_type(et) == 3) && (et % OFST == 0))
        {    
            add_instr("SETP %s_i\n", v_name[id]);
            add_instr("SET %s\n"  ,  v_name[id]);
        }
    }
    else
    {
        // left int e right int na memoria ------------------------------------

        if ((v_type[id] == 1) && (get_type(et) == 1) && (et % OFST != 0))
        {
            add_instr("%s %s\n", ld, v_name[et % OFST]);
            add_instr("SET %s\n",  v_name[id]);
        }

        // left int e right int no acc ----------------------------------------

        if ((v_type[id] == 1) && (get_type(et) == 1) && (et % OFST == 0))
        {
            add_instr("SET %s\n",  v_name[id]);
        }

        // left int e right float na memoria ----------------------------------

        if ((v_type[id] == 1) && (get_type(et) == 2) && (et % OFST != 0))
        {
            fprintf(stdout, "Atenção na linha %d: variável %s é int, mas recebe float.\n", line_num+1, rem_fname(v_name[id], fname));

            add_instr("%s %s\n", ld, v_name[et % OFST]);
            add_instr("SET %s\n",  v_name[id]);
        }

        // left int e right float no acc --------------------------------------

        if ((v_type[id] == 1) && (get_type(et) == 2) && (et % OFST == 0))
        {
            fprintf(stdout, "Atenção na linha %d: variável %s é int, mas recebe float.\n", line_num+1, rem_fname(v_name[id], fname));
            
            add_instr("SET %s\n",  v_name[id]);
        }

        // left int e right comp const ----------------------------------------

        if ((v_type[id] == 1) && (get_type(et) == 5))
        {
            fprintf (stdout, "Atenção na linha %d: nessa conversão, eu vou arredondar a parte real hein!\n", line_num+1);

            get_cmp_cst(et,&etr,&eti);
            
            add_instr("%s %s\n", ld, v_name[etr % OFST]);
            add_instr("SET %s\n",  v_name[id]);
        }

        // left int e right comp na memoria -----------------------------------

        if ((v_type[id] == 1) && (get_type(et) == 3) && (et % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: nessa conversão, eu vou arredondar a parte real hein!\n", line_num+1);

            get_cmp_ets(et,&etr,&eti);
            
            add_instr("%s %s\n", ld, v_name[etr % OFST]);
            add_instr("SET %s\n",  v_name[id]);
        }

        // left int e right comp no acc ---------------------------------------

        if ((v_type[id] == 1) && (get_type(et) == 3) && (et % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: nessa conversão, eu vou arredondar a parte real hein!\n", line_num+1);
    
            add_instr("SETP lixo\n");
            add_instr("SET %s\n",  v_name[id]);
        }

        // left float e right int na memoria ----------------------------------

        if ((v_type[id] == 2) && (get_type(et) == 1) && (et % OFST != 0))
        {
            fprintf(stdout, "Atenção na linha %d: variável %s é float, mas recebe int.\n", line_num+1, rem_fname(v_name[id], fname));

            add_instr("%s %s\n", ld, v_name[et % OFST]);
            add_instr("SET %s\n",  v_name[id]);
        }

        // left float e right int no acc --------------------------------------

        if ((v_type[id] == 2) && (get_type(et) == 1) && (et % OFST == 0))
        {
            fprintf(stdout, "Atenção na linha %d: variável %s é float, mas recebe int.\n", line_num+1, rem_fname(v_name[id], fname));
            
            add_instr("SET %s\n",  v_name[id]);
        }

        // left float e right float na memoria --------------------------------

        if ((v_type[id] == 2) && (get_type(et) == 2) && (et % OFST != 0))
        {
            add_instr("%s %s\n", ld, v_name[et % OFST]);
            add_instr("SET %s\n" , v_name[id]);
        }

        // left float e right float no acc ------------------------------------

        if ((v_type[id] == 2) && (get_type(et) == 2) && (et % OFST == 0))
        {
            add_instr("SET %s\n",  v_name[id]);
        }

        // left float e right comp const --------------------------------------

        if ((v_type[id] == 2) && (get_type(et) == 5))
        {
            fprintf (stdout, "Atenção na linha %d: nessa conversão, eu vou pegar só a parte real hein!\n", line_num+1);

            get_cmp_cst(et,&etr,&eti);
            
            add_instr("%s %s\n", ld, v_name[etr % OFST]);
            add_instr("SET %s\n",  v_name[id]);
        }

        // left float e right comp na memoria ---------------------------------

        if ((v_type[id] == 2) && (get_type(et) == 3) && (et % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: nessa conversão, eu vou pegar só a parte real hein!\n", line_num+1);

            get_cmp_ets(et,&etr,&eti);
            
            add_instr("%s %s\n", ld, v_name[etr % OFST]);
            add_instr("SET %s\n",  v_name[id]);
        }

        // left float e right comp no acc -------------------------------------

        if ((v_type[id] == 2) && (get_type(et) == 3) && (et % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: nessa conversão, eu vou pegar só a parte real hein!\n", line_num+1);
    
            add_instr("SETP lixo\n");
            add_instr("SET %s\n",  v_name[id]);
        }

        // left comp e right int na memoria -----------------------------------

        if ((v_type[id] == 3) && (get_type(et) == 1) && (et % OFST != 0))
        {
            fprintf(stdout, "Atenção na linha %d: variável %s é comp, mas recebe int.\n", line_num+1, rem_fname(v_name[id], fname));

            add_instr("%s %s\n", ld, v_name[et % OFST]);
            add_instr("SET %s\n"  ,  v_name[id]);

            add_instr("LOAD 0.0\n");
            add_instr("SET %s_i\n",  v_name[id]);
        }

        // left comp e right int no acc ---------------------------------------

        if ((v_type[id] == 3) && (get_type(et) == 1) && (et % OFST == 0))
        {
            fprintf(stdout, "Atenção na linha %d: variável %s é comp, mas recebe int.\n", line_num+1, rem_fname(v_name[id], fname));
            
            add_instr("SET %s\n"  ,  v_name[id]);

            add_instr("LOAD 0.0\n");
            add_instr("SET %s_i\n",  v_name[id]);
        }

        // left comp e right float na memoria ---------------------------------

        if ((v_type[id] == 3) && (get_type(et) == 2) && (et % OFST != 0))
        {
            fprintf(stdout, "Atenção na linha %d: variável %s é comp, mas recebe float.\n", line_num+1, rem_fname(v_name[id], fname));

            add_instr("%s %s\n", ld, v_name[et % OFST]);
            add_instr("SET %s\n" , v_name[id]);

            add_instr("LOAD 0.0\n");
            add_instr("SET %s_i\n",  v_name[id]);
        }

        // left comp e right float no acc -------------------------------------

        if ((v_type[id] == 3) && (get_type(et) == 2) && (et % OFST == 0))
        {
            fprintf(stdout, "Atenção na linha %d: variável %s é comp, mas recebe float.\n", line_num+1, rem_fname(v_name[id], fname));
            
            add_instr("SET %s\n",  v_name[id]);

            add_instr("LOAD 0.0\n");
            add_instr("SET %s_i\n",  v_name[id]);
        }

        // left comp e right comp const ---------------------------------------

        if ((v_type[id] == 3) && (get_type(et) == 5))
        {
            get_cmp_cst(et,&etr,&eti);
            
            add_instr("%s %s\n", ld, v_name[etr % OFST]);
            add_instr("SET %s\n"  ,  v_name[id]);

            add_instr("%s %s\n", ld, v_name[eti % OFST]);
            add_instr("SET %s_i\n",  v_name[id]);
        }

        // left comp e right comp na memoria ----------------------------------

        if ((v_type[id] == 3) && (get_type(et) == 3) && (et % OFST != 0))
        {
            get_cmp_ets(et,&etr,&eti);
            
            add_instr("%s %s\n", ld, v_name[etr % OFST]);
            add_instr("SET %s\n",  v_name[id]);

            add_instr("%s %s\n", ld, v_name[eti % OFST]);
            add_instr("SET %s_i\n",  v_name[id]);
        }

        // left comp e right comp no acc --------------------------------------

        if ((v_type[id] == 3) && (get_type(et) == 3) && (et % OFST == 0))
        {    
            add_instr("SETP %s_i\n", v_name[id]);
            add_instr("SET %s\n"  ,  v_name[id]);
        }
    }

    acc_ok     = 0;  // liberou o acc
    v_asgn[id] = 1;  // variavel recebeu um valor
}

// assign em array
void array_set(int id, int et, int fft)
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

    if (v_isar[id] == 0)
    {
        fprintf (stderr, "Erro na linha %d: %s não é um array.\n", line_num+1, rem_fname(v_name[id], fname));
        return;
    }

    // ------------------------------------------------------------------------
    // executa o set ----------------------------------------------------------
    // ------------------------------------------------------------------------

    char set_type[16];
    int  etr, eti;

    if (fft == 0) strcpy(set_type, "SRF"); else  strcpy(set_type, "ISRF");

    if (prtype == 0)
    {
        // left int e right int na memoria ------------------------------------

        if ((v_type[id] == 1) && (get_type(et) == 1) && (et % OFST != 0))
        {
            add_instr("PLD %s\n", v_name[et % OFST]);
            add_instr("%s\n", set_type);
            add_instr("SET %s\n",  v_name[id]);
        }

        // left int e right int no acc ----------------------------------------

        if ((v_type[id] == 1) && (get_type(et) == 1) && (et % OFST == 0))
        {
            add_instr("%s\n", set_type);
            add_instr("SET %s\n",  v_name[id]);
        }

        // left int e right float var na memoria ------------------------------

        if ((v_type[id] == 1) && (get_type(et) == 2) && (v_isco[et%OFST] == 0) && (et % OFST != 0))
        {
            fprintf(stdout, "Atenção na linha %d: variável %s é int, mas recebe float.\n", line_num+1, rem_fname(v_name[id], fname));

            add_instr("PLD %s\n", v_name[et % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("%s\n", set_type);
            add_instr("SET %s\n",  v_name[id]);
        }

        // left int e right float const na memoria ----------------------------

        if ((v_type[id] == 1) && (get_type(et) == 2) && (v_isco[et%OFST] == 1) && (et % OFST != 0))
        {
            fprintf(stdout, "Atenção na linha %d: variável %s é int, mas recebe float.\n", line_num+1, rem_fname(v_name[id], fname));

            add_instr("PLD %d // %s\n", f2mf(v_name[et % OFST]), v_name[et % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("%s\n", set_type);
            add_instr("SET %s\n",  v_name[id]);
        }

        // left int e right float no acc --------------------------------------

        if ((v_type[id] == 1) && (get_type(et) == 2) && (et % OFST == 0))
        {
            fprintf(stdout, "Atenção na linha %d: variável %s é int, mas recebe float.\n", line_num+1, rem_fname(v_name[id], fname));
            
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("%s\n", set_type);
            add_instr("SET %s\n",  v_name[id]);
        }

        // left int e right comp const ----------------------------------------

        if ((v_type[id] == 1) && (get_type(et) == 5))
        {
            fprintf (stdout, "Atenção na linha %d: nessa conversão, eu vou arredondar a parte real hein!\n", line_num+1);

            get_cmp_cst(et,&etr,&eti);
            
            add_instr("PLD %u // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("%s\n", set_type);
            add_instr("SET %s\n",  v_name[id]);
        }

        // left int e right comp na memoria -----------------------------------

        if ((v_type[id] == 1) && (get_type(et) == 3) && (et % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: nessa conversão, eu vou arredondar a parte real hein!\n", line_num+1);

            get_cmp_ets(et,&etr,&eti);
            
            add_instr("PLD %s\n", v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("%s\n", set_type);
            add_instr("SET %s\n",  v_name[id]);
        }

        // left int e right comp no acc ---------------------------------------

        if ((v_type[id] == 1) && (get_type(et) == 3) && (et % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: nessa conversão, eu vou arredondar a parte real hein!\n", line_num+1);
    
            add_instr("SETP lixo\n");
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("%s\n", set_type);
            add_instr("SET %s\n",  v_name[id]);
        }

        // left float e right int na memoria ----------------------------------

        if ((v_type[id] == 2) && (get_type(et) == 1) && (et % OFST != 0))
        {
            fprintf(stdout, "Atenção na linha %d: variável %s é float, mas recebe int.\n", line_num+1, rem_fname(v_name[id], fname));

            add_instr("PLD %s\n", v_name[et % OFST]);
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("%s\n", set_type);
            add_instr("SET %s\n",  v_name[id]);
        }

        // left float e right int no acc --------------------------------------

        if ((v_type[id] == 2) && (get_type(et) == 1) && (et % OFST == 0))
        {
            fprintf(stdout, "Atenção na linha %d: variável %s é float, mas recebe int.\n", line_num+1, rem_fname(v_name[id], fname));
            
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("%s\n", set_type);
            add_instr("SET %s\n",  v_name[id]);
        }

        // left float e right float var na memoria ----------------------------

        if ((v_type[id] == 2) && (get_type(et) == 2) && (v_isco[et%OFST] == 0) && (et % OFST != 0))
        {
            add_instr("PLD %s\n", v_name[et % OFST]);
            add_instr("%s\n", set_type);
            add_instr("SET %s\n" , v_name[id]);
        }

        // left float e right float const na memoria --------------------------

        if ((v_type[id] == 2) && (get_type(et) == 2) && (v_isco[et%OFST] == 1) && (et % OFST != 0))
        {
            add_instr("PLD %d // %s\n", f2mf(v_name[et % OFST]), v_name[et % OFST]);
            add_instr("%s\n", set_type);
            add_instr("SET %s\n" , v_name[id]);
        }

        // left float e right float no acc ------------------------------------

        if ((v_type[id] == 2) && (get_type(et) == 2) && (et % OFST == 0))
        {
            add_instr("%s\n", set_type);
            add_instr("SET %s\n",  v_name[id]);
        }

        // left float e right comp const --------------------------------------

        if ((v_type[id] == 2) && (get_type(et) == 5))
        {
            fprintf (stdout, "Atenção na linha %d: nessa conversão, eu vou pegar só a parte real hein!\n", line_num+1);

            get_cmp_cst(et,&etr,&eti);
            
            add_instr("PLD %u // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("%s\n", set_type);
            add_instr("SET %s\n",  v_name[id]);
        }

        // left float e right comp na memoria ---------------------------------

        if ((v_type[id] == 2) && (get_type(et) == 3) && (et % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: nessa conversão, eu vou pegar só a parte real hein!\n", line_num+1);

            get_cmp_ets(et,&etr,&eti);
            
            add_instr("PLD %s\n", v_name[etr % OFST]);
            add_instr("%s\n", set_type);
            add_instr("SET %s\n",  v_name[id]);
        }

        // left float e right comp no acc -------------------------------------

        if ((v_type[id] == 2) && (get_type(et) == 3) && (et % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: nessa conversão, eu vou pegar só a parte real hein!\n", line_num+1);
    
            add_instr("SETP lixo\n");
            add_instr("%s\n", set_type);
            add_instr("SET %s\n",  v_name[id]);
        }

        // left comp e right int na memoria -----------------------------------

        if ((v_type[id] == 3) && (get_type(et) == 1) && (et % OFST != 0))
        {
            fprintf(stdout, "Atenção na linha %d: variável %s é comp, mas recebe int.\n", line_num+1, rem_fname(v_name[id], fname));

            add_instr("SET aux_index\n");
            add_instr("PLD %s\n", v_name[et % OFST]);
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("%s\n", set_type);
            add_instr("SET %s\n",  v_name[id]);

            add_instr("LOAD aux_index\n");
            add_instr("PLD float_zero\n");
            add_instr("%s\n", set_type);
            add_instr("SET %s_i\n",  v_name[id]);
        }

        // left comp e right int no acc ---------------------------------------

        if ((v_type[id] == 3) && (get_type(et) == 1) && (et % OFST == 0))
        {
            fprintf(stdout, "Atenção na linha %d: variável %s é comp, mas recebe int.\n", line_num+1, rem_fname(v_name[id], fname));
            
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("SETP aux_tmp\n");
            add_instr("SET aux_index\n");
            add_instr("PLD aux_tmp\n");
            add_instr("%s\n", set_type);
            add_instr("SET %s\n",  v_name[id]);

            add_instr("LOAD aux_index\n");
            add_instr("PLD float_zero\n");
            add_instr("%s\n", set_type);
            add_instr("SET %s_i\n",  v_name[id]);
        }

        // left comp e right float var na memoria -----------------------------

        if ((v_type[id] == 3) && (get_type(et) == 2) && (v_isco[et%OFST] == 0) && (et % OFST != 0))
        {
            fprintf(stdout, "Atenção na linha %d: variável %s é comp, mas recebe float.\n", line_num+1, rem_fname(v_name[id], fname));

            add_instr("SET aux_index\n");
            add_instr("PLD %s\n", v_name[et % OFST]);
            add_instr("%s\n", set_type);
            add_instr("SET %s\n" , v_name[id]);

            add_instr("LOAD aux_index\n");
            add_instr("PLD float_zero\n");
            add_instr("%s\n", set_type);
            add_instr("SET %s_i\n",  v_name[id]);
        }

        // left comp e right float const na memoria ---------------------------

        if ((v_type[id] == 3) && (get_type(et) == 2) && (v_isco[et%OFST] == 1) && (et % OFST != 0))
        {
            fprintf(stdout, "Atenção na linha %d: variável %s é comp, mas recebe float.\n", line_num+1, rem_fname(v_name[id], fname));

            add_instr("SET aux_index\n");
            add_instr("PLD %d // %s\n", f2mf(v_name[et % OFST]), v_name[et % OFST]);
            add_instr("%s\n", set_type);
            add_instr("SET %s\n" , v_name[id]);

            add_instr("LOAD aux_index\n");
            add_instr("PLD float_zero\n");
            add_instr("%s\n", set_type);
            add_instr("SET %s_i\n",  v_name[id]);
        }

        // left comp e right float no acc -------------------------------------

        if ((v_type[id] == 3) && (get_type(et) == 2) && (et % OFST == 0))
        {
            fprintf(stdout, "Atenção na linha %d: variável %s é comp, mas recebe float.\n", line_num+1, rem_fname(v_name[id], fname));
            
            add_instr("SETP aux_tmp\n");
            add_instr("SET aux_index\n");
            add_instr("PLD aux_tmp\n");
            add_instr("%s\n", set_type);
            add_instr("SET %s\n",  v_name[id]);

            add_instr("LOAD aux_index\n");
            add_instr("PLD float_zero\n");
            add_instr("%s\n", set_type);
            add_instr("SET %s_i\n",  v_name[id]);
        }

        // left comp e right comp const ---------------------------------------

        if ((v_type[id] == 3) && (get_type(et) == 5))
        {
            get_cmp_cst(et,&etr,&eti);
            
            add_instr("SET aux_index\n");
            add_instr("PLD %u // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("%s\n", set_type);
            add_instr("SET %s\n",  v_name[id]);

            add_instr("LOAD aux_index\n");
            add_instr("PLD %u // %s\n", f2mf(v_name[eti % OFST]), v_name[eti % OFST]);
            add_instr("%s\n", set_type);
            add_instr("SET %s_i\n",  v_name[id]);
        }

        // left comp e right comp na memoria ----------------------------------

        if ((v_type[id] == 3) && (get_type(et) == 3) && (et % OFST != 0))
        {
            get_cmp_ets(et,&etr,&eti);
            
            add_instr("SET aux_index\n");
            add_instr("PLD %s\n", v_name[etr % OFST]);
            add_instr("%s\n", set_type);
            add_instr("SET %s\n",  v_name[id]);

            add_instr("LOAD aux_index\n");
            add_instr("PLD %s\n", v_name[eti % OFST]);
            add_instr("%s\n", set_type);
            add_instr("SET %s_i\n",  v_name[id]);
        }

        // left comp e right comp no acc --------------------------------------

        if ((v_type[id] == 3) && (get_type(et) == 3) && (et % OFST == 0))
        {    
            add_instr("SETP aux_tmp_i\n");
            add_instr("SETP aux_tmp_r\n");
            add_instr("SET aux_index\n");

            add_instr("PLD aux_tmp_r\n");
            add_instr("%s\n", set_type);
            add_instr("SET %s\n",  v_name[id]);

            add_instr("LOAD aux_index\n");
            add_instr("PLD aux_tmp_i\n");
            add_instr("%s\n", set_type);
            add_instr("SET %s_i\n",  v_name[id]);
        }
    }
    else
    {
        // left int e right int na memoria ------------------------------------

        if ((v_type[id] == 1) && (get_type(et) == 1) && (et % OFST != 0))
        {
            add_instr("PLD %s\n", v_name[et % OFST]);
            add_instr("%s\n", set_type);
            add_instr("SET %s\n",  v_name[id]);
        }

        // left int e right int no acc ----------------------------------------

        if ((v_type[id] == 1) && (get_type(et) == 1) && (et % OFST == 0))
        {
            add_instr("%s\n", set_type);
            add_instr("SET %s\n",  v_name[id]);
        }

        // left int e right float na memoria ----------------------------------

        if ((v_type[id] == 1) && (get_type(et) == 2) && (et % OFST != 0))
        {
            fprintf(stdout, "Atenção na linha %d: variável %s é int, mas recebe float.\n", line_num+1, rem_fname(v_name[id], fname));

            add_instr("PLD %s\n", v_name[et % OFST]);
            add_instr("%s\n", set_type);
            add_instr("SET %s\n",  v_name[id]);
        }

        // left int e right float no acc --------------------------------------

        if ((v_type[id] == 1) && (get_type(et) == 2) && (et % OFST == 0))
        {
            fprintf(stdout, "Atenção na linha %d: variável %s é int, mas recebe float.\n", line_num+1, rem_fname(v_name[id], fname));
            
            add_instr("%s\n", set_type);
            add_instr("SET %s\n",  v_name[id]);
        }

        // left int e right comp const ----------------------------------------

        if ((v_type[id] == 1) && (get_type(et) == 5))
        {
            fprintf (stdout, "Atenção na linha %d: nessa conversão, eu vou arredondar a parte real hein!\n", line_num+1);

            get_cmp_cst(et,&etr,&eti);
            
            add_instr("PLD %s\n", v_name[etr % OFST]);
            add_instr("%s\n", set_type);
            add_instr("SET %s\n",  v_name[id]);
        }

        // left int e right comp na memoria -----------------------------------

        if ((v_type[id] == 1) && (get_type(et) == 3) && (et % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: nessa conversão, eu vou arredondar a parte real hein!\n", line_num+1);

            get_cmp_ets(et,&etr,&eti);
            
            add_instr("PLD %s\n", v_name[etr % OFST]);
            add_instr("%s\n", set_type);
            add_instr("SET %s\n",  v_name[id]);
        }

        // left int e right comp no acc ---------------------------------------

        if ((v_type[id] == 1) && (get_type(et) == 3) && (et % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: nessa conversão, eu vou arredondar a parte real hein!\n", line_num+1);
    
            add_instr("SETP lixo\n");
            add_instr("%s\n", set_type);
            add_instr("SET %s\n",  v_name[id]);
        }

        // left float e right int na memoria ----------------------------------

        if ((v_type[id] == 2) && (get_type(et) == 1) && (et % OFST != 0))
        {
            fprintf(stdout, "Atenção na linha %d: variável %s é float, mas recebe int.\n", line_num+1, rem_fname(v_name[id], fname));

            add_instr("PLD %s\n", v_name[et % OFST]);
            add_instr("%s\n", set_type);
            add_instr("SET %s\n",  v_name[id]);
        }

        // left float e right int no acc --------------------------------------

        if ((v_type[id] == 2) && (get_type(et) == 1) && (et % OFST == 0))
        {
            fprintf(stdout, "Atenção na linha %d: variável %s é float, mas recebe int.\n", line_num+1, rem_fname(v_name[id], fname));
            
            add_instr("%s\n", set_type);
            add_instr("SET %s\n",  v_name[id]);
        }

        // left float e right float na memoria --------------------------------

        if ((v_type[id] == 2) && (get_type(et) == 2) && (et % OFST != 0))
        {
            add_instr("PLD %s\n", v_name[et % OFST]);
            add_instr("%s\n", set_type);
            add_instr("SET %s\n" , v_name[id]);
        }

        // left float e right float no acc ------------------------------------

        if ((v_type[id] == 2) && (get_type(et) == 2) && (et % OFST == 0))
        {
            add_instr("%s\n", set_type);
            add_instr("SET %s\n",  v_name[id]);
        }

        // left float e right comp const --------------------------------------

        if ((v_type[id] == 2) && (get_type(et) == 5))
        {
            fprintf (stdout, "Atenção na linha %d: nessa conversão, eu vou pegar só a parte real hein!\n", line_num+1);

            get_cmp_cst(et,&etr,&eti);
            
            add_instr("PLD %s\n", v_name[etr % OFST]);
            add_instr("%s\n", set_type);
            add_instr("SET %s\n",  v_name[id]);
        }

        // left float e right comp na memoria ---------------------------------

        if ((v_type[id] == 2) && (get_type(et) == 3) && (et % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: nessa conversão, eu vou pegar só a parte real hein!\n", line_num+1);

            get_cmp_ets(et,&etr,&eti);
            
            add_instr("PLD %s\n", v_name[etr % OFST]);
            add_instr("%s\n", set_type);
            add_instr("SET %s\n",  v_name[id]);
        }

        // left float e right comp no acc -------------------------------------

        if ((v_type[id] == 2) && (get_type(et) == 3) && (et % OFST == 0))
        {
            fprintf (stdout, "Atenção na linha %d: nessa conversão, eu vou pegar só a parte real hein!\n", line_num+1);
    
            add_instr("SETP lixo\n");
            add_instr("%s\n", set_type);
            add_instr("SET %s\n",  v_name[id]);
        }

        // left comp e right int na memoria -----------------------------------

        if ((v_type[id] == 3) && (get_type(et) == 1) && (et % OFST != 0))
        {
            fprintf(stdout, "Atenção na linha %d: variável %s é comp, mas recebe int.\n", line_num+1, rem_fname(v_name[id], fname));

            add_instr("SET aux_index\n");
            add_instr("PLD %s\n", v_name[et % OFST]);
            add_instr("%s\n", set_type);
            add_instr("SET %s\n",  v_name[id]);

            add_instr("LOAD aux_index\n");
            add_instr("PLD 0.0\n");
            add_instr("%s\n", set_type);
            add_instr("SET %s_i\n",  v_name[id]);
        }

        // left comp e right int no acc ---------------------------------------

        if ((v_type[id] == 3) && (get_type(et) == 1) && (et % OFST == 0))
        {
            fprintf(stdout, "Atenção na linha %d: variável %s é comp, mas recebe int.\n", line_num+1, rem_fname(v_name[id], fname));
            
            add_instr("SETP aux_tmp\n");
            add_instr("SET aux_index\n");
            add_instr("PLD aux_tmp\n");
            add_instr("%s\n", set_type);
            add_instr("SET %s\n",  v_name[id]);

            add_instr("LOAD aux_index\n");
            add_instr("PLD 0.0\n");
            add_instr("%s\n", set_type);
            add_instr("SET %s_i\n",  v_name[id]);
        }

        // left comp e right float na memoria ---------------------------------

        if ((v_type[id] == 3) && (get_type(et) == 2) && (et % OFST != 0))
        {
            fprintf(stdout, "Atenção na linha %d: variável %s é comp, mas recebe float.\n", line_num+1, rem_fname(v_name[id], fname));

            add_instr("SET aux_index\n");
            add_instr("PLD %s\n", v_name[et % OFST]);
            add_instr("%s\n", set_type);
            add_instr("SET %s\n" , v_name[id]);

            add_instr("LOAD aux_index\n");
            add_instr("PLD 0.0\n");
            add_instr("%s\n", set_type);
            add_instr("SET %s_i\n",  v_name[id]);
        }

        // left comp e right float no acc -------------------------------------

        if ((v_type[id] == 3) && (get_type(et) == 2) && (et % OFST == 0))
        {
            fprintf(stdout, "Atenção na linha %d: variável %s é comp, mas recebe float.\n", line_num+1, rem_fname(v_name[id], fname));
            
            add_instr("SETP aux_tmp\n");
            add_instr("SET aux_index\n");
            add_instr("PLD aux_tmp\n");
            add_instr("%s\n", set_type);
            add_instr("SET %s\n",  v_name[id]);

            add_instr("LOAD aux_index\n");
            add_instr("PLD 0.0\n");
            add_instr("%s\n", set_type);
            add_instr("SET %s_i\n",  v_name[id]);
        }

        // left comp e right comp const ---------------------------------------

        if ((v_type[id] == 3) && (get_type(et) == 5))
        {
            get_cmp_cst(et,&etr,&eti);
            
            add_instr("SET aux_index\n");
            add_instr("PLD %s\n", v_name[etr % OFST]);
            add_instr("%s\n", set_type);
            add_instr("SET %s\n",  v_name[id]);

            add_instr("LOAD aux_index\n");
            add_instr("PLD %s\n", v_name[eti % OFST]);
            add_instr("%s\n", set_type);
            add_instr("SET %s_i\n",  v_name[id]);
        }

        // left comp e right comp na memoria ----------------------------------

        if ((v_type[id] == 3) && (get_type(et) == 3) && (et % OFST != 0))
        {
            get_cmp_ets(et,&etr,&eti);
            
            add_instr("SET aux_index\n");
            add_instr("PLD %s\n", v_name[etr % OFST]);
            add_instr("%s\n", set_type);
            add_instr("SET %s\n",  v_name[id]);

            add_instr("LOAD aux_index\n");
            add_instr("PLD %s\n", v_name[eti % OFST]);
            add_instr("%s\n", set_type);
            add_instr("SET %s_i\n",  v_name[id]);
        }

        // left comp e right comp no acc --------------------------------------

        if ((v_type[id] == 3) && (get_type(et) == 3) && (et % OFST == 0))
        {    
            add_instr("SETP aux_tmp_i\n");
            add_instr("SETP aux_tmp_r\n");
            add_instr("SET aux_index\n");

            add_instr("PLD aux_tmp_r\n");
            add_instr("%s\n", set_type);
            add_instr("SET %s\n",  v_name[id]);

            add_instr("LOAD aux_index\n");
            add_instr("PLD aux_tmp_i\n");
            add_instr("%s\n", set_type);
            add_instr("SET %s_i\n",  v_name[id]);
        }
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
