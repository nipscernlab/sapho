#include "..\Headers\funcoes.h"
#include "..\Headers\variaveis.h"
#include "..\Headers\t2t.h"
#include "..\Headers\diretivas.h"
#include "..\Headers\data_declar.h"
#include "..\Headers\data_assign.h"
#include "..\Headers\data_use.h"
#include "..\Headers\labels.h"
#include <string.h>

// ----------------------------------------------------------------------------
// declaracao -----------------------------------------------------------------
// ----------------------------------------------------------------------------

// declara uma funcao
void declar_fun(int id1, int id2) //id1 -> tipo, id2 -> indice para o nome
{
    // se ainda nao for a funcao main, entao tem que dar um call pra ela antes
    // pois CALL main deve ser a primeira instrucao do processador depois do reset
    if ((mainok == 0) && (strcmp(v_name[id2], "main") != 0))
    {
        if (using_macro == 0) fprintf(f_asm, "CALL main\n@fim JMP fim\n");

        mainok = 2; // funcao main foi chamada no inicio
    }

    if (using_macro == 0) fprintf(f_asm, "@%s ", v_name[id2]);

    strcpy(fname, v_name[id2]); // seta a variavel de estado fname para o nome da funcao a ser analisada
    v_type[id2] = id1+6       ; // v_type vai ser funcao (void, int, float, comp) (6, 7, 8, 9)
    fun_id1     = id2         ; // seta a variavel de estado fun_id1 para o id do nome da funcao
    ret_ok      =     0       ; // seta a variavel de estado ret_ok para zero (vai comecar o parser da funcao)
}

// pega o primeiro parametro
void declar_firstpar(int id)
{
    // testes com numeros complexos -------------------------------------------
    if (v_type[id] > 2)
    {
        // primeiro pega o img da pilha
        int idi = get_img_id(id);
        if (using_macro == 0) fprintf(f_asm, "SETP %s\n", v_name[idi]);
    }
    // fim do teste -----------------------------------------------------------

    // o primeiro parametro da funcao eh com SET (pq eh o ultimo a ser chamado)
    // os proximos (se houver) sao com SETP em outra funcao
    if (using_macro == 0) fprintf(f_asm, "SET %s\n", v_name[id]);
}

// pega a partir do segundo parametro
int declar_par(int type, int id)
{
    declar_var(id); // nao pode passar array como parametro de funcao

    // parametro deve ser visto como variavel ja assigned
    v_asgn[id] = 1;

    // armazena informacao sobre o tipo de dado de todos os parametro em um unico numero
    v_fpar[fun_id1] = v_fpar[fun_id1]*10 + type;

    return id;
}

// vai dando SETP nos parametros, a medida que for achando eles
void set_par(int id)
{
    // testes com numeros complexos -------------------------------------------
    if (v_type[id] > 2)
    {
        int idi = get_img_id(id);
        if (using_macro == 0) fprintf(f_asm, "SETP %s\n", v_name[idi]);
    }
    // fim do teste -----------------------------------------------------------

        if (using_macro == 0) fprintf(f_asm, "SETP %s\n" , v_name[id]);
}

