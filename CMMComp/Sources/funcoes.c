#include "..\Headers\funcoes.h"
#include "..\Headers\variaveis.h"
#include "..\Headers\t2t.h"
#include "..\Headers\diretivas.h"
#include "..\Headers\data_declar.h"
#include "..\Headers\data_assign.h"
#include "..\Headers\data_use.h"
#include "..\Headers\labels.h"
#include "..\Headers\global.h"

#include <string.h>

// ----------------------------------------------------------------------------
// redeclaracao de variaveis globais ------------------------------------------
// ----------------------------------------------------------------------------

int fun_id2;         // guarda id da funcao sendo usada
int p_test;          // identifica parametros na chamada de funcoes (parecido com OFST, mas de valor 10)
int ret_ok;          // diz se teve um retorno da funcao corretamente
int fun_id1;         // guarda id da funcao sendo parseada
int v_fpar[NVARMAX]; // se ID eh uma funcao, diz a lista de parametros

// ----------------------------------------------------------------------------
// declaracao -----------------------------------------------------------------
// ----------------------------------------------------------------------------

// declara uma funcao
void declar_fun(int id1, int id2) //id1 -> tipo, id2 -> indice para o nome
{
    // se ainda nao for a funcao main, entao tem que dar um call pra ela antes
    // pois CALL main deve ser a primeira instrucao do processador depois do reset
    // Desabilitei a nao utilizacao de CALL para funcao main sozinho
    // preciso ver como fazer isso funcionar no gtkwave antes
    if ((mainok == 0) && (strcmp(v_name[id2], "main") != 0))
    //if (mainok == 0)
    {
        fprintf(f_asm, "CALL main\n");
        num_ins++;
        fprintf(f_lin, "%s\n", itob(-2,20));

        fprintf(f_asm, "@fim JMP fim\n");
        num_ins++;
        fprintf(f_lin, "%s\n", itob(-3,20));

        mainok = 2; // funcao main foi chamada no inicio
    }

    if (using_macro == 0) fprintf(f_asm, "@%s ", v_name[id2]);

    strcpy(fname, v_name[id2]); // seta a variavel de estado fname para o nome da funcao a ser analisada
    v_type[id2] = id1+6       ; // v_type vai ser funcao (void, int, float, comp) (6, 7, 8, 9)
    fun_id1     = id2         ; // seta a variavel de estado fun_id1 para o id do nome da funcao
    ret_ok      =     0       ; // seta a variavel de estado ret_ok para zero (vai comecar o parser da funcao)
}

