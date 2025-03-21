#include "..\Headers\oper.h"
#include "..\Headers\variaveis.h"
#include "..\Headers\t2t.h"
#include "..\Headers\diretivas.h"
#include "..\Headers\data_use.h"
#include "..\Headers\data_assign.h"
#include "..\Headers\stdlib.h"
#include "..\Headers\global.h"

#include <string.h>

// ----------------------------------------------------------------------------
// operacoes aritmeticas ------------------------------------------------------
// ----------------------------------------------------------------------------

// nega um numero
int oper_neg(int et)
{
    int  etr, eti;
    char num[128];

    char   ld[10]; if (acc_ok == 0) strcpy(  ld,  "LOD" ); else strcpy(ld,   "PLD"   );
    char  neg[10]; if (acc_ok == 0) strcpy( neg,  "NEGM"); else strcpy(neg,  "PNEGM" );
    char fneg[10]; if (acc_ok == 0) strcpy(fneg, "FNEGM"); else strcpy(fneg, "PFNEGM");

    // se for um int constante na memoria
    if ((get_type(et) == 1) && (v_isco[et % OFST] == 1) && (et % OFST != 0))
    {
        add_instr("%s -%s\n", ld, v_name[et % OFST]);
    }

    // se for um int variavel na memoria
    if ((get_type(et) == 1) && (v_isco[et % OFST] == 0) && (et % OFST != 0))
    {
        add_instr("%s %s\n", neg, v_name[et % OFST]);
    }

    // se for um int no acc
    if ((get_type(et) == 1) && (et % OFST == 0))
    {
        add_instr("NEG\n");
    }

    // se for um float constante na memoria
    if ((get_type(et) == 2) && (v_isco[et % OFST] == 1) && (et % OFST != 0))
    {
        sprintf  (num, "-%s", v_name[et % OFST]);
        add_instr("%s %u // %s\n", ld, f2mf(num), num);
    }

    // se for um float variavel na memoria
    if ((get_type(et) == 2) && (v_isco[et % OFST] == 0) && (et % OFST != 0))
    {
        add_instr("%s %s\n", fneg, v_name[et % OFST]);
    }

    // se for um float no acc
    if ((get_type(et) == 2) && (et % OFST == 0))
    {
        add_instr("FNEG\n");
    }

    // se for um comp constante na memoria
    if (get_type(et) == 5)
    {
        get_cmp_cst(et,&etr,&eti);

        // the real constant is never negative
        sprintf  (num, "-%s",       v_name[etr % OFST]);
        add_instr("%s %u // -%s\n", ld, f2mf(num), num);
        
        // the imaginary constant must be negated by code
        sprintf  (num, "%s",    v_name[eti % OFST]);
        add_instr("PFNEGM %u // %s\n", f2mf(num), num);
    }

    // se for um comp variavel na memoria
    if ((get_type(et) == 3) && (et % OFST != 0))
    {
        get_cmp_ets(et,&etr,&eti);

        add_instr("%s %s\n", fneg, v_name[etr % OFST]);
        add_instr("PFNEGM %s\n"  , v_name[eti % OFST]);
    }

    // se for um comp no acc
    if ((get_type(et) == 3) && (et % OFST == 0))
    {
        add_instr("SETP neg_aux\n");
        add_instr("FNEG\n");
        add_instr("PFNEGM neg_aux\n");
    }

    acc_ok = 1;
    if (get_type(et) == 5) et = 3*OFST;

    return get_type(et)*OFST;
}

