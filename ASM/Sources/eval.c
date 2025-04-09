// ----------------------------------------------------------------------------
// rotinas para gerar os arquivos .mif das memorias ... -----------------------
// a medida que o lexer vai escaneando o .asm ---------------------------------
// ----------------------------------------------------------------------------

#define NBITS_OPC 7 // tem que mudar no verilog de acordo (em proc.v)

// includes globais
#include   <math.h>
#include  <stdio.h>
#include <string.h>
#include <stdlib.h>

// includes locais
#include "..\Headers\t2t.h"
#include "..\Headers\hdl.h"
#include "..\Headers\eval.h"
#include "..\Headers\array.h"
#include "..\Headers\labels.h"
#include "..\Headers\opcodes.h"
#include "..\Headers\variaveis.h"
#include "..\Headers\simulacao.h"

// ----------------------------------------------------------------------------
// redeclaracao de variaveis globais ------------------------------------------
// ----------------------------------------------------------------------------

// diretorios de acesso aos arquivos
char proc_dir[1024];    // diretorio do processador
char temp_dir[1024];    // diretorio da pasta Tmp
char  hdl_dir[1024];    // diretorio da pasta HDL

// guarda os valores das diretivas
char prname [128];      // nome do processador
int  nubits  = 23;      // tamanho da palavra da ula
int  nbmant  = 16;      // numero de bits da mantissa
int  nbexpo  =  6;      // numero de bits do expoente
int  ndstac  = 10;      // tamanho da pilha de dados
int  sdepth  = 10;      // tamanho da pilha de subrotinas
int  nuioin  =  1;      // numero de portas de entrada
int  nuioou  =  1;      // numero de portas de saida
int  nugain  = 64;      // constante de divisao
int  fftsiz   = 8;      // tamanho da fft (em bits)

// ----------------------------------------------------------------------------
// variaveis locais -----------------------------------------------------------
// ----------------------------------------------------------------------------

FILE *f_data, *f_instr; // .mif das memorias de dado e instrucao

// variaveis de estados
int  state = 0;         // guarda estado do compilador
char opc_name[64];      // guarda nome   do opcode atual
int  opc_idx;           // guarda indice do opcode atual
int  arr_typ;           // guarda tipo    de array
int  arr_tam;           // guarda tamanho do array

// variaveis auxiliares
int  n_ins	  = 0;      // numero de instrucoes adicionadas
int  n_dat    = 0;      // numero de variaveis  adicionadas
int  itr_addr = 0;      // endereco de interrupcao
int  nbopr;             // num de bits de operando

// ----------------------------------------------------------------------------
// funcoes auxiliares ---------------------------------------------------------
// ----------------------------------------------------------------------------

// pega parametros no arquivo de log
int eval_get(char *fname, char *var, char *val)
{
    // abre o arquivo de log
    char path[1024];
    sprintf(path, "%s/%s", temp_dir, fname);
    FILE *input = fopen(path, "r");
    
    char linha[1001];
    char nome [128 ];
    while (fgets (linha,sizeof(linha),input)) if (sscanf(linha,"%s %s", nome, val) == 2) if (strcmp(nome,var) == 0)
    {
        fclose(input);
        return 1; // encontrou a variavel
    }

    fclose(input);
    return 0; // nao encontrou a variavel
}

// cadastra instrucao com a ula
// adiciona a variavel na memoria de dados
// por ultimo, coloca a instrucao na memoria de instrucao
void instr_ula(char *va, int is_const)
{
    // se for a primeira vez que a var aparece, faz o cadastro
    if (var_find(va) == -1)
    {
        var_add(va, is_const);                             // adiciona variavel na tabela
        fprintf(f_data, "%s\n", itob(var_val(va),nubits)); // adiciona variavel na mem de dados
        sim_reg(va);                                       // registra variavel no simulador (se for do usuario)
    }

    // escreve a nova instrucao
    fprintf(f_instr, "%s%s\n" , itob(opc_idx,NBITS_OPC), itob(var_find(va),nbopr));
    // cadastra, tambem, no tradutor da simulacao
    sim_add(opc_name,va);
}

// cadastra instrucoes de salto
void instr_salto(char *va)
{
    // escreve a nova instrucao
    fprintf(f_instr, "%s%s\n" , itob(opc_idx,NBITS_OPC), itob(lab_find(va),nbopr));
    // cadastra, tambem, no tradutor da simulacao
    sim_add(opc_name,va);
}

// ----------------------------------------------------------------------------
// funcoes de evolucao do lexer -----------------------------------------------
// ----------------------------------------------------------------------------

