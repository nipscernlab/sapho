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
int  exec_fft_set = 0;     // diz se o ultimo SET de um array foi ou nao com bit invertido
int  line_num     = 0;     // numero da linha sendo parseada
char fname [512];          // nome da funcao atual sendo parseada
int  v_asgn[NVARMAX];      // se variavel ja recebeu algum valor
int  v_isar[NVARMAX];      // se variavel eh um array
int  v_type[NVARMAX];      // 0 -> nao identificada, 1 -> int, 2 -> float
char v_name[NVARMAX][512]; // nome da variavel ou funcao

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

void var_set_new(int id, int et)
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

        // left int e right float na memoria ----------------------------------

        if ((v_type[id] == 1) && (get_type(et) == 2) && (et % OFST != 0))
        {
            fprintf(stdout, "Atenção na linha %d: variável %s é int, mas recebe float.\n", line_num+1, rem_fname(v_name[id], fname));

            add_instr("%s %s\n", ld, v_name[et % OFST]);
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

            split_cmp_const(et,&etr,&eti);
            
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

            split_cmp_const(et,&etr,&eti);
            
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

        // left comp e right float na memoria ---------------------------------

        if ((v_type[id] == 3) && (get_type(et) == 2) && (et % OFST != 0))
        {
            fprintf(stdout, "Atenção na linha %d: variável %s é comp, mas recebe float.\n", line_num+1, rem_fname(v_name[id], fname));

            add_instr("%s %s\n", ld, v_name[et % OFST]);
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
            split_cmp_const(et,&etr,&eti);
            
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

            split_cmp_const(et,&etr,&eti);
            
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

            split_cmp_const(et,&etr,&eti);
            
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
            split_cmp_const(et,&etr,&eti);
            
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

        // se float na memoria ------------------------------------------------

        if ((get_type(et) == 2) && (et % OFST != 0))
        {
            fprintf (stdout, "Atenção na linha %d: tá vendo que o índice do array tá em ponto flutuante né? Vou arredondar!\n", line_num+1);

            add_instr("LOAD %s\n", v_name[et % OFST]);
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

            split_cmp_const(et, &etr, &eti);

            add_instr("LOAD %s\n", v_name[etr % OFST]);
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

            split_cmp_const(et, &etr, &eti);

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

void array_1d_set(int id, int et, int fft)
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

    if (v_isar[id] == 2)
    {
        fprintf (stderr, "Erro na linha %d: tá faltando índice no array %s. Era pra ser de duas dimensões!\n", line_num+1, rem_fname(v_name[id], fname));
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

        // left int e right float na memoria ----------------------------------

        if ((v_type[id] == 1) && (get_type(et) == 2) && (et % OFST != 0))
        {
            fprintf(stdout, "Atenção na linha %d: variável %s é int, mas recebe float.\n", line_num+1, rem_fname(v_name[id], fname));

            add_instr("PLD %s\n", v_name[et % OFST]);
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

            split_cmp_const(et,&etr,&eti);
            
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

            split_cmp_const(et,&etr,&eti);
            
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

        // left comp e right float na memoria ---------------------------------

        if ((v_type[id] == 3) && (get_type(et) == 2) && (et % OFST != 0))
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
            split_cmp_const(et,&etr,&eti);
            
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

            split_cmp_const(et,&etr,&eti);
            
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

            split_cmp_const(et,&etr,&eti);
            
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
            split_cmp_const(et,&etr,&eti);
            
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
        add_instr("%s %s\n", cset, v_name[id]);
    }

    // float pra int ----------------------------------------------------------

    if ((left_type == 1) && (righ_type == 2))
    {
        if (warn) fprintf(stdout, "Atenção na linha %d: variável %s é int, mas recebe float.\n", line_num+1, rem_fname(v_name[id], fname));

        if (prtype == 0)
        {
            add_instr("CALL float2int\n");
            f2i = 1;
        }
            add_instr("%s %s\n", cset, v_name[id]);
    }

    // int pra float ----------------------------------------------------------

    if ((left_type == 2) && (righ_type == 1))
    {
        if (warn) fprintf(stdout, "Atenção na linha %d: variável %s é float, mas recebe int.\n", line_num+1, rem_fname(v_name[id], fname));

        if (prtype == 0)
        {
            add_instr("CALL int2float\n");
            i2f = 1;
        }
        add_instr("%s %s\n", cset, v_name[id]);
    }

    // float pra float --------------------------------------------------------

    if ((left_type == 2) && (righ_type == 2))
    {
        add_instr("%s %s\n", cset, v_name[id]);
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
        fprintf (stdout, "Atenção na linha %d: nessa conversão, eu vou arredondar a parte real hein!\n", line_num+1);

        split_cmp_const(et,&et_r,&et_i);
                var_set(id, et_r,is_array,0,0,1);
    }

    // left int e right comp var ----------------------------------------------

    if ((t_left == 1) && (t_right == 3) && (mem != 0))
    {
        fprintf (stdout, "Atenção na linha %d: nessa conversão, eu vou arredondar a parte real hein!\n", line_num+1);

        get_cmp_ets(et,&et_r,&et_i); // pega os IDs estendidos do right na memoria
            var_set(id, et_r,is_array,0,0,1);
    }

    // left int e right comp acc ----------------------------------------------

    if ((t_left == 1) && (t_right == 3) && (mem == 0))
    {
        fprintf (stdout, "Atenção na linha %d: nessa conversão, eu vou arredondar a parte real hein!\n", line_num+1);

        add_instr("SETP aux_cmp\n");
        var_set(id,2*OFST,is_array,0,0,1);
    }

    // left float e right comp const -----------------------------------------

    if ((t_left == 2) && (t_right == 5))
    {
        fprintf (stdout, "Atenção na linha %d: vou pegar só a parte real!\n", line_num+1);

        split_cmp_const(et,&et_r,&et_i);
                var_set(id, et_r,is_array,0,0,1);
    }

    // left float e right comp var --------------------------------------------

    if ((t_left == 2) && (t_right == 3) && (mem != 0))
    {
        fprintf (stdout, "Atenção na linha %d: vou pegar só a parte real!\n", line_num+1);

        get_cmp_ets(et,&et_r,&et_i); // pega os IDs estendidos do right na memoria
            var_set(id, et_r,is_array,0,0,1);
    }

    // left float e right comp acc ----------------------------------------------

    if ((t_left == 2) && (t_right == 3) && (mem == 0))
    {
        fprintf (stdout, "Atenção na linha %d: vou pegar só a parte real!\n", line_num+1);

        add_instr("SETP aux_cmp\n");
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
            add_instr("SET auxr_cmp\n");
            add_instr("LOAD aux_idx%d\n", a_cnt);
            add_instr("PLD auxr_cmp\n");
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
            add_instr("SET auxr_cmp\n");
            add_instr("LOAD aux_idx%d\n", a_cnt);
            add_instr("PLD auxr_cmp\n");
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
            add_instr("LOAD aux_idx%d\n", a_cnt);
            acc_ok = 1; // pra nao perder o indice
        }

        v_type[id_r] = 2; var_set(id_r,et_r,is_array,0,0,1); v_type[id_r] = 3;

        if (is_array > 0)
        {   // tem que salvar o indice momentaneamente em aux_cmpx
            add_instr("LOAD aux_idx%d\n", a_cnt--);
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
            add_instr("LOAD aux_idx%d\n", a_cnt);
            acc_ok = 1; // pra nao perder o indice
        }

        v_type[id_r] = 2; var_set(id_r,et_r,is_array,0,0,1); v_type[id_r] = 3;

        if (is_array > 0)
        {   // tem que salvar o indice momentaneamente em aux_cmpx
            add_instr("LOAD aux_idx%d\n", a_cnt--);
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
            add_instr("SETP auxi_cmp\n");
            add_instr("SET auxr_cmp\n");
            add_instr("LOAD aux_idx%d\n", a_cnt);
            add_instr("PLD auxi_cmp\n");
        }

        int pop = (is_array < 1); // se for array, usa PSET pra nao perder o img, se nao, usa SET
        v_type[id_i] = 2; var_set(id_i,2*OFST,is_array,pop,0,1); v_type[id_i] = 4;

        if (is_array > 0)
            // agora coloca o indice e a parte real pra fazer o SET
        {   // esse decremento eh para o indice do array do lado esquerdo (ex: d[3] = x;)
            add_instr("LOAD aux_idx%d\n", a_cnt--);
            add_instr("PLD auxr_cmp\n");
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