// soma dois numeros
int oper_soma(int et1, int et2)
{
    int etr, eti;

    char ld [10]; if (acc_ok == 0) strcpy(ld,"LOD"); else strcpy(ld ,"PLD" );
    char i2f[10]; if (acc_ok == 0) strcpy(i2f,"IFM"); else strcpy(i2f,"PIFM");

    // int var com int var
    if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==1) && (et2%OFST!=0))
    {
        add_instr("%s %s\n" , ld, v_name[et1%OFST]);
        add_instr("ADD %s\n",     v_name[et2%OFST]);
    }

    // int var com int acc
    if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==1) && (et2%OFST==0))
    {
        add_instr("ADD %s\n", v_name[et1%OFST]);
    }

    // int var com float var
    if ((get_type(et1)==1) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==0))
    {
        add_instr("%s %s\n", i2f, v_name[et1%OFST]);
        add_instr("FADD %s\n"   , v_name[et2%OFST]);
    }

    // int const com float var
    if ((get_type(et1)==1) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==0))
    {
        add_instr("%s %u // %s\n", ld, f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
        add_instr("FADD %s\n"    ,          v_name[et2%OFST]);
    }

    // int var com float const
    if ((get_type(et1)==1) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
    {
        add_instr("%s %s\n" , i2f,        v_name[et1%OFST]);
        add_instr("FADD %d // %s\n", f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
    }

    // int const com float const
    if ((get_type(et1)==1) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
    {
        add_instr("%s %u // %s\n", ld, f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
        add_instr("FADD %d // %s\n"  , f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
    }

    // int var com float acc
    if ((get_type(et1)==1) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==2) && (et2%OFST==0))
    {
        add_instr("PIFM %s\n", v_name[et1%OFST]);
        add_instr("SFADD\n");
    }

    // int const com float acc
    if ((get_type(et1)==1) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==2) && (et2%OFST==0))
    {
        add_instr("%s %u // %s\n", ld, f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
        add_instr("SFADD\n");
    }

    // int var com comp const
    if ((get_type(et1)==1) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==5))
    {
        get_cmp_cst(et2,&etr,&eti);

        add_instr("%s %s\n" , i2f,        v_name[et1%OFST]);
        add_instr("FADD %u // %s\n", f2mf(v_name[etr%OFST]), v_name[etr%OFST]);
        add_instr("PLD  %d // %s\n", f2mf(v_name[eti%OFST]), v_name[eti%OFST]);
    }

    // int const com comp const
    if ((get_type(et1)==1) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==5))
    {
        get_cmp_cst(et2,&etr,&eti);

        add_instr("%s %u // %s\n", ld, f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
        add_instr("FADD %u // %s\n"  , f2mf(v_name[etr%OFST]), v_name[etr%OFST]);
        add_instr("PLD  %d // %s\n"  , f2mf(v_name[eti%OFST]), v_name[eti%OFST]);
    }

    // int var com comp var
    if ((get_type(et1)==1) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==3) && (et2%OFST!=0))
    {
        get_cmp_ets(et2,&etr,&eti);

        add_instr("%s %s\n", i2f, v_name[et1%OFST]);
        add_instr("FADD %s\n"   , v_name[etr%OFST]);
        add_instr("PLD  %s\n"   , v_name[eti%OFST]);
    }

    // int const com comp var
    if ((get_type(et1)==1) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==3) && (et2%OFST!=0))
    {
        get_cmp_ets(et2,&etr,&eti);

        add_instr("%s %s\n", i2f, v_name[et1%OFST]);
        add_instr("FADD %s\n"   , v_name[etr%OFST]);
        add_instr("PLD  %s\n"   , v_name[eti%OFST]);
    }

    // int var com comp acc
    if ((get_type(et1)==1) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==3) && (et2%OFST==0))
    {
        add_instr("SETP aux_soma\n");
        add_instr("PIFM %s\n", v_name[et1%OFST]);
        add_instr("SFADD\n");
        add_instr("PLD aux_soma\n");
    }

    // int const com comp acc
    if ((get_type(et1)==1) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==3) && (et2%OFST==0))
    {
        add_instr("SETP aux_soma\n");
        add_instr("PLD %u // %s\n", f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
        add_instr("SFADD\n");
        add_instr("PLD aux_soma\n");
    }

    // int acc com int var
    if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==1) && (et2%OFST!=0))
    {
        add_instr("ADD %s\n", v_name[et2%OFST]);
    }

    // int acc com int acc
    if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==1) && (et2%OFST==0))
    {
        add_instr("SADD\n");
    }

    // int acc com float var
    if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==0))
    {
        add_instr("IFA\n");
        add_instr("FADD %s\n", v_name[et2%OFST]);
    }

    // int acc com float const
    if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
    {
        add_instr("IFA\n");
        add_instr("FADD %d // %s\n", f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
    }

    // int acc com float acc
    if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST==0))
    {
        add_instr("SETP aux_soma\n");
        add_instr("IFA\n");
        add_instr("PLD aux_soma\n");
        add_instr("SFADD\n");
    }

    // int acc com comp const
    if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==5))
    {
        get_cmp_cst(et2,&etr,&eti);

        add_instr("IFA\n");
        add_instr("FADD %d // %s\n", f2mf(v_name[etr%OFST]), v_name[etr%OFST]);
        add_instr("PLD  %d // %s\n", f2mf(v_name[eti%OFST]), v_name[eti%OFST]);
    }

    // int acc com comp var
    if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==3) && (et2%OFST!=0))
    {
        get_cmp_ets(et2,&etr,&eti);

        add_instr("IFA\n");
        add_instr("FADD %s\n", v_name[etr%OFST]);
        add_instr("PLD %s\n", v_name[eti%OFST]);
    }

    // int acc com comp acc
    if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==3) && (et2%OFST==0))
    {
        add_instr("SETP aux_imag\n");
        add_instr("SETP aux_real\n");
        add_instr("IFA\n");
        add_instr("FADD aux_real\n");
        add_instr("PLD  aux_imag\n");
    }

    // float var com int var
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==1) && (et2%OFST!=0))
    {
        add_instr("%s %s\n",i2f, v_name[et2%OFST]);
        add_instr("FADD %s\n",   v_name[et1%OFST]);
    }

    // float var com int acc
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==1) && (et2%OFST==0))
    {
        add_instr("IFA\n");
        add_instr("FADD %s\n", v_name[et1%OFST]);
    }

    // float var com float var
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==0))
    {
        add_instr("%s %s\n", ld, v_name[et1%OFST]);
        add_instr("FADD %s\n"  , v_name[et2%OFST]);
    }

    // float var com float const
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
    {
        add_instr("%s %s\n", ld,          v_name[et1%OFST]);
        add_instr("FADD %u // %s\n", f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
    }

    // float var com float acc
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==2) && (et2%OFST==0))
    {
        add_instr("FADD %s\n", v_name[et1%OFST]);
    }

    // float var com comp const
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==5))
    {
        get_cmp_cst(et2,&etr,&eti);

        add_instr("%s %s\n", ld, v_name[et1%OFST]);
        add_instr("FADD %d // %s\n", f2mf(v_name[etr%OFST]), v_name[etr%OFST]);
        add_instr("PLD %d // %s\n" , f2mf(v_name[eti%OFST]), v_name[eti%OFST]);
    }

    // float var com comp var
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==3) && (et2%OFST!=0))
    {
        get_cmp_ets(et2,&etr,&eti);

        add_instr("%s %s\n", ld, v_name[et1%OFST]);
        add_instr("FADD %s\n"  , v_name[etr%OFST]);
        add_instr("PLD %s\n"   , v_name[eti%OFST]);
    }

    // float var com comp acc
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==3) && (et2%OFST==0))
    {
        add_instr("SETP aux_soma\n");
        add_instr("FADD %s\n", v_name[et1%OFST]);
        add_instr("PLD  aux_soma\n");
    }

    // float const com int var
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==1) && (et2%OFST!=0))
    {
        add_instr("%s %s\n", i2f,         v_name[et2%OFST]);
        add_instr("FADD %u // %s\n", f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
    }

    // float const com int acc
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==1) && (et2%OFST==0))
    {
        add_instr("IFA\n");
        add_instr("FADD %u // %s\n", f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
    }

    // float const com float var
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==0))
    {
        add_instr("%s %u // %s\n", ld, f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
        add_instr("FADD %s\n"    ,          v_name[et2%OFST]);
    }

    // float const com float const
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
    {
        add_instr("%s %u // %s\n", ld, f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
        add_instr("FADD %u // %s\n"  , f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
    }

    // float const com float acc
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==2) && (et2%OFST==0))
    {
        add_instr("FADD %u // %s\n", f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
    }

    // float const com comp const
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==5))
    {
        get_cmp_cst(et2,&etr,&eti);

        add_instr("%s %u // %s\n", ld, f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
        add_instr("FADD %u // %s\n"  , f2mf(v_name[etr%OFST]), v_name[etr%OFST]);
        add_instr("PLD %d // %s\n"   , f2mf(v_name[eti%OFST]), v_name[eti%OFST]);
    }

    // float const com comp var
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==3) && (et2%OFST!=0))
    {
        get_cmp_ets(et2,&etr,&eti);

        add_instr("%s %u // %s\n", ld, f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
        add_instr("FADD %s\n"    ,          v_name[etr%OFST]);
        add_instr("PLD  %s\n"    ,          v_name[eti%OFST]);
    }

    // float const com comp acc
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==3) && (et2%OFST==0))
    {
        add_instr("SETP aux_soma\n");
        add_instr("FADD %u // %s\n", f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
        add_instr("PLD  aux_soma\n");
    }

    // float acc com int var
    if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==1) && (et2%OFST!=0))
    {
        add_instr("PIFM %s\n", v_name[et2%OFST]);
        add_instr("SFADD\n");
    }

    // float acc com int acc
    if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==1) && (et2%OFST==0))
    {
        add_instr("IFA\n");
        add_instr("SFADD\n");
    }

    // float acc com float var
    if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==0))
    {
        add_instr("FADD %s\n", v_name[et2%OFST]);
    }

    // float acc com float const
    if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
    {
        add_instr("FADD %u // %s\n", f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
    }

    // float acc com float acc
    if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST==0))
    {
        add_instr("SFADD\n");
    }

    // float acc com comp const
    if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==5))
    {
        get_cmp_cst(et2,&etr,&eti);

        add_instr("FADD %u // %s\n", f2mf(v_name[etr%OFST]), v_name[etr%OFST]);
        add_instr("PLD  %u // %s\n", f2mf(v_name[eti%OFST]), v_name[eti%OFST]);
    }

    // float acc com comp var
    if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==3) && (et2%OFST!=0))
    {
        get_cmp_ets(et2,&etr,&eti);

        add_instr("FADD %s\n", v_name[etr%OFST]);
        add_instr("PLD  %s\n", v_name[eti%OFST]);
    }

    // float acc com comp acc
    if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==3) && (et2%OFST==0))
    {
        add_instr("SETP aux_imag\n");
        add_instr("SFADD\n");
        add_instr("PLD  aux_imag\n");
    }

    // comp const com int var
    if ((get_type(et1)==5) && (get_type(et2)==1) && (et2%OFST!=0) && (v_isco[et2%OFST]==0))
    {
        get_cmp_cst(et1,&etr,&eti);

        add_instr("%s %s\n", i2f, v_name[et2%OFST]);
        add_instr("FADD %u // %s\n", f2mf(v_name[etr%OFST]), v_name[etr%OFST]);
        add_instr("PLD  %u // %s\n", f2mf(v_name[eti%OFST]), v_name[eti%OFST]);
    }

    // comp const com int const
    if ((get_type(et1)==5) && (get_type(et2)==1) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
    {
        get_cmp_cst(et1,&etr,&eti);

        add_instr("%s  %u // %s\n", ld, f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
        add_instr("FADD %u // %s\n",    f2mf(v_name[etr%OFST]), v_name[etr%OFST]);
        add_instr("PLD  %u // %s\n",    f2mf(v_name[eti%OFST]), v_name[eti%OFST]);
    }

    // comp const com int acc
    if ((get_type(et1)==5) && (get_type(et2)==1) && (et2%OFST==0))
    {
        get_cmp_cst(et1,&etr,&eti);

        add_instr("IFA\n");
        add_instr("FADD %u // %s\n", f2mf(v_name[etr%OFST]), v_name[etr%OFST]);
        add_instr("PLD  %u // %s\n", f2mf(v_name[eti%OFST]), v_name[eti%OFST]);
    }

    // comp const com float var
    if ((get_type(et1)==5) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==0))
    {
        get_cmp_cst(et1,&etr,&eti);

        add_instr("%s %s\n", ld, v_name[et2%OFST]);
        add_instr("FADD %u // %s\n", f2mf(v_name[etr%OFST]), v_name[etr%OFST]);
        add_instr("PLD  %u // %s\n", f2mf(v_name[eti%OFST]), v_name[eti%OFST]);
    }

    // comp const com float const
    if ((get_type(et1)==5) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
    {
        get_cmp_cst(et1,&etr,&eti);

        add_instr("%s  %u // %s\n", ld, f2mf(v_name[etr%OFST]), v_name[etr%OFST]);
        add_instr("FADD %u // %s\n"   , f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
        add_instr("PLD  %u // %s\n"   , f2mf(v_name[eti%OFST]), v_name[eti%OFST]);
    }

    // comp const com float acc
    if ((get_type(et1)==5) && (get_type(et2)==2) && (et2%OFST==0))
    {
        get_cmp_cst(et1,&etr,&eti);

        add_instr("FADD %u // %s\n", f2mf(v_name[etr%OFST]), v_name[etr%OFST]);
        add_instr("PLD  %u // %s\n", f2mf(v_name[eti%OFST]), v_name[eti%OFST]);
    }

    // comp const com comp const
    if ((get_type(et1)==5) && (get_type(et2)==5))
    {
        int et1r,et2r;
        int et1i,et2i;

        get_cmp_cst(et1,&et1r,&et1i);
        get_cmp_cst(et2,&et2r,&et2i);

        add_instr("%s  %u // %s\n" ,ld, f2mf(v_name[et1r%OFST]), v_name[et1r%OFST]);
        add_instr("FADD %u // %s\n",    f2mf(v_name[et2r%OFST]), v_name[et2r%OFST]);

        add_instr("PLD  %u // %s\n",    f2mf(v_name[et1i%OFST]), v_name[et1i%OFST]);
        add_instr("FADD %u // %s\n",    f2mf(v_name[et2i%OFST]), v_name[et2i%OFST]);
    }

    // comp const com comp var
    if ((get_type(et1)==5) && (get_type(et2)==3) && (et2%OFST!=0))
    {
        int et1r,et2r;
        int et1i,et2i;

        get_cmp_cst(et1,&et1r,&et1i);
        get_cmp_ets    (et2,&et2r,&et2i);

        add_instr("%s %u // %s\n", ld, f2mf(v_name[et1r%OFST]), v_name[et1r%OFST]);
        add_instr("FADD %s\n"        ,      v_name[et2r%OFST]);

        add_instr("PLD %u // %s\n"   , f2mf(v_name[et1i%OFST]), v_name[et1i%OFST]);
        add_instr("FADD %s\n"        ,      v_name[et2i%OFST]);
    }

    // comp const com comp acc
    if ((get_type(et1)==5) && (get_type(et2)==3) && (et2%OFST==0))
    {
        get_cmp_cst(et1,&etr,&eti);

        add_instr("SETP aux_soma\n");
        add_instr("FADD %u // %s\n", f2mf(v_name[etr%OFST]), v_name[etr%OFST]);

        add_instr("PLD  %u // %s\n", f2mf(v_name[eti%OFST]), v_name[eti%OFST]);
        add_instr("FADD aux_soma\n");
    }

    // comp var com int var
    if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==1) && (et2%OFST!=0))
    {
        get_cmp_ets(et1,&etr,&eti);

        add_instr("%s  %s\n", i2f, v_name[et2%OFST]);
        add_instr("FADD %s\n"    , v_name[etr%OFST]);
        add_instr("PLD  %s\n"    , v_name[eti%OFST]);
    }

    // comp var com int acc
    if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==1) && (et2%OFST==0))
    {
        get_cmp_ets(et1,&etr,&eti);

        add_instr("IFA\n");
        add_instr("FADD %s\n", v_name[etr%OFST]);
        add_instr("PLD  %s\n", v_name[eti%OFST]);
    }

    // comp var com float var
    if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==0))
    {
        get_cmp_ets(et1,&etr,&eti);

        add_instr("%s %s\n", ld, v_name[et2%OFST]);
        add_instr("FADD %s\n"  , v_name[etr%OFST]);
        add_instr("PLD  %s\n"  , v_name[eti%OFST]);
    }

    // comp var com float const
    if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
    {
        get_cmp_ets(et1,&etr,&eti);

        add_instr("%s %u // %s\n", ld, f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
        add_instr("FADD %s\n"    ,          v_name[etr%OFST]);
        add_instr("PLD  %s\n"    ,          v_name[eti%OFST]);
    }

    // comp var com float acc
    if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==2) && (et2%OFST==0))
    {
        get_cmp_ets(et1,&etr,&eti);

        add_instr("FADD %s\n", v_name[etr%OFST]);
        add_instr("PLD  %s\n", v_name[eti%OFST]);
    }

    // comp var com comp const
    if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==5))
    {
        int et1r,et2r;
        int et1i,et2i;

        get_cmp_ets(et1,&et1r,&et1i);
        get_cmp_cst(et2,&et2r,&et2i);

        add_instr("%s %s\n", ld,          v_name[et1r%OFST]);
        add_instr("FADD %u // %s\n", f2mf(v_name[et2r%OFST]), v_name[et2r%OFST]);

        add_instr("PLD %s\n",             v_name[et1i%OFST]);
        add_instr("FADD %u // %s\n", f2mf(v_name[et2i%OFST]), v_name[et2i%OFST]);
    }

    // comp var com comp var
    if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==3) && (et2%OFST!=0))
    {
        int et1r,et2r;
        int et1i,et2i;

        get_cmp_ets(et1,&et1r,&et1i);
        get_cmp_ets(et2,&et2r,&et2i);

        add_instr("%s  %s\n", ld, v_name[et1r%OFST]);
        add_instr("FADD %s\n",    v_name[et2r%OFST]);

        add_instr("PLD  %s\n",    v_name[et1i%OFST]);
        add_instr("FADD %s\n",    v_name[et2i%OFST]);
    }

    // comp var com comp acc
    if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==3) && (et2%OFST==0))
    {
        get_cmp_ets(et1,&etr,&eti);

        add_instr("SETP aux_soma\n");
        add_instr("FADD %s\n", v_name[etr%OFST]);

        add_instr("PLD  %s\n", v_name[eti%OFST]);
        add_instr("FADD aux_soma\n");
    }

    // comp acc com int var
    if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==1) && (et2%OFST!=0))
    {
        add_instr("SETP aux_soma\n");
        add_instr("PIFM %s\n", v_name[et2%OFST]);
        add_instr("SFADD\n");
        add_instr("PLD  aux_soma\n");
    }

    // comp acc com int acc
    if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==1) && (et2%OFST==0))
    {
        add_instr("SETP aux_int\n" );
        add_instr("SETP aux_soma\n");
        add_instr("PIFM aux_int\n" );
        add_instr("SFADD\n");
        add_instr("PLD  aux_soma\n");
    }

    // comp acc com float var
    if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==0))
    {
        add_instr("SETP aux_soma\n");
        add_instr("FADD %s\n", v_name[et2%OFST]);
        add_instr("PLD  aux_soma\n");
    }

    // comp acc com float const
    if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
    {
        add_instr("SETP aux_soma\n");
        add_instr("FADD %u // %s\n", f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
        add_instr("PLD  aux_soma\n");
    }

    // comp acc com float acc
    if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST==0))
    {
        add_instr("SETP aux_int\n" );
        add_instr("SETP aux_soma\n");
        add_instr("FADD aux_int\n" );
        add_instr("PLD  aux_soma\n");
    }

    // comp acc com comp const
    if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==5))
    {
        get_cmp_cst(et2,&etr,&eti);

        add_instr("SETP aux_soma\n");
        add_instr("FADD %u // %s\n", f2mf(v_name[etr%OFST]), v_name[etr%OFST]);

        add_instr("PLD  aux_soma\n");
        add_instr("FADD %u // %s\n", f2mf(v_name[eti%OFST]), v_name[eti%OFST]);
    }

    // comp acc com comp var
    if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==3) && (et2%OFST!=0))
    {
        get_cmp_ets(et2,&etr,&eti);

        add_instr("SETP aux_soma\n");
        add_instr("FADD %s\n", v_name[etr%OFST]);

        add_instr("PLD aux_soma\n");
        add_instr("FADD %s\n", v_name[eti%OFST]);
    }

    // comp acc com comp acc
    if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==3) && (et2%OFST==0))
    {
        add_instr("SETP aux_imag2\n");
        add_instr("SETP aux_real2\n");
        add_instr("SETP aux_imag1\n");
        add_instr("FADD aux_real2\n");

        add_instr("PLD  aux_imag1\n");
        add_instr("FADD aux_imag2\n");
    }

    acc_ok = 1;

    int type;
         if ((get_type(et1) > 2) || (get_type(et2) > 2))
         type = 3;
    else if ((get_type(et1) > 1) || (get_type(et2) > 1))
         type = 2;
    else type = 1;

    return type*OFST;
}

// subtracao entre dos numeros
int oper_subt(int et1, int et2)
{
    return oper_soma(et1,oper_neg(et2));
}

