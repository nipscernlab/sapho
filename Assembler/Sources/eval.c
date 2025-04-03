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
    // se estou ou nao na fase de pre-processamento
    pp = prep;          
    // a tabela eh resetada nas duas fases (melhor, vai por mim!)
    var_reset();        
    // pp soh conta, nao faz os arquivos ainda
    if (!pp)
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
    // vai pro estado que pega o argumento especifico da diretiva
    state = next_state;
}

// executado quando acha a diretiva #ITRAD
void eval_itrad()
{
    // instrucao atual eh cadastrada como ponto de interrupcao
    if (pp) itr_addr = n_ins;
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

    if (pp)
    {   
        // nao tem operando, ja pode contar uma instrucao
        if (state == 0) n_ins++;
        // cadastra mnemonico
        mne_add(nome);
    }

    if (!pp) 
    {   
        // nao tem operando, ja pode escrever a instrucao
        if (state == 0) fprintf(f_instr, "%s%s\n", itob(op,NBITS_OPC), itob(0,nbopr));
    }
}

// cadastra instrucao com a ula para a fase pp
void instr_ula_pp(char *va, int is_const)
{
    // se for a primeira vez que a var aparece, faz o cadastro
    if (var_find(va) == -1)
    {
        var_add(va, is_const); // adiciona variavel na tabela (esta fazendo isso nas duas fases)
        n_dat++;               // adiciona uma variavel
        sim_reg(va);           // registra variavel no simulador
    }

    // finamente, adiciona uma nova instrucao
    n_ins++;
}

// cadastra instrucao com a ula
// adiciona a variavel na memoria de dados
// por ultimo, coloca a instrucao na memoria de instrucao
void instr_ula(char *va, int is_const)
{
    // se for a primeira vez que a var aparece, faz o cadastro
    if (var_find(va) == -1)
    {
        var_add(va, is_const);                                     // adiciona variavel na tabela
        fprintf(f_data, "%s\n", itob(v_val[var_find(va)],nubits)); // adiciona variavel na mem de dados
    }

    // escreve a nova instrucao
    fprintf(f_instr, "%s%s\n" , itob(c_op,NBITS_OPC), itob(var_find(va),nbopr));
    // cadastra, tambem, no tradutor da simulacao
    sim_add(opcd,va);
}

// cadastra instrucoes de salto
void instr_salto(char *va)
{
    // escreve a nova instrucao
    fprintf(f_instr, "%s%s\n" , itob(c_op,NBITS_OPC), itob(find_label(va),nbopr));
    // cadastra, tambem, no tradutor da simulacao
    sim_add(opcd,va);
}

void eval_opernd(char *va, int is_const)
{
    switch (state)
    {
        case  1: if (pp) instr_ula_pp(va,is_const); else instr_ula(va,is_const); // operacoes com a ULA
                 state = 0;  break;
        case  2: if (pp) n_ins++; else instr_salto(va);                          // operacoes de salto
                 state = 0;  break;
        case  3: var_add(va,0);                                                  // achou um array sem inicializacao
                 state = 4;  break;
        case  4: if (pp) add_array_pp(atoi(va),""); else add_array(atoi(va),""); // declara  array sem inicializacao
                 state = 0;  break;
        case  5: if (pp) set_name(va);                                           // nome do processador
                 state = 0;  break;
        case  6: if (pp) set_nbits (atoi(va));                                   // numero de bits de dados
                 state = 0;  break;
        case  7: if (pp) set_nbmant(atoi(va));                                   // numero de bits de mantissa
                 state = 0;  break;
        case  8: if (pp) set_nbexpo(atoi(va));                                   // numero de bits do expoente
                 state = 0;  break;
        case  9: if (pp) set_ndstac(atoi(va));                                   // tamanho da pilha de dados
                 state = 0;  break;
        case 10: if (pp) set_sdepth(atoi(va));                                   // tamanho da pilha de instrucoes
                 state = 0;  break;
        case 11: if (pp) set_nuioin(atoi(va));                                   // numero de enderecoes de entrada
                 state = 0;  break;
        case 12: if (pp) set_nuioou(atoi(va));                                   // numero de enderecoes de saida
                 state = 0;  break;
        case 15: if (pp) set_nugain(atoi(va));                                   // valor da normalizacao
                 state = 0;  break;
        case 16: var_add(va,0);                                                  // declarando array com arquivo
                 state = 17; break;
        case 17: fil_typ = atoi(va);                                             // pega o tipo de array
                 state = 18; break;
        case 18: tam_var = atoi(va);                                             // pega o tamanho do array com arquivo
                 state = 19; break;
        case 19: if (pp) add_array_pp(tam_var,va); else add_array(tam_var,va);   // preenche memoria com valor do arquivo (zero se nao tem arquivo)
                 state =  0; break;
        case 20: if (pp) set_fftsiz(atoi(va));                                   // num de bits pra inverter na fft
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
    if ((find_mne("ILI") != 1) && (fft_siz > n_dat))
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
