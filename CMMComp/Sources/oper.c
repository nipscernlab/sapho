#include "..\Headers\oper.h"
#include "..\Headers\variaveis.h"
#include "..\Headers\t2t.h"
#include "..\Headers\diretivas.h"
#include "..\Headers\data_use.h"
#include "..\Headers\data_assign.h"
#include "..\Headers\stdlib.h"

#include <string.h>

// redeclaracao de variaveis globais
int fadd = 0; // se vai precisar de macros de ponto flutuante
int fdiv = 0; // se vai precisar de macros de ponto flutuante
int fmlt = 0; // se vai precisar de macros de ponto flutuante

int negacao(int et)
{
    int  etr, eti;
    char ld [10 ];
    char num[128];

    if (acc_ok == 0) strcpy(ld, "LOAD"); else strcpy(ld, "PLD");

    if (prtype == 0)
    {
        // se for um int constante na memoria
        if ((get_type(et) == 1) && (v_isco[et % OFST] == 1) && (et % OFST != 0))
        {
            add_instr("%s -%s\n", ld, v_name[et % OFST]);
        }

        // se for um int variavel na memoria
        if ((get_type(et) == 1) && (v_isco[et % OFST] == 0) && (et % OFST != 0))
        {
            add_instr("%s %s\n", ld, v_name[et % OFST]);
            add_instr("NEG\n");
        }

        // se for um int no acc
        if ((get_type(et) == 1) && (et % OFST == 0))
        {
            add_instr("NEG\n");
        }

        // se for um float constante na memoria
        if ((get_type(et) == 2) && (v_isco[et % OFST] == 1) && (et % OFST != 0))
        {
            sprintf  (num, "-%s",       v_name[et % OFST]);
            add_instr("%s %u // %s\n", ld, f2mf(num), num);
        }

        // se for um float variavel na memoria
        if ((get_type(et) == 2) && (v_isco[et % OFST] == 0) && (et % OFST != 0))
        {
            add_instr("%s %s\n", ld, v_name[et % OFST]);
            add_instr("PLD float_nbits\n");
            add_instr("SHL 1\n");
            add_instr("SADD\n");
        }

        // se for um float no acc
        if ((get_type(et) == 2) && (et % OFST == 0))
        {
            add_instr("PLD float_nbits\n");
            add_instr("SHL 1\n");
            add_instr("SADD\n");
        }

        // se for um comp constante na memoria
        if (get_type(et) == 5)
        {
            split_cmp_const(et,&etr,&eti);

            // the real constant is never negative
            sprintf  (num, "-%s",       v_name[etr % OFST]);
            add_instr("%s %u // -%s\n", ld, f2mf(num), num);
            
            // the imaginary constant must be negated by code
            sprintf  (num, "%s",    v_name[eti % OFST]);
            add_instr("PLD %u // %s\n", f2mf(num), num);
            add_instr("PLD float_nbits\n");
            add_instr("SHL 1\n");
            add_instr("SADD\n");
        }

        // se for um comp variavel na memoria
        if ((get_type(et) == 3) && (et % OFST != 0))
        {
            get_cmp_ets(et,&etr,&eti);

            add_instr("%s %s\n", ld, v_name[etr % OFST]);
            add_instr("PLD float_nbits\n");
            add_instr("SHL 1\n");
            add_instr("SADD\n");

            add_instr("PLD %s\n", v_name[eti % OFST]);
            add_instr("PLD float_nbits\n");
            add_instr("SHL 1\n");
            add_instr("SADD\n");
        }

        // se for um comp no acc
        if ((get_type(et) == 3) && (et % OFST == 0))
        {
            add_instr("PLD float_nbits\n");
            add_instr("SHL 1\n");
            add_instr("SADD\n");
            add_instr("SETP neg_aux\n");

            add_instr("PLD float_nbits\n");
            add_instr("SHL 1\n");
            add_instr("SADD\n");
            add_instr("PLD neg_aux\n");
        }
    }
    else
    {
        // se for um int constante na memoria
        if ((get_type(et) == 1) && (v_isco[et % OFST] == 1) && (et % OFST != 0))
        {
            add_instr("%s -%s\n", ld, v_name[et % OFST]);
        }

        // se for um int variavel na memoria
        if ((get_type(et) == 1) && (v_isco[et % OFST] == 0) && (et % OFST != 0))
        {
            add_instr("%s %s\n", ld, v_name[et % OFST]);
            add_instr("NEG\n");
        }

        // se for um int no acc
        if ((get_type(et) == 1) && (et % OFST == 0))
        {
            add_instr("NEG\n");
        }

        // se for um float constante na memoria
        if ((get_type(et) == 2) && (v_isco[et % OFST] == 1) && (et % OFST != 0))
        {
            add_instr("%s -%s\n", ld, v_name[et % OFST]);
        }

        // se for um float variavel na memoria
        if ((get_type(et) == 2) && (v_isco[et % OFST] == 0) && (et % OFST != 0))
        {
            add_instr("%s %s\n", ld, v_name[et % OFST]);
            add_instr("NEG\n");
        }

        // se for um float no acc
        if ((get_type(et) == 2) && (et % OFST == 0))
        {
            add_instr("NEG\n");
        }

        // se for um comp constante na memoria
        if (get_type(et) == 5)
        {
            split_cmp_const(et,&etr,&eti);

            // the real constant is never negative
            add_instr("%s -%s\n", ld, v_name[etr % OFST]);
            
            // the imaginary constant must be negated by code
            add_instr("PLD %s\n" ,    v_name[eti % OFST]);
            add_instr("NEG\n");
        }

        // se for um comp variavel na memoria
        if ((get_type(et) == 3) && (et % OFST != 0))
        {
            get_cmp_ets(et,&etr,&eti);

            add_instr("%s %s\n" , ld, v_name[etr % OFST]);
            add_instr("NEG\n");
            add_instr("PLD %s\n",     v_name[eti % OFST]);
            add_instr("NEG\n");
        }

        // se for um comp no acc
        if ((get_type(et) == 3) && (et % OFST == 0))
        {
            add_instr("NEG\n");
            add_instr("SETP neg_aux\n");

            add_instr("NEG\n");
            add_instr("PLD neg_aux\n");
        }
    }

    acc_ok = 1;
    if (get_type(et) == 5) et = 3*OFST;

    return get_type(et)*OFST;
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
    return (et >  2*OFST) ? 1 : 0;
}

// implementa operacoes com proc em ponto flutuante
void oper_fl(int et1, int et2, char *iop)
{
    // memoria e memoria ------------------------------------------------------

    if ((et1 % OFST != 0) && (et2 % OFST != 0))
    {
        // LOAD no segundo operador
        load_check(et2,0);
        // faz operacao com o endereco de memoria de et1
        add_instr("%s %s\n", iop, v_name[et1 % OFST]);
    }

    // acc e memoria ----------------------------------------------------------

    if ((et1 % OFST == 0) && (et2 % OFST != 0))
    {
        // LOAD no segundo operador jogando o primeiro pra pilha
        load_check(et2,0);
        // executa instrucao com a pilha
        add_instr("S%s\n", iop);
    }

    // memoria e acc ----------------------------------------------------------

    if ((et1 % OFST != 0) && (et2 % OFST == 0))
    {
        // nao precisa dar load, basta fazer a instrucao com et1 na memoria
        add_instr("%s %s\n", iop, v_name[et1 % OFST]);
    }

    // pilha e acc ------------------------------------------------------------

    if ((et1 % OFST == 0) && (et2 % OFST == 0))
    {
        // os dois operando ja estao no acc e pilha
        add_instr("S%s\n", iop);
    }
}

// implementa operacoes com proc em ponto fixo
void oper_fx(int et1, int et2, char *iop, char *fop, int *op, int nin)
{
    // int memoria e int memoria ----------------------------------------------

    if (imem(et1) && imem(et2))
    {
        // carrega et2 depois faz instrucao com memoria de et1
        load_check(et2,0);
        add_instr("%s %s\n", iop, v_name[et1 % OFST]);
    }

    // int memoria e float memoria --------------------------------------------

    if (imem(et1) && fmem(et2))
    {
        // carrega et1 e converte pra float
        load_check(et1,0);
        add_instr("CALL int2float\n");
        // carrega et2, jogando et1 pra pilha
        load_check(et2,0);
        // os dois operandos ja estao em ponto flutuante dentro do proc
        // entao executa macro de ponto flutuante correspondente
        fprintf(f_asm, "%s\n", fop);
        for (int i = 0; i<nin; i++) is_macro();
        *op = 1;
        i2f = 1;
    }

    // float memoria e int memoria --------------------------------------------

    if (fmem(et1) && imem(et2))
    {
        // carrega et1 (em ponto flutuante) pro acc
        load_check(et1,0);
        // joga et1 pra pilha e pega et2
        load_check(et2,0);
        // converte et2 pra float
        add_instr("CALL int2float\n");
        // execta a macro float correspondente
        fprintf(f_asm, "%s\n", fop);
        for (int i = 0; i<nin; i++) is_macro();
        *op = 1;
        i2f = 1;
    }

    // float memoria e float memoria ------------------------------------------

    if (fmem(et1) && fmem(et2))
    {
        // carrega as variaveis, ja em float, pro processador
        load_check(et1,0);
        load_check(et2,0);
        // executa macro correspondente
        fprintf(f_asm, "%s\n", fop);
        for (int i = 0; i<nin; i++) is_macro();
        *op = 1;
    }

    // int acc e int memoria --------------------------------------------------

    if (iacc(et1) && imem(et2))
    {
        // joga et1 pra pilha e carrega et2
        load_check(et2,0);
        // executa instrucao sem operando (ja ta todo mundo dentro do proc)
        add_instr("S%s\n", iop);
    }

    // int acc e float memoria ------------------------------------------------

    if (iacc(et1) && fmem(et2))
    {
        // pega o et1 (que ja esta no acc) e converte pra float
        add_instr("CALL int2float\n");
        // empurra et1 pra pilha e carrega et2
        load_check(et2,0);
        // executa macro float
        fprintf(f_asm, "%s\n", fop);
        for (int i = 0; i<nin; i++) is_macro();
        *op = 1;
        i2f = 1;
    }

    // float acc e int memoria ------------------------------------------------

    if (facc(et1) && imem(et2))
    {
        // xuxa et1 na pilha e carrega et2
        load_check(et2,0);
        // converte et2 pra float
        add_instr("CALL int2float\n");
        // executa a macro float
        fprintf(f_asm, "%s\n", fop);
        for (int i = 0; i<nin; i++) is_macro();
        *op = 1;
        i2f = 1;
    }

    // float acc e float memoria ----------------------------------------------

    if (facc(et1) && fmem(et2))
    {
        // xuxa et1 na pilha e carrega et2
        load_check(et2,0);
        // execta macro float
        fprintf(f_asm, "%s\n", fop);
        for (int i = 0; i<nin; i++) is_macro();
        *op = 1;
    }

    // int memoria e int acc --------------------------------------------------

    if (imem(et1) && iacc(et2))
    {
        // executa instrucao com et1 no argumento, sem pilha
        add_instr("%s %s\n", iop, v_name[et1 % OFST]);
    }

    // int memoria e float acc ------------------------------------------------

    if (imem(et1) && facc(et2))
    {
        if ((strcmp(iop, "DIV") == 0) ||
            (strcmp(iop, "LES") == 0) ||
            (strcmp(iop, "GRE") == 0))
            // em instrucoes onde a ordem importa (DIV, LES, GRE)
            // salva o acc em float_aux5 temporariamente, pra converter et1 pra float (nao pode jogar ele pra pilha)
            // salva et2 em float_aux5, carrega et1, converte pra float, empurra et1 na pilha e pega et2 de novo
        {
            add_instr("SET float_aux5\n");
            add_instr("LOAD %s\n", v_name[et1 % OFST]);
            add_instr("CALL int2float\n");
            add_instr("PLD float_aux5\n");
        }
            // depois daqui, o et1 ta na pilha e o et2 no acc (ordem correta pra chamar as macros)
        else
        {
            // se a ordem nao importa, pode xuxar et2 na pilha
            // xuxa et2 na pilha e pega et1
            load_check(et1, 0);
            // converte et1 pra float
            add_instr("CALL int2float\n");
        }
        // executa a macro da operacao
        fprintf(f_asm, "%s\n", fop);
        for (int i = 0; i<nin; i++) is_macro();
        *op = 1;
        i2f = 1;
    }

    // float memoria e int acc ------------------------------------------------

    if (fmem(et1) && iacc(et2))
    {
        // converte et2 pra float
        add_instr("CALL int2float\n");

        // if a ordem importa
        if ((strcmp(iop, "DIV") == 0) ||
            (strcmp(iop, "LES") == 0) ||
            (strcmp(iop, "GRE") == 0))
        {
            // a ordem importa
            // entao salva et2 temporariamente em float_aux4
            add_instr("SET float_aux4\n");
            // diz que acc ta liberado
            acc_ok = 0;
            // pega et1
            load_check(et1, 0);
            // xuxa et1 pra pilha e pega et2
            add_instr("PLD float_aux4\n");
        }
        else
            // se a ordem nao importa
            // carrega et1
            load_check(et1, 0);

        // executa macro da operacao
        fprintf(f_asm, "%s\n", fop);
        for (int i = 0; i<nin; i++) is_macro();
        *op = 1;
        i2f = 1;
    }

    // float memoria e float acc ----------------------------------------------

    if (fmem(et1) && facc(et2))
    {
        // se a ordem importa swapa as variaveis
        if ((strcmp(iop, "DIV") == 0) ||
            (strcmp(iop, "LES") == 0) ||
            (strcmp(iop, "GRE") == 0))
        {
            add_instr("SET float_aux4\n");
            acc_ok = 0;
            load_check(et1, 0);
            add_instr("PLD float_aux4\n");
        }
        else
            // se a ordem nao importa, xuxa et2 pra pilha e carrega et1
            load_check(et1, 0);

        fprintf(f_asm, "%s\n", fop);
        for (int i = 0; i<nin; i++) is_macro();
        *op = 1;
    }

    // int pilha e int acc ----------------------------------------------------

    if (iacc(et1) && iacc(et2))
    {
        // instrucoes com uso da pilha, ja na ordem certa
        add_instr("S%s\n", iop);
    }

    // int pilha e float acc --------------------------------------------------

    if (iacc(et1) && facc(et2))
    {
        // salva et2 em float_aux5 e pega a pilha
        // converte et1 pra float e empurra ele pra pilha, carregando o et2 de volta
        add_instr("SETP float_aux5\n");
        add_instr("CALL int2float\n");
        add_instr("PLD float_aux5\n");
        fprintf(f_asm, "%s\n", fop);
        for (int i = 0; i<nin; i++) is_macro();
        *op = 1;
        i2f = 1;
    }

    // float pilha e int acc --------------------------------------------------

    if (facc(et1) && iacc(et2))
    {
        // converte et2 pra float
        add_instr("CALL int2float\n");
        // executa macro da operacao
        fprintf(f_asm, "%s\n", fop);
        for (int i = 0; i<nin; i++) is_macro();
        *op = 1;
        i2f = 1;
    }

    // float pilha e float acc ------------------------------------------------

    if (facc(et1) && facc(et2))
    {
        // executa macro da operacao
        fprintf(f_asm, "%s\n", fop);
        for (int i = 0; i<nin; i++) is_macro();
        *op = 1;
    }
}

// executa as operacoes da ULA
// et1 -> reducao exp pro primeiro operando
// et2 -> reducao exp pro segundo  operando
// iop -> string da instrucao se ponto fixo
// fop -> string da instrucao se ponto flutuante
// op  -> retorna se a operacao feita foi em ponto flutuante pro proc em ponto fixo
// existem muitas possibilidades de combinacao, cada uma vai gerar um codigo diferente
int operacoes(int et1, int et2, char *iop, char *fop, int *op, int nin)
{
    if (prtype == 1)
        oper_fl(et1,et2,iop);
    else
        oper_fx(et1,et2,iop,fop,op,nin);

    acc_ok = 1;

    return ((et1 >= 2*OFST) || (et2 >= 2*OFST)) ? 2*OFST : OFST; // retorna o tipo de dados em id extendido (pra exp)
}

// operacoes que resultam em inteiro devem ser feitas por aqui
// ex: deslocamento de bits, inversao de bits, etc
// fok diz se tal operacao foi implementada no proc em ponto flutuante
int int_oper(int et1, int et2, char *op, char *code, int fok)
{
    if ((get_type(et1) > 1) || (get_type(et2) > 1))
        fprintf(stderr, "Erro na linha %d: uso incorreto de %s. Tem que passar tipo int. Viajou?\n", line_num+1, op);

    // tem que botar et2 = 0, caso a funcao seja de um unico operador
    // ex: inversao de bit
    if (et2 == 0)
    {
        load_check(et1,0);
        add_instr("%s\n", code); // vai ser uma instrucao sem parametro
    }
    else
    {
        int aux1, aux2;// variavel dummy, soh pra chamar a funcao "operacoes" corretamente
        operacoes(et1, et2, code, "", &aux1, aux2);
    }

    return OFST; // retorna o id extendido de int
}

