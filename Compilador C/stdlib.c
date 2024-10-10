#include "stdlib.h"
#include "variaveis.h"
#include "t2t.h"
#include "diretivas.h"
#include "data_assign.h"
#include "data_use.h"
#include "oper.h"

// executa instrucao IN
// et -> id extendido com endereco da porta
// tem que verificar se eh um inteiro (numero da porta)
int exec_in(int et)
{
    // da LOAD no endereco da porta, caso precise
    load_check(et,0); // porta eh sem sinal

    // se a porta eh um float, reclama e converte pra inteiro
    if (get_type(et) == 2)
    {
        fprintf(stdout, "Atenção na linha %d: endereço de entrada tem que ser int. Só me dando trabalho a toa!\n", line_num+1);

        // se for proc ponto fixo, chama macro de conversao pra inteiro
        if (prtype == 0)
        {
            if (using_macro == 0) fprintf(f_asm, "CALL float2int\n");
            f2i = 1; // seta variavel global dizendo que a macro float2int foi executada
        }
    }

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

    // ------------------------------------------------------------------------
    // checagem depende do processador ----------------------------------------
    // ------------------------------------------------------------------------

    int type = get_type(et);

    if (prtype == 0)
    {
        if (type == 2)
        {
            fprintf(stdout, "Atenção na linha %d: essa conversão pra inteiro gasta muito recurso!\n", line_num+1);

            if (using_macro == 0) fprintf(f_asm, "CALL float2int\n");
            f2i = 1;
        }
    }

    if (using_macro == 0) fprintf(f_asm, "ABS\n");

    return (prtype == 0) ? OFST : 2*OFST;
}

// executa instrucao PSET
// et eh o exp resultante
int exec_pst(int et)
{
    // ve se precisa carregar o argumento
    load_check(et,0);

    // ------------------------------------------------------------------------
    // checagem depende do processador ----------------------------------------
    // ------------------------------------------------------------------------

    int type = get_type(et);

    if (prtype == 0)
    {
        if (type == 2)
        {
            fprintf(stdout, "Atenção na linha %d: essa conversão pra inteiro gasta muito recurso!\n", line_num+1);

            if (using_macro == 0) fprintf(f_asm, "CALL float2int\n");
            f2i = 1;
        }
    }

    if (using_macro == 0) fprintf(f_asm, "PSET\n");

    return (prtype == 0) ? OFST : 2*OFST; // pra ajudar na reducao do exp, pois abs() eh uma funcao
}

// primeiro parametro da funcao out(a,b);
void exec_out1(int et)
{
    // checa se precisa carregar o parametro
    load_check(et,0);

    // se a porta eh um float, reclama e converte pra inteiro
    if (get_type(et) == 2)
    {
        fprintf(stdout, "Atenção na linha %d: endereço de entrada tem que ser int. Só me dando trabalho a toa!\n", line_num+1);

        // se for proc ponto fixo, chama macro de conversao pra inteiro
        if (prtype == 0)
        {
            if (using_macro == 0) fprintf(f_asm, "CALL float2int\n");
            f2i = 1; // seta variavel global dizendo que a macro float2int foi executada
        }
    }
}

// segundo parametro da funcao out(a,b)
void exec_out2(int et)
{
    load_check(et,0);

    int type = get_type(et);

    if (prtype == 0)
    {
        if (type == 2)
        {
            fprintf(stdout, "Atenção na linha %d: o processador é ponto fixo e você quer mandar um ponto flutuante pra fora? Vai gerar muito código!\n", line_num+1);

            if (using_macro == 0) fprintf(f_asm , "CALL float2int\n");
            f2i = 1;
        }
    }
    else // proc em ponto flutuante
    {
        if (type == 1)
        {
            fprintf(stdout, "Atenção na linha %d: o processador é ponto flutuante e você quer mandar um ponto fixo pra fora.\n", line_num+1);
        }
    }

    if (using_macro == 0) fprintf(f_asm, "OUT\n");

    acc_ok = 0;
}

int exec_sign(int et1, int et2)
{
    int left_type = get_type(et1);
    int righ_type = get_type(et2);

    if (prtype == 0)
    {
        // nao pode ser nem float nem comp
        if ((left_type != 1) || (righ_type != 1))
        {
            fprintf (stderr, "Erro na linha %d: parâmetros da função sign(.,.) devem ser inteiros!\n", line_num+1);
            return 0;
        }
    }

    int aux;
    return operacoes(et1, et2, "SIGN", "", &aux); // nao vou usar pf em software pro sign
}

int exec_norm(int et)
{
    if ((prtype == 0) || (get_type(et) != 1))
        fprintf (stderr, "Erro na linha %d: nada a ver! norm é só pra inteiro no processador em ponto fixo!\n", line_num+1);

    return int_oper(et,0,"norm(.)","NORM",0);
}
