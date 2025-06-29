// ----------------------------------------------------------------------------
// biblioteca padrao do sapho -------------------------------------------------
// ----------------------------------------------------------------------------

// includes globais
#include <string.h>
#include <stdlib.h>

// includes locais
#include "..\Headers\t2t.h"
#include "..\Headers\oper.h"
#include "..\Headers\stdlib.h"
#include "..\Headers\global.h"
#include "..\Headers\macros.h"
#include "..\Headers\data_use.h"
#include "..\Headers\variaveis.h"
#include "..\Headers\diretivas.h"

// ----------------------------------------------------------------------------
// entrada e saida ------------------------------------------------------------
// ----------------------------------------------------------------------------

// input
int exec_in(int id)
{
    if (atoi(v_name[id]) >= nuioin) fprintf(stderr, "Erro na linha %d: não tem porta de entrada %s não!\n", line_num+1, v_name[id]);
    if (acc_ok == 0) add_instr("INN %s\n", v_name[id]); else add_instr("P_INN %s\n", v_name[id]);

    acc_ok = 1;  // diz que o acc agora tem um valor carregado

    return OFST;
}

// output
void exec_out(int id, int et)
{
    if (atoi(v_name[id]) >= nuioou) fprintf(stderr, "Erro na linha %d: não tem porta de saída %s não!\n", line_num+1, v_name[id]);
    if (get_type(et) > 2) fprintf (stderr, "Erro na linha %d: primeiro seleciona qual informação desse número complexo você quer!\n", line_num+1);

    // int var
    if ((get_type(et) == 1) && (et%OFST!=0))
    {
       if (acc_ok == 0) add_instr("LOD %s\n", v_name[et%OFST]); else add_instr("P_LOD %s\n", v_name[et%OFST]);
    }

    // int acc
    if ((get_type(et) == 1) && (et%OFST==0))
    {
        // nao faz nada
    }

    // float var
    if ((get_type(et) == 2) && (et%OFST!=0))
    {
        fprintf(stdout, "Atenção na linha %d: convertendo o dado pra inteiro antes de sair do processador\n", line_num+1);

        if (acc_ok == 0) add_instr("F2I_M %s\n", v_name[et%OFST]); else add_instr("P_F2I_M %s\n", v_name[et%OFST]);
    }

    // float acc
    if ((get_type(et) == 2) && (et%OFST==0))
    {
        fprintf(stdout, "Atenção na linha %d: convertendo o dado pra inteiro antes de sair do processador\n", line_num+1);

        add_instr("F2I\n");
    }

    add_instr("OUT %s\n", v_name[id]);

    acc_ok = 0; // libera acc    
}

// ----------------------------------------------------------------------------
// funcoes especiais que evitam codigo ----------------------------------------
// ----------------------------------------------------------------------------

