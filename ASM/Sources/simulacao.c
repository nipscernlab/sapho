#include "..\Headers\simulacao.h"
#include "..\Headers\veri_comp.h"
#include "..\Headers\eval.h"
#include "..\Headers\variaveis.h"

#include <string.h>

int sim_v_cnt = 0;
int sim_n_opc     = 0;               // numero de instrucoes no arquivo de traducao
char v_namo[1000][64];    // nome da variavel encontrada
int  v_add [1000];        // endereco da variavel encontrada
int  v_tipo[1000];        // tipo da variavel encontrada
int  fim_addr;            // endereco do fim do programa

FILE *f_tran;

void sim_init()
{
    // abre arquivo de traducao de opcode na pasta Temp
    char path[1024];
    sprintf(path, "%s/trad_opcode.txt", temp_dir);
    f_tran  = fopen(path, "w");
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
            sprintf(v_namo[sim_v_cnt], "me%d_f_global_v_%s_e_", tipo, va);
        else
            sprintf(v_namo[sim_v_cnt], "me%d_f_%s_e_", tipo, var_name);
        v_add  [sim_v_cnt] = v_count-1;
        v_tipo [sim_v_cnt] = tipo;
        sim_v_cnt++;
    }
}

void sim_check_fim(char *la)
{
    // cadastra endereco da instrucao de fim do programa
    if (strcmp(la,"fim") == 0) fim_addr = n_ins;
}