// multiplica dois numeros
int oper_mult(int et1, int et2)
{
    int etr, eti;

    char ld [10]; if (acc_ok == 0) strcpy(ld ,"LOD"); else strcpy(ld ,"PLD" );
    char i2f[10]; if (acc_ok == 0) strcpy(i2f,"IFM"); else strcpy(i2f,"PIFM");

    // int var com int var
    if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==1) && (et2%OFST!=0))
    {
        add_instr("%s %s\n" , ld, v_name[et1%OFST]);
        add_instr("MLT %s\n",     v_name[et2%OFST]);
    }

    // int var com int acc
    if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==1) && (et2%OFST==0))
    {
        add_instr("MLT %s\n", v_name[et1%OFST]);
    }

    // int var com float var
    if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==0))
    {
        add_instr("%s %s\n", i2f, v_name[et1%OFST]);
        add_instr("FMLT %s\n"   , v_name[et2%OFST]);
    }

    // int var com float const
    if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
    {
        add_instr("%s %s\n" , i2f,        v_name[et1%OFST]);
        add_instr("FMLT %u // %s\n", f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
    }

    // int var com float acc
    if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==2) && (et2%OFST==0))
    {
        add_instr("PIFM %s\n", v_name[et1%OFST]);
        add_instr("SFMLT\n");
    }

    // int var com comp const
    if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==5))
    {
        get_cmp_cst(et2,&etr,&eti);

        oper_mult(et1,etr);
        oper_mult(et1,eti);
    }

    // int var com comp var
    if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==3) && (et2%OFST!=0))
    {
        get_cmp_ets(et2,&etr,&eti);

        oper_mult(et1,etr);
        oper_mult(et1,eti);
    }

    // int var com comp acc
    if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==3) && (et2%OFST==0))
    {
        add_instr("SETP aux_mult\n");
        oper_mult(et1,2*OFST);
        add_instr("PLD  aux_mult\n");
        oper_mult(et1,2*OFST);
    }

    // int acc com int var
    if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==1) && (et2%OFST!=0))
    {
        add_instr("MLT %s\n", v_name[et2%OFST]);
    }

    // int acc com int acc
    if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==1) && (et2%OFST==0))
    {
        add_instr("SMLT\n");
    }

    // int acc com float var
    if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==0))
    {
        add_instr("IFA\n");
        add_instr("FMLT %s\n", v_name[et2%OFST]);
    }

    // int acc com float const
    if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
    {
        add_instr("IFA\n");
        add_instr("FMLT %d // %s\n", f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
    }

    // int acc com float acc
    if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST==0))
    {
        add_instr("SETP aux_soma\n");
        add_instr("IFA\n");
        add_instr("FMLT aux_soma\n");
    }

    // int acc com comp const
    if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==5))
    {
        get_cmp_cst(et2,&etr,&eti);

        add_instr("SET aux_mult\n");
        oper_mult(et1,etr);
        add_instr("PLD aux_mult\n");
        oper_mult(et1,eti);
    }

    // int acc com comp var
    if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==3) && (et2%OFST!=0))
    {
        get_cmp_ets(et2,&etr,&eti);

        add_instr("SET aux_mult\n");
        oper_mult(et1,etr);
        add_instr("PLD aux_mult\n");
        oper_mult(et1,eti);
    }

    // int acc com comp acc
    if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==3) && (et2%OFST==0))
    {
        add_instr("SETP aux_imag\n");
        add_instr("SETP aux_real\n");
        add_instr("IFA\n");
        add_instr("SET  aux_mult\n");
        add_instr("FMLT aux_real\n");
        add_instr("PLD  aux_imag\n");
        add_instr("FMLT aux_mult\n");
    }

    // float var com int var
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==1) && (et2%OFST!=0))
    {
        add_instr("%s %s\n" ,i2f, v_name[et2%OFST]);
        add_instr("FMLT %s\n",    v_name[et1%OFST]);
    }

    // float var com int acc
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==1) && (et2%OFST==0))
    {
        add_instr("IFA\n");
        add_instr("FMLT %s\n", v_name[et1%OFST]);
    }

    // float var com float var
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==0))
    {
        add_instr("%s %s\n", ld, v_name[et1%OFST]);
        add_instr("FMLT %s\n",   v_name[et2%OFST]);
    }

    // float var com float const
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
    {
        add_instr("%s %s\n", ld,          v_name[et1%OFST]);
        add_instr("FMLT %d // %s\n", f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
    }

    // float var com float acc
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==2) && (et2%OFST==0))
    {
        add_instr("FMLT %s\n", v_name[et1%OFST]);
    }

    // float var com comp const
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==5))
    {
        get_cmp_cst(et2,&etr,&eti);

        oper_mult(et1,etr);
        oper_mult(et1,eti);
    }

    // float var com comp var
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==3) && (et2%OFST!=0))
    {
        get_cmp_ets(et2,&etr,&eti);

        oper_mult(et1,etr);
        oper_mult(et1,eti);
    }

    // float var com comp acc
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==3) && (et2%OFST==0))
    {
        add_instr("SETP aux_mult\n");
        oper_mult(et1,2*OFST);
        add_instr("PLD aux_mult\n");
        oper_mult(et1,2*OFST);
    }

    // float const com int var
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==1) && (et2%OFST!=0))
    {
        add_instr("%s %s\n", i2f,         v_name[et2%OFST]);
        add_instr("FMLT %u // %s\n", f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
    }

    // float const com int acc
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==1) && (et2%OFST==0))
    {
        add_instr("IFA\n");
        add_instr("FMLT %u // %s\n", f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
    }

    // float const com float var
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==0))
    {
        add_instr("%s %d // %s\n", ld, f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
        add_instr("FMLT %s\n"        ,      v_name[et2%OFST]);
    }

    // float const com float const
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
    {
        add_instr("%s %d // %s\n", ld, f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
        add_instr("FMLT %d // %s\n"  , f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
    }

    // float const com float acc
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==2) && (et2%OFST==0))
    {
        add_instr("FMLT %d // %s\n", f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
    }

    // float const com comp const
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==5))
    {
        get_cmp_cst(et2,&etr,&eti);

        oper_mult(et1,etr);
        oper_mult(et1,eti);
    }

    // float const com comp var
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==3) && (et2%OFST!=0))
    {
        get_cmp_ets(et2,&etr,&eti);

        oper_mult(et1,etr);
        oper_mult(et1,eti);
    }

    // float const com comp acc
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==3) && (et2%OFST==0))
    {
        add_instr("SETP aux_mult\n");
        oper_mult(et1,2*OFST);
        add_instr("PLD aux_mult\n");
        oper_mult(et1,2*OFST);
    }

    // float acc com int var
    if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==1) && (et2%OFST!=0))
    {
        add_instr("PIFM %s\n", v_name[et2%OFST]);
        add_instr("SFMLT\n");
    }

    // float acc com int acc
    if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==1) && (et2%OFST==0))
    {
        add_instr("IFA\n");
        add_instr("SFMLT\n");
    }

    // float acc com float var
    if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==0))
    {
        add_instr("FMLT %s\n", v_name[et2%OFST]);
    }

    // float acc com float const
    if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
    {
        add_instr("FMLT %d // %s\n", f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
    }

    // float acc com float acc
    if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST==0))
    {
        add_instr("SFMLT\n");
    }

    // float acc com comp const
    if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==5))
    {
        get_cmp_cst(et2,&etr,&eti);

        add_instr("SET aux_mult\n");
        oper_mult(et1,etr);
        add_instr("PLD aux_mult\n");
        oper_mult(et1,eti);
    }

    // float acc com comp var
    if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==3) && (et2%OFST!=0))
    {
        get_cmp_ets(et2,&etr,&eti);

        add_instr("SET aux_mult\n");
        oper_mult(et1,etr);
        add_instr("PLD aux_mult\n");
        oper_mult(et1,eti);
    }

    // float acc com comp acc
    if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==3) && (et2%OFST==0))
    {
        add_instr("SETP aux_imag\n");
        add_instr("SETP aux_real\n");
        add_instr("SET  aux_mult\n");
        add_instr("FMLT aux_real\n");
        add_instr("PLD  aux_imag\n");
        add_instr("FMLT aux_mult\n");
    }

    // comp const com int var
    if ((get_type(et1)==5) && (get_type(et2)==1) && (et2%OFST!=0))
    {
        get_cmp_cst(et1,&etr,&eti);

        oper_mult(etr,et2);
        oper_mult(eti,et2);
    }

    // comp const com int acc
    if ((get_type(et1)==5) && (get_type(et2)==1) && (et2%OFST==0))
    {
        get_cmp_cst(et1,&etr,&eti);

        add_instr("IFA\n");
        add_instr("SET aux_mult\n");
        oper_mult(etr,2*OFST);
        add_instr("PLD aux_mult\n");
        oper_mult(eti,2*OFST);
    }

    // comp const com float var
    if ((get_type(et1)==5) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==0))
    {
        get_cmp_cst(et1,&etr,&eti);

        oper_mult(etr,et2);
        oper_mult(eti,et2);
    }

    // comp const com float const
    if ((get_type(et1)==5) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
    {
        get_cmp_cst(et1,&etr,&eti);

        oper_mult(etr,et2);
        oper_mult(eti,et2);
    }

    // comp const com float acc
    if ((get_type(et1)==5) && (get_type(et2)==2) && (et2%OFST==0))
    {
        get_cmp_cst(et1,&etr,&eti);

        add_instr("SET aux_mult\n");
        oper_mult(etr,2*OFST);
        add_instr("PLD aux_mult\n");
        oper_mult(eti,2*OFST);
    }

    // comp const com comp const
    if ((get_type(et1)==5) && (get_type(et2)==5))
    {
        int et1r,et2r;
        int et1i,et2i;

        get_cmp_cst(et1,&et1r,&et1i);
        get_cmp_cst(et2,&et2r,&et2i);

        oper_mult(et1r,et2r);
        oper_mult(et1i,et2i);
        oper_subt(2*OFST,2*OFST);

        oper_mult(et1r,et2i);
        oper_mult(et1i,et2r);
        oper_soma(2*OFST,2*OFST);
    }

    // comp const com comp var
    if ((get_type(et1)==5) && (get_type(et2)==3) && (et2%OFST!=0))
    {
        int et1r,et2r;
        int et1i,et2i;

        get_cmp_cst(et1,&et1r,&et1i);
        get_cmp_ets(et2,&et2r,&et2i);

        oper_mult(et1r,et2r);
        oper_mult(et1i,et2i);
        oper_subt(2*OFST,2*OFST);

        oper_mult(et1r,et2i);
        oper_mult(et1i,et2r);
        oper_soma(2*OFST,2*OFST);
    }

    // comp const com comp acc
    if ((get_type(et1)==5) && (get_type(et2)==3) && (et2%OFST==0))
    {
        get_cmp_cst(et1,&etr,&eti);

        add_instr("SETP aux_imag\n");
        add_instr("SET aux_real\n");
        oper_mult(etr,2*OFST);
        add_instr("PLD aux_imag\n");
        oper_mult(eti,2*OFST);
        oper_subt(2*OFST,2*OFST);

        add_instr("PLD aux_imag\n");
        oper_mult(etr,2*OFST);
        add_instr("PLD aux_real\n");
        oper_mult(eti,2*OFST);
        oper_soma(2*OFST,2*OFST);
    }

    // comp var com int var
    if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==1) && (et2%OFST!=0))
    {
        get_cmp_ets(et1,&etr,&eti);

        oper_mult(etr,et2);
        oper_mult(eti,et2);
    }

    // comp var com int acc
    if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==1) && (et2%OFST==0))
    {
        get_cmp_ets(et1,&etr,&eti);

        add_instr("IFA\n");
        add_instr("SET aux_mult\n");
        oper_mult(etr,2*OFST);
        add_instr("PLD aux_mult\n");
        oper_mult(eti,2*OFST);
    }

    // comp var com float var
    if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==0))
    {
        get_cmp_ets(et1,&etr,&eti);

        oper_mult(etr,et2);
        oper_mult(eti,et2);
    }

    // comp var com float const
    if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
    {
        get_cmp_ets(et1,&etr,&eti);

        oper_mult(etr,et2);
        oper_mult(eti,et2);
    }

    // comp var com float acc
    if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==2) && (et2%OFST==0))
    {
        get_cmp_ets(et1,&etr,&eti);

        add_instr("SET aux_mult\n");
        oper_mult(etr,2*OFST);
        add_instr("PLD aux_mult\n");
        oper_mult(eti,2*OFST);
    }

    // comp var com comp const
    if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==5))
    {
        int et1r,et2r;
        int et1i,et2i;

        get_cmp_ets(et1,&et1r,&et1i);
        get_cmp_cst(et2,&et2r,&et2i);

        oper_mult(et1r,et2r);
        oper_mult(et1i,et2i);
        oper_subt(2*OFST,2*OFST);

        oper_mult(et1r,et2i);
        oper_mult(et1i,et2r);
        oper_soma(2*OFST,2*OFST);
    }

    // comp var com comp var
    if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==3) && (et2%OFST!=0))
    {
        int et1r,et2r;
        int et1i,et2i;

        get_cmp_ets(et1,&et1r,&et1i);
        get_cmp_ets(et2,&et2r,&et2i);

        oper_mult(et1r,et2r);
        oper_mult(et1i,et2i);
        oper_subt(2*OFST,2*OFST);

        oper_mult(et1r,et2i);
        oper_mult(et1i,et2r);
        oper_soma(2*OFST,2*OFST);
    }

    // comp var com comp acc
    if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==3) && (et2%OFST==0))
    {
        get_cmp_ets(et1,&etr,&eti);

        add_instr("SETP aux_imag\n");
        add_instr("SET aux_real\n");
        oper_mult(etr,2*OFST);
        add_instr("PLD aux_imag\n");
        oper_mult(eti,2*OFST);
        oper_subt(2*OFST,2*OFST);

        add_instr("PLD aux_imag\n");
        oper_mult(etr,2*OFST);
        add_instr("PLD aux_real\n");
        oper_mult(eti,2*OFST);
        oper_soma(2*OFST,2*OFST);
    }

    // comp acc com int var
    if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==1) && (et2%OFST!=0))
    {
        add_instr("SETP aux_imag\n");
        add_instr("PIFM %s\n", v_name[et2%OFST]);
        add_instr("SET  aux_float\n");
        add_instr("SFMLT\n");
        add_instr("PLD  aux_imag\n");
        add_instr("FMLT aux_float\n");
    }

    // comp acc com int acc
    if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==1) && (et2%OFST==0))
    {
        add_instr("IFA\n");
        add_instr("SETP aux_float\n");
        add_instr("SETP aux_imag\n");
        add_instr("FMLT aux_float\n");
        add_instr("PLD aux_imag\n");
        add_instr("FMLT aux_float\n");
    }

    // comp acc com float var
    if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==0))
    {
        add_instr("SETP aux_imag\n");
        add_instr("FMLT %s\n", v_name[et2%OFST]);
        add_instr("PLD aux_imag\n");
        add_instr("FMLT %s\n", v_name[et2%OFST]);
    }

    // comp acc com float const
    if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
    {
        add_instr("SETP aux_imag\n");
        add_instr("FMLT %d // %s\n", f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
        add_instr("PLD aux_imag\n");
        add_instr("FMLT %d // %s\n", f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
    }

    // comp acc com float acc
    if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST==0))
    {
        add_instr("SETP aux_float\n");
        add_instr("SETP aux_imag \n");
        add_instr("FMLT aux_float\n");
        add_instr("PLD  aux_imag \n");
        add_instr("FMLT aux_float\n");
    }

    // comp acc com comp const
    if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==5))
    {
        get_cmp_cst(et2,&etr,&eti);

        add_instr("SETP aux_imag\n");
        add_instr("SET aux_real\n");
        oper_mult(2*OFST,etr);
        add_instr("PLD aux_imag\n");
        oper_mult(2*OFST,eti);
        oper_subt(2*OFST,2*OFST);

        add_instr("PLD aux_imag\n");
        oper_mult(2*OFST,etr);
        add_instr("PLD aux_real\n");
        oper_mult(2*OFST,eti);
        oper_soma(2*OFST,2*OFST);
    }

    // comp acc com comp var
    if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==3) && (et2%OFST!=0))
    {
        get_cmp_ets(et2,&etr,&eti);

        add_instr("SETP aux_imag\n");
        add_instr("SET aux_real\n");
        oper_mult(2*OFST,etr);
        add_instr("PLD aux_imag\n");
        oper_mult(2*OFST,eti);
        oper_subt(2*OFST,2*OFST);

        add_instr("PLD aux_imag\n");
        oper_mult(2*OFST,etr);
        add_instr("PLD aux_real\n");
        oper_mult(2*OFST,eti);
        oper_soma(2*OFST,2*OFST);
    }

    // comp acc com comp acc
    if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==3) && (et2%OFST==0))
    {
        add_instr("SETP aux_imag2\n");
        add_instr("SETP aux_real2\n");
        add_instr("SETP aux_imag1\n");
        add_instr("SET  aux_real1\n");
        add_instr("PLD  aux_real2\n");
        oper_mult(2*OFST,2*OFST);
        add_instr("PLD  aux_imag1\n");
        add_instr("PLD  aux_imag2\n");
        oper_mult(2*OFST,2*OFST);
        oper_subt(2*OFST,2*OFST);

        add_instr("PLD  aux_imag1\n");
        add_instr("PLD  aux_real2\n");
        oper_mult(2*OFST,2*OFST);
        add_instr("PLD  aux_real1\n");
        add_instr("PLD  aux_imag2\n");
        oper_mult(2*OFST,2*OFST);
        oper_soma(2*OFST,2*OFST);
    }

    acc_ok = 1;

    int type;
         if ((get_type(et1) > 2) || (get_type(et2) > 2))
         type = 3;
    else if ((get_type(et1) > 1) || (get_type(et2) > 1))
         type = 2;
    else type = 1;

    return type*OFST;
}