// pega o sinal do primeiro argumento e coloca no segundo
int exec_sign(int et1, int et2)
{
    if ((get_type(et1) > 2) || (get_type(et2) > 2))
    {
        fprintf (stderr, "Erro na linha %d: não faz sentido o uso de sign(.,.) com números complexos!\n", line_num+1);
    }

    char ld[10]; if (acc_ok == 0) strcpy(ld,"LOD"); else strcpy(ld,"P_LOD");

    // int na memoria e int na memoria
    if ((get_type(et1) == 1) && (et1 % OFST != 0) && (get_type(et2) == 1) && (et2 % OFST != 0))
    {
        add_instr("%s %s\n", ld, v_name[et2%OFST]);
        add_instr("SGN %s\n"   , v_name[et1%OFST]);
    }

    // int na memoria e int no acc
    if ((get_type(et1) == 1) && (et1 % OFST != 0) && (get_type(et2) == 1) && (et2 % OFST == 0))
    {
        add_instr("SGN %s\n", v_name[et1%OFST]);
    }

    // int na memoria e float var na memoria
    if ((get_type(et1) == 1) && (et1 % OFST != 0) && (get_type(et2) == 2) && (et2 % OFST != 0))
    {
        add_instr("%s %s\n", ld, v_name[et2%OFST]);
        add_instr("F_SGN %s\n" , v_name[et1%OFST]);
    }

    // int na memoria e float no acc
    if ((get_type(et1) == 1) && (et1 % OFST != 0) && (get_type(et2) == 2) && (et2 % OFST == 0))
    {
        add_instr("F_SGN %s\n", v_name[et1%OFST]);
    }

    // int no acc e int na memoria
    if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 1) && (et2 % OFST != 0))
    {
        add_instr("%s %s\n", ld, v_name[et2%OFST]);
        add_instr("S_SGN\n");
    }

    // int no acc e int no acc
    if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 1) && (et2 % OFST == 0))
    {
        add_instr("S_SGN\n");
    }

    // int no acc e float var na memoria
    if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 2) && (et2 % OFST != 0))
    {
        add_instr("P_LOD %s\n", v_name[et2%OFST]  );
        add_instr("SF_SGN\n");
    }

    // int no acc e float no acc
    if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 2) && (et2 % OFST == 0))
    {
        add_instr("SF_SGN\n");
    }

    // float var e int var
    if ((get_type(et1) == 2) && (et1 % OFST != 0) && (get_type(et2) == 1) && (et2 % OFST != 0))
    {
        add_instr("%s %s\n", ld, v_name[et2%OFST]);
        add_instr("SGN %s\n"   , v_name[et1%OFST]);
    }

    // float var e int no acc
    if ((get_type(et1) == 2) && (et1 % OFST != 0) && (get_type(et2) == 1) && (et2 % OFST == 0))
    {
        add_instr("SGN %s\n"  , v_name[et1%OFST]);
    }

    // float var e float var
    if ((get_type(et1) == 2) && (et1 % OFST != 0) && (get_type(et2) == 2) && (et2 % OFST != 0))
    {
        add_instr("%s %s\n", ld, v_name[et2%OFST]);
        add_instr("F_SGN %s\n" , v_name[et1%OFST]);
    }

    // float var e float no acc
    if ((get_type(et1) == 2) && (et1 % OFST != 0) && (get_type(et2) == 2) && (et2 % OFST == 0))
    {
        add_instr("F_SGN %s\n" , v_name[et1%OFST]);
    }

    // float no acc e int na memoria
    if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 1) && (et2 % OFST != 0))
    {
        add_instr("%s %s\n", ld, v_name[et2%OFST]);
        add_instr("S_SGN\n");
    }

    // float no acc e int no acc
    if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 1) && (et2 % OFST == 0))
    {
        add_instr("S_SGN\n");
    }

    // float no acc e float var na memoria
    if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 2) && (et2 % OFST != 0))
    {
        add_instr("P_LOD %s\n", v_name[et2%OFST]);
        add_instr("SF_SGN\n");
    }

    // float no acc e float no acc
    if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 2) && (et2 % OFST == 0))
    {
        add_instr("SF_SGN\n");
    }

    acc_ok = 1;

    return get_type(et2)*OFST;
}

// valor absoluto (int, float e comp)
int exec_abs(int et)
{
    char  ld [10]; if (acc_ok == 0) strcpy(ld  , "LOD"   ); else strcpy(ld  , "P_LOD"  );
    char  abs[10]; if (acc_ok == 0) strcpy( abs, "ABS_M" ); else strcpy( abs, "P_ABS_M");
    char fabs[10]; if (acc_ok == 0) strcpy(fabs,"F_ABS_M"); else strcpy(fabs,"PF_ABS_M");

    // int na memoria
    if ((get_type(et) == 1) && (et % OFST != 0))
    {
        add_instr("%s %s\n", abs, v_name[et%OFST]);
    }

    // int no acc
    if ((get_type(et) == 1) && (et % OFST == 0))
    {
        add_instr("ABS\n");
    }

    // float na memoria
    if ((get_type(et) == 2) && (et % OFST != 0))
    {
        add_instr("%s %s\n", fabs, v_name[et%OFST]);
    }

    // float no acc
    if ((get_type(et) == 2) && (et % OFST == 0))
    {
        add_instr("F_ABS\n");
    }

    // comp const, na memoria e no acc
    if ((get_type(et) == 3) || (get_type(et) == 5))
    {
        et = exec_sqrt(exec_sqr2(et));
    }

    acc_ok = 1;

    return get_type(et)*OFST;
}

