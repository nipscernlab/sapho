#include "..\Headers\stdlib.h"
#include "..\Headers\variaveis.h"
#include "..\Headers\t2t.h"
#include "..\Headers\diretivas.h"
#include "..\Headers\data_assign.h"
#include "..\Headers\data_use.h"
#include "..\Headers\oper.h"

#include <math.h>

// redeclaracao de variaveis globais
int fatan  = 0; // se vai precisar de macros de ponto flutuante
int fatani = 0; // se vai precisar de macros de ponto flutuante
int fsqrt  = 0; // se vai precisar de macros de ponto flutuante
int fsqrti = 0; // se vai precisar de macros de ponto flutuante
int mgen   = 0; // se gera ou nao macros de ponto flutuante

// executa instrucao IN
// et -> id estendido com endereco da porta
// tem que verificar se eh um inteiro (numero da porta)
int exec_in(int et)
{
    // da LOAD no endereco da porta, caso precise
    load_check(et,0); // porta eh sem sinal

    // se a porta eh um float e proc eh int, reclama e converte pra inteiro
    if ((prtype == 0) && (get_type(et) == 2))
    {
        fprintf(stdout, "Atenção na linha %d: endereço de entrada tem que ser int. SÓ me dando trabalho a toa!\n", line_num+1);

        add_instr("CALL float2int\n");
        f2i = 1; // seta variavel global dizendo que a macro float2int foi executada
    }

    // testes com numeros complexos -------------------------------------------
    if (get_type(et) > 2)
        fprintf (stderr, "Erro na linha %d: Ah não, endereço de porta com número complexo já é demais!\n", line_num+1);
    // fim do teste -----------------------------------------------------------

    // executa instrucao IN
    add_instr("PUSH\n");
    add_instr("IN\n");

    // o retorno de IN depende do processador
    return (prtype == 0) ? OFST : 2*OFST;
}

// executa instrucao ABS
// et eh o exp pra pegar o valor absoluto
// o tipo de dado de entrada e saida sao de acordo com o tipo do et
int exec_abs(int et)
{
    // teste com numeros complexos --------------------------------------------
    if (get_type(et) > 2) return abs_comp(et);
    // fim do teste -----------------------------------------------------------

    // ve se precisa carregar o argumento
    load_check(et,0);

    // abs em ponto flutuante para proc em ponto fixo
    if ((prtype == 0) && (get_type(et) == 2))
    {
        if (is_macro() == 0)
        {
            fprintf(f_asm, "AND %d // zera o bit de sinal (abs pra float em software)\n",(int)pow(2,nbmant+nbexpo)-1);
        }

        fgen = 1;
    }
    else add_instr("ABS\n");

    return get_type(et)*OFST;
}

// modulo ao quadrado de um num complexo
int mod_sqr(int et)
{
    int type = get_type(et);

    int etr, eti;

    // se for uma constante ---------------------------------------------------

    if (type == 5)
    {
        split_cmp_const(et ,&etr,&eti); // pega o et de cada constante float
        etr  = oper_ari(etr,etr ,   0); // parte real ao quadrado
        eti  = oper_ari(eti,eti ,   0); // parte imag ao quadrado
        etr  = oper_ari(etr,eti ,   2); // soma os quadrados
    }

    // se estiver na memoria --------------------------------------------------

    if ((type == 3) && (et % OFST != 0))
    {
        get_cmp_ets    (et ,&etr,&eti); // pega o et de cada constante float
        etr  = oper_ari(etr,etr ,   0); // parte real ao quadrado
        eti  = oper_ari(eti,eti ,   0); // parte imag ao quadrado
        etr  = oper_ari(etr,eti ,   2); // soma os quadrados
    }

    // se estiver no acumulador -----------------------------------------------

    if ((type == 3) && (et % OFST == 0))
    {
        fprintf (f_asm, "PUSH\n");          // parte imag fica no acc e pilha
        oper_ari(2*OFST,2*OFST,0);          // multiplica acc com pilha
        fprintf (f_asm, "SETP  aux_cmp\n"); // salva temp e pega parte real

        fprintf (f_asm, "PUSH\n");          // parte real fica no acc e pilha
        oper_ari(2*OFST,2*OFST,0);          // multiplica acc com pilha
        fprintf (f_asm, "PLD  aux_cmp\n");  // xuxa o quadr do real pra pilha e pega o quadr do imag

        oper_ari(2*OFST,2*OFST,2);          // soma os quadrados

        etr = 2*OFST;                       // saida tem q ser et estendido pra float no acc
    }

    return etr;
}

