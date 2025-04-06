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
void eval_init()
{
    char aux[128];

    var_get("prname", prname);   //strcpy(prname,"proc_fft_teste");                  // nome do processador
    var_get("n_ins" ,    aux); n_ins  = atoi(aux); // numero de instrucoes adicionadas
    var_get("n_dat" ,    aux); n_dat  = atoi(aux); // numero de variaveis  adicionadas
    var_get("nubits",    aux); nubits = atoi(aux); // numero de variaveis  adicionadas

    if (var_get("itr_addr", aux) == 1) itr_addr = atoi(aux); // endereco de interrupcao
    else                               itr_addr = 0; // se nao tem, assume que eh zero

    lab_reg(); // registra labels no arquivo de log

    // num de bits de endereco para o operando (depois do mnemonico)
    // depende de quem eh maior, mem de dado ou de instr
    // esse valor foi achado na fase de pp
    nbopr = (n_ins > n_dat) ? ceil(log2(n_ins)) : ceil(log2(n_dat));
    
    // abre os arquivos .mif
    f_data  = fopen(get_dname(), "w");
    f_instr = fopen(get_iname(), "w");

    // inicializa rotinas pra simulacao com o iverilog
    sim_init();
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
    
}

// executado quando um novo opcode eh encontrado
void eval_opcode(int op, int next_state, char *text, char *nome)
{
    c_op  = op;          // cadastra opcode atual
    strcpy(opcd,text);   // guarda nome do opcode atual para arquivo de traducao

    // proximo estado depende do tipo de opcode:
    // 0: nao tem operando
    // 1: operando eh endereco da memoria de daddos
    // 2: operando eh endereco da memoria de instrucao
    state = next_state;

    // nao tem operando, ja pode escrever a instrucao
    if (state == 0)
    {
        fprintf(f_instr, "%s%s\n", itob(op,NBITS_OPC), itob(0,nbopr));
        sim_add(opcd,"");
    }
    // cadastra mnemonico
    mne_add(nome);
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
        sim_reg(va);                                               // registra variavel no simulador
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
        case  3: set_nbmant(atoi(va));                                   // numero de bits de mantissa
                 state = 0;  break;
        case  4: set_nbexpo(atoi(va));                                   // numero de bits do expoente
                 state = 0;  break;
        case  5: set_ndstac(atoi(va));                                   // tamanho da pilha de dados
                 state = 0;  break;
        case  6: set_sdepth(atoi(va));                                   // tamanho da pilha de instrucoes
                 state = 0;  break;
        case  7: set_nuioin(atoi(va));                                   // numero de enderecoes de entrada
                 state = 0;  break;
        case  8: set_nuioou(atoi(va));                                   // numero de enderecoes de saida
                 state = 0;  break;
        case  9: set_nugain(atoi(va));                                   // valor da normalizacao
                 state = 0;  break;
        case 10: set_fftsiz(atoi(va));                                   // num de bits pra inverter na fft
                 state =  0; break;
        case 11: var_add(va,0);                                                  // achou um array sem inicializacao
                 state = 12; break;
        case 12: add_array(atoi(va),""); // declara  array sem inicializacao
                 state = 0;  break;
        case 13: var_add(va,0);                                                  // achou um array com inicializacao
                 state = 14; break;
        case 14: fil_typ = atoi(va);                                             // pega o tipo de array
                 state = 15; break;
        case 15: tam_var = atoi(va);                                             // pega o tamanho do array com arquivo
                 state = 16; break;
        case 16: add_array(tam_var,va);   // preenche memoria com valor do arquivo (zero se nao tem arquivo)
                 state =  0; break;
        case 17: instr_ula(va,is_const); // operacoes com a ULA
                 state = 0;  break;
        case 18: instr_salto(va);                          // operacoes de salto
                 state = 0;  break;
    }
}

void eval_label(char *la)
{
    
}

void eval_finish()
{
    fclose(f_instr);

    // completa memoria de dados ----------------------------------------------

	for (int i=0; i<ndstac; i++)
        fprintf(f_data, "%s\n", itob(0,nubits));     // completando com a pilha

    fclose(f_data);

    // finaliza traducao ------------------------------------------------------

    fclose(f_tran);

    // gera arquivos ----------------------------------------------------------

    build_vv_file();  // arquivo verilog top level do processador   
    build_tb_file();  // arquivo de test bench
}