// pega o primeiro parametro
void declar_fst(int id)
{
    // testes com numeros complexos -------------------------------------------
    if (v_type[id] > 2)
    {
        // primeiro pega o img da pilha
        int idi = get_img_id(id);
        add_instr("SETP %s\n", v_name[idi]);
    }
    // fim do teste -----------------------------------------------------------

    // o primeiro parametro da funcao eh com SET (pq eh o ultimo a ser chamado)
    // os proximos (se houver) sao com SETP em outra funcao
    add_instr("SET %s\n", v_name[id]);
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
        add_instr("SETP %s\n", v_name[idi]);
    }
    // fim do teste -----------------------------------------------------------

        add_instr("SETP %s\n" , v_name[id]);
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

    int etr, eti;
    int left_type = v_type[fun_id1];

    if (prtype == 0)
    {
        // int com int var
        if ((left_type == 7) && (get_type(et) == 1) && (et%OFST!=0))
        {
            add_instr("LOAD %s\n", v_name[et%OFST]);
        }

        // int com int acc
        if ((left_type == 7) && (get_type(et) == 1) && (et%OFST==0))
        {
            // nao faz nada
        }

        // int com float var
        if ((left_type == 7) && (get_type(et) == 2) && (et%OFST!=0) && (v_isco[et%OFST]==0))
        {
            fprintf(stdout, "Atenção na linha %d: vai converter float para int no retorno da função %s? Dá-lhe código!\n", line_num+1, v_name[fun_id1]);

            add_instr("LOAD %s\n", v_name[et%OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
        }

        // int com float const
        if ((left_type == 7) && (get_type(et) == 2) && (et%OFST!=0) && (v_isco[et%OFST]==1))
        {
            fprintf(stdout, "Atenção na linha %d: vai converter float para int no retorno da função %s? Dá-lhe código!\n", line_num+1, v_name[fun_id1]);

            add_instr("LOAD %d // %s\n", f2mf(v_name[et%OFST]));
            add_instr("CALL float2int\n"); f2i = 1;
        }

        // int com float acc
        if ((left_type == 7) && (get_type(et) == 2) && (et%OFST==0))
        {
            fprintf(stdout, "Atenção na linha %d: vai converter float para int no retorno da função %s? Dá-lhe código!\n", line_num+1, v_name[fun_id1]);
            add_instr("CALL float2int\n"); f2i = 1;
        }

        // int com comp const
        if ((left_type == 7) && (get_type(et) == 5))
        {
            fprintf (stdout, "Atenção na linha %d: nessa conversão, eu vou arredondar a parte real hein!\n", line_num+1);

            get_cmp_cst(et,&etr,&eti);
            
            add_instr("LOAD %u // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
        }

        // int com comp var
        if ((left_type == 7) && (get_type(et) == 3) && (et%OFST!=0))
        {
            fprintf (stdout, "Atenção na linha %d: nessa conversão, eu vou arredondar a parte real hein!\n", line_num+1);

            get_cmp_ets(et,&etr,&eti);
            
            add_instr("LOAD %s\n", v_name[etr % OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
        }

        // int com comp acc
        if ((left_type == 7) && (get_type(et) == 3) && (et%OFST==0))
        {
            fprintf (stdout, "Atenção na linha %d: nessa conversão, eu vou arredondar a parte real hein!\n", line_num+1);
    
            add_instr("SETP lixo\n");
            add_instr("CALL float2int\n"); f2i = 1;
        }

        // float com int var
        if ((left_type == 8) && (get_type(et) == 1) && (et%OFST!=0))
        {
            fprintf(stdout, "Atenção na linha %d: retorno é float, mas recebe int.\n", line_num+1);

            add_instr("LOAD %s\n", v_name[et % OFST]);
            add_instr("CALL int2float\n"); i2f = 1;
        }

        // float com int acc
        if ((left_type == 8) && (get_type(et) == 1) && (et%OFST==0))
        {
            fprintf(stdout, "Atenção na linha %d: retorno é float, mas recebe int.\n", line_num+1);
            
            add_instr("CALL int2float\n"); i2f = 1;
        }

        // float com float var
        if ((left_type == 8) && (get_type(et) == 2) && (et%OFST!=0) && (v_isco[et%OFST]==0))
        {
            add_instr("LOAD %s\n", v_name[et % OFST]);
        }

        // float com float const
        if ((left_type == 8) && (get_type(et) == 2) && (et%OFST!=0) && (v_isco[et%OFST]==1))
        {
            add_instr("LOAD %u // %s\n", f2mf(v_name[et % OFST]), v_name[et % OFST]);
        }

        // float com float acc
        if ((left_type == 8) && (get_type(et) == 2) && (et%OFST==0))
        {
            // nao faz nada
        }

        // float com comp const
        if ((left_type == 8) && (get_type(et) == 5))
        {
            fprintf (stdout, "Atenção na linha %d: nessa conversão, eu vou pegar só a parte real hein!\n", line_num+1);

            get_cmp_cst(et,&etr,&eti);
            
            add_instr("LOAD %u // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
        }

        // float com comp var
        if ((left_type == 8) && (get_type(et) == 3) && (et%OFST!=0))
        {
            fprintf (stdout, "Atenção na linha %d: nessa conversão, eu vou pegar só a parte real hein!\n", line_num+1);

            get_cmp_ets(et,&etr,&eti);
            
            add_instr("LOAD %s\n", v_name[etr % OFST]);
        }

        // float com comp acc
        if ((left_type == 8) && (get_type(et) == 3) && (et%OFST==0))
        {
            fprintf (stdout, "Atenção na linha %d: nessa conversão, eu vou pegar só a parte real hein!\n", line_num+1);
    
            add_instr("SETP lixo\n");
        }

        // comp com int var
        if ((left_type == 9) && (get_type(et) == 1) && (et%OFST!=0))
        {
            fprintf(stdout, "Atenção na linha %d: retorno da função é comp, mas recebe int.\n", line_num+1);

            add_instr("LOAD %s\n", v_name[et % OFST]);
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("PLD float_zero\n");
        }

        // comp com int acc
        if ((left_type == 9) && (get_type(et) == 1) && (et%OFST==0))
        {
            fprintf(stdout, "Atenção na linha %d: retorno da função é comp, mas recebe int.\n", line_num+1);
            
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("PLD float_zero\n");
        }

        // comp com float var
        if ((left_type == 9) && (get_type(et) == 2) && (et%OFST!=0) && (v_isco[et%OFST]==0))
        {
            fprintf(stdout, "Atenção na linha %d: retorno da função é comp, mas recebe float.\n", line_num+1);

            add_instr("LOAD %s\n", v_name[et % OFST]);
            add_instr("PLD float_zero\n");
        }

        // comp com float const
        if ((left_type == 9) && (get_type(et) == 2) && (et%OFST!=0) && (v_isco[et%OFST]==1))
        {
            fprintf(stdout, "Atenção na linha %d: retorno da função é comp, mas recebe float.\n", line_num+1);

            add_instr("LOAD %u // %s\n", f2mf(v_name[et % OFST]), v_name[et % OFST]);
            add_instr("PLD float_zero\n");
        }

        // comp com float acc
        if ((left_type == 9) && (get_type(et) == 2) && (et%OFST==0))
        {
            fprintf(stdout, "Atenção na linha %d: retorno da função é comp, mas recebe float.\n", line_num+1);

            add_instr("PLD float_zero\n");
        }

        // comp com comp const
        if ((left_type == 9) && (get_type(et) == 5))
        {
            get_cmp_cst(et,&etr,&eti);
            
            add_instr("LOAD %u // %s\n", f2mf(v_name[etr % OFST]), v_name[etr % OFST]);
            add_instr("PLD  %u // %s\n", f2mf(v_name[eti % OFST]), v_name[eti % OFST]);
        }

        // comp com comp var
        if ((left_type == 9) && (get_type(et) == 3) && (et%OFST!=0))
        {
            get_cmp_ets(et,&etr,&eti);
            
            add_instr("LOAD %s\n", v_name[etr % OFST]);
            add_instr("PLD  %s\n", v_name[eti % OFST]);
        }

        // comp com comp acc
        if ((left_type == 9) && (get_type(et) == 3) && (et%OFST==0))
        {
            // nao faz nada
        }
    }

    // ------------------------------------------------------------------------
    // finaliza ---------------------------------------------------------------
    // ------------------------------------------------------------------------

    if (ret == 0) return;

    add_instr("RETURN\n");

    acc_ok = 0; // apesar de ter exp no acc, tem q liberar para comecar outra funcao
    ret_ok = 1; // apareceu a palavra chave return na funcao certinho
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
             fprintf(f_asm, "@fim JMP fim\n");
             num_ins++;
        fprintf(f_lin, "%s\n", itob(-3,20));
        }
        else add_instr("RETURN\n"); // tem subrotinas

        v_used[id] = 1; // funcao main foi usada
        mainok     = 1; // funcao main foi parseada
    }
    else if (v_type[id] == 6) {add_instr("RETURN\n");} // se eh tipo void, ainda precisa gerar um RETURN

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
         add_instr("JMP fim\n");  // ai nao usa RETURN, pula pro fim
    }
    else add_instr("RETURN\n");   // se nao, usa return padrao
}

// ----------------------------------------------------------------------------
// utilizacao -----------------------------------------------------------------
// ----------------------------------------------------------------------------

// da LOAD no primeiro parametro (se houver)
// get_type da o tipo de parametro (0, 1, 2, 3) (void, int, float, comp)
// p_test consegue guardar a posicao e tipo de todos os parametros na chamada da funcao
void par_exp(int et)
{
    p_test = 0; // inicializa a variavel de estado p_test
    p_test = p_test*10 + get_type(et);
    par_check(et);
    acc_ok = 1;
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

    add_instr("CALL %s\n", v_name[id]);

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

    add_instr("CALL %s\n",v_name[id]);

    v_used[id] = 1;             // funcao ja foi usada
    acc_ok     = 1;             // acc ta ocupado

    return (v_type[id]-6)*OFST; // retorna o tipo de dado (void, int, float ou comp)
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

    char ld[10];
    if (acc_ok == 0) strcpy(ld,"LOAD"); else strcpy(ld,"PLD");

    // ------------------------------------------------------------------------
    // checando todas as possibilidades ---------------------------------------
    // ------------------------------------------------------------------------

    int etr, eti;

    if (prtype == 0)
    {
        // original eh int e chamada eh int var -------------------------------

        if ((t_fun == 1) && (t_cal == 1) && (et % OFST != 0))
        {
            add_instr("%s %s\n", ld, v_name[et%OFST]);
        }

        // original eh int e chamada eh int acc -------------------------------

        if ((t_fun == 1) && (t_cal == 1) && (et % OFST == 0))
        {
            // nao faz nada
        }

        // original eh int e chamada eh float var -----------------------------

        if ((t_fun == 1) && (t_cal == 2) && (et % OFST != 0) && (v_isco[et%OFST] == 0))
        {
            fprintf(stdout, "Atenção na linha %d: convertendo float para int no parâmetro %d da função %s.\n", line_num+1, index, v_name[fun_id2]);

            add_instr("%s %s\n", ld, v_name[et%OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
        }

        // original eh int e chamada eh float const ---------------------------

        if ((t_fun == 1) && (t_cal == 2) && (et % OFST != 0) && (v_isco[et%OFST] == 1))
        {
            fprintf(stdout, "Atenção na linha %d: convertendo float para int no parâmetro %d da função %s.\n", line_num+1, index, v_name[fun_id2]);

            add_instr("%s %d // %s\n", ld, f2mf(v_name[et%OFST]), v_name[et%OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
        }

        // original eh int e chamada eh float acc -----------------------------

        if ((t_fun == 1) && (t_cal == 2) && (et % OFST == 0))
        {
            fprintf(stdout, "Atenção na linha %d: convertendo float para int no parâmetro %d da função %s.\n", line_num+1, index, v_name[fun_id2]);

            add_instr("CALL float2int\n"); f2i = 1;
        }

        // original eh int e chamada eh comp const ----------------------------

        if ((t_fun == 1) && (t_cal == 5))
        {
            fprintf(stdout, "Atenção na linha %d: convertendo comp para int no parâmetro %d da função %s.\n", line_num+1, index, v_name[fun_id2]);

            get_cmp_cst(et,&etr,&eti);

            add_instr("%s %d // %s\n", ld, f2mf(v_name[etr%OFST]), v_name[etr%OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
        }

        // original eh int e chamada eh comp var ------------------------------

        if ((t_fun == 1) && (t_cal == 3) && (et % OFST != 0))
        {
            fprintf(stdout, "Atenção na linha %d: convertendo comp para int no parâmetro %d da função %s.\n", line_num+1, index, v_name[fun_id2]);

            add_instr("%s %s\n", ld, v_name[et%OFST]);
            add_instr("CALL float2int\n"); f2i = 1;
        }

        // original eh int e chamada eh comp acc ------------------------------

        if ((t_fun == 1) && (t_cal == 3) && (et % OFST == 0))
        {
            fprintf(stdout, "Atenção na linha %d: convertendo comp para int no parâmetro %d da função %s.\n", line_num+1, index, v_name[fun_id2]);

            add_instr("SETP lixo_aux\n");
            add_instr("CALL float2int\n"); f2i = 1;
        }

        // original eh float e chamada eh int var -----------------------------

        if ((t_fun == 2) && (t_cal == 1) && (et % OFST != 0))
        {
            fprintf(stdout, "Atenção na linha %d: convertendo int para float no parâmetro %d da função %s.\n", line_num+1, index, v_name[fun_id2]);
            
            add_instr("%s %s\n", ld, v_name[et%OFST]);
            add_instr("CALL int2float\n"); i2f = 1;
        }

        // original eh float e chamada eh int acc -----------------------------

        if ((t_fun == 2) && (t_cal == 1) && (et % OFST == 0))
        {
            fprintf(stdout, "Atenção na linha %d: convertendo int para float no parâmetro %d da função %s.\n", line_num+1, index, v_name[fun_id2]);
            
            add_instr("CALL int2float\n"); i2f = 1;
        }

        // original eh float e chamada eh float var ---------------------------

        if ((t_fun == 2) && (t_cal == 2) && (et % OFST != 0) && (v_isco[et%OFST] == 0))
        {
            add_instr("%s %s\n", ld, v_name[et%OFST]);
        }

        // original eh float e chamada eh float const -------------------------

        if ((t_fun == 2) && (t_cal == 2) && (et % OFST != 0) && (v_isco[et%OFST] == 1))
        {
            add_instr("%s %d // %s\n", ld, f2mf(v_name[et%OFST]), v_name[et%OFST]);
        }

        // original eh float e chamada eh float acc ---------------------------

        if ((t_fun == 2) && (t_cal == 2) && (et % OFST == 0))
        {
            // nao faz nada
        }

        // original eh float e chamada eh comp const --------------------------

        if ((t_fun == 2) && (t_cal == 5))
        {
            fprintf(stdout, "Atenção na linha %d: convertendo comp para float no parâmetro %d da função %s.\n", line_num+1, index, v_name[fun_id2]);

            get_cmp_cst(et,&etr,&eti);

            add_instr("%s %d // %s\n", ld, f2mf(v_name[etr%OFST]), v_name[etr%OFST]);
        }

        // original eh float e chamada eh comp var ----------------------------

        if ((t_fun == 2) && (t_cal == 3) && (et % OFST != 0))
        {
            fprintf(stdout, "Atenção na linha %d: convertendo comp para float no parâmetro %d da função %s.\n", line_num+1, index, v_name[fun_id2]);

            add_instr("%s %s\n", ld, v_name[et%OFST]);
        }

        // original eh float e chamada eh comp acc ----------------------------

        if ((t_fun == 2) && (t_cal == 3) && (et % OFST == 0))
        {
            fprintf(stdout, "Atenção na linha %d: convertendo comp para float no parâmetro %d da função %s.\n", line_num+1, index, v_name[fun_id2]);

            add_instr("SETP lixo_aux\n");
        }

        // original eh comp e chamada eh int var ------------------------------

        if ((t_fun == 3) && (t_cal == 1) && (et % OFST != 0))
        {
            fprintf(stdout, "Atenção na linha %d: convertendo int para comp no parâmetro %d da função %s.\n", line_num+1, index, v_name[fun_id2]);
            
            add_instr("%s %s\n", ld, v_name[et%OFST]);
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("PLD %d // %s\n", f2mf("0.0"), "0.0");
        }

        // original eh comp e chamada eh int acc ------------------------------

        if ((t_fun == 3) && (t_cal == 1) && (et % OFST == 0))
        {
            fprintf(stdout, "Atenção na linha %d: convertendo int para comp no parâmetro %d da função %s.\n", line_num+1, index, v_name[fun_id2]);
            
            add_instr("CALL int2float\n"); i2f = 1;
            add_instr("PLD %d // %s\n", f2mf("0.0"), "0.0");
        }

        // original eh comp e chamada eh float var ----------------------------

        if ((t_fun == 3) && (t_cal == 2) && (et % OFST != 0) && (v_isco[et%OFST] == 0))
        {
            fprintf(stdout, "Atenção na linha %d: convertendo float para comp no parâmetro %d da função %s.\n", line_num+1, index, v_name[fun_id2]);

            add_instr("%s %s\n", ld, v_name[et%OFST]);
            add_instr("PLD %d // %s\n", f2mf("0.0"), "0.0");
        }

        // original eh comp e chamada eh float const --------------------------

        if ((t_fun == 3) && (t_cal == 2) && (et % OFST != 0) && (v_isco[et%OFST] == 1))
        {
            fprintf(stdout, "Atenção na linha %d: convertendo float para comp no parâmetro %d da função %s.\n", line_num+1, index, v_name[fun_id2]);

            add_instr("%s %d // %s\n", ld, f2mf(v_name[et%OFST]), v_name[et%OFST]);
            add_instr("PLD %d // %s\n", f2mf("0.0"), "0.0");
        }

        // original eh comp e chamada eh float acc ----------------------------

        if ((t_fun == 3) && (t_cal == 2) && (et % OFST == 0))
        {
            fprintf(stdout, "Atenção na linha %d: convertendo float para comp no parâmetro %d da função %s.\n", line_num+1, index, v_name[fun_id2]);

            add_instr("PLD %d // %s\n", f2mf("0.0"), "0.0");
        }

        // original eh comp e chamada eh comp const ---------------------------

        if ((t_fun == 3) && (t_cal == 5))
        {
            get_cmp_cst(et,&etr,&eti);

            add_instr("%s %d // %s\n" , ld, f2mf(v_name[etr%OFST]), v_name[etr%OFST]);
            add_instr("PLD %d // %s\n",     f2mf(v_name[eti%OFST]), v_name[eti%OFST]);
        }

        // original eh comp e chamada eh comp var -----------------------------

        if ((t_fun == 3) && (t_cal == 3) && (et % OFST != 0))
        {
            get_cmp_ets(et,&etr,&eti); // pega os IDs estendidos do right na memoria

            add_instr("%s %s\n" , ld, v_name[etr%OFST]);
            add_instr("PLD %s\n",     v_name[eti%OFST]);
        }

        // original eh comp e chamada eh comp acc -----------------------------

        if ((t_fun == 3) && (t_cal == 3) && (et % OFST == 0))
        {
            // nao faz nada
        }
    }
    else
    {
        // original eh int e chamada eh int var -------------------------------

        if ((t_fun == 1) && (t_cal == 1) && (et % OFST != 0))
        {
            add_instr("%s %s\n", ld, v_name[et%OFST]);
        }

        // original eh int e chamada eh int acc -------------------------------

        if ((t_fun == 1) && (t_cal == 1) && (et % OFST == 0))
        {
            // nao faz nada
        }

        // original eh int e chamada eh float ---------------------------------

        if ((t_fun == 1) && (t_cal == 2) && (et % OFST != 0))
        {
            fprintf(stdout, "Atenção na linha %d: convertendo float para int no parâmetro %d da função %s.\n", line_num+1, index, v_name[fun_id2]);

            add_instr("%s %s\n", ld, v_name[et%OFST]);
        }

        // original eh int e chamada eh float acc -----------------------------

        if ((t_fun == 1) && (t_cal == 2) && (et % OFST == 0))
        {
            fprintf(stdout, "Atenção na linha %d: convertendo float para int no parâmetro %d da função %s.\n", line_num+1, index, v_name[fun_id2]);
        }

        // original eh int e chamada eh comp const ----------------------------

        if ((t_fun == 1) && (t_cal == 5))
        {
            fprintf(stdout, "Atenção na linha %d: convertendo comp para int no parâmetro %d da função %s.\n", line_num+1, index, v_name[fun_id2]);

            get_cmp_cst(et,&etr,&eti);

            add_instr("%s %s\n", ld, v_name[etr%OFST]);
        }

        // original eh int e chamada eh comp var ------------------------------

        if ((t_fun == 1) && (t_cal == 3) && (et % OFST != 0))
        {
            fprintf(stdout, "Atenção na linha %d: convertendo comp para int no parâmetro %d da função %s.\n", line_num+1, index, v_name[fun_id2]);

            add_instr("%s %s\n", ld, v_name[et%OFST]);
        }

        // original eh int e chamada eh comp acc ------------------------------

        if ((t_fun == 1) && (t_cal == 3) && (et % OFST == 0))
        {
            fprintf(stdout, "Atenção na linha %d: convertendo comp para int no parâmetro %d da função %s.\n", line_num+1, index, v_name[fun_id2]);

            add_instr("SETP lixo_aux\n");
        }

        // original eh float e chamada eh int var -----------------------------

        if ((t_fun == 2) && (t_cal == 1) && (et % OFST != 0))
        {
            fprintf(stdout, "Atenção na linha %d: convertendo int para float no parâmetro %d da função %s.\n", line_num+1, index, v_name[fun_id2]);
            
            add_instr("%s %s\n", ld, v_name[et%OFST]);
        }

        // original eh float e chamada eh int acc -----------------------------

        if ((t_fun == 2) && (t_cal == 1) && (et % OFST == 0))
        {
            fprintf(stdout, "Atenção na linha %d: convertendo int para float no parâmetro %d da função %s.\n", line_num+1, index, v_name[fun_id2]);
        }

        // original eh float e chamada eh float -------------------------------

        if ((t_fun == 2) && (t_cal == 2) && (et % OFST != 0))
        {
            add_instr("%s %s\n", ld, v_name[et%OFST]);
        }

        // original eh float e chamada eh float acc ---------------------------

        if ((t_fun == 2) && (t_cal == 2) && (et % OFST == 0))
        {
            // nao faz nada
        }

        // original eh float e chamada eh comp const --------------------------

        if ((t_fun == 2) && (t_cal == 5))
        {
            fprintf(stdout, "Atenção na linha %d: convertendo comp para float no parâmetro %d da função %s.\n", line_num+1, index, v_name[fun_id2]);

            get_cmp_cst(et,&etr,&eti);

            add_instr("%s %s\n", ld, v_name[etr%OFST]);
        }

        // original eh float e chamada eh comp var ----------------------------

        if ((t_fun == 2) && (t_cal == 3) && (et % OFST != 0))
        {
            fprintf(stdout, "Atenção na linha %d: convertendo comp para float no parâmetro %d da função %s.\n", line_num+1, index, v_name[fun_id2]);

            add_instr("%s %s\n", ld, v_name[et%OFST]);
        }

        // original eh float e chamada eh comp acc ----------------------------

        if ((t_fun == 2) && (t_cal == 3) && (et % OFST == 0))
        {
            fprintf(stdout, "Atenção na linha %d: convertendo comp para float no parâmetro %d da função %s.\n", line_num+1, index, v_name[fun_id2]);

            add_instr("SETP lixo_aux\n");
        }

        // original eh comp e chamada eh int var ------------------------------

        if ((t_fun == 3) && (t_cal == 1) && (et % OFST != 0))
        {
            fprintf(stdout, "Atenção na linha %d: convertendo int para comp no parâmetro %d da função %s.\n", line_num+1, index, v_name[fun_id2]);
            
            add_instr("%s %s\n", ld, v_name[et%OFST]);
            add_instr("PLD 0.0\n");
        }

        // original eh comp e chamada eh int acc ------------------------------

        if ((t_fun == 3) && (t_cal == 1) && (et % OFST == 0))
        {
            fprintf(stdout, "Atenção na linha %d: convertendo int para comp no parâmetro %d da função %s.\n", line_num+1, index, v_name[fun_id2]);

            add_instr("PLD 0.0\n");
        }

        // original eh comp e chamada eh float var ----------------------------

        if ((t_fun == 3) && (t_cal == 2) && (et % OFST != 0))
        {
            fprintf(stdout, "Atenção na linha %d: convertendo float para comp no parâmetro %d da função %s.\n", line_num+1, index, v_name[fun_id2]);

            add_instr("%s %s\n", ld, v_name[et%OFST]);
            add_instr("PLD 0.0");
        }

        // original eh comp e chamada eh float acc ----------------------------

        if ((t_fun == 3) && (t_cal == 2) && (et % OFST == 0))
        {
            fprintf(stdout, "Atenção na linha %d: convertendo float para comp no parâmetro %d da função %s.\n", line_num+1, index, v_name[fun_id2]);

            add_instr("PLD 0.0");
        }

        // original eh comp e chamada eh comp const ---------------------------

        if ((t_fun == 3) && (t_cal == 5))
        {
            get_cmp_cst(et,&etr,&eti);

            add_instr("%s %s\n" , ld, v_name[etr%OFST]);
            add_instr("PLD %s\n",     v_name[eti%OFST]);
        }

        // original eh comp e chamada eh comp var -----------------------------

        if ((t_fun == 3) && (t_cal == 3) && (et % OFST != 0))
        {
            get_cmp_ets(et,&etr,&eti); // pega os IDs estendidos do right na memoria

            add_instr("%s %s\n" , ld, v_name[etr%OFST]);
            add_instr("PLD %s\n",     v_name[eti%OFST]);
        }

        // original eh comp e chamada eh comp acc -----------------------------

        if ((t_fun == 3) && (t_cal == 3) && (et % OFST == 0))
        {
            // nao faz nada
        }
    }
}
