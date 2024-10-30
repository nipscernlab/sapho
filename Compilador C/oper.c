#include "oper.h"
#include "variaveis.h"
#include "t2t.h"
#include "diretivas.h"
#include "data_use.h"
#include <string.h>

// gera instrucao pra negar a utlima reducao exp
int negacao(int et)
{
    // se for uma constante, soh coloca um sinal de - na frente
    if (v_isco[et % OFST])
        load_check(et,1);
    else
    {
        load_check(et,0);

        if ((prtype == 0) && (et >= 2*OFST))
        {
            if (using_macro == 0)
            {
                fprintf(f_asm, " // negacao em ponto flutuante\n");
                fprintf(f_asm, "PLD 1\nPLD float_nbits\nSSHL\nSADD\n");
                fprintf(f_asm, " // fim da negacao\n");
            }
        }
        else
        {
            if (using_macro == 0) fprintf(f_asm, "NEG\n"); // negacao em ponto fixo
        }
    }

    return (et < 2*OFST) ? OFST : 2*OFST; // et padrao para reducao exp
}

// checa se o acc esta com ponto fixo
int iacc(int et)
{
    return (et == OFST) ? 1 : 0;
}

// checa se variavel eh ponto fixo
int imem(int et)
{
    return ((et > OFST) && (et < 2*OFST)) ? 1 : 0;
}

// checa se acc eh ponto flutuante
int facc(int et)
{
    return (et == 2*OFST) ? 1 : 0;
}

// checa se variavel eh ponto flutuante
int fmem(int et)
{
    return (et > 2*OFST) ? 1 : 0;
}