int oper_inv(int et)
{
    if (get_type(et) > 1)
        fprintf(stderr, "Erro na linha %d: uso incorreto do operador '~'. Tem que passar tipo int. Viajou?\n", line_num+1);

    int etr, eti;

    char ld[10];
    if (acc_ok == 0) strcpy(ld,"LOAD"); else strcpy(ld,"PLD");

    // se for um int na memoria
    if ((get_type(et) == 1) && (et % OFST != 0)) add_instr("%s %s\n", ld, v_name[et%OFST]);
    add_instr("INV\n");

    acc_ok = 1;

    return OFST; // retorna o id extendido de int
}

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

    char ld[10];
    if (acc_ok == 0) strcpy(ld,"LOAD"); else strcpy(ld,"PLD");

    if (prtype == 0)
    {
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
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("%s %s\n", op, v_name[et1%OFST]);
        }

        // int var com float const
        if ((get_type(et1) == 1) && (et1%OFST != 0) && (get_type(et2) == 2) && (et2%OFST != 0) && (v_isco[et2%OFST]==1))
        {
            fprintf(stdout, "Atenção na linha %d: o segundo operando do shift tá dando float. Aí você me quebra!\n", line_num+1);

            add_instr("%s %d // %s\n", ld, f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("%s %s\n", op, v_name[et1%OFST]);
        }

        // int var com float acc
        if ((get_type(et1) == 1) && (et1%OFST != 0) && (get_type(et2) == 2) && (et2%OFST == 0))
        {
            add_instr("CALL float2int\n"); f2i = 1;
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
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("S%s\n", op);
        }

        // int acc com float const
        if ((get_type(et1) == 1) && (et1%OFST == 0) && (get_type(et2) == 2) && (et2%OFST != 0) && (v_isco[et2%OFST]==1))
        {
            fprintf(stdout, "Atenção na linha %d: o segundo operando do shift tá dando float. Aí você me quebra!\n", line_num+1);

            add_instr("PLD %d // %s\n", f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("S%s\n", op);
        }

        // int acc com float acc
        if ((get_type(et1) == 1) && (et1%OFST == 0) && (get_type(et2) == 2) && (et2%OFST == 0))
        {
            fprintf(stdout, "Atenção na linha %d: o segundo operando do shift tá dando float. Aí você me quebra!\n", line_num+1);
            
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("S%s\n", op);
        }
    }
    else
    {
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
            add_instr("%s %s\n", op, v_name[et1%OFST]);
        }

        // int var com float const
        if ((get_type(et1) == 1) && (et1%OFST != 0) && (get_type(et2) == 2) && (et2%OFST != 0) && (v_isco[et2%OFST]==1))
        {
            fprintf(stdout, "Atenção na linha %d: o segundo operando do shift tá dando float. Aí você me quebra!\n", line_num+1);

            add_instr("%s %d // %s\n", ld, f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
            add_instr("%s %s\n", op, v_name[et1%OFST]);
        }

        // int var com float acc
        if ((get_type(et1) == 1) && (et1%OFST != 0) && (get_type(et2) == 2) && (et2%OFST == 0))
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

        // int acc com float var
        if ((get_type(et1) == 1) && (et1%OFST == 0) && (get_type(et2) == 2) && (et2%OFST != 0) && (v_isco[et2%OFST]==0))
        {
            fprintf(stdout, "Atenção na linha %d: o segundo operando do shift tá dando float. Aí você me quebra!\n", line_num+1);

            add_instr("PLD %s\n", v_name[et2%OFST]);
            add_instr("S%s\n", op);
        }

        // int acc com float const
        if ((get_type(et1) == 1) && (et1%OFST == 0) && (get_type(et2) == 2) && (et2%OFST != 0) && (v_isco[et2%OFST]==1))
        {
            fprintf(stdout, "Atenção na linha %d: o segundo operando do shift tá dando float. Aí você me quebra!\n", line_num+1);

            add_instr("PLD %d // %s\n", f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
            add_instr("S%s\n", op);
        }

        // int acc com float acc
        if ((get_type(et1) == 1) && (et1%OFST == 0) && (get_type(et2) == 2) && (et2%OFST == 0))
        {
            fprintf(stdout, "Atenção na linha %d: o segundo operando do shift tá dando float. Aí você me quebra!\n", line_num+1);

            add_instr("S%s\n", op);
        }
    }

    acc_ok = 1;

    return OFST; // retorna o id extendido de int
}

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
    if (acc_ok == 0) strcpy(ld,"LOAD"); else strcpy(ld,"PLD");

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

int oper_linv(int et)
{
    int etr, eti;

    char ld[10];
    if (acc_ok == 0) strcpy(ld,"LOAD"); else strcpy(ld,"PLD");

    if (prtype == 0)
    {
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

            add_instr("%s %s\n", ld, v_name[et%OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("LINV\n");
        }

        // se for um float const na memoria
        if ((get_type(et) == 2) && (et % OFST != 0) && (v_isco[et%OFST] == 1))
        {
            fprintf(stdout, "Atenção na linha %d: expressão lógica com float? Você é uma pessoa confusa!\n", line_num+1);

            add_instr("%s %d // %s\n", ld, f2mf(v_name[et%OFST]), v_name[et%OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("LINV\n");
        }

        // se for um float no acc
        if ((get_type(et) == 2) && (et % OFST == 0))
        {
            fprintf(stdout, "Atenção na linha %d: expressão lógica com float? Você é uma pessoa confusa!\n", line_num+1);

            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("LINV\n");
        }

        // se for um comp const
        if (get_type(et) == 5)
        {
            fprintf(stdout, "Atenção na linha %d: expressão lógica com comp? Sério? Vou arredondar a parte real!\n", line_num+1);

            split_cmp_const(et,&etr,&eti);

            add_instr("%s %d // %s\n", ld, f2mf(v_name[etr%OFST]), v_name[etr%OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("LINV\n");
        }

        // se for um comp var
        if ((get_type(et) == 3) && (et % OFST != 0))
        {
            fprintf(stdout, "Atenção na linha %d: expressão lógica com comp? Sério? Vou arredondar a parte real!\n", line_num+1);

            add_instr("%s %s\n", ld, v_name[et%OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("LINV\n");
        }

        // se for um comp no acc
        if ((get_type(et) == 3) && (et % OFST == 0))
        {
            fprintf(stdout, "Atenção na linha %d: expressão lógica com comp? Sério? Vou arredondar a parte real!\n", line_num+1);

            add_instr("SETP aux_lixo\n");
            add_instr("CALL float2int\n"); f2i = 1;
            add_instr("LINV\n");
        }
    }
    else
    {
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
        if ((get_type(et) == 2) && (et % OFST != 0))
        {
            fprintf(stdout, "Atenção na linha %d: expressão lógica com float? Você é uma pessoa confusa!\n", line_num+1);

            add_instr("%s %s\n", ld, v_name[et%OFST]);
            add_instr("LINV\n");
        }

        // se for um float no acc
        if ((get_type(et) == 2) && (et % OFST == 0))
        {
            fprintf(stdout, "Atenção na linha %d: expressão lógica com float? Você é uma pessoa confusa!\n", line_num+1);

            add_instr("LINV\n");
        }

        // se for um comp const
        if (get_type(et) == 5)
        {
            fprintf(stdout, "Atenção na linha %d: expressão lógica com comp? Sério? Vou arredondar a parte real!\n", line_num+1);

            split_cmp_const(et,&etr,&eti);

            add_instr("%s %s\n", ld, v_name[etr%OFST]);
            add_instr("LINV\n");
        }

        // se for um comp var
        if ((get_type(et) == 3) && (et % OFST != 0))
        {
            fprintf(stdout, "Atenção na linha %d: expressão lógica com comp? Sério? Vou arredondar a parte real!\n", line_num+1);

            add_instr("%s %s\n", ld, v_name[et%OFST]);
            add_instr("LINV\n");
        }

        // se for um comp no acc
        if ((get_type(et) == 3) && (et % OFST == 0))
        {
            fprintf(stdout, "Atenção na linha %d: expressão lógica com comp? Sério? Vou arredondar a parte real!\n", line_num+1);

            add_instr("SETP aux_lixo\n");
            add_instr("LINV\n");
        }
    }

    acc_ok = 1;

    return OFST; // retorna o id extendido de int
}

int oper_mod(int et1, int et2)
{
    if ((get_type(et1) > 1) || (get_type(et2) > 1))
        fprintf(stderr, "Erro na linha %d: qual o sentido de calcular o resto da divisão ser com número inteiro? Vai se tratar!\n", line_num+1);

    char ld[10];
    if (acc_ok == 0) strcpy(ld,"LOAD"); else strcpy(ld,"PLD");

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

int oper_soma(int et1, int et2)
{
    int etr, eti;

    char ld[10];
    if (acc_ok == 0) strcpy(ld,"LOAD"); else strcpy(ld,"PLD");

    // 8*8 combinacoes para prtype = 0
    if (prtype == 0)
    {
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
        if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==0))
        {
            add_instr("%s %s\n" , ld, v_name[et1%OFST]);
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("PLD %s\n" , v_name[et2%OFST]);
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
        }

        // int var com float const
        if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
        {
            add_instr("%s %s\n" , ld, v_name[et1%OFST]);
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("PLD %d // %s\n" , f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
        }

        // int var com float acc
        if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==2) && (et2%OFST==0))
        {
            add_instr("PLD %s\n", v_name[et1%OFST]);
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
        }

        // int var com comp const
        if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==5))
        {
            split_cmp_const(et2,&etr,&eti);

            add_instr("%s %d // %s\n", ld, f2mf(v_name[etr%OFST]), v_name[etr%OFST]);
            add_instr("PLD %s\n", v_name[et1%OFST]);
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
            add_instr("PLD %d // %s\n", f2mf(v_name[eti%OFST]), v_name[eti%OFST]);
        }

        // int var com comp var
        if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==3) && (et2%OFST!=0))
        {
            get_cmp_ets(et2,&etr,&eti);

            add_instr("%s %s\n", ld, v_name[etr%OFST]);
            add_instr("PLD %s\n", v_name[et1%OFST]);
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
            add_instr("PLD %s\n", v_name[eti%OFST]);
        }

        // int var com comp acc
        if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==3) && (et2%OFST==0))
        {
            add_instr("SETP aux_soma\n");
            add_instr("PLD %s\n", v_name[et1%OFST]);
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
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
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("PLD %s\n", v_name[et2%OFST]);
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
        }

        // int acc com float const
        if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
        {
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("PLD %d // %s\n", f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
        }

        // int acc com float acc
        if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST==0))
        {
            add_instr("SETP aux_soma\n");
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("PLD aux_soma\n");
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
        }

        // int acc com comp const
        if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==5))
        {
            split_cmp_const(et2,&etr,&eti);

            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("PLD %d // %s\n", f2mf(v_name[etr%OFST]), v_name[etr%OFST]);
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
            add_instr("PLD %d // %s\n", f2mf(v_name[eti%OFST]), v_name[eti%OFST]);
        }

        // int acc com comp var
        if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==3) && (et2%OFST!=0))
        {
            get_cmp_ets(et2,&etr,&eti);

            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("PLD %s\n", v_name[etr%OFST]);
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
            add_instr("PLD %s\n", v_name[eti%OFST]);
        }

        // int acc com comp acc
        if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==3) && (et2%OFST==0))
        {
            add_instr("SETP aux_imag\n");
            add_instr("SETP aux_real\n");
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("PLD aux_real\n");
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
            add_instr("PLD aux_imag\n");
        }

        // float var com int var
        if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==1) && (et2%OFST!=0))
        {
            add_instr("%s %s\n" , ld, v_name[et2%OFST]);
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("PLD %s\n",     v_name[et1%OFST]);
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
        }

        // float var com int acc
        if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==1) && (et2%OFST==0))
        {
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("PLD %s\n",     v_name[et1%OFST]);
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
        }

        // float var com float var
        if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==0))
        {
            add_instr("%s %s\n" , ld, v_name[et1%OFST]);
            add_instr("PLD %s\n",     v_name[et2%OFST]);
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
        }

        // float var com float const
        if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
        {
            add_instr("%s %s\n", ld, v_name[et1%OFST]);
            add_instr("PLD %d // %s\n", f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
        }

        // float var com float acc
        if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==2) && (et2%OFST==0))
        {
            add_instr("PLD %s\n", v_name[et1%OFST]);
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
        }

        // float var com comp const
        if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==5))
        {
            split_cmp_const(et2,&etr,&eti);

            add_instr("%s %s\n", ld, v_name[et1%OFST]);
            add_instr("PLD %d // %s\n", f2mf(v_name[etr%OFST]), v_name[etr%OFST]);
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
            add_instr("PLD %d // %s\n", f2mf(v_name[eti%OFST]), v_name[eti%OFST]);
        }

        // float var com comp var
        if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==3) && (et2%OFST!=0))
        {
            get_cmp_ets(et2,&etr,&eti);

            add_instr("%s %s\n", ld, v_name[et1%OFST]);
            add_instr("PLD %s\n", v_name[etr%OFST]);
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
            add_instr("PLD %s\n", v_name[eti%OFST]);
        }

        // float var com comp acc
        if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==3) && (et2%OFST==0))
        {
            add_instr("SETP aux_soma\n");
            add_instr("PLD %s\n", v_name[et1%OFST]);
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
            add_instr("PLD aux_soma\n");
        }

        // float const com int var
        if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==1) && (et2%OFST!=0))
        {
            add_instr("%s %s\n" , ld, v_name[et2%OFST]);
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("PLD %d // %s\n", f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
        }

        // float const com int acc
        if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==1) && (et2%OFST==0))
        {
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("PLD %d // %s\n", f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
        }

        // float const com float var
        if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==0))
        {
            add_instr("%s %d // %s\n", ld, f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
            add_instr("PLD %s\n",     v_name[et2%OFST]);
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
        }

        // float const com float const
        if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
        {
            add_instr("%s %d // %s\n", ld, f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
            add_instr("PLD %d // %s\n", f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
        }

        // float const com float acc
        if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==2) && (et2%OFST==0))
        {
            add_instr("PLD %d // %s\n", f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
        }

        // float const com comp const
        if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==5))
        {
            split_cmp_const(et2,&etr,&eti);

            add_instr("%s %d // %s\n", ld, f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
            add_instr("PLD %d // %s\n", f2mf(v_name[etr%OFST]), v_name[etr%OFST]);
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
            add_instr("PLD %d // %s\n", f2mf(v_name[eti%OFST]), v_name[eti%OFST]);
        }

        // float const com comp var
        if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==3) && (et2%OFST!=0))
        {
            get_cmp_ets(et2,&etr,&eti);

            add_instr("%s %d // %s\n", ld, f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
            add_instr("PLD %s\n", v_name[etr%OFST]);
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
            add_instr("PLD %s\n", v_name[eti%OFST]);
        }

        // float const com comp acc
        if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==3) && (et2%OFST==0))
        {
            add_instr("SETP aux_soma\n");
            add_instr("PLD %d // %s\n", f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
            add_instr("PLD aux_soma\n");
        }

        // float acc com int var
        if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==1) && (et2%OFST!=0))
        {
            add_instr("PLD %s\n", v_name[et2%OFST]);
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
        }

        // float acc com int acc
        if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==1) && (et2%OFST==0))
        {
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
        }

        // float acc com float var
        if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==0))
        {
            add_instr("PLD %s\n", v_name[et2%OFST]);
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
        }

        // float acc com float const
        if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
        {
            add_instr("PLD %d // %s\n", f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
        }

        // float acc com float acc
        if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST==0))
        {
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
        }

        // float acc com comp const
        if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==5))
        {
            split_cmp_const(et2,&etr,&eti);

            add_instr("PLD %d // %s\n", f2mf(v_name[etr%OFST]), v_name[etr%OFST]);
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
            add_instr("PLD %d // %s\n", f2mf(v_name[eti%OFST]), v_name[eti%OFST]);
        }

        // float acc com comp var
        if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==3) && (et2%OFST!=0))
        {
            get_cmp_ets(et2,&etr,&eti);

            add_instr("PLD %s\n", v_name[etr%OFST]);
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
            add_instr("PLD %s\n", v_name[eti%OFST]);
        }

        // float acc com comp acc
        if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==3) && (et2%OFST==0))
        {
            add_instr("SETP aux_imag\n");
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
            add_instr("PLD aux_imag\n");
        }

        // comp const com int var
        if ((get_type(et1)==5) && (get_type(et2)==1) && (et2%OFST!=0))
        {
            split_cmp_const(et1,&etr,&eti);

            add_instr("%s %s\n", ld, v_name[et2%OFST]);
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("PLD %d // %s\n", f2mf(v_name[etr%OFST]), v_name[etr%OFST]);
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
            add_instr("PLD %d // %s\n", f2mf(v_name[eti%OFST]), v_name[eti%OFST]);
        }

        // comp const com int acc
        if ((get_type(et1)==5) && (get_type(et2)==1) && (et2%OFST==0))
        {
            split_cmp_const(et1,&etr,&eti);

            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("PLD %d // %s\n", f2mf(v_name[etr%OFST]), v_name[etr%OFST]);
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
            add_instr("PLD %d // %s\n", f2mf(v_name[eti%OFST]), v_name[eti%OFST]);
        }

        // comp const com float var
        if ((get_type(et1)==5) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==0))
        {
            split_cmp_const(et1,&etr,&eti);

            add_instr("%s %s\n", ld, v_name[et2%OFST]);
            add_instr("PLD %d // %s\n", f2mf(v_name[etr%OFST]), v_name[etr%OFST]);
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
            add_instr("PLD %d // %s\n", f2mf(v_name[eti%OFST]), v_name[eti%OFST]);
        }

        // comp const com float const
        if ((get_type(et1)==5) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
        {
            split_cmp_const(et1,&etr,&eti);

            add_instr("%s %d // %s\n", ld, f2mf(v_name[etr%OFST]), v_name[etr%OFST]);
            add_instr("PLD %d // %s\n", f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
            add_instr("PLD %d // %s\n", f2mf(v_name[eti%OFST]), v_name[eti%OFST]);
        }

        // comp const com float acc
        if ((get_type(et1)==5) && (get_type(et2)==2) && (et2%OFST==0))
        {
            split_cmp_const(et1,&etr,&eti);

            add_instr("PLD %d // %s\n", f2mf(v_name[etr%OFST]), v_name[etr%OFST]);
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
            add_instr("PLD %d // %s\n", f2mf(v_name[eti%OFST]), v_name[eti%OFST]);
        }

        // comp const com comp const
        if ((get_type(et1)==5) && (get_type(et2)==5))
        {
            int et1r,et2r;
            int et1i,et2i;

            split_cmp_const(et1,&et1r,&et1i);
            split_cmp_const(et2,&et2r,&et2i);

            add_instr("%s %d // %s\n", ld, f2mf(v_name[et1r%OFST]), v_name[et1r%OFST]);
            add_instr("PLD %d // %s\n",    f2mf(v_name[et2r%OFST]), v_name[et2r%OFST]);
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;

            add_instr("PLD %d // %s\n", f2mf(v_name[et1i%OFST]), v_name[et1i%OFST]);
            add_instr("PLD %d // %s\n", f2mf(v_name[et2i%OFST]), v_name[et2i%OFST]);
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
        }

        // comp const com comp var
        if ((get_type(et1)==5) && (get_type(et2)==3) && (et2%OFST!=0))
        {
            int et1r,et2r;
            int et1i,et2i;

            split_cmp_const(et1,&et1r,&et1i);
            get_cmp_ets    (et2,&et2r,&et2i);

            add_instr("%s %d // %s\n", ld, f2mf(v_name[et1r%OFST]), v_name[et1r%OFST]);
            add_instr("PLD %s\n", v_name[et2r%OFST]);
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;

            add_instr("PLD %d // %s\n", f2mf(v_name[et1i%OFST]), v_name[et1i%OFST]);
            add_instr("PLD %s\n", v_name[et2i%OFST]);
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
        }

        // comp const com comp acc
        if ((get_type(et1)==5) && (get_type(et2)==3) && (et2%OFST==0))
        {
            split_cmp_const(et1,&etr,&eti);

            add_instr("SETP aux_soma\n");
            add_instr("PLD %d // %s\n", f2mf(v_name[etr%OFST]), v_name[etr%OFST]);
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;

            add_instr("PLD %d // %s\n", f2mf(v_name[eti%OFST]), v_name[eti%OFST]);
            add_instr("PLD aux_soma\n");
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
        }

        // comp var com int var
        if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==1) && (et2%OFST!=0))
        {
            get_cmp_ets(et1,&etr,&eti);

            add_instr("%s %s\n", ld, v_name[et2%OFST]);
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("PLD %s\n", v_name[etr%OFST]);
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
            add_instr("PLD %s\n", v_name[eti%OFST]);
        }

        // comp var com int acc
        if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==1) && (et2%OFST==0))
        {
            get_cmp_ets(et1,&etr,&eti);

            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("PLD %s\n", v_name[etr%OFST]);
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
            add_instr("PLD %s\n", v_name[eti%OFST]);
        }

        // comp var com float var
        if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==0))
        {
            get_cmp_ets(et1,&etr,&eti);

            add_instr("%s %s\n", ld, v_name[et2%OFST]);
            add_instr("PLD %s\n", v_name[etr%OFST]);
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
            add_instr("PLD %s\n", v_name[eti%OFST]);
        }

        // comp var com float const
        if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
        {
            get_cmp_ets(et1,&etr,&eti);

            add_instr("%s %d // %s\n", ld, f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
            add_instr("PLD %s\n", v_name[etr%OFST]);
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
            add_instr("PLD %s\n", v_name[eti%OFST]);
        }

        // comp var com float acc
        if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==2) && (et2%OFST==0))
        {
            get_cmp_ets(et1,&etr,&eti);

            add_instr("PLD %s\n", v_name[etr%OFST]);
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
            add_instr("PLD %s\n", v_name[eti%OFST]);
        }

        // comp var com comp const
        if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==5))
        {
            int et1r,et2r;
            int et1i,et2i;

            get_cmp_ets    (et1,&et1r,&et1i);
            split_cmp_const(et2,&et2r,&et2i);

            add_instr("%s %s\n", ld, v_name[et1r%OFST]);
            add_instr("PLD %d // %s\n", f2mf(v_name[et2r%OFST]), v_name[et2r%OFST]);
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;

            add_instr("PLD %s\n", v_name[et1i%OFST]);
            add_instr("PLD %d // %s\n", f2mf(v_name[et2i%OFST]), v_name[et2i%OFST]);
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
        }

        // comp var com comp var
        if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==3) && (et2%OFST!=0))
        {
            int et1r,et2r;
            int et1i,et2i;

            get_cmp_ets(et1,&et1r,&et1i);
            get_cmp_ets(et2,&et2r,&et2i);

            add_instr("%s %s\n", ld, v_name[et1r%OFST]);
            add_instr("PLD %s\n",    v_name[et2r%OFST]);
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;

            add_instr("PLD %s\n", v_name[et1i%OFST]);
            add_instr("PLD %s\n", v_name[et2i%OFST]);
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
        }

        // comp var com comp acc
        if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==3) && (et2%OFST==0))
        {
            get_cmp_ets(et1,&etr,&eti);

            add_instr("SETP aux_soma\n");
            add_instr("PLD %s\n", v_name[etr%OFST]);
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;

            add_instr("PLD %s\n", v_name[eti%OFST]);
            add_instr("PLD aux_soma\n");
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
        }

        // comp acc com int var
        if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==1) && (et2%OFST!=0))
        {
            add_instr("SETP aux_soma\n");
            add_instr("PLD %s\n", v_name[et2%OFST]);
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
            add_instr("PLD aux_soma\n");
        }

        // comp acc com int acc
        if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==1) && (et2%OFST==0))
        {
            add_instr("SETP aux_int\n");
            add_instr("SETP aux_soma\n");
            add_instr("PLD aux_int\n");
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
            add_instr("PLD aux_soma\n");
        }

        // comp acc com float var
        if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==0))
        {
            add_instr("SETP aux_soma\n");
            add_instr("PLD %s\n", v_name[et2%OFST]);
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
            add_instr("PLD aux_soma\n");
        }

        // comp acc com float const
        if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
        {
            add_instr("SETP aux_soma\n");
            add_instr("PLD %d // %s\n", f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
            add_instr("PLD aux_soma\n");
        }

        // comp acc com float acc
        if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST==0))
        {
            add_instr("SETP aux_int\n");
            add_instr("SETP aux_soma\n");
            add_instr("PLD aux_int\n");
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
            add_instr("PLD aux_soma\n");
        }

        // comp acc com comp const
        if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==5))
        {
            split_cmp_const(et2,&etr,&eti);

            add_instr("SETP aux_soma\n");
            add_instr("PLD %d // %s\n", f2mf(v_name[etr%OFST]), v_name[etr%OFST]);
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;

            add_instr("PLD aux_soma\n");
            add_instr("PLD %d // %s\n", f2mf(v_name[eti%OFST]), v_name[eti%OFST]);
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
        }

        // comp acc com comp var
        if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==3) && (et2%OFST!=0))
        {
            get_cmp_ets(et2,&etr,&eti);

            add_instr("SETP aux_soma\n");
            add_instr("PLD %s\n", v_name[etr%OFST]);
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;

            add_instr("PLD aux_soma\n");
            add_instr("PLD %s\n", v_name[eti%OFST]);
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
        }

        // comp acc com comp acc
        if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==3) && (et2%OFST==0))
        {
            add_instr("SETP aux_imag2\n");
            add_instr("SETP aux_real2\n");
            add_instr("SETP aux_imag1\n");
            add_instr("PLD aux_real2\n");
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;

            add_instr("PLD aux_imag1\n");
            add_instr("PLD aux_imag2\n");
            add_instr("CALL denorm\n");
            add_instr("CALL float_add\n"); fadd = 1;
        }
    }
    else // 7*7 combinacoes para prtype = 1
    {
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
        if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==2) && (et2%OFST!=0))
        {
            add_instr("%s %s\n" , ld, v_name[et1%OFST]);
            add_instr("ADD %s\n",     v_name[et2%OFST]);
        }

        // int var com float acc
        if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==2) && (et2%OFST==0))
        {
            add_instr("ADD %s\n", v_name[et1%OFST]);
        }

        // int var com comp const
        if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==5))
        {
            split_cmp_const(et2,&etr,&eti);

            add_instr("%s %s\n" , ld, v_name[etr%OFST]);
            add_instr("ADD %s\n",     v_name[et1%OFST]);
            add_instr("PLD %s\n",     v_name[eti%OFST]);
        }

        // int var com comp var
        if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==3) && (et2%OFST!=0))
        {
            get_cmp_ets(et2,&etr,&eti);

            add_instr("%s %s\n" , ld, v_name[etr%OFST]);
            add_instr("ADD %s\n",     v_name[et1%OFST]);
            add_instr("PLD %s\n",     v_name[eti%OFST]);
        }

        // int var com comp acc
        if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==3) && (et2%OFST==0))
        {
            add_instr("SETP aux_soma\n");
            add_instr("ADD %s\n", v_name[et1%OFST]);
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
        if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST!=0))
        {
            add_instr("ADD %s\n", v_name[et2%OFST]);
        }

        // int acc com float acc
        if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST==0))
        {
            add_instr("SADD\n");
        }

        // int acc com comp const
        if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==5))
        {
            split_cmp_const(et2,&etr,&eti);

            add_instr("ADD %s\n", v_name[etr%OFST]);
            add_instr("PLD %s\n", v_name[eti%OFST]);
        }

        // int acc com comp var
        if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==3) && (et2%OFST!=0))
        {
            get_cmp_ets(et2,&etr,&eti);

            add_instr("ADD %s\n", v_name[etr%OFST]);
            add_instr("PLD %s\n", v_name[eti%OFST]);
        }

        // int acc com comp acc
        if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==3) && (et2%OFST==0))
        {
            add_instr("SETP aux_imag\n");
            add_instr("SADD\n");
            add_instr("PLD aux_imag\n");
        }

        // float var com int var
        if ((get_type(et1)==2) && (et1%OFST!=0) && (get_type(et2)==1) && (et2%OFST!=0))
        {
            add_instr("%s %s\n" , ld, v_name[et2%OFST]);
            add_instr("ADD %s\n",     v_name[et1%OFST]);
        }

        // float var com int acc
        if ((get_type(et1)==2) && (et1%OFST!=0) && (get_type(et2)==1) && (et2%OFST==0))
        {
            add_instr("ADD %s\n", v_name[et1%OFST]);
        }

        // float var com float var
        if ((get_type(et1)==2) && (et1%OFST!=0) && (get_type(et2)==2) && (et2%OFST!=0))
        {
            add_instr("%s %s\n" , ld, v_name[et2%OFST]);
            add_instr("ADD %s\n",     v_name[et1%OFST]);
        }

        // float var com float acc
        if ((get_type(et1)==2) && (et1%OFST!=0) && (get_type(et2)==2) && (et2%OFST==0))
        {
            add_instr("ADD %s\n", v_name[et1%OFST]);
        }

        // float var com comp const
        if ((get_type(et1)==2) && (et1%OFST!=0) && (get_type(et2)==5))
        {
            split_cmp_const(et2,&etr,&eti);

            add_instr("%s %s\n" , ld, v_name[et1%OFST]);
            add_instr("ADD %s\n",     v_name[etr%OFST]);
            add_instr("PLD %s\n",     v_name[eti%OFST]);
        }

        // float var com comp var
        if ((get_type(et1)==2) && (et1%OFST!=0) && (get_type(et2)==3) && (et2%OFST!=0))
        {
            get_cmp_ets(et2,&etr,&eti);

            add_instr("%s %s\n" , ld, v_name[et1%OFST]);
            add_instr("ADD %s\n",     v_name[etr%OFST]);
            add_instr("PLD %s\n",     v_name[eti%OFST]);
        }

        // float var com comp acc
        if ((get_type(et1)==2) && (et1%OFST!=0) && (get_type(et2)==3) && (et2%OFST==0))
        {
            add_instr("SETP aux_soma\n");
            add_instr("ADD %s\n", v_name[et1%OFST]);
            add_instr("PLD aux_soma\n");
        }

        // float acc com int var
        if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==1) && (et2%OFST!=0))
        {
            add_instr("ADD %s\n", v_name[et2%OFST]);
        }

        // float acc com int acc
        if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==1) && (et2%OFST==0))
        {
            add_instr("SADD\n");
        }

        // float acc com float var
        if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST!=0))
        {
            add_instr("ADD %s\n", v_name[et2%OFST]);
        }

        // float acc com float acc
        if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST==0))
        {
            add_instr("SADD\n");
        }

        // float acc com comp const
        if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==5))
        {
            split_cmp_const(et2,&etr,&eti);

            add_instr("ADD %s\n", v_name[etr%OFST]);
            add_instr("PLD %s\n", v_name[eti%OFST]);
        }

        // float acc com comp var
        if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==3) && (et2%OFST!=0))
        {
            get_cmp_ets(et2,&etr,&eti);

            add_instr("ADD %s\n", v_name[etr%OFST]);
            add_instr("PLD %s\n", v_name[eti%OFST]);
        }

        // float acc com comp acc
        if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==3) && (et2%OFST==0))
        {
            add_instr("SETP aux_imag\n");
            add_instr("SADD\n");
            add_instr("PLD aux_imag\n");
        }

        // comp const com int var
        if ((get_type(et1)==5) && (get_type(et2)==1) && (et2%OFST!=0))
        {
            split_cmp_const(et1,&etr,&eti);

            add_instr("%s %s\n" , ld, v_name[et2%OFST]);
            add_instr("ADD %s\n",     v_name[etr%OFST]);
            add_instr("PLD %s\n",     v_name[eti%OFST]);
        }

        // comp const com int acc
        if ((get_type(et1)==5) && (get_type(et2)==1) && (et2%OFST==0))
        {
            split_cmp_const(et1,&etr,&eti);

            add_instr("ADD %s\n",     v_name[etr%OFST]);
            add_instr("PLD %s\n",     v_name[eti%OFST]);
        }

        // comp const com float var
        if ((get_type(et1)==5) && (get_type(et2)==2) && (et2%OFST!=0))
        {
            split_cmp_const(et1,&etr,&eti);

            add_instr("%s %s\n" , ld, v_name[et2%OFST]);
            add_instr("ADD %s\n",     v_name[etr%OFST]);
            add_instr("PLD %s\n",     v_name[eti%OFST]);
        }

        // comp const com float acc
        if ((get_type(et1)==5) && (get_type(et2)==2) && (et2%OFST==0))
        {
            split_cmp_const(et1,&etr,&eti);

            add_instr("ADD %s\n", v_name[etr%OFST]);
            add_instr("PLD %s\n", v_name[eti%OFST]);
        }

        // comp const com comp const
        if ((get_type(et1)==5) && (get_type(et2)==5))
        {
            int et1r,et2r;
            int et1i,et2i;

            split_cmp_const(et1,&et1r,&et1i);
            split_cmp_const(et2,&et2r,&et2i);

            add_instr("%s %s\n" , ld, v_name[et1r%OFST]);
            add_instr("ADD %s\n",     v_name[et2r%OFST]);
            add_instr("PLD %s\n",     v_name[et1i%OFST]);
            add_instr("ADD %s\n",     v_name[et2i%OFST]);
        }

        // comp const com comp var
        if ((get_type(et1)==5) && (get_type(et2)==3) && (et2%OFST!=0))
        {
            int et1r,et2r;
            int et1i,et2i;

            split_cmp_const(et1,&et1r,&et1i);
            get_cmp_ets    (et2,&et2r,&et2i);

            add_instr("%s %s\n" , ld, v_name[et1r%OFST]);
            add_instr("ADD %s\n",     v_name[et2r%OFST]);
            add_instr("PLD %s\n",     v_name[et1i%OFST]);
            add_instr("ADD %s\n",     v_name[et2i%OFST]);
        }

        // comp const com comp acc
        if ((get_type(et1)==5) && (get_type(et2)==3) && (et2%OFST==0))
        {
            split_cmp_const(et1,&etr,&eti);

            add_instr("SETP aux_soma\n");
            add_instr("ADD %s\n", v_name[etr%OFST]);
            add_instr("PLD %s\n", v_name[eti%OFST]);
            add_instr("ADD aux_soma\n");
        }

        // comp var com int var
        if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==1) && (et2%OFST!=0))
        {
            get_cmp_ets(et1,&etr,&eti);

            add_instr("%s %s\n" , ld, v_name[et2%OFST]);
            add_instr("ADD %s\n",     v_name[etr%OFST]);
            add_instr("PLD %s\n",     v_name[eti%OFST]);
        }

        // comp var com int acc
        if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==1) && (et2%OFST==0))
        {
            get_cmp_ets(et1,&etr,&eti);

            add_instr("ADD %s\n", v_name[etr%OFST]);
            add_instr("PLD %s\n", v_name[eti%OFST]);
        }

        // comp var com float var
        if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==2) && (et2%OFST!=0))
        {
            get_cmp_ets(et1,&etr,&eti);

            add_instr("%s %s\n" , ld, v_name[et2%OFST]);
            add_instr("ADD %s\n",     v_name[etr%OFST]);
            add_instr("PLD %s\n",     v_name[eti%OFST]);
        }

        // comp var com float acc
        if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==2) && (et2%OFST==0))
        {
            get_cmp_ets(et1,&etr,&eti);

            add_instr("ADD %s\n", v_name[etr%OFST]);
            add_instr("PLD %s\n", v_name[eti%OFST]);
        }

        // comp var com comp const
        if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==5))
        {
            int et1r,et2r;
            int et1i,et2i;

            get_cmp_ets    (et1,&et1r,&et1i);
            split_cmp_const(et2,&et2r,&et2i);

            add_instr("%s %s\n" , ld, v_name[et1r%OFST]);
            add_instr("ADD %s\n",     v_name[et2r%OFST]);
            add_instr("PLD %s\n",     v_name[et1i%OFST]);
            add_instr("ADD %s\n",     v_name[et2i%OFST]);
        }

        // comp var com comp var
        if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==3) && (et2%OFST!=0))
        {
            int et1r,et2r;
            int et1i,et2i;

            get_cmp_ets(et1,&et1r,&et1i);
            get_cmp_ets(et2,&et2r,&et2i);

            add_instr("%s %s\n" , ld, v_name[et1r%OFST]);
            add_instr("ADD %s\n",     v_name[et2r%OFST]);
            add_instr("PLD %s\n",     v_name[et1i%OFST]);
            add_instr("ADD %s\n",     v_name[et2i%OFST]);
        }

        // comp var com comp acc
        if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==3) && (et2%OFST==0))
        {
            get_cmp_ets(et1,&etr,&eti);

            add_instr("SETP aux_soma\n");
            add_instr("ADD %s\n", v_name[etr%OFST]);
            add_instr("PLD %s\n", v_name[eti%OFST]);
            add_instr("ADD aux_soma\n");
        }

        // comp acc com int var
        if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==1) && (et2%OFST!=0))
        {
            add_instr("SETP aux_soma\n");
            add_instr("ADD %s\n", v_name[et2%OFST]);
            add_instr("PLD aux_soma\n");
        }

        // comp acc com int acc
        if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==1) && (et2%OFST==0))
        {
            add_instr("SETP aux_int\n");
            add_instr("SETP aux_imag\n");
            add_instr("ADD aux_int\n");
            add_instr("PLD aux_imag\n");
        }

        // comp acc com float var
        if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST!=0))
        {
            add_instr("SETP aux_soma\n");
            add_instr("ADD %s\n", v_name[et2%OFST]);
            add_instr("PLD aux_soma\n");
        }

        // comp acc com float acc
        if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST==0))
        {
            add_instr("SETP aux_int\n");
            add_instr("SETP aux_soma\n");
            add_instr("ADD aux_int\n");
            add_instr("PLD aux_soma\n");
        }

        // comp acc com comp const
        if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==5))
        {
            split_cmp_const(et2,&etr,&eti);

            add_instr("ADD %s\n", v_name[eti%OFST]);
            add_instr("SETP aux_imag\n");
            add_instr("ADD %s\n", v_name[etr%OFST]);
            add_instr("PLD aux_imag\n");
        }

        // comp acc com comp var
        if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==3) && (et2%OFST!=0))
        {
            get_cmp_ets(et2,&etr,&eti);

            add_instr("ADD %s\n", v_name[eti%OFST]);
            add_instr("SETP aux_imag\n");
            add_instr("ADD %s\n", v_name[etr%OFST]);
            add_instr("PLD aux_imag\n");
        }

        // comp acc com comp acc
        if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==3) && (et2%OFST==0))
        {
            add_instr("SETP aux_imag2\n");
            add_instr("SETP aux_real2\n");
            add_instr("ADD aux_imag2\n");
            add_instr("SETP aux_imag2\n");
            add_instr("ADD aux_real2\n");
            add_instr("PLD aux_imag2\n");
        }
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

