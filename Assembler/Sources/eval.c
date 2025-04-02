// ----------------------------------------------------------------------------
// rotinas para gerar os arquivos .mif das memorias ... -----------------------
// a medida que o lex vai escaneando o .asm -----------------------------------
// ----------------------------------------------------------------------------

// includes globais
#include   <math.h>
#include  <stdio.h>
#include <string.h>
#include <stdlib.h>

// includes locais
#include "..\Headers\t2t.h"
#include "..\Headers\eval.h"
#include "..\Headers\array.h"
#include "..\Headers\labels.h"
#include "..\Headers\variaveis.h"
#include "..\Headers\veri_comp.h"
#include "..\Headers\simulacao.h"
#include "..\Headers\mnemonicos.h"

// ----------------------------------------------------------------------------
// redeclaracao de variaveis globais ------------------------------------------
// ----------------------------------------------------------------------------

// variaveis de estado --------------------------------------------------------

int  pp       = 1;        // pre-processing
int  isrf     = 0;        // diz se achou uma instrucao pra fazer FFT
char opcd[64];            // guarda opcode atual

// informacoes atualizadas durante o pp ---------------------------------------

int  n_ins	  = 0;        // numero de instrucoes adicionadas
int  n_dat    = 0;        // numero de variaveis  adicionadas
int  itr_addr = 0;        // endereco de interrupcao

// guarda os valores das diretivas
char prname [128];        // nome do processador
int  nubits  = 23;        // tamanho da palavra da ula
int  nbmant  = 16;        // numero de bits da mantissa
int  nbexpo  =  6;        // numero de bits do expoente
int  ndstac  = 10;        // tamanho da pilha de dados
int  sdepth  = 10;        // tamanho da pilha de subrotinas
int  nuioin  =  1;        // numero de portas de entrada
int  nuioou  =  1;        // numero de portas de saida
int  nugain  = 64;        // constante de divisao
int  fftsiz   = 8;        // tamanho da fft (em bits)

// informacoes atualizadas depois do pp ---------------------------------------

int  nbopr;               // num de bits de operando

// ----------------------------------------------------------------------------
// variaveis locais -----------------------------------------------------------
// ----------------------------------------------------------------------------

#define NBITS_OPC 7     // tem que mudar no verilog de acordo (em proc.v)

FILE *f_data, *f_instr; // .mif das memorias de dado e instrucao

// variaveis de estado --------------------------------------------------------

int state = 0;          // estado do compilador
int c_op;               // guarda opcode atual

// executada antes de iniciar o lexer
void eval_init(int prep)
{
    pp = prep;          // se estou ou nao na fase de pre-processamento

    var_reset();        // reseta a contagem das variaveis

    if (!pp)            // pp soh conta, nao faz os arquivos ainda
    {
        // num de bits de endereco para o operando (depois do mnemonico)
        // depende de quem eh maior, mem de dado ou de instr
        // esse valor foi achado na fase de pp
        nbopr = (n_ins > n_dat+ndstac) ? ceil(log2(n_ins)) : ceil(log2(n_dat+ndstac));

        // abre os arquivos .mif
        f_data  = fopen(get_dname(), "w");
        f_instr = fopen(get_iname(), "w");

        // inicializa rotinas pra simulacao com o iverilog
        sim_init();
    }
}

// executado quando uma diretiva eh encontrada
void eval_direct(int next_state)
{
    state = next_state; // vai pro estado que pega o argumento especifico da diretiva
}

// executado quando acha a diretiva #ITRAD
void eval_itrad()
{
    if (pp) itr_addr = n_ins;
}

// essa eh a funcao que realmente escreve na mem de instrucao
void add_instr(int opc, int opr)
{
    // se for fase de pp, soh conta o num de instrucoes
    if ( pp)
    {
        n_ins++;

        // se vai usar inversao de bits (opc 3 e 7)
        // entao, precisa checar tamanho da memoria no final
        if ((opc == 3) || (opc == 7)) isrf = 1;
    }

    // se nao, escreve a instrucao no .mif em binario
    if (!pp) fprintf(f_instr, "%s%s\n" , itob(opc,NBITS_OPC), itob(opr,nbopr));
}