// divisao entre dois numeros
int oper_divi(int et1, int et2)
{
    int etr, eti;

    char ld[10];
    if (acc_ok == 0) strcpy(ld,"LOD"); else strcpy(ld,"PLD");

    char i2f[10];
    if (acc_ok == 0) strcpy(i2f,"IFM"); else strcpy(i2f,"PIFM");

    // int var com int var
    if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==1) && (et2%OFST!=0))
    {
        add_instr("%s %s\n" , ld, v_name[et2%OFST]);
        add_instr("DIV %s\n",     v_name[et1%OFST]);
    }

    // int var com int acc
    if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==1) && (et2%OFST==0))
    {
        add_instr("DIV %s\n", v_name[et1%OFST]);
    }

    // int var com float var
    if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==0))
    {
        add_instr("%s %s\n" , i2f, v_name[et1%OFST]);
        add_instr("PLD %s\n" , v_name[et2%OFST]);
        add_instr("SFDIV\n");
    }

    // int var com float const
    if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
    {
        add_instr("%s %s\n" , i2f, v_name[et1%OFST]);
        add_instr("PLD %u // %s\n" , f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
        add_instr("SFDIV\n");
    }

    // int var com float acc
    if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==2) && (et2%OFST==0))
    {
        add_instr("SET aux_div\n");
        add_instr("IFM %s\n", v_name[et1%OFST]);
        add_instr("PLD aux_div\n");
        add_instr("SFDIV\n");
    }

    // int var com comp const
    if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==5))
    {
        get_cmp_cst(et2,&etr,&eti);

        oper_mult(etr,etr);         // parte real ao quadrado
        oper_mult(eti,eti);         // parte imag ao quadrado
        oper_soma(2*OFST,2*OFST);   // soma os quadrados
        add_instr("SET aux_cmp\n"); // salva o resultado
        acc_ok = 0;

        oper_mult(et1,etr);         // mult int com parte real
        add_instr("PLD aux_cmp\n"); // pega o denominador
        oper_divi(2*OFST,2*OFST);   // faz a divisao

        oper_mult(et1,eti);         // mult int com parte imag
        add_instr("PLD aux_cmp\n"); // pega o denominador
        oper_divi(2*OFST,2*OFST);   // faz a divisao
        oper_neg (2*OFST);
    }

    // int var com comp var
    if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==3) && (et2%OFST!=0))
    {
        get_cmp_ets(et2,&etr,&eti);

        oper_mult(etr,etr);         // parte real ao quadrado
        oper_mult(eti,eti);         // parte imag ao quadrado
        oper_soma(2*OFST,2*OFST);   // soma os quadrados
        add_instr("SET aux_cmp\n"); // salva o resultado
        acc_ok = 0;

        oper_mult(et1,etr);         // mult int com parte real
        add_instr("PLD aux_cmp\n"); // pega o denominador
        oper_divi(2*OFST,2*OFST);   // faz a divisao

        oper_mult(et1,eti);         // mult int com parte imag
        add_instr("PLD aux_cmp\n"); // pega o denominador
        oper_divi(2*OFST,2*OFST);   // faz a divisao
        oper_neg (2*OFST);
    }

    // int var com comp acc
    if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==3) && (et2%OFST==0))
    {
        add_instr("SETP aux_imag  \n");           // salva a parte imaginaria
        add_instr("SET  aux_real  \n");           // salva a parte real
        add_instr("FMLT aux_real  \n");
        add_instr("PLD  aux_imag  \n");           // pega a parte imaginaria
        add_instr("FMLT aux_imag  \n");
        add_instr("SFADD          \n");
        add_instr("SET  aux_cmp   \n");           // salva o modulo ao quadrado

        add_instr("IFM  %s\n", v_name[et1%OFST]); 
        add_instr("SET  aux_float \n");           // salva o float
        add_instr("FMLT aux_real  \n");
        add_instr("PLD  aux_cmp   \n");           // pega o modulo ao quadrado
        add_instr("SFDIV \n");                    // faz a divisao

        add_instr("PLD aux_float  \n");           // pega o float
        add_instr("FMLT aux_imag  \n");
        add_instr("PLD aux_cmp    \n");           // pega o modulo ao quadrado
        add_instr("SFDIV \n");                    // faz a divisao
        oper_neg (2*OFST);
    }

    // int acc com int var
    if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==1) && (et2%OFST!=0))
    {
        add_instr("%s %s\n", ld, v_name[et2%OFST]);
        add_instr("SDIV\n");
    }

    // int acc com int acc
    if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==1) && (et2%OFST==0))
    {
        add_instr("SDIV\n");
    }

    // int acc com float var
    if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==0))
    {
        add_instr("IFA\n");
        add_instr("PLD %s\n", v_name[et2%OFST]);
        add_instr("SFDIV\n");
    }

    // int acc com float const
    if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
    {
        add_instr("IFA\n");
        add_instr("PLD %u // %s\n", f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
        add_instr("SFDIV\n");
    }

    // int acc com float acc
    if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST==0))
    {
        add_instr("SETP aux_soma\n");
        add_instr("IFA\n");
        add_instr("PLD aux_soma\n");
        add_instr("SFDIV\n");
    }

    // int acc com comp const
    if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==5))
    {
        get_cmp_cst(et2,&etr,&eti);

        add_instr("IFA\n");
        add_instr("SET aux_float\n");
        acc_ok = 0;

        oper_mult(etr,etr);         // parte real ao quadrado
        oper_mult(eti,eti);         // parte imag ao quadrado
        oper_soma(2*OFST,2*OFST);   // soma os quadrados
        add_instr("SET aux_cmp\n"); // salva o resultado

        add_instr("LOD aux_float\n");
        oper_mult(2*OFST,etr);      // mult float com parte real
        add_instr("PLD aux_cmp\n"); // pega o denominador
        oper_divi(2*OFST,2*OFST);   // faz a divisao

        add_instr("PLD  aux_float\n");
        oper_mult(2*OFST,eti);      // mult float com parte imag
        add_instr("PLD aux_cmp\n"); // pega o denominador
        oper_divi(2*OFST,2*OFST);   // faz a divisao
        oper_neg (2*OFST);
    }

    // int acc com comp var
    if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==3) && (et2%OFST!=0))
    {
        get_cmp_ets(et2,&etr,&eti);

        add_instr("IFA\n");
        add_instr("SET aux_float\n");
        acc_ok = 0;

        oper_mult(etr,etr);         // parte real ao quadrado
        oper_mult(eti,eti);         // parte imag ao quadrado
        oper_soma(2*OFST,2*OFST);   // soma os quadrados
        add_instr("SET aux_cmp\n"); // salva o resultado

        add_instr("LOD aux_float\n");
        oper_mult(2*OFST,etr);      // mult float com parte real
        add_instr("PLD aux_cmp\n"); // pega o denominador
        oper_divi(2*OFST,2*OFST);   // faz a divisao

        add_instr("PLD  aux_float\n");
        oper_mult(2*OFST,eti);      // mult float com parte imag
        add_instr("PLD aux_cmp\n"); // pega o denominador
        oper_divi(2*OFST,2*OFST);   // faz a divisao
        oper_neg (2*OFST);
    }

    // int acc com comp acc
    if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==3) && (et2%OFST==0))
    {
        add_instr("SETP aux_imag  \n");           // salva a parte imaginaria
        add_instr("SETP aux_real  \n");           // salva a parte real
        add_instr("IFA \n");
        add_instr("SET  aux_float \n");           // salva o float

        add_instr("LOD aux_real  \n");           // coloca a parte real na pilha
        add_instr("FMLT aux_real  \n");
        add_instr("PLD  aux_imag  \n");           // pega a parte imaginaria
        add_instr("FMLT aux_imag  \n");
        add_instr("SFADD          \n");
        add_instr("SET  aux_cmp   \n");           // salva o modulo ao quadrado

        add_instr("LOD  aux_float\n");           // carrega o float
        add_instr("FMLT aux_real  \n");
        add_instr("PLD aux_cmp    \n");           // pega o modulo ao quadrado
        add_instr("SFDIV \n");                    // faz a divisao

        add_instr("PLD  aux_float \n");           // pega o float
        add_instr("FMLT aux_imag  \n");
        add_instr("PLD  aux_cmp   \n");           // pega o modulo ao quadrado
        add_instr("SFDIV \n");                    // faz a divisao
        oper_neg (2*OFST);
    }

    // float var com int var
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==1) && (et2%OFST!=0))
    {
        add_instr("%s %s\n", i2f, v_name[et2%OFST]);
        add_instr("FDIV %s\n"   , v_name[et1%OFST]);
    }

    // float var com int acc
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==1) && (et2%OFST==0))
    {
        add_instr("IFA\n");
        add_instr("FDIV %s\n", v_name[et1%OFST]);
    }

    // float var com float var
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==0))
    {
        add_instr("%s %s\n" , ld, v_name[et2%OFST]);
        add_instr("FDIV %s\n"   , v_name[et1%OFST]);
    }

    // float var com float const
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
    {
        add_instr("%s %d // %s\n", ld, f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
        add_instr("FDIV %s\n"    ,          v_name[et1%OFST]);
    }

    // float var com float acc
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==2) && (et2%OFST==0))
    {
        add_instr("FDIV %s\n", v_name[et1%OFST]);
    }

    // float var com comp const
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==5))
    {
        get_cmp_cst(et2,&etr,&eti);

        oper_mult(etr,etr);         // parte real ao quadrado
        oper_mult(eti,eti);         // parte imag ao quadrado
        oper_soma(2*OFST,2*OFST);   // soma os quadrados
        add_instr("SET aux_cmp\n"); // salva o resultado
        acc_ok = 0;

        oper_mult(et1,etr);         // mult float com parte real
        add_instr("PLD aux_cmp\n"); // pega o denominador
        oper_divi(2*OFST,2*OFST);   // faz a divisao

        oper_mult(et1,eti);         // mult float com parte imag
        add_instr("PLD aux_cmp\n"); // pega o denominador
        oper_divi(2*OFST,2*OFST);   // faz a divisao
        oper_neg (2*OFST);
    }

    // float var com comp var
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==3) && (et2%OFST!=0))
    {
        get_cmp_ets(et2,&etr,&eti);

        oper_mult(etr,etr);         // parte real ao quadrado
        oper_mult(eti,eti);         // parte imag ao quadrado
        oper_soma(2*OFST,2*OFST);   // soma os quadrados
        add_instr("SET aux_cmp\n"); // salva o resultado
        acc_ok = 0;

        oper_mult(et1,etr);         // mult float com parte real
        add_instr("PLD aux_cmp\n"); // pega o denominador
        oper_divi(2*OFST,2*OFST);   // faz a divisao

        oper_mult(et1,eti);         // mult float com parte imag
        add_instr("PLD aux_cmp\n"); // pega o denominador
        oper_divi(2*OFST,2*OFST);   // faz a divisao
        oper_neg (2*OFST);
    }

    // float var com comp acc
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==3) && (et2%OFST==0))
    {
        add_instr("SETP aux_imag  \n");           // salva a parte imaginaria
        add_instr("SET  aux_real  \n");           // salva a parte real
        add_instr("FMLT aux_real  \n");
        add_instr("PLD  aux_imag  \n");           // pega a parte imaginaria
        add_instr("FMLT aux_imag  \n");
        add_instr("SFADD          \n");
        add_instr("SET  aux_cmp   \n");           // salva o modulo ao quadrado

        add_instr("LOD %s\n", v_name[et1%OFST]); // carrega o float
        add_instr("FMLT aux_real  \n");
        add_instr("PLD aux_cmp    \n");           // pega o modulo ao quadrado
        add_instr("SFDIV \n");                    // faz a divisao

        add_instr("PLD %s\n", v_name[et1%OFST]);  // carrega o float
        add_instr("FMLT aux_imag  \n");
        add_instr("PLD aux_cmp    \n");           // pega o modulo ao quadrado
        add_instr("SFDIV \n");                    // faz a divisao
        oper_neg (2*OFST);
    }

    // float const com int var
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==1) && (et2%OFST!=0))
    {
        add_instr("%s %s\n", i2f   ,      v_name[et2%OFST]);
        add_instr("FDIV %u // %s\n", f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
    }

    // float const com int acc
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==1) && (et2%OFST==0))
    {
        add_instr("IFA\n");
        add_instr("FDIV %u // %s\n", f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
    }

    // float const com float var
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==0))
    {
        add_instr("%s %s\n", ld    ,      v_name[et2%OFST]);
        add_instr("FDIV %u // %s\n", f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
    }

    // float const com float const
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
    {
        add_instr("%s %u // %s\n", ld, f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
        add_instr("FDIV %u // %s\n"  , f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
    }

    // float const com float acc
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==2) && (et2%OFST==0))
    {
        add_instr("FDIV %u // %s\n", f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
    }

    // float const com comp const
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==5))
    {
        get_cmp_cst(et2,&etr,&eti);

        oper_mult(etr,etr);         // parte real ao quadrado
        oper_mult(eti,eti);         // parte imag ao quadrado
        oper_soma(2*OFST,2*OFST);   // soma os quadrados
        add_instr("SET aux_cmp\n"); // salva o resultado
        acc_ok = 0;

        oper_mult(et1,etr);         // mult float com parte real
        add_instr("PLD aux_cmp\n"); // pega o denominador
        oper_divi(2*OFST,2*OFST);   // faz a divisao

        oper_mult(et1,eti);         // mult float com parte imag
        add_instr("PLD aux_cmp\n"); // pega o denominador
        oper_divi(2*OFST,2*OFST);   // faz a divisao
        oper_neg (2*OFST);
    }

    // float const com comp var
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==3) && (et2%OFST!=0))
    {
        get_cmp_ets(et2,&etr,&eti);

        oper_mult(etr,etr);         // parte real ao quadrado
        oper_mult(eti,eti);         // parte imag ao quadrado
        oper_soma(2*OFST,2*OFST);   // soma os quadrados
        add_instr("SET aux_cmp\n"); // salva o resultado
        acc_ok = 0;

        oper_mult(et1,etr);         // mult float com parte real
        add_instr("PLD aux_cmp\n"); // pega o denominador
        oper_divi(2*OFST,2*OFST);   // faz a divisao

        oper_mult(et1,eti);         // mult float com parte imag
        add_instr("PLD aux_cmp\n"); // pega o denominador
        oper_divi(2*OFST,2*OFST);   // faz a divisao
        oper_neg (2*OFST);
    }

    // float const com comp acc
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==3) && (et2%OFST==0))
    {
        add_instr("SETP aux_imag  \n");           // salva a parte imaginaria
        add_instr("SET  aux_real  \n");           // salva a parte real
        add_instr("FMLT aux_real  \n");
        add_instr("PLD  aux_imag  \n");           // pega a parte imaginaria
        add_instr("FMLT aux_imag  \n");
        add_instr("SFADD          \n");
        add_instr("SET  aux_cmp   \n");           // salva o modulo ao quadrado

        add_instr("LOD %d // %s\n", f2mf(v_name[et1%OFST]), v_name[et1%OFST]); // carrega o float
        add_instr("FMLT aux_real  \n");
        add_instr("PLD aux_cmp    \n");           // pega o modulo ao quadrado
        add_instr("SFDIV \n");                    // faz a divisao

        add_instr("PLD %d // %s\n", f2mf(v_name[et1%OFST]), v_name[et1%OFST]); // carrega o float
        add_instr("FMLT aux_imag  \n");
        add_instr("PLD aux_cmp    \n");           // pega o modulo ao quadrado
        add_instr("SFDIV \n");                    // faz a divisao
        oper_neg (2*OFST);
    }

    // float acc com int var
    if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==1) && (et2%OFST!=0))
    {
        add_instr("PIFM %s\n", v_name[et2%OFST]);
        add_instr("SFDIV\n");
    }

    // float acc com int acc
    if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==1) && (et2%OFST==0))
    {
        add_instr("IFA\n");
        add_instr("SFDIV\n");
    }

    // float acc com float var
    if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==0))
    {
        add_instr("PLD %s\n", v_name[et2%OFST]);
        add_instr("SFDIV\n");
    }

    // float acc com float const
    if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
    {
        add_instr("PLD %d // %s\n", f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
        add_instr("SFDIV\n");
    }

    // float acc com float acc
    if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST==0))
    {
        add_instr("SFDIV\n");
    }

    // float acc com comp const
    if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==5))
    {
        get_cmp_cst(et2,&etr,&eti);

        add_instr("SET aux_float\n");
        acc_ok = 0;

        oper_mult(etr,etr);         // parte real ao quadrado
        oper_mult(eti,eti);         // parte imag ao quadrado
        oper_soma(2*OFST,2*OFST);   // soma os quadrados
        add_instr("SET aux_cmp\n"); // salva o resultado

        add_instr("LOD aux_float\n");
        oper_mult(2*OFST,etr);      // mult float com parte real
        add_instr("PLD aux_cmp\n"); // pega o denominador
        oper_divi(2*OFST,2*OFST);   // faz a divisao

        add_instr("PLD  aux_float\n");
        oper_mult(2*OFST,eti);      // mult float com parte imag
        add_instr("PLD aux_cmp\n"); // pega o denominador
        oper_divi(2*OFST,2*OFST);   // faz a divisao
        oper_neg (2*OFST);
    }

    // float acc com comp var
    if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==3) && (et2%OFST!=0))
    {
        get_cmp_ets(et2,&etr,&eti);

        add_instr("SET aux_float\n");
        acc_ok = 0;

        oper_mult(etr,etr);         // parte real ao quadrado
        oper_mult(eti,eti);         // parte imag ao quadrado
        oper_soma(2*OFST,2*OFST);   // soma os quadrados
        add_instr("SET aux_cmp\n"); // salva o resultado

        add_instr("LOD aux_float\n");
        oper_mult(2*OFST,etr);      // mult float com parte real
        add_instr("PLD aux_cmp\n"); // pega o denominador
        oper_divi(2*OFST,2*OFST);   // faz a divisao

        add_instr("PLD  aux_float\n");
        oper_mult(2*OFST,eti);      // mult float com parte imag
        add_instr("PLD aux_cmp\n"); // pega o denominador
        oper_divi(2*OFST,2*OFST);   // faz a divisao
        oper_neg (2*OFST);
    }

    // float acc com comp acc
    if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==3) && (et2%OFST==0))
    {
        add_instr("SETP aux_imag  \n");           // salva a parte imaginaria
        add_instr("SETP aux_real  \n");           // salva a parte real
        add_instr("SET  aux_float \n");           // salva o float

        add_instr("LOD aux_real  \n");           // coloca a parte real na pilha
        add_instr("FMLT aux_real  \n");
        add_instr("PLD  aux_imag  \n");           // pega a parte imaginaria
        add_instr("FMLT aux_imag  \n");
        add_instr("SFADD          \n");
        add_instr("SET  aux_cmp   \n");           // salva o modulo ao quadrado

        add_instr("LOD  aux_float\n");           // carrega o float
        add_instr("FMLT aux_real  \n");
        add_instr("PLD aux_cmp    \n");           // pega o modulo ao quadrado
        add_instr("SFDIV \n");                    // faz a divisao

        add_instr("PLD aux_float  \n");           // pega o float
        add_instr("FMLT aux_imag  \n");
        add_instr("PLD aux_cmp    \n");           // pega o modulo ao quadrado
        add_instr("SFDIV \n");                    // faz a divisao
        oper_neg (2*OFST);
    }

    // comp const com int var
    if ((get_type(et1)==5) && (get_type(et2)==1) && (et2%OFST!=0))
    {
        get_cmp_cst(et1,&etr,&eti);

        oper_divi(etr,et2);
        oper_divi(eti,et2);
    }

    // comp const com int acc
    if ((get_type(et1)==5) && (get_type(et2)==1) && (et2%OFST==0))
    {
        get_cmp_cst(et1,&etr,&eti);

        add_instr("IFA\n");
        add_instr("SET aux_div\n");
        oper_divi(etr,2*OFST);
        add_instr("PLD aux_div\n");
        oper_divi(eti,2*OFST);
    }

    // comp const com float var
    if ((get_type(et1)==5) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==0))
    {
        get_cmp_cst(et1,&etr,&eti);

        oper_divi(etr,et2);
        oper_divi(eti,et2);
    }

    // comp const com float const
    if ((get_type(et1)==5) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
    {
        get_cmp_cst(et1,&etr,&eti);

        oper_divi(etr,et2);
        oper_divi(eti,et2);
    }

    // comp const com float acc
    if ((get_type(et1)==5) && (get_type(et2)==2) && (et2%OFST==0))
    {
        get_cmp_cst(et1,&etr,&eti);

        add_instr("SET aux_mult\n");
        oper_divi(etr,2*OFST);
        add_instr("PLD aux_mult\n");
        oper_divi(eti,2*OFST);
    }

    // comp const com comp const
    if ((get_type(et1)==5) && (get_type(et2)==5))
    {
        int et1r,et2r;
        int et1i,et2i;

        get_cmp_cst(et1,&et1r,&et1i);
        get_cmp_cst(et2,&et2r,&et2i);

        oper_mult(et2r,et2r);
        oper_mult(et2i,et2i);
        oper_soma(2*OFST,2*OFST);
        add_instr("SET aux_mod\n");
        acc_ok = 0;

        oper_mult(et1r,et2r);
        oper_mult(et1i,et2i);
        oper_soma(2*OFST,2*OFST);
        add_instr("PLD aux_mod\n");
        oper_divi(2*OFST,2*OFST);

        oper_mult(et1i,et2r);
        oper_mult(et1r,et2i);
        oper_subt(2*OFST,2*OFST);
        add_instr("PLD aux_mod\n");
        oper_divi(2*OFST,2*OFST);
    }

    // comp const com comp var
    if ((get_type(et1)==5) && (get_type(et2)==3) && (et2%OFST!=0))
    {
        int et1r,et2r;
        int et1i,et2i;

        get_cmp_cst(et1,&et1r,&et1i);
        get_cmp_ets    (et2,&et2r,&et2i);

        oper_mult(et2r,et2r);
        oper_mult(et2i,et2i);
        oper_soma(2*OFST,2*OFST);
        add_instr("SET aux_mod\n");
        acc_ok = 0;

        oper_mult(et1r,et2r);
        oper_mult(et1i,et2i);
        oper_soma(2*OFST,2*OFST);
        add_instr("PLD aux_mod\n");
        oper_divi(2*OFST,2*OFST);

        oper_mult(et1i,et2r);
        oper_mult(et1r,et2i);
        oper_subt(2*OFST,2*OFST);
        add_instr("PLD aux_mod\n");
        oper_divi(2*OFST,2*OFST);
    }

    // comp const com comp acc
    if ((get_type(et1)==5) && (get_type(et2)==3) && (et2%OFST==0))
    {
        get_cmp_cst(et1,&etr,&eti);

        add_instr("SETP aux_imag\n");
        add_instr("SET  aux_real\n");
        add_instr("FMLT aux_real\n");
        add_instr("PLD  aux_imag\n");
        add_instr("FMLT aux_imag\n");
        oper_soma(2*OFST,2*OFST);
        add_instr("SET aux_mod\n");

        add_instr("LOD aux_real\n");
        oper_mult(etr,2*OFST);
        add_instr("PLD  aux_imag\n");
        oper_mult(eti,2*OFST);
        oper_soma(2*OFST,2*OFST);
        add_instr("PLD aux_mod\n");
        oper_divi(2*OFST,2*OFST);

        add_instr("PLD aux_real\n");
        oper_mult(eti,2*OFST);
        add_instr("PLD  aux_imag\n");
        oper_mult(etr,2*OFST);
        oper_subt(2*OFST,2*OFST);
        add_instr("PLD aux_mod\n");
        oper_divi(2*OFST,2*OFST);
    }

    // comp var com int var
    if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==1) && (et2%OFST!=0))
    {
        get_cmp_ets(et1,&etr,&eti);

        oper_divi(etr,et2);
        oper_divi(eti,et2);
    }

    // comp var com int acc
    if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==1) && (et2%OFST==0))
    {
        get_cmp_ets(et1,&etr,&eti);

        add_instr("IFA\n");
        add_instr("SET aux_mult\n");
        oper_divi(etr,2*OFST);
        add_instr("PLD aux_mult\n");
        oper_divi(eti,2*OFST);
    }

    // comp var com float var
    if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==0))
    {
        get_cmp_ets(et1,&etr,&eti);

        oper_divi(etr,et2);
        oper_divi(eti,et2);
    }

    // comp var com float const
    if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
    {
        get_cmp_ets(et1,&etr,&eti);

        oper_divi(etr,et2);
        oper_divi(eti,et2);
    }

    // comp var com float acc
    if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==2) && (et2%OFST==0))
    {
        get_cmp_ets(et1,&etr,&eti);

        add_instr("SET aux_mult\n");
        oper_divi(etr,2*OFST);
        add_instr("PLD aux_mult\n");
        oper_divi(eti,2*OFST);
    }

    // comp var com comp const
    if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==5))
    {
        int et1r,et2r;
        int et1i,et2i;

        get_cmp_ets    (et1,&et1r,&et1i);
        get_cmp_cst(et2,&et2r,&et2i);

        oper_mult(et2r,et2r);
        oper_mult(et2i,et2i);
        oper_soma(2*OFST,2*OFST);
        add_instr("SET aux_mod\n");
        acc_ok = 0;

        oper_mult(et1r,et2r);
        oper_mult(et1i,et2i);
        oper_soma(2*OFST,2*OFST);
        add_instr("PLD aux_mod\n");
        oper_divi(2*OFST,2*OFST);

        oper_mult(et1i,et2r);
        oper_mult(et1r,et2i);
        oper_subt(2*OFST,2*OFST);
        add_instr("PLD aux_mod\n");
        oper_divi(2*OFST,2*OFST);
    }

    // comp var com comp var
    if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==3) && (et2%OFST!=0))
    {
        int et1r,et2r;
        int et1i,et2i;

        get_cmp_ets(et1,&et1r,&et1i);
        get_cmp_ets(et2,&et2r,&et2i);

        oper_mult(et2r,et2r);
        oper_mult(et2i,et2i);
        oper_soma(2*OFST,2*OFST);
        add_instr("SET aux_mod\n");
        acc_ok = 0;

        oper_mult(et1r,et2r);
        oper_mult(et1i,et2i);
        oper_soma(2*OFST,2*OFST);
        add_instr("PLD aux_mod\n");
        oper_divi(2*OFST,2*OFST);

        oper_mult(et1i,et2r);
        oper_mult(et1r,et2i);
        oper_subt(2*OFST,2*OFST);
        add_instr("PLD aux_mod\n");
        oper_divi(2*OFST,2*OFST);
    }

    // comp var com comp acc
    if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==3) && (et2%OFST==0))
    {
        get_cmp_ets(et1,&etr,&eti);

        add_instr("SETP aux_imag\n");
        add_instr("SET  aux_real\n");
        add_instr("FMLT aux_real\n");
        add_instr("PLD  aux_imag\n");
        add_instr("FMLT aux_imag\n");
        oper_soma(2*OFST,2*OFST);
        add_instr("SET aux_mod\n");

        add_instr("LOD aux_real\n");
        oper_mult(etr,2*OFST);
        add_instr("PLD  aux_imag\n");
        oper_mult(eti,2*OFST);
        oper_soma(2*OFST,2*OFST);
        add_instr("PLD aux_mod\n");
        oper_divi(2*OFST,2*OFST);

        add_instr("PLD aux_real\n");
        oper_mult(eti,2*OFST);
        add_instr("PLD  aux_imag\n");
        oper_mult(etr,2*OFST);
        oper_subt(2*OFST,2*OFST);
        add_instr("PLD aux_mod\n");
        oper_divi(2*OFST,2*OFST);
    }

    // comp acc com int var
    if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==1) && (et2%OFST!=0))
    {
        add_instr("SETP aux_imag\n");
        add_instr("PIFM %s\n", v_name[et2%OFST]);
        add_instr("SET aux_float\n");
        add_instr("SFDIV\n");
        add_instr("PLD aux_imag\n");
        add_instr("PLD aux_float\n");
        add_instr("SFDIV\n");
    }

    // comp acc com int acc
    if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==1) && (et2%OFST==0))
    {
        add_instr("IFA\n");
        add_instr("SETP aux_float\n");
        add_instr("SETP aux_imag\n");
        add_instr("PLD aux_float\n");
        add_instr("SFDIV\n");
        add_instr("PLD aux_imag\n");
        add_instr("PLD aux_float\n");
        add_instr("SFDIV\n");
    }

    // comp acc com float var
    if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==0))
    {
        add_instr("SETP aux_imag\n");
        add_instr("PLD %s\n", v_name[et2%OFST]);
        add_instr("SFDIV\n");
        add_instr("PLD aux_imag\n");
        add_instr("PLD %s\n", v_name[et2%OFST]);
        add_instr("SFDIV\n");
    }

    // comp acc com float const
    if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
    {
        add_instr("SETP aux_imag\n");
        add_instr("PLD %d // %s\n", f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
        add_instr("SFDIV\n");
        add_instr("PLD aux_imag\n");
        add_instr("PLD %d // %s\n", f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
        add_instr("SFDIV\n");
    }

    // comp acc com float acc
    if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST==0))
    {
        add_instr("SETP aux_float\n");
        add_instr("SETP aux_imag\n");
        add_instr("PLD aux_float\n");
        add_instr("SFDIV\n");
        add_instr("PLD aux_imag\n");
        add_instr("PLD aux_float\n");
        add_instr("SFDIV\n");
    }

    // comp acc com comp const
    if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==5))
    {
        get_cmp_cst(et2,&etr,&eti);

        add_instr("SETP aux_imag\n");
        add_instr("SET  aux_real\n");
        acc_ok = 0;

        oper_mult(etr,etr);
        oper_mult(eti,eti);
        oper_soma(2*OFST,2*OFST);
        add_instr("SET aux_mod\n");

        add_instr("LOD aux_real\n");
        oper_mult(etr,2*OFST);
        add_instr("PLD  aux_imag\n");
        oper_mult(eti,2*OFST);
        oper_soma(2*OFST,2*OFST);
        add_instr("PLD aux_mod\n");
        oper_divi(2*OFST,2*OFST);

        add_instr("PLD aux_real\n");
        oper_mult(eti,2*OFST);
        add_instr("PLD  aux_imag\n");
        oper_mult(etr,2*OFST);
        oper_subt(2*OFST,2*OFST);
        add_instr("PLD aux_mod\n");
        oper_divi(2*OFST,2*OFST);
    }

    // comp acc com comp var
    if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==3) && (et2%OFST!=0))
    {
        get_cmp_ets(et2,&etr,&eti);

        add_instr("SETP aux_imag\n");
        add_instr("SET  aux_real\n");
        acc_ok = 0;

        oper_mult(etr,etr);
        oper_mult(eti,eti);
        oper_soma(2*OFST,2*OFST);
        add_instr("SET aux_mod\n");

        add_instr("LOD aux_real\n");
        oper_mult(etr,2*OFST);
        add_instr("PLD  aux_imag\n");
        oper_mult(eti,2*OFST);
        oper_soma(2*OFST,2*OFST);
        add_instr("PLD aux_mod\n");
        oper_divi(2*OFST,2*OFST);

        add_instr("PLD aux_real\n");
        oper_mult(eti,2*OFST);
        add_instr("PLD  aux_imag\n");
        oper_mult(etr,2*OFST);
        oper_subt(2*OFST,2*OFST);
        add_instr("PLD aux_mod\n");
        oper_divi(2*OFST,2*OFST);
    }

    // comp acc com comp acc
    if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==3) && (et2%OFST==0))
    {
        add_instr("SETP aux_imag2 \n");
        add_instr("SETP aux_real2 \n");
        add_instr("SETP aux_imag1 \n");
        add_instr("SET  aux_real1 \n");

        add_instr("LOD aux_real2 \n");
        add_instr("FMLT aux_real2 \n");
        add_instr("PLD  aux_imag2\n");
        add_instr("FMLT aux_imag2\n");
        add_instr("SFADD          \n");
        add_instr("SET aux_mod    \n");

        add_instr("LOD aux_real1 \n");
        add_instr("FMLT aux_real2 \n");
        add_instr("PLD  aux_imag1 \n");
        add_instr("FMLT aux_imag2 \n");
        add_instr("SFADD          \n");
        add_instr("PLD  aux_mod   \n");
        add_instr("SFDIV\n");

        add_instr("PLD  aux_real1 \n");
        add_instr("FMLT aux_imag2 \n");
        add_instr("PLD  aux_imag1 \n");
        add_instr("FMLT aux_real2 \n");
        oper_subt(2*OFST,2*OFST);
        add_instr("PLD  aux_mod   \n");
        add_instr("SFDIV\n");
    }

    acc_ok = 1;

    int type;
         if ((get_type(et1) > 2) || (get_type(et2) > 2))
         type = 3;
    else if ((get_type(et1) > 1) || (get_type(et2) > 1))
         type = 2;
    else type = 1;

    return type*OFST;
}

