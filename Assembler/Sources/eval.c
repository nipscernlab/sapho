#include "eval.h"
#include "t2t.h"
#include "variaveis.h"
#include "labels.h"
#include "veri_comp.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#define NBITS_OPC 6 // tem que mudar em verilog de acordo (em proc_fx.v e proc_fl.v)

FILE *f_data, *f_instr; // .mif das memorias de dado e instrucao

int state = 0;     // estado do compilador
int c_op;          // guarda opcode atual
int nbopr;         // num de bits de operando
int tam_var;       // auxilia no preenchimento de array em memoria

void eval_init(int prep)
{
    pp   = prep;   // se estou ou nao na fase de pre-processamento

    var_reset();   // reseta a contagem de simbolos

    if (pp)        // pp soh conta, nao faz os arquivos ainda
    {
        n_ins = 0;
        n_dat = 0;
    }
    else
    {
        isrf  = 0; // ainda nao usou fft

        // num de bits de endereco para o operando (depois do mnemonico)
        // depende de quem eh maior, mem de dado ou de instr
        // esse valor foi achado na fase de pp
        nbopr   = (n_ins > n_dat+ndstac) ? ceil(log2(n_ins)) : ceil(log2(n_dat+ndstac));

        // abre os arquivos .mif
        f_data  = fopen(get_dname(), "w");
        f_instr = fopen(get_iname(), "w");
    }
}

void add_instr(int opc, int opr)
{
    // se for fase de pp, soh conta o num de instrucoes
    if ( pp) n_ins++;

    // se nao, escreve a instrucao no .mif em binario
    if (!pp) fprintf(f_instr, "%s%s\n", itob(opc,NBITS_OPC), itob(opr,nbopr));
}

void add_data(int val)
{
    // se for pp, soh conta a quantidade de dados
    if ( pp) n_dat++;

    // se nao, escreve o valor no .mif
    if (!pp)
    {
        // pega o tamanho da palavra de dado
        int s = (float_point) ? nbmant + nbexpo + 1 : nbits;
        // escreve o dado em binario no .mif
        fprintf(f_data, "%s\n", itob(val,s));
    }
}

// achei um simbolo depois de um mnemonico
// se for novo, salva o simbolo na tabela
// se o simbolo for uma constante, converte seu valor para binario
// coloca o simbolo na memoria de dados
// por ultimo, coloca a instrucao na memoria
void operando(char *va, int is_const)
{
    if (find_var(va) == -1)
    {
        int val;
        if (float_point)
            val = (is_const) ? f2mf(va) : 0;
        else
            val = (is_const) ? atoi(va) : 0;

        add_var (va, val);
        add_data(    val);
    }

    add_instr(c_op, find_var(va));
}

// preenche um array na memoria com dados em arquivo
// f_name eh o nome do arquivo a ser lido
// tam eh o tamanho do arquivo
// na fase de pp soh conta as variaveis
void get_addr(char *f_name, int tam)
{
    FILE* filepointer;

    // primeiro pega o caminho completo e abre o arquivo ----------------------

    char addr_tab[2048];
    if(pp == 0)
    {
        int tamanho = strlen(f_name); // tamanho da string do nome do arquivo
        int idxToDel = tamanho-1;     // indice para deletar, nesse caso o ultimo, as aspas.
        strcpy(addr_tab, "");
        memmove(&f_name[idxToDel], &f_name[idxToDel +1], 1); // deletando de fato o indice
        strcat(addr_tab, d_name);
        strcat(addr_tab, f_name);

        filepointer = fopen(addr_tab, "r");
        if (filepointer == NULL)
        fprintf(stderr, "Erro: Não rolou de abrir/achar o arquivo %s!!\n", addr_tab);
    }

    // agora le o arquivo -----------------------------------------------------

    int  i,val;
    char linha[512];

    for (i = 0; i < tam ; i++)
    {
        if(pp == 0)
        {
            fgets(linha, sizeof(linha), filepointer);

            if(float_point)
                val = f2mf(linha);
            else
                val = atoi(linha);

            add_data(val);
        }
        else
           add_data(0); // isso aqui eh soh pra contar as variaveis
    }

    if (pp == 0) fclose(filepointer);

    return;
}

// adiciona array na memoria de dados
// se for array normal, completa com zero
// se for array inicializado, chama get_addr para preencher
// va eh o tamanho do array
void array_size(int va, char *f_name)
{
    // incrementa o tamanho da memoria de acordo
    inc_vcont(va-1);

    if (strcmp(f_name, "") == 0)
        for (int i = 0; i < va; i++) add_data(0);
    else
        get_addr(f_name, va);
}

void eval_direct(int next_state)
{
    state = next_state;
}

