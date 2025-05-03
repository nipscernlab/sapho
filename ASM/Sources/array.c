// ----------------------------------------------------------------------------
// tratamento de arrays em assembly -------------------------------------------
// ----------------------------------------------------------------------------

// includes globais
#include <string.h>
#include <stdlib.h>
#include  <ctype.h>

// includes locais
#include "..\Headers\t2t.h"
#include "..\Headers\eval.h"
#include "..\Headers\array.h"
#include "..\Headers\variaveis.h"

// ----------------------------------------------------------------------------
// funcoes auxiliares ---------------------------------------------------------
// ----------------------------------------------------------------------------

// funcao auxiliar para remover aspas de uma string
void rem_aspas(char *str)
{
    int j = 0; char c;
    for (int i = 0; (c = str[i]) != '\0'; i++) if (c != '"') str[j++] = c;
    str[j] = '\0';
}

// funcao auxiliar para verificar se o conteudo da linha de um arquivo eh um int valido
int linha_e_inteiro(const char *linha)
{
    char *endptr;

    while ( isspace((unsigned char)*linha) ) linha++ ; // ignora espaços em branco no começo
    if    (*linha == '\0' || *linha == '\n') return 0; // se linha ta vazia, retorna com 0
    strtol( linha, &endptr, 10);                       // converte a string para long
    while ( isspace((unsigned char)*endptr)) endptr++; // verifica se o restante da string é só espaços ou '\n'
    return *endptr == '\0';                            // eh inteiro se não sobrou mais nada
}

// funcao auxiliar para verificar se uma linha representa um float valido
int linha_e_float(const char *linha)
{
    char *endptr;

    while ( isspace((unsigned char)*linha) ) linha++ ; // Ignora espaços em branco iniciais
    if    (*linha == '\0' || *linha == '\n') return 0; // linha vazia
    strtof( linha,  &endptr);                          // Tenta converter
    if    ( linha == endptr)                 return 0; // Nenhum caractere foi consumido
    while ( isspace((unsigned char)*endptr)) endptr++; // Pula espaços após o número
    return *endptr == '\0';                            // Deve ter terminado a string
}

// função auxiliar para verificar e extrair um float válido
int parse_float(const char *str, float *out_value, const char **out_end)
{
    char *endptr;

    float val = strtof(str, &endptr); // tenta converter
    if (str  == endptr)     return 0; // falha na conversão
    *out_value =    val;              // pega o valor convertido
    *out_end   = endptr;              // aponta para o proximo caractere da linha
                            return 1; // conversao ok
}

// funcao auxiliar para verificar se uma linha contém um comp valido
int linha_e_comp(const char *linha)
{
    const char *p = linha;
    float f1, f2;

    while (isspace((unsigned char)*p)) p++; // Ignora espaços
    if (!parse_float(p, &f1, &p)) return 0; // Primeiro float
    while (isspace((unsigned char)*p)) p++; // Ignora espaços
    if (!parse_float(p, &f2, &p)) return 0; // Segundo float
    if (*p != 'i')                return 0; // se o proximocaractere nao for a letra i, retorna
    p++;                                    // incrementa o ponteiro
    while (isspace((unsigned char)*p)) p++; // Ignora espaços
    return *p == '\0';                      // deve terminar aqui
}

// funcao auxiliar para preencher array na memoria de dados
// usado com inicializacao de array (ex: int x[10] "nome do arquivo")
// f_name  -> nome do arquivo a ser lido
// tam     -> tamanho do arquivo
// fil_typ -> tipo de dado
// f_data  -> arquivo da memoria de dados
void fill_mem(char *f_name, int tam, int fil_typ, FILE *f_data)
{
    // abre o arquivo para leitura -------------------------------------------

    rem_aspas(f_name);
    char path[2048];      sprintf(path  , "%s/Software/%s", proc_dir, f_name);
    FILE *f_file =          fopen(path  , "r");
    if   (f_file == NULL) fprintf(stderr, "Erro: não rolou de abrir o arquivo %s!!\n", path);
   
    // agora le o arquivo -----------------------------------------------------

    int  val;
    char linha[128];
    char real [64], imag[64];

    int i = 0;
    while (fgets (linha, sizeof(linha), f_file))
    {
        i++;

        // se for tipo int
        if (fil_typ == 1)
        {
            if (linha_e_inteiro(linha))
                val = atoi(linha);
            else
                fprintf(stderr, "Erro: a linha %d do arquivo %s não é um inteiro válido!\n", i, f_name); 
        }

        // se for tipo float
        if (fil_typ == 2)
        {
            if (linha_e_float(linha))
                val = f2mf(linha);
            else
                fprintf(stderr, "Erro: a linha %d do arquivo %s não é um float válido!\n", i, f_name); 
        }

        // se for a parte real de um comp
        if (fil_typ == 3)
        {
            if (linha_e_comp(linha))
            {
                sscanf(linha, "%s %si", real, imag);
                val = f2mf(real);
            }
            else
                fprintf(stderr, "Erro: a linha %d do arquivo %s não é um comp válido!\n", i, f_name); 
        }

        // se for a parte imaginaria de um comp
        if (fil_typ == 4)
        {
            if (linha_e_comp(linha))
            {
                sscanf(linha, "%s %si", real, imag);
                val = f2mf(imag);
            } 
        }

        // se tem mais dados do que o necessario, da um warning e sai
        if (i > tam)
        {
            fprintf(stdout, "Atenção: tá sobrando %d linhas no arquivo %s!\n", i-tam, f_name);
            break;
        }
        // senao, preenche a memoria com o novo valor
        else
            fprintf(f_data, "%s\n", itob(val,nubits));
    }

    // se tem menos dados do que o necessario, gera um erro
    if ((i < tam) && (fil_typ != 4))
        fprintf(stderr, "Erro: tá faltando %d linhas no arquivo %s!\n", tam-i, f_name);

    fclose(f_file);
}

// ----------------------------------------------------------------------------
// funcoes de manipulacao de arrays -------------------------------------------
// ----------------------------------------------------------------------------

// adiciona array na memoria de dados
// se for array normal (f_name = ""), completa com zero
// se for array inicializado, chama fill_mem para preencher
void arr_add(int size, int type, char *f_name, FILE *f_data)
{
    // incrementa o tamanho da memoria de acordo
    var_inc(size-1);
    // se nao tem arquivo, preenche com zero
    if (strcmp(f_name, "") == 0)
        for (int i = 0; i < size; i++) fprintf(f_data, "%s\n", itob(0,nubits));
    else
        fill_mem(f_name, size, type, f_data);
}