// resto da divisao
int oper_mod(int et1, int et2)
{
    if ((get_type(et1) > 1) || (get_type(et2) > 1))
        fprintf(stderr, "Erro na linha %d: qual o sentido de calcular o resto da divisão sem ser com número inteiro? Vai se tratar!\n", line_num+1);

    char ld[10];
    if (acc_ok == 0) strcpy(ld,"LOD"); else strcpy(ld,"PLD");

    // int var com int var
    if ((get_type(et1) == 1) && (et1%OFST != 0) && (get_type(et2) == 1) && (et2%OFST != 0))
    {
        add_instr("%s %s\n" , ld, v_name[et2%OFST]);
        add_instr("MOD %s\n",     v_name[et1%OFST]);
    }

    // int var com int acc
    if ((get_type(et1) == 1) && (et1%OFST != 0) && (get_type(et2) == 1) && (et2%OFST == 0))
    {
        add_instr("MOD %s\n", v_name[et1%OFST]);
    }

    // int acc com int var
    if ((get_type(et1) == 1) && (et1%OFST == 0) && (get_type(et2) == 1) && (et2%OFST != 0))
    {
        add_instr("PLD %s\n", v_name[et2%OFST]);
        add_instr("SMOD\n");
    }

    // int acc com int acc
    if ((get_type(et1) == 1) && (et1%OFST == 0) && (get_type(et2) == 1) && (et2%OFST == 0))
    {
        add_instr("SMOD\n");
    }

    acc_ok = 1;

    return OFST; // retorna o id extendido de int
}

