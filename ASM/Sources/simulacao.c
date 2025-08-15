// ----------------------------------------------------------------------------
// rotinas para simulacao com o iverilog/gtkwave ------------------------------
// ----------------------------------------------------------------------------

// includes globais
#include  <stdio.h>
#include <string.h>

// includes locais
#include "..\Headers\eval.h"
#include "..\Headers\variaveis.h"

// ----------------------------------------------------------------------------
// variaveis locais -----------------------------------------------------------
// ----------------------------------------------------------------------------

// parametros de simulacao
int clk_frq;           // frequencia do clock de simulacao
int clk_num;           // num de clocks maximo a simular
int sim_typ;           // tipo de simulacao (um proc ou multicore)

// arquivo de traducao
FILE *f_tran;          // arquivo de traducao de opcode
int  sim_n_opc = 0;    // numero de instrucoes no arquivo de traducao
int  sim_fim;          // endereco do fim do programa

// variaveis declaradas pelo usuario
int  s_count  = 0;     // numero de variaveis registradas
char s_name[1000][64]; // nome da variavel encontrada
int  s_addr[1000];     // endereco da variavel encontrada
int  s_type[1000];     // tipo da variavel encontrada

// arrays declarados pelo usuario
int  s_count_arr  = 0;     // numero de arrays registrados
char s_name_arr[1000][64]; // nome do array encontrado
int  s_addr_arr[1000];     // endereco do array encontrado
int  s_type_arr[1000];     // tipo do array encontrado
int  s_size_arr[1000];     // tamanho do array encontrado

// ----------------------------------------------------------------------------
// funcoes auxiliares ---------------------------------------------------------
// ----------------------------------------------------------------------------

// funcao auxiliar que procura no arquivo cmm_log.txt se ...
// char *va aponta para uma variavel declarada no .cmm
// tambem retorna o tipo da variavel encontrada
int sim_is_var(char *va, int *tipo, int *is_global, char *nome)
{
    // abre o arquivo de log --------------------------------------------------

    char path[1024]; sprintf(path, "%s/cmm_log.txt", temp_dir);
    FILE *input = fopen(path, "r");
  
    // varre as linhas do arquivo procurando a variavel -----------------------

    char texto[1001];
    char funcao[128];
    char variav[128];

    int ok = 0;
    while(fgets(texto, 1001, input) != NULL)
    {
        // pega os 3 parametros da variavel -----------------------------------

        if (sscanf (texto, "%s %s %d", funcao, variav, tipo) != 3) continue;

        // monta o nome da variavel com os dados do arquivo de log ------------

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

        // se for a variavel desejada, monta o nome dela e sai do loop --------

        if (strcmp(nome,va) == 0)
        {
            sprintf(nome , "%s_v_%s", funcao, variav);
            ok = 1; break;
        }
    }
    
    fclose(input);

    return ok;
}

// funcao auxiliar que procura no arquivo cmm_log.txt se ...
// char *va aponta para uma array declarado no .cmm
// tambem retorna o tipo e tamanho do array encontrado
int sim_is_arr(char *va, int *tipo, int *size, int *is_global, char *nome)
{
    // abre o arquivo de log --------------------------------------------------

    char path[1024]; sprintf(path, "%s/cmm_log.txt", temp_dir);
    FILE *input = fopen(path, "r");
  
    // varre as linhas do arquivo procurando a variavel -----------------------

    char texto[1001];
    char funcao[128];
    char variav[128];

    int ok = 0;
    while(fgets(texto, 1001, input) != NULL)
    {
        // pega os 4 parametros do array --------------------------------------

        if (sscanf (texto, "%s %s %d %d", funcao, variav, tipo, size) != 4) continue;

        // monta o nome do array com os dados do arquivo de log ---------------

        // se for array global, nao coloca o nome da funcao
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

        // se for o arrayl desejada, monta o nome dele e sai do loop ----------

        if (strcmp(nome,va) == 0)
        {
            sprintf(nome , "%s_v_%s", funcao, variav);
            ok = 1; break;
        }
    }
    
    fclose(input);

    return ok;
}

// ----------------------------------------------------------------------------
// rotinas para acessar parametros da simulacao -------------------------------
// ----------------------------------------------------------------------------

int sim_clk    (){return clk_frq;} // pega frequencia do clock de simulacao
int sim_clk_num(){return clk_num;} // pega numero de clocks a simular
int sim_multi  (){return sim_typ;} // pega tipo de simulacao (um proc ou multicore)

// ----------------------------------------------------------------------------
// rotinas para acoes com o arquivo de traducao -------------------------------
// ----------------------------------------------------------------------------

// cria arquivo de traducao de opcode
// e inicializa variaveis de simulacao
void sim_init(int clk, int clk_n, int s_typ)
{
    // abre arquivo de traducao de opcode na pasta Temp
    char path[1024];
    sprintf(path, "%s/trad_opcode.txt", temp_dir);
    f_tran  = fopen(path, "w");

    clk_frq = clk;   // frequencia do clock  de simulacao
    clk_num = clk_n; // numero de clocks a simular
    sim_typ = s_typ; // tipo de simulacao (um proc ou multicore)
}

// adiciona opcode e operando no arquivo de traducao
void sim_add(char *opc, char *opr)
{
    fprintf(f_tran , "%d %s %s\n", sim_n_opc++, opc, opr);
}

void sim_set_fim(int fim){sim_fim  = fim;} // define  endereco de @fim
int  sim_get_fim(       ){return sim_fim;} // retorna endereco de @fim
void sim_finish (       ){fclose(f_tran);} // fecha arquivo de traducao

// ----------------------------------------------------------------------------
// rotinas para acoes com variaveis do usuario --------------------------------
// ----------------------------------------------------------------------------

// procura, no arquivo de log, se eh uma variavel declarada no codigo .cmm
// se sim, cadastra ela para mostrar no simulador
void sim_regi(char *va)
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
        s_addr[s_count] = var_cnt()-1;
        s_type[s_count] = tipo;
        s_count++;
    }
}

// procura, no arquivo cmm_log.txt, se eh um array declarado no codigo .cmm
// se sim, cadastra ele para mostrar no simulador
void sim_regi_arr(char *va)
{
    int  tipo, size;
    int  is_global;
    char var_name[128];

    if (sim_is_arr(va, &tipo, &size, &is_global, var_name))
    {
        if (is_global)
            sprintf(s_name_arr[s_count_arr], "arr_me%d_f_global_v_%s_e_", tipo, va);
        else
            sprintf(s_name_arr[s_count_arr], "arr_me%d_f_%s_e_", tipo, var_name);
        s_addr_arr[s_count_arr] = var_cnt()-1;
        s_type_arr[s_count_arr] = tipo;
        s_size_arr[s_count_arr] = size;
        s_count_arr++;
    }
}

char* sim_name    (int i){return s_name    [i];} // pega nome     da variavel
int   sim_addr    (int i){return s_addr    [i];} // pega endereco da variavel
int   sim_type    (int i){return s_type    [i];} // pega tipo     da variavel
int   sim_cont    (     ){return s_count      ;} // pega numero   de variaveis registradas

char* sim_name_arr(int i){return s_name_arr[i];} // pega nome     do array
int   sim_addr_arr(int i){return s_addr_arr[i];} // pega endereco do array
int   sim_type_arr(int i){return s_type_arr[i];} // pega tipo     do array
int   sim_cont_arr(     ){return s_count_arr  ;} // pega numero   de arrays registradas
int   sim_size_arr(int i){return s_size_arr[i];} // pega tamanho  do array