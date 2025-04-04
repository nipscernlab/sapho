// ----------------------------------------------------------------------------
// rotinas para calcular o tamanho das memorias ... ---------------------------
// a medida que o lex vai escaneando o .asm -----------------------------------
// ----------------------------------------------------------------------------

#include  <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "..\Headers\variaveis.h"

int  ndstac;        // tamanho da pilha de dados
int  var_tam;       // tamanho do array
char var_name[128]; // nome da variavel que esta sendo lida

int  n_ins = 0;     // numero de instrucoes
int  state = 0;     // estado do compilador

// executado quando uma diretiva eh encontrada
void eval_direct(int next_state)
{
    // vai pro estado que pega o argumento especifico da diretiva
    state = next_state;
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

void eval_opernd(char *va)
{
    switch (state)
    {
        case  5: ndstac = atoi(va);           // tamanho da pilha de dados
                 state = 0;  break;
        case 11: strcpy(var_name,va);         // achou um array sem inicializacao
                 state = 12; break;
        case 12: var_add(var_name, atoi(va)); // declara  array sem inicializacao
                 state = 0;  break;
        case 13: strcpy(var_name,va);         // achou um array com inicializacao
                 state = 14; break;
        case 14: // soh passa pro 15          // pega o tipo de dado
                 state = 15; break;
        case 15: var_tam = atoi(va);          // pega o tamanho do array com arquivo
                 state = 16; break;
        case 16: var_add(var_name,var_tam);   // preenche memoria com valor do arquivo
                 state =  0; break;
        case 17: var_add(va,1); n_ins++;      // operacoes com a ULA
                 state = 0;  break;
        case 18: n_ins++;                     // operacoes de salto
                 state = 0;  break;
    }
}

void eval_finish()
{
    printf("n_ins = %d, n_dat = %d\n", n_ins, ndstac+var_cnt());
}
