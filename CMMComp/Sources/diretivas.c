#include "..\Headers\diretivas.h"
#include "..\Headers\variaveis.h"
#include "..\Headers\global.h"
#include "..\Headers\t2t.h"

#include <string.h>
#include <stdlib.h>

// ----------------------------------------------------------------------------
// redeclaracao de variaveis globais ------------------------------------------
// ----------------------------------------------------------------------------

char pr_name[128]; // nome do processador
int  nbmant  = 16; // numero de bits de mantissa
int  nbexpo  =  6; // numero de bits de expoente

FILE *f_lin;       // arquivo de linhas do programa em cmm
int  itr_ok  =  0; // se ja usou ou nao interrupcao
int  num_ins =  0; // numero de instrucoes do parse
int  mainok  =  0; // status da funcao main: 0 -> nao usada, 1 -> declarada, 2 -> chamada no inicio

// ----------------------------------------------------------------------------
// Controle de diretivas ------------------------------------------------------
// ----------------------------------------------------------------------------

// escreve as diretivas de compilacao no arquivo asm
void exec_dire(char *dir, int id, int t)
{
    add_sinst(0, "%s %s\n", dir, v_name[id]);

    int ival = atoi(v_name[id]);

    // acao a tomar dependendo da diretiva
    switch(t)
    {
        case 2: nbmant = ival; fprintf(f_log, "%d\n", nbmant); break;
        case 3: nbexpo = ival; fprintf(f_log, "%d\n", nbexpo); break;
        // rever essa questao do num de i/o
        case 4: if (ival < 1) fprintf(stderr, "Erro na linha %d: pra que você quer um processador sem entrada de dados?\n", line_num+1); break;
        case 5: if (ival < 1) fprintf(stderr, "Erro na linha %d: pra que você quer um processador sem saída de dados?\n"  , line_num+1); break;
        case 6: strcpy (pr_name,v_name[id]); fprintf(f_log, "%s\n",v_name[id]); break;
    }
}

// nao deixa o parser escrever no arquivo assembler
// ao inves disso, copia o codigo de uma macro
void use_macro(int ids, int global, int id_num)
{
    if (using_macro == 1)
        fprintf(stderr, "Erro na linha %d: tá chamando uma macro dentro da outra. você é uma pessoa confusa!\n", line_num+1);

    // se for global, tem q ver se tem que chamar a funcao main ainda ---------

    if ((mainok == 0) && (global == 1))
    {
        add_sinst(-2, "CAL main\n");
        add_sinst(-3, "@fim JMP fim\n");

        mainok = 2; // funcao main foi chamada no inicio
    }

    // remover as aspas da string (trabalho da porra!) ------------------------

    char f_name[64];
    strcpy(f_name, v_name[ids]);
    char file_name[512];
    int  tamanho = strlen(f_name); // tamanho da string
    int idxToDel = tamanho-1;      // indice para deletar, nesse caso o ultimo, as aspas.
    strcpy ( file_name, "");
    memmove(&f_name[idxToDel], &f_name[idxToDel+1], 1); // deletando de fato as ultimas aspas
    strcat ( file_name, f_name+1); // agora copia, tirando as primeiras aspas

    char    mac_name[1024];
    sprintf(mac_name, "%s/%s", dir_soft, file_name);

    // copia o codigo do arquivo asm ------------------------------------------

    FILE *f_macro;
    char a;
        f_macro  =    fopen  (mac_name, "r");
    if (f_macro == 0) fprintf(stderr, "Erro na linha %d: cadê a macro %s? Tinha que estar na pasta Software!\n", line_num+1, file_name);
	do {      a  =    fgetc  (f_macro); if (a != EOF) fputc(a,f_asm);} while (a != EOF);
                      fputc  ('\n',f_asm);
	                  fclose (f_macro);

    // preenche tabela de codigo cmm com -1 (INTERNO) -------------------------
    
    int n = atoi(v_name[id_num]);
    for (int i = 0; i < n; i++)
    {
        num_ins++;
        fprintf(f_lin, "%s\n", itob(-4,20));
    }

    using_macro = 1;
}

// libera o parser pra salvar no arquivo assembler
void end_macro()
{
    if (using_macro == 0) fprintf(stderr, "Erro na linha %d: não estou achando o começo da macro\n", line_num+1);
        using_macro  = 0;
}

// gera diretiva #ITRAD
// ainda tenho q checar os lugares q nao podem ter isso
// ex: dentro de loop, dentro de switch case, pensar ...
// talvez um warning ja sirva
void use_inter()
{
    if (itr_ok == 1) fprintf(stderr, "Erro na linha %d: já tem uma interrupção rolando em outro ponto antes desse!\n", line_num+1);
    add_sinst(0, "#ITRAD\n");
    itr_ok = 1;
}