// aqui o filho chora e a mae nao ve
// executa as operacoes da ULA
// caso precise, swapa a posicao dos operandos pra chamar as instrucoes na ordem certa
// et1 -> reducao exp pro primeiro operando
// et2 -> reducao exp pro segundo  operando
// iop -> instrucao se ponto fixo
// fop -> instrucao se ponto flutuante
// op  -> retorna se a operacao feita foi em ponto flutuante pro proc em ponto fixo
// existem muitas possibilidades de combinacao, cada uma vai gerar um codigo diferente
int operacoes(int et1, int et2, char *iop, char *fop, int *op)
{
    // o proc eh em ponto flutuante
    if (prtype == 1)
    {
        if ((et1 % OFST != 0) && (et2 % OFST != 0))              // memoria e memoria
        {
            // LOAD no segundo operador
            load_check(et2,0);
            // faz operacao com o endereco de memoria de et1
            if (using_macro == 0) fprintf(f_asm, "%s %s\n", iop, v_name[et1 % OFST]);
        }
        else if ((et1 % OFST == 0) && (et2 % OFST != 0))         // acc e memoria
        {
            // LOAD no segundo operador jogando o primeiro pra pilha
            load_check(et2,0);
            // executa instrucao com a pilha
            if (using_macro == 0) fprintf(f_asm, "S%s\n", iop);
        }
        else if ((et1 % OFST != 0) && (et2 % OFST == 0))         // memoria e acc
        {
            // nao precisa dar load, basta fazer a instrucao com et1 na memoria
            if (using_macro == 0) fprintf(f_asm, "%s %s\n", iop, v_name[et1 % OFST]);
        }
        else                                                     // pilha e acc
        {
            // os dois operando ja estao no acc e pilha
            if (using_macro == 0) fprintf(f_asm, "S%s\n", iop);
        }
    }
    else // o proc eh ponto fixo (ai tem muuuitas possibilidades)
    {
        if (imem(et1) && imem(et2))                              // int memoria e int memoria
        {
            // carrega et2 depois faz instrucao com memoria de et1
            load_check(et2,0);
            if (using_macro == 0) fprintf(f_asm, "%s %s\n", iop, v_name[et1 % OFST]);
        }
        else if (imem(et1) && fmem(et2))                         // int memoria e float memoria
        {
            // carrega et1 e converte pra float
            load_check(et1,0);
            if (using_macro == 0) fprintf(f_asm, "CALL int2float\n");
            // carrega et2, jogando et1 pra pilha
            load_check(et2,0);
            // os dois operandos ja estao em ponto flutuante dentro do proc
            // entao executa macro de ponto flutuante correspondente
            if (using_macro == 0) fprintf(f_asm, "%s\n", fop);
            *op = 1;
            i2f = 1;
        }
        else if (fmem(et1) && imem(et2))                         // float memoria e int memoria
        {
            // carrega et1 (em ponto flutuante) pro acc
            load_check(et1,0);
            // joga et1 pra pilha e pega et2
            load_check(et2,0);
            // converte et2 pra float
            if (using_macro == 0) fprintf(f_asm, "CALL int2float\n");
            // execta a macro float correspondente
            if (using_macro == 0) fprintf(f_asm, "%s\n", fop);
            *op = 1;
            i2f = 1;
        }
        else if (fmem(et1) && fmem(et2))                         // float memoria e float memoria
        {
            // carrega as variaveis, ja em float, pro processador
            load_check(et1,0);
            load_check(et2,0);
            // executa macro correspondente
            if (using_macro == 0) fprintf(f_asm, "%s\n", fop);
            *op = 1;
        }
        else if (iacc(et1) && imem(et2))                         // int acc e int memoria
        {
            // joga et1 pra pilha e carrega et2
            load_check(et2,0);
            // executa instrucao sem operando (ja ta todo mundo dentro do proc)
            if (using_macro == 0) fprintf(f_asm, "S%s\n", iop);
        }
        else if (iacc(et1) && fmem(et2))                         // int acc e float memoria
        {
            // pega o et1 (que ja esta no acc) e converte pra float
            if (using_macro == 0) fprintf(f_asm, "CALL int2float\n");
            // empurra et1 pra pilha e carrega et2
            load_check(et2,0);
            // executa macro float
            if (using_macro == 0) fprintf(f_asm, "%s\n", fop);
            *op = 1;
            i2f = 1;
        }
        else if (facc(et1) && imem(et2))                         // float acc e int memoria
        {
            // xuxa et1 na pilha e carrega et2
            load_check(et2,0);
            // converte et2 pra float
            if (using_macro == 0) fprintf(f_asm, "CALL int2float\n");
            // executa a macro float
            if (using_macro == 0) fprintf(f_asm, "%s\n", fop);
            *op = 1;
            i2f = 1;
        }
        else if (facc(et1) && fmem(et2))                         // float acc e float memoria
        {
            // xuxa et1 na pilha e carrega et2
            load_check(et2,0);
            // execta macro float
            if (using_macro == 0) fprintf(f_asm, "%s\n", fop);
            *op = 1;
        }
        else if (imem(et1) && iacc(et2))                         // int memoria e int acc
        {
            // executa instrucao com et1 no argumento, sem pilha
            if (using_macro == 0) fprintf(f_asm, "%s %s\n", iop, v_name[et1 % OFST]);
        }
        else if (imem(et1) && facc(et2))                         // int memoria e float acc
        {
            if ((strcmp(iop, "DIV") == 0) ||
                (strcmp(iop, "LES") == 0) ||
                (strcmp(iop, "GRE") == 0))
                // em instrucoes onde a ordem importa (DIV, LES, GRE)
                // salva o acc em float_aux5 temporariamente, pra converter et1 pra float (nao pode jogar ele pra pilha)
                // salva et2 em float_aux5, carrega et1, converte pra float, empurra et1 na pilha e pega et2 de novo
            {
                if (using_macro == 0) fprintf(f_asm, "SET float_aux5\nLOAD %s\nCALL int2float\nPLD float_aux5\n", v_name[et1 % OFST]);
            }
                // depois daqui, o et1 ta na pilha e o et2 no acc (ordem correta pra chamar as macros)
            else
            {
                // se a ordem nao importa, pode xuxar et2 na pilha
                // xuxa et2 na pilha e pega et1
                load_check(et1, 0);
                // converte et1 pra float
                if (using_macro == 0) fprintf(f_asm, "CALL int2float\n");
            }
            // executa a macro da operacao
            if (using_macro == 0) fprintf(f_asm, "%s\n", fop);
            *op = 1;
            i2f = 1;
        }
        else if (fmem(et1) && iacc(et2))                         // float memoria e int acc
        {
            // converte et2 pra float
            if (using_macro == 0) fprintf(f_asm, "CALL int2float\n");

            // if a ordem importa
            if ((strcmp(iop, "DIV") == 0) ||
                (strcmp(iop, "LES") == 0) ||
                (strcmp(iop, "GRE") == 0))
            {
                // a ordem importa
                // entao salva et2 temporariamente em float_aux4
                if (using_macro == 0) fprintf(f_asm, "SET float_aux4\n");
                // diz que acc ta liberado
                acc_ok = 0;
                // pega et1
                load_check(et1, 0);
                // xuxa et1 pra pilha e pega et2
                if (using_macro == 0) fprintf(f_asm, "PLD float_aux4\n");
            }
            else
                // se a ordem nao importa
                // carrega et1
                load_check(et1, 0);

            // executa macro da operacao
            if (using_macro == 0) fprintf(f_asm, "%s\n", fop);
            *op = 1;
            i2f = 1;
        }
        else if (fmem(et1) && facc(et2))                         // float memoria e float acc
        {
            // se a ordem importa swapa as variaveis
            if ((strcmp(iop, "DIV") == 0) ||
                (strcmp(iop, "LES") == 0) ||
                (strcmp(iop, "GRE") == 0))
            {
                if (using_macro == 0) fprintf(f_asm, "SET float_aux4\n");
                acc_ok = 0;
                load_check(et1, 0);
                if (using_macro == 0) fprintf(f_asm, "PLD float_aux4\n");
            }
            else
                // se a ordem nao importa, xuxa et2 pra pilha e carrega et1
                load_check(et1, 0);

            if (using_macro == 0) fprintf(f_asm, "%s\n", fop);
            *op = 1;
        }
        else if (iacc(et1) && iacc(et2))                         // int pilha e int acc
        {
            // instrucoes com uso da pilha, ja na ordem certa
            if (using_macro == 0) fprintf(f_asm, "S%s\n", iop);
        }
        else if (iacc(et1) && facc(et2))                         // int pilha e float acc
        {
            // salva et2 em float_aux5 e pega a pilha
            // converte et1 pra float e empurra ele pra pilha, carregando o et2 de volta
            if (using_macro == 0) fprintf(f_asm, "SETP float_aux5\nCALL int2float\nPLD float_aux5\n%s\n", fop);
            *op = 1;
            i2f = 1;
        }
        else if (facc(et1) && iacc(et2))                         // float pilha e int acc
        {
            // converte et2 pra float
            if (using_macro == 0) fprintf(f_asm, "CALL int2float\n");
            // executa macro da operacao
            if (using_macro == 0) fprintf(f_asm, "%s\n", fop);
            *op = 1;
            i2f = 1;
        }
        else if (facc(et1) && facc(et2))                         // float pilha e float acc
        {
            // executa macro da operacao
            if (using_macro == 0) fprintf(f_asm, "%s\n", fop);
            *op = 1;
        }
    }

    return ((et1 >= 2*OFST) || (et2 >= 2*OFST)) ? 2*OFST : OFST; // retorna o tipo de dados em id extendido (pra exp)
}