int oper_subt(int et1, int et2)
{
    return oper_soma(et1,negacao(et2));
}

int oper_mult(int et1, int et2)
{
    int etr, eti;

    char ld[10];
    if (acc_ok == 0) strcpy(ld,"LOAD"); else strcpy(ld,"PLD");

    // 8*8 combinacoes para prtype = 0
    if (prtype == 0)
    {
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
            add_instr("%s %s\n" , ld, v_name[et1%OFST]);
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("PLD %s\n" , v_name[et2%OFST]);
            add_instr("CALL float_mult\n"); fmlt = 1;
        }

        // int var com float const
        if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
        {
            add_instr("%s %s\n" , ld, v_name[et1%OFST]);
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("PLD %d // %s\n" , f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
            add_instr("CALL float_mult\n"); fmlt = 1;
        }

        // int var com float acc
        if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==2) && (et2%OFST==0))
        {
            add_instr("PLD %s\n", v_name[et1%OFST]);
            add_instr("CALL int2float\n");  i2f  = 1;
            add_instr("CALL float_mult\n"); fmlt = 1;
        }

        // int var com comp const
        if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==5))
        {
            split_cmp_const(et2,&etr,&eti);

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
            add_instr("PLD aux_mult\n");
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
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("PLD %s\n", v_name[et2%OFST]);
            add_instr("CALL float_mult\n"); fmlt = 1;
        }

        // int acc com float const
        if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
        {
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("PLD %d // %s\n", f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
            add_instr("CALL float_mult\n"); fmlt = 1;
        }

        // int acc com float acc
        if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST==0))
        {
            add_instr("SETP aux_soma\n");
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("PLD aux_soma\n");
            add_instr("CALL float_mult\n"); fmlt = 1;
        }

        // int acc com comp const
        if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==5))
        {
            split_cmp_const(et2,&etr,&eti);

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
            add_instr("SETP aux_imag  \n");
            add_instr("SETP aux_real  \n");
            add_instr("CALL int2float \n"); i2f  = 1;
            add_instr("SET  aux_mult  \n");
            add_instr("PLD  aux_real  \n");
            add_instr("CALL float_mult\n"); fmlt = 1;
            add_instr("PLD  aux_imag  \n");
            add_instr("PLD  aux_mult  \n");
            add_instr("CALL float_mult\n"); fmlt = 1;
        }

        // float var com int var
        if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==1) && (et2%OFST!=0))
        {
            add_instr("%s %s\n" , ld, v_name[et2%OFST]);
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("PLD %s\n",     v_name[et1%OFST]);
            add_instr("CALL float_mult\n"); fmlt = 1;
        }

        // float var com int acc
        if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==1) && (et2%OFST==0))
        {
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("PLD %s\n",     v_name[et1%OFST]);
            add_instr("CALL float_mult\n"); fmlt = 1;
        }

        // float var com float var
        if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==0))
        {
            add_instr("%s %s\n" , ld, v_name[et1%OFST]);
            add_instr("PLD %s\n",     v_name[et2%OFST]);
            add_instr("CALL float_mult\n"); fmlt = 1;
        }

        // float var com float const
        if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
        {
            add_instr("%s %s\n", ld, v_name[et1%OFST]);
            add_instr("PLD %d // %s\n", f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
            add_instr("CALL float_mult\n"); fmlt = 1;
        }

        // float var com float acc
        if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==2) && (et2%OFST==0))
        {
            add_instr("PLD %s\n", v_name[et1%OFST]);
            add_instr("CALL float_mult\n"); fmlt = 1;
        }

        // float var com comp const
        if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==5))
        {
            split_cmp_const(et2,&etr,&eti);

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
            add_instr("%s %s\n" , ld, v_name[et2%OFST]);
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("PLD %d // %s\n", f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
            add_instr("CALL float_mult\n"); fmlt = 1;
        }

        // float const com int acc
        if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==1) && (et2%OFST==0))
        {
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("PLD %d // %s\n", f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
            add_instr("CALL float_mult\n"); fmlt = 1;
        }

        // float const com float var
        if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==0))
        {
            add_instr("%s %d // %s\n", ld, f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
            add_instr("PLD %s\n", v_name[et2%OFST]);
            add_instr("CALL float_mult\n"); fmlt = 1;
        }

        // float const com float const
        if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
        {
            add_instr("%s %d // %s\n", ld, f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
            add_instr("PLD %d // %s\n", f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
            add_instr("CALL float_mult\n"); fmlt = 1;
        }

        // float const com float acc
        if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==2) && (et2%OFST==0))
        {
            add_instr("PLD %d // %s\n", f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
            add_instr("CALL float_mult\n"); fmlt = 1;
        }

        // float const com comp const
        if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==5))
        {
            split_cmp_const(et2,&etr,&eti);

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
            add_instr("PLD %s\n", v_name[et2%OFST]);
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("CALL float_mult\n"); fmlt = 1;
        }

        // float acc com int acc
        if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==1) && (et2%OFST==0))
        {
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("CALL float_mult\n"); fmlt = 1;
        }

        // float acc com float var
        if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==0))
        {
            add_instr("PLD %s\n", v_name[et2%OFST]);
            add_instr("CALL float_mult\n"); fmlt = 1;
        }

        // float acc com float const
        if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
        {
            add_instr("PLD %d // %s\n", f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
            add_instr("CALL float_mult\n"); fmlt = 1;
        }

        // float acc com float acc
        if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST==0))
        {
            add_instr("CALL float_mult\n"); fmlt = 1;
        }

        // float acc com comp const
        if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==5))
        {
            split_cmp_const(et2,&etr,&eti);

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
            add_instr("SETP aux_imag  \n");
            add_instr("SETP aux_real  \n");
            add_instr("SET  aux_mult  \n");
            add_instr("PLD  aux_real  \n");
            add_instr("CALL float_mult\n"); fmlt = 1;
            add_instr("PLD  aux_imag  \n");
            add_instr("PLD  aux_mult  \n");
            add_instr("CALL float_mult\n"); fmlt = 1;
        }

        // comp const com int var
        if ((get_type(et1)==5) && (get_type(et2)==1) && (et2%OFST!=0))
        {
            split_cmp_const(et1,&etr,&eti);

            oper_mult(etr,et2);
            oper_mult(eti,et2);
        }

        // comp const com int acc
        if ((get_type(et1)==5) && (get_type(et2)==1) && (et2%OFST==0))
        {
            split_cmp_const(et1,&etr,&eti);

            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("SET aux_mult\n");
            oper_mult(etr,2*OFST);
            add_instr("PLD aux_mult\n");
            oper_mult(eti,2*OFST);
        }

        // comp const com float var
        if ((get_type(et1)==5) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==0))
        {
            split_cmp_const(et1,&etr,&eti);

            oper_mult(etr,et2);
            oper_mult(eti,et2);
        }

        // comp const com float const
        if ((get_type(et1)==5) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
        {
            split_cmp_const(et1,&etr,&eti);

            oper_mult(etr,et2);
            oper_mult(eti,et2);
        }

        // comp const com float acc
        if ((get_type(et1)==5) && (get_type(et2)==2) && (et2%OFST==0))
        {
            split_cmp_const(et1,&etr,&eti);

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

            split_cmp_const(et1,&et1r,&et1i);
            split_cmp_const(et2,&et2r,&et2i);

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

            split_cmp_const(et1,&et1r,&et1i);
            get_cmp_ets    (et2,&et2r,&et2i);

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
            split_cmp_const(et1,&etr,&eti);

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

            add_instr("CALL int2float\n"); i2f = 1;
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

            get_cmp_ets    (et1,&et1r,&et1i);
            split_cmp_const(et2,&et2r,&et2i);

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
            add_instr("PLD %s\n", v_name[et2%OFST]);
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("SET aux_float\n");
            add_instr("CALL float_mult\n"); fmlt = 1;
            add_instr("PLD aux_imag\n");
            add_instr("PLD aux_float\n");
            add_instr("CALL float_mult\n"); fmlt = 1;
        }

        // comp acc com int acc
        if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==1) && (et2%OFST==0))
        {
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("SETP aux_float\n");
            add_instr("SETP aux_imag\n");
            add_instr("PLD aux_float\n");
            add_instr("CALL float_mult\n"); fmlt = 1;
            add_instr("PLD aux_imag\n");
            add_instr("PLD aux_float\n");
            add_instr("CALL float_mult\n"); fmlt = 1;
        }

        // comp acc com float var
        if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==0))
        {
            add_instr("SETP aux_imag\n");
            add_instr("PLD %s\n", v_name[et2%OFST]);
            add_instr("CALL float_mult\n"); fmlt = 1;
            add_instr("PLD aux_imag\n");
            add_instr("PLD %s\n", v_name[et2%OFST]);
            add_instr("CALL float_mult\n"); fmlt = 1;
        }

        // comp acc com float const
        if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
        {
            add_instr("SETP aux_imag\n");
            add_instr("PLD %d // %s\n", f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
            add_instr("CALL float_mult\n"); fmlt = 1;
            add_instr("PLD aux_imag\n");
            add_instr("PLD %d // %s\n", f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
            add_instr("CALL float_mult\n"); fmlt = 1;
        }

        // comp acc com float acc
        if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST==0))
        {
            add_instr("SETP aux_float\n");
            add_instr("SETP aux_imag\n");
            add_instr("PLD aux_float\n");
            add_instr("CALL float_mult\n"); fmlt = 1;
            add_instr("PLD aux_imag\n");
            add_instr("PLD aux_float\n");
            add_instr("CALL float_mult\n"); fmlt = 1;
        }

        // comp acc com comp const
        if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==5))
        {
            split_cmp_const(et2,&etr,&eti);

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
    }
    else // 7*7 combinacoes para prtype = 1
    {
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
        if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==2) && (et2%OFST!=0))
        {
            add_instr("%s %s\n" , ld, v_name[et1%OFST]);
            add_instr("MLT %s\n",     v_name[et2%OFST]);
        }

        // int var com float acc
        if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==2) && (et2%OFST==0))
        {
            add_instr("MLT %s\n", v_name[et1%OFST]);
        }

        // int var com comp const
        if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==5))
        {
            split_cmp_const(et2,&etr,&eti);

            add_instr("%s %s\n" , ld, v_name[etr%OFST]);
            add_instr("MLT %s\n",     v_name[et1%OFST]);
            add_instr("PLD %s\n",     v_name[eti%OFST]);
            add_instr("MLT %s\n",     v_name[et1%OFST]);
        }

        // int var com comp var
        if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==3) && (et2%OFST!=0))
        {
            get_cmp_ets(et2,&etr,&eti);

            add_instr("%s %s\n" , ld, v_name[etr%OFST]);
            add_instr("MLT %s\n",     v_name[et1%OFST]);
            add_instr("PLD %s\n",     v_name[eti%OFST]);
            add_instr("MLT %s\n",     v_name[et1%OFST]);
        }

        // int var com comp acc
        if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==3) && (et2%OFST==0))
        {
            add_instr("SETP aux_soma\n");
            add_instr("MLT  %s\n", v_name[et1%OFST]);
            add_instr("PLD  aux_soma\n");
            add_instr("MLT  %s\n", v_name[et1%OFST]);
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
        if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST!=0))
        {
            add_instr("MLT %s\n", v_name[et2%OFST]);
        }

        // int acc com float acc
        if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST==0))
        {
            add_instr("SMLT\n");
        }

        // int acc com comp const
        if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==5))
        {
            split_cmp_const(et2,&etr,&eti);

            add_instr("SET aux_mult\n");
            add_instr("MLT %s\n", v_name[etr%OFST]);
            add_instr("PLD %s\n", v_name[eti%OFST]);
            add_instr("MLT aux_mult\n");
        }

        // int acc com comp var
        if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==3) && (et2%OFST!=0))
        {
            get_cmp_ets(et2,&etr,&eti);

            add_instr("SET aux_mult\n");
            add_instr("MLT %s\n", v_name[etr%OFST]);
            add_instr("PLD %s\n", v_name[eti%OFST]);
            add_instr("MLT aux_mult\n");
        }

        // int acc com comp acc
        if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==3) && (et2%OFST==0))
        {
            add_instr("SETP aux_imag\n");
            add_instr("SETP aux_real\n");
            add_instr("SET aux_int\n");
            add_instr("MLT aux_real\n");
            add_instr("PLD aux_imag\n");
            add_instr("MLT aux_int\n");
        }

        // float var com int var
        if ((get_type(et1)==2) && (et1%OFST!=0) && (get_type(et2)==1) && (et2%OFST!=0))
        {
            add_instr("%s %s\n" , ld, v_name[et2%OFST]);
            add_instr("MLT %s\n",     v_name[et1%OFST]);
        }

        // float var com int acc
        if ((get_type(et1)==2) && (et1%OFST!=0) && (get_type(et2)==1) && (et2%OFST==0))
        {
            add_instr("MLT %s\n", v_name[et1%OFST]);
        }

        // float var com float var
        if ((get_type(et1)==2) && (et1%OFST!=0) && (get_type(et2)==2) && (et2%OFST!=0))
        {
            add_instr("%s %s\n" , ld, v_name[et2%OFST]);
            add_instr("MLT %s\n",     v_name[et1%OFST]);
        }

        // float var com float acc
        if ((get_type(et1)==2) && (et1%OFST!=0) && (get_type(et2)==2) && (et2%OFST==0))
        {
            add_instr("MLT %s\n", v_name[et1%OFST]);
        }

        // float var com comp const
        if ((get_type(et1)==2) && (et1%OFST!=0) && (get_type(et2)==5))
        {
            split_cmp_const(et2,&etr,&eti);

            add_instr("%s %s\n" , ld, v_name[et1%OFST]);
            add_instr("MLT %s\n",     v_name[etr%OFST]);
            add_instr("PLD %s\n",     v_name[eti%OFST]);
            add_instr("MLT %s\n",     v_name[et1%OFST]);
        }

        // float var com comp var
        if ((get_type(et1)==2) && (et1%OFST!=0) && (get_type(et2)==3) && (et2%OFST!=0))
        {
            get_cmp_ets(et2,&etr,&eti);

            add_instr("%s %s\n" , ld, v_name[et1%OFST]);
            add_instr("MLT %s\n",     v_name[etr%OFST]);
            add_instr("PLD %s\n",     v_name[eti%OFST]);
            add_instr("MLT %s\n",     v_name[et1%OFST]);
        }

        // float var com comp acc
        if ((get_type(et1)==2) && (et1%OFST!=0) && (get_type(et2)==3) && (et2%OFST==0))
        {
            add_instr("SETP aux_soma\n");
            add_instr("MLT %s\n", v_name[et1%OFST]);
            add_instr("PLD aux_soma\n");
            add_instr("MLT %s\n", v_name[et1%OFST]);
        }

        // float acc com int var
        if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==1) && (et2%OFST!=0))
        {
            add_instr("MLT %s\n", v_name[et2%OFST]);
        }

        // float acc com int acc
        if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==1) && (et2%OFST==0))
        {
            add_instr("SMLT\n");
        }

        // float acc com float var
        if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST!=0))
        {
            add_instr("MLT %s\n", v_name[et2%OFST]);
        }

        // float acc com float acc
        if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST==0))
        {
            add_instr("SMLT\n");
        }

        // float acc com comp const
        if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==5))
        {
            split_cmp_const(et2,&etr,&eti);

            add_instr("SET aux_mult\n");
            add_instr("MLT %s\n", v_name[etr%OFST]);
            add_instr("PLD %s\n", v_name[eti%OFST]);
            add_instr("MLT aux_mult\n");
        }

        // float acc com comp var
        if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==3) && (et2%OFST!=0))
        {
            get_cmp_ets(et2,&etr,&eti);

            add_instr("SET aux_mult\n");
            add_instr("MLT %s\n", v_name[etr%OFST]);
            add_instr("PLD %s\n", v_name[eti%OFST]);
            add_instr("MLT aux_mult\n");
        }

        // float acc com comp acc
        if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==3) && (et2%OFST==0))
        {
            add_instr("SETP aux_imag\n");
            add_instr("SETP aux_real\n");
            add_instr("SET aux_float\n");
            add_instr("MLT aux_real\n");
            add_instr("PLD aux_imag\n");
            add_instr("MLT aux_float\n");
        }

        // comp const com int var
        if ((get_type(et1)==5) && (get_type(et2)==1) && (et2%OFST!=0))
        {
            split_cmp_const(et1,&etr,&eti);

            add_instr("%s %s\n" , ld, v_name[et2%OFST]);
            add_instr("MLT %s\n",     v_name[etr%OFST]);
            add_instr("PLD %s\n",     v_name[eti%OFST]);
            add_instr("MLT %s\n",     v_name[et2%OFST]);
        }

        // comp const com int acc
        if ((get_type(et1)==5) && (get_type(et2)==1) && (et2%OFST==0))
        {
            split_cmp_const(et1,&etr,&eti);

            add_instr("SET aux_mult\n");
            add_instr("MLT %s\n",     v_name[etr%OFST]);
            add_instr("PLD %s\n",     v_name[eti%OFST]);
            add_instr("MLT aux_mult\n");
        }

        // comp const com float var
        if ((get_type(et1)==5) && (get_type(et2)==2) && (et2%OFST!=0))
        {
            split_cmp_const(et1,&etr,&eti);

            add_instr("%s %s\n" , ld, v_name[et2%OFST]);
            add_instr("MLT %s\n",     v_name[etr%OFST]);
            add_instr("PLD %s\n",     v_name[eti%OFST]);
            add_instr("MLT %s\n" ,    v_name[et2%OFST]);
        }

        // comp const com float acc
        if ((get_type(et1)==5) && (get_type(et2)==2) && (et2%OFST==0))
        {
            split_cmp_const(et1,&etr,&eti);

            add_instr("SET aux_mult\n");
            add_instr("MLT %s\n", v_name[etr%OFST]);
            add_instr("PLD %s\n", v_name[eti%OFST]);
            add_instr("MLT aux_mult\n");
        }

        // comp const com comp const
        if ((get_type(et1)==5) && (get_type(et2)==5))
        {
            int et1r,et2r;
            int et1i,et2i;

            split_cmp_const(et1,&et1r,&et1i);
            split_cmp_const(et2,&et2r,&et2i);

            add_instr("%s %s\n" , ld, v_name[et1r%OFST]);
            add_instr("MLT %s\n",     v_name[et2r%OFST]);
            add_instr("PLD %s\n",     v_name[et1i%OFST]);
            add_instr("MLT %s\n",     v_name[et2i%OFST]);
            add_instr("NEG\n");
            add_instr("SADD\n");

            add_instr("PLD %s\n" ,    v_name[et1r%OFST]);
            add_instr("MLT %s\n",     v_name[et2i%OFST]);
            add_instr("PLD %s\n",     v_name[et1i%OFST]);
            add_instr("MLT %s\n",     v_name[et2r%OFST]);
            add_instr("SADD\n");
        }

        // comp const com comp var
        if ((get_type(et1)==5) && (get_type(et2)==3) && (et2%OFST!=0))
        {
            int et1r,et2r;
            int et1i,et2i;

            split_cmp_const(et1,&et1r,&et1i);
            get_cmp_ets    (et2,&et2r,&et2i);

            add_instr("%s %s\n" , ld, v_name[et1r%OFST]);
            add_instr("MLT %s\n",     v_name[et2r%OFST]);
            add_instr("PLD %s\n",     v_name[et1i%OFST]);
            add_instr("MLT %s\n",     v_name[et2i%OFST]);
            add_instr("NEG\n");
            add_instr("SADD\n");

            add_instr("PLD %s\n" ,    v_name[et1r%OFST]);
            add_instr("MLT %s\n",     v_name[et2i%OFST]);
            add_instr("PLD %s\n",     v_name[et1i%OFST]);
            add_instr("MLT %s\n",     v_name[et2r%OFST]);
            add_instr("SADD\n");
        }

        // comp const com comp acc
        if ((get_type(et1)==5) && (get_type(et2)==3) && (et2%OFST==0))
        {
            split_cmp_const(et1,&etr,&eti);

            add_instr("SETP aux_imag\n");
            add_instr("SET  aux_real\n");
            add_instr("MLT %s\n", v_name[etr%OFST]);
            add_instr("PLD %s\n", v_name[eti%OFST]);
            add_instr("MLT aux_imag\n");
            add_instr("NEG\n");
            add_instr("SADD\n");

            add_instr("PLD %s\n", v_name[etr%OFST]);
            add_instr("MLT aux_imag\n");
            add_instr("PLD %s\n", v_name[eti%OFST]);
            add_instr("MLT aux_real\n");
            add_instr("SADD\n");
        }

        // comp var com int var
        if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==1) && (et2%OFST!=0))
        {
            get_cmp_ets(et1,&etr,&eti);

            add_instr("%s %s\n" , ld, v_name[et2%OFST]);
            add_instr("MLT %s\n",     v_name[etr%OFST]);
            add_instr("PLD %s\n",     v_name[eti%OFST]);
            add_instr("MLT %s\n",     v_name[et2%OFST]);
        }

        // comp var com int acc
        if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==1) && (et2%OFST==0))
        {
            get_cmp_ets(et1,&etr,&eti);

            add_instr("SET aux_mult\n");
            add_instr("MLT %s\n", v_name[etr%OFST]);
            add_instr("PLD %s\n", v_name[eti%OFST]);
            add_instr("MLT aux_mult\n");
        }

        // comp var com float var
        if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==2) && (et2%OFST!=0))
        {
            get_cmp_ets(et1,&etr,&eti);

            add_instr("%s %s\n" , ld, v_name[et2%OFST]);
            add_instr("MLT %s\n",     v_name[etr%OFST]);
            add_instr("PLD %s\n",     v_name[eti%OFST]);
            add_instr("MLT %s\n",     v_name[et2%OFST]);
        }

        // comp var com float acc
        if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==2) && (et2%OFST==0))
        {
            get_cmp_ets(et1,&etr,&eti);

            add_instr("SET aux_mult\n");
            add_instr("MLT %s\n", v_name[etr%OFST]);
            add_instr("PLD %s\n", v_name[eti%OFST]);
            add_instr("MLT aux_mult\n");
        }

        // comp var com comp const
        if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==5))
        {
            int et1r,et2r;
            int et1i,et2i;

            get_cmp_ets    (et1,&et1r,&et1i);
            split_cmp_const(et2,&et2r,&et2i);

            add_instr("%s %s\n" , ld, v_name[et1r%OFST]);
            add_instr("MLT %s\n",     v_name[et2r%OFST]);
            add_instr("PLD %s\n",     v_name[et1i%OFST]);
            add_instr("MLT %s\n",     v_name[et2i%OFST]);
            add_instr("NEG\n" );
            add_instr("SADD\n");

            add_instr("PLD %s\n" ,    v_name[et1r%OFST]);
            add_instr("MLT %s\n",     v_name[et2i%OFST]);
            add_instr("PLD %s\n",     v_name[et1i%OFST]);
            add_instr("MLT %s\n",     v_name[et2r%OFST]);
            add_instr("SADD\n");
        }

        // comp var com comp var
        if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==3) && (et2%OFST!=0))
        {
            int et1r,et2r;
            int et1i,et2i;

            get_cmp_ets(et1,&et1r,&et1i);
            get_cmp_ets(et2,&et2r,&et2i);

            add_instr("%s %s\n" , ld, v_name[et1r%OFST]);
            add_instr("MLT %s\n",     v_name[et2r%OFST]);
            add_instr("PLD %s\n",     v_name[et1i%OFST]);
            add_instr("MLT %s\n",     v_name[et2i%OFST]);
            add_instr("NEG\n" );
            add_instr("SADD\n");

            add_instr("PLD %s\n" ,    v_name[et1r%OFST]);
            add_instr("MLT %s\n",     v_name[et2i%OFST]);
            add_instr("PLD %s\n",     v_name[et1i%OFST]);
            add_instr("MLT %s\n",     v_name[et2r%OFST]);
            add_instr("SADD\n");
        }

        // comp var com comp acc
        if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==3) && (et2%OFST==0))
        {
            get_cmp_ets(et1,&etr,&eti);

            add_instr("SETP aux_imag\n");
            add_instr("SET  aux_real\n");
            add_instr("MLT %s\n", v_name[etr%OFST]);
            add_instr("PLD %s\n", v_name[eti%OFST]);
            add_instr("MLT aux_imag\n");
            add_instr("NEG\n");
            add_instr("SADD\n");

            add_instr("PLD %s\n", v_name[etr%OFST]);
            add_instr("MLT aux_imag\n");
            add_instr("PLD %s\n", v_name[eti%OFST]);
            add_instr("MLT aux_real\n");
            add_instr("SADD\n");
        }

        // comp acc com int var
        if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==1) && (et2%OFST!=0))
        {
            add_instr("SETP aux_imag\n");
            add_instr("MLT %s\n", v_name[et2%OFST]);
            add_instr("PLD aux_imag\n");
            add_instr("MLT %s\n", v_name[et2%OFST]);
        }

        // comp acc com int acc
        if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==1) && (et2%OFST==0))
        {
            add_instr("SETP aux_int\n");
            add_instr("SETP aux_imag\n");
            add_instr("MLT aux_int\n");
            add_instr("PLD aux_imag\n");
            add_instr("MLT aux_int\n");
        }

        // comp acc com float var
        if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST!=0))
        {
            add_instr("SETP aux_imag\n");
            add_instr("MLT %s\n", v_name[et2%OFST]);
            add_instr("PLD aux_imag\n");
            add_instr("MLT %s\n", v_name[et2%OFST]);
        }

        // comp acc com float acc
        if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST==0))
        {
            add_instr("SETP aux_int\n");
            add_instr("SETP aux_imag\n");
            add_instr("MLT aux_int\n");
            add_instr("PLD aux_imag\n");
            add_instr("MLT aux_int\n");
        }

        // comp acc com comp const
        if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==5))
        {
            split_cmp_const(et2,&etr,&eti);

            add_instr("SETP aux_imag\n");
            add_instr("SET aux_real\n");
            add_instr("MLT %s\n", v_name[etr%OFST]);
            add_instr("PLD aux_imag\n");
            add_instr("MLT %s\n", v_name[eti%OFST]);
            add_instr("NEG\n");
            add_instr("SADD\n");

            add_instr("PLD aux_real\n");
            add_instr("MLT %s\n", v_name[eti%OFST]);
            add_instr("PLD aux_imag\n");
            add_instr("MLT %s\n", v_name[etr%OFST]);
            add_instr("SADD\n");
        }

        // comp acc com comp var
        if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==3) && (et2%OFST!=0))
        {
            get_cmp_ets(et2,&etr,&eti);

            add_instr("SETP aux_imag\n");
            add_instr("SET aux_real\n");
            add_instr("MLT %s\n", v_name[etr%OFST]);
            add_instr("PLD aux_imag\n");
            add_instr("MLT %s\n", v_name[eti%OFST]);
            add_instr("NEG\n");
            add_instr("SADD\n");

            add_instr("PLD aux_real\n");
            add_instr("MLT %s\n", v_name[eti%OFST]);
            add_instr("PLD aux_imag\n");
            add_instr("MLT %s\n", v_name[etr%OFST]);
            add_instr("SADD\n");
        }

        // comp acc com comp acc
        if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==3) && (et2%OFST==0))
        {
            add_instr("SETP aux_imag2\n");
            add_instr("SETP aux_real2\n");
            add_instr("SETP aux_imag1\n");
            add_instr("SET  aux_real1\n");
            add_instr("MLT  aux_real2\n");
            add_instr("PLD  aux_imag1\n");
            add_instr("MLT  aux_imag2\n");
            add_instr("NEG\n");
            add_instr("SADD\n");

            add_instr("PLD  aux_real1\n");
            add_instr("MLT  aux_imag2\n");
            add_instr("PLD  aux_real2\n");
            add_instr("MLT  aux_imag1\n");
            add_instr("SADD\n");
        }
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

