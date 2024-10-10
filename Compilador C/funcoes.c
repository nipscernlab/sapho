#include "funcoes.h"
#include "variaveis.h"
#include "t2t.h"
#include "diretivas.h"
#include "data_declar.h"
#include "data_assign.h"
#include "data_use.h"
#include "labels.h"
#include <string.h>

// ----------------------------------------------------------------------------
// declaracao -----------------------------------------------------------------
// ----------------------------------------------------------------------------

void declar_fun(int id1, int id2) //id1 -> tipo, id2 -> indice para o nome; v_name -> tabela com os nomes
{
    // testa se eh a funcao main e atualiza o status dela
    if ((mainok == 0) && (strcmp(v_name[id2], "main") != 0))
    {
        if (using_macro == 0) fprintf(f_asm, "CALL main\n@fim JMP fim\n");

        mainok = 2; // funcao main foi chamada no inicio
    }

    if (using_macro == 0) fprintf(f_asm, "@%s ", v_name[id2]);

    strcpy(fname, v_name[id2]); // seta a variavel de estado fname para o nome da funcao a ser analisada
    v_type[id2] = id1+5       ; // v_type vai ser funcao (void, int, float) (5, 6, 7)
    fun_id1     = id2         ; // seta a variavel de estado fun_id1 para o id do nome da funcao
    ret_ok      =     0       ; // seta a variavel de estado ret_ok para zero (vai comecar o parser da funcao)
}

// pega o primeiro parametro
void declar_firstpar(int id)
{
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
    if (using_macro == 0) fprintf(f_asm, "SETP %s\n" , v_name[id]);
}

// quando acha a palavra chave return
void declar_ret(int et) // tipo estendido da expressao x
{
    // ve se precisa carregar x
    load_check(et,0);

    // checa se eh funcao mesmo, ou void por engano
    if (v_type[fun_id1] == 5)
        fprintf (stderr, "Erro na linha %d: retorno em função void? viajou!\n", line_num+1);

    // testa se esta dentro de um if/else
    if (get_if() > 0)
        fprintf(stdout, "Cuidado na linha %d: usar return dentro de if/else pode dar pau, caso você esqueça em algum lugar!\n", line_num+1);

    // ------------------------------------------------------------------------
    // checa todas as combinacoes ---------------------------------------------
    // ------------------------------------------------------------------------

    int left_type = v_type[fun_id1];
    int righ_type = get_type(et);

    // funcao eh int mas o return eh float ------------------------------------

    if ((left_type == 6) && (righ_type == 2))
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

    if ((left_type == 7) && (righ_type == 1))
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

    if (using_macro == 0) fprintf(f_asm, "RETURN\n");

    acc_ok = 0; // acc liberado, ta certo isso?
    ret_ok = 1; // apareceu a palavra chave return na funcao certinho
}

// fim do parser da declaracao de uma funcao
void func_ret(int id) // id -> id da funcao atual
{
    // checa se a funcao teve a instrucao return x;
    if ((v_type[id] != 5) && (ret_ok == 0))
        fprintf (stderr, "Erro na função %s: cadê o retorno pra essa função?\n", v_name[id]);

    if (strcmp(v_name[id], "main") == 0) // se eh funcao main ...
    {
        if (mainok == 0) // ainda nao tinha executado a funcao main
        {
             if (using_macro == 0) fprintf(f_asm, "@fim JMP fim\n");
        }
        else if (using_macro == 0) fprintf(f_asm, "RETURN\n"); // tem subrotinas

        v_used[id] = 1; // funcao main foi usada
            mainok = 1; // funcao main foi parseada
    }
    else if (v_type[id] == 5) {if (using_macro == 0) fprintf(f_asm, "RETURN\n");} // se eh tipo void, ainda precisa gerar um RETURN

    // variavel de ambiente fname fica vazia (saiu de uma funcao)
    strcpy(fname, "");
}

// ----------------------------------------------------------------------------
// utilizacao -----------------------------------------------------------------
// ----------------------------------------------------------------------------

// da LOAD no primeiro parametro (se houver)
// get_type da o tipo de parametro (0, 1, 2 ou 3) (void, int, float ou comp)
// p_test consegue guardar a posicao e tipo de todos os parametros na chamada da funcao
void par_exp(int et)
{
    p_test = get_type(et);
    par_check(et);
}

// da LOAD nos proximos parametros
void par_listexp(int et)
{
    p_test = p_test*10 + get_type(et);
    par_check(et);
}

// executa instrucao CALL para funcoes tipo void (por isso o v de void)
void vcall(int id)
{
    // posso usar funcao com chamada void tb, por isso testar tudo aqui
    if  ((v_type[id] != 5) && (v_type[id] != 6) && (v_type[id] != 7))
    {
        fprintf(stderr, "Erro na linha %d: cadê essa função %s?\n", line_num+1, rem_fname(v_name[id], fname));
        return;
    }

    if (get_npar(p_test) != get_npar(v_fpar[id])) // p_test tem a lista de par na chamada e v_fpar na declaracao
        fprintf(stderr, "Erro na linha %d: olha lá direito quantos parâmetros tem a função %s.\n", line_num+1, rem_fname(v_name[id], fname));

    if (using_macro == 0) fprintf(f_asm, "CALL %s\n", v_name[id]);

    v_used[id] = 1; // funcao ja foi chamada
    acc_ok     = 0; // acc ta liberado
}

// executa instrucao CALL para funcoes com retorno (por isso o f de funcao)
int fcall(int id)
{
    if  (v_type[id] == 5)
    {
        fprintf (stderr, "Erro na linha %d: olha lá a funcao %s, você vai ver que ela nao retorna nada.\n", line_num+1, v_name[id]);
        return 0;
    }
    else if ((v_type[id] != 6) && (v_type[id] != 7))
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

    v_used[id] = 1; // funcao ja foi usada

    return (v_type[id]-5)*OFST; // retorna o tipo de dado (void , int ou float)
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
