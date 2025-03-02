#include "..\Headers\data_assign.h"
#include "..\Headers\data_use.h"
#include "..\Headers\variaveis.h"
#include "..\Headers\diretivas.h"
#include "..\Headers\t2t.h"

#include <string.h>

// ----------------------------------------------------------------------------
// redeclaracao de variaveis globais ------------------------------------------
// ----------------------------------------------------------------------------

FILE *f_asm;               // arquivo de saida

int  acc_ok       = 0;     // 0 -> acc vazio (use LOAD)  , 1 -> acc carregado (use PLD)
int  prtype       = 0;     // 0 -> processador fonto fixo, 1 -> processador ponto flutuante
int  line_num     = 0;     // numero da linha sendo parseada
int  i2f = 0, f2i = 0;     // se vai precisar das macros int2float e float2int

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

// ----------------------------------------------------------------------------
// funcoes de assign ----------------------------------------------------------
// ----------------------------------------------------------------------------

// assign padrao, ex: x = y;
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

// assign em array, ex x[i] = y;
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

// assign com operador ++
void pplus_assign(int id)
{
    exp_pplus(id);
    acc_ok = 0; // liberou o acc;
}

// assign com operador ++ em array 1D
void aplus_assign(int id, int et)
{
    array_pplus(id,et);
    acc_ok = 0; // liberou o acc;
}

// assign com operador ++ em array 2D
void aplu2_assign(int id, int et1, int et2)
{
    array_2plus(id,et1,et2);
    acc_ok = 0; // liberou o acc;
}
