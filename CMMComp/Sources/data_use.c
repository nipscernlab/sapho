#include "data_use.h"
#include "variaveis.h"
#include "diretivas.h"
#include "t2t.h"
#include "data_assign.h"
#include "oper.h"
#include <string.h>
#include <stdlib.h>

// funcao interna para ajudar a gerar a string que vai depois do LOAD
// usada no load_check
void prepar_oper(char *num, int id, int et, int neg)
{
    char nf[64] = "";               // comeca vazio
    if (neg == 1) strcat(nf, "-");  // xuxa o sinal - se for pra negar uma constante
    strcat(nf,v_name[id]);          // xuxa o nome da variavel/constante

    // se for uma constante do tipo float e o proc eh ponto fixo
    if ((v_isco[id] == 1) && (et >= 2*OFST) && (prtype == 0))
    {
        // tem que printar o numero inteiro coorrespondente
        itoa(f2mf(nf), num, 10);
        // acrescenta um comentario pra saber qual eh o numero em ponto flutuante
        strcat(num, " // ");
        strcat(num,     nf);
    }
    // se nao, eh soh printar o numero mesmo
    else strcpy(num, nf);
}

// carrega uma variavel no acc caso ela ja nao esteja
// et eh um indice que diz se a variavel eh
// nao declarada (menor que OFST), int (de OFST a 2*OFTS) ou float (acima de 2*OFST)
// se et for OFST ou 2*OFST, eh um int ou um float ja carregado no acc respectivamente
// ai nao precisa gerar a intrucao LOAD
// neg (0,1) -> (normal,negacao)
void load_check(int et, int neg)
{
    int id = et % OFST;  // o id eh recuperado pegando o resto da divisao

    if (id == 0) return; // se for uma reducao exp, nao carrega nada, ja ta no acc
                         // nao tem id = 0 na tabela? isso pode dar M
                         // R: criei uma variavel NULL (com LOAD NULL) para ocupar a primeira posicao

    // prepara o tipo de acesso, caso seja array
    char srf[10];
    if ((v_isar[id] > 0) && (exec_fft_use == 1))
         strcpy(srf,"ISRF");
    else strcpy(srf,"SRF" );

    // se eh array, no acc ja tem o indice
    // entao coloca o indice na pilha e da LOAD
    if (v_isar[id] > 0)
    {
        if (using_macro == 0) fprintf(f_asm, "PUSH\n%s\nLOAD %s\n" , srf, v_name[id]);
    }
    else // se nao eh array, carrega a variavel
    {
        char num[64]; prepar_oper(num, id, et, neg);

        // se o acc nao tem nenhum resultado, carrega normalmente
        if (acc_ok == 0)
        {
            if (using_macro == 0) fprintf(f_asm, "LOAD %s\n" , num);
        }
        else // se acc carregado, empurra o valor pra pilha e carrega o novo
        {
            if (using_macro == 0) fprintf(f_asm, "PLD %s\n"  , num);
        }
    }

    acc_ok = 1;  // diz que o acc agora tem um valor carregado
}

// prepara o indice do array e carrega ele no acc
// flag diz se eh array invertido no uso (1) ou no set (2)
void array_1d_check(int id, int et, int flag)
{
    // tem que ver se eh array mesmo
    if (v_isar[id] == 0)
        fprintf (stderr, "Erro na linha %d: %s não é array não, abensoado!\n", line_num+1, rem_fname(v_name[id], fname));

    // tem que ver se eh array 1D mesmo
    if (v_isar[id] == 2)
        fprintf (stderr, "Erro na linha %d: array %s tem duas dimensões!\n"  , line_num+1, rem_fname(v_name[id], fname));

    // seta se eh array invertido
    if (flag == 1) exec_fft_use = 1; // array invertido no uso (depois do =)
    if (flag == 2) exec_fft_set = 1; // array invertido no set (antes  do =)

    // da load no argumento do array
    load_check(et,0);

    // testa se o indice do array eh tipo float
    // se for, converte pra inteiro, caso o proc seja ponto fixo
    // se for ponto flutuante, a conversao eh automatica no hardware
    if (get_type(et) == 2)
    {
        if (prtype == 0)
        {
            fprintf(stdout, "Atenção na linha %d: índice de array tem que ser do tipo int. Vou quebrar o teu galho.\n", line_num+1);

            if (using_macro == 0) fprintf(f_asm, "CALL   float2int\n");
            f2i = 1; // seta a variavel de estado que diz que usou a macro float2int
        }
    }

    // teste com numeros complexos --------------------------------------------
    if (v_type[id] > 2) array_1d_check_cmp(et);
    // final do teste ---------------------------------------------------------
}

