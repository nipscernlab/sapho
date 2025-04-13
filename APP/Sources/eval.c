// ----------------------------------------------------------------------------
// rotinas para uso no lexer --------------------------------------------------
// ----------------------------------------------------------------------------

// includes globais
#include  <stdio.h>
#include <string.h>
#include <stdlib.h>

// includes locais
#include "..\Headers\variaveis.h"

// ----------------------------------------------------------------------------
// declaracao de variaveis locais ---------------------------------------------
// ----------------------------------------------------------------------------

// variaveis auxiliares para lexer de arrays
int   tam_arr;      // tamanho do array
char name_arr[128]; // nome da variavel que esta sendo lida

// variaveis de estado
int  ndstac;        // tamanho da pilha de dados
int  n_ins = 0;     // numero de instrucoes
int  state = 0;     // estado do compilador

FILE *f_log;        // arquivo de log

// ----------------------------------------------------------------------------
// rotinas que atuam no lexer -------------------------------------------------
// ----------------------------------------------------------------------------

// executado antes do lexer
void eval_init(char *path)
{
    char    file[1001];
    sprintf(file, "%s/app_log.txt", path);

    f_log = fopen(file, "w");
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
    fprintf(f_log, "itr_addr %d\n", n_ins);
}

// executado quando um novo opcode eh encontrado
void eval_opcode(int next_state)
{
    // proximo estado depende do tipo de opcode:
    // 0 : nao tem operando
    // 17: operando eh endereco da memoria de daddos
    // 18: operando eh endereco da memoria de instrucao
    state = next_state;

    // nao tem operando, ja pode contar uma instrucao
    if (state == 0) n_ins++;
}

// executado quando um novo operando eh encontrado
void eval_opernd(char *va)
{
    switch (state)
    {
        case  1: fprintf(f_log, "prname %s\n", va ); state =  0; break; // nome do processador
        case  2: fprintf(f_log, "nubits %s\n", va ); state =  0; break; // num de bits da ula
        case  3: fprintf(f_log, "nbmant %s\n", va ); state =  0; break; // num de bits da mantissa
        case  4: fprintf(f_log, "nbexpo %s\n", va ); state =  0; break; // num de bits do expoente
        case  5:         ndstac   =       atoi(va ); state =  0; break; // tamanho da pilha de dados
        case 11: strcpy (name_arr,             va ); state = 12; break; // achou um array sem inicializacao
        case 12: var_add(name_arr,        atoi(va)); state =  0; break; // declara  array sem inicializacao
        case 13: strcpy (name_arr,             va ); state = 14; break; // achou um array com inicializacao
        case 14:                                     state = 15; break; // pega o tipo de dado (nao precisa no app)
        case 15:          tam_arr =       atoi(va ); state = 16; break; // pega o tamanho do array com arquivo
        case 16: var_add(name_arr,         tam_arr); state =  0; break; // preenche memoria com valor do arquivo
        case 17: var_add(va,1);             n_ins++; state =  0; break; // operacoes com a ULA
        case 18:                            n_ins++; state =  0; break; // operacoes de salto      
    }
}

// executado quando um novo label eh encontrado
void eval_label(char *va)
{
    fprintf(f_log, "%s %d\n", va, n_ins);
}

// executado depois do lexer
void eval_finish()
{
    fprintf(f_log, "n_ins %d\n", n_ins);
    fprintf(f_log, "n_dat %d\n", var_cnt()+ndstac);
    fclose (f_log);
}