// checa se uma operacao com inteiro pode ser executada
int int_oper(int et1, int et2, char *op, char *code, int fok)
{
    if ((prtype == 1) && (fok == 0))
        fprintf(stderr, "Erro na linha %d: processador em ponto flutuante n�o aceita %s. Voc� entendeu direito o que isso faz?\n", line_num+1, op);

    if ((prtype == 0) && ((et1 >= 2*OFST) || (et2 >= 2*OFST)))
        fprintf(stderr, "Erro na linha %d: uso incorreto de %s. Tem que passar tipo int.\n", line_num+1, op);

    // tem que botar et2 = 0, caso a funcao seja de um unico operador
    if (et2 == 0)
    {
        load_check(et1,0);

        if (using_macro == 0) fprintf(f_asm, "%s\n", code); // vai ser uma instrucao sem parametro
    }
    else
    {
        int aux;// variavel dummy, soh pra chamar a funcao "operacoes" corretamente
        operacoes(et1, et2, code, "", &aux);
    }

    return OFST; // retorna o id extendido de int
}

int oper_cmp(int et1, int et2, int op)
{
    switch (op)
    {
        case 0: operacoes(et1,et2, "LES", "CALL denorm\nLOAD float_aux3\nLES float_aux1", &fgen); // <
                break;
        case 1: operacoes(et1,et2, "GRE", "CALL denorm\nLOAD float_aux3\nGRE float_aux1", &fgen); // >
                break;
        case 2: operacoes(et1,et2, "LES", "CALL denorm\nLOAD float_aux3\nLES float_aux1", &fgen); // >=
                if (using_macro == 0) fprintf(f_asm, "LINV\n");
                break;
        case 3: operacoes(et1,et2, "GRE", "CALL denorm\nLOAD float_aux3\nGRE float_aux1", &fgen); // <=
                if (using_macro == 0) fprintf(f_asm, "LINV\n");
                break;
        case 4: operacoes(et1,et2, "EQU", "CALL denorm\nLOAD float_aux3\nEQU float_aux1", &fgen); // ==
                break;
        case 5: operacoes(et1,et2, "EQU", "CALL denorm\nLOAD float_aux3\nEQU float_aux1", &fgen); // !=
                if (using_macro == 0) fprintf(f_asm, "LINV\n");
                break;
    }

    return OFST;
}

int oper_ari(int et1, int et2, int op)
{
    int r, ret;

    switch (op)
    {
            case 0 : ret = operacoes(et1,et2, "MLT", "CALL float_mult"             , &fmlt); break; // *
            case 1 : ret = operacoes(et1,et2, "DIV", "CALL float_div"              , &fdiv); break; // /
            case 2 : ret = operacoes(et1,et2, "ADD", "CALL denorm\nCALL float_add" , &fadd); break; // +
            case 3 :   r = negacao  (et2);
                     ret = operacoes(et1,  r, "ADD", "CALL denorm\nCALL float_add" , &fadd); break; // -
            default: ret = 0;
    }

    return ret;
}
