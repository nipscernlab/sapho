#include "..\Headers\stdlib.h"
#include "..\Headers\variaveis.h"
#include "..\Headers\t2t.h"
#include "..\Headers\diretivas.h"
#include "..\Headers\data_assign.h"
#include "..\Headers\data_use.h"
#include "..\Headers\oper.h"
#include "..\Headers\global.h"

#include <math.h>
#include <string.h>

// ----------------------------------------------------------------------------
// redeclaracao de variaveis globais ------------------------------------------
// ----------------------------------------------------------------------------

int fatani = 0; // se vai precisar de macro pra arco tangente
int fsqrti = 0; // se vai precisar de macro pra raiz quadrada

// ----------------------------------------------------------------------------
// entrada e saida ------------------------------------------------------------
// ----------------------------------------------------------------------------

// input
int exec_in(int id)
{
    if (acc_ok == 0) add_instr("LOD %s\n", v_name[id]); else add_instr("PLD %s\n", v_name[id]);

    // executa instrucao IN
    add_instr("IN\n");

    acc_ok = 1;  // diz que o acc agora tem um valor carregado

    return OFST;
}

// primeiro parametro da funcao out(a,b)
// o endereco da porta
void exec_out1(int id)
{
    add_instr("LOD %s\n", v_name[id]);
    acc_ok = 1;
}

