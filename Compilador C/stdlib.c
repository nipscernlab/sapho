#include "stdlib.h"
#include "variaveis.h"
#include "t2t.h"
#include "diretivas.h"
#include "data_assign.h"
#include "data_use.h"
#include "oper.h"

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
// o tipo de dado de entrada e saida sao de acordo com o tipo de processador
int exec_abs(int et)
{
    // ve se precisa carregar o argumento
    load_check(et,0);

    if ((prtype == 0) && (get_type(et) == 2))
    {
        if (using_macro == 0)
        {
            fprintf(f_asm, "\nPLD float_nbits");
            fprintf(f_asm, "             // ABS em ponto flutuante\n" );
            fprintf(f_asm, "SHL 1\nINV\nSAND");
            fprintf(f_asm, "                        // fim do ABS\n\n");
        }

        fgen = 1;
    }
    else if (using_macro == 0) fprintf(f_asm, "ABS\n");

    return get_type(et)*OFST;
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

        if (using_macro == 0) fprintf(f_asm, "CALL float2int\n");
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

    if ((prtype == 0) && (get_type(et) == 2))
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
/*double my_sqrt(float num)
{
    float x = num;
    float epslon = 0.000001;

    while (1)
    {
        float raiz = 0.5 * (x+num/x);
        if (fabs(x - raiz) < epslon) break;
        x = raiz;
    }

    return raiz;
}*/