// valor absoluto de um num complexo
int abs_comp(int et)
{
    return exec_sqrt(mod_sqr(et));          // computa a raiz quadrada do modulo ao quadrdo
}

// nega um num complexo
// apesar de estar em stdlib, nao vou fazer uma funcao explicita
// vai ser usada na reducao -exp -> exp
// e tb no assign x =- y;
int neg_comp(int et)
{
    int type = get_type(et);

    int etr, eti;

    // se for uma constante ---------------------------------------------------

    if (type == 5)
    {
        split_cmp_const(et ,&etr,&eti); // pega o et de cada constante float
        negacao(etr); // nega parte real
        negacao(eti); // nega parte imag
    }

    // se estiver na memoria --------------------------------------------------

    if ((type == 3) && (et % OFST != 0))
    {
        get_cmp_ets    (et ,&etr,&eti); // pega o et de cada constante float
        negacao(etr); // nega parte real
        negacao(eti); // nega parte imag
    }

    // se estiver no acumulador -----------------------------------------------

    if ((type == 3) && (et % OFST == 0))
    {
        negacao(2*OFST);
        fprintf(f_asm, "SETP aux_cmp\n"); // salva temp e pega parte real
        negacao(2*OFST);
        fprintf(f_asm, "PLD aux_cmp\n");
    }

    return 3*OFST;
}

// executa instrucao PSET
// et eh o exp resultante
int exec_pst(int et)
{
    // ve se precisa carregar o argumento
    load_check(et,0);

    if ((prtype == 0) && (get_type(et) == 2))
    {
        fprintf(stdout, "Atenção na linha %d: essa conversão pra inteiro gasta muito recurso!\n", line_num+1);

        add_instr("CALL float2int\n");
        f2i = 1;
    }

    // testes com numeros complexos -------------------------------------------
    if (get_type(et) > 2)
        fprintf (stderr, "Erro na linha %d: Não faz nenhum sentido usar a função 'pset(.)' com números complexos!\n", line_num+1);
    // fim do teste -----------------------------------------------------------

    add_instr("PSET\n");

    return (prtype == 0) ? OFST : 2*OFST;
}

// primeiro parametro da funcao out(a,b)
// o endereco da porta
void exec_out1(int et)
{
    // testes com numeros complexos -------------------------------------------
    if (get_type(et) > 2)
        fprintf (stderr, "Erro na linha %d: Ah não, endereço de porta com número complexo já é demais!\n", line_num+1);
    // fim do teste -----------------------------------------------------------

    // checa se precisa carregar o parametro
    load_check(et,0);

    // se a porta eh um float, reclama e converte pra inteiro
    if ((prtype == 0) && (get_type(et) == 2))
    {
        fprintf(stdout, "Atenção na linha %d: endereço de entrada tem que ser int. SÓ me dando trabalho a toa!\n", line_num+1);

        add_instr("CALL float2int\n");
        f2i = 1; // seta variavel global dizendo que a macro float2int foi executada
    }
}

// segundo parametro da funcao out(a,b)
// o valor de saida
void exec_out2(int et)
{
    // testes com numeros complexos -------------------------------------------
    if (get_type(et) > 2)
        fprintf (stderr, "Erro na linha %d: primeiro seleciona qual informação desse número complexo você quer ver!\n", line_num+1);
    // fim do teste -----------------------------------------------------------

    load_check(et,0);

    if ((prtype == 0) && (get_type(et) > 1))
    {
        fprintf(stdout, "Atenção na linha %d: o processador é ponto fixo e você quer mandar um ponto flutuante pra fora? Vai gerar muito código!\n", line_num+1);

        add_instr("CALL float2int\n");
        f2i = 1;
    }

    add_instr("OUT\n");

    acc_ok = 0; // libera acc
}

// executa a instrucao SIGN
int exec_sign(int et1, int et2)
{
    // testes com numeros complexos -------------------------------------------
    if ((get_type(et1) > 2) || (get_type(et2) > 2))
    {
        fprintf (stderr, "Erro na linha %d: não faz sentido o uso de sign(.,.) com números complexos!\n", line_num+1);
    }
    // fim do teste -----------------------------------------------------------

    if (prtype == 0)
    {
        // se for pra colocar o sinal em um float
        if (get_type(et2) == 2)
        {
            // mem com mem
            if ((et1%OFST != 0) && (et2%OFST != 0))
            {
                load_check(et1,0);
                add_instr("AND %d\n", 1 << (nbmant+nbexpo));
                add_instr("ADD %s\n", v_name[et2%OFST]);
            }
            // completar ...
            return 2*OFST;
        }
    }

    int aux1, aux2;                                  // precisa mas nao vai ser usado
    return operacoes(et1,et2,"SIGN","",&aux1, aux2); // nao vou usar pf em software pro sign por aqui
}