// segundo parametro da funcao out(a,b)
// valor de saida
void exec_out2(int et)
{
    if (get_type(et) > 2)
        fprintf (stderr, "Erro na linha %d: primeiro seleciona qual informação desse número complexo você quer!\n", line_num+1);

    // int var
    if ((get_type(et) == 1) && (et%OFST!=0))
    {
        add_instr("PLD %s\n", v_name[et%OFST]);
    }

    // int acc
    if ((get_type(et) == 1) && (et%OFST==0))
    {
        // nao faz nada
    }

    // float var
    if ((get_type(et) == 2) && (et%OFST!=0) && (v_isco[et%OFST]==0))
    {
        fprintf(stdout, "Atenção na linha %d: convertendo o dado pra inteiro antes de sair do processador\n", line_num+1);

        add_instr("PFIM %s\n", v_name[et%OFST]);
    }

    // float const
    if ((get_type(et) == 2) && (et%OFST!=0) && (v_isco[et%OFST]==1))
    {
        fprintf(stdout, "Atenção na linha %d: convertendo o dado pra inteiro antes de sair do processador\n", line_num+1);

        add_instr("PFIM %d // %s\n", f2mf(v_name[et%OFST]), v_name[et%OFST]);
    }

    // float acc
    if ((get_type(et) == 2) && (et%OFST==0))
    {
        fprintf(stdout, "Atenção na linha %d: convertendo o dado pra inteiro antes de sair do processador\n", line_num+1);

        add_instr("FIA\n");
    }

    add_instr("OUT\n");

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

    char ld[10]; if (acc_ok == 0) strcpy(ld,"LOD"); else strcpy(ld,"PLD");

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
    if ((get_type(et1) == 1) && (et1 % OFST != 0) && (get_type(et2) == 2) && (v_isco[et2%OFST]==0) && (et2 % OFST != 0))
    {
        add_instr("%s %s\n", ld, v_name[et2%OFST]);
        add_instr("FSGN %s\n"  , v_name[et1%OFST]);
    }

    // int na memoria e float const na memoria
    if ((get_type(et1) == 1) && (et1 % OFST != 0) && (get_type(et2) == 2) && (v_isco[et2%OFST]==1) && (et2 % OFST != 0))
    {
        add_instr("%s %d // %s\n", ld, f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
        add_instr("FSGN %s\n"    ,          v_name[et1%OFST]);
    }

    // int na memoria e float no acc
    if ((get_type(et1) == 1) && (et1 % OFST != 0) && (get_type(et2) == 2) && (et2 % OFST == 0))
    {
        add_instr("FSGN %s\n", v_name[et1%OFST]);
    }

    // int no acc e int na memoria
    if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 1) && (et2 % OFST != 0))
    {
        add_instr("%s %s\n", ld, v_name[et2%OFST]);
        add_instr("SSGN\n");
    }

    // int no acc e int no acc
    if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 1) && (et2 % OFST == 0))
    {
        add_instr("SSGN\n");
    }

    // int no acc e float var na memoria
    if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 2) && (v_isco[et2%OFST]==0) && (et2 % OFST != 0))
    {
        add_instr("PLD %s\n", v_name[et2%OFST]  );
        add_instr("SFSGN\n");
    }

    // int no acc e float const na memoria
    if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 2) && (v_isco[et2%OFST]==1) && (et2 % OFST != 0))
    {
        add_instr("PLD %u // %s\n", f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
        add_instr("SFSGN\n");
    }

    // int no acc e float no acc
    if ((get_type(et1) == 1) && (et1 % OFST == 0) && (get_type(et2) == 2) && (et2 % OFST == 0))
    {
        add_instr("SFSGN\n");
    }

    // float var na memoria e int na memoria
    if ((get_type(et1) == 2) && (v_isco[et1%OFST]==0) && (et1 % OFST != 0) && (get_type(et2) == 1) && (et2 % OFST != 0))
    {
        add_instr("%s %s\n", ld, v_name[et2%OFST]);
        add_instr("SGN %s\n"   , v_name[et1%OFST]);
    }

    // float var na memoria e int no acc
    if ((get_type(et1) == 2) && (v_isco[et1%OFST]==0) && (et1 % OFST != 0) && (get_type(et2) == 1) && (et2 % OFST == 0))
    {
        add_instr("SGN %s\n"  , v_name[et1%OFST]);
    }

    // float var na memoria e float var na memoria
    if ((get_type(et1) == 2) && (v_isco[et1%OFST]==0) && (et1 % OFST != 0) && (get_type(et2) == 2) && (v_isco[et2%OFST]==0) && (et2 % OFST != 0))
    {
        add_instr("%s %s\n", ld, v_name[et2%OFST]);
        add_instr("FSGN %s\n"  , v_name[et1%OFST]);
    }

    // float var na memoria e float const na memoria
    if ((get_type(et1) == 2) && (v_isco[et1%OFST]==0) && (et1 % OFST != 0) && (get_type(et2) == 2) && (v_isco[et2%OFST]==1) && (et2 % OFST != 0))
    {   
        add_instr("%s %u // %s\n", ld, f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
        add_instr("FSGN %s\n"  , v_name[et1%OFST]);
    }

    // float var na memoria e float no acc
    if ((get_type(et1) == 2) && (v_isco[et1%OFST]==0) && (et1 % OFST != 0) && (get_type(et2) == 2) && (et2 % OFST == 0))
    {
        add_instr("FSGN %s\n"  , v_name[et1%OFST]);
    }

    // float const na memoria e int na memoria
    if ((get_type(et1) == 2) && (v_isco[et1%OFST]==1) && (et1 % OFST != 0) && (get_type(et2) == 1) && (et2 % OFST != 0))
    {
        add_instr("%s %s\n", ld, v_name[et2%OFST]);
        add_instr("SGN %d // %s\n", f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
    }

    // float const na memoria e int no acc
    if ((get_type(et1) == 2) && (v_isco[et1%OFST]==1) && (et1 % OFST != 0) && (get_type(et2) == 1) && (et2 % OFST == 0))
    {
        add_instr("SGN %d // %s\n", f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
    }

    // float const na memoria e float var na memoria
    if ((get_type(et1) == 2) && (v_isco[et1%OFST]==1) && (et1 % OFST != 0) && (get_type(et2) == 2) && (v_isco[et2%OFST]==0) && (et2 % OFST != 0))
    {
        add_instr("%s %s\n", ld, v_name[et2%OFST]);
        add_instr("FSGN %d // %s\n", f2mf(v_name[et1%OFST] ), v_name[et1%OFST]);
    }

    // float const na memoria e float const na memoria
    if ((get_type(et1) == 2) && (v_isco[et1%OFST]==1) && (et1 % OFST != 0) && (get_type(et2) == 2) && (v_isco[et2%OFST]==1) && (et2 % OFST != 0))
    {
        add_instr("%s %d // %s\n", ld, f2mf(v_name[et2%OFST] ), v_name[et2%OFST]);
        add_instr("FSGN %d // %s\n"  , f2mf(v_name[et1%OFST] ), v_name[et1%OFST]);
    }

    // float const na memoria e float no acc
    if ((get_type(et1) == 2) && (v_isco[et1%OFST]==1) && (et1 % OFST != 0) && (get_type(et2) == 2) && (et2 % OFST == 0))
    {
        add_instr("FSGN %d // %s\n"  , f2mf(v_name[et1%OFST] ), v_name[et1%OFST]);
    }

    // float no acc e int na memoria
    if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 1) && (et2 % OFST != 0))
    {
        add_instr("%s %s\n", ld, v_name[et2%OFST]);
        add_instr("SSGN\n");
    }

    // float no acc e int no acc
    if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 1) && (et2 % OFST == 0))
    {
        add_instr("SSGN\n");
    }

    // float no acc e float var na memoria
    if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 2) && (v_isco[et2%OFST]==0) && (et2 % OFST != 0))
    {
        add_instr("PLD %s\n", v_name[et2%OFST]);
        add_instr("SFSGN\n");
    }

    // float no acc e float const na memoria
    if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 2) && (v_isco[et2%OFST]==1) && (et2 % OFST != 0))
    {
        add_instr("PLD %d // %s\n", f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
        add_instr("SFSGN\n");
    }

    // float no acc e float no acc
    if ((get_type(et1) == 2) && (et1 % OFST == 0) && (get_type(et2) == 2) && (et2 % OFST == 0))
    {
        add_instr("SFSGN\n");
    }

    acc_ok = 1;

    return get_type(et2)*OFST;
}

