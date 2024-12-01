#include "oper.h"
#include "variaveis.h"
#include "t2t.h"
#include "diretivas.h"
#include "data_use.h"
#include "data_assign.h"
#include "stdlib.h"
#include <string.h>

// gera instrucao pra negar a reducao pra exp
// eu nao vou implementar uma instrucao SUB
// nego o segundo arqumento e somo
int negacao(int et)
{
    // se for uma constante, soh coloca um sinal de - na frente
    if (v_isco[et % OFST])
        load_check(et,1); // o segundo parametro faz a necagao
    // se eh uma variavel, da LOAD normal e depois um NEG
    else
    {
        load_check(et,0);

        // se for float em ponto fixo, usa uma macro pra trocar o bit de sinal
        if ((prtype == 0) && (et >= 2*OFST))
        {
            if (using_macro == 0)
            {
                fprintf(f_asm, "\nPLD float_nbits");
                fprintf(f_asm, "             // negacao em ponto flutuante\n");
                fprintf(f_asm, "SHL 1\nSADD");
                fprintf(f_asm, "                        // fim da negacao\n\n");
            }
        }
        // tenho que fazer se for int em ponto flutuante aqui ainda
        else
        {
            if (using_macro == 0) fprintf(f_asm, "NEG\n"); // negacao em ponto fixo
        }
    }

    return get_type(et)*OFST; // et padrao para reducao exp
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
void oper_fl(int et1, int et2, char *iop, char *fop, int *op)
{
    // memoria e memoria ------------------------------------------------------

    if ((et1 % OFST != 0) && (et2 % OFST != 0))
    {
        // LOAD no segundo operador
        load_check(et2,0);
        // faz operacao com o endereco de memoria de et1
        if (using_macro == 0) fprintf(f_asm, "%s %s\n", iop, v_name[et1 % OFST]);
    }

    // acc e memoria ----------------------------------------------------------

    if ((et1 % OFST == 0) && (et2 % OFST != 0))
    {
        // LOAD no segundo operador jogando o primeiro pra pilha
        load_check(et2,0);
        // executa instrucao com a pilha
        if (using_macro == 0) fprintf(f_asm, "S%s\n", iop);
    }

    // memoria e acc ----------------------------------------------------------

    if ((et1 % OFST != 0) && (et2 % OFST == 0))
    {
        // nao precisa dar load, basta fazer a instrucao com et1 na memoria
        if (using_macro == 0) fprintf(f_asm, "%s %s\n", iop, v_name[et1 % OFST]);
    }

    // pilha e acc ------------------------------------------------------------

    if ((et1 % OFST == 0) && (et2 % OFST == 0))
    {
        // os dois operando ja estao no acc e pilha
        if (using_macro == 0) fprintf(f_asm, "S%s\n", iop);
    }
}

// implementa operacoes com proc em ponto fixo
void oper_fx(int et1, int et2, char *iop, char *fop, int *op)
{
    // int memoria e int memoria ----------------------------------------------

    if (imem(et1) && imem(et2))
    {
        // carrega et2 depois faz instrucao com memoria de et1
        load_check(et2,0);
        if (using_macro == 0) fprintf(f_asm, "%s %s\n", iop, v_name[et1 % OFST]);
    }

    // int memoria e float memoria --------------------------------------------

    if (imem(et1) && fmem(et2))
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

    // float memoria e int memoria --------------------------------------------

    if (fmem(et1) && imem(et2))
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

    // float memoria e float memoria ------------------------------------------

    if (fmem(et1) && fmem(et2))
    {
        // carrega as variaveis, ja em float, pro processador
        load_check(et1,0);
        load_check(et2,0);
        // executa macro correspondente
        if (using_macro == 0) fprintf(f_asm, "%s\n", fop);
        *op = 1;
    }

    // int acc e int memoria --------------------------------------------------

    if (iacc(et1) && imem(et2))
    {
        // joga et1 pra pilha e carrega et2
        load_check(et2,0);
        // executa instrucao sem operando (ja ta todo mundo dentro do proc)
        if (using_macro == 0) fprintf(f_asm, "S%s\n", iop);
    }

    // int acc e float memoria ------------------------------------------------

    if (iacc(et1) && fmem(et2))
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

    // float acc e int memoria ------------------------------------------------

    if (facc(et1) && imem(et2))
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

    // float acc e float memoria ----------------------------------------------

    if (facc(et1) && fmem(et2))
    {
        // xuxa et1 na pilha e carrega et2
        load_check(et2,0);
        // execta macro float
        if (using_macro == 0) fprintf(f_asm, "%s\n", fop);
        *op = 1;
    }

    // int memoria e int acc --------------------------------------------------

    if (imem(et1) && iacc(et2))
    {
        // executa instrucao com et1 no argumento, sem pilha
        if (using_macro == 0) fprintf(f_asm, "%s %s\n", iop, v_name[et1 % OFST]);
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

    // float memoria e int acc ------------------------------------------------

    if (fmem(et1) && iacc(et2))
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

    // float memoria e float acc ----------------------------------------------

    if (fmem(et1) && facc(et2))
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

    // int pilha e int acc ----------------------------------------------------

    if (iacc(et1) && iacc(et2))
    {
        // instrucoes com uso da pilha, ja na ordem certa
        if (using_macro == 0) fprintf(f_asm, "S%s\n", iop);
    }

    // int pilha e float acc --------------------------------------------------

    if (iacc(et1) && facc(et2))
    {
        // salva et2 em float_aux5 e pega a pilha
        // converte et1 pra float e empurra ele pra pilha, carregando o et2 de volta
        if (using_macro == 0) fprintf(f_asm, "SETP float_aux5\nCALL int2float\nPLD float_aux5\n%s\n", fop);
        *op = 1;
        i2f = 1;
    }

    // float pilha e int acc --------------------------------------------------

    if (facc(et1) && iacc(et2))
    {
        // converte et2 pra float
        if (using_macro == 0) fprintf(f_asm, "CALL int2float\n");
        // executa macro da operacao
        if (using_macro == 0) fprintf(f_asm, "%s\n", fop);
        *op = 1;
        i2f = 1;
    }

    // float pilha e float acc ------------------------------------------------

    if (facc(et1) && facc(et2))
    {
        // executa macro da operacao
        if (using_macro == 0) fprintf(f_asm, "%s\n", fop);
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
int operacoes(int et1, int et2, char *iop, char *fop, int *op)
{
    if (prtype == 1)
        oper_fl(et1,et2,iop,fop,op);
    else
        oper_fx(et1,et2,iop,fop,op);

    acc_ok = 1;

    return ((et1 >= 2*OFST) || (et2 >= 2*OFST)) ? 2*OFST : OFST; // retorna o tipo de dados em id extendido (pra exp)
}

// operacoes que resultam em inteiro devem ser feitas por aqui
// ex: deslocamento de bits, inversao de bits, etc
// fok diz se tal operacao foi implementada no proc em ponto flutuante
int int_oper(int et1, int et2, char *op, char *code, int fok)
{
    if ((prtype == 1) && (fok == 0))
        fprintf(stderr, "Erro na linha %d: processador em ponto flutuante não aceita %s ainda. Aguarde!\n", line_num+1, op);

    if ((get_type(et1) > 1) || (get_type(et2) > 1))
        fprintf(stderr, "Erro na linha %d: uso incorreto de %s. Tem que passar tipo int. Viajou?\n", line_num+1, op);

    // tem que botar et2 = 0, caso a funcao seja de um unico operador
    // ex: inversao de bit
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

        if (using_macro == 0) fprintf(f_asm, "SETP %s\n", v_name[id]);
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

        if (using_macro == 0) fprintf(f_asm, "SETP %s\n", v_name[id]);
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

        if (using_macro == 0) fprintf(f_asm, "SETP %s\n", v_name[id]);
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

        if (using_macro == 0) fprintf(f_asm, "SETP %s\n", v_name[id]);
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

        if (using_macro == 0) fprintf(f_asm, "SETP %s\n", v_name[id]);
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

        if (using_macro == 0) fprintf(f_asm, "SETP %s\n", v_name[id]);
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

        if (using_macro == 0) fprintf(f_asm, "SETP %s\n", v_name[id]);
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

        if (using_macro == 0) fprintf(f_asm, "SETP %s\n", v_name[id]);
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

        if (using_macro == 0) fprintf(f_asm, "SETP %s\n", v_name[id]);
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

// operacoes aritmeticas devem ser feitas por aqui
int oper_ari(int et1, int et2, int op)
{
    // teste com numeros complexos --------------------------------------------
    if ((get_type(et1) > 2) || (get_type(et2) > 2)) return oper_ari_cmp(et1,et2,op);
    // fim do teste -----------------------------------------------------------

    int r,ret;

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

                 if (using_macro == 0) fprintf(f_asm, "SET aux_cmp\n"); // salva o resultado
                 acc_ok = 0;

                 oper_ari(et1,et2_r,0);                                 // mult int com parte real
                 if (using_macro == 0) fprintf(f_asm, "PLD aux_cmp\n"); // pega o denominador
                 oper_ari(2*OFST,2*OFST,1);                             // faz a divisao

                 oper_ari(et1,et2_i,0);                                 // mult int com parte imag
                 if (using_macro == 0) fprintf(f_asm, "PLD aux_cmp\n"); // pega o denominador
                 oper_ari(2*OFST,2*OFST,1);                             // faz a divisao
                  negacao(2*OFST);
                 break;
        // soma e sub -----------------------------------------------------
        default:   oper_ari(et1,et2_r,op); // soma parte real
                 load_check(    et2_i, 0); // coloca o imag no acc
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
                 if (using_macro == 0) fprintf(f_asm, "SET %s\n", v_name[id]);
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

        if (using_macro == 0) fprintf(f_asm, "SETP %s\n", v_name[id2_i]);
        if (using_macro == 0) fprintf(f_asm, "SET  %s\n", v_name[id2_r]);
        acc_ok = 0;

        ari_int_cmp(op,et1,et2_r,et2_i);
    }

    // left int acc com right comp const ----------------------------------------

    if ((l_type == 1) && (r_type == 5) && (et1 % OFST == 0))
    {
        id  = exec_id("aux_int");
        et1 = OFST + id;

        if (using_macro == 0) fprintf(f_asm, "SET  %s\n", v_name[id]);
        acc_ok = 0;

        split_cmp_const(   et2,&et2_r,&et2_i);
            ari_int_cmp(op,et1, et2_r, et2_i);
    }

    // left int acc com right comp var ----------------------------------------

    if ((l_type == 1) && (r_type == 3) && (et1 % OFST == 0) && (et2 % OFST != 0))
    {
        id  = exec_id("aux_int");
        et1 = OFST + id;

        if (using_macro == 0) fprintf(f_asm, "SET  %s\n", v_name[id]);
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

        if (using_macro == 0) fprintf(f_asm, "SETP %s\n", v_name[id2_i]);
        if (using_macro == 0) fprintf(f_asm, "SETP %s\n", v_name[id2_r]);
        if (using_macro == 0) fprintf(f_asm, "SET  %s\n", v_name[id   ]);
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

        if (using_macro == 0) fprintf(f_asm, "SETP %s\n", v_name[id2_i]);
        if (using_macro == 0) fprintf(f_asm, "SET  %s\n", v_name[id2_r]);
        acc_ok = 0;

        ari_int_cmp(op,et1,et2_r,et2_i);
    }

    // left float acc com right comp const ------------------------------------

    if ((l_type == 2) && (r_type == 5) && (et1 % OFST == 0))
    {
        id  = exec_id("aux_flt");
        et1 = 2*OFST + id;

        if (using_macro == 0) fprintf(f_asm, "SET %s\n", v_name[id   ]);
        acc_ok = 0;

        split_cmp_const(   et2,&et2_r,&et2_i);
            ari_int_cmp(op,et1, et2_r, et2_i);
    }

    // left float acc com right comp var --------------------------------------

    if ((l_type == 2) && (r_type == 3) && (et1 % OFST == 0) && (et2 % OFST != 0))
    {
        id  = exec_id("aux_flt");
        et1 = 2*OFST + id;

        if (using_macro == 0) fprintf(f_asm, "SET  %s\n", v_name[id   ]);
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

        if (using_macro == 0) fprintf(f_asm, "SETP %s\n", v_name[id2_i]);
        if (using_macro == 0) fprintf(f_asm, "SETP %s\n", v_name[id2_r]);
        if (using_macro == 0) fprintf(f_asm, "SET  %s\n", v_name[id   ]);
        acc_ok = 0;

        ari_int_cmp(op,et1,et2_r,et2_i);
    }

    // left comp const com right int var --------------------------------------

    if ((l_type == 5) && (r_type == 1) && (et2 % OFST != 0))
    {
        split_cmp_const(et1,&et1_r,&et1_i);

        oper_ari(et1_r,et2,op);
        oper_ari(et1_i,et2,op);
    }

    // left comp const com right int acc --------------------------------------

    if ((l_type == 5) && (r_type == 1) && (et2 % OFST == 0))
    {
        split_cmp_const(et1,&et1_r,&et1_i);

        oper_ari(et1_r,et2,op);
        oper_ari(et1_i,et2,op);
    }

    // left comp const com right float var ------------------------------------

    if ((l_type == 5) && (r_type == 2) && (et2 % OFST != 0))
    {
        split_cmp_const(et1,&et1_r,&et1_i);

        oper_ari(et1_r,et2,op);
        oper_ari(et1_i,et2,op);
    }

    // left comp const com right float acc ------------------------------------

    if ((l_type == 5) && (r_type == 2) && (et2 % OFST == 0))
    {
        split_cmp_const(et1,&et1_r,&et1_i);

        oper_ari(et1_r,et2,op);
        oper_ari(et1_i,et2,op);
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

        if (using_macro == 0) fprintf(f_asm, "SETP %s\n", v_name[id2_i]);
        if (using_macro == 0) fprintf(f_asm, "SET  %s\n", v_name[id2_r]);
        acc_ok = 0;

        ari_cmp_cmp(op,et1_r,et1_i,et2_r,et2_i);
    }

    // left comp var com right int var ----------------------------------------

    if ((l_type == 3) && (r_type == 1) && (et1 % OFST != 0) && (et2 % OFST != 0))
    {
        get_cmp_ets(et1,&et1_r,&et1_i);

        oper_ari(et1_r,et2,op);
        oper_ari(et1_i,et2,op);
    }

    // left comp var com right int acc ----------------------------------------

    if ((l_type == 3) && (r_type == 1) && (et1 % OFST != 0) && (et2 % OFST == 0))
    {
        get_cmp_ets(et1,&et1_r,&et1_i);

        oper_ari(et1_r,et2,op);
        oper_ari(et1_i,et2,op);
    }

    // left comp var com right float var --------------------------------------

    if ((l_type == 3) && (r_type == 2) && (et1 % OFST != 0) && (et2 % OFST != 0))
    {
        get_cmp_ets(et1,&et1_r,&et1_i);

        oper_ari(et1_r,et2,op);
        oper_ari(et1_i,et2,op);
    }

    // left comp var com right float acc --------------------------------------

    if ((l_type == 3) && (r_type == 2) && (et1 % OFST != 0) && (et2 % OFST == 0))
    {
        get_cmp_ets(et1,&et1_r,&et1_i);

        oper_ari(et1_r,et2,op);
        oper_ari(et1_i,et2,op);
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

        if (using_macro == 0) fprintf(f_asm, "SETP %s\n", v_name[id2_i]);
        if (using_macro == 0) fprintf(f_asm, "SET  %s\n", v_name[id2_r]);
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

        if (using_macro == 0) fprintf(f_asm, "SETP %s\n", v_name[id1_i]);
        if (using_macro == 0) fprintf(f_asm, "SET  %s\n", v_name[id1_r]);
        acc_ok = 0;

        oper_ari(et1_r,et2,op);
        oper_ari(et1_i,et2,op);
    }

    // left comp acc com right int acc ----------------------------------------

    if ((l_type == 3) && (r_type == 1) && (et1 % OFST == 0) && (et2 % OFST == 0))
    {
        id1_r = exec_id("aux_cmp_r");
        id1_i = exec_id("aux_cmp_i");
        et1_r = 2*OFST + id1_r;
        et1_i = 2*OFST + id1_i;

        if (using_macro == 0) fprintf(f_asm, "SETP %s\n", v_name[id1_i]);
        if (using_macro == 0) fprintf(f_asm, "SET  %s\n", v_name[id1_r]);
        acc_ok = 0;

        oper_ari(et1_r,et2,op);
        oper_ari(et1_i,et2,op);
    }

    // left comp acc com right float var --------------------------------------

    if ((l_type == 3) && (r_type == 2) && (et1 % OFST == 0) && (et2 % OFST != 0))
    {
        id1_r = exec_id("aux_cmp_r");
        id1_i = exec_id("aux_cmp_i");
        et1_r = 2*OFST + id1_r;
        et1_i = 2*OFST + id1_i;

        if (using_macro == 0) fprintf(f_asm, "SETP %s\n", v_name[id1_i]);
        if (using_macro == 0) fprintf(f_asm, "SET  %s\n", v_name[id1_r]);
        acc_ok = 0;

        oper_ari(et1_r,et2,op);
        oper_ari(et1_i,et2,op);
    }

    // left comp acc com right float acc --------------------------------------

    if ((l_type == 3) && (r_type == 2) && (et1 % OFST == 0) && (et2 % OFST == 0))
    {
        id1_r = exec_id("aux_cmp_r");
        id1_i = exec_id("aux_cmp_i");

        et1_r = 2*OFST + id1_r;
        et1_i = 2*OFST + id1_i;

        if (using_macro == 0) fprintf(f_asm, "SETP %s\n", v_name[id1_i]);
        if (using_macro == 0) fprintf(f_asm, "SET  %s\n", v_name[id1_r]);
        acc_ok = 0;

        oper_ari(et1_r,et2,op);
        oper_ari(et1_i,et2,op);
    }

    // left comp acc com right comp const -------------------------------------

    if ((l_type == 3) && (r_type == 5) && (et1 % OFST == 0))
    {
        id1_r = exec_id("aux_cmp_r");
        id1_i = exec_id("aux_cmp_i");
        et1_r = 2*OFST + id1_r;
        et1_i = 2*OFST + id1_i;

        if (using_macro == 0) fprintf(f_asm, "SETP %s\n", v_name[id1_i]);
        if (using_macro == 0) fprintf(f_asm, "SET  %s\n", v_name[id1_r]);
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

        if (using_macro == 0) fprintf(f_asm, "SETP %s\n", v_name[id1_i]);
        if (using_macro == 0) fprintf(f_asm, "SET  %s\n", v_name[id1_r]);
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

        if (using_macro == 0) fprintf(f_asm, "SETP %s\n", v_name[id2_i]);
        if (using_macro == 0) fprintf(f_asm, "SETP %s\n", v_name[id2_r]);
        if (using_macro == 0) fprintf(f_asm, "SETP %s\n", v_name[id1_i]);
        if (using_macro == 0) fprintf(f_asm, "SET  %s\n", v_name[id1_r]);
        acc_ok = 0;

        ari_cmp_cmp(op,et1_r,et1_i,et2_r,et2_i);
    }

    return OFST*3;
}