// faz a checagem do indice do array para var complexas
void array_1d_check_cmp(int et)
{
    if (get_type(et) > 2)
        fprintf (stderr, "Erro na linha %d: usando número complexo em índice de array? tsts...\n", line_num+1);

    // salva o indice na variavel aux_cmpx, pra usar depois na parte complexa
    // esse incremento serve tanto para array no lado esquerdo, quanto para lado direito
    if (using_macro == 0) fprintf(f_asm, "SET aux_idx%d\n", ++a_cnt);
    acc_ok = 0; // libera o acumulador
}

// prepara o indice do array 2D e carrega ele no acc
// tb eh usada na atribuicao
// tem muito codigo que repete aqui e no array_1d_check
// tentar otimizar
void array_2d_check(int id, int et1, int et2)
{
    // tem que ver se eh array 2D mesmo
    if (v_isar[id] == 0)
        fprintf (stderr, "Erro na linha %d: %s não é array não, abensoado!\n", line_num+1, rem_fname(v_name[id], fname));

    // tem que ver se nao eh array 1D
    if (v_isar[id] == 1)
        fprintf (stderr, "Erro na linha %d: array %s tem uma dimensão só!\n", line_num+1, rem_fname(v_name[id], fname));

    // se os dois indices ja estao na pilha, tem que tirar o indice 2 de la e salvar temporariamente
    if ((et1 % OFST == 0) && (et2 % OFST == 0))
    {
        int id  =  exec_id("aux_idj");
            et2 = get_type(et2)*OFST + id;

        v_used[id] = 1;
        v_asgn[id] = 1;

        if (using_macro == 0) fprintf(f_asm, "SETP %s\n", v_name[id]);
    }

    // da load no primeiro argumento do array
    load_check(et1,0);

    // testa se o indice do primeiro argumento eh tipo float
    // se for, converte pra inteiro, caso o proc seja ponto fixo
    // se for ponto flutuante, a conversao eh automatica no hardware
    if (get_type(et1) == 2)
    {
        if (prtype == 0)
        {
            fprintf(stdout, "Atenção na linha %d: primeiro índice do array tá dando float. Vou arredondar pra baixo.\n", line_num+1);

            if (using_macro == 0) fprintf(f_asm, "CALL float2int\n");
            f2i = 1; // seta a variavel de estado que diz que usou a macro float2int
        }
    }

    // multiplica pelo tamanho da primeira dimensao
    if (using_macro == 0) fprintf(f_asm, "MLT %s_arr_size\n", v_name[id]);

    // da load no segundo argumento do array
    load_check(et2,0);

    // testa se o indice do segundo argumento eh tipo float
    // se for, converte pra inteiro, caso o proc seja ponto fixo
    // se for ponto flutuante, a conversao eh automatica no hardware
    if (get_type(et1) == 2)
    {
        if (prtype == 0)
        {
            fprintf(stdout, "Atenção na linha %d: segundo índice do array tá dando float. Vou arredondar pra baixo.\n", line_num+1);

            if (using_macro == 0) fprintf(f_asm, "CALL   float2int\n");
            f2i = 1; // seta a variavel de estado que diz que usou a macro float2int
        }
    }

    // soma com a conta anterior
    if (using_macro == 0) fprintf(f_asm, "SADD\n");

    // teste com numeros complexos --------------------------------------------
    if (v_type[id] > 2) array_2d_check_cmp(et1,et2);
    // fim do teste -----------------------------------------------------------
}

