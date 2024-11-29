#include "stdlib.h"
#include "variaveis.h"
#include "t2t.h"
#include "diretivas.h"
#include "data_assign.h"
#include "data_use.h"
#include "oper.h"
#include <math.h>

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
        fprintf(stdout, "Atenção na linha %d: endereço de entrada tem que ser int. Só me dando trabalho a toa!\n", line_num+1);

        if (using_macro == 0) fprintf(f_asm, "CALL float2int\n");
        f2i = 1; // seta variavel global dizendo que a macro float2int foi executada
    }

    // testes com numeros complexos -------------------------------------------
    if (get_type(et) > 2)
        fprintf (stderr, "Erro na linha %d: Ah não, endereço de porta com número complexo já é demais!\n", line_num+1);
    // fim do teste -----------------------------------------------------------

    // executa instrucao IN
    if (using_macro == 0) fprintf(f_asm, "PUSH\nIN\n");

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
        if (using_macro == 0)
        {
            fprintf(f_asm, "AND %d           // zera o bit de sinal (abs pra float em software)\n",(int)pow(2,nbmant+nbexpo)-1);
            //fprintf(f_asm, "\nPLD float_nbits");
            //fprintf(f_asm, "             // ABS em ponto flutuante\n" );
            //fprintf(f_asm, "SHL 1\nINV\nSAND");
            //fprintf(f_asm, "                        // fim do ABS\n\n");
        }

        fgen = 1;
    }
    else if (using_macro == 0) fprintf(f_asm, "ABS\n");

    return get_type(et)*OFST;
}

// valor absoluto de um num complexo
int abs_comp(int et)
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

    return exec_sqrt(etr);                  // computa a raiz quadrada e retorna
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
        fprintf(f_asm, "SETP  aux_cmp\n"); // salva temp e pega parte real
        negacao(2*OFST);
        fprintf(f_asm, "PLD   aux_cmp\n");
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

        if (using_macro == 0) fprintf(f_asm, "CALL   float2int\n");
        f2i = 1;
    }

    // testes com numeros complexos -------------------------------------------
    if (get_type(et) > 2)
        fprintf (stderr, "Erro na linha %d: Não faz nenhum sentido usar a função 'pset(.)' com números complexos!\n", line_num+1);
    // fim do teste -----------------------------------------------------------

    if (using_macro == 0) fprintf(f_asm, "PSET\n");

    return (prtype == 0) ? OFST : 2*OFST;
}

// primeiro parametro da funcao out(a,b)
// o endereco da porta
void exec_out1(int et)
{
    // checa se precisa carregar o parametro
    load_check(et,0);

    // se a porta eh um float, reclama e converte pra inteiro
    if ((prtype == 0) && (get_type(et) == 2))
    {
        fprintf(stdout, "Atenção na linha %d: endereço de entrada tem que ser int. Só me dando trabalho a toa!\n", line_num+1);

        if (using_macro == 0) fprintf(f_asm, "CALL float2int\n");
        f2i = 1; // seta variavel global dizendo que a macro float2int foi executada
    }
}

// segundo parametro da funcao out(a,b)
// o valor de saida
void exec_out2(int et)
{
    load_check(et,0);

    if ((prtype == 0) && (get_type(et) > 1))
    {
        fprintf(stdout, "Atenção na linha %d: o processador é ponto fixo e você quer mandar um ponto flutuante pra fora? Vai gerar muito código!\n", line_num+1);

        if (using_macro == 0) fprintf(f_asm , "CALL float2int\n");
        f2i = 1;
    }

    if (using_macro == 0) fprintf(f_asm, "OUT\n");

    acc_ok = 0; // libera acc
}

// executa a instrucao SIGN
int exec_sign(int et1, int et2)
{
    if (prtype == 0)
    {
        // nao vou converter pra float em ponto fixo
        // soh se um dia alguem precisar
        if ((get_type(et1) != 1) || (get_type(et2) != 1))
        {
            fprintf (stderr, "Erro na linha %d: parâmetros da função sign(.,.) devem ser inteiros!\n", line_num+1);
            return 0;
        }
    }

    int aux;                                  // precisa mas nao vai ser usado
    return operacoes(et1,et2,"SIGN","",&aux); // nao vou usar pf em software pro sign
}

// executa instrucao NORM
int exec_norm(int et)
{
    if ((prtype == 1) || (get_type(et) != 1))
        fprintf (stderr, "Erro na linha %d: nada a ver! norm é só pra inteiro no processador em ponto fixo!\n", line_num+1);

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
        if (using_macro == 0) fprintf(f_asm , "CALL int2float\n"  );
        if (using_macro == 0) fprintf(f_asm , "CALL float_sqrti\n");

        fgen   = 1;
        fsqrti = 1;
        fadd   = 1;
        fmlt   = 1;
        fdiv   = 1;
    }

    if ((prtype == 1) && (type < 3))
    {
        if (using_macro == 0) fprintf(f_asm , "CALL float_sqrt\n");

        fsqrt = 1;
    }

    // testes com numeros complexos -------------------------------------------
    if (type > 2)
    {
        fprintf (stderr, "Erro na linha %d: não implementei raiz quadrada de número complexo. Se vira!\n", line_num+1);
    }
    // fim do teste -----------------------------------------------------------

    mgen = 1;

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
        if (using_macro == 0) fprintf(f_asm , "SETP aux_cmp\n");
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
        if (using_macro == 0) fprintf(f_asm , "SETP aux_cmp\n");
        if (using_macro == 0) fprintf(f_asm , "LOAD aux_cmp\n");
    }

    acc_ok = 1;
    return 2*OFST;
}
