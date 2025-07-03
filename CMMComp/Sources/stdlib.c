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
#include "..\Headers\funcoes.h"
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

// ----------------------------------------------------------------------------
// funcoes especiais para trabalho com vetores --------------------------------
// ----------------------------------------------------------------------------

// multiplicacao entre dois vetores
int exec_vtv(int id1, int id2)
{
    // ------------------------------------------------------------------------
    // checa consistencia -----------------------------------------------------
    // ------------------------------------------------------------------------

    // checa se id1 foi declarado
    if (v_type[id1] == 0) fprintf(stderr, "Erro na linha %d: não tem essa variável %s!\n", line_num+1, rem_fname(v_name[id1], fname));

    // checa se id2 foi declarado
    if (v_type[id2] == 0) fprintf(stderr, "Erro na linha %d: não tem essa variável %s!\n", line_num+1, rem_fname(v_name[id2], fname));

    // checa se sao vetores mesmo
    if (v_isar[id1] != 1 || v_isar[id2] != 1) fprintf(stderr, "Erro na linha %d: o nome tá dizendo, produto vetorial é entre vetores!\n", line_num+1);

    // checa se tamanhos sao iguais
    if (v_size[id1] != v_size[id2]) fprintf(stderr, "Erro na linha %d: vetores de tamanhos diferentes? Vai estudar Álgebra Linear primeiro!\n", line_num+1);

    // checa se sao do mesmo tipo
    if (v_type[id1] != v_type[id2]) fprintf(stderr, "Erro na linha %d: tipos de dados diferentes. Você é uma pessoa confusa!\n", line_num+1);

    // checa se tem variavel tipo comp
    if (v_type[id1] == 3 || v_type[id2] == 3) fprintf(stderr, "Erro na linha %d: não implementei isso pra número complexo ainda. Se vira!\n", line_num+1);
    
    // ------------------------------------------------------------------------
    // prepara variaveis locais -----------------------------------------------
    // ------------------------------------------------------------------------

    char ld [10]; if (acc_ok == 0) strcpy(ld ,"LOD"); else strcpy(ld ,"P_LOD");

    int N = v_size[id1];

    // ------------------------------------------------------------------------
    // implementa o produto entre vetores -------------------------------------
    // ------------------------------------------------------------------------

    // int com int
    if ((v_type[id1] == 1) && (v_type[id2] == 1))
    {
        add_instr( "%s %s\n", ld, v_name[id1]);
        add_instr("MLT %s\n",     v_name[id2]);

        for (int i = 1; i < N; i++)
        {
            add_instr("P_LOD_V %s %d\n", v_name[id1], i);
            add_instr(  "MLT_V %s %d\n", v_name[id2], i);
            add_instr("S_ADD\n");
        }
    }

    // float com float
    if ((v_type[id1] == 2) && (v_type[id2] == 2))
    {
        add_instr(   "%s %s\n", ld, v_name[id1]);
        add_instr("F_MLT %s\n",     v_name[id2]);

        for (int i = 1; i < N; i++)
        {
            add_instr("P_LOD_V %s %d\n", v_name[id1], i);
            add_instr("F_MLT_V %s %d\n", v_name[id2], i);
            add_instr("SF_ADD\n");
        }
    }

    acc_ok = 1;
    return v_type[id1]*OFST;
}

// multiplicacao de matriz por vetor
void exec_Mv(int idy, int idM, int idv)
{
    // ------------------------------------------------------------------------
    // checa consistencia -----------------------------------------------------
    // ------------------------------------------------------------------------

    // checa se idy foi declarada
    if (v_type[idy] == 0) fprintf(stderr, "Erro na linha %d: tem que declarar %s primeiro!\n", line_num+1, rem_fname(v_name[idy], fname));
    
    // checa se idM foi declarada
    if (v_type[idM] == 0) fprintf(stderr, "Erro na linha %d: tem que declarar %s primeiro!\n", line_num+1, rem_fname(v_name[idM], fname));
    
    // checa se idy foi declarada
    if (v_type[idv] == 0) fprintf(stderr, "Erro na linha %d: tem que declarar %s primeiro!\n", line_num+1, rem_fname(v_name[idv], fname));
    
    // checa se os tipos sao os mesmos
    if (v_type[idy] != v_type[idM] || v_type[idy] != v_type[idv])
    {
        fprintf(stderr, "Erro na linha %d: as variáveis tem que ser do mesmo tipo!\n", line_num+1);
    }

    // checa se nao eh comp
    if (v_type[idy] == 3 || v_type[idM] == 3 || v_type[idv] == 3)
    {
        fprintf(stderr, "Erro na linha %d: não implementei isso pra números complexos ainda. Se vira!\n", line_num+1);
    }

    // checa se idy eh um vetor
    if (v_isar[idy] != 1) fprintf(stderr, "Erro na linha %d: %s nem vetor é, abensoado!\n", line_num+1, rem_fname(v_name[idy], fname));

    // ------------------------------------------------------------------------
    // atualiza status das variaveis ------------------------------------------
    // ------------------------------------------------------------------------

    v_used[idM] = 1;
    v_used[idv] = 1;

    // ------------------------------------------------------------------------
    // prepara variaveis locais -----------------------------------------------
    // ------------------------------------------------------------------------

    int N = v_size[idM];
    int M = v_siz2[idM];

    // ------------------------------------------------------------------------
    // implementa o produto entre matriz e vetor ------------------------------
    // ------------------------------------------------------------------------

    // int com int
    if ((v_type[idM] == 1) && (v_type[idv] == 1))
    {
        for (int i = 0; i < N; i++)
        {
            add_instr("LOD_V %s %d\n", v_name[idM], i*M);
            add_instr("MLT %s\n"     , v_name[idv]);

            for (int j = 1; j < M; j++)
            {
                add_instr("P_LOD_V %s %d\n", v_name[idM], i*M+j);
                add_instr(  "MLT_V %s %d\n", v_name[idv],     j);
                add_instr("S_ADD\n");
            }

            add_instr("SET_V %s %d\n", v_name[idy], i);
        }
    }

    // float com float
    if ((v_type[idM] == 2) && (v_type[idv] == 2))
    {
        for (int i = 0; i < N; i++)
        {
            add_instr("LOD_V %s %d\n", v_name[idM], i*M);
            add_instr("F_MLT %s\n"   , v_name[idv]);

            for (int j = 1; j < M; j++)
            {
                add_instr("P_LOD_V %s %d\n", v_name[idM], i*M+j);
                add_instr("F_MLT_V %s %d\n", v_name[idv],     j);
                add_instr("SF_ADD\n");
            }

            add_instr("SET_V %s %d\n", v_name[idy], i);
        }
    }
}