int oper_divi(int et1, int et2)
{
    int etr, eti;

    char ld[10];
    if (acc_ok == 0) strcpy(ld,"LOAD"); else strcpy(ld,"PLD");

    // 8*8 combinacoes para prtype = 0
    if (prtype == 0)
    {
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
            add_instr("%s %s\n" , ld, v_name[et1%OFST]);
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("PLD %s\n" , v_name[et2%OFST]);
            add_instr("CALL float_div\n"); fdiv = 1;
        }

        // int var com float const
        if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
        {
            add_instr("%s %s\n" , ld, v_name[et1%OFST]);
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("PLD %d // %s\n" , f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
            add_instr("CALL float_div\n"); fdiv = 1;
        }

        // int var com float acc
        if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==2) && (et2%OFST==0))
        {
            add_instr("SET aux_div\n");
            add_instr("LOAD %s\n", v_name[et1%OFST]);
            add_instr("CALL int2float\n"); i2f  = 1;
            add_instr("PLD aux_div\n");
            add_instr("CALL float_div\n"); fdiv = 1;
        }

        // int var com comp const
        if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==5))
        {
            split_cmp_const(et2,&etr,&eti);

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
            negacao  (2*OFST);
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
            negacao  (2*OFST);
        }

        // int var com comp acc
        if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==3) && (et2%OFST==0))
        {
            add_instr("SETP aux_imag  \n");           // salva a parte imaginaria
            add_instr("SET  aux_real  \n");           // salva a parte real
            add_instr("PLD  aux_real  \n");           // coloca a parte real na pilha
            add_instr("CALL float_mult\n"); fmlt = 1; // parte real ao quadrado
            add_instr("PLD  aux_imag  \n");           // pega a parte imaginaria
            add_instr("PLD  aux_imag  \n");           // pega ela de novo
            add_instr("CALL float_mult\n"); fmlt = 1; // parte imag ao quadrado
            add_instr("CALL denorm    \n");           // prepara a soma
            add_instr("CALL float_add \n"); fadd = 1; // soma os quadrados
            add_instr("SET  aux_cmp   \n");           // salva o modulo ao quadrado

            add_instr("LOAD %s\n", v_name[et1%OFST]); // carrega o num inteiro
            add_instr("CALL int2float \n"); i2f = 1;  // converte ele pra float
            add_instr("SET aux_float  \n");           // salva o float
            add_instr("PLD aux_real   \n");           // pega a parte real
            add_instr("CALL float_mult\n"); fmlt = 1; // mult float com parte real
            add_instr("PLD aux_cmp    \n");           // pega o modulo ao quadrado
            add_instr("CALL float_div \n"); fdiv = 1; // faz a divisao

            add_instr("PLD aux_float  \n");           // pega o float
            add_instr("PLD aux_imag   \n");           // pega a parte imag
            add_instr("CALL float_mult\n"); fmlt = 1; // mult float com parte imaginaria
            add_instr("PLD aux_cmp    \n");           // pega o modulo ao quadrado
            add_instr("CALL float_div \n"); fdiv = 1; // faz a divisao
            add_instr("PLD float_nbits\n");           // faz a negacao
            add_instr("SHL 1\n");
            add_instr("SADD\n");
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
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("PLD %s\n", v_name[et2%OFST]);
            add_instr("CALL float_div\n"); fdiv = 1;
        }

        // int acc com float const
        if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
        {
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("PLD %d // %s\n", f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
            add_instr("CALL float_div\n"); fdiv = 1;
        }

        // int acc com float acc
        if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST==0))
        {
            add_instr("SETP aux_soma\n");
            add_instr("CALL int2float\n"); i2f  = 1;
            add_instr("PLD aux_soma\n");
            add_instr("CALL float_div\n"); fdiv = 1;
        }

        // int acc com comp const
        if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==5))
        {
            split_cmp_const(et2,&etr,&eti);

            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("SET aux_float\n");
            acc_ok = 0;

            oper_mult(etr,etr);         // parte real ao quadrado
            oper_mult(eti,eti);         // parte imag ao quadrado
            oper_soma(2*OFST,2*OFST);   // soma os quadrados
            add_instr("SET aux_cmp\n"); // salva o resultado

            add_instr("LOAD aux_float\n");
            oper_mult(2*OFST,etr);      // mult float com parte real
            add_instr("PLD aux_cmp\n"); // pega o denominador
            oper_divi(2*OFST,2*OFST);   // faz a divisao

            add_instr("PLD  aux_float\n");
            oper_mult(2*OFST,eti);      // mult float com parte imag
            add_instr("PLD aux_cmp\n"); // pega o denominador
            oper_divi(2*OFST,2*OFST);   // faz a divisao
            negacao  (2*OFST);
        }

        // int acc com comp var
        if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==3) && (et2%OFST!=0))
        {
            get_cmp_ets(et2,&etr,&eti);

            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("SET aux_float\n");
            acc_ok = 0;

            oper_mult(etr,etr);         // parte real ao quadrado
            oper_mult(eti,eti);         // parte imag ao quadrado
            oper_soma(2*OFST,2*OFST);   // soma os quadrados
            add_instr("SET aux_cmp\n"); // salva o resultado

            add_instr("LOAD aux_float\n");
            oper_mult(2*OFST,etr);      // mult float com parte real
            add_instr("PLD aux_cmp\n"); // pega o denominador
            oper_divi(2*OFST,2*OFST);   // faz a divisao

            add_instr("PLD  aux_float\n");
            oper_mult(2*OFST,eti);      // mult float com parte imag
            add_instr("PLD aux_cmp\n"); // pega o denominador
            oper_divi(2*OFST,2*OFST);   // faz a divisao
            negacao  (2*OFST);
        }

        // int acc com comp acc
        if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==3) && (et2%OFST==0))
        {
            add_instr("SETP aux_imag  \n");           // salva a parte imaginaria
            add_instr("SETP aux_real  \n");           // salva a parte real
            add_instr("CALL int2float \n"); i2f  = 1; // converte int pra float
            add_instr("SET  aux_float \n");           // salva o float

            add_instr("LOAD aux_real  \n");           // coloca a parte real na pilha
            add_instr("PLD  aux_real  \n");           // coloca de novo
            add_instr("CALL float_mult\n"); fmlt = 1; // parte real ao quadrado
            add_instr("PLD  aux_imag  \n");           // pega a parte imaginaria
            add_instr("PLD  aux_imag  \n");           // pega ela de novo
            add_instr("CALL float_mult\n"); fmlt = 1; // parte imag ao quadrado
            add_instr("CALL denorm    \n");           // prepara a soma
            add_instr("CALL float_add \n"); fadd = 1; // soma os quadrados
            add_instr("SET  aux_cmp   \n");           // salva o modulo ao quadrado

            add_instr("LOAD  aux_float\n");           // carrega o float
            add_instr("PLD aux_real   \n");           // pega a parte real
            add_instr("CALL float_mult\n"); fmlt = 1; // mult float com parte real
            add_instr("PLD aux_cmp    \n");           // pega o modulo ao quadrado
            add_instr("CALL float_div \n"); fdiv = 1; // faz a divisao

            add_instr("PLD aux_float  \n");           // pega o float
            add_instr("PLD aux_imag   \n");           // pega a parte imag
            add_instr("CALL float_mult\n"); fmlt = 1; // mult float com parte imaginaria
            add_instr("PLD aux_cmp    \n");           // pega o modulo ao quadrado
            add_instr("CALL float_div \n"); fdiv = 1; // faz a divisao
            add_instr("PLD float_nbits\n");           // faz a negacao
            add_instr("SHL 1\n");
            add_instr("SADD\n");
        }

        // float var com int var
        if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==1) && (et2%OFST!=0))
        {
            add_instr("%s %s\n" , ld, v_name[et1%OFST]);
            add_instr("PLD %s\n",     v_name[et2%OFST]);
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("CALL float_div\n"); fdiv = 1;
        }

        // float var com int acc
        if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==1) && (et2%OFST==0))
        {
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("SET aux_float\n");
            add_instr("LOAD %s\n", v_name[et1%OFST]);
            add_instr("PLD aux_float\n");
            add_instr("CALL float_div\n"); fdiv = 1;
        }

        // float var com float var
        if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==0))
        {
            add_instr("%s %s\n" , ld, v_name[et1%OFST]);
            add_instr("PLD %s\n",     v_name[et2%OFST]);
            add_instr("CALL float_div\n"); fdiv = 1;
        }

        // float var com float const
        if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
        {
            add_instr("%s %s\n", ld, v_name[et1%OFST]);
            add_instr("PLD %d // %s\n", f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
            add_instr("CALL float_div\n"); fdiv = 1;
        }

        // float var com float acc
        if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==2) && (et2%OFST==0))
        {
            add_instr("SET aux_float\n");
            add_instr("LOAD %s\n", v_name[et1%OFST]);
            add_instr("PLD aux_float\n");
            add_instr("CALL float_div\n"); fdiv = 1;
        }

        // float var com comp const
        if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==5))
        {
            split_cmp_const(et2,&etr,&eti);

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
            negacao  (2*OFST);
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
            negacao  (2*OFST);
        }

        // float var com comp acc
        if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==0) && (get_type(et2)==3) && (et2%OFST==0))
        {
            add_instr("SETP aux_imag  \n");           // salva a parte imaginaria
            add_instr("SET  aux_real  \n");           // salva a parte real
            add_instr("PLD  aux_real  \n");           // coloca a parte real na pilha
            add_instr("CALL float_mult\n"); fmlt = 1; // parte real ao quadrado
            add_instr("PLD  aux_imag  \n");           // pega a parte imaginaria
            add_instr("PLD  aux_imag  \n");           // pega ela de novo
            add_instr("CALL float_mult\n"); fmlt = 1; // parte imag ao quadrado
            add_instr("CALL denorm    \n");           // prepara a soma
            add_instr("CALL float_add \n"); fadd = 1; // soma os quadrados
            add_instr("SET  aux_cmp   \n");           // salva o modulo ao quadrado

            add_instr("LOAD %s\n", v_name[et1%OFST]); // carrega o float
            add_instr("PLD aux_real   \n");           // pega a parte real
            add_instr("CALL float_mult\n"); fmlt = 1; // mult float com parte real
            add_instr("PLD aux_cmp    \n");           // pega o modulo ao quadrado
            add_instr("CALL float_div \n"); fdiv = 1; // faz a divisao

            add_instr("PLD %s\n", v_name[et1%OFST]);  // carrega o float
            add_instr("PLD aux_imag   \n");           // pega a parte imag
            add_instr("CALL float_mult\n"); fmlt = 1; // mult float com parte imaginaria
            add_instr("PLD aux_cmp    \n");           // pega o modulo ao quadrado
            add_instr("CALL float_div \n"); fdiv = 1; // faz a divisao
            add_instr("PLD float_nbits\n");           // faz a negacao
            add_instr("SHL 1\n");
            add_instr("SADD\n");
        }

        // float const com int var
        if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==1) && (et2%OFST!=0))
        {
            add_instr("%s %d // %s\n" , ld, f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
            add_instr("PLD %s\n", v_name[et2%OFST]);
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("CALL float_div\n"); fdiv = 1;
        }

        // float const com int acc
        if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==1) && (et2%OFST==0))
        {
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("SET aux_float\n");
            add_instr("LOAD %d // %s\n", f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
            add_instr("PLD aux_float\n");
            add_instr("CALL float_div\n"); fdiv = 1;
        }

        // float const com float var
        if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==0))
        {
            add_instr("%s %d // %s\n" , ld, f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
            add_instr("PLD %s\n",     v_name[et2%OFST]);
            add_instr("CALL float_div\n"); fdiv = 1;
        }

        // float const com float const
        if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
        {
            add_instr("%s %d // %s\n", ld, f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
            add_instr("PLD %d // %s\n", f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
            add_instr("CALL float_div\n"); fdiv = 1;
        }

        // float const com float acc
        if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==2) && (et2%OFST==0))
        {
            add_instr("SET aux_float\n");
            add_instr("LOAD %d // %s\n", f2mf(v_name[et1%OFST]), v_name[et1%OFST]);
            add_instr("PLD aux_float\n");
            add_instr("CALL float_div\n"); fdiv = 1;
        }

        // float const com comp const
        if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==5))
        {
            split_cmp_const(et2,&etr,&eti);

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
            negacao  (2*OFST);
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
            negacao  (2*OFST);
        }

        // float const com comp acc
        if ((get_type(et1)==2) && (et1%OFST!=0) && (v_isco[et1%OFST]==1) && (get_type(et2)==3) && (et2%OFST==0))
        {
            add_instr("SETP aux_imag  \n");           // salva a parte imaginaria
            add_instr("SET  aux_real  \n");           // salva a parte real
            add_instr("PLD  aux_real  \n");           // coloca a parte real na pilha
            add_instr("CALL float_mult\n"); fmlt = 1; // parte real ao quadrado
            add_instr("PLD  aux_imag  \n");           // pega a parte imaginaria
            add_instr("PLD  aux_imag  \n");           // pega ela de novo
            add_instr("CALL float_mult\n"); fmlt = 1; // parte imag ao quadrado
            add_instr("CALL denorm    \n");           // prepara a soma
            add_instr("CALL float_add \n"); fadd = 1; // soma os quadrados
            add_instr("SET  aux_cmp   \n");           // salva o modulo ao quadrado

            add_instr("LOAD %d // %s\n", f2mf(v_name[et1%OFST]), v_name[et1%OFST]); // carrega o float
            add_instr("PLD aux_real   \n");           // pega a parte real
            add_instr("CALL float_mult\n"); fmlt = 1; // mult float com parte real
            add_instr("PLD aux_cmp    \n");           // pega o modulo ao quadrado
            add_instr("CALL float_div \n"); fdiv = 1; // faz a divisao

            add_instr("PLD %d // %s\n", f2mf(v_name[et1%OFST]), v_name[et1%OFST]); // carrega o float
            add_instr("PLD aux_imag   \n");           // pega a parte imag
            add_instr("CALL float_mult\n"); fmlt = 1; // mult float com parte imaginaria
            add_instr("PLD aux_cmp    \n");           // pega o modulo ao quadrado
            add_instr("CALL float_div \n"); fdiv = 1; // faz a divisao
            add_instr("PLD float_nbits\n");           // faz a negacao
            add_instr("SHL 1\n");
            add_instr("SADD\n");
        }

        // float acc com int var
        if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==1) && (et2%OFST!=0))
        {
            add_instr("PLD %s\n", v_name[et2%OFST]);
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("CALL float_div\n"); fdiv = 1;
        }

        // float acc com int acc
        if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==1) && (et2%OFST==0))
        {
            add_instr("CALL int2float\n"); i2f  = 1;
            add_instr("CALL float_div\n"); fdiv = 1;
        }

        // float acc com float var
        if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==0))
        {
            add_instr("PLD %s\n", v_name[et2%OFST]);
            add_instr("CALL float_div\n"); fdiv = 1;
        }

        // float acc com float const
        if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
        {
            add_instr("PLD %d // %s\n", f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
            add_instr("CALL float_div\n"); fdiv = 1;
        }

        // float acc com float acc
        if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST==0))
        {
            add_instr("CALL float_div\n"); fdiv = 1;
        }

        // float acc com comp const
        if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==5))
        {
            split_cmp_const(et2,&etr,&eti);

            add_instr("SET aux_float\n");
            acc_ok = 0;

            oper_mult(etr,etr);         // parte real ao quadrado
            oper_mult(eti,eti);         // parte imag ao quadrado
            oper_soma(2*OFST,2*OFST);   // soma os quadrados
            add_instr("SET aux_cmp\n"); // salva o resultado

            add_instr("LOAD aux_float\n");
            oper_mult(2*OFST,etr);      // mult float com parte real
            add_instr("PLD aux_cmp\n"); // pega o denominador
            oper_divi(2*OFST,2*OFST);   // faz a divisao

            add_instr("PLD  aux_float\n");
            oper_mult(2*OFST,eti);      // mult float com parte imag
            add_instr("PLD aux_cmp\n"); // pega o denominador
            oper_divi(2*OFST,2*OFST);   // faz a divisao
            negacao  (2*OFST);
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

            add_instr("LOAD aux_float\n");
            oper_mult(2*OFST,etr);      // mult float com parte real
            add_instr("PLD aux_cmp\n"); // pega o denominador
            oper_divi(2*OFST,2*OFST);   // faz a divisao

            add_instr("PLD  aux_float\n");
            oper_mult(2*OFST,eti);      // mult float com parte imag
            add_instr("PLD aux_cmp\n"); // pega o denominador
            oper_divi(2*OFST,2*OFST);   // faz a divisao
            negacao  (2*OFST);
        }

        // float acc com comp acc
        if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==3) && (et2%OFST==0))
        {
            add_instr("SETP aux_imag  \n");           // salva a parte imaginaria
            add_instr("SETP aux_real  \n");           // salva a parte real
            add_instr("SET  aux_float \n");           // salva o float

            add_instr("LOAD aux_real  \n");           // coloca a parte real na pilha
            add_instr("PLD  aux_real  \n");           // coloca de novo
            add_instr("CALL float_mult\n"); fmlt = 1; // parte real ao quadrado
            add_instr("PLD  aux_imag  \n");           // pega a parte imaginaria
            add_instr("PLD  aux_imag  \n");           // pega ela de novo
            add_instr("CALL float_mult\n"); fmlt = 1; // parte imag ao quadrado
            add_instr("CALL denorm    \n");           // prepara a soma
            add_instr("CALL float_add \n"); fadd = 1; // soma os quadrados
            add_instr("SET  aux_cmp   \n");           // salva o modulo ao quadrado

            add_instr("LOAD  aux_float\n");           // carrega o float
            add_instr("PLD aux_real   \n");           // pega a parte real
            add_instr("CALL float_mult\n"); fmlt = 1; // mult float com parte real
            add_instr("PLD aux_cmp    \n");           // pega o modulo ao quadrado
            add_instr("CALL float_div \n"); fdiv = 1; // faz a divisao

            add_instr("PLD aux_float  \n");           // pega o float
            add_instr("PLD aux_imag   \n");           // pega a parte imag
            add_instr("CALL float_mult\n"); fmlt = 1; // mult float com parte imaginaria
            add_instr("PLD aux_cmp    \n");           // pega o modulo ao quadrado
            add_instr("CALL float_div \n"); fdiv = 1; // faz a divisao
            add_instr("PLD float_nbits\n");           // faz a negacao
            add_instr("SHL 1\n");
            add_instr("SADD\n");
        }

        // comp const com int var
        if ((get_type(et1)==5) && (get_type(et2)==1) && (et2%OFST!=0))
        {
            split_cmp_const(et1,&etr,&eti);

            oper_divi(etr,et2);
            oper_divi(eti,et2);
        }

        // comp const com int acc
        if ((get_type(et1)==5) && (get_type(et2)==1) && (et2%OFST==0))
        {
            split_cmp_const(et1,&etr,&eti);

            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("SET aux_div\n");
            oper_divi(etr,2*OFST);
            add_instr("PLD aux_div\n");
            oper_divi(eti,2*OFST);
        }

        // comp const com float var
        if ((get_type(et1)==5) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==0))
        {
            split_cmp_const(et1,&etr,&eti);

            oper_divi(etr,et2);
            oper_divi(eti,et2);
        }

        // comp const com float const
        if ((get_type(et1)==5) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
        {
            split_cmp_const(et1,&etr,&eti);

            oper_divi(etr,et2);
            oper_divi(eti,et2);
        }

        // comp const com float acc
        if ((get_type(et1)==5) && (get_type(et2)==2) && (et2%OFST==0))
        {
            split_cmp_const(et1,&etr,&eti);

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

            split_cmp_const(et1,&et1r,&et1i);
            split_cmp_const(et2,&et2r,&et2i);

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

            split_cmp_const(et1,&et1r,&et1i);
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
            split_cmp_const(et1,&etr,&eti);

            add_instr("SETP aux_imag\n");
            add_instr("SET  aux_real\n");
            add_instr("PLD  aux_real\n");
            add_instr("CALL float_mult\n"); fmlt = 1;
            add_instr("PLD  aux_imag\n");
            add_instr("PLD  aux_imag\n");
            add_instr("CALL float_mult\n"); fmlt = 1;
            oper_soma(2*OFST,2*OFST);
            add_instr("SET aux_mod\n");

            add_instr("LOAD aux_real\n");
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

            add_instr("CALL int2float\n"); i2f = 1;
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
            split_cmp_const(et2,&et2r,&et2i);

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
            add_instr("PLD  aux_real\n");
            add_instr("CALL float_mult\n"); fmlt = 1;
            add_instr("PLD  aux_imag\n");
            add_instr("PLD  aux_imag\n");
            add_instr("CALL float_mult\n"); fmlt = 1;
            oper_soma(2*OFST,2*OFST);
            add_instr("SET aux_mod\n");

            add_instr("LOAD aux_real\n");
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
            add_instr("PLD %s\n", v_name[et2%OFST]);
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("SET aux_float\n");
            add_instr("CALL float_div\n"); fdiv = 1;
            add_instr("PLD aux_imag\n");
            add_instr("PLD aux_float\n");
            add_instr("CALL float_div\n"); fdiv = 1;
        }

        // comp acc com int acc
        if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==1) && (et2%OFST==0))
        {
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("SETP aux_float\n");
            add_instr("SETP aux_imag\n");
            add_instr("PLD aux_float\n");
            add_instr("CALL float_div\n"); fdiv = 1;
            add_instr("PLD aux_imag\n");
            add_instr("PLD aux_float\n");
            add_instr("CALL float_div\n"); fdiv = 1;
        }

        // comp acc com float var
        if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==0))
        {
            add_instr("SETP aux_imag\n");
            add_instr("PLD %s\n", v_name[et2%OFST]);
            add_instr("CALL float_div\n"); fdiv = 1;
            add_instr("PLD aux_imag\n");
            add_instr("PLD %s\n", v_name[et2%OFST]);
            add_instr("CALL float_div\n"); fdiv = 1;
        }

        // comp acc com float const
        if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST!=0) && (v_isco[et2%OFST]==1))
        {
            add_instr("SETP aux_imag\n");
            add_instr("PLD %d // %s\n", f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
            add_instr("CALL float_div\n"); fdiv = 1;
            add_instr("PLD aux_imag\n");
            add_instr("PLD %d // %s\n", f2mf(v_name[et2%OFST]), v_name[et2%OFST]);
            add_instr("CALL float_div\n"); fdiv = 1;
        }

        // comp acc com float acc
        if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST==0))
        {
            add_instr("SETP aux_float\n");
            add_instr("SETP aux_imag\n");
            add_instr("PLD aux_float\n");
            add_instr("CALL float_div\n"); fdiv = 1;
            add_instr("PLD aux_imag\n");
            add_instr("PLD aux_float\n");
            add_instr("CALL float_div\n"); fdiv = 1;
        }

        // comp acc com comp const
        if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==5))
        {
            split_cmp_const(et2,&etr,&eti);

            add_instr("SETP aux_imag\n");
            add_instr("SET  aux_real\n");
            acc_ok = 0;

            oper_mult(etr,etr);
            oper_mult(eti,eti);
            oper_soma(2*OFST,2*OFST);
            add_instr("SET aux_mod\n");

            add_instr("LOAD aux_real\n");
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

            add_instr("LOAD aux_real\n");
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

            add_instr("LOAD aux_real2 \n");
            add_instr("PLD  aux_real2 \n");
            add_instr("CALL float_mult\n"); fmlt = 1;
            add_instr("PLD  aux_imag2\n");
            add_instr("PLD  aux_imag2\n");
            add_instr("CALL float_mult\n"); fmlt = 1;
            add_instr("CALL denorm    \n");
            add_instr("CALL float_add \n"); fadd = 1;
            add_instr("SET aux_mod    \n");

            add_instr("LOAD aux_real1 \n");
            add_instr("PLD  aux_real2 \n");
            add_instr("CALL float_mult\n"); fmlt = 1;
            add_instr("PLD  aux_imag1 \n");
            add_instr("PLD  aux_imag2 \n");
            add_instr("CALL float_mult\n"); fmlt = 1;
            add_instr("CALL denorm    \n");
            add_instr("CALL float_add \n"); fadd = 1;
            add_instr("PLD  aux_mod   \n");
            add_instr("CALL float_div \n"); fdiv = 1;

            add_instr("PLD  aux_real1 \n");
            add_instr("PLD  aux_imag2 \n");
            add_instr("CALL float_mult\n"); fmlt = 1;
            add_instr("PLD  aux_imag1 \n");
            add_instr("PLD  aux_real2 \n");
            add_instr("CALL float_mult\n"); fmlt = 1;
            add_instr("PLD float_nbits\n");
            add_instr("SHL 1          \n");
            add_instr("SADD           \n");
            add_instr("CALL denorm    \n");
            add_instr("CALL float_add \n"); fadd = 1;
            add_instr("PLD  aux_mod   \n");
            add_instr("CALL float_div \n"); fdiv = 1;
        }
    }
    else // 7*7 combinacoes para prtype = 1
    {
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
        if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==2) && (et2%OFST!=0))
        {
            add_instr("%s %s\n" , ld, v_name[et2%OFST]);
            add_instr("DIV %s\n",     v_name[et1%OFST]);
        }

        // int var com float acc
        if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==2) && (et2%OFST==0))
        {
            add_instr("DIV %s\n", v_name[et1%OFST]);
        }

        // int var com comp const
        if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==5))
        {
            split_cmp_const(et2,&etr,&eti);

            oper_mult(etr,etr );        // parte real ao quadrado
            oper_mult(eti,eti );        // parte imag ao quadrado
            add_instr("SADD\n");        // soma os quadrados
            add_instr("SET aux_cmp\n"); // salva o resultado
            acc_ok = 0;

            oper_mult(et1,etr );        // mult int com parte real
            add_instr("PLD aux_cmp\n"); // pega o denominador
            add_instr("SDIV\n");        // faz a divisao

            oper_mult(et1,eti );        // mult int com parte imag
            add_instr("PLD aux_cmp\n"); // pega o denominador
            add_instr("SDIV\n");        // faz a divisao
            add_instr("NEG\n" );
        }

        // int var com comp var
        if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==3) && (et2%OFST!=0))
        {
            get_cmp_ets(et2,&etr,&eti);

            oper_mult(etr,etr );        // parte real ao quadrado
            oper_mult(eti,eti );        // parte imag ao quadrado
            add_instr("SADD\n");        // soma os quadrados
            add_instr("SET aux_cmp\n"); // salva o resultado
            acc_ok = 0;

            oper_mult(et1,etr );        // mult int com parte real
            add_instr("PLD aux_cmp\n"); // pega o denominador
            add_instr("SDIV\n");        // faz a divisao

            oper_mult(et1,eti );        // mult int com parte imag
            add_instr("PLD aux_cmp\n"); // pega o denominador
            add_instr("SDIV\n");        // faz a divisao
            add_instr("NEG\n" );
        }

        // int var com comp acc
        if ((get_type(et1)==1) && (et1%OFST!=0) && (get_type(et2)==3) && (et2%OFST==0))
        {
            add_instr("SETP aux_imag  \n");           // salva a parte imaginaria
            add_instr("SET  aux_real  \n");           // salva a parte real

            add_instr("MLT  aux_real  \n");           // parte real ao quadrado
            add_instr("PLD  aux_imag  \n");           // pega a parte imaginaria
            add_instr("MLT  aux_imag  \n");           // parte imag ao quadrado
            add_instr("SADD           \n");           // soma os quadrados
            add_instr("SET  aux_cmp   \n");           // salva o modulo ao quadrado

            add_instr("LOAD %s\n", v_name[et1%OFST]); // carrega o num inteiro
            add_instr("MLT aux_real   \n");           // mult com parte real
            add_instr("PLD aux_cmp    \n");           // pega o modulo ao quadrado
            add_instr("SDIV           \n");           // faz a divisao

            add_instr("PLD %s\n", v_name[et1%OFST]);  // carrega o num inteiro
            add_instr("MLT aux_imag   \n");           // mult com parte imaginaria
            add_instr("PLD aux_cmp    \n");           // pega o modulo ao quadrado
            add_instr("SDIV           \n");           // faz a divisao
            add_instr("NEG            \n");           // faz a negacao
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
        if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST!=0))
        {
            add_instr("%s %s\n", ld, v_name[et2%OFST]);
            add_instr("SDIV\n");
        }

        // int acc com float acc
        if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST==0))
        {
            add_instr("SDIV\n");
        }

        // int acc com comp const
        if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==5))
        {
            split_cmp_const(et2,&etr,&eti);

            add_instr("SET aux_float\n"); // salva num
            acc_ok = 0;

            oper_mult(etr,etr );          // parte real ao quadrado
            oper_mult(eti,eti );          // parte imag ao quadrado
            add_instr("SADD\n");          // soma os quadrados
            add_instr("SET aux_cmp\n");   // salva o resultado
            acc_ok = 0;

            add_instr("LOAD aux_float\n");// carrega num
            oper_mult(OFST,etr);          // mult num com parte real
            add_instr("PLD aux_cmp\n");   // pega o denominador
            add_instr("SDIV\n");          // faz a divisao

            add_instr("PLD aux_float\n"); // carrega num
            oper_mult(OFST,eti);          // mult num com parte imag
            add_instr("PLD aux_cmp\n");   // pega o denominador
            add_instr("SDIV\n");          // faz a divisao
            add_instr("NEG\n" );
        }

        // int acc com comp var
        if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==3) && (et2%OFST!=0))
        {
            get_cmp_ets(et2,&etr,&eti);

            add_instr("SET aux_float\n"); // salva num
            acc_ok = 0;

            oper_mult(etr,etr );          // parte real ao quadrado
            oper_mult(eti,eti );          // parte imag ao quadrado
            add_instr("SADD\n");          // soma os quadrados
            add_instr("SET aux_cmp\n");   // salva o resultado
            acc_ok = 0;

            add_instr("LOAD aux_float\n");// carrega num
            oper_mult(OFST,etr);          // mult num com parte real
            add_instr("PLD aux_cmp\n");   // pega o denominador
            add_instr("SDIV\n");          // faz a divisao

            add_instr("PLD aux_float\n"); // carrega num
            oper_mult(OFST,eti);          // mult num com parte imag
            add_instr("PLD aux_cmp\n");   // pega o denominador
            add_instr("SDIV\n");          // faz a divisao
            add_instr("NEG\n" );
        }

        // int acc com comp acc
        if ((get_type(et1)==1) && (et1%OFST==0) && (get_type(et2)==3) && (et2%OFST==0))
        {
            add_instr("SETP aux_imag  \n");           // salva a parte imaginaria
            add_instr("SETP aux_real  \n");           // salva a parte real
            add_instr("SET  aux_float \n");           // salva o float

            add_instr("LOAD aux_real  \n");           // pega parte real
            add_instr("MLT  aux_real  \n");           // real ao quadrado
            add_instr("PLD  aux_imag  \n");           // pega a parte imaginaria
            add_instr("MLT  aux_imag  \n");           // imag ao quadrado
            add_instr("SADD           \n");           // soma os quadrados
            add_instr("SET  aux_cmp   \n");           // salva o modulo ao quadrado

            add_instr("LOAD  aux_float\n");           // carrega o float
            add_instr("MLT   aux_real \n");           // mult com parte real
            add_instr("PLD aux_cmp    \n");           // pega o modulo ao quadrado
            add_instr("SDIV           \n");           // faz a divisao

            add_instr("PLD aux_float  \n");           // pega o float
            add_instr("MLT aux_imag   \n");           // multiplica com parte imag
            add_instr("PLD aux_cmp    \n");           // pega o modulo ao quadrado
            add_instr("SDIV           \n");           // faz a divisao
            add_instr("NEG            \n");           // faz a negacao
        }

        // float var com int var
        if ((get_type(et1)==2) && (et1%OFST!=0) && (get_type(et2)==1) && (et2%OFST!=0))
        {
            add_instr("%s %s\n" , ld, v_name[et2%OFST]);
            add_instr("DIV %s\n",     v_name[et1%OFST]);
        }

        // float var com int acc
        if ((get_type(et1)==2) && (et1%OFST!=0) && (get_type(et2)==1) && (et2%OFST==0))
        {
            add_instr("DIV %s\n", v_name[et1%OFST]);
        }

        // float var com float var
        if ((get_type(et1)==2) && (et1%OFST!=0) && (get_type(et2)==2) && (et2%OFST!=0))
        {
            add_instr("%s %s\n" , ld, v_name[et2%OFST]);
            add_instr("DIV %s\n",     v_name[et1%OFST]);
        }

        // float var com float acc
        if ((get_type(et1)==2) && (et1%OFST!=0) && (get_type(et2)==2) && (et2%OFST==0))
        {
            add_instr("DIV %s\n", v_name[et1%OFST]);
        }

        // float var com comp const
        if ((get_type(et1)==2) && (et1%OFST!=0) && (get_type(et2)==5))
        {
            split_cmp_const(et2,&etr,&eti);

            oper_mult(etr,etr );        // parte real ao quadrado
            oper_mult(eti,eti );        // parte imag ao quadrado
            add_instr("SADD\n");        // soma os quadrados
            add_instr("SET aux_cmp\n"); // salva o resultado
            acc_ok = 0;

            oper_mult(et1,etr );        // mult int com parte real
            add_instr("PLD aux_cmp\n"); // pega o denominador
            add_instr("SDIV\n");        // faz a divisao

            oper_mult(et1,eti );        // mult int com parte imag
            add_instr("PLD aux_cmp\n"); // pega o denominador
            add_instr("SDIV\n");        // faz a divisao
            add_instr("NEG\n" );
        }

        // float var com comp var
        if ((get_type(et1)==2) && (et1%OFST!=0) && (get_type(et2)==3) && (et2%OFST!=0))
        {
            get_cmp_ets(et2,&etr,&eti);

            oper_mult(etr,etr );        // parte real ao quadrado
            oper_mult(eti,eti );        // parte imag ao quadrado
            add_instr("SADD\n");        // soma os quadrados
            add_instr("SET aux_cmp\n"); // salva o resultado
            acc_ok = 0;

            oper_mult(et1,etr );        // mult int com parte real
            add_instr("PLD aux_cmp\n"); // pega o denominador
            add_instr("SDIV\n");        // faz a divisao

            oper_mult(et1,eti );        // mult int com parte imag
            add_instr("PLD aux_cmp\n"); // pega o denominador
            add_instr("SDIV\n");        // faz a divisao
            add_instr("NEG\n" );
        }

        // float var com comp acc
        if ((get_type(et1)==2) && (et1%OFST!=0) && (get_type(et2)==3) && (et2%OFST==0))
        {
            add_instr("SETP aux_imag  \n");           // salva a parte imaginaria
            add_instr("SET  aux_real  \n");           // salva a parte real

            add_instr("MLT  aux_real  \n");           // parte real ao quadrado
            add_instr("PLD  aux_imag  \n");           // pega a parte imaginaria
            add_instr("MLT  aux_imag  \n");           // parte imag ao quadrado
            add_instr("SADD           \n");           // soma os quadrados
            add_instr("SET  aux_cmp   \n");           // salva o modulo ao quadrado

            add_instr("LOAD %s\n", v_name[et1%OFST]); // carrega o num inteiro
            add_instr("MLT aux_real   \n");           // mult com parte real
            add_instr("PLD aux_cmp    \n");           // pega o modulo ao quadrado
            add_instr("SDIV           \n");           // faz a divisao

            add_instr("PLD %s\n", v_name[et1%OFST]);  // carrega o num inteiro
            add_instr("MLT aux_imag   \n");           // mult com parte imaginaria
            add_instr("PLD aux_cmp    \n");           // pega o modulo ao quadrado
            add_instr("SDIV           \n");           // faz a divisao
            add_instr("NEG            \n");           // faz a negacao
        }

        // float acc com int var
        if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==1) && (et2%OFST!=0))
        {
            add_instr("%s %s\n", ld, v_name[et2%OFST]);
            add_instr("SDIV\n");
        }

        // float acc com int acc
        if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==1) && (et2%OFST==0))
        {
            add_instr("SDIV\n");
        }

        // float acc com float var
        if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST!=0))
        {
            add_instr("%s %s\n", ld, v_name[et2%OFST]);
            add_instr("SDIV\n");
        }

        // float acc com float acc
        if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST==0))
        {
            add_instr("SDIV\n");
        }

        // float acc com comp const
        if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==5))
        {
            split_cmp_const(et2,&etr,&eti);

            add_instr("SET aux_float\n"); // salva num
            acc_ok = 0;

            oper_mult(etr,etr );          // parte real ao quadrado
            oper_mult(eti,eti );          // parte imag ao quadrado
            add_instr("SADD\n");          // soma os quadrados
            add_instr("SET aux_cmp\n");   // salva o resultado
            acc_ok = 0;

            add_instr("LOAD aux_float\n");// carrega num
            oper_mult(OFST,etr);          // mult num com parte real
            add_instr("PLD aux_cmp\n");   // pega o denominador
            add_instr("SDIV\n");          // faz a divisao

            add_instr("PLD aux_float\n"); // carrega num
            oper_mult(OFST,eti);          // mult num com parte imag
            add_instr("PLD aux_cmp\n");   // pega o denominador
            add_instr("SDIV\n");          // faz a divisao
            add_instr("NEG\n" );
        }

        // float acc com comp var
        if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==3) && (et2%OFST!=0))
        {
            get_cmp_ets(et2,&etr,&eti);

            add_instr("SET aux_float\n"); // salva num
            acc_ok = 0;

            oper_mult(etr,etr );          // parte real ao quadrado
            oper_mult(eti,eti );          // parte imag ao quadrado
            add_instr("SADD\n");          // soma os quadrados
            add_instr("SET aux_cmp\n");   // salva o resultado
            acc_ok = 0;

            add_instr("LOAD aux_float\n");// carrega num
            oper_mult(OFST,etr);          // mult num com parte real
            add_instr("PLD aux_cmp\n");   // pega o denominador
            add_instr("SDIV\n");          // faz a divisao

            add_instr("PLD aux_float\n"); // carrega num
            oper_mult(OFST,eti);          // mult num com parte imag
            add_instr("PLD aux_cmp\n");   // pega o denominador
            add_instr("SDIV\n");          // faz a divisao
            add_instr("NEG\n" );
        }

        // float acc com comp acc
        if ((get_type(et1)==2) && (et1%OFST==0) && (get_type(et2)==3) && (et2%OFST==0))
        {
            add_instr("SETP aux_imag  \n");           // salva a parte imaginaria
            add_instr("SETP aux_real  \n");           // salva a parte real
            add_instr("SET  aux_float \n");           // salva o float

            add_instr("LOAD aux_real  \n");           // pega parte real
            add_instr("MLT  aux_real  \n");           // real ao quadrado
            add_instr("PLD  aux_imag  \n");           // pega a parte imaginaria
            add_instr("MLT  aux_imag  \n");           // imag ao quadrado
            add_instr("SADD           \n");           // soma os quadrados
            add_instr("SET  aux_cmp   \n");           // salva o modulo ao quadrado

            add_instr("LOAD  aux_float\n");           // carrega o float
            add_instr("MLT   aux_real \n");           // mult com parte real
            add_instr("PLD aux_cmp    \n");           // pega o modulo ao quadrado
            add_instr("SDIV           \n");           // faz a divisao

            add_instr("PLD aux_float  \n");           // pega o float
            add_instr("MLT aux_imag   \n");           // multiplica com parte imag
            add_instr("PLD aux_cmp    \n");           // pega o modulo ao quadrado
            add_instr("SDIV           \n");           // faz a divisao
            add_instr("NEG            \n");           // faz a negacao
        }

        // comp const com int var
        if ((get_type(et1)==5) && (get_type(et2)==1) && (et2%OFST!=0))
        {
            split_cmp_const(et1,&etr,&eti);

            add_instr("%s %s\n" , ld, v_name[et2%OFST]);
            add_instr("DIV %s\n",     v_name[etr%OFST]);
            add_instr("PLD %s\n",     v_name[et2%OFST]);
            add_instr("DIV %s\n",     v_name[eti%OFST]);
        }

        // comp const com int acc
        if ((get_type(et1)==5) && (get_type(et2)==1) && (et2%OFST==0))
        {
            split_cmp_const(et1,&etr,&eti);

            add_instr("SET aux_mult\n");
            add_instr("DIV %s\n",     v_name[etr%OFST]);
            add_instr("PLD aux_mult\n");
            add_instr("DIV %s\n",     v_name[eti%OFST]);
            
        }

        // comp const com float var
        if ((get_type(et1)==5) && (get_type(et2)==2) && (et2%OFST!=0))
        {
            split_cmp_const(et1,&etr,&eti);

            add_instr("%s %s\n" , ld, v_name[et2%OFST]);
            add_instr("DIV %s\n",     v_name[etr%OFST]);
            add_instr("PLD %s\n",     v_name[et2%OFST]);
            add_instr("DIV %s\n",     v_name[eti%OFST]);
        }

        // comp const com float acc
        if ((get_type(et1)==5) && (get_type(et2)==2) && (et2%OFST==0))
        {
            split_cmp_const(et1,&etr,&eti);

            add_instr("SET aux_mult\n");
            add_instr("DIV %s\n",     v_name[etr%OFST]);
            add_instr("PLD aux_mult\n");
            add_instr("DIV %s\n",     v_name[eti%OFST]);
        }

        // comp const com comp const
        if ((get_type(et1)==5) && (get_type(et2)==5))
        {
            int et1r,et2r;
            int et1i,et2i;

            split_cmp_const(et1,&et1r,&et1i);
            split_cmp_const(et2,&et2r,&et2i);

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

            split_cmp_const(et1,&et1r,&et1i);
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
            split_cmp_const(et1,&etr,&eti);

            add_instr("SETP aux_imag\n");
            add_instr("SET  aux_real\n");
            add_instr("MLT  aux_real\n");
            add_instr("PLD  aux_imag\n");
            add_instr("MLT  aux_imag\n");
            add_instr("SADD         \n");
            add_instr("SET  aux_mod \n");

            add_instr("LOAD aux_real\n");
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

            add_instr("%s %s\n" , ld, v_name[et2%OFST]);
            add_instr("DIV %s\n",     v_name[etr%OFST]);
            add_instr("PLD %s\n",     v_name[et2%OFST]);
            add_instr("DIV %s\n",     v_name[eti%OFST]);
        }

        // comp var com int acc
        if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==1) && (et2%OFST==0))
        {
            get_cmp_ets(et1,&etr,&eti);

            add_instr("SET aux_mult\n");
            add_instr("DIV %s\n",     v_name[etr%OFST]);
            add_instr("PLD aux_mult\n");
            add_instr("DIV %s\n",     v_name[eti%OFST]);
        }

        // comp var com float var
        if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==2) && (et2%OFST!=0))
        {
            get_cmp_ets(et1,&etr,&eti);

            add_instr("%s %s\n" , ld, v_name[et2%OFST]);
            add_instr("DIV %s\n",     v_name[etr%OFST]);
            add_instr("PLD %s\n",     v_name[et2%OFST]);
            add_instr("DIV %s\n",     v_name[eti%OFST]);
        }

        // comp var com float acc
        if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==2) && (et2%OFST==0))
        {
            get_cmp_ets(et1,&etr,&eti);

            add_instr("SET aux_mult\n");
            add_instr("DIV %s\n",     v_name[etr%OFST]);
            add_instr("PLD aux_mult\n");
            add_instr("DIV %s\n",     v_name[eti%OFST]);
        }

        // comp var com comp const
        if ((get_type(et1)==3) && (et1%OFST!=0) && (get_type(et2)==5))
        {
            int et1r,et2r;
            int et1i,et2i;

            get_cmp_ets    (et1,&et1r,&et1i);
            split_cmp_const(et2,&et2r,&et2i);

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
            add_instr("MLT  aux_real\n");
            add_instr("PLD  aux_imag\n");
            add_instr("MLT  aux_imag\n");
            add_instr("SADD         \n");
            add_instr("SET  aux_mod \n");

            add_instr("LOAD aux_real\n");
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
            add_instr("PLD %s\n", v_name[et2%OFST]);
            add_instr("SDIV\n");
            add_instr("PLD %s\n", v_name[et2%OFST]);
            add_instr("DIV aux_imag\n");
        }

        // comp acc com int acc
        if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==1) && (et2%OFST==0))
        {
            add_instr("SETP aux_int\n");
            add_instr("SETP aux_imag\n");
            add_instr("PLD aux_int\n");
            add_instr("SDIV\n");
            add_instr("PLD aux_int\n");
            add_instr("DIV aux_imag\n");
        }

        // comp acc com float var
        if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST!=0))
        {
            add_instr("SETP aux_imag\n");
            add_instr("PLD %s\n", v_name[et2%OFST]);
            add_instr("SDIV\n");
            add_instr("PLD %s\n", v_name[et2%OFST]);
            add_instr("DIV aux_imag\n");
        }

        // comp acc com float acc
        if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==2) && (et2%OFST==0))
        {
            add_instr("SETP aux_int\n");
            add_instr("SETP aux_imag\n");
            add_instr("PLD aux_int\n");
            add_instr("SDIV\n");
            add_instr("PLD aux_int\n");
            add_instr("DIV aux_imag\n");
        }

        // comp acc com comp const
        if ((get_type(et1)==3) && (et1%OFST==0) && (get_type(et2)==5))
        {
            split_cmp_const(et2,&etr,&eti);

            add_instr("SETP aux_imag\n");
            add_instr("SET  aux_real\n");
            acc_ok = 0;

            oper_mult(etr,etr);
            oper_mult(eti,eti);
            oper_soma(2*OFST,2*OFST);
            add_instr("SET aux_mod\n");

            add_instr("LOAD aux_real\n");
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

            add_instr("LOAD aux_real\n");
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

            add_instr("LOAD aux_real2 \n");
            add_instr("MLT  aux_real2 \n");
            add_instr("PLD  aux_imag2 \n");
            add_instr("MLT  aux_imag2 \n");
            add_instr("SADD           \n");
            add_instr("SET aux_mod    \n");

            add_instr("LOAD aux_real1 \n");
            add_instr("MLT  aux_real2 \n");
            add_instr("PLD  aux_imag1 \n");
            add_instr("MLT  aux_imag2 \n");
            add_instr("SADD           \n");
            add_instr("PLD  aux_mod   \n");
            add_instr("SDIV           \n");

            add_instr("PLD  aux_real1 \n");
            add_instr("MLT  aux_imag2 \n");
            add_instr("PLD  aux_imag1 \n");
            add_instr("MLT  aux_real2 \n");
            add_instr("NEG            \n");
            add_instr("SADD           \n");
            add_instr("PLD  aux_mod   \n");
            add_instr("SDIV           \n");
        }
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