void eval_opcode(int op, int next_state)
{
    c_op  = op;
    state = next_state;

    if (state == 0)                   // nao tem operando, ja pode escrever a instrucao
    {
        if (op == 55)                 // se for interrupcao, pegar o endereco atual durante o pp
        {
            if (pp) itr_addr = n_ins; // endereco da interrupcao
            add_instr(op, itr_addr);
        }
        else
            add_instr(op,0);
    }
}

void eval_opernd(char *va, int is_const)
{
    switch (state)
    {
        case  1: operando  (va, is_const);                       // operacoes com a ULA
                 state = 0;  break;
        case  2: add_instr (c_op, find_label(va));               // label
                 state = 0;  break;
        case  3: add_var   (va,0);                               // declarando array normal
                 state = 4;  break;
        case  4: array_size(atoi(va), "");                       // tamanho do array normal
                 state = 0;  break;
        case  5: if (pp) set_name(va);                           // nome do processador
                 state = 0;  break;
        case  6: if (pp) set_nbits (atoi(va));                   // numero de bits de dados
                 state = 0;  break;
        case  7: if (pp) set_nbmant(atoi(va));                   // numero de bits de mantissa
                 state = 0;  break;
        case  8: if (pp) set_nbexpo(atoi(va));                   // numero de bits do expoente
                 state = 0;  break;
        case  9: if (pp) set_ndstac(atoi(va));                   // tamanho da pilha de dados
                 state = 0;  break;
        case 10: if (pp) set_sdepth(atoi(va));                   // tamanho da pilha de instrucoes
                 state = 0;  break;
        case 11: if (pp) set_nuioin(atoi(va));                   // numero de enderecoes de io - entrada
                 state = 0;  break;
        case 12: if (pp) set_nuioou(atoi(va));                   // numero de enderecoes de io - saida
                 state = 0;  break;
        case 13: if (pp) set_float_point(atoi(va));              // 1 para ponto-flutuante e 0 para ponto-fixo
                 state = 0;  break;
        case 14: if (pp){va[strlen(va)-1] = 92; set_dir(va);}    // diretorio
                 state = 0;  break;
        case 15: if (pp)set_nugain(atoi(va));                    // valor da normalizacao
                 state = 0;  break;
        case 16: add_var(va,0);                                  // declarando array com arquivo
                 state = 17; break;
        case 17: tam_var = atoi(va);                             // pega o tamanho do array com arquivo
                 state = 18; break;
        case 18: array_size(tam_var,va);                         // preenche memoria com valor do arquivo (zero se nao tem arquivo)
                 state =  0; break;
        case 19: if (pp) set_fftsiz(atoi(va));                   // num de bits pra inverter na fft
                 state =  0; break;
    }

}

void eval_label(char *la)
{
    if (pp) add_label(la, n_ins);
}

void eval_finish()
{
    int i, aux;

    // completa memoria de instrucao ------------------------------------------
/*
    aux = n_ins;
    n_ins = (n_ins % (int)pow(2, logb(n_ins))) ? pow(2, logb(n_ins)+1) : pow(2, logb(n_ins));
    for (i=0; i<n_ins-aux; i++)
        fprintf(f_instr, "%s\n", itob(0,NBITS_OPC+nbopr));    // completando com potencia de 2
*/
    fclose(f_instr);

    // completa memoria de dados ----------------------------------------------

    int s = (float_point) ? nbmant + nbexpo + 1 : nbits;

    n_dat = ndstac+n_dat;
	for (i=0; i<ndstac; i++)
        fprintf(f_data, "%s\n", itob(0,s));     // completando com a pilha
/*
    aux = n_dat;
    n_dat = (n_dat % (int)pow(2, logb(n_dat))) ? pow(2, logb(n_dat)+1) : pow(2, logb(n_dat));
    for (i=0; i<n_dat-aux; i++)
        fprintf(f_data, "%s\n", itob(0,s));     // completando com potencia de 2
*/
    int fft_siz = 2*pow(2,fftsiz);
    if ((isrf == 1) && (fft_siz > n_dat))
    {
        aux = n_dat;
        n_dat = fft_siz;
        for (i=0; i<n_dat-aux; i++)
            fprintf(f_data, "%s\n", itob(0,s)); // completa com tamanho da fft (se necessario)
    }

    fclose(f_data);

    // checa integridade do tamanho do dado -----------------------------------

    if ((float_point == 0) && (nbits != nbmant+nbexpo+1))
        fprintf(stderr, "Erro: NUBITS (%d) tem que ser NBMANT (%d) + NBEXPO (%d) + 1!\n", nbits, nbmant, nbexpo);

    build_vfile();
}