// quando acha a palavra chave return
void declar_ret(int et, int ret)
{
    // checa se eh funcao mesmo, ou void por engano
    if (v_type[fun_id1] == 6)
        fprintf (stderr, "Erro na linha %d: valor de retorno em função void? viajou!\n", line_num+1);

    // testa se esta dentro de um if/else
    if ((get_if() > 0) && (v_type[fun_id1] != 6))
        fprintf(stdout, "Cuidado na linha %d: usar return dentro de if/else pode dar pau, caso você esqueça em algum lugar!\n", line_num+1);

    // ------------------------------------------------------------------------
    // checa todas as combinacoes ---------------------------------------------
    // ------------------------------------------------------------------------

    // testes com numeros complexos -------------------------------------------
    if ((v_type[fun_id1] > 8) || (get_type(et) > 2))
    {
        // se retorno eh complexo, executa isso e sai
        declar_ret_cmp(et); return;
    }
    // fim do teste -----------------------------------------------------------

    int left_type = v_type[fun_id1];
    int righ_type = get_type(et);

    load_check(et,0);

    // funcao eh int mas o return eh float ------------------------------------

    if ((left_type == 7) && (righ_type == 2))
    {
        if (prtype == 0)
        {
            fprintf(stdout, "Atenção na linha %d: vai mesmo retornar float para int na função %s? Vou meter um monte de instruções assembly pra isso?\n", line_num+1, v_name[fun_id1]);

            if (using_macro == 0) fprintf(f_asm, "CALL float2int\n");
            f2i = 1;
        }
        else
            fprintf(stdout, "Atenção na linha %d: convertendo float para int no retorno da função %s.\n", line_num+1, v_name[fun_id1]);
    }

    // funcao eh float mas o return eh int -------------------------------------

    if ((left_type == 8) && (righ_type == 1))
    {
        fprintf(stdout, "Atenção na linha %d: convertendo int para float no retorno da função %s.\n", line_num+1, v_name[fun_id1]);

        if (prtype == 0)
        {
            if (using_macro == 0) fprintf(f_asm, "CALL int2float\n");
            i2f = 1;
        }
    }

    // ------------------------------------------------------------------------
    // finaliza ---------------------------------------------------------------
    // ------------------------------------------------------------------------

    if (ret == 0) return;

    if (using_macro == 0) fprintf(f_asm, "RETURN\n");

    acc_ok = 0; // apesar de ter exp no acc, tem q liberar para comecar outra funcao
    ret_ok = 1; // apareceu a palavra chave return na funcao certinho
}

