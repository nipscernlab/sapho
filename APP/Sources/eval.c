// ----------------------------------------------------------------------------
// rotinas para uso no lexer --------------------------------------------------
// ----------------------------------------------------------------------------

// includes globais
#include  <stdio.h>
#include <string.h>
#include <stdlib.h>

// includes locais
#include "..\Headers\variaveis.h"

// variaveis auxiliares para lexer de arrays
int  tam_arr;       // tamanho do array
char name_arr[128]; // nome da variavel que esta sendo lida

// variaveis de estado
int  ndstac;        // tamanho da pilha de dados
int  n_ins = 0;     // numero de instrucoes
int  state = 0;     // estado do compilador

FILE *f_log;        // arquivo de log

// executado antes do lexer
void eval_init(char *path)
{
    f_log = fopen(path, "w");
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
        case  1: fprintf(f_log, "prname %s\n", va); // nome do processador
                 state = 0;  break;
        case  2: fprintf(f_log, "nubits %s\n", va); // num de bits da ula
                 state = 0;  break;
        case  5: ndstac = atoi(va);                 // tamanho da pilha de dados
                 state = 0;  break;
        case 11: strcpy (name_arr,va);              // achou um array sem inicializacao
                 state = 12; break;
        case 12: var_add(name_arr, atoi(va));       // declara  array sem inicializacao
                 state = 0;  break;
        case 13: strcpy (name_arr,va);              // achou um array com inicializacao
                 state = 14; break;
        case 14: // soh passa pro 15                // pega o tipo de dado
                 state = 15; break;
        case 15: tam_arr = atoi(va);                // pega o tamanho do array com arquivo
                 state = 16; break;
        case 16: var_add(name_arr,tam_arr);         // preenche memoria com valor do arquivo
                 state =  0; break;
        case 17: var_add(va,1); n_ins++;            // operacoes com a ULA
                 state = 0;  break;
        case 18: n_ins++;                           // operacoes de salto
                 state = 0;  break;
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
    fprintf(f_log, "n_dat %d\n", ndstac+var_cnt());
    fclose (f_log);
}
