#include "..\Headers\array.h"
#include "..\Headers\veri_comp.h"
#include "..\Headers\eval.h"
#include "..\Headers\t2t.h"
#include "..\Headers\variaveis.h"

#include  <stdio.h>
#include  <string.h>
#include <stdlib.h>

int fil_typ;            // auxilia no preenchimento de array em memoria (tipo de dado)
int tam_var;            // auxilia no preenchimento de array em memoria (tamanho do array)

// funcao auxiliar para remover espacos em branco
void rem_space(char *text)
{
    int i = 0, j = 0;
    char temp[256];
    strcpy(temp, text);
    while (temp[i] != '\0')
    {
        while (temp[i] == ' ') i++;
        text[j] = temp[i];
        i++;
        j++;
    }
    text[j] = '\0';
}

// funcao auxiliar para preencher array na memoria de dados
// usado com inicializacao de array (ex: int x[10] "nome do arquivo")
// f_name eh o nome do arquivo a ser lido
// tam eh o tamanho do arquivo
void fill_mem1(char *f_name, int tam)
{

    int  i;

    for (i = 0; i < tam ; i++)
    {
        
        n_dat++;
    }
}

// funcao auxiliar para preencher array na memoria de dados
// usado com inicializacao de array (ex: int x[10] "nome do arquivo")
// f_name eh o nome do arquivo a ser lido
// tam eh o tamanho do arquivo
void fill_mem(char *f_name, int tam)
{
    FILE* filepointer = NULL;

    // primeiro pega o caminho completo e abre o arquivo ----------------------
    // mudar a sintaxe para nao precisar das aspas
    char addr_tab[2048];
   
        int tamanho = strlen(f_name); // tamanho da string do nome do arquivo
        int idxToDel = tamanho-1;     // indice para deletar, nesse caso o ultimo, as aspas.
        memmove(&f_name[idxToDel], &f_name[idxToDel +1], 1); // deletando de fato o indice
        sprintf(addr_tab, "%s/Software/%s", proc_dir, f_name);

        filepointer = fopen(addr_tab, "r");
        if (filepointer == NULL)
        fprintf(stderr, "Erro: Não rolou de abrir/achar o arquivo %s!!\n", addr_tab);
   

    // agora le o arquivo -----------------------------------------------------

    int  i,val = 0;
    char linha[512];

    for (i = 0; i < tam ; i++)
    {
        
            // le linha por linha
            // o que fazer depende ...
            // do tipo de proc e do tipo de dado
            fgets(linha, sizeof(linha), filepointer);

            // com int
            if (fil_typ == 1)
            {
                val = atoi(linha);
            }

            // com float
            if (fil_typ == 2)
            {
                val = f2mf(linha);
            }

            // com real comp
            if (fil_typ == 3)
            {
                char  num[64];
                float real,img;

                rem_space(linha);
                   sscanf(linha,"%f %f",&real,&img);

                   sprintf(num,"%f",real);
                val = f2mf(num);
            }

            // com imag comp
            if (fil_typ == 4)
            {
                char  num[64];
                float real,img;

                rem_space(linha);
                   sscanf(linha,"%f %f",&real,&img);

                   sprintf(num,"%f",img);
                val = f2mf(num);
            }

            fprintf(f_data, "%s\n", itob(val,nubits));
        
    }

    fclose(filepointer);
}

// adiciona array na memoria de dados
// para a fase de pre-processamento
// va eh o tamanho do array
void add_array_pp(int va, char *f_name)
{
    // incrementa o tamanho da memoria de acordo
    inc_vcont(va-1);
    // incrementa o num de variaveis de acordo
    n_dat += va;
}

// adiciona array na memoria de dados
// se for array normal, completa com zero
// se for array inicializado, chama fill_mem para preencher
// va eh o tamanho do array
void add_array(int va, char *f_name)
{
    // incrementa o tamanho da memoria de acordo
    inc_vcont(va-1);
    // se nao tem arquivo, preenche com zero
    if (strcmp(f_name, "") == 0)
        for (int i = 0; i < va; i++) fprintf(f_data, "%s\n", itob(0,nubits));
    else
        fill_mem(f_name, va);
}