// igual o var_set_comp, mas sem array
void declar_ret_cmp(int et)
{
    int et_r, et_i;

    int t_left  =   v_type[fun_id1]-6;
    int t_right = get_type(et);

    int mem = et % OFST;

    // left int e right comp const --------------------------------------------

    if ((t_left == 1) && (t_right == 5))
    {
        fprintf (stdout, "Atenção na linha %d: nessa conversão, eu vou arredondar a parte real hein!\n", line_num+1);

        split_cmp_const(et,&et_r,&et_i);
             declar_ret(et_r,1);
    }

    // left int e right comp var ----------------------------------------------

    if ((t_left == 1) && (t_right == 3) && (mem != 0))
    {
        fprintf (stdout, "Atenção na linha %d: nessa conversão, eu vou arredondar a parte real hein!\n", line_num+1);

        get_cmp_ets(et,&et_r,&et_i); // pega os IDs estendidos do right na memoria
         declar_ret(et_r,1);
    }

    // left int e right comp acc ----------------------------------------------

    if ((t_left == 1) && (t_right == 3) && (mem == 0))
    {
        fprintf (stdout, "Atenção na linha %d: nessa conversão, eu vou arredondar a parte real hein!\n", line_num+1);

        if (using_macro == 0) fprintf(f_asm, "SETP aux_img\n");
        declar_ret(2*OFST,1);
    }

    // left float e right comp const -----------------------------------------

    if ((t_left == 2) && (t_right == 5))
    {
        fprintf (stdout, "Atenção na linha %d: vou pegar só a parte real!\n", line_num+1);

        split_cmp_const(et,&et_r,&et_i);
             declar_ret(et_r,1);
    }

    // left float e right comp var --------------------------------------------

    if ((t_left == 2) && (t_right == 3) && (mem != 0))
    {
        fprintf (stdout, "Atenção na linha %d: vou pegar só a parte real!\n", line_num+1);

        get_cmp_ets(et,&et_r,&et_i); // pega os IDs estendidos do right na memoria
         declar_ret(et_r,1);
    }

    // left float e right comp acc ----------------------------------------------

    if ((t_left == 2) && (t_right == 3) && (mem == 0))
    {
        fprintf (stdout, "Atenção na linha %d: vou pegar só a parte real!\n", line_num+1);

        if (using_macro == 0) fprintf(f_asm, "SETP aux_img\n");
        declar_ret(2*OFST,1);
    }

    // left comp e right int var ----------------------------------------------

    if ((t_left == 3) && (t_right == 1) && (mem != 0))
    {
        fprintf (stdout, "Atenção na linha %d: pra converter de int pra comp, eu vou jogar 0 na parte imaginária.\n", line_num+1);

        v_type[fun_id1] = 8; declar_ret(et  ,0); v_type[fun_id1] = 9;

        // gambiarra pra dar LOAD no zero da parte imaginaria
        et_i = 2*OFST + exec_num("0.0");
        v_type[et_i%OFST] = 2;
        v_isco[et_i%OFST] = 1;
        v_used[et_i%OFST] = 1;

        v_type[fun_id1] = 8; declar_ret(et_i,1); v_type[fun_id1] = 9;
    }

    // left comp e right int acc ----------------------------------------------

    if ((t_left == 3) && (t_right == 1) && (mem == 0))
    {
        fprintf (stdout, "Atenção na linha %d: pra converter de int pra comp, eu vou jogar 0 na parte imaginária.\n", line_num+1);

        v_type[fun_id1] = 8; declar_ret(et  ,0); v_type[fun_id1] = 9;

        // gambiarra pra dar LOAD no zero da parte imaginaria
        et_i = 2*OFST + exec_num("0.0");
        v_type[et_i%OFST] = 2;
        v_isco[et_i%OFST] = 1;
        v_used[et_i%OFST] = 1;

        v_type[fun_id1] = 8; declar_ret(et_i,1); v_type[fun_id1] = 9;
    }

    // left comp e right float var --------------------------------------------

    if ((t_left == 3) && (t_right == 2) && (mem != 0))
    {
        fprintf (stdout, "Atenção na linha %d: pra converter de float pra comp, eu vou jogar 0 na parte imaginária.\n", line_num+1);

        v_type[fun_id1] = 8; declar_ret(et  ,0); v_type[fun_id1] = 9;

        // gambiarra pra dar LOAD no zero da parte imaginaria
        et_i = 2*OFST + exec_num("0.0");
        v_type[et_i%OFST] = 2;
        v_isco[et_i%OFST] = 1;
        v_used[et_i%OFST] = 1;

        v_type[fun_id1] = 8; declar_ret(et_i,1); v_type[fun_id1] = 9;
    }

    // left comp e right float acc --------------------------------------------

    if ((t_left == 3) && (t_right == 2) && (mem == 0))
    {
        fprintf (stdout, "Atenção na linha %d: pra converter de float pra comp, eu vou jogar 0 na parte imaginária.\n", line_num+1);

        v_type[fun_id1] = 8; declar_ret(et  ,0); v_type[fun_id1] = 9;

        // gambiarra pra dar LOAD no zero da parte imaginaria
        et_i = 2*OFST + exec_num("0.0");
        v_type[et_i%OFST] = 2;
        v_isco[et_i%OFST] = 1;
        v_used[et_i%OFST] = 1;

        v_type[fun_id1] = 8; declar_ret(et_i,1); v_type[fun_id1] = 9;
    }

    // left comp e right comp const -------------------------------------------

    if ((t_left == 3) && (t_right == 5))
    {
        split_cmp_const(et,&et_r,&et_i);

        v_type[fun_id1] = 8; declar_ret(et_r,0); v_type[fun_id1] = 9;
        v_type[fun_id1] = 8; declar_ret(et_i,1); v_type[fun_id1] = 9;
    }

    // left comp e right comp var ---------------------------------------------

    if ((t_left == 3) && (t_right == 3) && (mem != 0))
    {
        get_cmp_ets(et,&et_r,&et_i); // pega os IDs estendidos do right na memoria

        v_type[fun_id1] = 8; declar_ret(et_r,0); v_type[fun_id1] = 9;
        v_type[fun_id1] = 8; declar_ret(et_i,1); v_type[fun_id1] = 9;
    }

    // left comp e right comp acc ---------------------------------------------

    if ((t_left == 3) && (t_right == 3) && (mem == 0))
    {
        v_type[fun_id1] = 8; declar_ret(2*OFST,1); v_type[fun_id1] = 9;
    }
}

