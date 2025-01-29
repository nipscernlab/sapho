#include "..\Headers\diretivas.h"
#include "..\Headers\variaveis.h"
#include "..\Headers\t2t.h"
#include "..\Headers\funcoes.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>

// escreve as diretivas de compilacao no arquivo asm
void exec_diretivas(char *dir, int id, int t)
{
    if (using_macro == 0) fprintf(f_asm, "%s %s\n", dir, v_name[id]);

    int ival = atoi(v_name[id]);

    // acao a tomar dependendo da diretiva
    switch(t)
    {
        case 1: prtype = ival; break;
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
void use_macro(char *f_name, int global)
{
    if (using_macro == 1)
        fprintf(stderr, "Erro na linha %d: Tá chamando uma macro dentro da outra. Você é uma pessoa confusa!\n", line_num+1);

    // se for global, tem q ver se tem que chamar a funcao main ainda
    if ((mainok == 0) && (global == 1))
    {
        fprintf(f_asm, "CALL main\n@fim JMP fim\n");

        mainok = 2; // funcao main foi chamada no inicio
    }

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
    if (f_macro == 0) fprintf(stderr, "Erro na linha %d: Cadê a macro %s? Tinha que estar na pasta do projeto do SAPHO!\n", line_num+1, file_name);
	do {      a  =    fgetc  (f_macro); if (a != EOF) fputc(a,f_asm);} while (a != EOF);
	                  fclose (f_macro);

    using_macro = 1;
}

// libera o parser pra salvar no arquivo assembler
void end_macro()
{
    if (using_macro == 0) fprintf(stderr, "Erro na linha %d: Não estou achando o começo da macro\n", line_num+1);
        using_macro  = 0;
}

// converte o inteiro x para binario de comprimento w
// tentar mudar para conseguir converter int maior de 32 bits
char *itob(int x, int w)
{
	int z;
    char *b = (char *) malloc(w+1);
    b[0] = '\0';

	int s = (w > 31) ? 31 : w;
	if (w > 31)
    {
        for (z = 0; z < w- 31; z++)
            if (x < 0) strcat(b,"1");
            else       strcat(b,"0");
    }

    for (z = pow(2,s-1); z > 0; z >>= 1)
		strcat(b, ((x & z) == z) ? "1" : "0");

    return b;
}

int is_macro()
{
    num_ins++;
    if (using_macro == 0) fprintf(f_ltp, "%d %d\n", num_ins, line_num+1);

    return using_macro;
}

// gera diretiva #ITRAD
// ainda tenho q checar os lugares q nao podem ter isso
// ex: dentro de loop, dentro de switch case, pensar ...
// talvez um warning ja sirva
void use_inter()
{
    if (itr_ok      == 1) fprintf(stderr, "Erro na linha %d: já tem uma interrupção rolando em outro ponto antes desse!\n", line_num+1);
    if (using_macro == 0) fprintf( f_asm, "#ITRAD\n");

    itr_ok = 1;
}