// valor absoluto (int, float e comp)
int exec_abs(int et)
{
    char  ld [10]; if (acc_ok == 0) strcpy(ld  , "LOD" ); else strcpy(ld  , "PLD"  );
    char  abs[10]; if (acc_ok == 0) strcpy( abs, "ABSM"); else strcpy( abs, "PABSM");
    char fabs[10]; if (acc_ok == 0) strcpy(fabs,"FABSM"); else strcpy(fabs,"PFABSM");

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
        add_instr("FABS\n");
    }

    // comp const, na memoria e no acc
    if ((get_type(et) == 3) || (get_type(et) == 5))
    {
        et = exec_absc(et);
    }

    acc_ok = 1;

    return get_type(et)*OFST;
}

// zera se for negativo
int exec_pst(int et)
{
    char  ld [10]; if (acc_ok == 0) strcpy( ld , "LOD" ); else strcpy( ld , "PLD"  );
    char  pst[10]; if (acc_ok == 0) strcpy( pst, "PSTM"); else strcpy( pst, "PPSTM");
    char fpst[10]; if (acc_ok == 0) strcpy(fpst,"FPSTM"); else strcpy(fpst,"PFPSTM");

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
        add_instr("FPST\n");
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

    char nrm[10]; if (acc_ok == 0) strcpy(nrm,"NRMM"); else strcpy(nrm,"PNRMM");

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

// codigo em C+- para calcular raiz quadrada para float
// deixar aqui pra lembrar de onde vieram as macros
// float_sqrt.asm e float_sqrti.asm
/*double my_sqrt(float num)
{
    float x = num;
    float epslon = 0.000008;  // menor numero possivel = 2^(m-1)*2^(-(2^(e-1)))
                              // para m = 16 e = 6, o num eh: 0.000007629...
    while (1)
    {
        float raiz = 0.5 * (x+num/x);
        if (fabs(x - raiz) < epslon) break;
        x = raiz;
    }

    return raiz;
}*/

int exec_sqrt(int et)
{
    if (get_type(et) > 2)
    {
        fprintf (stderr, "Erro na linha %d: não implementei raiz quadrada de número complexo ainda. Se vira!\n", line_num+1);
    }

    char ld [10]; if (acc_ok == 0) strcpy(ld ,"LOD" ); else strcpy(ld ,"PLD" );
    char i2f[10]; if (acc_ok == 0) strcpy(i2f, "IFM"); else strcpy(i2f,"PIFM");

    // int na memoria
    if ((get_type(et) == 1) && (et % OFST != 0))
    {
        add_instr("%s %s\n", i2f, v_name[et%OFST]);
        add_instr("CALL float_sqrti\n"); fsqrti=1;
    }

    // int no acc
    if ((get_type(et) == 1) && (et % OFST == 0))
    {
        add_instr("IFA\n");
        add_instr("CALL float_sqrti\n"); fsqrti=1;
    }

    // float var na memoria
    if ((get_type(et) == 2) && (v_isco[et % OFST] == 0) && (et % OFST != 0))
    {
        add_instr("%s %s\n", ld, v_name[et%OFST]);
        add_instr("CALL float_sqrti\n"); fsqrti=1;
    }
        
    // float const na memoria
    if ((get_type(et) == 2) && (v_isco[et % OFST] == 1) && (et % OFST != 0))
    {
        add_instr("%s %d // %s\n", ld, f2mf(v_name[et%OFST]), v_name[et%OFST]);
        add_instr("CALL float_sqrti\n"); fsqrti=1;
    }

    // float no acc
    if ((get_type(et) == 2) && (et % OFST == 0))
    {
        add_instr("CALL float_sqrti\n"); fsqrti=1;
    }

    acc_ok = 1;

    return 2*OFST;
}

// codigo em C+- para calcular arco-tg para float
// deixar aqui pra lembrar de onde vieram as macros
// float_atan.asm e float_atani.asm
/*float atan(float x)
{
    float pi2 = 3.1415/2.0;

    if (abs(x) > 1.0) return sign(x,pi2) - atan(1.0/x);

    float termo      = x;
    float x2         = x*x;
    float resultado  = termo;
    float tolerancia = 0.000008/x2;

    int indiceX = 3;

    while (abs(termo) > tolerancia) {
        termo = termo * (- x2 * (indiceX - 2)) / indiceX;

        resultado = resultado + termo;
        indiceX = indiceX + 2;
    }

    return resultado;
}*/

int exec_atan(int et)
{
    if (get_type(et) > 2) fprintf (stderr, "Erro na linha %d: não implementei arco-tangente de número complexo ainda. Se vira!\n", line_num+1);

    char ld [10]; if (acc_ok == 0) strcpy(ld ,"LOD" ); else strcpy(ld , "PLD");
    char i2f[10]; if (acc_ok == 0) strcpy(i2f,"IFM" ); else strcpy(i2f,"PIFM");

    // int na memoria
    if ((get_type(et) == 1) && (et % OFST != 0))
    {
        add_instr("%s %s\n", i2f, v_name[et%OFST]);
        add_instr("CALL float_atani\n"); fatani=1;
    }

    // int no acc
    if ((get_type(et) == 1) && (et % OFST == 0))
    {
        add_instr("IFA\n");
        add_instr("CALL float_atani\n"); fatani=1;
    }

    // float var na memoria
    if ((get_type(et) == 2) && (v_isco[et % OFST] == 0) && (et % OFST != 0))
    {
        add_instr("%s %s\n", ld, v_name[et%OFST]);
        add_instr("CALL float_atani\n"); fatani=1;
    }
        
    // float const na memoria
    if ((get_type(et) == 2) && (v_isco[et % OFST] == 1) && (et % OFST != 0))
    {
        add_instr("%s %d // %s\n", ld, f2mf(v_name[et%OFST]), v_name[et%OFST]);
        add_instr("CALL float_atani\n"); fatani=1;
    }

    // float no acc
    if ((get_type(et) == 2) && (et % OFST == 0))
    {
        add_instr("CALL float_atani\n"); fatani=1;
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

    char ld[10];
    if (acc_ok == 0) strcpy(ld,"LOD"); else strcpy(ld,"PLD");

    // comp const
    if (get_type(et) == 5)
    {
        int et_r, et_i;
        get_cmp_cst(et,&et_r,&et_i);

        add_instr("%s %d // %s\n", ld, f2mf(v_name[et_r%OFST]), v_name[et_r%OFST]);
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

    char ld[10];
    if (acc_ok == 0) strcpy(ld,"LOD"); else strcpy(ld,"PLD");

    // comp const
    if (get_type(et) == 5)
    {
        int et_r, et_i;
        get_cmp_cst(et,&et_r,&et_i);

        add_instr("%s %d // %s\n", ld, f2mf(v_name[et_i%OFST]), v_name[et_i%OFST]);
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
        add_instr("SETP aux_cmp\n");
        add_instr("LOD aux_cmp\n");
    }

    acc_ok = 1;

    return 2*OFST;
}

// modulo ao quadrado de um num complexo
int exec_sqr2(int et)
{
    int type = get_type(et);

    int etr, eti;

    // se for uma constante ---------------------------------------------------

    if (type == 5)
    {
        get_cmp_cst(et ,&etr,&eti); // pega o et de cada constante float
        etr  = oper_mult(etr,etr);      // parte real ao quadrado
        eti  = oper_mult(eti,eti);      // parte imag ao quadrado
        etr  = oper_soma(etr,eti);      // soma os quadrados
    }

    // se estiver na memoria --------------------------------------------------

    if ((type == 3) && (et % OFST != 0))
    {
        get_cmp_ets    (et ,&etr,&eti); // pega o et de cada constante float
        etr  = oper_mult(etr,etr);      // parte real ao quadrado
        eti  = oper_mult(eti,eti);      // parte imag ao quadrado
        etr  = oper_soma(etr,eti);      // soma os quadrados
    }

    // se estiver no acumulador -----------------------------------------------

    if ((type == 3) && (et % OFST == 0))
    {
        add_instr("PUSH\n");                // parte imag fica no acc e pilha
        oper_mult(2*OFST,2*OFST );          // multiplica acc com pilha
        add_instr("SETP aux_cmp\n");        // salva temp e pega parte real

        add_instr("PUSH\n");                // parte real fica no acc e pilha
        oper_mult(2*OFST,2*OFST );          // multiplica acc com pilha
        add_instr("PLD  aux_cmp\n");        // xuxa o quadr do real pra pilha e pega o quadr do imag

        oper_soma(2*OFST,2*OFST);           // soma os quadrados

        etr = 2*OFST;                       // saida tem q ser et estendido pra float no acc
    }

    return etr; // 2*OFST?
}

// valor absoluto de um num complexo
int exec_absc(int et)
{
    return exec_sqrt(exec_sqr2(et));        // computa a raiz quadrada do modulo ao quadrdo
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
        oper_divi(et_r,et_i);
    }

    // comp na memoria
    if ((get_type(et) == 3) && (id != 0))
    {
        get_cmp_ets(et,&et_i,&et_r);
        oper_divi(et_r,et_i);
    }

    // comp no acc
    if ((get_type(et) == 3) && (id == 0))
    {
        id   = exec_id("aux_cmp_i");
        et_i = 2*OFST + id;

        add_instr("SETP %s\n", v_name[id]);

        oper_divi(et_i,2*OFST);
    }

    exec_atan(2*OFST);

    acc_ok = 1;
    return 2*OFST;
}