// fim do parser da declaracao de uma funcao
void func_ret(int id) // id -> id da funcao atual
{
    // checa se a funcao teve a instrucao return x;
    if ((v_type[id] != 6) && (ret_ok == 0))
        fprintf (stderr, "Erro na função %s: cadê o retorno pra essa função?\n", v_name[id]);

    if (strcmp(v_name[id], "main") == 0) // se eh funcao main ...
    {
        if (mainok == 0) // soh tem a funcao main
        {
             if (using_macro == 0) fprintf(f_asm, "@fim JMP fim\n");
        }
        else if (using_macro == 0) fprintf(f_asm, "RETURN\n"); // tem subrotinas

        v_used[id] = 1; // funcao main foi usada
        mainok     = 1; // funcao main foi parseada
    }
    else if (v_type[id] == 6) {if (using_macro == 0) fprintf(f_asm, "RETURN\n");} // se eh tipo void, ainda precisa gerar um RETURN

    // variavel de ambiente fname fica vazia (saiu de uma funcao)
    strcpy(fname, "");
}

// retorno sem exp (return;)
void void_ret()
{
    // checa se eh void mesmo, ou funcao por engano
    if (v_type[fun_id1] != 6)
        fprintf (stderr, "Erro na linha %d: cadê o valor de retorno da função?\n", line_num+1);


    if ((strcmp(fname, "main") == 0) && (mainok == 0))       // se eh funcao main e soh tem ela ...
    {
         if (using_macro == 0) fprintf(f_asm, "JMP fim\n");  // ai nao usa RETURN, pula pro fim
    }
    else if (using_macro == 0) fprintf(f_asm, "RETURN\n");   // se nao, usa return padrao
}

// ----------------------------------------------------------------------------
// utilizacao -----------------------------------------------------------------
// ----------------------------------------------------------------------------

// da LOAD no primeiro parametro (se houver)
// get_type da o tipo de parametro (0, 1, 2) (void, int, float)
// p_test consegue guardar a posicao e tipo de todos os parametros na chamada da funcao
void par_exp(int et)
{
    p_test = 0;

    if (get_type(et) < 3)
    {
        p_test = p_test*10 + get_type(et);
        par_check(et);
    }
    // testes com numeros complexos -------------------------------------------
    else par_exp_cmp(et);
    // fim do teste -----------------------------------------------------------
}

// par_exp pra numer complexos
void par_exp_cmp(int et)
{
    int eti;

    p_test = p_test*10 + get_type(et);

    // acc --------------------------------------------------------------------

    if (et % OFST == 0)
    {
        par_check(et );

        eti = 3*OFST;
        par_check(eti);
    }

    // memoria ----------------------------------------------------------------

    if ((et > 3*OFST) && (et < 5*OFST))
    {
        par_check(et);

        int id = get_img_id(et % OFST);
        eti    = 3*OFST + id;
        par_check(eti);
    }

    // constante complexa -----------------------------------------------------

    if (et > 5*OFST)
    {
        int et_r, et_i;
        split_cmp_const(et,&et_r,&et_i);

        par_check(et_r);
        par_check(et_i);
    }
}

// da LOAD nos proximos parametros
void par_listexp(int et)
{
    if (get_type(et) < 3)
    {
        p_test = p_test*10 + get_type(et);
        par_check(et);
    }
    // testes com numeros complexos -------------------------------------------
    else par_exp_cmp(et);
    // fim do teste -----------------------------------------------------------
}