// executa instrucao NORM
int exec_norm(int et)
{
    if (get_type(et) != 1)
        fprintf (stderr, "Erro na linha %d: nada a ver! norm é só pra inteiro!\n", line_num+1);

    return int_oper(et,0,"norm(.)","NORM",0);
}

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

// executa macro sqrt
int exec_sqrt(int et)
{
    load_check(et,0);

    int type = get_type(et);

    if ((prtype == 0) && (type < 3))
    {
        if (type == 1)
        add_instr("CALL int2float\n"  );
        add_instr("CALL float_sqrti\n");

        fgen   = 1;
        fsqrti = 1;
        fadd   = 1;
        fmlt   = 1;
        fdiv   = 1;
    }

    if ((prtype == 1) && (type < 3))
    {
        add_instr("CALL float_sqrt\n");

        fsqrt = 1;
    }

    // testes com numeros complexos -------------------------------------------
    if (type > 2)
    {
        fprintf (stderr, "Erro na linha %d: não implementei raiz quadrada de número complexo ainda. Se vira!\n", line_num+1);
    }
    // fim do teste -----------------------------------------------------------

    mgen = 1; // gera macros para funcoes aritmeticas

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

// executa macro atan
int exec_atan(int et)
{
    load_check(et,0);

    int type = get_type(et);

    if ((prtype == 0) && (type < 3))
    {
        if (type == 1)
        add_instr("CALL int2float\n"  );
        add_instr("CALL float_atani\n");

        fgen   = 1;
        fatani = 1;
        fadd   = 1;
        fmlt   = 1;
        fdiv   = 1;
    }

    if ((prtype == 1) && (type < 3))
    {
        add_instr("CALL float_atan\n");

        fatan = 1;
    }

    // testes com numeros complexos -------------------------------------------
    if (type > 2)
    {
        fprintf (stderr, "Erro na linha %d: não implementei atan(.) de número complexo ainda. Se vira!\n", line_num+1);
    }
    // fim do teste -----------------------------------------------------------

    mgen = 1; // gera macros para funcoes aritmeticas

    return 2*OFST;
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
        split_cmp_const(et,&et_i,&et_r);
        oper_ari(et_r,et_i,1);
    }

    // comp na memoria
    if ((get_type(et) == 3) && (id != 0))
    {
        get_cmp_ets(et,&et_i,&et_r);
        oper_ari(et_r,et_i,1);
    }

    // comp no acc
    if ((get_type(et) == 3) && (id == 0))
    {
        id   = exec_id("aux_cmp_i");
        et_i = 2*OFST + id;

        add_instr("SETP %s\n", v_name[id]);

        oper_ari(et_i,2*OFST,1);
    }

    exec_atan(2*OFST);

    acc_ok = 1;
    return 2*OFST;
}

// retorna a parte real de um comp
int exec_real(int et)
{
    if (get_type(et) < 3) fprintf (stderr, "Erro na linha %d: argumento da função real(.) tem que ser complexo!\n", line_num+1);

    int id = et % OFST;

    // comp const
    if (get_type(et) == 5)
    {
        int et_r, et_i;
        split_cmp_const(et,&et_r,&et_i);
        load_check(et_r,0);
    }

    // comp na memoria
    if ((get_type(et) == 3) && (id != 0))
    {
        load_check(et,0);
    }

    // comp no acc
    if ((get_type(et) == 3) && (id == 0))
    {
        add_instr("SETP aux_cmp\n");
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

    // comp const
    if (get_type(et) == 5)
    {
        split_cmp_const(et,&et_r,&et_i);
        load_check(et_i,0);
    }

    // comp na memoria
    if ((get_type(et) == 3) && (id != 0))
    {
        get_cmp_ets(et,&et_r,&et_i);
        load_check(et_i,0);
    }

    // comp no acc
    if ((get_type(et) == 3) && (id == 0))
    {
        add_instr("SETP aux_cmp\n");
        add_instr("LOAD aux_cmp\n");
    }

    acc_ok = 1;
    return 2*OFST;
}
