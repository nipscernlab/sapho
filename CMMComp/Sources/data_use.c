// ----------------------------------------------------------------------------
// rotinas para reducao exp ---------------------------------------------------
// ----------------------------------------------------------------------------

// includes globais
#include <stdio.h>

// includes locais
#include "..\Headers\t2t.h"
#include "..\Headers\oper.h"
#include "..\Headers\global.h"
#include "..\Headers\funcoes.h"
#include "..\Headers\data_use.h"
#include "..\Headers\variaveis.h"
#include "..\Headers\data_assign.h"
#include "..\Headers\array_index.h"

// reducao de constantes para exp
// nao da load, soh atualiza estados das variaveis
int num2exp(int id, int dtype)
{
    v_used[id] = 1;
    v_isco[id] = 1;
    v_isar[id] = 0;
    v_type[id] = dtype;

    return dtype*OFST+id;
}

// reducao de ID pra exp
// ainda nao da load, soh checa e atualiza estados da variavel
int id2exp(int id)
{
    // Testa se a variavel ja foi declarada
    if (v_type[id] == 0)
        fprintf (stderr, "Erro na linha %d: mané, declara a variável '%s' direito!\n", line_num+1, rem_fname(v_name[id], fname));

    // Testa se a variavel ja recebeu um valor
    if (v_asgn[id] == 0)
        fprintf (stdout, "Atenção na linha %d: como você quer usar '%s' se você nem deu um valor ainda?\n", line_num+1, rem_fname(v_name[id], fname));

    // Se for um array, esqueceram o indice
    if (v_isar[id] > 0)
        fprintf (stderr, "Erro na linha %d: cadê o índice de array da variável '%s'?\n", line_num+1, rem_fname(v_name[id], fname));

    v_used[id] = 1;

    return v_type[id]*OFST+id;
}

// reducao de ++ pra exp
int pplus2exp(int id)
{
    if (v_type[id] > 2)
        fprintf (stderr, "Erro na linha %d: o que você bebeu pra querer incrementar um número complexo?\n", line_num+1);

    // equivalente a pegar o x na expressao (x+1)
    int et = id2exp(id);

    // agora transforma o 1 em um exp
    // primeiro faz o lexer do 1
    if (find_var("1") == -1) add_var("1");
    int lval = find_var("1");
    // pega se deve vir de INUM ou FNUM
    int type = get_type(et);
    // depois o parser
    int et1 = num2exp(lval,type);
    // depois faz operacao de soma
    int ret = oper_soma(et,et1);
    // por ultimo, atribui de volta pra id
    ass_set(id, ret);

    acc_ok = 1; //nao pode liberar o acc, pois eh um exp

    return ret;
}

// reducao de ++ pra exp em array 1D
int pplus1d2exp(int id, int ete)
{
    if (v_type[id] > 2)
        fprintf (stderr, "Erro na linha %d: o que você bebeu pra querer incrementar um número complexo?\n", line_num+1);

    // equivalente a pegar o x na expressao (x+1)
    int et = arr_1d2exp(id,ete,0);
    // agora transforma o 1 em um exp
    // primeiro faz o lexer do 1
    if (find_var("1") == -1) add_var("1");
    int lval = find_var("1");
    // pega se deve vir de INUM ou FNUM
    int type = get_type(et);
    // depois o parser
    int et1 = num2exp(lval,type);
    // depois faz operacao de soma
    int ret = oper_soma(et,et1);
    // faz o load no indice do array novamente
    arr_1d_index(id, ete);
    // por ultimo, atribui de volta pra id
    ass_array(id, ret, 0);

    acc_ok = 1; //nao pode liberar o acc, pois eh um exp

    return ret;
}

// reducao de ++ pra exp em array 2D
int pplus2d2exp(int id, int et1, int et2)
{
    if (v_type[id] > 2)
        fprintf (stderr, "Erro na linha %d: o que você bebeu pra querer incrementar um número complexo?\n", line_num+1);

    // equivalente a pegar o x na expressao (x+1)
    int et = arr_2d2exp(id,et1,et2);
    // agora transforma o 1 em um exp
    // primeiro faz o lexer do 1
    if (find_var("1") == -1) add_var("1");
    int lval = find_var("1");
    // pega se deve vir de INUM ou FNUM
    int type = get_type(et);
    // depois o parser
    int etx = num2exp(lval,type);
    // depois faz operacao de soma
    int ret = oper_soma(et,etx);
    // faz o load no indice do array novamente
    arr_2d_index(id, et1, et2);
    // por ultimo, atribui de volta pra id
    ass_array(id, ret, 0);

    acc_ok = 1; //nao pode liberar o acc, pois eh um exp

    return ret;
}