// executa instrucao CALL para funcoes tipo void (por isso o v de void)
void vcall(int id)
{
    // posso usar funcao com chamada void tb, por isso testar tudo aqui
    if  (v_type[id] < 6)
    {
        fprintf(stderr, "Erro na linha %d: cadê essa função %s?\n", line_num+1, rem_fname(v_name[id], fname));
        return;
    }

    // checa numero de parametros
    if (get_npar(p_test) != get_npar(v_fpar[id])) // p_test tem a lista de par na chamada e v_fpar na declaracao
    {
        fprintf(stderr, "Erro na linha %d: olha lá direito quantos parâmetros tem a função %s.\n", line_num+1, rem_fname(v_name[id], fname));
    }

    if (using_macro == 0) fprintf(f_asm, "CALL %s\n", v_name[id]);

    v_used[id] = 1; // funcao ja foi chamada
    acc_ok     = 0; // acc ta liberado
}

// executa instrucao CALL para funcoes com retorno (por isso o f de funcao)
int fcall(int id)
{
    if (v_type[id] == 6)
    {
        fprintf (stderr, "Erro na linha %d: olha lá a funcao %s, você vai ver que ela nao retorna nada.\n", line_num+1, v_name[id]);
        return 0;
    }
    else if (v_type[id] < 6)
    {
        fprintf (stderr, "Erro na linha %d: A função %s tá onde?\n", line_num+1, rem_fname(v_name[id], fname));
        return 0;
    }

    if (get_npar(p_test) != get_npar(v_fpar[id]))
    {
        fprintf(stderr, "Erro na linha %d: lista de parâmetros da função %s difere da original.\n", line_num+1, v_name[id]);
        return 0;
    }

    if (using_macro == 0) fprintf(f_asm, "CALL %s\n",v_name[id]);

    v_used[id] = 1;             // funcao ja foi usada

    return (v_type[id]-6)*OFST; // retorna o tipo de dado (void , int, float ou comp)
}

// calcula quantos parametros uma funcao tem
int get_npar(int par)
{
    int t_fun = par;
    int n_par = 0;

    while (t_fun != 0)
    {
        t_fun = t_fun/10;
        n_par++;
    }

    return n_par;
}

// checa se o argumento passado pra funcao esta ok
void par_check(int et)
{
    // pega numero de parametros original
    int n_par = get_npar(v_fpar[fun_id2]);

    // pega tipo e posicao do parametro atual a ser chamado
    int  t_cal = p_test; // vai guardar o tipo de parametro (0, 1, 2 ou 3)
    int  aux   = p_test;
    int id_cal = n_par ;
    int  index = 1;      // vai guardar a posicao do parametro
    while (aux > 10)
    {
           aux = aux   / 10;
         t_cal = t_cal % 10;
        id_cal--;
         index++;
    }

    // pega tipo do parametro atual na funcao original
    int t_fun = v_fpar[fun_id2];
    int i;
    for (i = 1; i < id_cal; i++) t_fun = t_fun/10;
    t_fun = t_fun % 10;

    // ufa, agora sim, posso testar os parametros
    load_check(et,0);

    // ------------------------------------------------------------------------
    // checando todas as possibilidades ---------------------------------------
    // ------------------------------------------------------------------------

    // falta fazer pra comp?

    // original eh int e chamada eh float -------------------------------------

    if ((t_fun == 1) && (t_cal == 2))
    {
        fprintf(stdout, "Atenção na linha %d: convertendo float para int no parâmetro %d da função %s.\n", line_num+1, index, v_name[fun_id2]);

        if (prtype == 0)
        {
            if (using_macro == 0) fprintf(f_asm, "CALL float2int\n");
            f2i = 1;
        }
    }

    // original eh float e chamada eh int -------------------------------------

    if ((t_fun == 2) && (t_cal == 1))
    {
        fprintf(stdout, "Atenção na linha %d: convertendo int para float no parâmetro %d da função %s.\n", line_num+1, index, v_name[fun_id2]);

        if (prtype == 0)
        {
            if (using_macro == 0) fprintf(f_asm, "CALL int2float\n");
            i2f = 1;
        }
    }
}