// ----------------------------------------------------------------------------
// operacoes de comparacao ----------------------------------------------------
// ----------------------------------------------------------------------------

// compara maior, menor e igual (separar o igual pra economizar clock)
int oper_cmp(int et1, int et2, int type)
{
    int  etr, eti;

    char ld [10]; if (acc_ok == 0) strcpy(ld, "LOD"); else strcpy(ld, "PLD" );
    char i2f[10]; if (acc_ok == 0) strcpy(i2f,"IFM"); else strcpy(i2f,"PIFM");

    char op[16];
    switch (type)
    {
        case 0: strcpy(op, "LES"); break;
        case 1: strcpy(op, "GRE"); break;
        case 2: strcpy(op, "EQU"); break;
    }

    char fop[16];
    switch (type)
    {
        case 0: strcpy(fop, "FLES"); break;
        case 1: strcpy(fop, "FGRE"); break;
        case 2: strcpy(fop, "EQU" ); break;
    }

    // int var com int var
    if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==1) && (et2%OFST!=0))
    {
        add_instr("%s %s\n", ld, v_name[et2%OFST]);
        add_instr("%s %s\n", op, v_name[et1%OFST]);
    }

    // int var com int acc
    if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==1) && (et2%OFST==0))
    {
        add_instr("%s %s\n", op, v_name[et1%OFST]);
    }

    // int var com float var
    if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==0))
    {
        add_instr("%s %s\n", i2f, v_name[et1%OFST]);
        add_instr("PLD %s\n"    , v_name[et2%OFST]);
        add_instr("S%s\n", fop);
    }

    // int var com float const
    if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
    {
        add_instr("%s %s\n", i2f   ,      v_name[et1%OFST]);
        add_instr("PLD %u // %s\n" , f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
        add_instr("S%s\n", fop);
    }

    // int var com float acc
    if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==2) && (et2%OFST==0))
    {
        add_instr("SET aux_cmp\n");
        add_instr("IFM %s\n", v_name[et1%OFST]);
        add_instr("PLD aux_cmp\n");
        add_instr("S%s\n", fop);
    }

    // int var com comp const
    if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==5))
    {
        fprintf(stdout, "Atenção na linha %d: comparando int com comp? Vou pegar o módulo.\n", line_num+1);

        oper_mult(et1,et1);
        exec_sqr2(et2);
        oper_cmp (OFST,2*OFST,type);
    }

    // int var com comp var
    if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==3) && (et2%OFST!=0))
    {
        fprintf(stdout, "Atenção na linha %d: comparando int com comp? Vou pegar o módulo.\n", line_num+1);

        oper_mult(et1,et1);
        exec_sqr2(et2);
        oper_cmp (OFST,2*OFST,type);
    }

    // int var com comp acc
    if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==3) && (et2%OFST==0))
    {
        fprintf(stdout, "Atenção na linha %d: comparando int com comp? Vou pegar o módulo.\n", line_num+1);

        add_instr("SETP aux_cmpi\n");
        add_instr("SET  aux_cmpr\n");
        acc_ok = 0;

        oper_mult(et1,et1);
        add_instr("PLD aux_cmpr\n");
        add_instr("PLD aux_cmpi\n");
        exec_sqr2(3*OFST);
        oper_cmp (OFST,2*OFST,type);
    }

    // int acc com int var
    if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==1) && (et2%OFST!=0))
    {
        add_instr("%s %s\n", ld, v_name[et2%OFST]);
        add_instr("S%s\n", op);
    }

    // int acc com int acc
    if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==1) && (et2%OFST==0))
    {
        add_instr("S%s\n", op);
    }

    // int acc com float var
    if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==0))
    {
        add_instr("IFA\n");
        add_instr("PLD %s\n", v_name[et2%OFST]);
        add_instr("S%s\n", fop);
    }

    // int acc com float const
    if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
    {
        add_instr("IFA\n");
        add_instr("PLD %d // %s\n", f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
        add_instr("S%s\n", fop);
    }

    // int acc com float acc
    if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST==0))
    {
        add_instr("SETP aux_cmp\n");
        add_instr("IFA\n");
        add_instr("PLD aux_cmp\n");
        add_instr("S%s\n", fop);
    }

    // int acc com comp const
    if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==5))
    {
        fprintf(stdout, "Atenção na linha %d: comparando int com comp? Vou pegar o módulo.\n", line_num+1);

        add_instr("SET aux_cmp\n");
        add_instr("PLD aux_cmp\n");
        oper_mult(et1,et1);
        exec_sqr2(et2);
        oper_cmp (OFST,2*OFST,type);
    }

    // int acc com comp var
    if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==3) && (et2%OFST!=0))
    {
        fprintf(stdout, "Atenção na linha %d: comparando int com comp? Vou pegar o módulo.\n", line_num+1);

        add_instr("SET aux_cmp\n");
        add_instr("PLD aux_cmp\n");
        oper_mult(et1,et1);
        exec_sqr2(et2);
        oper_cmp (OFST,2*OFST,type);
    }

    // int acc com comp acc
    if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==3) && (et2%OFST==0))
    {
        fprintf(stdout, "Atenção na linha %d: comparando int com comp? Vou pegar o módulo.\n", line_num+1);

        add_instr("SETP aux_cmpi\n");
        add_instr("SETP aux_cmpr\n");
        add_instr("SET  aux_cmp \n");

        add_instr("PLD aux_cmp\n");
        oper_mult(et1,et1);
        add_instr("PLD aux_cmpr\n");
        add_instr("PLD aux_cmpi\n");
        exec_sqr2(3*OFST);
        oper_cmp (OFST,2*OFST,type);
    }

    // float var com int var
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==1) && (et2%OFST!=0))
    {
        add_instr("%s %s\n",  ld, v_name[et1%OFST]);
        add_instr("PIFM %s\n"   , v_name[et2%OFST]);
        add_instr("S%s\n", fop);
    }

    // float var com int acc
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==1) && (et2%OFST==0))
    {
        add_instr("IFA\n");
        add_instr("SET aux_cmp\n");
        add_instr("LOD %s\n", v_name[et1%OFST]);
        add_instr("PLD aux_cmp\n");
        add_instr("S%s\n", fop);
    }

    // float var com float var
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==0))
    {
        add_instr("%s %s\n" , ld, v_name[et1%OFST]);
        add_instr("PLD %s\n",     v_name[et2%OFST]);
        add_instr("S%s\n", fop);
    }

    // float var com float const
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
    {
        add_instr("%s %s\n" , ld, v_name[et1%OFST]);
        add_instr("PLD %d // %s\n", f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
        add_instr("S%s\n", fop);
    }

    // float var com float acc
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==2) && (et2%OFST==0))
    {
        add_instr("SET aux_float\n");
        add_instr("LOD %s\n", v_name[et1%OFST]);
        add_instr("PLD aux_float\n");
        add_instr("S%s\n", fop);
    }

    // float var com comp const
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==5))
    {
        fprintf(stdout, "Atenção na linha %d: comparando float com comp? Vou pegar o módulo.\n", line_num+1);

        oper_mult(et1,et1);
        exec_sqr2(et2);
        oper_cmp (2*OFST,2*OFST,type);
    }

    // float var com comp var
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==3) && (et2%OFST!=0))
    {
        fprintf(stdout, "Atenção na linha %d: comparando float com comp? Vou pegar o módulo.\n", line_num+1);

        oper_mult(et1,et1);
        exec_sqr2(et2);
        oper_cmp (2*OFST,2*OFST,type);
    }

    // float var com comp acc
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==3) && (et2%OFST==0))
    {
        fprintf(stdout, "Atenção na linha %d: comparando int com comp? Vou pegar o módulo.\n", line_num+1);

        add_instr("SETP aux_cmpi\n");
        add_instr("SET  aux_cmpr\n");
        acc_ok = 0;

        oper_mult(et1,et1);
        add_instr("PLD aux_cmpr\n");
        add_instr("PLD aux_cmpi\n");
        exec_sqr2(3*OFST);
        oper_cmp (2*OFST,2*OFST,type);
    }

    // float const com int var
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==1) && (et2%OFST!=0))
    {
        add_instr("%s %d // %s\n" , ld, f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
        add_instr("PIFM %s\n", v_name[et2%OFST]);
        add_instr("S%s\n", fop);
    }

    // float const com int acc
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==1) && (et2%OFST==0))
    {
        add_instr("IFA\n");
        add_instr("SET aux_cmp\n");
        add_instr("LOD %d // %s\n", f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
        add_instr("PLD aux_cmp\n");
        add_instr("S%s\n", fop);
    }

    // float const com float var
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==0))
    {
        add_instr("%s %d // %s\n" , ld, f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
        add_instr("PLD %s\n",     v_name[et2%OFST]);
        add_instr("S%s\n", fop);
    }

    // float const com float const
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
    {
        add_instr("%s  %d // %s\n", ld, f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
        add_instr("PLD %d // %s\n",     f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
        add_instr("S%s\n", fop);
    }

    // float const com float acc
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==2) && (et2%OFST==0))
    {
        add_instr("SET aux_float\n");
        add_instr("LOD %d // %s\n", f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
        add_instr("PLD aux_float\n");
        add_instr("S%s\n", fop);
    }

    // float const com comp const
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==5))
    {
        fprintf(stdout, "Atenção na linha %d: comparando float com comp? Vou pegar o módulo.\n", line_num+1);

        oper_mult(et1,et1);
        exec_sqr2(et2);
        oper_cmp (2*OFST,2*OFST,type);
    }

    // float const com comp var
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==3) && (et2%OFST!=0))
    {
        fprintf(stdout, "Atenção na linha %d: comparando float com comp? Vou pegar o módulo.\n", line_num+1);

        oper_mult(et1,et1);
        exec_sqr2(et2);
        oper_cmp (2*OFST,2*OFST,type);
    }

    // float const com comp acc
    if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==3) && (et2%OFST==0))
    {
        fprintf(stdout, "Atenção na linha %d: comparando int com comp? Vou pegar o módulo.\n", line_num+1);

        add_instr("SETP aux_cmpi\n");
        add_instr("SET  aux_cmpr\n");
        acc_ok = 0;

        oper_mult(et1,et1);
        add_instr("PLD aux_cmpr\n");
        add_instr("PLD aux_cmpi\n");
        exec_sqr2(3*OFST);
        oper_cmp (2*OFST,2*OFST,type);
    }

    // float acc com int var
    if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==1) && (et2%OFST!=0))
    {
        add_instr("PIFM %s\n", v_name[et2%OFST]);
        add_instr("S%s\n", fop);
    }

    // float acc com int acc
    if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==1) && (et2%OFST==0))
    {
        add_instr("IFA\n");
        add_instr("S%s\n", fop);
    }

    // float acc com float var
    if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==0))
    {
        add_instr("PLD %s\n", v_name[et2%OFST]);
        add_instr("S%s\n", fop);
    }

    // float acc com float const
    if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
    {
        add_instr("PLD %d // %s\n", f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
        add_instr("S%s\n", fop);
    }

    // float acc com float acc
    if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST==0))
    {
        add_instr("S%s\n", fop);
    }

    // float acc com comp const
    if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==5))
    {
        fprintf(stdout, "Atenção na linha %d: comparando float com comp? Vou pegar o módulo.\n", line_num+1);

        add_instr("SET aux_cmp\n");
        add_instr("PLD aux_cmp\n");
        oper_mult(et1,et1);
        exec_sqr2(et2);
        oper_cmp (2*OFST,2*OFST,type);
    }

    // float acc com comp var
    if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==3) && (et2%OFST!=0))
    {
        fprintf(stdout, "Atenção na linha %d: comparando float com comp? Vou pegar o módulo.\n", line_num+1);

        add_instr("SET aux_cmp\n");
        add_instr("PLD aux_cmp\n");
        oper_mult(et1,et1);
        exec_sqr2(et2);
        oper_cmp (2*OFST,2*OFST,type);
    }

    // float acc com comp acc
    if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==3) && (et2%OFST==0))
    {
        fprintf(stdout, "Atenção na linha %d: comparando float com comp? Vou pegar o módulo.\n", line_num+1);

        add_instr("SETP aux_cmpi\n");
        add_instr("SETP aux_cmpr\n");
        add_instr("SET  aux_cmp \n");

        add_instr("PLD aux_cmp\n");
        oper_mult(et1,et1);
        add_instr("PLD aux_cmpr\n");
        add_instr("PLD aux_cmpi\n");
        exec_sqr2(3*OFST);
        oper_cmp (2*OFST,2*OFST,type);
    }

    // comp const com int var
    if ((get_type(et1)==5) && (get_type(et2)==1) && (et2%OFST!=0))
    {
        fprintf(stdout, "Atenção na linha %d: comparando int com comp? Vou pegar o módulo.\n", line_num+1);

        exec_sqr2(et1);
        oper_mult(et2,et2);
        oper_cmp (2*OFST,OFST,type);
    }

    // comp const com int acc
    if ((get_type(et1)==5) && (get_type(et2)==1) && (et2%OFST==0))
    {
        fprintf(stdout, "Atenção na linha %d: comparando int com comp? Vou pegar o módulo.\n", line_num+1);

        add_instr("SET aux_cmp\n");
        acc_ok = 0;
        exec_sqr2  (et1);
        add_instr("PLD aux_cmp\n");
        add_instr("PLD aux_cmp\n");
        oper_mult(et2,et2);
        oper_cmp (2*OFST,OFST,type);
    }

    // comp const com float var
    if ((get_type(et1)==5) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==0))
    {
        fprintf(stdout, "Atenção na linha %d: comparando float com comp? Vou pegar o módulo.\n", line_num+1);

        exec_sqr2(et1);
        oper_mult(et2,et2);
        oper_cmp (2*OFST,2*OFST,type);
    }

    // comp const com float const
    if ((get_type(et1)==5) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
    {
        fprintf(stdout, "Atenção na linha %d: comparando float com comp? Vou pegar o módulo.\n", line_num+1);

        exec_sqr2(et1);
        oper_mult(et2,et2);
        oper_cmp (2*OFST,2*OFST,type);
    }

    // comp const com float acc
    if ((get_type(et1)==5) && (get_type(et2)==2) && (et2%OFST==0))
    {
        fprintf(stdout, "Atenção na linha %d: comparando float com comp? Vou pegar o módulo.\n", line_num+1);

        add_instr("SET aux_cmp\n");
        acc_ok = 0;
        exec_sqr2(et1);
        add_instr("PLD aux_cmp\n");
        add_instr("PLD aux_cmp\n");
        oper_mult(et2,et2);
        oper_cmp (2*OFST,2*OFST,type);
    }

    // comp const com comp const
    if ((get_type(et1)==5) && (get_type(et2)==5))
    {
        fprintf(stdout, "Atenção na linha %d: comparação com número complexo? Vou usar o módulo.\n", line_num+1);

        exec_sqr2(et1);
        exec_sqr2(et2);
        oper_cmp (2*OFST,2*OFST,type);
    }

    // comp const com comp var
    if ((get_type(et1)==5) && (get_type(et2)==3) && (et2%OFST!=0))
    {
        fprintf(stdout, "Atenção na linha %d: comparação com número complexo? Vou usar o módulo.\n", line_num+1);

        exec_sqr2(et1);
        exec_sqr2(et2);
        oper_cmp (2*OFST,2*OFST,type);
    }

    // comp const com comp acc
    if ((get_type(et1)==5) && (get_type(et2)==3) && (et2%OFST==0))
    {
        fprintf(stdout, "Atenção na linha %d: comparação com número complexo? Vou usar o módulo.\n", line_num+1);

        add_instr("SETP aux_imag\n");
        add_instr("SET  aux_real\n");
        acc_ok = 0;
        exec_sqr2(et1);
        add_instr("PLD aux_real\n");
        add_instr("PLD aux_imag\n");
        exec_sqr2(et2);
        oper_cmp (2*OFST,2*OFST,type);
    }

    // comp var com int var
    if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==1) && (et2%OFST!=0))
    {
        fprintf(stdout, "Atenção na linha %d: comparando int com comp? Vou pegar o módulo.\n", line_num+1);

        exec_sqr2(et1);
        oper_mult(et2,et2);
        oper_cmp (2*OFST,OFST,type);
    }

    // comp var com int acc
    if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==1) && (et2%OFST==0))
    {
        fprintf(stdout, "Atenção na linha %d: comparando int com comp? Vou pegar o módulo.\n", line_num+1);

        add_instr("SET aux_cmp\n");
        acc_ok = 0;
        exec_sqr2(et1);
        add_instr("PLD aux_cmp\n");
        add_instr("PLD aux_cmp\n");
        oper_mult(et2,et2);
        oper_cmp (2*OFST,OFST,type);
    }

    // comp var com float var
    if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==0))
    {
        fprintf(stdout, "Atenção na linha %d: comparando float com comp? Vou pegar o módulo.\n", line_num+1);

        exec_sqr2(et1);
        oper_mult(et2,et2);
        oper_cmp (2*OFST,2*OFST,type);
    }

    // comp var com float const
    if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
    {
        fprintf(stdout, "Atenção na linha %d: comparando float com comp? Vou pegar o módulo.\n", line_num+1);

        exec_sqr2(et1);
        oper_mult(et2,et2);
        oper_cmp (2*OFST,2*OFST,type);
    }

    // comp var com float acc
    if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==2) && (et2%OFST==0))
    {
        fprintf(stdout, "Atenção na linha %d: comparando float com comp? Vou pegar o módulo.\n", line_num+1);

        add_instr("SET aux_cmp\n");
        acc_ok = 0;
        exec_sqr2(et1);
        add_instr("PLD aux_cmp\n");
        add_instr("PLD aux_cmp\n");
        oper_mult(et2,et2);
        oper_cmp (2*OFST,2*OFST,type);
    }

    // comp var com comp const
    if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==5))
    {
        fprintf(stdout, "Atenção na linha %d: comparação com número complexo? Vou usar o módulo.\n", line_num+1);

        exec_sqr2(et1);
        exec_sqr2(et2);
        oper_cmp (2*OFST,2*OFST,type);
    }

    // comp var com comp var
    if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==3) && (et2%OFST!=0))
    {
        fprintf(stdout, "Atenção na linha %d: comparação com número complexo? Vou usar o módulo.\n", line_num+1);

        exec_sqr2(et1);
        exec_sqr2(et2);
        oper_cmp (2*OFST,2*OFST,type);
    }

    // comp var com comp acc
    if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==3) && (et2%OFST==0))
    {
        fprintf(stdout, "Atenção na linha %d: comparação com número complexo? Vou usar o módulo.\n", line_num+1);

        add_instr("SETP aux_imag\n");
        add_instr("SET  aux_real\n");
        acc_ok = 0;
        exec_sqr2(et1);
        add_instr("PLD aux_real\n");
        add_instr("PLD aux_imag\n");
        exec_sqr2(et2);
        oper_cmp (2*OFST,2*OFST,type);
    }

    // comp acc com int var
    if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==1) && (et2%OFST!=0))
    {
        fprintf(stdout, "Atenção na linha %d: comparação com número complexo? Vou usar o módulo.\n", line_num+1);

        exec_sqr2(et1);
        oper_mult(et2,et2);
        oper_cmp (2*OFST,OFST,type);
    }

    // comp acc com int acc
    if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==1) && (et2%OFST==0))
    {
        fprintf(stdout, "Atenção na linha %d: comparação com número complexo? Vou usar o módulo.\n", line_num+1);

        add_instr("SETP aux_cmp\n");
        exec_sqr2(et1);
        add_instr("PLD aux_cmp\n");
        add_instr("PLD aux_cmp\n");
        oper_mult(et2,et2);
        oper_cmp (2*OFST,OFST,type);
    }

    // comp acc com float var
    if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==0))
    {
        fprintf(stdout, "Atenção na linha %d: comparação com número complexo? Vou usar o módulo.\n", line_num+1);

        exec_sqr2(et1);
        oper_mult(et2,et2);
        oper_cmp (2*OFST,2*OFST,type);
    }

    // comp acc com float const
    if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
    {
        fprintf(stdout, "Atenção na linha %d: comparação com número complexo? Vou usar o módulo.\n", line_num+1);

        exec_sqr2(et1);
        oper_mult(et2,et2);
        oper_cmp (2*OFST,2*OFST,type);
    }

    // comp acc com float acc
    if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST==0))
    {
        fprintf(stdout, "Atenção na linha %d: comparação com número complexo? Vou usar o módulo.\n", line_num+1);

        add_instr("SETP aux_cmp2\n");
        exec_sqr2(et1);
        add_instr("PLD aux_cmp2\n");
        add_instr("PLD aux_cmp2\n");
        oper_mult(et2,et2);
        oper_cmp (2*OFST,2*OFST,type);
    }

    // comp acc com comp const
    if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==5))
    {
        fprintf(stdout, "Atenção na linha %d: comparação com número complexo? Vou usar o módulo.\n", line_num+1);

        exec_sqr2(et1);
        exec_sqr2(et2);
        oper_cmp (2*OFST,2*OFST,type);
    }

    // comp acc com comp var
    if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==3) && (et2%OFST!=0))
    {
        fprintf(stdout, "Atenção na linha %d: comparação com número complexo? Vou usar o módulo.\n", line_num+1);

        exec_sqr2(et1);
        exec_sqr2(et2);
        oper_cmp (2*OFST,2*OFST,type);
    }

    // comp acc com comp acc
    if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==3) && (et2%OFST==0))
    {
        fprintf(stdout, "Atenção na linha %d: comparação com número complexo? Vou usar o módulo.\n", line_num+1);

        add_instr("SETP aux_imag\n");
        add_instr("SETP aux_real\n");
        exec_sqr2(et1);
        add_instr("PLD aux_real\n");
        add_instr("PLD aux_imag\n");
        exec_sqr2(et2);
        oper_cmp (2*OFST,2*OFST,type);
    }

    acc_ok = 1;

    return OFST;
}