// faz a checagem do indice do array 2d para var complexas
void array_2d_check_cmp(int et1, int et2)
{
    if ((get_type(et1) > 2) || (get_type(et2) > 2))
        fprintf (stderr, "Erro na linha %d: usando número complexo em índice de array? tsts...\n", line_num+1);

    // salva o indice na variavel aux_cmpx, pra usar depois na parte complexa
    // esse incremento serve tanto para array no lado esquerdo, quanto para lado direito
    if (using_macro == 0) fprintf(f_asm, "SET aux_idx%d\n", ++a_cnt);
    acc_ok = 0; // libera o acumulador
}

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
        fprintf (stderr, "Erro na linha %d: mané, declara a variável %s direito!\n", line_num+1, rem_fname(v_name[id], fname));

    // Testa se a variavel ja recebeu um valor
    if (v_asgn[id] == 0)
        fprintf (stdout, "Atenção na linha %d: como você quer usar %s se você nem deu um valor ainda?\n", line_num+1, rem_fname(v_name[id], fname));

    // Se for um array, esqueceram o indice
    if (v_isar[id] > 0)
        fprintf (stderr, "Erro na linha %d: cadê o índice de array da variável %s?\n", line_num+1, rem_fname(v_name[id], fname));

    v_used[id] = 1;

    return v_type[id]*OFST+id;
}

// reducao de array 1d pra exp
int array1d2exp(int id, int et, int fft)
{
    // testa se a variavel ja foi declarada
    if (v_type[id] == 0)
        fprintf (stderr, "Erro na linha %d: mané, declara a variável %s direito!\n", line_num+1, rem_fname(v_name[id], fname));

    // testa se a variavel ja recebeu um valor
    if (v_asgn[id] == 0)
        fprintf (stdout, "Atenção na linha %d: como você quer usar %s se você nem deu um valor ainda?\n", line_num+1, rem_fname(v_name[id], fname));

    // prepara o indice e coloca ele no acc
    array_1d_check(id,et,fft);

    v_used[id] = 1;

    // array ja eh uma operacao entre ID e exp dando outro exp
    // nao eh soh pegar um token terminal na memoria
    // entao tem q executar o LOAD
    load_check(v_type[id]*OFST+id,0); // ver comentario em load_check
                                      // segundo parametro: array eh definido como variavel sem sinal

    // testes com numeros complexos -------------------------------------------
    if (v_type[id] > 2)
    {
        int idi = get_img_id(id);
        // pega o indice que foi armazenado em aux_cmp (no array_1d_check acima)
        // esse decremento em a_cnt eh para array do lado direito
        if (using_macro == 0) fprintf(f_asm, "PLD aux_idx%d\n", a_cnt--);

        // da load na parte imaginaria
        load_check(v_type[id]*OFST+idi,0);
    }
    // fim do teste -----------------------------------------------------------

    exec_fft_use = 0;                 // desativa indice invertido no uso (caso tenha sido usado)

    return v_type[id]*OFST;           // array ja eh executado e gera id extendido de reducao exp
}

