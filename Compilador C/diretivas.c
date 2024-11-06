#include "diretivas.h"
#include "variaveis.h"
#include "t2t.h"
#include <string.h>
#include <stdlib.h>

// escreve as diretivas de compilacao no arquivo asm
void exec_diretivas(char *dir, int id, int t)
{
    if (using_macro == 0) fprintf(f_asm, "%s %s\n", dir, v_name[id]);

    int ival = atoi(v_name[id]);

    // acao a tomar dependendo da diretiva
    switch(t)
    {
        case 1: prtype = ival; break;
        case 2: nbmant = ival; break;
        case 3: nbexpo = ival; break;
        case 4: if (ival < 1) fprintf(stderr, "Erro na linha %d: pra que voc� quer um processador sem entrada de dados?\n", line_num+1); break;
        case 5: if (ival < 1) fprintf(stderr, "Erro na linha %d: pra que voc� quer um processador sem sa�da de dados?\n"  , line_num+1); break;
    }
}

// nao deixa o parser escrever no arquivo assembler
// ao inves disso, copia o codigo de uma macro
void use_macro(char *f_name)
{
    if (using_macro == 1)
        fprintf(stderr, "Erro na linha %d: T� chamando uma macro dentro da outra. Voc� � uma pessoa confusa!\n", line_num+1);

    // remover as aspas da sitring (trabalho da porra!)
    char file_name[512];
    int  tamanho = strlen(f_name); // tamanho da string
    int idxToDel = tamanho-1;      // indice para deletar, nesse caso o ultimo, as aspas.
    strcpy ( file_name, "");
    memmove(&f_name[idxToDel], &f_name[idxToDel+1], 1); // deletando de fato as ultimas aspas
    strcat ( file_name, f_name+1); // agora copia, tirando as primeiras aspas

    // copia o codigo do arquivo asm
    FILE *f_macro;
    char a;
        f_macro  =    fopen  (file_name, "r");
    if (f_macro == 0) fprintf(stderr, "Aten��o na linha %d: Cad� a macro %s? Tinha que estar na pasta do projeto do SAPHO!\n", line_num+1, file_name);
	do {      a  =    fgetc  (f_macro); if (a != EOF) fputc(a,f_asm);} while (a != EOF);
	                  fclose (f_macro);

    using_macro = 1;
}

// libera o parser pra salvar no arquivo assembler
void end_macro()
{
    if (using_macro == 0) fprintf(stderr, "Aten��o na linha %d: N�o estou achando o come�o da macro\n", line_num+1);
        using_macro  = 0;
}

// gera instrucao ITRAD
// ainda tenho q checar os lugares q nao podem ter isso
// ex: dentro de loop, dentro de switch case, pensar ...
// talvez um warning ja sirva
void use_inter()
{
    if (using_macro == 0) fprintf(f_asm, "ITRAD\n");
}