// zera se for negativo
int exec_pst(int et)
{
    char  ld [10]; if (acc_ok == 0) strcpy( ld , "LOD"   ); else strcpy( ld , "P_LOD"  );
    char  pst[10]; if (acc_ok == 0) strcpy( pst, "PST_M" ); else strcpy( pst, "P_PST_M");
    char fpst[10]; if (acc_ok == 0) strcpy(fpst,"F_PST_M"); else strcpy(fpst,"PF_PST_M");

    // int na memoria
    if ((get_type(et) == 1) && (et % OFST != 0))
    {
        add_instr("%s %s\n", pst, v_name[et%OFST]);
    }

    // int no acc
    if ((get_type(et) == 1) && (et % OFST == 0))
    {
        add_instr("PST\n");
    }

    // float na memoria
    if ((get_type(et) == 2)  && (et % OFST != 0))
    {
        add_instr("%s %s\n", fpst, v_name[et%OFST]);
    }

    // float no acc
    if ((get_type(et) == 2)  && (et % OFST == 0))
    {
        add_instr("F_PST\n");
    }

    // comp
    if (get_type(et) > 2)
    {
        fprintf (stderr, "Erro na linha %d: Não faz nenhum sentido usar a função 'pset(.)' com números complexos!\n", line_num+1);
    }

    acc_ok = 1;

    return get_type(et)*OFST;
}

// divisao por constante
int exec_norm(int et)
{
    if (get_type(et) != 1)
        fprintf (stderr, "Erro na linha %d: nada a ver! norm(.) é só pra inteiro!\n", line_num+1);

    char nrm[10]; if (acc_ok == 0) strcpy(nrm,"NRM_M"); else strcpy(nrm,"P_NRM_M");

    // int na memoria
    if ((get_type(et) == 1) && (et % OFST != 0))
    {
        add_instr("%s %s\n", nrm, v_name[et%OFST]);
    }

    // int no acc
    if ((get_type(et) == 1) && (et % OFST == 0))
    {
        add_instr("NRM\n");
    }

    acc_ok = 1;

    return OFST;
}

// ----------------------------------------------------------------------------
// funcoes aritmeticas --------------------------------------------------------
// ----------------------------------------------------------------------------

// raiz quadrada
int exec_sqrt(int et)
{
    if (get_type(et) > 2)
    {
        fprintf (stderr, "Erro na linha %d: não implementei raiz quadrada de número complexo ainda. Se vira!\n", line_num+1);
    }

    char ld [10]; if (acc_ok == 0) strcpy(ld ,"LOD"   ); else strcpy(ld ,"P_LOD"  );
    char i2f[10]; if (acc_ok == 0) strcpy(i2f, "I2F_M"); else strcpy(i2f,"P_I2F_M");

    // int na memoria
    if ((get_type(et) == 1) && (et % OFST != 0))
    {
        add_instr("%s %s\n", i2f, v_name[et%OFST]);
        add_instr("CAL float_sqrt\n");
    }

    // int no acc
    if ((get_type(et) == 1) && (et % OFST == 0))
    {
        add_instr("I2F\n");
        add_instr("CAL float_sqrt\n");
    }

    // float na memoria
    if ((get_type(et) == 2) && (et % OFST != 0))
    {
        add_instr("%s %s\n", ld, v_name[et%OFST]);
        add_instr("CAL float_sqrt\n");
    }

    // float no acc
    if ((get_type(et) == 2) && (et % OFST == 0))
    {
        add_instr("CAL float_sqrt\n");
    }

    acc_ok = 1;

    return 2*OFST;
}

// arco-tangente
int exec_atan(int et)
{
    if (get_type(et) > 2) fprintf (stderr, "Erro na linha %d: não implementei arco-tangente de número complexo ainda. Se vira!\n", line_num+1);

    char ld [10]; if (acc_ok == 0) strcpy(ld ,"LOD"  ); else strcpy(ld ,"P_LOD"  );
    char i2f[10]; if (acc_ok == 0) strcpy(i2f,"I2F_M"); else strcpy(i2f,"P_I2F_M");

    // int na memoria
    if ((get_type(et) == 1) && (et % OFST != 0))
    {
        add_instr("%s %s\n", i2f, v_name[et%OFST]);
        add_instr("CAL float_atan\n");
    }

    // int no acc
    if ((get_type(et) == 1) && (et % OFST == 0))
    {
        add_instr("I2F\n");
        add_instr("CAL float_atan\n");
    }

    // float na memoria
    if ((get_type(et) == 2) && (et % OFST != 0))
    {
        add_instr("%s %s\n", ld, v_name[et%OFST]);
        add_instr("CAL float_atan\n");
    }

    // float no acc
    if ((get_type(et) == 2) && (et % OFST == 0))
    {
        add_instr("CAL float_atan\n");
    }

    acc_ok = 1;

    return 2*OFST;
}