// multiplicacao do vetor por uma constante
void exec_cv(int idy, int et, int idv)
{
    int N = v_size[idv];

    char g[64];
    if (et%OFST==0)
    {
        add_instr("SET aux_var\n");
        strcpy(g,"aux_var");
    }
    else strcpy(g,v_name[et%OFST]);

    for (int i = 0; i < N; i++)
    {
        add_instr("LOD_V %s %d\n", v_name[idv], i);
        add_instr(   "F_MLT %s\n", g);
        add_instr("SET_V %s %d\n", v_name[idy], i);
    }
}

// soma ponderada no segundo vetor
void exec_apcb(int idy, int ida, int etc, int idb)
{
    int N = v_size[idy];

    char g[64];
    if (etc%OFST==0)
    {
        add_instr("SET aux_var\n");
         strcpy(g,"aux_var");
    }
    else strcpy(g,v_name[etc%OFST]);

    for (int i = 0; i < N; i++)
    {
        add_instr(  "LOD_V %s %d\n", v_name[idb], i);
        add_instr(     "F_MLT %s\n", g);
        add_instr("F_ADD_V %s %d\n", v_name[ida], i);
        add_instr(  "SET_V %s %d\n", v_name[idy], i);
    }
}

// produto externo entre dois vetores
void exec_vvt(int idM, int ida, int idb)
{
    int N = v_size[ida];

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            add_instr(  "LOD_V %s %d\n", v_name[ida],     i);
            add_instr("F_MLT_V %s %d\n", v_name[idb],     j);
            add_instr(  "SET_V %s %d\n", v_name[idM], N*j+i);
        }
    }
}

// subtracao de matriz com produto externo
void exec_Mmvvt(int idA, int idB, int ida, int idb)
{
    int N = v_size[ida];

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            add_instr(  "LOD_V %s %d\n", v_name[ida],     i);
            add_instr("F_MLT_V %s %d\n", v_name[idb],     j);
            add_instr("F_NEG\n");
            add_instr("F_ADD_V %s %d\n", v_name[idB], N*j+i);
            add_instr(  "SET_V %s %d\n", v_name[idA], N*j+i);
        }
    }
}

// produto entre constante e matriz
void exec_cM(int idA, int etc, int idM)
{
    int N = v_size[idM];
    int M = v_siz2[idM];

    char g[64];
    if (etc%OFST==0)
    {
        add_instr("SET aux_var\n");
         strcpy(g,"aux_var");
    }
    else strcpy(g,v_name[etc%OFST]);

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            add_instr("LOD_V %s %d\n", v_name[idM], M*i+j);
            add_instr("F_MLT %s\n", g);
            add_instr("SET_V %s %d\n", v_name[idA], M*i+j);
        }
    }
}

// gera matriz identidade
void exec_eye(int idM)
{
    int N = v_size[idM];

    // otimizar para nao ficar dando load toda hora
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (i == j) add_instr("LOD 1.0\n"); else add_instr("LOD 0.0\n");
            add_instr("SET_V %s %d\n", v_name[idM], N*i+j);
        }
    }
}

// gera vetor de zeros
void exec_v0(int idv)
{
    int N = v_size[idv];

    add_instr("LOD 0.0\n");
    for (int i = 0; i < N; i++) add_instr("SET_V %s %d\n", v_name[idv], i);
}