// compara maior e igual (contrario de menor)
int oper_greq(int et1, int et2)
{
    return oper_linv(oper_cmp(et1,et2,0));
}

// compara menor e igual (contrasrio de maior)
int oper_leeq(int et1, int et2)
{
    return oper_linv(oper_cmp(et1,et2,1));
}

// comparar se eh diferente (contrario de igual)
int oper_dife(int et1, int et2)
{
    return oper_linv(oper_cmp(et1,et2,2));
}

// ----------------------------------------------------------------------------
// operacoes condicionais (usadas em if else while) ---------------------------
// ----------------------------------------------------------------------------

// inversao logica (!)
int oper_linv(int et)
{
    int etr, eti;

    char ld [10]; if (acc_ok == 0) strcpy(ld ,"LOD"); else strcpy(ld,"PLD");
    char f2i[10]; if (acc_ok == 0) strcpy(f2i,"FIM"); else strcpy(f2i,"PFIM");

    // se for um int na memoria
    if ((get_type(et) == 1) && (et % OFST != 0))
    {
        add_instr("%s %s\n", ld, v_name[et%OFST]);
        add_instr("LINV\n");
    }

    // se for um int no acc
    if ((get_type(et) == 1) && (et % OFST == 0))
    {
        add_instr("LINV\n");
    }

    // se for um float var na memoria
    if ((get_type(et) == 2) && (et % OFST != 0) && (v_isco[et%OFST] == 0))
    {
        fprintf(stdout, "Atenção na linha %d: expressão lógica com float? Você é uma pessoa confusa!\n", line_num+1);

        add_instr("%s %s\n", f2i, v_name[et%OFST]);
        add_instr("LINV\n");
    }

    // se for um float const na memoria
    if ((get_type(et) == 2) && (et % OFST != 0) && (v_isco[et%OFST] == 1))
    {
        fprintf(stdout, "Atenção na linha %d: expressão lógica com float? Você é uma pessoa confusa!\n", line_num+1);

        add_instr("%s %d // %s\n", f2i, f2mf(v_name[et%OFST]), v_name[et%OFST]);
        add_instr("LINV\n");
    }

    // se for um float no acc
    if ((get_type(et) == 2) && (et % OFST == 0))
    {
        fprintf(stdout, "Atenção na linha %d: expressão lógica com float? Você é uma pessoa confusa!\n", line_num+1);

        add_instr("FIA\n");
        add_instr("LINV\n");
    }

    // se for um comp const
    if (get_type(et) == 5)
    {
        fprintf(stdout, "Atenção na linha %d: expressão lógica com comp? Sério? Vou arredondar a parte real!\n", line_num+1);

        get_cmp_cst(et,&etr,&eti);

        add_instr("%s %d // %s\n", f2i, f2mf(v_name[etr%OFST]), v_name[etr%OFST]);
        add_instr("LINV\n");
    }

    // se for um comp var
    if ((get_type(et) == 3) && (et % OFST != 0))
    {
        fprintf(stdout, "Atenção na linha %d: expressão lógica com comp? Sério? Vou arredondar a parte real!\n", line_num+1);

        add_instr("%s %s\n", f2i, v_name[et%OFST]);
        add_instr("LINV\n");
    }

    // se for um comp no acc
    if ((get_type(et) == 3) && (et % OFST == 0))
    {
        fprintf(stdout, "Atenção na linha %d: expressão lógica com comp? Sério? Vou arredondar a parte real!\n", line_num+1);

        add_instr("POP\n");
        add_instr("FIA\n");
        add_instr("LINV\n");
    }

    acc_ok = 1;

    return OFST; // retorna o id extendido de int
}

