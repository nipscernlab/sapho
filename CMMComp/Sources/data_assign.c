#include "..\Headers\data_assign.h"
#include "..\Headers\data_use.h"
#include "..\Headers\variaveis.h"
#include "..\Headers\diretivas.h"
#include "..\Headers\t2t.h"
#include "..\Headers\global.h"

#include <string.h>

// ----------------------------------------------------------------------------
// redeclaracao de variaveis globais ------------------------------------------
// ----------------------------------------------------------------------------

FILE *f_asm;               // arquivo de saida

int  acc_ok   = 0;         // 0 -> acc vazio (use LOD)  , 1 -> acc carregado (use P_LOD)
int  line_num = 0;         // numero da linha sendo parseada

char fname [512];          // nome da funcao atual sendo parseada
int  v_asgn[NVARMAX];      // se variavel ja recebeu algum valor
int  v_isar[NVARMAX];      // se variavel eh um array
int  v_type[NVARMAX];      // 0 -> nao identificada, 1 -> int, 2 -> float
char v_name[NVARMAX][512]; // nome da variavel ou funcao

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

    // left int e right int na memoria ------------------------------------

    if ((v_type[id] == 1) && (get_type(et) == 1) && (et % OFST != 0))
    {
        add_instr("LOD %s\n",  v_name[et % OFST]);
        add_instr("SET %s\n" , v_name[id       ]);
    }

    // left int e right int no acc ----------------------------------------

    if ((v_type[id] == 1) && (get_type(et) == 1) && (et % OFST == 0))
    {
        add_instr("SET %s\n", v_name[id]);
    }

    // left int e right float var na memoria ------------------------------

    if ((v_type[id] == 1) && (get_type(et) == 2) && (v_isco[et % OFST] == 0) && (et % OFST != 0))
    {
        fprintf(stdout, "Atenção na linha %d: variável %s é int, mas recebe float.\n", line_num+1, rem_fname(v_name[id], fname));

        add_instr("F2I_M %s\n", v_name[et % OFST]);
        add_instr("SET %s\n", v_name[id       ]);
    }

    // left int e right float const na memoria ----------------------------

    if ((v_type[id] == 1) && (get_type(et) == 2) && (v_isco[et % OFST] == 1) && (et % OFST != 0))
    {
        fprintf(stdout, "Atenção na linha %d: variável %s é int, mas recebe float.\n", line_num+1, rem_fname(v_name[id], fname));

        add_instr("F2I_M %s\n", v_name[et % OFST]);
        add_instr("SET %s\n",                                v_name[id       ]);
    }

    // left int e right float no acc --------------------------------------

    if ((v_type[id] == 1) && (get_type(et) == 2) && (et % OFST == 0))
    {
        fprintf(stdout, "Atenção na linha %d: variável %s é int, mas recebe float.\n", line_num+1, rem_fname(v_name[id], fname));
        
        // tentar fazer uma instrucao que faz f2i do acumulador e seta na memoria ao mesmo tempo (FIAS)
        add_instr("F2I\n");
        add_instr("SET %s\n", v_name[id]);
    }

    // left int e right comp const ----------------------------------------

    if ((v_type[id] == 1) && (get_type(et) == 5))
    {
        fprintf (stdout, "Atenção na linha %d: nessa conversão, eu vou arredondar a parte real hein!\n", line_num+1);

        get_cmp_cst(et,&etr,&eti);
        
        add_instr("F2I_M %s\n", v_name[etr % OFST]);
        add_instr("SET %s\n",                                 v_name[id        ]);
    }

    // left int e right comp na memoria -----------------------------------

    if ((v_type[id] == 1) && (get_type(et) == 3) && (et % OFST != 0))
    {
        fprintf (stdout, "Atenção na linha %d: nessa conversão, eu vou arredondar a parte real hein!\n", line_num+1);
        
        add_instr("F2I_M %s\n", v_name[et % OFST]);
        add_instr("SET %s\n", v_name[id       ]);
    }

    // left int e right comp no acc ---------------------------------------

    if ((v_type[id] == 1) && (get_type(et) == 3) && (et % OFST == 0))
    {
        fprintf (stdout, "Atenção na linha %d: nessa conversão, eu vou arredondar a parte real hein!\n", line_num+1);

        add_instr("POP\n");
        add_instr("F2I\n");
        add_instr("SET %s\n", v_name[id]);
    }

    // left float e right const int na memoria ----------------------------

    if ((v_type[id] == 2) && (get_type(et) == 1) && (et % OFST != 0) && (v_isco[et % OFST] == 1))
    {
        fprintf(stdout, "Atenção na linha %d: variável %s é float, mas recebe int.\n", line_num+1, rem_fname(v_name[id], fname));

        add_instr("LOD %s\n", v_name[et % OFST]);
        add_instr("SET %s\n", v_name[id]);
    }

    // left float e right var int na memoria ------------------------------
    if ((v_type[id] == 2) && (get_type(et) == 1) && (et % OFST != 0) && (v_isco[et % OFST] == 0))
    {
        fprintf(stdout, "Atenção na linha %d: variável %s é float, mas recebe int.\n", line_num+1, rem_fname(v_name[id], fname));

        add_instr("I2F_M %s\n", v_name[et % OFST]);
        add_instr("SET %s\n", v_name[id]);
    }

    // left float e right int no acc --------------------------------------

    if ((v_type[id] == 2) && (get_type(et) == 1) && (et % OFST == 0))
    {
        fprintf(stdout, "Atenção na linha %d: variável %s é float, mas recebe int.\n", line_num+1, rem_fname(v_name[id], fname));
        
        add_instr("I2F\n");
        add_instr("SET %s\n", v_name[id]);
    }

    // left float e right float const na memoria --------------------------

    if ((v_type[id] == 2) && (get_type(et) == 2) && (v_isco[et%OFST] == 1) && (et%OFST != 0))
    {
        add_instr("LOD %s\n", v_name[et % OFST]);
        add_instr("SET %s\n",                                 v_name[id       ]);
    }

    // left float e right float var na memoria ----------------------------

    if ((v_type[id] == 2) && (get_type(et) == 2) && (v_isco[et%OFST] == 0) && (et%OFST != 0))
    {
        add_instr("LOD %s\n" , v_name[et % OFST]);
        add_instr("SET %s\n" , v_name[id       ]);
    }

    // left float e right float no acc ------------------------------------

    if ((v_type[id] == 2) && (get_type(et) == 2) && (et % OFST == 0))
    {
        add_instr("SET %s\n", v_name[id]);
    }

    // left float e right comp const --------------------------------------

    if ((v_type[id] == 2) && (get_type(et) == 5))
    {
        fprintf (stdout, "Atenção na linha %d: nessa conversão, eu vou pegar só a parte real hein!\n", line_num+1);

        get_cmp_cst(et,&etr,&eti);
        
        add_instr("LOD %s\n", v_name[etr % OFST]);
        add_instr("SET %s\n",                                  v_name[id        ]);
    }

    // left float e right comp na memoria ---------------------------------

    if ((v_type[id] == 2) && (get_type(et) == 3) && (et % OFST != 0))
    {
        fprintf (stdout, "Atenção na linha %d: nessa conversão, eu vou pegar só a parte real hein!\n", line_num+1);

        get_cmp_ets(et,&etr,&eti);
        
        add_instr("LOD %s\n",  v_name[etr % OFST]);
        add_instr("SET %s\n",  v_name[id        ]);
    }

    // left float e right comp no acc -------------------------------------

    if ((v_type[id] == 2) && (get_type(et) == 3) && (et % OFST == 0))
    {
        fprintf (stdout, "Atenção na linha %d: nessa conversão, eu vou pegar só a parte real hein!\n", line_num+1);

        add_instr("POP\n");
        add_instr("SET %s\n", v_name[id]);
    }

    // left comp e right int const na memoria -----------------------------

    if ((v_type[id] == 3) && (get_type(et) == 1) && (et % OFST != 0) && (v_isco[et % OFST] == 1))
    {
        fprintf(stdout, "Atenção na linha %d: variável %s é comp, mas recebe int.\n", line_num+1, rem_fname(v_name[id], fname));

        add_instr("LOD %s\n", v_name[et % OFST]);
        add_instr("SET %s\n", v_name[id]);

        add_instr("LOD 0.0\n");
        add_instr("SET %s_i\n", v_name[id]);
    }

    // left comp e right int var na memoria -------------------------------

    if ((v_type[id] == 3) && (get_type(et) == 1) && (et % OFST != 0) && (v_isco[et % OFST] == 0))
    {
        fprintf(stdout, "Atenção na linha %d: variável %s é comp, mas recebe int.\n", line_num+1, rem_fname(v_name[id], fname));

        add_instr("I2F_M %s\n", v_name[et % OFST]);
        add_instr("SET %s\n", v_name[id]);

        add_instr("LOD 0.0\n");
        add_instr("SET %s_i\n", v_name[id]);
    }

    // left comp e right int no acc ---------------------------------------

    if ((v_type[id] == 3) && (get_type(et) == 1) && (et % OFST == 0))
    {
        fprintf(stdout, "Atenção na linha %d: variável %s é comp, mas recebe int.\n", line_num+1, rem_fname(v_name[id], fname));
        
        add_instr("I2F\n");
        add_instr("SET %s\n", v_name[id]);

        add_instr("LOD 0.0\n");
        add_instr("SET %s_i\n", v_name[id]);
    }

    // left comp e right float var na memoria -----------------------------

    if ((v_type[id] == 3) && (get_type(et) == 2) && (v_isco[et%OFST] == 0) && (et % OFST != 0))
    {
        fprintf(stdout, "Atenção na linha %d: variável %s é comp, mas recebe float.\n", line_num+1, rem_fname(v_name[id], fname));

        add_instr("LOD %s\n" , v_name[et % OFST]);
        add_instr("SET %s\n" , v_name[id       ]);

        add_instr("LOD 0.0\n");
        add_instr("SET %s_i\n", v_name[id]);
    }

    // left comp e right float const na memoria -----------------------------

    if ((v_type[id] == 3) && (get_type(et) == 2) && (v_isco[et%OFST] == 1) && (et % OFST != 0))
    {
        fprintf(stdout, "Atenção na linha %d: variável %s é comp, mas recebe float.\n", line_num+1, rem_fname(v_name[id], fname));

        add_instr("LOD %s\n", v_name[et % OFST]);
        add_instr("SET %s\n"                                , v_name[id       ]);

        add_instr("LOD 0.0\n");
        add_instr("SET %s_i\n", v_name[id]);
    }

    // left comp e right float no acc -------------------------------------

    if ((v_type[id] == 3) && (get_type(et) == 2) && (et % OFST == 0))
    {
        fprintf(stdout, "Atenção na linha %d: variável %s é comp, mas recebe float.\n", line_num+1, rem_fname(v_name[id], fname));
        
        add_instr("SET %s\n", v_name[id]);

        add_instr("LOD 0.0\n");
        add_instr("SET %s_i\n", v_name[id]);
    }

    // left comp e right comp const ---------------------------------------

    if ((v_type[id] == 3) && (get_type(et) == 5))
    {
        get_cmp_cst(et,&etr,&eti);
        
        add_instr("LOD %s\n", v_name[etr % OFST]);
        add_instr("SET %s\n"                                 , v_name[id        ]);

        add_instr("LOD %s\n", v_name[eti % OFST]);
        add_instr("SET %s_i\n"                               , v_name[id        ]);
    }

    // left comp e right comp na memoria ----------------------------------

    if ((v_type[id] == 3) && (get_type(et) == 3) && (et % OFST != 0))
    {
        get_cmp_ets(et,&etr,&eti);
        
        add_instr("LOD %s\n"  , v_name[etr % OFST]);
        add_instr("SET %s\n"  , v_name[id        ]);

        add_instr("LOD %s\n"  , v_name[eti % OFST]);
        add_instr("SET %s_i\n", v_name[id        ]);
    }

    // left comp e right comp no acc --------------------------------------

    if ((v_type[id] == 3) && (get_type(et) == 3) && (et % OFST == 0))
    {    
        add_instr("SET_P %s_i\n", v_name[id]);
        add_instr("SET %s\n"  ,  v_name[id]);
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
    // checagem entre array e nao-array ---------------------------------------
    // ------------------------------------------------------------------------

    if (v_isar[id] == 0)
    {
        fprintf (stderr, "Erro na linha %d: %s não é um array.\n", line_num+1, rem_fname(v_name[id], fname));
        return;
    }

    // ------------------------------------------------------------------------
    // executa o set ----------------------------------------------------------
    // ------------------------------------------------------------------------

    int  etr, eti;

    char set_type[16]; if (fft == 0) strcpy(set_type, "SRF"); else strcpy(set_type, "IRF");

    // left int e right int na memoria ------------------------------------

    if ((v_type[id] == 1) && (get_type(et) == 1) && (et % OFST != 0))
    {
        add_instr("P_LOD %s\n",  v_name[et % OFST]);
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

        add_instr("P_F2I_M %s\n", v_name[et % OFST]);
        add_instr("%s\n", set_type);
        add_instr("SET %s\n",  v_name[id]);
    }

    // left int e right float const na memoria ----------------------------

    if ((v_type[id] == 1) && (get_type(et) == 2) && (v_isco[et%OFST] == 1) && (et % OFST != 0))
    {
        fprintf(stdout, "Atenção na linha %d: variável %s é int, mas recebe float.\n", line_num+1, rem_fname(v_name[id], fname));

        add_instr("P_F2I_M %s\n", v_name[et % OFST]);
        add_instr("%s\n", set_type);
        add_instr("SET %s\n", v_name[id]);
    }

    // left int e right float no acc --------------------------------------

    if ((v_type[id] == 1) && (get_type(et) == 2) && (et % OFST == 0))
    {
        fprintf(stdout, "Atenção na linha %d: variável %s é int, mas recebe float.\n", line_num+1, rem_fname(v_name[id], fname));
        
        add_instr("F2I\n");
        add_instr("%s\n", set_type);
        add_instr("SET %s\n", v_name[id]);
    }

    // left int e right comp const ----------------------------------------

    if ((v_type[id] == 1) && (get_type(et) == 5))
    {
        fprintf (stdout, "Atenção na linha %d: nessa conversão, eu vou arredondar a parte real hein!\n", line_num+1);

        get_cmp_cst(et,&etr,&eti);
        
        add_instr("P_F2I_M %s\n", v_name[etr % OFST]);
        add_instr("%s\n", set_type);
        add_instr("SET %s\n", v_name[id]);
    }

    // left int e right comp na memoria -----------------------------------

    if ((v_type[id] == 1) && (get_type(et) == 3) && (et % OFST != 0))
    {
        fprintf (stdout, "Atenção na linha %d: nessa conversão, eu vou arredondar a parte real hein!\n", line_num+1);

        get_cmp_ets(et,&etr,&eti);
        
        add_instr("P_F2I_M %s\n", v_name[etr % OFST]);
        add_instr("%s\n", set_type);
        add_instr("SET %s\n",  v_name[id]);
    }

    // left int e right comp no acc ---------------------------------------

    if ((v_type[id] == 1) && (get_type(et) == 3) && (et % OFST == 0))
    {
        fprintf (stdout, "Atenção na linha %d: nessa conversão, eu vou arredondar a parte real hein!\n", line_num+1);

        add_instr("POP\n");
        add_instr("F2I\n");
        add_instr("%s\n", set_type);
        add_instr("SET %s\n",  v_name[id]);
    }

    // left float e right int na memoria ----------------------------------

    if ((v_type[id] == 2) && (get_type(et) == 1) && (et % OFST != 0))
    {
        fprintf(stdout, "Atenção na linha %d: variável %s é float, mas recebe int.\n", line_num+1, rem_fname(v_name[id], fname));

        add_instr("P_I2F_M %s\n", v_name[et % OFST]);
        add_instr("%s\n", set_type);
        add_instr("SET %s\n" , v_name[id]);
    }

    // left float e right int no acc --------------------------------------

    if ((v_type[id] == 2) && (get_type(et) == 1) && (et % OFST == 0))
    {
        fprintf(stdout, "Atenção na linha %d: variável %s é float, mas recebe int.\n", line_num+1, rem_fname(v_name[id], fname));
        
        add_instr("I2F\n");
        add_instr("%s\n", set_type);
        add_instr("SET %s\n", v_name[id]);
    }

    // left float e right float var na memoria ----------------------------

    if ((v_type[id] == 2) && (get_type(et) == 2) && (v_isco[et%OFST] == 0) && (et % OFST != 0))
    {
        add_instr("P_LOD %s\n", v_name[et % OFST]);
        add_instr("%s\n", set_type);
        add_instr("SET %s\n", v_name[id]);
    }

    // left float e right float const na memoria --------------------------

    if ((v_type[id] == 2) && (get_type(et) == 2) && (v_isco[et%OFST] == 1) && (et % OFST != 0))
    {
        add_instr("P_LOD %s\n", v_name[et % OFST]);
        add_instr("%s\n", set_type);
        add_instr("SET %s\n", v_name[id]);
    }

    // left float e right float no acc ------------------------------------

    if ((v_type[id] == 2) && (get_type(et) == 2) && (et % OFST == 0))
    {
        add_instr("%s\n", set_type);
        add_instr("SET %s\n", v_name[id]);
    }

    // left float e right comp const --------------------------------------

    if ((v_type[id] == 2) && (get_type(et) == 5))
    {
        fprintf (stdout, "Atenção na linha %d: nessa conversão, eu vou pegar só a parte real hein!\n", line_num+1);

        get_cmp_cst(et,&etr,&eti);
        
        add_instr("P_LOD %s\n", v_name[etr % OFST]);
        add_instr("%s\n", set_type);
        add_instr("SET %s\n", v_name[id]);
    }

    // left float e right comp na memoria ---------------------------------

    if ((v_type[id] == 2) && (get_type(et) == 3) && (et % OFST != 0))
    {
        fprintf (stdout, "Atenção na linha %d: nessa conversão, eu vou pegar só a parte real hein!\n", line_num+1);

        get_cmp_ets(et,&etr,&eti);
        
        add_instr("P_LOD %s\n", v_name[etr % OFST]);
        add_instr("%s\n", set_type);
        add_instr("SET %s\n", v_name[id]);
    }

    // left float e right comp no acc -------------------------------------

    if ((v_type[id] == 2) && (get_type(et) == 3) && (et % OFST == 0))
    {
        fprintf (stdout, "Atenção na linha %d: nessa conversão, eu vou pegar só a parte real hein!\n", line_num+1);

        add_instr("POP\n");
        add_instr("%s\n", set_type);
        add_instr("SET %s\n", v_name[id]);
    }

    // left comp e right int na memoria -----------------------------------

    if ((v_type[id] == 3) && (get_type(et) == 1) && (et % OFST != 0))
    {
        fprintf(stdout, "Atenção na linha %d: variável %s é comp, mas recebe int.\n", line_num+1, rem_fname(v_name[id], fname));

        add_instr("SET aux_index\n");
        add_instr("P_I2F_M %s\n", v_name[et % OFST]);
        add_instr("%s\n", set_type);
        add_instr("SET %s\n",  v_name[id]);

        add_instr("LOD aux_index\n");
        add_instr("P_LOD 0.0\n");
        add_instr("%s\n", set_type);
        add_instr("SET %s_i\n", v_name[id]);
    }

    // left comp e right int no acc ---------------------------------------

    if ((v_type[id] == 3) && (get_type(et) == 1) && (et % OFST == 0))
    {
        fprintf(stdout, "Atenção na linha %d: variável %s é comp, mas recebe int.\n", line_num+1, rem_fname(v_name[id], fname));
        
        add_instr("I2F\n"                );
        add_instr("SET_P aux_tmp\n"       );
        add_instr("SET  aux_index\n"     );
        add_instr("P_LOD  aux_tmp\n"       );
        add_instr("%s\n", set_type       );
        add_instr("SET  %s\n", v_name[id]);

        add_instr("LOD aux_index\n"                  );
        add_instr("P_LOD 0.0\n");
        add_instr("%s\n", set_type                    );
        add_instr("SET %s_i\n", v_name[id]            );
    }

    // left comp e right float var na memoria -----------------------------

    if ((v_type[id] == 3) && (get_type(et) == 2) && (v_isco[et%OFST] == 0) && (et % OFST != 0))
    {
        fprintf(stdout, "Atenção na linha %d: variável %s é comp, mas recebe float.\n", line_num+1, rem_fname(v_name[id], fname));

        add_instr("SET aux_index\n");
        add_instr("P_LOD %s\n", v_name[et % OFST]);
        add_instr("%s\n", set_type);
        add_instr("SET %s\n" , v_name[id]);

        add_instr("LOD aux_index\n");
        add_instr("P_LOD 0.0\n");
        add_instr("%s\n", set_type);
        add_instr("SET %s_i\n",  v_name[id]);
    }

    // left comp e right float const na memoria ---------------------------

    if ((v_type[id] == 3) && (get_type(et) == 2) && (v_isco[et%OFST] == 1) && (et % OFST != 0))
    {
        fprintf(stdout, "Atenção na linha %d: variável %s é comp, mas recebe float.\n", line_num+1, rem_fname(v_name[id], fname));

        add_instr("SET aux_index\n");
        add_instr("P_LOD %s\n", v_name[et % OFST]);
        add_instr("%s\n", set_type);
        add_instr("SET %s\n", v_name[id]);

        add_instr("LOD aux_index\n");
        add_instr("P_LOD 0.0\n");
        add_instr("%s\n", set_type);
        add_instr("SET %s_i\n", v_name[id]);
    }

    // left comp e right float no acc -------------------------------------

    if ((v_type[id] == 3) && (get_type(et) == 2) && (et % OFST == 0))
    {
        fprintf(stdout, "Atenção na linha %d: variável %s é comp, mas recebe float.\n", line_num+1, rem_fname(v_name[id], fname));
        
        add_instr("SET_P aux_tmp\n"  );
        add_instr("SET  aux_index\n");
        add_instr("P_LOD  aux_tmp\n"  );
        add_instr("%s\n", set_type  );
        add_instr("SET %s\n",  v_name[id]);

        add_instr("LOD aux_index\n");
        add_instr("P_LOD 0.0\n");
        add_instr("%s\n", set_type);
        add_instr("SET %s_i\n", v_name[id]);
    }

    // left comp e right comp const ---------------------------------------

    if ((v_type[id] == 3) && (get_type(et) == 5))
    {
        get_cmp_cst(et,&etr,&eti);
        
        add_instr("SET aux_index\n");
        add_instr("P_LOD %s\n", v_name[etr % OFST]);
        add_instr("%s\n", set_type);
        add_instr("SET %s\n", v_name[id]);

        add_instr("LOD aux_index\n");
        add_instr("P_LOD %s\n", v_name[eti % OFST]);
        add_instr("%s\n", set_type);
        add_instr("SET %s_i\n", v_name[id]);
    }

    // left comp e right comp na memoria ----------------------------------

    if ((v_type[id] == 3) && (get_type(et) == 3) && (et % OFST != 0))
    {
        get_cmp_ets(et,&etr,&eti);
        
        add_instr("SET aux_index\n");
        add_instr("P_LOD %s\n", v_name[etr % OFST]);
        add_instr("%s\n", set_type);
        add_instr("SET %s\n",  v_name[id]);

        add_instr("LOD aux_index\n");
        add_instr("P_LOD %s\n", v_name[eti % OFST]);
        add_instr("%s\n", set_type);
        add_instr("SET %s_i\n",  v_name[id]);
    }

    // left comp e right comp no acc --------------------------------------

    if ((v_type[id] == 3) && (get_type(et) == 3) && (et % OFST == 0))
    {    
        add_instr("SET_P aux_tmp_i\n");
        add_instr("SET_P aux_tmp_r\n");
        add_instr("SET aux_index\n");

        add_instr("P_LOD aux_tmp_r\n");
        add_instr("%s\n", set_type);
        add_instr("SET %s\n",  v_name[id]);

        add_instr("LOD aux_index\n");
        add_instr("P_LOD aux_tmp_i\n");
        add_instr("%s\n", set_type);
        add_instr("SET %s_i\n",  v_name[id]);
    }

    acc_ok     = 0;  // liberou o acc
    v_asgn[id] = 1;  // variavel recebeu um valor
}

// assign com operador ++
void pplus_assign(int id)
{
    pplus2exp(id);
    acc_ok = 0; // liberou o acc;
}

// assign com operador ++ em array 1D
void aplus_assign(int id, int et)
{
    pplus1d2exp(id,et);
    acc_ok = 0; // liberou o acc;
}

// assign com operador ++ em array 2D
void aplu2_assign(int id, int et1, int et2)
{
    pplus2d2exp(id,et1,et2);
    acc_ok = 0; // liberou o acc;
}