// da pra juntar com o de cima
// soh uma linha eh diferente
int array2d2exp(int id, int et1, int et2)
{
    // testa se a variavel ja foi declarada
    if (v_type[id] == 0)
        fprintf (stderr, "Erro na linha %d: mané, declara a variável %s direito!\n", line_num+1, rem_fname(v_name[id], fname));

    // testa se a variavel ja recebeu um valor
    if (v_asgn[id] == 0)
        fprintf (stdout, "Atenção na linha %d: como você quer usar %s se você nem deu um valor ainda?\n", line_num+1, rem_fname(v_name[id], fname));

    // prepara o indice e coloca ele no acc
    array_2d_check(id,et1,et2);

    v_used[id] = 1;

    // array ja eh uma operacao entre ID e exp dando outro exp
    // nao eh soh pegar um token terminal na memoria
    // entao tem q executar
    load_check(v_type[id]*OFST+id,0); // ver comentario em load_check
                                      // segundo parametro: array eh definido como variavel sem sinal

    // testes com numeros complexos -------------------------------------------
    if (v_type[id] > 2)
    {
        int idi = get_img_id(id);
        // pega o indice que foi armazenado em aux_cmp (no array_2d_check acima)
        // esse decremento em a_cnt eh para array do lado direito
        if (using_macro == 0) fprintf(f_asm, "PLD aux_idx%d\n", a_cnt--);

        // da load na parte imaginaria
        load_check(v_type[id]*OFST+idi,0);
    }
    // fim do teste -----------------------------------------------------------

    return v_type[id]*OFST;           // array ja eh executado e gera id extendido de reducao exp
}

// reducao de ++ pra exp
int exp_pplus(int id)
{
    // testes com numeros complexos -------------------------------------------
    if (v_type[id] > 2)
        fprintf (stderr, "Erro na linha %d: o que você bebeu pra querer incrementar um número complexo?\n", line_num+1);
    // fim do teste -----------------------------------------------------------

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
    int ret = oper_ari(et,et1,2);

    // por ultimo, atribui de volta pra id
    var_set(id, ret, v_isar[id],0,1,0);

    acc_ok = 1; //nao pode liberar o acc, pois eh um exp

    return ret;
}

// reducao de ++ pra exp em array 1D
int array_pplus(int id, int ete)
{
    // testes com numeros complexos -------------------------------------------
    if (v_type[id] > 2)
        fprintf (stderr, "Erro na linha %d: o que você bebeu pra querer incrementar um número complexo?\n", line_num+1);
    // fim do teste -----------------------------------------------------------

    // equivalente a pegar o x na expressao (x+1)
    int et = array1d2exp(id,ete,0);

    // agora transforma o 1 em um exp
    // primeiro faz o lexer do 1
    if (find_var("1") == -1) add_var("1");
    int lval = find_var("1");
    // pega se deve vir de INUM ou FNUM
    int type = get_type(et);
    // depois o parser
    int et1 = num2exp(lval,type);

    // depois faz operacao de soma
    int ret = oper_ari(et,et1,2);

    // faz o load no indice do array novamente
    array_1d_check(id, ete, 0);
    // por ultimo, atribui de volta pra id
    var_set(id, ret, v_isar[id],0,1,0);

    acc_ok = 1; //nao pode liberar o acc, pois eh um exp

    return ret;
}

// reducao de ++ pra exp em array 2D
int array_2plus(int id, int et1, int et2)
{
    // testes com numeros complexos -------------------------------------------
    if (v_type[id] > 2)
        fprintf (stderr, "Erro na linha %d: o que você bebeu pra querer incrementar um número complexo?\n", line_num+1);
    // fim do teste -----------------------------------------------------------

    // equivalente a pegar o x na expressao (x+1)
    int et = array2d2exp(id,et1,et2);

    // agora transforma o 1 em um exp
    // primeiro faz o lexer do 1
    if (find_var("1") == -1) add_var("1");
    int lval = find_var("1");
    // pega se deve vir de INUM ou FNUM
    int type = get_type(et);
    // depois o parser
    int etx = num2exp(lval,type);

    // depois faz operacao de soma
    int ret = oper_ari(et,etx,2);

    // faz o load no indice do array novamente
    array_2d_check(id, et1, et2);
    // por ultimo, atribui de volta pra id
    var_set(id, ret, v_isar[id],0,1,0);

    acc_ok = 1; //nao pode liberar o acc, pois eh um exp

    return ret;
}