int oper_lanor(int et1, int et2, int type)
{
    if ((get_type(et1) > 1) || (get_type(et2) > 1))
    fprintf(stderr, "Erro na linha %d: Operação lógica, só entre números inteiros!\n", line_num+1);

    int etr, eti;

    char ld[10];
    if (acc_ok == 0) strcpy(ld,"LOAD"); else strcpy(ld,"PLD");

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

// prepara a operacao com inteiro
int oper_int(int et1, int et2, int op)
{
    switch (op)
    {
        case  0: int_oper(et1,  0,"!"  ,"LINV",1); break;
        case  1: int_oper(et1,  0,"~"  , "INV",0); break;
        case  2: int_oper(et1,et2,"<<" , "SHL",0); break;
        case  3: int_oper(et1,et2,">>" , "SHR",0); break;
        case  4: int_oper(et1,et2,">>>", "SRS",0); break;
        case  5: int_oper(et1,et2, "&" , "AND",0); break;
        case  6: int_oper(et1,et2, "|" ,  "OR",0); break;
        case  7: int_oper(et1,et2, "^" , "XOR",0); break;
        case  8: int_oper(et1,et2, "&&","LAND",1); break;
        case  9: int_oper(et1,et2, "||", "LOR",1); break;
        case 10: int_oper(et1,et2,  "%", "MOD",0); break;
    }

    return OFST;
}

// operacoes de comparacao com num complexos
// as comparacoes sao feitas entre os valores absolutos
// pra evitar conflito com <= e >=
int oper_cmp_cmp(int et1, int et2, int op)
{
    int l_type = get_type(et1);
    int r_type = get_type(et2);

    int id;

    // left int var com right comp const --------------------------------------

    if ((l_type == 1) && (r_type == 5) && (et1 % OFST != 0))
    {
        et2 =   mod_sqr(et2);
        oper_cmp(et1,et2,op);
    }

    // left int var com right comp var ----------------------------------------

    if ((l_type == 1) && (r_type == 3) && (et1 % OFST != 0) && (et2 % OFST != 0))
    {
        et2 =   mod_sqr(et2);
        oper_cmp(et1,et2,op);
    }

    // left int var com right comp acc ----------------------------------------

    if ((l_type == 1) && (r_type == 3) && (et1 % OFST != 0) && (et2 % OFST == 0))
    {
        et2 =   mod_sqr(et2);
        oper_cmp(et1,et2,op);
    }

    // left int acc com right comp const ----------------------------------------

    if ((l_type == 1) && (r_type == 5) && (et1 % OFST == 0))
    {
        et2 =   mod_sqr(et2);
        oper_cmp(et1,et2,op);
    }

    // left int acc com right comp var ----------------------------------------

    if ((l_type == 1) && (r_type == 3) && (et1 % OFST == 0) && (et2 % OFST != 0))
    {
        et2 =   mod_sqr(et2);
        oper_cmp(et1,et2,op);
    }

    // left int acc com right comp acc ----------------------------------------

    if ((l_type == 1) && (r_type == 3) && (et1 % OFST == 0) && (et2 % OFST == 0))
    {
        et2 =   mod_sqr(et2);
        oper_cmp(et1,et2,op);
    }

    // left float var com right comp const ------------------------------------

    if ((l_type == 2) && (r_type == 5) && (et1 % OFST != 0))
    {
        et2 =   mod_sqr(et2);
        oper_cmp(et1,et2,op);
    }

    // left float var com right comp var --------------------------------------

    if ((l_type == 2) && (r_type == 3) && (et1 % OFST != 0) && (et2 % OFST != 0))
    {
        et2 =   mod_sqr(et2);
        oper_cmp(et1,et2,op);
    }

    // left float var com right comp acc --------------------------------------

    if ((l_type == 2) && (r_type == 3) && (et1 % OFST != 0) && (et2 % OFST == 0))
    {
        et2 =   mod_sqr(et2);
        oper_cmp(et1,et2,op);
    }

    // left float acc com right comp const ------------------------------------

    if ((l_type == 2) && (r_type == 5) && (et1 % OFST == 0))
    {
        et2 =   mod_sqr(et2);
        oper_cmp(et1,et2,op);
    }

    // left float acc com right comp var --------------------------------------

    if ((l_type == 2) && (r_type == 3) && (et1 % OFST == 0) && (et2 % OFST != 0))
    {
        et2 =   mod_sqr(et2);
        oper_cmp(et1,et2,op);
    }

    // left float acc com right comp acc --------------------------------------

    if ((l_type == 2) && (r_type == 3) && (et1 % OFST == 0) && (et2 % OFST == 0))
    {
        et2 =   mod_sqr(et2);
        oper_cmp(et1,et2,op);
    }

    // left comp const com right int var --------------------------------------

    if ((l_type == 5) && (r_type == 1) && (et2 % OFST != 0))
    {
        et1 =   mod_sqr(et1);
        oper_cmp(et1,et2,op);
    }

    // left comp const com right int acc --------------------------------------

    if ((l_type == 5) && (r_type == 1) && (et2 % OFST == 0))
    {
        id = exec_id("aux_et2");
        et2 = OFST + id;

        add_instr("SETP %s\n", v_name[id]);
        acc_ok = 0;

        et1 =   mod_sqr(et1);
        oper_cmp(et1,et2,op);
    }

    // left comp const com right float var ------------------------------------

    if ((l_type == 5) && (r_type == 2) && (et2 % OFST != 0))
    {
        et1 =   mod_sqr(et1);
        oper_cmp(et1,et2,op);
    }

    // left comp const com right float acc ------------------------------------

    if ((l_type == 5) && (r_type == 2) && (et2 % OFST == 0))
    {
        id = exec_id("aux_et2");
        et2 = 2*OFST + id;

        add_instr("SETP %s\n", v_name[id]);
        acc_ok = 0;

        et1 =   mod_sqr(et1);
        oper_cmp(et1,et2,op);
    }

    // left comp const com right comp const -----------------------------------

    if ((l_type == 5) && (r_type == 5))
    {
        et1 =   mod_sqr(et1);
        et2 =   mod_sqr(et2);
        oper_cmp(et1,et2,op);
    }

    // left comp const com right comp var -------------------------------------

    if ((l_type == 5) && (r_type == 3) && (et2 % OFST != 0))
    {
        et1 =   mod_sqr(et1);
        et2 =   mod_sqr(et2);
        oper_cmp(et1,et2,op);
    }

    // left comp const com right comp acc -------------------------------------

    if ((l_type == 5) && (r_type == 3) && (et2 % OFST == 0))
    {
        et2 = mod_sqr(et2);

        id  = exec_id("aux_et2");
        et2 = 2*OFST + id;

        add_instr("SETP %s\n", v_name[id]);
        acc_ok = 0;

        et1 =   mod_sqr(et1);
        oper_cmp(et1,et2,op);
    }

    // left comp var com right int var ----------------------------------------

    if ((l_type == 3) && (r_type == 1) && (et1 % OFST != 0) && (et2 % OFST != 0))
    {
        et1 =   mod_sqr(et1);
        oper_cmp(et1,et2,op);
    }

    // left comp var com right int acc ----------------------------------------

    if ((l_type == 3) && (r_type == 1) && (et1 % OFST != 0) && (et2 % OFST == 0))
    {
        id  = exec_id("aux_et2");
        et2 = OFST + id;

        add_instr("SETP %s\n", v_name[id]);
        acc_ok = 0;

        et1 =   mod_sqr(et1);
        oper_cmp(et1,et2,op);
    }

    // left comp var com right float var --------------------------------------

    if ((l_type == 3) && (r_type == 2) && (et1 % OFST != 0) && (et2 % OFST != 0))
    {
        et1 =   mod_sqr(et1);
        oper_cmp(et1,et2,op);
    }

    // left comp var com right float acc --------------------------------------

    if ((l_type == 3) && (r_type == 2) && (et1 % OFST != 0) && (et2 % OFST == 0))
    {
        id  = exec_id("aux_et2");
        et2 = 2*OFST + id;

        add_instr("SETP %s\n", v_name[id]);
        acc_ok = 0;

        et1 =   mod_sqr(et1);
        oper_cmp(et1,et2,op);
    }

    // left comp var com right comp const -------------------------------------

    if ((l_type == 3) && (r_type == 5) && (et1 % OFST != 0))
    {
        et1 =   mod_sqr(et1);
        et2 =   mod_sqr(et2);
        oper_cmp(et1,et2,op);
    }

    // left comp var com right comp var ---------------------------------------

    if ((l_type == 3) && (r_type == 3) && (et1 % OFST != 0) && (et2 % OFST != 0))
    {
        et1 =   mod_sqr(et1);
        et2 =   mod_sqr(et2);
        oper_cmp(et1,et2,op);
    }

    // left comp var com right comp acc ---------------------------------------

    if ((l_type == 3) && (r_type == 3) && (et1 % OFST != 0) && (et2 % OFST == 0))
    {
        et2 = mod_sqr(et2);

        id  = exec_id("aux_et2");
        et2 = 2*OFST + id;

        add_instr("SETP %s\n", v_name[id]);
        acc_ok = 0;

        et1 =   mod_sqr(et1);
        oper_cmp(et1,et2,op);
    }

    // left comp acc com right int var ----------------------------------------

    if ((l_type == 3) && (r_type == 1) && (et1 % OFST == 0) && (et2 % OFST != 0))
    {
        et1 =   mod_sqr(et1);
        oper_cmp(et1,et2,op);
    }

    // left comp acc com right int acc ----------------------------------------

    if ((l_type == 3) && (r_type == 1) && (et1 % OFST == 0) && (et2 % OFST == 0))
    {
        et2 = mod_sqr(et2);

        id  = exec_id("aux_et2");
        et2 = OFST + id;

        add_instr("SETP %s\n", v_name[id]);
        acc_ok = 0;

        et1 =   mod_sqr(et1);
        oper_cmp(et1,et2,op);
    }

    // left comp acc com right float var --------------------------------------

    if ((l_type == 3) && (r_type == 2) && (et1 % OFST == 0) && (et2 % OFST != 0))
    {
        et1 =   mod_sqr(et1);
        oper_cmp(et1,et2,op);
    }

    // left comp acc com right float acc --------------------------------------

    if ((l_type == 3) && (r_type == 2) && (et1 % OFST == 0) && (et2 % OFST == 0))
    {
        et2 = mod_sqr(et2);

        id  = exec_id("aux_et2");
        et2 = 2*OFST + id;

        add_instr("SETP %s\n", v_name[id]);
        acc_ok = 0;

        et1 =   mod_sqr(et1);
        oper_cmp(et1,et2,op);
    }

    // left comp acc com right comp const -------------------------------------

    if ((l_type == 3) && (r_type == 5) && (et1 % OFST == 0))
    {
        et1 =   mod_sqr(et1);
        et2 =   mod_sqr(et2);
        oper_cmp(et1,et2,op);
    }

    // left comp acc com right comp var ---------------------------------------

    if ((l_type == 3) && (r_type == 3) && (et1 % OFST == 0) && (et2 % OFST != 0))
    {
        et1 =   mod_sqr(et1);
        et2 =   mod_sqr(et2);
        oper_cmp(et1,et2,op);
    }

    // left comp acc com right comp acc ---------------------------------------

    if ((l_type == 3) && (r_type == 3) && (et1 % OFST == 0) && (et2 % OFST == 0))
    {
        et2 = mod_sqr(et2);

        id  = exec_id("aux_et2");
        et2 = 2*OFST + id;

        add_instr("SETP %s\n", v_name[id]);
        acc_ok = 0;

        et1 =   mod_sqr(et1);
        oper_cmp(et1,et2,op);
    }

    return OFST;
}

// operacoes de comparacao devem ser feitas por aqui
int oper_cmp(int et1, int et2, int op)
{
    // teste com numeros complexos --------------------------------------------
    if ((get_type(et1) > 2) || (get_type(et2) > 2)) return oper_cmp_cmp(et1,et2,op);
    // fim do teste -----------------------------------------------------------

    switch (op)
    {
        case 0: operacoes(et1,et2, "LES", "CALL denorm\nLOAD float_aux3\nLES float_aux1", &fgen, 3); // <
                break;
        case 1: operacoes(et1,et2, "GRE", "CALL denorm\nLOAD float_aux3\nGRE float_aux1", &fgen, 3); // >
                break;
        case 2: operacoes(et1,et2, "LES", "CALL denorm\nLOAD float_aux3\nLES float_aux1", &fgen, 3); // >=
                add_instr("LINV\n");
                break;
        case 3: operacoes(et1,et2, "GRE", "CALL denorm\nLOAD float_aux3\nGRE float_aux1", &fgen, 3); // <=
                add_instr("LINV\n");
                break;
        case 4: operacoes(et1,et2, "EQU", "CALL denorm\nLOAD float_aux3\nEQU float_aux1", &fgen, 3); // ==
                break;
        case 5: operacoes(et1,et2, "EQU", "CALL denorm\nLOAD float_aux3\nEQU float_aux1", &fgen, 3); // !=
                add_instr("LINV\n");
                break;
    }

    return OFST;
}

// operacoes aritmeticas devem ser feitas por aqui
int oper_ari(int et1, int et2, int op)
{
    // teste com numeros complexos --------------------------------------------
    if ((get_type(et1) > 2) || (get_type(et2) > 2)) return oper_ari_cmp(et1,et2,op);
    // fim do teste -----------------------------------------------------------

    int r,ret;

    switch (op)
    {
            case 0 : ret = operacoes(et1,et2, "MLT", "CALL float_mult"             , &fmlt, 1); break; // *
            case 1 : ret = operacoes(et1,et2, "DIV", "CALL float_div"              , &fdiv, 1); break; // /
            case 2 : ret = operacoes(et1,et2, "ADD", "CALL denorm\nCALL float_add" , &fadd, 2); break; // +
            case 3 :   r = negacao  (et2);
                     ret = operacoes(et1,  r, "ADD", "CALL denorm\nCALL float_add" , &fadd, 2); break; // -
            default: ret = 0;
    }

    return ret;
}

// contas entre int/float e comp
void ari_int_cmp(int op, int et1, int et2_r, int et2_i)
{
    switch (op)
    {   // mult -----------------------------------------------------------
        case 0:  oper_ari(et1,et2_r,0); // mult com parte real
                 oper_ari(et1,et2_i,0); // mult com parte imag
                 break;
        // div ------------------------------------------------------------
        case 1:  oper_ari(et2_r ,et2_r ,0); // parte real ao quadrado
                 oper_ari(et2_i ,et2_i ,0); // parte imag ao quadrado
                 oper_ari(2*OFST,2*OFST,2); // soma os quadrados

                 add_instr("SET aux_cmp\n"); // salva o resultado
                 acc_ok = 0;

                 oper_ari(et1,et2_r,0);                                 // mult int com parte real
                 add_instr("PLD aux_cmp\n"); // pega o denominador
                 oper_ari(2*OFST,2*OFST,1);                             // faz a divisao

                 oper_ari(et1,et2_i,0);                                 // mult int com parte imag
                 add_instr("PLD aux_cmp\n"); // pega o denominador
                 oper_ari(2*OFST,2*OFST,1);                             // faz a divisao
                  negacao(2*OFST);
                 break;
        // soma e sub -----------------------------------------------------
        default:   oper_ari(et1,et2_r,op); // soma parte real
                 load_check(    et2_i, 0); // coloca o imag no acc
    }
}

// contas entre comp e int/float
void ari_cmp_int(int op, int et1_r, int et1_i, int et2)
{
    oper_ari(et1_r,et2,op);

    switch (op)
    {
        case  0:
        case  1:   oper_ari(et1_i,et2,op); break; // * e / tem q fazer com parte imag tb
        default: load_check(et1_i,0);             // + e - soh carrega parte imag
    }
}

// contas entre comp e comp
void ari_cmp_cmp(int op, int et1_r, int et1_i, int et2_r, int et2_i)
{
    switch (op)
    {   // mult -----------------------------------------------------------
        case 0:  oper_ari(et1_r ,et2_r ,0);
                 oper_ari(et1_i ,et2_i ,0);
                 oper_ari(2*OFST,2*OFST,3);

                 oper_ari(et1_r ,et2_i ,0);
                 oper_ari(et1_i ,et2_r ,0);
                 oper_ari(2*OFST,2*OFST,2);
                 break;
        // div ------------------------------------------------------------
        case 1:  oper_ari(et2_r ,et2_r ,0);
                 oper_ari(et2_i ,et2_i ,0);
                 oper_ari(2*OFST,2*OFST,2);

                 int id = exec_id("aux_cmp_r");
                 int et = 2*OFST + id;
                 add_instr("SET %s\n", v_name[id]);
                 acc_ok = 0;

                 oper_ari(et1_r ,et2_r ,0);
                 oper_ari(et1_i ,et2_i ,0);
                 oper_ari(2*OFST,2*OFST,2);
                 oper_ari(2*OFST,  et  ,1);

                 oper_ari(et1_i ,et2_r ,0);
                 oper_ari(et1_r ,et2_i ,0);
                 oper_ari(2*OFST,2*OFST,3);
                 oper_ari(2*OFST, et   ,1);
                // break;
        // soma e sub -----------------------------------------------------
        default: oper_ari(et1_r,et2_r,op);
                 oper_ari(et1_i,et2_i,op);
    }
}

// operacoes aritmeticas com numeros complexos
int oper_ari_cmp(int et1, int et2, int op)
{
    int l_type = get_type(et1);
    int r_type = get_type(et2);

    int et1_r, et1_i;
    int et2_r, et2_i;
    int id1_r, id1_i;
    int id2_r, id2_i;

    int id;

    // left int var com right comp const --------------------------------------

    if ((l_type == 1) && (r_type == 5) && (et1 % OFST != 0))
    {
        split_cmp_const(   et2,&et2_r,&et2_i);
            ari_int_cmp(op,et1, et2_r, et2_i);
    }

    // left int var com right comp var ----------------------------------------

    if ((l_type == 1) && (r_type == 3) && (et1 % OFST != 0) && (et2 % OFST != 0))
    {
        get_cmp_ets(   et2,&et2_r,&et2_i);
        ari_int_cmp(op,et1, et2_r, et2_i);
    }

    // left int var com right comp acc ----------------------------------------

    if ((l_type == 1) && (r_type == 3) && (et1 % OFST != 0) && (et2 % OFST == 0))
    {
        id2_r = exec_id("aux_cmp_r");
        id2_i = exec_id("aux_cmp_i");
        et2_r = 2*OFST + id2_r;
        et2_i = 2*OFST + id2_i;

        add_instr("SETP %s\n", v_name[id2_i]);
        add_instr("SET  %s\n", v_name[id2_r]);
        acc_ok = 0;

        ari_int_cmp(op,et1,et2_r,et2_i);
    }

    // left int acc com right comp const ----------------------------------------

    if ((l_type == 1) && (r_type == 5) && (et1 % OFST == 0))
    {
        id  = exec_id("aux_int");
        et1 = OFST + id;

        add_instr("SET  %s\n", v_name[id]);
        acc_ok = 0;

        split_cmp_const(   et2,&et2_r,&et2_i);
            ari_int_cmp(op,et1, et2_r, et2_i);
    }

    // left int acc com right comp var ----------------------------------------

    if ((l_type == 1) && (r_type == 3) && (et1 % OFST == 0) && (et2 % OFST != 0))
    {
        id  = exec_id("aux_int");
        et1 = OFST + id;

        add_instr("SET  %s\n", v_name[id]);
        acc_ok = 0;

        get_cmp_ets(   et2,&et2_r,&et2_i);
        ari_int_cmp(op,et1, et2_r, et2_i);
    }

    // left int acc com right comp acc ----------------------------------------

    if ((l_type == 1) && (r_type == 3) && (et1 % OFST == 0) && (et2 % OFST == 0))
    {
        id    = exec_id("aux_int");
        et1   = OFST + id;

        id2_r = exec_id("aux_cmp_r");
        id2_i = exec_id("aux_cmp_i");
        et2_r = 2*OFST + id2_r;
        et2_i = 2*OFST + id2_i;

        add_instr("SETP %s\n", v_name[id2_i]);
        add_instr("SETP %s\n", v_name[id2_r]);
        add_instr("SET  %s\n", v_name[id   ]);
        acc_ok = 0;

        ari_int_cmp(op,et1,et2_r,et2_i);
    }

    // left float var com right comp const ------------------------------------

    if ((l_type == 2) && (r_type == 5) && (et1 % OFST != 0))
    {
        split_cmp_const(   et2,&et2_r,&et2_i);
            ari_int_cmp(op,et1, et2_r, et2_i);
    }

    // left float var com right comp var --------------------------------------

    if ((l_type == 2) && (r_type == 3) && (et1 % OFST != 0) && (et2 % OFST != 0))
    {
        get_cmp_ets(   et2,&et2_r,&et2_i);
        ari_int_cmp(op,et1, et2_r, et2_i);
    }

    // left float var com right comp acc --------------------------------------

    if ((l_type == 2) && (r_type == 3) && (et1 % OFST != 0) && (et2 % OFST == 0))
    {
        id2_r = exec_id("aux_cmp_r");
        id2_i = exec_id("aux_cmp_i");
        et2_r = 2*OFST + id2_r;
        et2_i = 2*OFST + id2_i;

        add_instr("SETP %s\n", v_name[id2_i]);
        add_instr("SET  %s\n", v_name[id2_r]);
        acc_ok = 0;

        ari_int_cmp(op,et1,et2_r,et2_i);
    }

    // left float acc com right comp const ------------------------------------

    if ((l_type == 2) && (r_type == 5) && (et1 % OFST == 0))
    {
        id  = exec_id("aux_flt");
        et1 = 2*OFST + id;

        add_instr("SET %s\n", v_name[id   ]);
        acc_ok = 0;

        split_cmp_const(   et2,&et2_r,&et2_i);
            ari_int_cmp(op,et1, et2_r, et2_i);
    }

    // left float acc com right comp var --------------------------------------

    if ((l_type == 2) && (r_type == 3) && (et1 % OFST == 0) && (et2 % OFST != 0))
    {
        id  = exec_id("aux_flt");
        et1 = 2*OFST + id;

        add_instr("SET  %s\n", v_name[id   ]);
        acc_ok = 0;

        get_cmp_ets(   et2,&et2_r,&et2_i);
        ari_int_cmp(op,et1, et2_r, et2_i);
    }

    // left float acc com right comp acc --------------------------------------

    if ((l_type == 2) && (r_type == 3) && (et1 % OFST == 0) && (et2 % OFST == 0))
    {
        id  = exec_id("aux_flt");
        et1 = 2*OFST + id;

        id2_r = exec_id("aux_cmp_r");
        id2_i = exec_id("aux_cmp_i");
        et2_r = 2*OFST + id2_r;
        et2_i = 2*OFST + id2_i;

        add_instr("SETP %s\n", v_name[id2_i]);
        add_instr("SETP %s\n", v_name[id2_r]);
        add_instr("SET  %s\n", v_name[id   ]);
        acc_ok = 0;

        ari_int_cmp(op,et1,et2_r,et2_i);
    }

    // left comp const com right int var --------------------------------------

    if ((l_type == 5) && (r_type == 1) && (et2 % OFST != 0))
    {
        split_cmp_const(   et1  ,&et1_r,&et1_i);
          ari_cmp_int  (op,et1_r, et1_i, et2  );
    }

    // left comp const com right int acc --------------------------------------

    if ((l_type == 5) && (r_type == 1) && (et2 % OFST == 0))
    {
        split_cmp_const(   et1  ,&et1_r,&et1_i);
          ari_cmp_int  (op,et1_r, et1_i, et2  );
    }

    // left comp const com right float var ------------------------------------

    if ((l_type == 5) && (r_type == 2) && (et2 % OFST != 0))
    {
        split_cmp_const(   et1  ,&et1_r,&et1_i);
          ari_cmp_int  (op,et1_r, et1_i, et2  );
    }

    // left comp const com right float acc ------------------------------------

    if ((l_type == 5) && (r_type == 2) && (et2 % OFST == 0))
    {
        split_cmp_const(   et1  ,&et1_r,&et1_i);
          ari_cmp_int  (op,et1_r, et1_i, et2  );
    }

    // left comp const com right comp const -----------------------------------

    if ((l_type == 5) && (r_type == 5))
    {
        split_cmp_const(et1,&et1_r,&et1_i);
        split_cmp_const(et2,&et2_r,&et2_i);

        ari_cmp_cmp(op,et1_r,et1_i,et2_r,et2_i);
    }

    // left comp const com right comp var -------------------------------------

    if ((l_type == 5) && (r_type == 3) && (et2 % OFST != 0))
    {
        split_cmp_const(et1,&et1_r,&et1_i);
            get_cmp_ets(et2,&et2_r,&et2_i);

        ari_cmp_cmp(op,et1_r,et1_i,et2_r,et2_i);
    }

    // left comp const com right comp acc -------------------------------------

    if ((l_type == 5) && (r_type == 3) && (et2 % OFST == 0))
    {
        split_cmp_const(et1,&et1_r,&et1_i);

        id2_r = exec_id("aux_cmp_r");
        id2_i = exec_id("aux_cmp_i");
        et2_r = 2*OFST + id2_r;
        et2_i = 2*OFST + id2_i;

        add_instr("SETP %s\n", v_name[id2_i]);
        add_instr("SET  %s\n", v_name[id2_r]);
        acc_ok = 0;

        ari_cmp_cmp(op,et1_r,et1_i,et2_r,et2_i);
    }

    // left comp var com right int var ----------------------------------------

    if ((l_type == 3) && (r_type == 1) && (et1 % OFST != 0) && (et2 % OFST != 0))
    {
        get_cmp_ets(   et1  ,&et1_r,&et1_i);
        ari_cmp_int(op,et1_r, et1_i, et2  );
    }

    // left comp var com right int acc ----------------------------------------

    if ((l_type == 3) && (r_type == 1) && (et1 % OFST != 0) && (et2 % OFST == 0))
    {
        get_cmp_ets(   et1  ,&et1_r,&et1_i);
        ari_cmp_int(op,et1_r, et1_i, et2  );
    }

    // left comp var com right float var --------------------------------------

    if ((l_type == 3) && (r_type == 2) && (et1 % OFST != 0) && (et2 % OFST != 0))
    {
        get_cmp_ets(   et1  ,&et1_r,&et1_i);
        ari_cmp_int(op,et1_r, et1_i, et2  );
    }

    // left comp var com right float acc --------------------------------------

    if ((l_type == 3) && (r_type == 2) && (et1 % OFST != 0) && (et2 % OFST == 0))
    {
        get_cmp_ets(   et1  ,&et1_r,&et1_i);
        ari_cmp_int(op,et1_r, et1_i, et2  );
    }

    // left comp var com right comp const -------------------------------------

    if ((l_type == 3) && (r_type == 5) && (et1 % OFST != 0))
    {
            get_cmp_ets(et1,&et1_r,&et1_i);
        split_cmp_const(et2,&et2_r,&et2_i);

        ari_cmp_cmp(op,et1_r,et1_i,et2_r,et2_i);
    }

    // left comp var com right comp var ---------------------------------------

    if ((l_type == 3) && (r_type == 3) && (et1 % OFST != 0) && (et2 % OFST != 0))
    {
        get_cmp_ets(et1,&et1_r,&et1_i);
        get_cmp_ets(et2,&et2_r,&et2_i);

        ari_cmp_cmp(op,et1_r,et1_i,et2_r,et2_i);
    }

    // left comp var com right comp acc ---------------------------------------

    if ((l_type == 3) && (r_type == 3) && (et1 % OFST != 0) && (et2 % OFST == 0))
    {
        get_cmp_ets(et1,&et1_r,&et1_i);

        id2_r = exec_id("aux_cmp_r");
        id2_i = exec_id("aux_cmp_i");
        et2_r = 2*OFST + id2_r;
        et2_i = 2*OFST + id2_i;

        add_instr("SETP %s\n", v_name[id2_i]);
        add_instr("SET  %s\n", v_name[id2_r]);
        acc_ok = 0;

        ari_cmp_cmp(op,et1_r,et1_i,et2_r,et2_i);
    }

    // left comp acc com right int var ----------------------------------------

    if ((l_type == 3) && (r_type == 1) && (et1 % OFST == 0) && (et2 % OFST != 0))
    {
        id1_r = exec_id("aux_cmp_r");
        id1_i = exec_id("aux_cmp_i");
        et1_r = 2*OFST + id1_r;
        et1_i = 2*OFST + id1_i;

        add_instr("SETP %s\n", v_name[id1_i]);
        add_instr("SET  %s\n", v_name[id1_r]);
        acc_ok = 0;

        ari_cmp_int(op,et1_r,et1_i,et2);
    }

    // left comp acc com right int acc ----------------------------------------

    if ((l_type == 3) && (r_type == 1) && (et1 % OFST == 0) && (et2 % OFST == 0))
    {
        id1_r = exec_id("aux_cmp_r");
        id1_i = exec_id("aux_cmp_i");
        et1_r = 2*OFST + id1_r;
        et1_i = 2*OFST + id1_i;

        add_instr("SETP %s\n", v_name[id1_i]);
        add_instr("SET  %s\n", v_name[id1_r]);
        acc_ok = 0;

        ari_cmp_int(op,et1_r,et1_i,et2);
    }

    // left comp acc com right float var --------------------------------------

    if ((l_type == 3) && (r_type == 2) && (et1 % OFST == 0) && (et2 % OFST != 0))
    {
        id1_r = exec_id("aux_cmp_r");
        id1_i = exec_id("aux_cmp_i");
        et1_r = 2*OFST + id1_r;
        et1_i = 2*OFST + id1_i;

        add_instr("SETP %s\n", v_name[id1_i]);
        add_instr("SET  %s\n", v_name[id1_r]);
        acc_ok = 0;

        ari_cmp_int(op,et1_r,et1_i,et2);
    }

    // left comp acc com right float acc --------------------------------------

    if ((l_type == 3) && (r_type == 2) && (et1 % OFST == 0) && (et2 % OFST == 0))
    {
        id1_r = exec_id("aux_cmp_r");
        id1_i = exec_id("aux_cmp_i");

        et1_r = 2*OFST + id1_r;
        et1_i = 2*OFST + id1_i;

        add_instr("SETP %s\n", v_name[id1_i]);
        add_instr("SET  %s\n", v_name[id1_r]);
        acc_ok = 0;

        ari_cmp_int(op,et1_r,et1_i,et2);
    }

    // left comp acc com right comp const -------------------------------------

    if ((l_type == 3) && (r_type == 5) && (et1 % OFST == 0))
    {
        id1_r = exec_id("aux_cmp_r");
        id1_i = exec_id("aux_cmp_i");
        et1_r = 2*OFST + id1_r;
        et1_i = 2*OFST + id1_i;

        add_instr("SETP %s\n", v_name[id1_i]);
        add_instr("SET  %s\n", v_name[id1_r]);
        acc_ok = 0;

        split_cmp_const(et2,&et2_r,&et2_i);

        ari_cmp_cmp(op,et1_r,et1_i,et2_r,et2_i);
    }

    // left comp acc com right comp var ---------------------------------------

    if ((l_type == 3) && (r_type == 3) && (et1 % OFST == 0) && (et2 % OFST != 0))
    {
        id1_r = exec_id("aux_cmp_r");
        id1_i = exec_id("aux_cmp_i");
        et1_r = 2*OFST + id1_r;
        et1_i = 2*OFST + id1_i;

        add_instr("SETP %s\n", v_name[id1_i]);
        add_instr("SET  %s\n", v_name[id1_r]);
        acc_ok = 0;

        get_cmp_ets(et2,&et2_r,&et2_i);
        ari_cmp_cmp(op,et1_r,et1_i,et2_r,et2_i);
    }

    // left comp acc com right comp acc ---------------------------------------

    if ((l_type == 3) && (r_type == 3) && (et1 % OFST == 0) && (et2 % OFST == 0))
    {
        id1_r = exec_id("aux_cmp_r_1");
        id1_i = exec_id("aux_cmp_i_1");
        id2_r = exec_id("aux_cmp_r_2");
        id2_i = exec_id("aux_cmp_i_2");

        et1_r = 2*OFST + id1_r;
        et1_i = 2*OFST + id1_i;
        et2_r = 2*OFST + id2_r;
        et2_i = 2*OFST + id2_i;

        add_instr("SETP %s\n", v_name[id2_i]);
        add_instr("SETP %s\n", v_name[id2_r]);
        add_instr("SETP %s\n", v_name[id1_i]);
        add_instr("SET  %s\n", v_name[id1_r]);
        acc_ok = 0;

        ari_cmp_cmp(op,et1_r,et1_i,et2_r,et2_i);
    }

    return OFST*3;
}