// essa eh a funcao que realmente escreve na mem de dados
void add_data(int val)
{
    // se for pp, soh conta a quantidade de dados
    // se nao, escreve o valor no .mif
    if (pp) n_dat++; else fprintf(f_data, "%s\n", itob(val,nubits));
}

// executado quando um novo opcode eh encontrado
void eval_opcode(int op, int next_state, char *text, char *nome)
{
    c_op  = op;          // cadastra opcode atual
    strcpy(opcd,text);   // guarda nome do opcode atual

    // proximo estado depende do tipo de opcode:
    // 0: nao tem operando
    // 1: operando eh endereco da memoria de daddos
    // 2: operando eh endereco da memoria de instrucao
    state = next_state;

    // nao tem operando, ja pode escrever a instrucao
    if (state == 0) add_instr(op,0);

    // cadastra mnemonico para alocar recurso em hardware
    mne_add(nome);
}

// cadastra instrucao com a ula
// se o operando for uma constante, converte seu valor para binario ...
// e coloca o simbolo na memoria de dados
// por ultimo, coloca a instrucao na memoria de instrucao
void instr_ula(char *va, int is_const)
{
    // se for a primeira vez que a var aparece, faz o cadastro
    if (var_find(va) == -1)
    {
        // transforma char *va pra int val
        int val;
        switch(is_const)
        {
            case 0: val = 0;        break; // nao eh constante
            case 1: val = atoi(va); break; // constante tipo int
            case 2: val = f2mf(va); break; // constante tipo float
        }

        var_add   (va, val); // adiciona variavel na tabela (esta fazendo isso nas duas fases pq?)
        add_data  (    val); // adiciona variavel na mem de dados
        if (pp) sim_reg(va); // registra variavel no simulador
    }

    // finamente, cadastra a nova instrucao
    add_instr(c_op, var_find(va));
    // cadastra, tambem, no tradutor da simulacao
    if (!pp) sim_add(opcd,va);
}

// cadastra instrucoes de salto
void instr_salto(char *va)
{
    add_instr(c_op, find_label(va));
    if (!pp) sim_add(opcd,va);
}

void eval_opernd(char *va, int is_const)
{
    switch (state)
    {
        case  1: instr_ula  (va, is_const);                       // operacoes com a ULA
                 state = 0;  break;
        case  2: instr_salto(va);                                 // operacoes de salto
                 state = 0;  break;
        case  3: var_add   (va,0);                               // achou um array sem inicializacao
                 state = 4;  break;
        case  4: add_array (atoi(va), "");                       // declara  array sem inicializacao
                 state = 0;  break;
        case  5: if (pp) set_name  (va);                         // nome do processador
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
      //case 13: era usado para setar float ou int
      //case 14: era usado para o nome do diretorio
        case 15: if (pp) set_nugain(atoi(va));                   // valor da normalizacao
                 state = 0;  break;
        case 16: var_add(va,0);                                  // declarando array com arquivo
                 state = 17; break;
        case 17: fil_typ = atoi(va);                             // pega o tipo de array
                 state = 18; break;
        case 18: tam_var = atoi(va);                             // pega o tamanho do array com arquivo
                 state = 19; break;
        case 19: add_array (tam_var,va);                         // preenche memoria com valor do arquivo (zero se nao tem arquivo)
                 state =  0; break;
        case 20: if (pp) set_fftsiz(atoi(va));                   // num de bits pra inverter na fft
                 state =  0; break;
    }

}

void eval_label(char *la)
{
    if (pp)
    {
        add_label(la, n_ins); // cadastra label

        sim_check_fim(la);
    }
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

    int s = nubits;

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

    if (nubits != nbmant+nbexpo+1)
        fprintf(stderr, "Erro: NUBITS (%d) tem que ser NBMANT (%d) + NBEXPO (%d) + 1!\n", nubits, nbmant, nbexpo);

    // gera arquivos ----------------------------------------------------------

    build_vv_file();  // arquivo verilog top level do processador   
    build_tb_file();  // arquivo de test bench

    // finaliza traducao ------------------------------------------------------

    fclose(f_tran);
}