// seno
int exec_sin(int et)
{
    if (get_type(et) > 2) fprintf (stderr, "Erro na linha %d: não implementei seno pra número complexo ainda. Se vira!\n", line_num+1);

    char ld [10]; if (acc_ok == 0) strcpy(ld ,"LOD"  ); else strcpy(ld ,"P_LOD"  );
    char i2f[10]; if (acc_ok == 0) strcpy(i2f,"I2F_M"); else strcpy(i2f,"P_I2F_M");

    // int na memoria
    if ((get_type(et) == 1) && (et % OFST != 0))
    {
        add_instr("%s %s\n", i2f, v_name[et%OFST]);
        add_instr("CAL float_sin\n");
    }

    // int no acc
    if ((get_type(et) == 1) && (et % OFST == 0))
    {
        add_instr("I2F\n");
        add_instr("CAL float_sin\n");
    }

    // float na memoria
    if ((get_type(et) == 2) && (et % OFST != 0))
    {
        add_instr("%s %s\n", ld, v_name[et%OFST]);
        add_instr("CAL float_sin\n");
    }

    // float no acc
    if ((get_type(et) == 2) && (et % OFST == 0))
    {
        add_instr("CAL float_sin\n");
    }

    acc_ok = 1;

    return 2*OFST;
}

// cosseno
int exec_cos(int et)
{
    if (get_type(et) > 2) fprintf (stderr, "Erro na linha %d: não implementei cosseno pra número complexo ainda. Se vira!\n", line_num+1);

    char ld [10]; if (acc_ok == 0) strcpy(ld ,"LOD"  ); else strcpy(ld ,"P_LOD"  );
    char i2f[10]; if (acc_ok == 0) strcpy(i2f,"I2F_M"); else strcpy(i2f,"P_I2F_M");

    // int na memoria
    if ((get_type(et) == 1) && (et % OFST != 0))
    {
        add_instr("%s %s\n", i2f, v_name[et%OFST]);
        add_instr("F_NEG\n");
        add_instr("F_ADD 1.570796327");
        add_instr("CAL float_sin\n");
    }

    // int no acc
    if ((get_type(et) == 1) && (et % OFST == 0))
    {
        add_instr("I2F\n");
        add_instr("F_NEG\n");
        add_instr("F_ADD 1.570796327");
        add_instr("CAL float_sin\n");
    }

    // float na memoria
    if ((get_type(et) == 2) && (et % OFST != 0))
    {
        add_instr("%s %s\n", ld, v_name[et%OFST]);
        add_instr("F_NEG\n");
        add_instr("F_ADD 1.570796327");
        add_instr("CAL float_sin\n");
    }

    // float no acc
    if ((get_type(et) == 2) && (et % OFST == 0))
    {
        add_instr("F_NEG\n");
        add_instr("F_ADD 1.570796327");
        add_instr("CAL float_sin\n");
    }

    acc_ok = 1;

    return 2*OFST;
}

// ----------------------------------------------------------------------------
// funcoes especiais para numeros complexos -----------------------------------
// ----------------------------------------------------------------------------

// retorna a parte real de um comp
int exec_real(int et)
{
    if (get_type(et) < 3) fprintf (stderr, "Erro na linha %d: argumento da função real(.) tem que ser complexo!\n", line_num+1);

    int id = et % OFST;

    char ld[10]; if (acc_ok == 0) strcpy(ld,"LOD"); else strcpy(ld,"P_LOD");

    // comp const
    if (get_type(et) == 5)
    {
        int et_r, et_i;
        get_cmp_cst(et,&et_r,&et_i);

        add_instr("%s %s\n", ld, v_name[et_r%OFST]);
    }

    // comp na memoria
    if ((get_type(et) == 3) && (id != 0))
    {
        add_instr("%s %s\n", ld, v_name[id]);
    }

    // comp no acc
    if ((get_type(et) == 3) && (id == 0))
    {
        add_instr("POP\n");
    }

    acc_ok = 1;
    
    return 2*OFST;
}

