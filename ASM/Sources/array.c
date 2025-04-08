// ----------------------------------------------------------------------------
// tratamento de arrays em assembly -------------------------------------------
// ----------------------------------------------------------------------------

// includes globais
#include <string.h>
#include <stdlib.h>

// includes locais
#include "..\Headers\t2t.h"
#include "..\Headers\eval.h"
#include "..\Headers\array.h"
#include "..\Headers\variaveis.h"

// funcao auxiliar para remover aspas de uma string
void rem_aspas(char *str)
{
    int j = 0; char c;
    for (int i = 0; (c = str[i]) != '\0'; i++) if (c != '"') str[j++] = c;
    str[j] = '\0';
}

// funcao auxiliar para preencher array na memoria de dados
// usado com inicializacao de array (ex: int x[10] "nome do arquivo")
// f_name eh o nome do arquivo a ser lido
// tam eh o tamanho do arquivo
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

    for (int i = 0; i < tam ; i++)
    {
        // le linha por linha
        // o que fazer depende do tipo de dado
        fgets (linha, sizeof(linha), f_file);
        sscanf(linha, "%s %si",real,   imag);

        if (fil_typ == 1) val = atoi(linha);    // tipo int
        if (fil_typ == 2) val = f2mf(linha);    // tipo float
        if (fil_typ == 3) val = f2mf(real );    // tipo comp (parte real)
        if (fil_typ == 4) val = f2mf(imag );    // tipo comp (parte imag)

        fprintf(f_data, "%s\n", itob(val,nubits));
    }

    fclose(f_file);
}

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