// and ou logicos (&& ||)
int oper_lanor(int et1, int et2, int type)
{
    if ((get_type(et1) > 1) || (get_type(et2) > 1))
    fprintf(stderr, "Erro na linha %d: Operação lógica, só entre números inteiros!\n", line_num+1);

    int etr, eti;

    char ld[10];
    if (acc_ok == 0) strcpy(ld,"LOD"); else strcpy(ld,"PLD");

    char op[16];
    switch (type)
    {
        case 0: strcpy(op, "LAND"); break;
        case 1: strcpy(op, "LOR" ); break;
    }

    // int var com int var
    if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==1) && (et2%OFST!=0))
    {
        add_instr("%s %s\n", ld, v_name[et2%OFST]);
        add_instr("%s %s\n", op, v_name[et1%OFST]);
    }

    // int var com int acc
    if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==1) && (et2%OFST==0))
    {
        add_instr("%s %s\n", op, v_name[et1%OFST]);
    }

    // int acc com int var
    if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==1) && (et2%OFST!=0))
    {
        add_instr("%s %s\n", op, v_name[et2%OFST]);
    }

    // int acc com int acc
    if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==1) && (et2%OFST==0))
    {
        add_instr("S%s\n", op);
    }

    acc_ok = 1;

    return OFST;
}

// ----------------------------------------------------------------------------
// opercao com portas logicas bitwise (~ & |) ---------------------------------
// ----------------------------------------------------------------------------

// porta inversora
int oper_inv(int et)
{
    if (get_type(et) > 1)
        fprintf(stderr, "Erro na linha %d: uso incorreto do operador '~'. Tem que passar tipo int. Viajou?\n", line_num+1);

    int etr, eti;

    char ld[10]; if (acc_ok == 0) strcpy(ld,"LOD"); else strcpy(ld,"PLD");

    // se for um int na memoria
    if ((get_type(et) == 1) && (et % OFST != 0)) add_instr("%s %s\n", ld, v_name[et%OFST]);
    
    add_instr("INV\n");

    acc_ok = 1;

    return OFST; // retorna o id extendido de int
}

// portas logicas de duas entradas (& | ^)
int oper_bitw(int et1, int et2, int type)
{
    if ((get_type(et1) > 1) || (get_type(et2) > 1))
        fprintf(stderr, "Erro na linha %d: qual o sentido de fazer operações bitwise sem ser com número inteiro? Vai se tratar!\n", line_num+1);

    char op[16];
    switch (type)
    {
        case  0: strcpy(op, "AND"); break;
        case  1: strcpy(op, "OR" ); break;
        case  2: strcpy(op, "XOR"); break;
    }

    char ld[10];
    if (acc_ok == 0) strcpy(ld,"LOD"); else strcpy(ld,"PLD");

    // int var com int var
    if ((get_type(et1) == 1) && (et1%OFST != 0) && (get_type(et2) == 1) && (et2%OFST != 0))
    {
        add_instr("%s %s\n", ld, v_name[et2%OFST]);
        add_instr("%s %s\n", op, v_name[et1%OFST]);
    }

    // int var com int acc
    if ((get_type(et1) == 1) && (et1%OFST != 0) && (get_type(et2) == 1) && (et2%OFST == 0))
    {
        add_instr("%s %s\n", op, v_name[et1%OFST]);
    }

    // int acc com int var
    if ((get_type(et1) == 1) && (et1%OFST == 0) && (get_type(et2) == 1) && (et2%OFST != 0))
    {
        add_instr("PLD %s\n", v_name[et2%OFST]);
        add_instr("S%s\n", op);
    }

    // int acc com int acc
    if ((get_type(et1) == 1) && (et1%OFST == 0) && (get_type(et2) == 1) && (et2%OFST == 0))
    {
        add_instr("S%s\n", op);
    }

    acc_ok = 1;

    return OFST; // retorna o id extendido de int
}

// ----------------------------------------------------------------------------
// operacoes de deslocamento de bits ------------------------------------------
// ----------------------------------------------------------------------------

int oper_shift(int et1, int et2, int type)
{
    if (get_type(et1) == 2)
        fprintf(stderr, "Erro na linha %d: deslocamento de bit em variáel float? Você é uma pessoa confusa!\n", line_num+1);

    if (get_type(et1) > 2)
        fprintf(stderr, "Erro na linha %d: como você quer que eu desloque bits de um número complexo? Viajou?\n", line_num+1);

    if (get_type(et2) > 2)
        fprintf(stderr, "Erro na linha %d: usando comp pra deslocar bits? Você é uma pessoa confusa!\n", line_num+1);


    char op[16];
    switch (type)
    {
        case  0: strcpy(op, "SHL"); break;
        case  1: strcpy(op, "SHR"); break;
        case  2: strcpy(op, "SRS"); break;
    }

    int etr, eti;

    char ld[10]; if (acc_ok == 0) strcpy(ld,"LOD"); else strcpy(ld,"PLD");

    // int var com int var
    if ((get_type(et1) == 1) && (et1%OFST != 0) && (get_type(et2) == 1) && (et2%OFST != 0))
    {
        add_instr("%s %s\n", ld, v_name[et2%OFST]);
        add_instr("%s %s\n", op, v_name[et1%OFST]);
    }

    // int var com int acc
    if ((get_type(et1) == 1) && (et1%OFST != 0) && (get_type(et2) == 1) && (et2%OFST == 0))
    {
        add_instr("%s %s\n", op, v_name[et1%OFST]);
    }

    // int var com float var
    if ((get_type(et1) == 1) && (et1%OFST != 0) && (get_type(et2) == 2) && (et2%OFST != 0) && (v_isco[et2%OFST]==0))
    {
        fprintf(stdout, "Atenção na linha %d: o segundo operando do shift tá dando float. Aí você me quebra!\n", line_num+1);

        add_instr("%s %s\n", ld, v_name[et2%OFST]);
        add_instr("FIA\n");
        add_instr("%s %s\n", op, v_name[et1%OFST]);
    }

    // int var com float const
    if ((get_type(et1) == 1) && (et1%OFST != 0) && (get_type(et2) == 2) && (et2%OFST != 0) && (v_isco[et2%OFST]==1))
    {
        fprintf(stdout, "Atenção na linha %d: o segundo operando do shift tá dando float. Aí você me quebra!\n", line_num+1);

        add_instr("%s %d // %s\n", ld, f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
        add_instr("FIA\n");
        add_instr("%s %s\n", op, v_name[et1%OFST]);
    }

    // int var com float acc
    if ((get_type(et1) == 1) && (et1%OFST != 0) && (get_type(et2) == 2) && (et2%OFST == 0))
    {
        add_instr("FIA\n");
        add_instr("%s %s\n", op, v_name[et1%OFST]);
    }

    // int acc com int var
    if ((get_type(et1) == 1) && (et1%OFST == 0) && (get_type(et2) == 1) && (et2%OFST != 0))
    {
        add_instr("PLD %s\n", v_name[et2%OFST]);
        add_instr("S%s\n", op);
    }

    // int acc com int acc
    if ((get_type(et1) == 1) && (et1%OFST == 0) && (get_type(et2) == 1) && (et2%OFST == 0))
    {
        add_instr("S%s\n", op);
    }

    // int acc com float var
    if ((get_type(et1) == 1) && (et1%OFST == 0) && (get_type(et2) == 2) && (et2%OFST != 0) && (v_isco[et2%OFST]==0))
    {
        fprintf(stdout, "Atenção na linha %d: o segundo operando do shift tá dando float. Aí você me quebra!\n", line_num+1);

        add_instr("PLD %s\n", v_name[et2%OFST]);
        add_instr("FIA\n");
        add_instr("S%s\n", op);
    }

    // int acc com float const
    if ((get_type(et1) == 1) && (et1%OFST == 0) && (get_type(et2) == 2) && (et2%OFST != 0) && (v_isco[et2%OFST]==1))
    {
        fprintf(stdout, "Atenção na linha %d: o segundo operando do shift tá dando float. Aí você me quebra!\n", line_num+1);

        add_instr("PLD %d // %s\n", f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
        add_instr("FIA\n");
        add_instr("S%s\n", op);
    }

    // int acc com float acc
    if ((get_type(et1) == 1) && (et1%OFST == 0) && (get_type(et2) == 2) && (et2%OFST == 0))
    {
        fprintf(stdout, "Atenção na linha %d: o segundo operando do shift tá dando float. Aí você me quebra!\n", line_num+1);
        
        add_instr("FIA\n");
        add_instr("S%s\n", op);
    }

    acc_ok = 1;

    return OFST; // retorna o id extendido de int
}