// retorna a parte imag de um comp
int exec_imag(int et)
{
    if (get_type(et) < 3) fprintf (stderr, "Erro na linha %d: argumento da função imag(.) tem que ser complexo!\n", line_num+1);

    int id = et % OFST;
    int et_r, et_i;

    char ld[10]; if (acc_ok == 0) strcpy(ld,"LOD"); else strcpy(ld,"P_LOD");

    // comp const
    if (get_type(et) == 5)
    {
        int et_r, et_i;
        get_cmp_cst(et,&et_r,&et_i);

        add_instr("%s %s\n", ld, v_name[et_i%OFST]);
    }

    // comp na memoria
    if ((get_type(et) == 3) && (id != 0))
    {
        get_cmp_ets(et,&et_r,&et_i);
        add_instr("%s %s\n", ld, v_name[et_i%OFST]);
    }

    // comp no acc
    if ((get_type(et) == 3) && (id == 0))
    {
        add_instr("SET_P aux_var\n");
        add_instr("LOD   aux_var\n");
    }

    acc_ok = 1;

    return 2*OFST;
}

// modulo ao quadrado de um num complexo
// ainda nao tem um equivalente no parser (colocar)
// mas é usado internamente em oper.c
int exec_sqr2(int et)
{
    int type = get_type(et);

    int etr, eti;

    // se for uma constante ---------------------------------------------------

    if (type == 5)
    {
        get_cmp_cst(et ,&etr,&eti);     // pega o et de cada constante float
        etr  = oper_mult(etr, etr);     // parte real ao quadrado
        eti  = oper_mult(eti, eti);     // parte imag ao quadrado
        etr  = oper_soma(etr, eti);     // soma os quadrados
    }

    // se estiver na memoria --------------------------------------------------

    if ((type == 3) && (et % OFST != 0))
    {
        get_cmp_ets(et ,&etr,&eti);     // pega o et de cada constante float
        etr  = oper_mult(etr, etr);     // parte real ao quadrado
        eti  = oper_mult(eti, eti);     // parte imag ao quadrado
        etr  = oper_soma(etr, eti);     // soma os quadrados
    }

    // se estiver no acumulador -----------------------------------------------

    if ((type == 3) && (et % OFST == 0))
    {
        add_instr("PSH\n");             // parte imag fica no acc e pilha
        oper_mult(2*OFST,2*OFST );      // multiplica acc com pilha
        add_instr("SET_P aux_var\n");   // salva temp e pega parte real

        add_instr("PSH\n");             // parte real fica no acc e pilha
        oper_mult(2*OFST,2*OFST );      // multiplica acc com pilha
        add_instr("P_LOD aux_var\n");   // xuxa o quadr do real pra pilha e pega o quadr do imag

        oper_soma(2*OFST,2*OFST);       // soma os quadrados

        etr = 2*OFST;                   // saida tem q ser et estendido pra float no acc
    }

    return etr; // 2*OFST?
}

// calcula a fase (em radianos) de um num complexo
int exec_fase(int et)
{
    if (get_type(et) < 3) fprintf (stderr, "Erro na linha %d: argumento da função fase(.) tem que ser complexo!\n", line_num+1);

    int id = et % OFST;
    int et_r, et_i;

    // comp const
    if (get_type(et) == 5)
    {
        get_cmp_cst(et,&et_i,&et_r);
        oper_divi  (et_r,et_i);
    }

    // comp na memoria
    if ((get_type(et) == 3) && (id != 0))
    {
        get_cmp_ets(et,&et_i,&et_r);
        oper_divi  (et_r,et_i);
    }

    // comp no acc
    if ((get_type(et) == 3) && (id == 0))
    {
        id   = exec_id("aux_var");
        et_i = 2*OFST + id;

        add_instr("SET_P %s\n", v_name[id]);
        oper_divi(et_i,2*OFST);
    }

    exec_atan(2*OFST);

    acc_ok = 1;
    return 2*OFST;
}