// executado antes de iniciar o lexer
void eval_init(int clk, int clk_n, int s_typ)
{
    char aux[256];

    // pega parametros no arquivo app_log.txt ---------------------------------

    eval_get("app_log.txt","prname", prname);                     // nome do processador
    eval_get("app_log.txt","n_ins" ,    aux); n_ins  = atoi(aux); // numero de instrucoes adicionadas
    eval_get("app_log.txt","n_dat" ,    aux); n_dat  = atoi(aux); // numero de variaveis  adicionadas
    eval_get("app_log.txt","nubits",    aux); nubits = atoi(aux); // numero de bits da ULA

    if (eval_get("app_log.txt","itr_addr", aux) == 1) itr_addr = atoi(aux); // endereco de interrupcao

    lab_reg(); // registra labels do arquivo de log

    // executa a inicializacao ------------------------------------------------

    // num de bits de endereco para o operando (depois do mnemonico)
    // depende de quem eh maior, mem de dado ou de instr
    nbopr = (n_ins > n_dat) ? ceil(log2(n_ins)) : ceil(log2(n_dat));
    
    // abre os arquivos .mif
    sprintf(aux, "%s/Hardware/%s_data.mif", proc_dir, prname); f_data  = fopen(aux, "w");
    sprintf(aux, "%s/Hardware/%s_inst.mif", proc_dir, prname); f_instr = fopen(aux, "w");

    // inicializa rotinas pra simulacao com o iverilog
    sim_init(clk, clk_n, s_typ);
}

// executado quando uma diretiva eh encontrada
void eval_direct(int next_state)
{
    // vai pro estado que pega o argumento especifico da diretiva
    state = next_state;
}

// executado quando um novo opcode eh encontrado
void eval_opcode(int op, int next_state, char *text, char *nome)
{
    opc_idx = op;          // cadastra opcode atual
    strcpy(opc_name,text); // guarda nome do opcode atual para arquivo de traducao

    // proximo estado depende do tipo de opcode:
    // 0: nao tem operando
    // 1: operando eh endereco da memoria de daddos
    // 2: operando eh endereco da memoria de instrucao
    state = next_state;

    // nao tem operando, ja pode escrever a instrucao
    if (state == 0)
    {
        fprintf(f_instr, "%s%s\n", itob(op,NBITS_OPC), itob(0,nbopr));
        sim_add(opc_name,"");
    }
    // cadastra opcode
    opc_add(nome);
}

// executado quando um operando eh encontrado
void eval_opernd(char *va, int is_const)
{
    switch (state)
    {
        case  3: nbmant =  atoi(va);                   state =  0; break; // numero de bits de mantissa
        case  4: nbexpo =  atoi(va);                   state =  0; break; // numero de bits do expoente
        case  5: ndstac =  atoi(va);                   state =  0; break; // tamanho da pilha de dados
        case  6: sdepth =  atoi(va);                   state =  0; break; // tamanho da pilha de instrucoes
        case  7: nuioin =  atoi(va);                   state =  0; break; // numero de enderecoes de entrada
        case  8: nuioou =  atoi(va);                   state =  0; break; // numero de enderecoes de saida
        case  9: nugain =  atoi(va);                   state =  0; break; // valor da normalizacao
        case 10: fftsiz =  atoi(va);                   state =  0; break; // num de bits pra inverter na fft
        case 11: var_add       (va ,0);                state = 12; break; // achou um array sem inicializacao
        case 12: arr_add  (atoi(va),0     ,"",f_data); state =  0; break; // declara  array sem inicializacao
        case 13: var_add       (va ,0);                state = 14; break; // achou um array com inicializacao
        case 14: arr_typ = atoi(va);                   state = 15; break; // pega o tipo de array
        case 15: arr_tam = atoi(va);                   state = 16; break; // pega o tamanho do array com arquivo
        case 16: arr_add  (arr_tam,arr_typ,va,f_data); state =  0; break; // preenche memoria com valor do arquivo (zero se nao tem arquivo)
        case 17: instr_ula     (va,is_const);          state =  0; break; // operacoes com a ULA
        case 18: instr_salto   (va);                   state =  0; break; // operacoes de salto
    }
}

// executado depois do lexer
void eval_finish()
{
    // completa memoria de dados com a pilha
	for (int i=0; i<ndstac; i++) fprintf(f_data, "%s\n", itob(0,nubits));

    // ja pode fechar os arquivos .mif
    fclose(f_instr);
    fclose(f_data );

    // finaliza simulacao
    sim_finish();

    // gera arquivos hdl
    hdl_vv_file(n_ins,n_dat,nbopr,itr_addr); // arquivo verilog top level do processador   
    hdl_tb_file();                           // arquivo verilog de test bench
}