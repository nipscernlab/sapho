#include "..\Headers\simulacao.h"
#include "..\Headers\hdl.h"
#include "..\Headers\eval.h"
#include "..\Headers\variaveis.h"

#include  <stdio.h>
#include <string.h>

int s_count = 0;
int sim_n_opc     = 0;               // numero de instrucoes no arquivo de traducao
char s_name[1000][64];    // nome da variavel encontrada
int  v_addr [1000];        // endereco da variavel encontrada
int  s_type[1000];        // tipo da variavel encontrada
int  sim_fim;            // endereco do fim do programa
FILE *f_tran;
int clk_frq;          // frequencia do clock de simulacao
int clk_num;         // num de clocks maximo a simular
int sim_typ;         // tipo de simulacao (um proc ou multicore)

void sim_init(int clk, int clk_n, int s_typ)
{
    // abre arquivo de traducao de opcode na pasta Temp
    char path[1024];
    sprintf(path, "%s/trad_opcode.txt", temp_dir);
    f_tran  = fopen(path, "w");

    clk_frq = clk;        // frequencia do clock  de simulacao
    clk_num = clk_n;     // numero de clocks a simular
    sim_typ = s_typ;     // tipo de simulacao (um proc ou multicore)
}

void sim_add (char *opc, char *opr)
{
    fprintf(f_tran , "%d %s %s\n", sim_n_opc++, opc, opr);
}

// funcao auxiliar que procura no arquivo cmm_log.txt se ...
// char *va aponta para uma variavel declarada no .cmm
// tambem retorna o tipo da variavel encontrada
int sim_is_var(char *va, int *tipo, int *is_global, char *nome)
{
    char texto[1001];
    char funcao[128];
    char variav[128];

    // abre o arquivo de log
    char path[1024];
    sprintf(path, "%s/cmm_log.txt", temp_dir);
    FILE *input = fopen(path, "r");

    // pula as 3 primeiras linhas
    fgets(texto, 1001, input); // nome do processador
    fgets(texto, 1001, input); // numero de bits da mantissa
    fgets(texto, 1001, input); // numero de bits do expoente
  
    int ok = 0;
    // varre as linhas do arquivo
    while(fgets(texto, 1001, input) != NULL)
    {
        // secao de variaveis termina quando encontra um #
        if (strstr(texto, "#") != NULL) break;

        sscanf (texto, "%s %s %d", funcao, variav, tipo);

        // se for variavel global, nao coloca o nome da funcao
        if (strcmp(funcao,"global")==0)
        {
            *is_global = 1;
            sprintf(nome , "%s", variav);
        }
        else
        {
            *is_global = 0;
            sprintf(nome , "%s_%s", funcao, variav);
        }

        if (strcmp(nome,va) == 0)
        {
            sprintf(nome , "%s_v_%s", funcao, variav);
            ok = 1;
            break;
        }
    }
    
    fclose(input);

    return ok;
}

// procura, no arquivo de log, se eh uma variavel declarada no codigo .cmm
// se sim, cadastra ela para mostrar no simulador
void sim_reg(char *va)
{
    int  tipo;
    int  is_global;
    char var_name[128];

    if (sim_is_var(va, &tipo, &is_global, var_name))
    {
        if (is_global)
            sprintf(s_name[s_count], "me%d_f_global_v_%s_e_", tipo, va);
        else
            sprintf(s_name[s_count], "me%d_f_%s_e_", tipo, var_name);
        v_addr  [s_count] = var_cnt()-1;
        s_type [s_count] = tipo;
        s_count++;
    }
}

char* sim_name(int i)
{
    return s_name[i];
}

int sim_addr(int i)
{
    return v_addr[i];
}

int sim_type(int i)
{
    return s_type[i];
}

int sim_cnt()
{
    return s_count;
}

void sim_finish()
{
    fclose(f_tran);
}

void sim_set_fim(int fim)
{
    sim_fim = fim;
}

int sim_get_fim()
{
    return sim_fim;
}

int sim_clk()
{
    return clk_frq;
}

int sim_clk_num()
{
    return clk_num; // numero de clocks a simular
}

int sim_multi()
{
    return sim_typ; // tipo de simulacao (um proc ou multicore)
}