// junta doi numeros reais pra fazer um complexo
int exec_comp(int etr, int eti)
{
    if (get_type(etr) > 2 || get_type(eti > 2)) fprintf (stderr, "Erro na linha %d: argumentos da função complex(.,.) não podem ser complexos!\n", line_num+1);

    char ld [10]; if (acc_ok == 0) strcpy(ld ,"LOD"  ); else strcpy(ld ,"P_LOD"  );
    char i2f[10]; if (acc_ok == 0) strcpy(i2f,"I2F_M"); else strcpy(i2f,"P_I2F_M");

    // int na memoria e int na memoria
    if ((get_type(etr) == 1) && (etr % OFST != 0) && (get_type(eti) == 1) && (eti % OFST != 0))
    {
        add_instr("%s %s\n", i2f, v_name[etr%OFST]);
        add_instr("P_I2F_M %s\n", v_name[eti%OFST]);
    }

    // int na memoria e int no acc
    if ((get_type(etr) == 1) && (etr % OFST != 0) && (get_type(eti) == 1) && (eti % OFST == 0))
    {
        add_instr("SET aux_var\n");
        add_instr("I2F_M %s\n", v_name[etr%OFST]);
        add_instr("P_I2F_M aux_var\n");
    }

    // int na memoria e float na memoria
    if ((get_type(etr) == 1) && (etr % OFST != 0) && (get_type(eti) == 2) && (eti % OFST != 0))
    {
        add_instr("%s %s\n", i2f, v_name[etr%OFST]);
        add_instr("P_LOD %s\n"  , v_name[eti%OFST]);
    }

    // int na memoria e float no acc
    if ((get_type(etr) == 1) && (etr % OFST != 0) && (get_type(eti) == 2) && (eti % OFST == 0))
    {
        add_instr("SET aux_var\n");
        add_instr("I2F_M %s\n", v_name[etr%OFST]);
        add_instr("P_LOD aux_var\n");
    }

    // int no acc e int na memoria
    if ((get_type(etr) == 1) && (etr % OFST == 0) && (get_type(eti) == 1) && (eti % OFST != 0))
    {
        add_instr("I2F\n");
        add_instr("P_I2F_M %s\n", v_name[eti%OFST]);
    }

    // int no acc e int no acc
    if ((get_type(etr) == 1) && (etr % OFST == 0) && (get_type(eti) == 1) && (eti % OFST == 0))
    {
        add_instr("SET_P aux_var\n");
        add_instr("I2F\n");
        add_instr("P_I2F_M aux_var\n");
    }

    // int no acc e float na memoria
    if ((get_type(etr) == 1) && (etr % OFST == 0) && (get_type(eti) == 2) && (eti % OFST != 0))
    {
        add_instr("I2F\n");
        add_instr("P_LOD %s\n", v_name[eti%OFST]);
    }

    // int no acc e float no acc
    if ((get_type(etr) == 1) && (etr % OFST == 0) && (get_type(eti) == 2) && (eti % OFST == 0))
    {
        add_instr("SET_P aux_var\n");
        add_instr("I2F\n");
        add_instr("P_LOD aux_var\n");
    }

    // float na memoria e int na memoria
    if ((get_type(etr) == 2) && (etr % OFST != 0) && (get_type(eti) == 1) && (eti % OFST != 0))
    {
        add_instr("%s %s\n",  ld, v_name[etr%OFST]);
        add_instr("P_I2F_M %s\n", v_name[eti%OFST]);
    }

    // float na memoria e int no acc
    if ((get_type(etr) == 2) && (etr % OFST != 0) && (get_type(eti) == 1) && (eti % OFST == 0))
    {
        add_instr("SET aux_var\n");
        add_instr("LOD %s\n", v_name[etr%OFST]);
        add_instr("P_I2F_M aux_var\n");
    }

    // float na memoria e float na memoria
    if ((get_type(etr) == 2) && (etr % OFST != 0) && (get_type(eti) == 2) && (eti % OFST != 0))
    {
        add_instr("%s %s\n", ld, v_name[etr%OFST]);
        add_instr("P_LOD %s\n" , v_name[eti%OFST]);
    }

    // float na memoria e float no acc
    if ((get_type(etr) == 2) && (etr % OFST != 0) && (get_type(eti) == 2) && (eti % OFST == 0))
    {
        add_instr("SET aux_var\n");
        add_instr("LOD %s\n", v_name[etr%OFST]);
        add_instr("P_LOD aux_var\n");
    }

    // float no acc e int na memoria
    if ((get_type(etr) == 2) && (etr % OFST == 0) && (get_type(eti) == 1) && (eti % OFST != 0))
    {
        add_instr("P_I2F_M %s\n", v_name[eti%OFST]);
    }

    // float no acc e int no acc
    if ((get_type(etr) == 2) && (etr % OFST == 0) && (get_type(eti) == 1) && (eti % OFST == 0))
    {
        add_instr("I2F\n");
    }

    // float no acc e float na memoria
    if ((get_type(etr) == 2) && (etr % OFST == 0) && (get_type(eti) == 2) && (eti % OFST != 0))
    {
        add_instr("P_LOD %s\n", v_name[eti%OFST]);
    }

    // float no acc e float no acc
    if ((get_type(etr) == 1) && (etr % OFST == 0) && (get_type(eti) == 2) && (eti % OFST == 0))
    {
        // nao faz nada
    }

    acc_ok = 1;
    return 3*OFST;
}